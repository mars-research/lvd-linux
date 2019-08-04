#ifndef __CORETEMP_CALLER_H__
#define __CORETEMP_CALLER_H__

#include "../coretemp_hwmon_glue_helper.h"

int dispatch_sync_loop(struct fipc_message *message);
int dispatch_async_loop(struct fipc_message *message);
int glue_coretemp_hwmon_init(void);
void glue_coretemp_hwmon_exit(void);
int notifier_fn_t_callee(struct fipc_message *_request);
int probe_callee(struct fipc_message *_request);
int remove_callee(struct fipc_message *_request);
int attr_show_callee(struct fipc_message *_request);
struct cpuinfo_x86 * __cpu_data(int cpu);

int __coretemp_lcd_init(void);
void __coretemp_lcd_exit(void);

#endif	/* __CORETEMP_CALLER_H__ */
