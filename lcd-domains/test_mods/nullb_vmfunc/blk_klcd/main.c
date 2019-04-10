/*
 * main.c - module init/exit for vfs klcd
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <thc.h>

#include "nullb_callee.h"

#include <lcd_config/post_hook.h>

#include "../benchmark.h"
#include <linux/kthread.h>
/* Don't use e.g. 0.5 * HZ. This will use floating point instructions.
 * I thik floating point in general is bad in the kernel. But it's
 * especially bad with thc/async. */
#define VFS_REGISTER_FREQ (50)

//INIT_MARKER(disp_loop);
INIT_BENCHMARK_DATA(disp_loop);
//static DEFINE_SPINLOCK(td_lock);

static LIST_HEAD(drv_infos);
int pmfs_ready;
struct cspace *klcd_cspace;

/*drv_infos is a global list that has a list of drivers registered to it
 * ch_grp is another list internal to a particular drv_info that has a list
 * of channels to poll on */
struct drv_info * 
add_drv(struct thc_channel_group_item *curr_item, struct glue_cspace *cspace,
	cptr_t sync_endpoint, struct thc_channel *aux_chnl)
{
	struct thc_channel_group *ch_grp;
	struct drv_info *drv_info;
	
	drv_info = kmalloc(sizeof(*drv_info), GFP_KERNEL);
	if (!drv_info)
		goto fail1;

	/* TODO Move this out of this func */
	ch_grp = kzalloc(sizeof(*ch_grp), GFP_KERNEL);
	if(!ch_grp) {
		goto fail2;
	}

	init_chnl_group(ch_grp);
	
	drv_info->aux_chnl = aux_chnl;
	drv_info->cspace = cspace;
	drv_info->ch_grp = ch_grp;
	drv_info->sync_endpoint = sync_endpoint;
	INIT_LIST_HEAD(&drv_info->list);
	list_add(&drv_info->list, &drv_infos);

	return drv_info;

fail2:
	kfree(drv_info);
fail1:
	return NULL;
}

/* TODO add a parameter to distinguish various elements */
struct drv_info * get_drvinfo(void)
{
	struct drv_info *info;
	list_for_each_entry(info, &drv_infos, list);
	return info;
}

void remove_drv(struct drv_info *drv)
{
	LIBLCD_MSG("remove drv called");
	list_del_init(&drv->list);
	kfree(drv);
}

/* Routines for channel group manipulation */
void init_chnl_group(struct thc_channel_group *ch_grp)
{
        INIT_LIST_HEAD(&ch_grp->head);
        ch_grp->size = 0;
}

void add_chnl_group_item(struct thc_channel_group_item *item, 
			struct thc_channel_group *ch_grp)
{
	list_add(&item->list, &ch_grp->head);
	ch_grp->size++;
}

void remove_chnl_group_item(struct thc_channel_group_item *item)
{
	list_del_init(&item->list);
	kfree(item);
}

void del_chnl_group_list(int chnl_id, struct thc_channel_group *ch_grp)
{
	struct thc_channel_group_item *item, *next;

	list_for_each_entry_safe(item, next, &ch_grp->head, list) {

		if(item) {
			kfree(item);
		}	
	}

	kfree(ch_grp);
}

/* LOOP ------------------------------------------------------------ */

static int async_loop(struct drv_info **drv_out, struct thc_channel_group_item **curr_item,
			struct fipc_message **msg_out)
{
	struct drv_info *cursor, *next;
	int ret;

	list_for_each_entry_safe(cursor, next, &drv_infos, list) {

		//ret = thc_ipc_poll_recv(cursor->chnl, msg_out);
		ret = thc_poll_recv_group(cursor->ch_grp, curr_item, msg_out);
		//spin_lock(&td_lock);
		//ret = thc_poll_recv_group_klcd(cursor->ch_grp, curr_item, msg_out);
		//spin_unlock(&td_lock);
		if (ret == -EPIPE) {
			/*
			 * drv channel is dead; free the channel,
			 * and remove from list
			 */
			LIBLCD_MSG("channel dead");
			kfree((*curr_item)->channel);
			remove_chnl_group_item(*curr_item);
			remove_drv(cursor);
		} else if (ret == -EWOULDBLOCK) {
			/*
			 * Skip over empty channels
			 */
			continue;
		} else if (ret) {
			/*
			 * Unexpected error
			 */
			LIBLCD_ERR("error ret = %d on async channel");
			return ret;
		} else {
			/*
			 * Got a msg
			 */
			*drv_out = cursor;
			return 0;
		}

	}
	/*
	 * Nothing for us to recv right now
	 */
	return -EWOULDBLOCK;
}

