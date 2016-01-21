/* 
 * ipc.c
 *
 * Copyright: University of Utah
 */

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <lcd-domains/types.h>
#include "internal.h"

/* CREATE -------------------------------------------------- */

int __lcd_create_sync_endpoint(struct lcd *caller, cptr_t slot)
{
	struct lcd_sync_endpoint *e;
	int ret;
	/*
	 * Alloc
	 */
	e = kmalloc(sizeof(*e), GFP_KERNEL);
	if (!e) {
		LCD_ERR("no mem");
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Set up send/recv queues
	 */
	INIT_LIST_HEAD(&e->senders);
	INIT_LIST_HEAD(&e->receivers);
	/*
	 * Set up lock
	 */
	mutex_init(&e->lock);
	/*
	 * Insert into caller's cspace
	 */
	ret = cap_insert(&caller->cspace, slot, e,
			__lcd_get_libcap_type(
				LCD_MICROKERNEL_TYPE_ID_SYNC_EP));
	if (ret) {
		LCD_ERR("insert");
		goto fail2;
	}

	return 0;

fail2:
	kfree(e);
fail1:
	return ret;
}

/* Destroy is trivial, and happens in the delete callback for the
 * sync endpoint capability type. */

/* LOOKUP -------------------------------------------------- */

static int lookup_ep(struct cspace *cspace, cptr_t slot, struct cnode **cnode)
{
	int ret;
	cap_type_t t;
	/*
	 * Look up
	 */
	ret = cap_cnode_get(cspace, slot, cnode);
	if (ret)
		goto fail1;
	/*
	 * Confirm it's an lcd or klcd
	 */
	t = cap_cnode_type(*cnode);
	if (t != __lcd_get_libcap_type(LCD_MICROKERNEL_TYPE_ID_SYNC_EP)) {
		LCD_ERR("not a sync ipc endpoint");
		goto fail2;
	}

	return 0;

fail2:
	cap_cnode_put(*cnode);
fail1:
	return ret;
}

int __lcd_get_sync_endpoint(struct lcd *caller, cptr_t sync_ep_cptr,
			struct cnode **cnode, 
			struct lcd_sync_endpoint **e)
{
	int ret;
	/*
	 * Look up and lock cnode containing endpoint
	 */
	ret = lookup_ep(cspace, endpoint, cnode);
	if (ret)
		goto fail1;
	*e = cap_cnode_object(cnode);
	/*
	 * Lock endpoint struct
	 */
	ret = mutex_lock_interruptible(&(*e)->lock);
	if (ret) {
		LCD_ERR("interrupted");
		goto fail2;
	}
	
	return 0; /* caller should match with __lcd_put_sync_endpoint */

fail2:
	cap_cnode_put(*cnode);
fail1:
	return ret;
}

void __lcd_put_sync_endpoint(struct lcd *caller, struct cnode *cnode, 
			struct lcd_sync_endpoint *e)
{
	/*
	 * Unlock endpoint
	 */
	mutex_unlock(&ep->lock);
	/*
	 * Release cnode
	 */
	cap_cnode_put(cnode);
}

/* SEND/RECV/CALL/REPLY -------------------------------------------------- */

static void copy_msg_regs(struct lcd *sender, struct lcd *receiver)
{
	memcpy(receiver->utcb->mr, sender->utcb->mr,
		sizeof(receiver->utcb->mr));
}

static void copy_msg_cap(struct lcd *sender, struct lcd *receiver,
			cptr_t from_ptr, cptr_t to_ptr)
{
	int ret;

	ret = cap_grant(sender->cspace, from_ptr,
			receiver->cspace, to_ptr);
	if (ret) {
		LCD_DEBUG(LCD_DEBUG_ERR,
			"failed to transfer cap @ 0x%lx in lcd %p to slot @ 0x%lx in lcd %p",
			cptr_val(from_ptr), sender, cptr_val(to_ptr), 
			receiver);
	}
}

static void copy_msg_caps(struct lcd *sender, struct lcd *receiver)
{
	cptr_t from, to;
	int i;
	for (i = 0; i < LCD_NUM_REGS; i++) {
		/*
		 * Grant on non-null cptrs
		 */
		from = sender->utcb->cr[i];
		to = receiver->utcb->cr[i];
		if (!cptr_is_null(from) && !cptr_is_null(to)) {
			copy_msg_cap(sender, receiver, from, to);
		}
	}
}

static void delete_reply_endpoint(struct lcd *sender)
{
	cap_delete(sender->cspace, LCD_CPTR_REPLY_ENDPOINT);
}

static void copy_call_endpoint(struct lcd *sender, struct lcd *receiver)
{
	int ret;
	ret = cap_grant(sender->cspace, LCD_CPTR_CALL_ENDPOINT,
			receiver->cspace, LCD_CPTR_REPLY_ENDPOINT);
	if (ret)
		LCD_DEBUG(LCD_DEBUG_ERR,
			"error granting call endpoint cap");
}

static int transmit_msg(struct lcd *sender, struct lcd *receiver,
			int making_call, int doing_reply)
{
	int ret = 0;
	/*
	 * See if either is dead
	 */
	if (lcd_status_dead(sender) || lcd_status_dead(receiver)) {
		LCD_ERR("dead lcd");
		return -EINVAL;
	}
	/*
	 * Copy general registers
	 */
	copy_msg_regs(sender, receiver);
	/*
	 * Transfer capabilities
	 */
	copy_msg_caps(sender, receiver);
	/*
	 * If sender is making call, grant capability to sender's call
	 * endpoint to receiver
	 */
	if (making_call)
		copy_call_endpoint(sender, receiver);
	/*
	 * If sender is doing reply, delete capability to reply endpoint
	 */
	if (doing_reply)
		delete_reply_endpoint(sender);
	return ret;
}

static int wait_for_transmit(struct lcd *lcd, struct cnode *ep_cnode,
			struct lcd_sync_endpoint *ep)
{
	int ret;
	/*
	 * Release endpoint e so others can use it
	 */
	__lcd_put_sync_endpoint(lcd, ep_cnode, ep);
	/*
	 * Loop until the condition is true, or we get a signal (interrupted)
	 *
	 * This is taken from the waitqueue implementation (wait.h) for
	 * wait_event_interruptible.
	 */
	for (;;) {
		/*
		 * Get ready to sleep 
		 */
		set_current_state(TASK_INTERRUPTIBLE);
		/*
		 * Check if transmit activity
		 */
		switch (atomic_read(&lcd->xmit_flag)) {
		case LCD_XMIT_SUCCESS:
			ret = 0;
			goto out;
		case LCD_XMIT_FAILED:
			LCD_DEBUG(LCD_DEBUG_ERR, "xmit failed");
			ret = -EIO;
			goto out;
		default:
			/* fall through */
			break;
		}
		/*
		 * Check if we're dead
		 *
		 * The current code (in do send/recv) doesn't take the
		 * lcd out of the queue.
		 *
		 * XXX: This needs a lot more attention actually when I start
		 * testing it ...
		 */
		if (lcd_status_dead(lcd)) {
			LCD_DEBUG(LCD_DEBUG_ERR, 
				"lcd %p died inside xmit loop", lcd);
			ret = -EIO;
			goto out;
		}
		/*
		 * Check if we're signaled
		 */
		if (!signal_pending(current)) {
			/*
			 * We weren't signaled, and message still not sent;
			 * go back to schleep
			 */
			schedule();
			continue;
		}
		/*
		 * We were interrupted
		 */
		LCD_DEBUG(LCD_DEBUG_ERR, "lcd xmit loop interrupted");
		ret = -EINTR;
		break;
	}
out:
	set_current_state(TASK_RUNNING);
	/*
	 * Unset our flag
	 */
	atomic_set(&lcd->xmit_flag, 0);
	return ret;
}

static void debug_dump_utcb(struct lcd *lcd)
{
	int i;

	printk(KERN_ERR "LCD %p UTCB DUMP\n", lcd);
	printk(KERN_ERR "---------------------------------\n");
	
	/* General regs */
	for (i = 0; i < LCD_NUM_REGS; i++) {
		printk("  mr[%d] = %llx\n", i,
			lcd->utcb->mr[i]);
	}

	/* Cptrs */
	for (i = 0; i < LCD_NUM_REGS; i++) {
		printk("  cr[%d] = %lx\n", i,
			cptr_val(lcd->utcb->cr[i]));
	}
}

static int do_send(struct lcd *sender, struct cnode *cnode, 
		struct lcd_sync_endpoint *ep, int making_call,
		int doing_reply)
{
	int ret;
	struct lcd *receiver;

	if (LCD_DEBUG_LVL >= 3)
		debug_dump_utcb(sender);

	if (list_empty(&ep->receivers)) {
		/*
		 * No one receiving; put myself in ep's sender list
		 */
		list_add_tail(&sender->endpoint_queue, &ep->senders);
		/*
		 * Mark myself as making a call, if necessary, so that recvr
		 * knows it needs to copy reply endpoint cap.
		 */
		sender->making_call = making_call;
		sender->doing_reply = doing_reply;
		/*
		 * Wait for a receiver to receive sender's message.
		 *
		 * wait_for_transmit will unlock ep
		 */
		ret = wait_for_transmit(sender, cnode, ep);
		if (ret) { 
			LCD_ERR("transmit failed");
			goto fail1;
		}

		return 0;
	}
	/*
	 * Otherwise, someone waiting to receive. Remove from
	 * receiving queue.
	 */
	receiver = list_first_entry(&ep->receivers, struct lcd, endpoint_queue);
        list_del_init(&receiver->endpoint_queue);
	/*
	 * Lock sender and receiver, for access to their utcb's
	 */
	ret = mutex_lock_interruptible(&sender->lock);
	if (ret) {
		LCD_ERR("interrupted");
		goto fail2;
	}
	ret = mutex_lock_interruptible(&receiver->lock);
	if (ret) {
		LCD_ERR("interrupted");
		goto fail3;
	}
	/*
	 * Put endpoint
	 */
	__lcd_put_sync_endpoint(sender, cnode, ep);
	/*
	 * Send message, and wake up receiver
	 */
	ret = transmit_msg(sender, receiver, making_call, doing_reply);
	if (ret) {
		LCD_ERR("transmit");
		mutex_unlock(&sender->lock);
		/*
		 * Notify receiver and then unlock
		 */
		set_lcd_xmit(receiver, LCD_XMIT_FAILED);
		wake_up_process(receiver->kthread);
		mutex_unlock(&receiver->lock);
		return ret;
	}
	/*
	 * Tell receiver xmit was successful
	 */
	set_lcd_xmit(receiver, LCD_XMIT_SUCCESS);
	wake_up_process(receiver->kthread);
	/*
	 * Unlock lcd's
	 */
	mutex_unlock(&sender->lock);
	mutex_unlock(&receiver->lock);

	return 0;

fail3:
	mutex_unlock(&sender->lock);
fail2:
	__lcd_put_sync_endpoint(sender, cnode, ep);
fail1:
	return ret;
}

int __lcd_send(struct lcd *caller, cptr_t endpoint)
{
	struct lcd_sync_endpoint *ep;
	struct cnode *cnode;
	int ret;
	/*
	 * Look up and lock endpoint
	 */
	ret = __lcd_get_sync_endpoint(caller, endpoint, &cnode, &ep);
	if (ret) {
		LCD_DEBUG(LCD_DEBUG_ERR,
			"ep lookup for cptr 0x%lx", cptr_val(endpoint));
		goto fail1;
	}
	/* do_send does put on ep; we're not doing a call or reply */
	ret = do_send(caller, cnode, ep, 0, 0);
	if (ret) {
		LCD_ERR("failed send");
		goto fail2;
	}

	return 0;

fail2:
	__lcd_put_sync_endpoint(caller, &cnode, &ep);
fail1:
	return ret;
}

static int do_recv(struct lcd *receiver, struct cnode *cnode, 
		struct lcd_sync_endpoint *ep)
{
	int ret;
	struct lcd *sender;

	if (LCD_DEBUG_LVL >= 3)
		debug_dump_utcb(receiver);

	if (list_empty(&ep->senders)) {
		/*
		 * No one sending; put myself in ep's receiver list
		 */
		list_add_tail(&receiver->endpoint_queue, &ep->receivers);
		/*
		 * Wait for a sender to receive message
		 *
		 * wait_for_transmit will unlock ep
		 */
		ret = wait_for_transmit(receiver, cnode, ep);
		if (ret) { 
			LCD_ERR("transmit failed");
			goto fail1;
		}

		return 0;
	}
	/*
	 * Otherwise, someone waiting to send. Remove from
	 * sending queue.
	 */
	sender = list_first_entry(&ep->senders, struct lcd, endpoint_queue);
        list_del_init(&sender->endpoint_queue);
	/*
	 * Lock sender and receiver, for access to their utcb's. (Notice we
	 * lock in the same order as in do_send.)
	 */
	ret = mutex_lock_interruptible(&sender->lock);
	if (ret) {
		LCD_ERR("interrupted");
		goto fail2;
	}
	ret = mutex_lock_interruptible(&receiver->lock);
	if (ret) {
		LCD_ERR("interrupted");
		goto fail3;
	}
	/*
	 * Put endpoint
	 */
	__lcd_put_sync_endpoint(receiver, cnode, ep);
	/*
	 * Send message, and wake up sender
	 *
	 * XXX: we aren't returning any info to receiver to signal whether
	 * sender did call (so it knows to do reply). For now, we'll assume
	 * they have an agreement in their protocol.
	 */
	ret = transmit_msg(sender, receiver, sender->making_call,
			sender->doing_reply);
	if (ret) {
		LCD_ERR("transmit");
		/*
		 * Notify sender, and unlock.
		 */
		set_lcd_xmit(sender, LCD_XMIT_FAILED);
		wake_up_process(sender->kthread);
		mutex_unlock(&sender->lock);
		mutex_unlock(&receiver->lock);
		return ret;
	}
	/*
	 * Tell sender the transmit is done
	 */
	set_lcd_xmit(sender, LCD_XMIT_SUCCESS);
	wake_up_process(sender->kthread);
	/*
	 * Unlock lcd's
	 */
	mutex_unlock(&sender->lock);
	mutex_unlock(&receiver->lock);

	return 0;

fail3:
	mutex_unlock(&sender->lock);
fail2:
	__lcd_put_sync_endpoint(receiver, cnode, ep);
fail1:
	return ret;
}

int __lcd_recv(struct lcd *caller, cptr_t endpoint)
{
	struct lcd_sync_endpoint *ep;
	struct cnode *cnode;
	int ret;
	/*
	 * Look up and lock endpoint
	 */
	ret = __lcd_get_sync_endpoint(caller, endpoint, &cnode, &ep);
	if (ret) {
		LCD_DEBUG(LCD_DEBUG_ERR,
			"ep lookup failed for cptr 0x%lx", cptr_val(endpoint));
		goto fail1;
	}
	/* do_recv does put on ep */
	ret = do_recv(caller, cnode, ep);
	if (ret) {
		LCD_ERR("failed receive");
		goto fail2;
	}

	return 0;

fail2:
	__lcd_put_sync_endpoint(caller, cnode, ep);
fail1:
	return ret;
}

int __lcd_call(struct lcd *caller, cptr_t endpoint)
{
	struct lcd_sync_endpoint *ep;
	struct cnode *cnode;
	int ret;
	/*
	 * Look up and lock endpoint
	 */
	ret = __lcd_get_sync_endpoint(caller, endpoint, &cnode, &ep);
	if (ret) {
		LCD_DEBUG(LCD_DEBUG_ERR,
			"ep lookup failed for cptr 0x%lx", cptr_val(endpoint));
		goto fail1;
	}
	/* do_send does put on ep; we're doing a call */
	ret = do_send(caller, cnode, ep, 1, 0);
	if (ret) {
		LCD_ERR("failed send");
		goto fail2;
	}
	/*
	 * Do a receive on the lcd's call endpoint
	 */
	return __lcd_recv(caller, LCD_CPTR_CALL_ENDPOINT);

fail2:
	__lcd_put_sync_endpoint(caller, cnode, ep);
fail1:
	return ret;
}

int __lcd_reply(struct lcd *caller)
{
	struct lcd_sync_endpoint *ep;
	struct cnode *cnode;
	int ret;
	ret = __lcd_get_sync_endpoint(caller, LCD_CPTR_REPLY_ENDPOINT, 
				&cnode, &ep);
	if (ret) {
		LCD_DEBUG(LCD_DEBUG_ERR,
			"ep lookup failed for cptr 0x%lx", cptr_val(endpoint));
		goto fail1;
	}
	/* do_send does put on ep; we're doing a reply */
	ret = do_send(caller, cnode, ep, 0, 1);
	if (ret) {
		LCD_ERR("failed send");
		goto fail2;
	}

	return 0;

fail2:
	__lcd_put_sync_endpoint(caller, cnode, ep);
fail1:
	return ret;
}

/* INIT/EXIT -------------------------------------------------- */

int __lcd_ipc_init(void)
{
	/* Nothing to do for now */
	return 0;
}

void __lcd_ipc_exit(void)
{
	return;
}

