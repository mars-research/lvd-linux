/* 
 * microkernel.c
 *
 * Authors: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <lcd-prototype/lcd.h>
#include <lcd-prototype/lcd-msg.h>

struct lcd_rvp {
	struct list_head senders;
	struct list_head receivers;
	struct mutex *lock;
};

static struct list_head all_lcds;

/* API ROUTINES -------------------------------------------------- */

static void handle_api_cap_alloc(struct lcd_thread *t)
{
	cptr_t c;
	int ret;

	ret = lcd_alloc_cap(t->lcd->cspace->cap_cache, &c);
	if (ret) {
		__lcd_set_m0(LCD_ENOMEM);
		return;
	}
	__lcd_set_m0(0);
	__lcd_set_m1(c);
	return;
}

static int create_lcd(char *mname, struct lcd **out)
{
	/*
	 * Allocate lcd struct
	 */
}

static void handle_api_create_lcd1(struct lcd_thread *t)
{
	char *mname;
	u64 mname_len;
	cptr_t *cptrs;
	u64 cptr_len;
	int ret;
	struct lcd *lcd;
	/*
	 * Get args
	 *
	 * XXX: More care required when type 1 lcd's moved to separate
	 * address space.
	 */
	if (lcd_max_m(t) < 5) {
		__lcd_set_m0(t, LCD_EINVAL);
		return;
	}
	mname = __lcd_m1(t);
	mname_len = __lcd_m2(t);
	cptrs = __lcd_m3(t);
	cptr_len = __lcd_m4(t);
	/*
	 * Make sure mname is NUL terminated
	 */
	mname[mnane_len] = 0;
	/*
	 * Load module
	 */
	ret = request_module(mname);
	if (ret)
		goto fail1;
	/*
	 * Create lcd
	 */
	ret = create_lcd(mname, &lcd);
	if (ret)
		goto fail2;
	/*
	 * Add lcd to list of all lcd's
	 */
	list_add_head(&lcd->all_lcds, &all_lcds);
	/*
	 * Install capabilities (including microkernel cap) in lcd's cspace
	 */
	ret = install_caps(lcd, cptrs, cptr_len);
	if (ret)
		goto fail3;
	/*
	 * Spin up kthread, etc.
	 */
	ret = boot_lcd(lcd);
	if (ret) 
		goto fail4;

	__lcd_set_m0(0);
	return;

fail4:
	/* any cap tear down necessary? */
fail3:
	/* tear down lcd */
fail2:
	/* unload module? */
fail1:
	__lcd_set_m0(LCD_EIO);
	return;
}

static void handle_api_create_lcd2(struct lcd_thread *t)
{
	/*
	 * Equivalent to type 1 for now, since same addr space ...
	 */
}

static void handle_api_exit(struct lcd_thread *t)
{
	/*
	 * */

}

static void api_call(struct lcd_thread *t)
{
	/*
	 * Confirm there is at least one valid message register (containing
	 * the api call index)
	 */
	if (lcd_max_m(t->tcb) == 0) {
		__lcd_set_r0(t, LCD_EINVAL);
		return;
	}

	switch (__lcd_m0(t)) {
	case LCD_API_CAP_ALLOC:
		/* check capability */
		break;
	case LCD_API_CREATE_LCD1:
		/* check capability */
		break;
	case LCD_API_CREATE_LCD2:
		/* check capability */
		break;
	case LCD_API_EXIT:
		/* check capability */
		break;
	default:
		__lcd_set_r0(t, LCD_EINVAL);
		return;
	}

	__lcd_set_r0(t, 0);
	return;
}

/* IPC -------------------------------------------------- */

static void copy_msg_regs(struct lcd_thread *sender, 
			struct lcd_thread *receiver)
{
	int i;
	for (i = 0; i < lcd_max_m(sender->tcb); i++)
		receiver->tcb->m[i] = sender->tcb->m[i];
}

static void copy_msg_cap(struct lcd_thread *sender, struct lcd_thread *receiver,
			cptr_t sender_cptr, cptr_t receiver_cptr,
			unsigned int rights)
{
	int ret;

	ret = lcd_cnode_grant(sender->cspace, receiver->cspace, 
			from_ptr, to_ptr, rights);
	if (ret)
		LCD_ERR("failed to transfer cap @ %d in lcd_thread %p to slot @ %d in lcd_thread %p",
			sender_cptr, sender, receiver_cptr, receiver);
}

static void copy_msg_caps(struct lcd_thread *sender, 
			struct lcd_thread *receiver)
{
	int i;
	for (i = 0; i < lcd_max_oc(sender->tcb) && 
		     i < lcd_max_ic(receiver->tcb); i++) {
		copy_msg_cap(sender, receiver, sender->tcb->oc[i],
			sender->tcb->oc_rights[i], receiver->tcb->ic[i]);
	}
}

