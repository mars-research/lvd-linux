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
	/*
	 * owner is needed so we can identify an lcd_thread via an lcd_rvp.
	 * For now, there is a 1-1 correspondence between lcd_thread's and
	 * the rvp's they can receive on.
	 */
	struct lcd_thread *owner;
	struct list_head senders;
	struct list_head receivers;
	struct mutex lock;
};

static struct list_head all_lcds;

/* API ROUTINES -------------------------------------------------- */

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

static int __create_lcd(struct lcd **out)
{
	struct lcd *lcd;
	/*
	 * Allocate lcd struct
	 */
	lcd = kzalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd)
		return -ENOMEM;
	/* 
	 * TODO: init cspace 
	 */
	
	/* 
	 * Init lists and lock
	 */
	mutex_init(&lcd->lock);
	list_init(&lcd->threads);
	list_init(&lcd->all_lists);

	*out = lcd;

	return 0;
}

static int create_lcd(char *mname, struct lcd **out)
{
	struct lcd *lcd;
	int ret;
	/*
	 * Init lcd data structure
	 */
	ret = __create_lcd(&lcd);
	if (ret)
		return ret;
	/*
	 * Load module via usermode helper. This call blocks
	 * and returns the exit status of lcd-modprobe.
	 */
	ret = request_lcd_module(mname);
	if (ret < 0) {
		LCD_ERR("load module");
		goto fail1;
	}
	/*
	 * Find loaded module, and inc its ref count; must hold module mutex
	 * while finding module.
	 */
	mutex_lock(&module_mutex);
	lcd->m = find_module(mname);
	mutex_unlock(&module_mutex);	
	if (!lcd->m) {
		LCD_ERR("couldn't find module");
		goto fail1;
	}
	if(!try_module_get(lcd->m)) {
		LCD_ERR("incrementing module ref count");
		goto fail2;
	}

	*out = lcd;

	return 0;

fail2:
	ret = do_sys_delete_module(mname, 0, 1);
	if (ret)
		LCD_ERR("deleting module");
fail1:
	kfree(lcd);
	return -EIO;	
}

void tear_down_lcd(struct lcd **lcd_ptr)
{
	struct lcd *lcd;
	lcd = *lcd_ptr;
	/*
	 * Confirm no contained threads
	 */
	BUG_ON(!list_empty(&lcd->threads));
	/*
	 * Put the module (rm it?)
	 */
	if (lcd->type != LCD_TYPE_3)
		module_put(lcd->m);
	/*
	 * Tear down cspace
	 *
	 * XXX: What to do if non-zero ret?
	 */
	ret = lcd_cap_destroy_cspace(lcd->cspace);
	if (ret) {
		LCD_ERR("couldn't destroy cspace");
		return;
	}
	/*
	 * Remove myself from list of lcd's
	 */
	list_del(&lcd->all_list);
	/*
	 * Free lcd struct
	 */
	kfree(lcd);
	*lcd_ptr = NULL;
	return;
}

int install_caps(struct lcd_thread *caller, struct lcd *new_lcd, 
		cptr_t *cptrs, unsigned int *rights, unsigned int cptr_len,
		cptr_t **out_cptrs)
{
	unsigned int i;
	cptr_t new_c;
	int ret;

	/*
	 * TODO: what about microkernel cap?
	 */

	for (i = 0; i < cptr_len; i++) {
		/*
		 * Allocate a slot in new lcd's cspace
		 */
		ret = lcd_alloc_cap(new_lcd->cspace->cap_cache, &new_c);
		if (ret)
			goto fail;
		/*
		 * Grant from caller's cspace to new one
		 */
		ret = lcd_cnode_grant(caller->lcd->cspace, new_lcd->cspace, 
				cptrs[i], new_c, rights[i]);
		if (ret)
			goto fail;
		/*
		 * Store cptr
		 */
		*out_cptrs[i] = new_c;
	}

	return 0;


fail:
	LCD_ERR("failed to grant caps in creation of lcd");
	return -EIO;
}

struct microkernel_load {
	struct module *m;
	cptr_t *cptrs;
	unsigned int cptr_len;
};

