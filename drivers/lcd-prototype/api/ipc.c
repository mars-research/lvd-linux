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
	for (i = 0; i < from->max_valid_reg_idx; i++)
		to->regs[i] = from->regs[i];
}

static void copy_msg_cap(struct lcd *from, struct lcd *to,
			cptr_t from_ptr, cptr_t to_ptr)
{
	int ret;

	ret = lcd_cnode_grant(from->cspace, to->cspace, from_ptr, to_ptr,
			LCD_CAP_ALL_RIGHTS);
	if (ret) {
		LCD_ERR("failed to transfer cap @ %d in lcd %p to slot @ %d in lcd %p",
			from_ptr, from, to_ptr, to);
	}
}

static void copy_msg_caps(struct lcd *from, struct lcd *to)
{
	int i;
	for (i = 0; i < from->max_valid_out_cap_reg_idx &&
		     i < to->max_valid_in_cap_reg_idx; i++)
		copy_msg_cap(from, to, from->out_cap_regs[i],
			to->in_cap_regs[i]);
}

static void copy_call_endpoint(struct lcd *from, struct lcd *to)
{
	copy_msg_cap(from, to, from->utcb.call_endpoint_cap,
		to->utcb.reply_endpoint_cap);
	return;	
}

static void transfer_msg(struct sync_endpoint_proxy *from_proxy,
			struct sync_endpoint_proxy *to_proxy, 
			int making_call)
{
	struct lcd *from;
	struct lcd *to;
	
	from = from_proxy->parent;
	to = to_proxy->parent;
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
	
	return; 
}

static int lcd_do_send(struct lcd *lcd, cptr_t c, int making_call)
{
	int ret;
	struct sync_endpoint_proxy *from_proxy;
	struct sync_endpoing_proxy *to_proxy;
	struct sync_endpoint *e;
	struct cnode *cnode;
	
	/*
	 * Lookup cnode
	 */
	ret = __lcd_lookup_cnode(lcd->cspace, c, &cnode);
	if (ret)
		goto fail1;
	/*
	 * Confirm type and permissions
	 */
	if (!__lcd_cnode_is_sync_ep(cnode) || !__lcd_cnode_can_write(cnode)) {
		ret = -EACCES;
		goto fail1;
	}
	/*
	 * Get synch end point proxy and end point
	 */
	from_proxy = __lcd_cnode_object(cnode);
	e = from_proxy->endpoint;
	/*
	 * Check if any recvr waiting, and do immediate send, wake up
	 * recvr
	 *
	 * (Locking endpoint is probably pointless for now since
	 * it's used primarily when cap is locked.)
	 */
	mutex_lock(&e->lock);
	if (list_empty(&e->receivers)) {
		/*
		 * No one receiving; put myself in e's sender list and lcd's
		 * sending eps list.
		 *
		 * XXX: maybe we want per-lcd lock?
		 */
		list_add_tail(&from_proxy->lcd_proxy_list, &lcd->sending_eps);
		list_add_tail(&from_proxy->endpoint_proxy_list, &e->senders);
		/*
		 * Mark myself as making a call, if necessary, so that recvr
		 * knows it needs to copy reply rendezvous point cap.
		 */
		lcd->making_call = making_call;
		/*
		 * IMPORTANT: Must unlock cap before going to sleep.
		 */
		mutex_unlock(&e->unlock());
		lcd_cap_unlock();

		set_current_state(TASK_INTERRUPTIBLE);
		schedule();		

		/*
		 * Someone woke me up; re-lock cap. Receiver should transfer
		 * message, so we just return.
		 *
		 * Reset making_call var.
		 */
		lcd->making_call = 0;
		lcd_cap_lock();

		return 0;
	}

	/*
	 * Otherwise, someone waiting to receive. Remove from
	 * receiving queue.
	 */
	to_proxy = list_first_entry(&e->receivers, struct sync_endpoint_proxy,
				endpoint_proxy_list);
        list_del_init(&to_proxy->endpoint_proxy_list);
        list_del_init(&to_proxy->lcd_proxy_list);
	mutex_unlock(&e->lock);

	/*
	 * XXX: for now, unlock cap (transfer_msg needs cap unlocked, since
	 * it eventually calls grant)
	 */
	lcd_cap_unlock();

	/*
	 * Send message, and wake up lcd
	 */
	transfer_msg(from_proxy, to_proxy, making_call);

	lcd_cap_lock();

	wake_up_process(to_proxy->parent->parent);

	return 0;
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

	ret = lcd_do_send(lcd, c);

	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();

	return ret;
}

