/* 
 * ipc.c
 *
 * Authors: Anton Burtsev <aburtsev@flux.utah.edu>
 *          Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 */

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <lcd-prototype/lcd.h>
#include "../include/common.h"
#include "defs.h"

static void copy_msg_regs(struct lcd *from, struct lcd *to)
{
	int i;
	for (i = 0; i < from->utcb.max_valid_reg_idx; i++)
		to->utcb.regs[i] = from->utcb.regs[i];
	/*
	 * reset
	 */
	from->utcb.max_valid_reg_idx = 0;
}

static void copy_msg_cap(struct lcd *from, struct lcd *to,
			cptr_t from_ptr, cptr_t to_ptr)
{
	int ret;

	ret = lcd_cnode_grant(from->cspace, to->cspace, from_ptr, to_ptr,
			LCD_CAP_RIGHT_ALL);
	if (ret) {
		LCD_ERR("failed to transfer cap @ %d in lcd %p to slot @ %d in lcd %p",
			from_ptr, from, to_ptr, to);
	}
}

static void copy_msg_caps(struct lcd *from, struct lcd *to)
{
	int i;
	for (i = 0; i < from->utcb.max_valid_out_cap_reg_idx &&
		     i < to->utcb.max_valid_in_cap_reg_idx; i++) {
		LCD_MSG("copying cap at %d", i);
		copy_msg_cap(from, to, from->utcb.out_cap_regs[i],
			to->utcb.in_cap_regs[i]);
	}
	/*
	 * reset
	 */
	from->utcb.max_valid_out_cap_reg_idx = 0;
	to->utcb.max_valid_in_cap_reg_idx = 0;
}

static void copy_call_endpoint(struct lcd *from, struct lcd *to)
{
	copy_msg_cap(from, to, from->utcb.call_endpoint_cap,
		to->utcb.reply_endpoint_cap);
	return;	
}

static void transfer_msg(struct lcd *from, struct lcd *to, int making_call)
{
	int ret;
	struct cnode *reply_cnode;
	/*
	 * Copy valid regs
	 */
	copy_msg_regs(from, to);
	/*
	 * Transfer capabilities
	 */
	copy_msg_caps(from, to);
	if (making_call)
		copy_call_endpoint(from, to);
	/*
	 * Free up receiver cnode slot if sender wasn't making call
	 *
	 * XXX: this is obviously a hack
	 */
	if (!making_call) {
		ret = lcd_cap_lock();
		if (ret) {
			LCD_ERR("couldn't lock cap system");
			return;
		}
		ret = __lcd_cnode_lookup(to->cspace, 
					to->utcb.reply_endpoint_cap,
					&reply_cnode);

		if (ret) {
			LCD_ERR("couldn't find reply cnode");
			lcd_cap_unlock();
			return;
		}
		
		__lcd_cnode_free(reply_cnode);
		lcd_cap_unlock();
	}

	return; 
}

static int lcd_do_send(struct lcd *from, cptr_t c, int making_call)
{
	int ret;
	struct lcd *to;
	struct cnode *cnode;
	struct sync_endpoint *e;
	
	/*
	 * Lookup cnode
	 */
	ret = __lcd_cnode_lookup(from->cspace, c, &cnode);
	if (ret) {
		LCD_ERR("looking up cnode");
		goto fail1;
	}
	/*
	 * Confirm type and permissions
	 */
	if (!__lcd_cnode_is_sync_ep(cnode) || !__lcd_cnode_can_write(cnode)) {
		LCD_ERR("cnode not an endpoint, or bad perms");
		ret = -EACCES;
		goto fail1;
	}
	/*
	 * Get synch end point
	 */
	e = __lcd_cnode_object(cnode);
	/*
	 * Check if any recvr waiting, and do immediate send, wake up
	 * recvr
	 *
	 * (Locking endpoint is probably pointless for now since
	 * it's used primarily when cap is locked.)
	 */
	ret = mutex_lock_interruptible(&e->lock);
	if (ret) {
		LCD_ERR("interrupted");
		goto fail1;
	}

	if (list_empty(&e->receivers)) {
		/*
		 * No one receiving; put myself in e's sender list
		 */
		list_add_tail(&from->senders, &e->senders);
		/*
		 * Mark myself as making a call, if necessary, so that recvr
		 * knows it needs to copy reply rendezvous point cap.
		 */
		from->making_call = making_call;
		/*
		 * IMPORTANT: Must unlock cap and e's lock before going to 
		 * sleep.
		 */
		mutex_unlock(&e->lock);
		lcd_cap_unlock();

		set_current_state(TASK_INTERRUPTIBLE);
		schedule();		

		/*
		 * Someone woke me up; re-lock cap. Receiver should transfer
		 * message, so we just return.
		 *
		 * Reset making_call var.
		 */
		from->making_call = 0;
		
		return lcd_cap_lock();
	}

	/*
	 * Otherwise, someone waiting to receive. Remove from
	 * receiving queue.
	 */

	to = list_first_entry(&e->receivers, struct lcd, receivers);
        list_del_init(&to->receivers);
	mutex_unlock(&e->lock);

	/*
	 * XXX: for now, unlock cap (transfer_msg needs cap unlocked, since
	 * it eventually calls grant)
	 */
	lcd_cap_unlock();

	/*
	 * Send message, and wake up lcd
	 */
	transfer_msg(from, to, making_call);

	wake_up_process(to->parent);

	return lcd_cap_lock();
fail1:
	return ret;
}