/*
static int async_aux_loop(struct drv_info **drv_out, struct fipc_message **msg_out)
{
	struct drv_info *cursor, *next;
	int ret;

	list_for_each_entry_safe(cursor, next, &drv_infos, list) {

		ret = thc_ipc_poll_recv(cursor->aux_chnl, msg_out);
		//spin_lock(&td_lock);
		//spin_unlock(&td_lock);
		if (ret == -EPIPE) {
			
			 // drv channel is dead; free the channel,
			 // and remove from list
			 
			LIBLCD_MSG("channel dead");
			kfree(cursor->aux_chnl);
			//remove_drv(cursor);
		} else if (ret == -EWOULDBLOCK) {
			
			 //Skip over empty channels
			 
			continue;
		} else if (ret) {
			
			 //Unexpected error
			 
			LIBLCD_ERR("error ret = %d on async channel");
			return ret;
		} else {
			
			 //Got a msg
			 
			*drv_out = cursor;
			return 0;
		}

	}
	
	 //Nothing for us to recv right now
	 
	return -EWOULDBLOCK;
} */

static int do_one_register(cptr_t register_chnl)
{
	int ret;
	cptr_t sync_endpoint, tx, rx;
	//cptr_t tx_aux, rx_aux;

	/*
	 * Set up cptrs
	 */
	ret = lcd_cptr_alloc(&sync_endpoint);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail1;
	}
	ret = lcd_cptr_alloc(&tx);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail2;
	}
	ret = lcd_cptr_alloc(&rx);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail3;
	}

	//ret = lcd_cptr_alloc(&tx_aux);
	//if (ret) {
	//	LIBLCD_ERR("cptr alloc failed");
	//	goto fail4;
	//}
	//ret = lcd_cptr_alloc(&rx_aux);
	//if (ret) {
	//	LIBLCD_ERR("cptr alloc failed");
	//	goto fail5;
	//}

	/*
	 * Set up regs and poll
	 */
	lcd_set_cr0(sync_endpoint);
	lcd_set_cr1(tx);
	lcd_set_cr2(rx);

	//lcd_set_cr3(tx_aux);
	//lcd_set_cr4(rx_aux);

	//printk("polling for sync recv...\n");
	ret = lcd_sync_poll_recv(register_chnl);
	if (ret) {
		//if (ret == -EWOULDBLOCK) {
			//printk("EWOULDBLOCK \n");
			//ret = 0;
		//}
		goto free_cptrs;
	}
	/*
	 * Dispatch to register handler
	 */
	ret = dispatch_sync_loop();
	if (ret)
		return ret; /* dispatch fn is responsible for cptr cleanup */

	return 0;

free_cptrs:
	lcd_set_cr0(CAP_CPTR_NULL);
	lcd_set_cr1(CAP_CPTR_NULL);
	lcd_set_cr2(CAP_CPTR_NULL);
	//lcd_set_cr3(CAP_CPTR_NULL);
	//lcd_set_cr4(CAP_CPTR_NULL);
	//lcd_cptr_free(rx_aux);
//fail5:
//	lcd_cptr_free(tx_aux);
//fail4:
	lcd_cptr_free(rx);
fail3:
	lcd_cptr_free(tx);
fail2:
	lcd_cptr_free(sync_endpoint);
fail1:
	return ret;
}
/*
static __always_inline void bench_end(void) {
	BENCH_END(disp_loop);
	return;
}

static __always_inline void bench_start(void) {
	BENCH_BEGIN(disp_loop);
	return;
} */

/*
static __always_inline void marker_begin(void) {
	MARKER_BEGIN(disp_loop);
	return;
}*/

