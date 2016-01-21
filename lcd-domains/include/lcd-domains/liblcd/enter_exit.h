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

