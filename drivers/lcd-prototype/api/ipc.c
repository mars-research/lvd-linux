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

int __lcd_send(struct lcd *lcd, cptr_t c)
{
	
	return -ENOSYS;
}

int __lcd_recv(struct task_struct *to, cptr_t c)
{
	return -ENOSYS;
}

int __lcd_call(struct task_struct *from, cptr_t c)
{
	return -ENOSYS;
}

int __lcd_select(struct task_struct *from, cptr_t *cs, int cs_count,
		cptr_t *c_out)
{
	return -ENOSYS;
}

int lcd_mk_sync_endpoint(struct lcd *lcd, cptr_t c)
{
	struct sync_endpoint *e;
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
	 * Initialize data
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

inline void transfer_msg(struct task_struct *to, struct task_struct *from) {

	printk(KERN_INFO "Sending %d registers (recv ready to take %d)\n", 
			from->utcb->msg_info.valid_regs, to->utcb->msg_info.valid_regs);
	// copy the message registers
	// XXX: BU: maybe MIN(of valid_regs)?
	memcpy(&to->utcb->msg_info.regs, 
		&from->utcb->msg_info.regs, 
		sizeof(uint64_t)*to->utcb->msg_info.valid_regs);

	// BU: TODO: transfer capabilities
	//
	
	// Transfer err code
	to->utcb->msg_info.err =  from->utcb->msg_info.err; 
	return; 
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


int ipc_send(capability_t rvp_cap, struct message_info *msg)
{
	struct task_struct *recv_task;
	struct sync_ipc *sync_ipc;
	struct cnode *cnode;
	unsigned long flags;

	printk(KERN_ERR "ipc_send:%s: sending on cap %lld\n", current->comm, rvp_cap);
	
        cnode = lcd_cnode_lookup(&current->cspace, rvp_cap);
	if (cnode == NULL || cnode->type != LCD_TYPE_SYNC_EP) {
		printk(KERN_ERR "ipc_send: can't resolve rendezvous capabilty: %lld\n", rvp_cap);
		return -EINVAL;   
	}

	sync_ipc = (struct sync_ipc *) cnode->object;
	
	BUG_ON(!sync_ipc); 

	// XXX: BU: Maybe I need to do some reference counting for IPC 
	// objects here (before releasing the lock)
	lcd_cnode_release(cnode);

	spin_lock_irqsave(&sync_ipc->lock, flags); 	
	if (list_empty(&sync_ipc->receivers)) {

		set_current_state(TASK_INTERRUPTIBLE);
		list_add_tail(&current->sync_rendezvous, &sync_ipc->senders);
		
		printk(KERN_ERR "ipc_send:%s: putting myself to sleep\n", current->comm);
		
		spin_unlock_irqrestore(&sync_ipc->lock, flags);
		schedule();
		printk(KERN_ERR "ipc_send: someone woke me up\n");
		return 0; 

	}

	recv_task = list_first_entry(&sync_ipc->receivers, 
					struct task_struct, 
					sync_rendezvous);

        list_del(&recv_task->sync_rendezvous); 
	spin_unlock_irqrestore(&sync_ipc->lock, flags);
	 
	printk(KERN_ERR "ipc_send: found other end %s\n", recv_task->comm);
	
	transfer_msg(recv_task, current); 

	wake_up_process(recv_task); 
	printk(KERN_ERR "ipc_send: finished\n");
	return 0;

}
EXPORT_SYMBOL(ipc_send);

int ipc_recv(capability_t rvp_cap, struct message_info *msg)
{
	struct task_struct *send_task;
	struct sync_ipc *sync_ipc;
	struct cnode *cnode;
	unsigned long flags;

	
	printk(KERN_ERR "ipc_recv:%s: receiving on cap %lld\n", current->comm, rvp_cap);
	
        cnode = lcd_cnode_lookup(&current->cspace, rvp_cap);
	if (cnode == NULL || cnode->type != LCD_TYPE_SYNC_EP) {
		printk(KERN_ERR "ipc_recv: can't resolve capability: %lld\n", rvp_cap);
		return -EINVAL;   
	}

	sync_ipc = (struct sync_ipc *) cnode->object;
	BUG_ON(!sync_ipc); 

	// XXX: BU: Maybe I need to do some reference counting for IPC 
	// objects here (before releasing the lock)
	lcd_cnode_release(cnode);
	
	spin_lock_irqsave(&sync_ipc->lock, flags); 	
	if (list_empty(&sync_ipc->senders)) {

		set_current_state(TASK_INTERRUPTIBLE);
		list_add_tail(&current->sync_rendezvous, &sync_ipc->receivers);
		
		printk(KERN_ERR "ipc_recv:%s: putting myself to sleep\n", current->comm);
		
		spin_unlock_irqrestore(&sync_ipc->lock, flags);
		schedule();		
		printk(KERN_ERR "ipc_recv: someone woke me up\n");
		return 0; 
	}

	send_task = list_first_entry(&sync_ipc->senders,
					struct task_struct, 
					sync_rendezvous);

        list_del(&send_task->sync_rendezvous); 
	spin_unlock_irqrestore(&sync_ipc->lock, flags);
	 
	printk(KERN_ERR "ipc_send: other end %s\n", send_task->comm);

	transfer_msg(current, send_task); 
	
	wake_up_process(send_task); 
	printk(KERN_ERR "ipc_recv: finished\n");
	return 0;

}
EXPORT_SYMBOL(ipc_recv);