static void transfer_msg(struct lcd_thread *sender, struct lcd_thread *receiver,
			int is_call)
{
	copy_msg_regs(sender, receiver);
	copy_msg_caps(sender, receiver);
	if (is_call) {
		copy_msg_cap(sender, receiver, sender->call_cptr,
			receiver->reply_cptr, LCD_CAP_RIGHT_SEND);
	}	
}

static struct lcd_rvp *lookup_rvp(struct lcd_thread *lcd_thread, cptr_t c,
				unsigned int required_rights)
{
	struct cnode *cnode;
	ret = lcd_cnode_lookup(lcd_thread->lcd->cspace, c, &cnode);
	if (ret)
		return NULL;
	if (cnode->type != LCD_CAP_TYPE_SYNC_EP)
		return NULL;
	/* TODO: Uncomment when rights are implemented in caps. */
	/* if (cnode->rights & required_rights != required_rights) */
	/* 	return NULL; */
	return cnode->object;
}

static void lcd_do_send(struct lcd_thread *sender, struct lcd_rvp *rvp,
			int is_call)
{
	int ret;
	struct lcd_thread *receiver;

	ret = mutex_lock_interruptible(&rvp->lock);
	if (ret) {
		LCD_ERR("interrupted");
		__lcd_set_r0(sender, LCD_EINTR);
		return;
	}

	if (list_empty(&rvp->receivers)) {
		/*
		 * No one receiving; put myself in rvp's sender list
		 */
		list_add_tail(&sender->rvp_queue, &rvp->senders);
		/*
		 * Mark myself as calling, if needed, so receiver knows to
		 * copy special reply cap.
		 */
		sender->is_calling = is_call;
		/*
		 * Unlock mutex before we sleep.
		 */
		mutex_unlock(&rvp->lock);

		set_current_state(TASK_INTERRUPTIBLE);
		schedule();

		/*
		 * Someone woke me up. Receiver should have transferred
		 * message, so we just return.
		 */
		__lcd_set_r0(sender, 0);
		return;
	}
	/*
	 * Otherwise, someone waiting to receive. Remove from
	 * receiving queue.
	 */
	receiver = list_first_entry(&rvp->receivers, struct lcd_thread, 
				rvp_queue);
        list_del_init(&receiver->rvp_queue);
	/*
	 * Done with rvp for now.
	 */
	mutex_unlock(&rvp->lock);
	/*
	 * Send message, and wake up receiver.
	 */
	transfer_msg(sender, receiver, is_call);
	wake_up_process(receiver->thread);
	/*
	 * Return success
	 */
	__lcd_set_r0(sender, 0);
	return;
}

static void lcd_send(struct lcd_thread *sender)
{
	cptr_t c;
	struct lcd_rvp *rvp;

	c = __lcd_r1(sender);
	/*
	 * Check for api call.
	 */
	if (c == LCD_API_CPTR) {
		api_call(sender);
		return;
	}	
	/*
	 * Look up rvp and confirm send rights
	 *
	 * TODO: Uncomment below when send rights implemented.
	 */

	rvp = lookup_rvp(sender, c); /*, LCD_CAP_RIGHT_SEND); */
	if (!rvp) {
		LCD_ERR("lookup for rvp at %llu failed", c);
		__lcd_set_r0(sender, LCD_EINVAL);
		return;
	}
	lcd_do_send(sender, rvp, 0);
	return;
}

