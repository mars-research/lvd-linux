/* 
 * syscall.h
 *
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 */
#ifndef LCD_DOMAINS_SYSCALL_H
#define LCD_DOMAINS_SYSCALL_H

enum lcd_syscall {

	LCD_SYSCALL_EXIT       = 0,

	LCD_SYSCALL_SEND       = 1,
	LCD_SYSCALL_RECV       = 2,
	LCD_SYSCALL_CALL       = 3,
	LCD_SYSCALL_REPLY      = 4,

};

#endif /* LCD_DOMAINS_SYSCALL_H */