static int lcd_kthread_main(void *__info)
{
	struct microkernel_load *info;

	info = __info;

	info->m->gmain(info->cptr_len, info->cptrs);

	return 0;
}

int setup_lcd_tcb(struct lcd_thread *lcd_thread)
{
	struct lcd_tcb *lcd_tcb;

	lcd_tcb = kzalloc(sizeof(*lcd_tcb));
	if (!lcd_tcb)
		return -ENOMEM;
	
	lcd_thread->tcb = lcd_tcb;
	return 0;
}

/*
 * Must call after installing lcd_thread's parent lcd (so it can access the
 * lcd's cspace).
 */
int setup_lcd_rvp(struct lcd_thread *lcd_thread)
{
	struct lcd_rvp *lcd_rvp;
	int ret;
	cptr_t c;
	/*
	 * Make lcd_rvp
	 */
	lcd_rvp = kmalloc(sizeof(*lcd_rvp));
	if (!lcd_rvp)
		return -ENOMEM;
	mutex_init(&lcd_rvp->lock);
	list_init(&lcd_rvp->senders);
	list_init(&lcd_rvp->receivers);
	lcd_rvp->owner = lcd_thread;
	/*
	 * Install in cspace
	 */
	ret = lcd_alloc_cap(lcd_thread->lcd->cspace->cap_cache, &c);
	if (ret) {
		kfree(lcd_rvp);
		return -ENOMEM;
	}
	/*
	 * XXX: Do we want all rights for this rvp?
	 */
	ret = lcd_cap_insert(lcd_thread->lcd->cspace, c, lcd_rvp,
			LCD_CAP_TYPE_SYNC_EP, LCD_CAP_RIGHT_ALL);
	if (ret) {
		/* XXX: does this free the rvp? */
		lcd_free_cap(lcd_thread->lcd->cspace->cap_cache, c);
		return -EIO;
	}
	/*
	 * Store cptr in lcd_thread
	 */
	lcd_thread->call_cptr = c;
	return 0;
}

/*
 * Assumptions: No receivers in rvp's receiver queue (this thread is the
 * only thread that can receive).
 */
void tear_down_lcd_rvp(struct lcd_thread *lcd_thread, struct lcd_rvp *rvp)
{
	struct cnode *cnode;
	int ret;
	struct list_head *cursor;
	struct list_head *next;
	struct lcd_thread *sender;
	/*
	 * Revoke all rights except kill on the rvp (so no one except the parent
	 * can use it anymore)
	 */
	ret = lcd_cap_revoke(lcd_thread->lcd->cspace, lcd_thread->calling_cptr,
			~LCD_CAP_RIGHT_KILL);	
	if (ret) {
		/* XXX: just fail? */
		LCD_ERR("failed to revoke rights on rvp during tear down");
		return;
	}
	/*
	 * Wake up all senders in rvp (should be no receivers)
	 *
	 * XXX: If any threads are sitting in the send queue sleeping,
	 * they will be woken up and will think their message was sent.
	 * We should probably communicate failure somehow.
	 */
	ret = mutex_lock_interruptible(&rvp->lock);
	if (ret) {
		/* XXX: just fail? */
		LCD_ERR("couldn't lock rvp to tear it down");
		return;
	}
	list_for_each_safe(cursor, next, &rvp->senders) {
		sender = list_entry(cursor, struct lcd_thread, rvp_queue);
		list_del_init(&sender->rvp_queue);
		wake_up_process(sender->thread);
	}
	mutex_unlock(&rvp->lock);

	/*
	 * Don't delete the rvp yet. The parent will need it to reap for type
	 * 1 and 2.
	 */

	return;
}

