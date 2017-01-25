

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <thc.h>


#include "./nullnet_callee.h"

/* COMPILER: This is always included after all includes. */
#include <lcd_config/post_hook.h>
/* LOOP ------------------------------------------------------------ */
struct fs_info {
	struct thc_channel *chnl;
	struct glue_cspace *cspace;
	cptr_t sync_endpoint;
	struct list_head list;
};
static LIST_HEAD(fs_infos);

struct fs_info * 
add_fs(struct thc_channel *chnl, struct glue_cspace *cspace,
	cptr_t sync_endpoint)
{
	struct fs_info *fs_info;
	
	fs_info = kmalloc(sizeof(*fs_info), GFP_KERNEL);
	if (!fs_info)
		goto fail1;
	fs_info->chnl = chnl;
	fs_info->cspace = cspace;
	fs_info->sync_endpoint = sync_endpoint;
	INIT_LIST_HEAD(&fs_info->list);
	list_add(&fs_info->list, &fs_infos);

	return fs_info;

fail1:
	return NULL;
}

void remove_fs(struct fs_info *fs)
{
	list_del_init(&fs->list);
	kfree(fs);
}



static int async_loop(struct fs_info **fs_out, struct fipc_message **msg_out)
{
	struct fs_info *cursor, *next;
	int ret;

	list_for_each_entry_safe(cursor, next, &fs_infos, list) {

		ret = thc_ipc_poll_recv(cursor->chnl, msg_out);
		if (ret == -EPIPE) {
			/*
			 * fs channel is dead; free the channel,
			 * and remove from list
			 */
			kfree(cursor->chnl);
			remove_fs(cursor);
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
			*fs_out = cursor;
			return 0;
		}

	}
	/*
	 * Nothing for us to recv right now
	 */
	return -EWOULDBLOCK;
}

/* LOOP -------------------------------------------------- */
static int do_one_register(cptr_t register_chnl)
{
	int ret;
	cptr_t sync_endpoint, tx, rx;
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
	/*
	 * Set up regs and poll
	 */
	lcd_set_cr0(sync_endpoint);
	lcd_set_cr1(tx);
	lcd_set_cr2(rx);
	ret = lcd_sync_poll_recv(register_chnl);
	if (ret) {
		if (ret == -EWOULDBLOCK)
			ret = 0;
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
	lcd_cptr_free(sync_endpoint);
fail3:
	lcd_cptr_free(tx);
fail2:
	lcd_cptr_free(rx);
fail1:
	return ret;
}
#define REGISTER_FREQ	50

static void loop(cptr_t register_chnl)
{
	unsigned long tics = jiffies + REGISTER_FREQ;
	struct fipc_message *msg;
	struct fs_info *fs;
	int stop = 0;
	int ret;

	DO_FINISH(

		while (!stop) {
			//LIBLCD_MSG(" tics %lu\n", tics);
			if (jiffies >= tics) {
				/*
				 * Listen for a register call
				 */
				ret = do_one_register(register_chnl);
				if (ret) {
					LIBLCD_ERR("register error");
					break;
				}
				tics = jiffies + REGISTER_FREQ;
				continue;
			}
			/*
			if (pmfs_ready) {
				pmfs_ready = 0;
				ASYNC(
					stop = do_pmfs_test();
					);
			}
			 */
			if (stop)
				break;
			ret = async_loop(&fs, &msg);
			if (!ret) {
				ASYNC(
					ret = dispatch_async_loop(
						fs->chnl, 
						msg,
						fs->cspace,
						fs->sync_endpoint);
					if (ret) {
						LIBLCD_ERR("fs dispatch err");
						/* (break won't work here) */
						stop = 1;
					}
					);
			} else if (ret != -EWOULDBLOCK) {
				LIBLCD_ERR("async loop failed");
				stop = 1;
				break;
			}

			if (kthread_should_stop()) {
					LIBLCD_MSG("kthread should stop");
				stop = 1;
				break;
			}

#ifndef CONFIG_PREEMPT
			/*
			 * Play nice with the rest of the system
			 */
			cond_resched();
#endif
		}

		LIBLCD_MSG("vfs exited loop");

		THCStopAllAwes();

		);

	/* 
	 * NOTE: If the vfs klcd quits / is killed before 
	 * unregister_filesystem runs, it could cause some proc fs
	 * crap to crash (the struct file_system_type is still in
	 * the registered fs list, but e.g. the const char *name just
	 * went bye-bye when we unloaded the vfs's .ko.)
	 */

	LIBLCD_MSG("EXITED DUMMY DO_FINISH");


}

/* INIT / EXIT ---------------------------------------- */

static int net_klcd_init(void) 
{
	int ret;
	cptr_t net_chnl;
	/*
	 * Set up cptr cache, etc.
	 */
	ret = lcd_enter();
	if (ret) {
		LIBLCD_ERR("lcd enter");
		goto fail1;
	}

	/*
	 * XXX: Hack: boot provided us with one cptr for the net chnl
	 */
	ret = lcd_cptr_alloc(&net_chnl);
	if (ret) {
		LIBLCD_ERR("alloc cptr");
		goto fail2;
	}
	LIBLCD_MSG("==========> got cptr %lu\n", net_chnl.cptr);
	/*
	 * Init net glue
	 */
	ret = glue_nullnet_init();
	LIBLCD_MSG("-===== > glue nullnet init called\n");
	if (ret) {
		LIBLCD_ERR("net init");
		goto fail3;
	}
	/*
	 * Enter sync/async dispatch loop
	 */
	LIBLCD_MSG(">>>>> Looping .... \n");
	loop(net_chnl);
	/*
	 * Tear down net glue
	 */
	glue_nullnet_exit();

	lcd_exit(0);
	
	return 0;

fail3:
fail2:
	lcd_exit(ret);
fail1:
	return ret;
}

static int __net_klcd_init(void)
{
	int ret;
	LIBLCD_MSG("%s: entering", __func__);
	LCD_MAIN({

			ret = net_klcd_init();

		});
	return ret;
}

/* 
 * make module loader happy (so we can unload). we don't actually call
 * this before unloading the lcd (yet)
 */
static void __exit net_klcd_exit(void)
{
	LIBLCD_MSG("%s: exiting", __func__);
	return;
}

module_init(__net_klcd_init);
module_exit(net_klcd_exit);
MODULE_LICENSE("GPL");