static void handle_loop(long id)
{
	struct fipc_message *msg;
	struct thc_channel_group_item *curr_item;
	struct drv_info *drv;
	int stop = 0;
	int ret;
	//static int count = 0;

	DO_FINISH(

		while (!stop) {

			if (stop)
				break;
		
			ret = async_loop(&drv, &curr_item, &msg);
			if (!ret) {
				//(async_msg_get_fn_type(msg) == BLK_MQ_END_REQUEST) ? count ++ : -1;
				//(async_msg_get_fn_type(msg) == BLK_MQ_START_REQUEST) ? bench_start() : -1;
				//((id == 0) && (async_msg_get_fn_type(msg) == BLK_MQ_END_REQUEST)) ? marker_begin() : -1;
				//((id == 0xab) && (async_msg_get_fn_type(msg) == BLK_MQ_END_REQUEST)) ? bench_end() : -1;
				ASYNC(
				

					ret = dispatch_async_loop(
						curr_item->channel, 
						msg,
						drv->cspace,
						drv->sync_endpoint);
					if (ret) {
						LIBLCD_ERR("drv dispatch err");
						/* (break won't work here) */
						stop = 1;
					}
					);
			//((async_msg_get_fn_type(msg) == BLK_MQ_END_REQUEST)) ? bench_end() : -1;
			} else if (ret != -EWOULDBLOCK) {
				LIBLCD_ERR("async loop failed");
				stop = 1;
				break;
			}

			if (kthread_should_stop()) {
				LIBLCD_ERR("kthread should stop");
				stop = 1;
				if(id == 0) {
					blk_exit(curr_item->channel);
				}
				break;
			}

#ifndef CONFIG_PREEMPT
			/*
			 * Play nice with the rest of the system
			 */
			cond_resched();
#endif
		}
		
		if(id == 0) {
			BENCH_COMPUTE_STAT(disp_loop);
			//MARKER_DUMP(disp_loop);
			//printk("main klcd count --> %d \n",count);
		}
		LIBLCD_MSG("blk exited loop, calling blk_exit");

		);

}

/*
static void handle_aux_loop(long id)
{
	struct fipc_message *msg;
	struct drv_info *drv;
	int stop = 0;
	int ret;
	static int cnt_aux = 0;

	DO_FINISH(

		while (!stop) {

			if (stop)
				break;
		
			//BENCH_BEGIN(disp_loop);
			ret = async_aux_loop(&drv, &msg);
			if (!ret) {
				//((id == 0) && (async_msg_get_fn_type(msg) == BLK_MQ_END_REQUEST)) ? marker_begin() : -1;
				(async_msg_get_fn_type(msg) == BLK_MQ_END_REQUEST) ? cnt_aux ++ : -1;
				//((id == 0xab) && (async_msg_get_fn_type(msg) == BLK_MQ_END_REQUEST)) ? bench_end() : -1;
				ASYNC(
					ret = dispatch_async_loop(
						drv->aux_chnl, 
						msg,
						drv->cspace,
						drv->sync_endpoint);
					if (ret) {
						LIBLCD_ERR("drv dispatch err");
						// (break won't work here) 
						stop = 1;
					}
					);
			} else if (ret != -EWOULDBLOCK) {
				LIBLCD_ERR("async loop failed");
				stop = 1;
				break;
			}

			if (kthread_should_stop()) {
				LIBLCD_ERR("kthread should stop");
				stop = 1;
				break;
			}

#ifndef CONFIG_PREEMPT
			
			 // Play nice with the rest of the system
			 
			cond_resched();
#endif
		}
		
		if(id == 0xab) {
			printk("aux klcd count --> %d \n",cnt_aux);
			//BENCH_COMPUTE_STAT(disp_loop);
			//MARKER_DUMP(disp_loop);
		}
		LIBLCD_MSG("blk exited loop, calling blk_exit");
		THCStopAllAwes();

		);

}
*/

int klcd_task(void *data) {
	
	long id = 0xAB;

	lcd_enter();
	
	LCD_MAIN({
		//handle_aux_loop(id);
		handle_loop(id);
	});
	
	lcd_exit(0);
	return 0;
} 