static void finish_lcd_thread_tear_down(struct lcd_thread *thread,
					struct lcd_rvp *rvp)
{
	/*
	 * Delete rvp from all cspaces
	 *
	 * XXX: Does this delete the rvp? I'm assuming no.
	 */
	lcd_cap_delete(thread->lcd->cspace, thread->calling_cptr);
	/*
	 * Free the rvp
	 *
	 * XXX: Possible race if someone has already looked up the rvp
	 * and is about to try sending. The real solution would use
	 * reference counting on the rvp or some sort of cnode lock.
	 */
	kfree(rvp);
	/*
	 * Remove thread from containing lcd's threads
	 */
	ret = mutex_lock_interruptible(&thread->lcd->lock);
	if (ret) {
		/* XXX: just return? */
		LCD_ERR("failed to lock lcd to remove myself from list");
		return;
	}
	list_del(&thread->threads);
	mutex_unlock(&thread->lcd->lock);
	/*
	 * Free lcd_thread struct
	 *
	 * XXX: Any race condition possible? Possible for another thread to
	 * try to kill this lcd_thread we are destroying?
	 */
	kfree(thread);

	return;
}

/*
 * Assumptions: Not in an rvp queue when this is called (otherwise,
 * the thread would be asleep).
 */
void tear_down_lcd_thread(struct lcd_thread *lcd_thread)
{
	int ret;
	struct lcd_rvp *rvp;
	/*
	 * Free tcb and gmain args
	 */
	kfree(lcd_thread->tcb);
	kfree(lcd_thread->gmain_args);
	/*
	 * Look up rvp using calling cptr. 
	 *
	 * XXX: We shouldn't care about rights when tearing down, right?
	 */
	rvp = lookup_rvp(lcd_thread, lcd_thread->calling_cptr, 0);
	if (!rvp)
		goto fail1;
	/*
	 * Partially tear down my rvp (we need it to hang around until
	 * the parent has had a chance to reap, for type 1 and type 2 lcds).
	 */
	tear_down_lcd_rvp(lcd_thread, rvp);

	/*
	 * XXX: Any other cspace crap to clean up? Cspace itself not
	 * torn down until lcd torn down.
	 */

	/*
	 * For Type 1 and Type 2:
	 *
	 *    Signal that I've exited; someone should eventually notice, and 
	 *    call finish_lcd_thread_tear_down.
	 *
	 * For Type 3:
	 *
	 *    We finish now.
	 *
	 * XXX: No return value for now.
	 */
	if (lcd_thread->lcd->type != LCD_TYPE_3)
		complete(&lcd_thread->exited);
	else
		finish_lcd_thread_tear_down(lcd_thread, rvp);

	return;
}

int setup_lcd_thread(struct lcd *lcd, struct task_struct *t)
{
	struct lcd_thread *lcd_thread;
	int ret;

	lcd_thread = kmalloc(sizeof(*lcd_thread));
	if(!lcd_thread) {
		ret = -ENOMEM;
		goto fail1;
	}

	/*
	 * TCB
	 */
	ret = setup_lcd_tcb(lcd_thread);
	if (ret)
		goto fail2;

	lcd_thread->thread = t;
	lcd_thread->lcd = lcd;
	
	/*
	 * LCD thread-specific rvp
	 */
	ret = setup_lcd_rvp(lcd_thread);
	if (ret)
		goto fail3;

	list_init(&lcd_thread->threads);
	list_init(&lcd_thread->rvp_queue);

	lcd_thread->is_calling = 0;

	lcd_thread->should_stop = 0;
	init_completion(&lcd_thread->completion);

	/*
	 * Add to lcd's list of threads
	 */
	list_add(&lcd_thread->threads, &lcd->threads);

	return 0;
fail3:
	kfree(lcd_thread->tcb);	
fail2:
	kfree(lcd_thread);
fail1:
	return ret;
}

int boot_lcd(struct lcd *lcd, cptr_t *cptrs, unsigned int cptr_len)
{
	struct task_struct *t;
	struct microkernel_load *info;
	int ret;
	/*
	 * Set up info
	 */
	info = kmalloc(sizeof(*info), GFP_KERNEL);
	if (!info) {
		LCD_ERR("alloc info");
		ret = -ENOMEM;
		goto fail1;
	}
	info->m = lcd->m;
	info->cptrs;
	info->cptr_len;
	/*
	 * Create kthread and info.
	 */
	t = kthread_create(lcd_kthread_main, info, lcd->m->name);
	if (!t) {
		LCD_ERR("spawning kthread");
		ret = -EIO;
		goto fail2;
	}
	/*
	 * kthread owns info after this point ...
	 *
	 * Set up lcd_thread for this kthread
	 */
	ret = setup_lcd_thread(lcd, t);
	if (ret)
		goto fail3;
	/*
	 * Store pointer to gmain args so we can free them when the
	 * lcd_thread is torn down.
	 */
	t->lcd_thread->gmain_args = cptrs;
	/*
	 * Wake up kthread
	 */
	wake_up_process(t);

	return 0;

fail3:
	put_task_struct(t);
fail2:
	kfree(info);
fail1:
	return ret;
}

