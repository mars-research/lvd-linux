#ifndef GLUE_USER_H
#define GLUE_USER_H

#include <linux/etherdevice.h>
#include <net/rtnetlink.h>

int __module_lcd_init(void);
void __module_lcd_exit(void);

#endif
