#ifndef __CORETEMP_CALLEE_H__
#define __CORETEMP_CALLEE_H__

#include "../coretemp_hwmon_glue_helper.h"

int x86_match_cpu_callee(struct fipc_message *_request);
int __platform_driver_register_callee(struct fipc_message *_request);
int platform_device_unregister_callee(struct fipc_message *_request);
int __register_cpu_notifier_callee(struct fipc_message *_request);
int platform_driver_unregister_callee(struct fipc_message *_request);
int __unregister_cpu_notifier_callee(struct fipc_message *_request);
int platform_device_add_callee(struct fipc_message *_request);
int platform_device_alloc_callee(struct fipc_message *_request);
int platform_device_put_callee(struct fipc_message *_request);
int pci_get_domain_bus_and_slot_callee(struct fipc_message *_request);
int devm_hwmon_device_register_with_groups_callee(struct fipc_message *_request);
int rdmsr_safe_on_cpu_callee(struct fipc_message *_request);
int rdmsr_on_cpu_callee(struct fipc_message *_request);
int sysfs_create_group_callee(struct fipc_message *_request);
int sysfs_remove_group_callee(struct fipc_message *_request);
int cpu_maps_update_begin_callee(struct fipc_message *_request);
int cpu_maps_update_done_callee(struct fipc_message *_request);
int glue_coretemp_hwmon_init(void);
void glue_coretemp_hwmon_exit(void);

#endif	/* __CORETEMP_CALLEE_H__ */