static int __handle_api_create_lcd(struct lcd_thread *t, struct lcd **out)
{
	char *mname;
	u64 mname_len;
	cptr_t *cptrs;
	unsigned int *rights;
	u64 cptr_len;
	int ret;
	struct lcd *lcd;
	cptr_t *out_cptrs;
	/*
	 * Get args
	 *
	 * XXX: More care required when type 1 lcd's moved to separate
	 * address space.
	 */
	if (lcd_max_m(t) < 6)
		return -EINVAL;

	mname = __lcd_m1(t);
	mname_len = __lcd_m2(t);
	cptrs = __lcd_m3(t);
	rights = __lcd_m4(t);
	cptr_len = __lcd_m5(t);

	/*
	 * Make sure mname is NUL terminated
	 */
	mname[mnane_len] = 0;
	/*
	 * Create lcd
	 */
	ret = create_lcd(mname, &lcd);
	if (ret)
		goto fail1;
	/*
	 * Add lcd to list of all lcd's
	 */
	list_add_head(&lcd->all_lcds, &all_lcds);
	/*
	 * Install capabilities (including microkernel cap) in lcd's cspace
	 */
	out_cptrs = kmalloc(sizeof(*out_cptrs) * cptr_len);
	if (!out_cptrs)
		goto fail2;
	ret = install_caps(lcd, cptrs, rights, cptr_len, &out_cptrs);
	if (ret)
		goto fail3;
	/*
	 * Spin up kthread, etc.
	 */
	ret = boot_lcd(lcd, out_cptrs, cptr_len);
	if (ret) 
		goto fail4;

	*out = lcd;

	/* out_cptrs freed when lcd_thread torn down */

	return;

fail4:
	/* any cap tear down necessary? */
fail3:
	kfree(out_cptrs);
fail2:
	tear_down_lcd(lcd);
fail1:
	return -EIO;
}

static void handle_api_create_lcd1(struct lcd_thread *t)
{
	int ret;
	struct lcd *lcd;
	ret = __handle_api_create_lcd(t, &lcd);
	if (ret) {
		/* error */
		__lcd_set_m0(t, LCD_EIO);
		return;
	} else {
		lcd->type = LCD_TYPE_1;
		__lcd_set_m0(0);
		return;
	}
}

static void handle_api_create_lcd2(struct lcd_thread *t)
{
	/*
	 * XXX: Equivalent to type 1 for now, since we are using the same
	 * address space for type 1 lcd's for now.
	 */
	int ret;
	struct lcd *lcd;
	ret = __handle_api_create_lcd(t, &lcd);
	if (ret) {
		/* error */
		__lcd_set_m0(t, LCD_EIO);
		return;
	} else {
		lcd->type = LCD_TYPE_2;
		__lcd_set_m0(0);
		return;
	}
}

static void handle_api_exit(struct lcd_thread *t)
{
	/*
	 * Behavior differs depending on lcd type.
	 *
	 * Type 1 and Type 2 lcd's:
	 *
	 *    The creator of the lcd is responsible for `reaping' the lcd's
	 *    lcd_threads by removing them from the lcd's list and freeing
	 *    the lcd_thread struct.
	 *
	 *    This is done in api_kill.
	 *
	 *    The containing kthread will be reaped by kthreadd.
	 *
	 * Type 3 lcd's:
	 *
	 *    All lcd_thread-specific data is freed up, and the thread is
	 *    removed from the lcd.
	 */
	tear_down_lcd_thread(t);
	if (t->lcd->type != LCD_TYPE_3) {
		do_exit(0);
	}
	else {
		/* null out lcd_thread field */
		current->lcd_thread = NULL;
	}		
}


