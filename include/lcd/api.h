/* 
 * Author: Anton Burtsev <aburtsev@flux.utah.edu>
 * Copyright: University of Utah
 */
#ifndef _LCD_API_H_
#define _LCD_API_H_

#include <uapi/linux/lcd-api.h>
#include <lcd/cap.h>
#include <lcd/ipc.h>

int lcd_api_init(void);
int lcd_api_exit(void); 
#endif
