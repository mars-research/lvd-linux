/* 
 * syscall.h
 *
 * Author: Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 */
#ifndef LCD_DOMAINS_SYSCALL_H
#define LCD_DOMAINS_SYSCALL_H

#define	LCD_SYSCALL_EXIT       0
#define	LCD_SYSCALL_SEND       1
#define	LCD_SYSCALL_RECV       2
#define	LCD_SYSCALL_CALL       3
#define	LCD_SYSCALL_REPLY      4
#define	LCD_SYSCALL_PUTCHAR    5
#define	LCD_SYSCALL_PAGE_ALLOC 6
#define	LCD_SYSCALL_PAGE_MAP   7
#define	LCD_SYSCALL_PAGE_UNMAP 8
#define	LCD_SYSCALL_CAP_DELETE 9
#define	LCD_SYSCALL_SYNC_EP    10


#endif /* LCD_DOMAINS_SYSCALL_H */