static void loop(cptr_t register_chnl)
{
	unsigned long tics = jiffies + VFS_REGISTER_FREQ;
	//struct fipc_message *msg;
	//struct thc_channel_group_item *curr_item;
	//struct drv_info *drv;
	int stop = 0;
	int ret;
	//struct task_struct *sub_task = NULL;

	while(!stop) {
		if (jiffies >= tics) {
			ret = do_one_register(register_chnl);
			if (ret == 0) {
				LIBLCD_MSG("registered driver successfully \n");
				break;
			}
			else if (ret == -EWOULDBLOCK) {
				tics = jiffies + VFS_REGISTER_FREQ;
				continue;
			}
			else {
				LIBLCD_ERR("register error");
				break;
			}
		}
	}
	
	//sub_task = kthread_create(klcd_task, NULL, "klcd_task");
	//if (IS_ERR(sub_task)) {
	//	ret = PTR_ERR(sub_task);
	//	sub_task = NULL;
	//	printk("[KLCD] thread not created, continuing with a single thread \n");;
	//}
	
	//wake_up_process(sub_task);
	
	handle_loop(0);
	//kthread_stop(sub_task);
/*	
	DO_FINISH(

		while (!stop) {

			//printk("entry -> ticks: %ld jiffies: %ld \n",tics, jiffies);
			//if (jiffies >= tics) {
				
				//Listen for a register call
			//	ret = do_one_register(register_chnl);
			//	if (ret) {
			//		LIBLCD_ERR("register error");
			//		break;
			//	} 
				//printk("register --> ticks: %ld jiffies: %ld \n",tics, jiffies);
			//	tics = jiffies + VFS_REGISTER_FREQ;
			//	continue;
			//}

			if (stop)
				break;
			//BENCH_BEGIN(disp_loop);
			ret = async_loop(&drv, &curr_item, &msg);
			if (!ret) {
				//(async_msg_get_fn_type(msg) == BLK_MQ_END_REQUEST) ? bench_end() : -1;
				ASYNC(
					ret = dispatch_async_loop(
						curr_item->channel, 
						msg,
						drv->cspace,
						drv->sync_endpoint);
					if (ret) {
						LIBLCD_ERR("drv dispatch err");
						//(break won't work here)
						stop = 1;
					}
					);
			} else if (ret != -EWOULDBLOCK) {
				LIBLCD_ERR("async loop failed");
				stop = 1;
				break;
			}

			if (kthread_should_stop()) {
				LIBLCD_ERR("kthread should stop");
				stop = 1;
				blk_exit(curr_item->channel);
				break;
			}

#ifndef CONFIG_PREEMPT
			
			//Play nice with the rest of the system
			 
			cond_resched();
#endif
		}
		//BENCH_COMPUTE_STAT(disp_loop);
		LIBLCD_MSG("blk exited loop, calling blk_exit");
		THCStopAllAwes();

		);
*/
	/* 
	 * NOTE: If the vfs klcd quits / is killed before 
	 * unregister_filesystem runs, it could cause some proc fs
	 * crap to crash (the struct file_system_type is still in
	 * the registered fs list, but e.g. the const char *name just
	 * went bye-bye when we unloaded the vfs's .ko.)
	 */

//done:		
	LIBLCD_MSG("EXITED BLK DO_FINISH");

	return;
}

/* INIT / EXIT ---------------------------------------- */

static int blk_klcd_init(void) 
{
	int ret;
	cptr_t blk_chnl;
	/*
	 * Set up cptr cache, etc.
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter");
		goto fail1;
	}
	/*
	 * XXX: Hack: boot provided us with one cptr for the vfs channel
	 */
	ret = lcd_cptr_alloc(&blk_chnl);
	if (ret) {
		LIBLCD_ERR("alloc cptr");
		goto fail2;
	}

	klcd_cspace = get_current_cspace(current);
	/*
	 * Init blk glue
	 */
	ret = glue_blk_init();
	if (ret) {
		LIBLCD_ERR("vfs init");
		goto fail3;
	}
	/*
	 * Enter sync/async dispatch loop
	 */
	loop(blk_chnl);
	/*
	 * Tear down blk glue
	 */
	glue_blk_exit();

	lcd_exit(0);
	
	return 0;

fail3:
fail2:
	lcd_exit(ret);
fail1:
	return ret;
}

static int _blk_klcd_init(void)
{
	int ret;

	LCD_MAIN({

			ret = blk_klcd_init();

		});

	return ret;
}

/* 
 * make module loader happy (so we can unload). we don't actually call
 * this before unloading the lcd (yet)
 */
static void __exit _blk_klcd_exit(void)
{
	return;
}

module_init(_blk_klcd_init);
module_exit(_blk_klcd_exit);
MODULE_LICENSE("GPL");
