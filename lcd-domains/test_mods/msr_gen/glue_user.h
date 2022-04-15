#ifndef GLUE_USER_H
#define GLUE_USER_H

#include <linux/module.h>
#include <linux/cpu.h>

int __module_lcd_init(void);
void __module_lcd_exit(void);

#ifndef LCD_ISOLATE
#define get_jiffies()           ({ jiffies; })
#endif

#endif