static int lcd_do_recv(struct lcd *lcd, cptr_t c)
{
	int ret;
	struct sync_endpoint_proxy *from_proxy;
	struct sync_endpoing_proxy *to_proxy;
	struct sync_endpoint *e;
	struct cnode *cnode;
	
	/*
	 * Lookup cnode
	 */
	ret = __lcd_lookup_cnode(lcd->cspace, c, &cnode);
	if (ret)
		goto fail1;
	/*
	 * Confirm type and permissions
	 */
	if (!__lcd_cnode_is_sync_ep(cnode) || !__lcd_cnode_can_read(cnode)) {
		ret = -EACCES;
		goto fail1;
	}
	/*
	 * Get synch end point proxy and end point
	 */
	to_proxy = __lcd_cnode_object(cnode);
	e = to_proxy->endpoint;
	/*
	 * Check if any sender waiting, and do immediate recv, wake up
	 * sender
	 *
	 * (Locking endpoint is probably pointless for now since
	 * it's used primarily when cap is locked.)
	 */
	mutex_lock(&e->lock);
	if (list_empty(&e->receivers)) {
		/*
		 * No one sending; put myself in e's recvr list and lcd's
		 * receiving eps list.
		 *
		 * XXX: maybe we want per-lcd lock?
		 */
		list_add_tail(&to_proxy->lcd_proxy_list, &lcd->recving_eps);
		list_add_tail(&to_proxy->endpoint_proxy_list, &e->receivers);
		/*
		 *
		 * IMPORTANT: Must unlock cap before going to sleep.
		 */
		mutex_unlock(&e->unlock());
		lcd_cap_unlock();

		set_current_state(TASK_INTERRUPTIBLE);
		schedule();		

		/*
		 * Someone woke me up; re-lock cap. Receiver should transfer
		 * message, so we just return.
		 */
		lcd_cap_lock();
		return 0;
	}

	/*
	 * Otherwise, someone waiting to receive. Remove from
	 * sending queue.
	 */
	from_proxy = list_first_entry(&e->senders, struct sync_endpoint_proxy,
				endpoint_proxy_list);
        list_del_init(&from_proxy->endpoint_proxy_list);
        list_del_init(&from_proxy->lcd_proxy_list);
	mutex_unlock(&e->lock);

	/*
	 * XXX: for now, unlock cap (transfer_msg needs cap unlocked, since
	 * it eventually calls grant)
	 */
	lcd_cap_unlock();

	/*
	 * Send message, and wake up lcd
	 */
	transfer_msg(from_proxy, to_proxy, from_proxy->parent->making_call);

	lcd_cap_lock();

	wake_up_process(to_proxy->parent->parent);

	return 0;
fail1:
	return ret;
}

