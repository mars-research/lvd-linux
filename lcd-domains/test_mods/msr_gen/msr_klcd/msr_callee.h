#pragma once
#include "../msr_glue_helper.h"
int __class_create_callee(struct fipc_message *_request);
int capable_callee(struct fipc_message *_request);
int class_destroy_callee(struct fipc_message *_request);
int device_create_callee(struct fipc_message *_request);
int device_destroy_callee(struct fipc_message *_request);
int no_seek_end_llseek_callee(struct fipc_message *_request);
int cpu_maps_update_begin_callee(struct fipc_message *_request);
int cpu_maps_update_done_callee(struct fipc_message *_request);
int rdmsr_safe_on_cpu_callee(struct fipc_message *_request);
int rdmsr_safe_regs_on_cpu_callee(struct fipc_message *_request);
int __register_chrdev_callee(struct fipc_message *_request);
int __unregister_chrdev_callee(struct fipc_message *_request);
int wrmsr_safe_on_cpu_callee(struct fipc_message *_request);
int wrmsr_safe_regs_on_cpu_callee(struct fipc_message *_request);
void dispatch_sync_loop(struct fipc_message *message);

int glue_msr_init(void);
void glue_msr_exit(void);
