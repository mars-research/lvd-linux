#ifndef GLUE_USER_H
#define GLUE_USER_H

#include <linux/device-mapper.h>

#define INIT_FIPC_MSG(msg)              memset(msg, 0x0, sizeof(*msg))

int __module_lcd_init(void);
void __module_lcd_exit(void);

#endif