int __lcd_recv(struct lcd *lcd, cptr_t c)
{
	int ret;
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

int __lcd_call(struct lcd *lcd, cptr_t c)
{
	int ret;
	/*
	 * LOCK cap
	 */
	ret = lcd_cap_lock();
	if (ret)
		return ret;

	ret = lcd_do_send(lcd, c, 1);

	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();

	return ret;
}

int __lcd_reply(struct lcd *lcd)
{
	int ret;
	/*
	 * LOCK cap
	 */
	ret = lcd_cap_lock();
	if (ret)
		return ret;

	ret = lcd_do_send(lcd, lcd->reply_endpoint_cap, 0);

	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();

	return ret;
}

int __lcd_select(struct task_struct *from, cptr_t *cs, int cs_count,
		cptr_t *c_out)
{
	return -ENOSYS;
}

int lcd_mk_sync_endpoint(struct lcd *lcd, cptr_t c)
{
	struct sync_endpoint *e;
	struct sync_endpoint_proxy *proxy;
	struct cnode *cnode;
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
	INIT_LIST_HEAD(&e->proxies);
	mutex_init(&e->lock);
	/*
	 * Allocate end point proxy
	 */
	proxy = kmalloc(sizeof(*proxy), GFP_KERNEL);
	if (!proxy) {
		ret = -ENOMEM;
		goto fail2;
	}
	/*
	 * Initialize proxy
	 */
	INIT_LIST_HEAD(&proxy->lcd_proxy_list);
	INIT_LIST_HEAD(&proxy->endpoint_proxy_list);
	INIT_LIST_HEAD(&proxy->proxies);
	proxy->parent = lcd;
	proxy->ep = e;
	/*
	 * Add proxy to end point
	 */
	list_add(&proxy->proxies, &e->proxies);
	/*
	 * Insert endpoint proxy into cspace at c
	 */
	ret = lcd_cnode_insert(lcd->cspace, c, proxy, LCD_CAP_TYPE_SYNC_EP,
			LCD_CAP_RIGHT_ALL);
	if (ret) {
		LCD_ERR("insert endpoint");
		goto fail3;
	}
	return 0;

fail3:
	kfree(proxy);
fail2:
	kfree(e);
fail1:
	return ret;
}

static void __cleanup_sync_endpoint_proxies(struct sync_endpoint *e)
{
	struct list_head *ptr;
	struct list_head *n;
	struct sync_endpoint_proxy *proxy;

	list_for_each_safe(ptr, n, &e->proxies) {
		proxy = list_entry(ptr, struct sync_endpoint_proxy, proxies);
		/*
		 * Remove proxy from parent lcd's list (send or recv)
		 */
		list_del(&proxy->lcd_active_list);
		/*
		 * Remove proxy from end point's list (send or recv)
		 */
		list_del(&proxy->endpoint_active_list);
		/*
		 * Remove proxy from all of end point's proxies 
		 * (active/inactive)
		 */
		list_del(&proxy->proxies);
		/*
		 * Free mem
		 */
		kfree(proxy);
	}
}

static int __cleanup_sync_endpoint(struct cnode *cnode)
{
	struct sync_endpoint_proxy *proxy;
	struct sync_endpoint *e;
	/*
	 * Check that cnode contains sync ep (proxy), and lcd is owner
	 */
	if (!__lcd_cnode_is_sync_ep(cnode))
		return -EINVAL;
	if (!__lcd_cnode_is_owner(cnode))
		return -EINVAL;

	proxy = __lcd_cnode_object(cnode);
	e = proxy->endpoint;

	/*
	 * Remove from cspaces first, so no one can refer to sync ep (via
	 * a proxy)
	 */
	__lcd_cnode_free(cnode);

	/*
	 * Remove all proxies (proxy var invalid after this point!)
	 */
	__cleanup_sync_endpoint_proxies(e);

	/*
	 * Free end point
	 */
	kfree(e);

	return 0;
}

static int __lcd_rm_sync_endpoint(struct lcd *lcd, cptr_t cptr)
{
	int ret;
	/*
	 * Look up cnode
	 */
	ret = __lcd_cnode_lookup(lcd->cspace, c, &cnode);
	if (ret) {
		LCD_ERR("cnode lookup at %lld", c);
		return ret;
	}
	/*
	 * Remove it from lcd's cspace, and recursively remove from
	 * any cspaces with derivations
	 */
	return __cleanup_sync_endpoint(cnode);
}

int lcd_rm_sync_endpoint(struct lcd *lcd, cptr_t cptr)
{
	struct cnode *cnode;
	int ret;
	/*
	 * LOCK cap
	 */
	ret = lcd_cap_lock();
	if (ret)
		return ret;
	ret = __lcd_rm_sync_endpoint(lcd, cptr);
	/*
	 * UNLOCK cap
	 */
	lcd_cap_unlock();
	return ret;
}

int ipc_reply(capability_t cap, struct message_info *msg)
{
	return ipc_send(cap, msg);
}
EXPORT_SYMBOL(ipc_reply);

int ipc_call(capability_t cap, struct message_info *msg)
{
	int ret; 

	// The last capability register is expected to 
	// have the reply capability
	if (msg->valid_cap_regs == 0) 
		return -EINVAL; 

	ret = ipc_send(cap, msg);
	if (ret)
		return ret;
	
	// The last capability register is expected to 
	// have the reply capability
	ret = ipc_recv(msg->cap_regs[msg->valid_cap_regs - 1], msg); 
	return ret; 
}
EXPORT_SYMBOL(ipc_call);