int __lcd_send(struct lcd *lcd, cptr_t c)
{
	int ret;
	/*
	 * LOCK cap
	 */
	ret = lcd_cap_lock();
	if (ret)
		return ret;

	ret = lcd_do_send(lcd, c, 0);

	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();

	return ret;
}
EXPORT_SYMBOL(__lcd_send);

static int lcd_do_recv(struct lcd *to, cptr_t c)
{
	int ret;
	struct lcd *from;
	struct sync_endpoint *e;
	struct cnode *cnode;
	
	/*
	 * Lookup cnode
	 */
	ret = __lcd_cnode_lookup(to->cspace, c, &cnode);
	if (ret) {
		LCD_ERR("recvr looking up cnode");
		goto fail1;
	}
	/*
	 * Confirm type and permissions
	 */
	if (!__lcd_cnode_is_sync_ep(cnode) || !__lcd_cnode_can_read(cnode)) {
		LCD_ERR("not an endpoint, or bad perms");
		ret = -EACCES;
		goto fail1;
	}
	/*
	 * Get synch end point proxy and end point
	 */
	e = __lcd_cnode_object(cnode);
	/*
	 * Check if any sender waiting, and do immediate recv, wake up
	 * sender
	 *
	 * (Locking endpoint is probably pointless for now since
	 * it's used primarily when cap is locked.)
	 */
	ret = mutex_lock_interruptible(&e->lock);
	if (ret) {
		LCD_ERR("interrupted");
		return ret;
	}

	if (list_empty(&e->senders)) {
		/*
		 * No one sending; put myself in e's recvr list and lcd's
		 * receiving eps list.
		 *
		 * XXX: maybe we want per-lcd lock?
		 */
		list_add_tail(&to->receivers, &e->receivers);
		/*
		 *
		 * IMPORTANT: Must unlock cap before going to sleep.
		 */
		mutex_unlock(&e->lock);
		lcd_cap_unlock();

		set_current_state(TASK_INTERRUPTIBLE);
		schedule();		

		/*
		 * Someone woke me up; re-lock cap. Receiver should transfer
		 * message, so we just return.
		 */
		return lcd_cap_lock();
	}

	/*
	 * Otherwise, someone waiting to send. Remove from
	 * sending queue.
	 */
	from = list_first_entry(&e->senders, struct lcd, senders);
        list_del_init(&from->senders);
	mutex_unlock(&e->lock);

	/*
	 * XXX: for now, unlock cap (transfer_msg needs cap unlocked, since
	 * it eventually calls grant)
	 */
	lcd_cap_unlock();

	/*
	 * Send message, and wake up lcd
	 */
	transfer_msg(from, to, from->making_call);

	wake_up_process(from->parent);

	return lcd_cap_lock();
fail1:
	return ret;
}

static int lcd_reply_alloc(struct lcd *lcd)
{
	cptr_t reply_cap;
	int ret;

	ret = lcd_cnode_alloc(lcd->cspace, &reply_cap);
	if (ret) {
		LCD_ERR("alloc failed");
		return ret;
	}

	lcd->utcb.reply_endpoint_cap = reply_cap;

	return 0;
}

int __lcd_recv(struct lcd *lcd, cptr_t c)
{
	int ret;
	/*
	 * Alloc slot for reply endpoint, in case sender does call
	 */
	ret = lcd_reply_alloc(lcd);
	if (ret)
		return ret;	
	/*
	 * LOCK cap
	 */
	ret = lcd_cap_lock();
	if (ret)
		return ret;

	ret = lcd_do_recv(lcd, c);

	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();

	return ret;
}
EXPORT_SYMBOL(__lcd_recv);

