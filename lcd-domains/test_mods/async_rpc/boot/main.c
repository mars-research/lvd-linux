/*
 * main.c
 *
 * Contains init/exit for rpc test
 */

#include <libfipc.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include "../test_helpers.h"
#include "rpc.h"

#define CALLER_CPU 1
#define CALLEE_CPU 3
#define CHANNEL_ORDER 2 /* channel is 2^CHANNEL_ORDER pages */

MODULE_LICENSE("GPL");

static int setup_and_run_test(void)
{
	int ret;
	struct fipc_ring_channel *caller_header, *callee_header;
	struct task_struct *caller_thread, *callee_thread;
	/*
	 * Initialize fipc
	 */
	ret = fipc_init();
	if (ret) {
		pr_err("Error init'ing libfipc, ret = %d\n", ret);
		goto fail0;
	}
	/*
	 * Set up channel
	 */
	ret = test_fipc_create_channel(CHANNEL_ORDER, &caller_header, 
				&callee_header);
	if (ret) {
		pr_err("Error creating channel, ret = %d\n", ret);
		goto fail1;
	}
	/*
	 * Set up threads
	 */
	caller_thread = test_fipc_spawn_thread_with_channel(caller_header, 
							caller,
							CALLER_CPU);
	if (!caller_thread) {
		pr_err("Error setting up caller thread\n");
		goto fail2;
	}
	callee_thread = test_fipc_spawn_thread_with_channel(callee_header, 
							callee,
							CALLEE_CPU);
	if (!callee_thread) {
		pr_err("Error setting up callee thread\n");
		goto fail3;
	}
	/*
	 * Wake them up; they will run until they exit.
	 */
	wake_up_process(caller_thread);
	wake_up_process(callee_thread);
	msleep(1000);
	/*
	 * Wait for them to complete, so we can tear things down
	 */
	ret = test_fipc_wait_for_thread(caller_thread);
	if (ret)
		pr_err("Caller returned non-zero exit status %d\n", ret);
	ret = test_fipc_wait_for_thread(callee_thread);
	if (ret)
		pr_err("Callee returned non-zero exit status %d\n", ret);
	/*
	 * Tear things down
	 */
	test_fipc_free_channel(CHANNEL_ORDER, caller_header, callee_header);

	fipc_fini();

	return 0;

fail3:
	test_fipc_release_thread(caller_thread);
fail2:
	test_fipc_free_channel(CHANNEL_ORDER, caller_header, callee_header);
fail1:
	fipc_fini();
fail0:
	return ret;
}

static int __init rpc_init(void)
{
	int ret = 0;

	ret = setup_and_run_test();

        return ret;
}
static void __exit rpc_rmmod(void)
{
	return;
}

module_init(rpc_init);
module_exit(rpc_rmmod);
