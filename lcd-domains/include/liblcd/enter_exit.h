/*
 * enter_exit.h
 *
 * These functions should be invoked when an LCD/thread starts
 * executing, and before the thread exits (or when the thread
 * exits).
 *
 * Copyright: University of Utah
 */
#ifndef LCD_DOMAINS_ENTER_EXIT_H
#define LCD_DOMAINS_ENTER_EXIT_H

#include <linux/kernel.h>

#ifdef LCD_ISOLATE
#define LCD_MAYBE_NORETURN __noreturn
#else
#define LCD_MAYBE_NORETURN
#endif

/**
 * lcd_enter -- Initializes runtime environment for an LCD
 *
 * No matter if you are isolated or non-isolated, this should be called 
 * before you invoked any other functions in the liblcd interface.
 *
 * IMPORTANT: It is highly recommended that you use the LCD_MAIN macro
 * (see below).
 *
 * Semantics:
 *
 *    -- Isolated code: this bootstraps the environment inside the container
 *    -- Non-isolated code: this switches a non-isolated thread into
 *                          "LCD mode": initializes a cspace and other data 
 *                          structures for the caller, so that it can
 *                          start creating LCDs, doing ipc, etc.
 */
int lcd_enter(void);
/**
 * LCD_MAIN -- Tweaks the stack so that libasync works correctly
 *
 * Here is a usage example:
 *
 *        static int my_main(void)
 *        {
 *                 int ret;
 *
 *                 ret = lcd_enter();
 *
 *                 // Do some things
 *
 *                 lcd_exit(ret);
 *
 *                 return ret; // lcd_exit returns for non-isolated
 *        }
 *
 *        int __init my_module_init(void)
 *        {
 *             int ret;
 *
 *             LCD_MAIN({
 *
 *                     ret = my_main();
 *
 *             });
 *
 *             return ret;
 *        }
 *        module_init(my_module_init);
 *
 * Implementation Note: Isolated code has a 0 return address at the
 * top (beginning) of every stack, so this hack is only necessary for
 * non-isolated code right now.
 */
#ifdef LCD_ISOLATE
#define LCD_MAIN(_CODE)	do { _CODE } while(0)
#else
#define LCD_MAIN(_CODE)	do { _CODE } while(0)
#endif
#if 0
#define LCD_MAIN(_CODE)	do {						\
									\
		/* NULL out return address on stack so that libasync */ \
		/* will stop stack walk here.			     */ \
		/*						     */	\
		/* XXX: A touch of arch-dependent code here, but     */	\
		/* no biggie. When I used gcc's			     */	\
		/* __builtin_frame_address it broke (gcc null'd out  */	\
		/* the return address, but didn't restore it ... ?)  */	\
		/*						     */	\
		volatile void **__frame_ptr;				\
		volatile void *__saved_ret_addr;			\
		asm ("movq %%rbp, %0" : "=g"(__frame_ptr) ::);		\
		__saved_ret_addr = *(__frame_ptr + 1);			\
		*(__frame_ptr + 1) = NULL;				\
									\
		do { _CODE } while(0);					\
									\
		/* Restore old return address to stack. */		\
		*(__frame_ptr + 1) = __saved_ret_addr;			\
									\
	} while (0);
#endif
/**
 * lcd_exit -- Exit from LCD (mode) with return value
 *
 * This should be invoked when you are exiting an isolated LCD 
 * (think: user-level exit call), or when you are a non-isolated
 * thread and want to exit out of LCD mode. Tears down all data
 * structures associated with LCD.
 *
 * For isolated code, this function does not return (just like
 * regular user-level exit).
 */
void LCD_MAYBE_NORETURN lcd_exit(int retval);
void lvd_exit(int retval);

#define LCD_EXIT() ({							\
	*((volatile void**)__builtin_frame_address(0) + 1) = NULL;	\
	lcd_enter();							\
		})
/**
 * lcd_abort -- Abruptly exit from LCD
 *
 * For non-isolated code, this is equivalent to lcd_exit.
 *
 * For isolated code, this immediately exits out of the LCD without
 * trying to tear down its environment.
 */
void LCD_MAYBE_NORETURN lcd_abort(void);

#endif /* LCD_DOMAINS_ENTER_EXIT_H */

