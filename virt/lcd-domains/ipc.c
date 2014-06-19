// Sync IPC specific routines

#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/sched.h>

#include <lcd/cap.h>
#include <lcd/lcd.h>

//#include "lcd_defs.h"

void display_mr(utcb_t *p_utcb) {
	printk(KERN_ERR "Message Regs at utcb %p - %d ,%d , %d\n", p_utcb, p_utcb->mr[0], p_utcb->mr[1], p_utcb->mr[3]);
}


int ipc_send(u32 myself, u32 recv_capid)
{
	struct lcd *recv_lcd, *snd_lcd;
	ipc_wait_list_elem stack_elem;

	printk(KERN_ERR "ipc_send : myself %d reciever %d\n", myself, recv_capid);
	//chk if the reciever is ready
	// fetch the reciever task struct from if
	recv_lcd = (struct lcd *) get_cap_obj(recv_capid);
	if (recv_lcd == NULL) {
		printk(KERN_ERR "ipc_send : Cant get object for reciever %d\n", recv_capid);
		return -1;   
	}

	snd_lcd = (struct lcd *) get_cap_obj(myself);
	if (snd_lcd == NULL) {
		printk(KERN_ERR "ipc_send : Cant get object for myself %d\n", myself);
		return -1;   
	}

	if (recv_lcd->sync_ipc.state == IPC_RCV_WAIT && \
		recv_lcd->sync_ipc.expected_sender == myself) {

		printk(KERN_ERR "ipc_send : partner %d expecting me\n", recv_capid);
		//copy the message registers
		memcpy(recv_lcd->shared, snd_lcd->shared, sizeof(utcb_t));
		//awaken the thread
		wake_up_process(recv_lcd->sync_ipc.task);
    
		//looks like there is no need for a reciever queue
		//as if a process invokes a recv and finds no
		//corresponding senders , then it puts itself to sleep
		recv_lcd->sync_ipc.state = IPC_DONT_CARE;
		recv_lcd->sync_ipc.expected_sender = 0;
		//No case of 

	} else {
		// put him in the Q
		recv_lcd->sync_ipc.snd_sleepers++;
		set_current_state(TASK_INTERRUPTIBLE);
		stack_elem.peer = myself;
		stack_elem.task = current;
		// recv_lcd->sync_ipc.status = IPC_SND_WAIT;
		list_add_tail(&stack_elem.list, &recv_lcd->sync_ipc.snd_q);
		printk(KERN_ERR "ipc_send : putting myself to sleep %p\n", current);

		schedule();

	}

	printk(KERN_ERR "ipc_send : Finished\n");
	return 0;

}
EXPORT_SYMBOL(ipc_send);

int ipc_recv(u32 myself, u32 send_capid) 
{
	struct lcd *recv_lcd, *snd_lcd;
	struct list_head *ptr;
	ipc_wait_list_elem *entry;

	printk(KERN_ERR "ipc_recv : myself %d sender %d\n", myself, send_capid);

	recv_lcd = (struct lcd *) get_cap_obj(myself);
	if (recv_lcd == NULL) {
		printk(KERN_ERR "ipc_recv : Cant get object for my id %d\n", myself);
		return -1;   
	}

	snd_lcd = (struct lcd *) get_cap_obj(send_capid);
	if (snd_lcd == NULL) {
		printk(KERN_ERR "ipc_recv : Cant get object for peer id %d\n", send_capid);
		//return -1;   
	}

	//check if one of the senders in the snd q is our intended
	// recipient
	if (recv_lcd->sync_ipc.snd_sleepers > 0) {

		printk(KERN_ERR "ipc_recv : Num of senders in Q %d \n", \
			recv_lcd->sync_ipc.snd_sleepers);

		list_for_each(ptr, &recv_lcd->sync_ipc.snd_q) {
			entry = list_entry(ptr, ipc_wait_list_elem, list);
			if (entry->peer == send_capid) {
				printk(KERN_ERR "ipc_recv : Found expected sender %d\n", send_capid);

				recv_lcd->sync_ipc.snd_sleepers--;
				//copy the message registers
				memcpy(recv_lcd->shared, snd_lcd->shared, sizeof(utcb_t));
				//remove the entry
				list_del(ptr);
				//wakeup
				wake_up_process(entry->task);
				// we dont care for state in snd_wait
				//recv_lcd->sync_ipc.status = IPC_RUNNING; 
				printk(KERN_ERR "ipc_recv : Returning after waking up sender\n");
				return 0; 
			}
		}
	}
	printk(KERN_ERR "ipc_recv : Scheduling out myself\n");
	// we cant proceed further
	recv_lcd->sync_ipc.state = IPC_RCV_WAIT ;
	recv_lcd->sync_ipc.expected_sender = send_capid;
	set_current_state(TASK_INTERRUPTIBLE);
	schedule();

	printk(KERN_ERR "ipc_recv : Somebody woke me\n");
	return 0;

}
EXPORT_SYMBOL(ipc_recv);