static void lcd_do_recv(struct lcd_thread *receiver, struct lcd_rvp *rvp,
{
	int ret;
	struct lcd_thread *sender;

	ret = mutex_lock_interruptible(&rvp->lock);
	if (ret) {
		LCD_ERR("interrupted");
		__lcd_set_r0(receiver, LCD_EINTR);
		return;
	}

	if (list_empty(&rvp->senders)) {
		/*
		 * No one sending; put myself in rvp's receiver list
		 */
		list_add_tail(&receiver->rvp_queue, &rvp->receivers);
		/*
		 * Unlock mutex before we sleep.
		 */
		mutex_unlock(&rvp->lock);

		set_current_state(TASK_INTERRUPTIBLE);
		schedule();

		/*
		 * Someone woke me up. Sender should have transferred
		 * message, so we just return.
		 */
		__lcd_set_r0(receiver, 0);
		return;
	}
	/*
	 * Otherwise, someone waiting to send. Remove from
	 * sending queue.
	 */
	sender = list_first_entry(&rvp->senders, struct lcd_thread, 
				rvp_queue);
        list_del_init(&sender->rvp_queue);
	/*
	 * Done with rvp for now.
	 */
	mutex_unlock(&rvp->lock);
	/*
	 * Receive message, and wake up sender.
	 */
	transfer_msg(sender, receiver, sender->is_calling);
	wake_up_process(sender->thread);
	/*
	 * Return success
	 */
	__lcd_set_r0(receiver, 0);
	return;
}

static void lcd_recv(struct lcd_thread *receiver)
{
	cptr_t c;
	struct lcd_rvp *rvp;
	/*
	 * Look up rvp and confirm receive rights
	 *
	 * TODO: Uncomment below when receive rights implemented.
	 */
	c = __lcd_r1(receiver);
	rvp = lookup_rvp(receiver, c); /*, LCD_CAP_RIGHT_RECV); */
	if (!rvp) {
		LCD_ERR("lookup for rvp at %llu failed", c);
		__lcd_set_r0(receiver, LCD_EINVAL);
		return;
	}
	lcd_do_recv(receiver, rvp);
	return;
}

static void lcd_call(struct lcd_thread *sender)
{
	cptr_t c;
	struct lcd_rvp *rvp;
	/*
	 * Look up rvp and confirm send rights
	 *
	 * TODO: Uncomment below when send rights implemented.
	 */
	c = __lcd_r1(sender);
	rvp = lookup_rvp(sender, c); /*, LCD_CAP_RIGHT_SEND); */
	if (!rvp) {
		LCD_ERR("lookup for rvp at %llu failed", c);
		__lcd_set_r0(sender, LCD_EINVAL);
		return;
	}
	/*
	 * Do send
	 */
	lcd_do_send(sender, rvp, 1);
	/*
	 * Listen on my rvp
	 */
	rvp = lookup_rvp(sender, sender->call_cptr); /*, LCD_CAP_RIGHT_RECV); */
	if (!rvp) {
		LCD_ERR("lookup for rvp at %llu failed", c);
		__lcd_set_r0(sender, LCD_EINVAL);
		return;
	}
	lcd_do_recv(sender, rvp);
	return;
}

static void lcd_reply(struct lcd_thread *sender)
{
	struct lcd_rvp *rvp;
	/*
	 * Look up rvp and confirm send rights
	 *
	 * TODO: Uncomment below when send rights implemented.
	 */
	rvp = lookup_rvp(sender, sender->reply_cptr);/*, LCD_CAP_RIGHT_SEND); */
	if (!rvp) {
		LCD_ERR("lookup for reply rvp at %llu failed", 
			sender->reply_cptr);
		__lcd_set_r0(sender, LCD_EINVAL);
		return;
	}
	lcd_do_send(sender, rvp, 0);
	return;
}

/* INTERRUPT -------------------------------------------------- */

/*
 * The `interrupt' handler.
 */
void lcd_int(void)
{
	struct lcd_thread *lcd_thread = current->lcd_thread;
	
	/*
	 * Check that calling thread is an lcd thread
	 */
	if (!lcd_thread)
		return;
	/*
	 * Confirm there is at least one valid machine register (containing
	 * the system call index)
	 */
	if (lcd_max_r(lcd_thread->tcb) == 0) {
		__lcd_set_r0(lcd_thread, LCD_EINVAL);
		return;
	}

	switch (__lcd_r0(lcd_thread)) {
	case LCD_SYSCALL_SEND:
		lcd_send(lcd_thread);
		break;
	case LCD_SYSCALL_RECV:
		lcd_recv(lcd_thread);
		break;
	case LCD_SYSCALL_CALL:
		lcd_call(lcd_thread);
		break;
	case LCD_SYSCALL_REPLY:
		lcd_reply(lcd_thread);
		break;
	default:
		__lcd_set_r0(lcd_thread, LCD_EINVAL);
		break;
	}
	
	/*
	 * Reset counters
	 */
	lcd_set_max_r(lcd_thread->tcb, 0);
	lcd_set_max_m(lcd_thread->tcb, 0);
	lcd_set_max_oc(lcd_thread->tcb, 0);
	lcd_set_max_ic(lcd_thread->tcb, 0);
	/*
	 * Clear is_call var
	 */
	lcd_thread->is_calling = 0;

	return;
}
EXPORT_SYMBOL(lcd_int);

int __init microkernel_init(void)
{
	LCD_MSG("microkernel starting");
	return 0;
}

void __exit microkernel_exit(void)
{
	LCD_MSG("microkernel exiting");
	return;
}

module_init(microkernel_init);
module_exit(microkernel_exit);