static int lcd_call_alloc(struct lcd *lcd)
{
	int ret;
	cptr_t call_cptr;

	ret = lcd_cnode_alloc(lcd->cspace, &call_cptr);
	if (ret) {
		LCD_ERR("alloc failed");
		return ret;
	}

	ret = __lcd_mk_sync_endpoint(lcd, call_cptr);
	if (ret) {
		LCD_ERR("couldn't make endpoint");
		return ret;
	}

	lcd->utcb.call_endpoint_cap = call_cptr;

	return 0;
}

int __lcd_call(struct lcd *lcd, cptr_t c)
{
	int ret;
	/*
	 * Alloc special call endpoint
	 */
	ret = lcd_call_alloc(lcd);
	if (ret)
		return ret;
	/*
	 * LOCK cap
	 */
	ret = lcd_cap_lock();
	if (ret)
		goto out;

	ret = lcd_do_send(lcd, c, 1);

	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();

	if (ret) {
		LCD_ERR("send of call failed");
		goto out;
	}

	/*
	 * Receive on my special end point
	 */
	ret = __lcd_recv(lcd, lcd->utcb.call_endpoint_cap);
	if (ret) {
		LCD_ERR("recv of call failed");
		goto out;
	}

	ret = 0;
	goto out;

out:
	__lcd_rm_sync_endpoint(lcd, lcd->utcb.call_endpoint_cap);
	return ret;
}
EXPORT_SYMBOL(__lcd_call);

int __lcd_reply(struct lcd *lcd)
{
	int ret;
	/*
	 * LOCK cap
	 */
	ret = lcd_cap_lock();
	if (ret)
		return ret;

	ret = lcd_do_send(lcd, lcd->utcb.reply_endpoint_cap, 0);

	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();

	return ret;
}
EXPORT_SYMBOL(__lcd_reply);

int __lcd_mk_sync_endpoint(struct lcd *lcd, cptr_t c)
{
	struct sync_endpoint *e;
	int ret = -EINVAL;
	/*
	 * Allocate end point
	 */
	e = kmalloc(sizeof(*e), GFP_KERNEL);
	if (!e) {
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Initialize end point
	 */
	INIT_LIST_HEAD(&e->senders);
	INIT_LIST_HEAD(&e->receivers);
	mutex_init(&e->lock);
	/*
	 * Insert endpoint into cspace at c
	 */
	ret = lcd_cnode_insert(lcd->cspace, c, e, LCD_CAP_TYPE_SYNC_EP,
			LCD_CAP_RIGHT_ALL);
	if (ret) {
		LCD_ERR("insert endpoint");
		goto fail2;
	}
	return 0;

fail2:
	kfree(e);
fail1:
	return ret;
}
EXPORT_SYMBOL(__lcd_mk_sync_endpoint);

static int __cleanup_sync_endpoint(struct cnode *cnode)
{
	struct sync_endpoint *e;
	/*
	 * Check that cnode contains sync ep, and lcd is owner
	 */
	if (!__lcd_cnode_is_sync_ep(cnode))
		return -EINVAL;
	if (!__lcd_cnode_is_owner(cnode))
		return -EINVAL;

	e = __lcd_cnode_object(cnode);

	/*
	 * Remove from cspaces first, so no one can refer to sync ep
	 */
	__lcd_cnode_free(cnode);

	/*
	 * Free end point
	 */
	kfree(e);

	return 0;
}

static int lcd_do_rm_sync_endpoint(struct lcd *lcd, cptr_t cptr)
{
	int ret;
	struct cnode *cnode;
	/*
	 * Look up cnode
	 */
	ret = __lcd_cnode_lookup(lcd->cspace, cptr, &cnode);
	if (ret) {
		LCD_ERR("cnode lookup at %lld", cptr);
		return ret;
	}
	/*
	 * Remove it from lcd's cspace, and recursively remove from
	 * any cspaces with derivations
	 */
	return __cleanup_sync_endpoint(cnode);
}

int __lcd_rm_sync_endpoint(struct lcd *lcd, cptr_t cptr)
{
	int ret;
	/*
	 * LOCK cap
	 */
	ret = lcd_cap_lock();
	if (ret)
		return ret;
	ret = lcd_do_rm_sync_endpoint(lcd, cptr);
	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();
	return ret;
}
EXPORT_SYMBOL(__lcd_rm_sync_endpoint);
