/*
 * lcd.c - code for lcd llvm example
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <liblcd/liblcd.h>

#include <lcd_config/post_hook.h>

static int func1(int a, int b)
{
	int c;

	c = a + b;

	return 17;
}

static int func2(void)
{
	return 29;
}

/* These are just arbitrary integer labels that the sender
 * will use to indicate which function they wish to invoke
 * via ipc. */
#define FUNC1 1
#define FUNC2 2

static void loop(cptr_t chnl)
{
	int ret;
	int counter = 0;

	for (;;) {

		counter++;

		/* Invoke receive on ipc channel. */
		ret = lcd_sync_recv(chnl);
		if (ret) {
			LIBLCD_ERR("Receive failed with ret %d, exiting.",
				ret);
			lcd_exit(ret);
		}

		/* Switch on the scalar value in message register r0. */
		switch (lcd_r0()) {

		case FUNC1:
			
			/* Invoke func1. We expect the arguments to
			 * be in message registers r1 and r2. */
			ret = func1(lcd_r1(), lcd_r2());

			/* Store the return value in message register r0. */
			lcd_set_r0(ret);

			/* Reply to sender */
			ret = lcd_sync_reply();
			if (ret) {
				LIBLCD_ERR("Reply failed with ret %d, exiting.",
					ret);
				lcd_exit(ret); /* Never returns */
			}
			
			break;

		case FUNC2:

			/* Invoke func2 */
			ret = func2();

			/* Store the return value in message register r0. */
			lcd_set_r0(ret);

			/* Reply to sender */
			ret = lcd_sync_reply();
			if (ret) {
				LIBLCD_ERR("Reply failed with ret %d, exiting.",
					ret);
				lcd_exit(ret); /* Never returns */
			}

			break;

		default:
			LIBLCD_ERR("Unrecognized function label %d, exiting.",
				lcd_r0());
			lcd_exit(-EINVAL); /* Never returns. */
		}

		/* HACK: After two messages, we just quit. */
		if (counter >= 2)
			lcd_exit(0); /* Never returns. */
	}
}

/* INIT/EXIT -------------------------------------------------- */

/* llvm_example_lcd_init is the entry point for the isolated code (i.e.,
 * it is the first bit of code invoked when the isolated code starts
 * running inside a container).
 *
 * llvm_example_lcd_exit is never called, and for now, just ignore it.
 *
 * Background on init/exit: Every Linux kernel module usually has an init
 * and an exit. init is invoked when the module is installed, exit invoked
 * when the module is uninstalled. 
 *
 * In this simple example, init just enters a dispatch loop that never
 * returns. */

int __noreturn 
llvm_example_lcd_init(void) 
{
	int ret = 0;
	cptr_t chnl;

	/* lcd_enter: This gives a chance for the execution environment
	 * to bootstrap itself. In the real environment, lcd_enter
	 * will bootstrap memory allocators and other subsystems *inside*
	 * the container. */
	ret = lcd_enter();
	if (ret)
		goto out; /* Fail */
	
	/* lcd_get_boot_info: Returns a struct that the container uses
	 * to bootstrap itself. Whoever creates this container is supposed
	 * to set up this bootstrap struct. (Yes this is ugly, but it's
	 * reality.) One of the things this struct contains is an array
	 * of (cptrs to) capabilities that the creator granted this container.
	 *
	 * In this simple example, we expect a capability to an ipc channel
	 * in the first slot of the array. */
	chnl = lcd_get_boot_info()->cptrs[0];
	
	loop(chnl); /* Never returns */

out:
	/* lcd_exit: This is similar to a user-level exit system call. It
	 * doesn't return. */
	lcd_exit(ret);
}

void 
llvm_example_lcd_exit(void)
{
	/* No-op, and in this simple example, never called. */
	return;
}

module_init(llvm_example_lcd_init);
module_exit(llvm_example_lcd_exit);