/*
 * Assumptions: Only one thread will call this for a specific targeted lcd
 * at one time.
 */
static void handle_api_kill(struct lcd_thread *caller)
{
	struct lcd_rvp *rvp;
	struct lcd_thread *target;
	cptr_t c;
	int ret;
	/*
	 * Look up the rvp corresponding to the lcd_thread to kill / reap.
	 *
	 * XXX: Caller must have kill rights?
	 *
	 * Also note: If the targeted lcd_thread has already torn itself
	 * down partially, the rvp will still be in the caller's cspace
	 * so that we can look it up.
	 */
	c = __lcd_m1(caller);
	rvp = lookup_rvp(caller, c, LCD_CAP_RIGHT_KILL);
	if (!rvp)
		goto fail1;
	/*
	 * Get the targeted lcd_thread, and tell it to stop.
	 */
	target = rvp->owner;
	target->should_stop = 1;
	/*
	 * Wait until it exits. If it has already exited, this will return
	 * immediately.
	 */
	ret = wait_for_completion_interruptible(&target->exited);
	if (ret) {
		/* XXX: just fail? */
		LCD_ERR("failed to wait till target exited");
		goto fail1;
	}
	/*
	 * lcd_thread exited; do the rest of the tear down. This may
	 * tear down the containing lcd if this is the last thread
	 * inside it.
	 */
	finish_lcd_thread_tear_down(target, rvp);

	__lcd_set_m0(caller, 0);
	return;

fail1:
	__lcd_set_m0(caller, LCD_EIO);
	return;
}

/* Returns 1 if thread in type 3 lcd just exited. */
static int api_call(struct lcd_thread *t)
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
		handle_api_cap_alloc(t);
		break;
	case LCD_API_CREATE_LCD1:
		handle_api_create_lcd1(t);
		break;
	case LCD_API_CREATE_LCD2:
		handle_api_create_lcd2(t);
		break;
	case LCD_API_EXIT:
		handle_api_exit(t);
		/* Type 1 and Type 2's never return, but Type 3 will,
		 * so we need to do a return here instead of a break. */
		return 1;
	case LCD_API_KILL:
		handle_api_kill(t);
		break;
	default:
		__lcd_set_r0(t, LCD_EINVAL);
		return;
	}

	__lcd_set_r0(t, 0);
	return 0;
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

	ret = lcd_cnode_grant(sender->lcd->cspace, receiver->lcd->cspace, 
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

static int lcd_send(struct lcd_thread *sender)
{
	cptr_t c;
	struct lcd_rvp *rvp;

	c = __lcd_r1(sender);
	/*
	 * Check for api call. Returns 1 if thread in type 3 lcd exited.
	 */
	if (c == LCD_API_CPTR)
		return api_call(sender);
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

static void lcd_do_recv(struct lcd_thread *receiver, struct lcd_rvp *rvp)
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

/* NON-ISOLATED CODE INTERFACE ---------------------------------------- */

int lcd_create_self(int (*init)(cptr_t), int (*entry)(cptr_t),
		int (*exit)(cptr_t), struct lcd **out)
{
	struct lcd *lcd;
	int ret;
	/*
	 * Init lcd data structure
	 */
	ret = __create_lcd(&lcd);
	if (ret)
		goto fail1;
	/*
	 * Install entry / exit routines
	 */
	lcd->entry = entry;
	lcd->exit = exit;
	/*
	 * Set up lcd_thread using current calling thread
	 */
	ret = setup_lcd_thread(lcd, current);
	if (ret)
		goto fail2;
	/*
	 * Call init, with microkernel API cap
	 */
	ret = init(LCD_API_CPTR);
	if (ret)
		goto fail3;

	*out = lcd;

	return 0;
fail3:
	/* XXX: correct to do this here? */
	tear_down_lcd_thread(current->lcd_thread);
fail2:
	/* XXX: correct to do this here? */
	tear_down_lcd(lcd);
fail1:
	return ret;
}

int lcd_enter(struct lcd *lcd)
{
	/*
	 * Set up lcd_thread using current calling thread
	 */
	ret = setup_lcd_thread(lcd, current);
	if (ret)
		goto fail1;
	/*
	 * Call lcd's entry, with microkernel API cap
	 */
	ret = lcd->entry(LCD_API_CPTR);
	if (ret)
		goto fail2;

	return 0;
fail2:
	/* XXX: correct to do this here? */
	tear_down_lcd_thread(current->lcd_thread);
fail1:
	return ret;
}

int lcd_exit(struct lcd *lcd)
{
	/*
	 * Set up lcd_thread using current calling thread
	 */
	ret = setup_lcd_thread(lcd, current);
	if (ret)
		goto fail1;
	/*
	 * Call lcd's exit, with microkernel API cap
	 */
	ret = lcd->exit(LCD_API_CPTR);
	if (ret)
		goto fail2;

	return 0;
fail2:
	/* XXX: correct to do this here? */
	tear_down_lcd_thread(current->lcd_thread);
fail1:
	return ret;
}

/* INTERRUPT -------------------------------------------------- */

/*
 * The `interrupt' handler.
 */
void lcd_int(void)
{
	struct lcd_thread *lcd_thread = current->lcd_thread;
	int ret;
	
	/*
	 * Check that calling thread is an lcd thread
	 */
	if (!lcd_thread)
		return;
	/*
	 * Check if someone told us to stop. Call to api exit
	 * never returns.
	 */
	if (lcd_thread->should_stop)
		handle_api_exit(lcd_thread);
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
		/* Need to return immediately if thread in type 3 lcd just
		 * exited. */
		ret = lcd_send(lcd_thread);
		if (ret == 1)
			return;
		else
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

	/*
	 * Check if someone told us to stop (after we possibly woke up
	 * from sleeping in an rvp queue). Call to api exit
	 * never returns.
	 */
	if (lcd_thread->should_stop)
		handle_api_exit(lcd_thread);

	return;
}


/* IOCTL FOR MODPROBE -------------------------------------------------- */

/**
 * Does insmod syscall on behalf of user code call to ioctl.
 */
static int microkernel_init_module(void __user *_mi)
{
	int r;
	struct lcd_prototype_module_args mi;
	
	/*
	 * Grab info
	 */
	r = copy_from_user(&mi, _mi, sizeof(mi));
	if (r) {
		LCD_ERR("copy_from_user");
		return r;
	}

	/*
	 * Init module (for_lcd = 1)
	 */
	return do_sys_init_module(mi.module_image, mi.len, 
				mi.param_values, 1);
}

static long microkernel_dev_ioctl(struct file *filp,
				unsigned int ioctl, unsigned long arg)
{
	long ret = -EINVAL;

	switch (ioctl) {
	case LCD_PROTOTYPE_INIT_MODULE:
		ret = microkernel_init_module((void __user *)arg);
		goto out;
	default:
		return -ENOTTY;
	}

out:
	return ret;
}


static const struct file_operations microkernel_chardev_ops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl	= microkernel_dev_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= microkernel_dev_ioctl,
#endif
	.llseek		= noop_llseek,
};

static struct miscdevice microkernel_dev = {
	LCD_PROTOTYPE_MINOR,
	"lcd-prototype",
	&microkernel_chardev_ops,
};


int __init microkernel_init(void)
{
	int ret;
	LCD_MSG("microkernel starting");
	/*
	 * Set up ioctl for patched modprobe
	 */
	ret = misc_register(&prototype_dev);
	if (ret) {
		LCD_ERR("registering microkernel char device interface");
		return -EIO;
	}
	
	return 0;
}

void __exit microkernel_exit(void)
{
	LCD_MSG("microkernel exiting");
	return;
}

module_init(microkernel_init);
module_exit(microkernel_exit);

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(lcd_int);
EXPORT_SYMBOL(lcd_create_self);
EXPORT_SYMBOL(lcd_enter);
EXPORT_SYMBOL(lcd_exit);
