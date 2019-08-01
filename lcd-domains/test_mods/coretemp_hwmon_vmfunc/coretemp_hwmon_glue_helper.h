#ifndef __CORETEMP_GLUE_HELPER_H__
#define __CORETEMP_GLUE_HELPER_H__

#include <asm/cpu_device_id.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/hwmon.h>
#include <linux/sysfs.h>
#include <linux/hwmon-sysfs.h>
#include <linux/pci.h>
#include <libfipc.h>
#include "coretemp_hwmon_common.h"

struct attribute_group_container {
	struct attribute_group attribute_group;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct device_container {
	struct device device;
	struct cptr other_ref;
	struct cptr my_ref;
};

struct device_ptr_container {
	struct device *dev;
	struct cptr other_ref;
	struct cptr my_ref;
	struct hlist_node hentry;
};

struct kobject_container {
	struct kobject kobject;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct module_container {
	struct module module;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct notifier_block_container {
	struct notifier_block notifier_block;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct pci_dev_container {
	struct pci_dev pci_dev;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct platform_device_container {
	struct platform_device platform_device;
	struct cptr other_ref;
	struct cptr my_ref;
};

struct platform_device_ptr_container {
	struct platform_device *pdev;
	struct cptr other_ref;
	struct cptr my_ref;
	struct hlist_node hentry;
};

struct platform_driver_container {
	struct platform_driver platform_driver;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct x86_cpu_id_container {
	struct x86_cpu_id x86_cpu_id;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct trampoline_hidden_args {
	void *struct_container;
	struct glue_cspace *cspace;
	struct lcd_trampoline_handle *t_handle;
	struct thc_channel *async_chnl;
	struct cptr sync_ep;
};
int glue_cap_insert_attribute_group_type(struct glue_cspace *cspace,
		struct attribute_group_container *attribute_group_container,
		struct cptr *c_out);
int glue_cap_insert_device_type(struct glue_cspace *cspace,
		struct device_container *device_container,
		struct cptr *c_out);
int glue_cap_insert_kobject_type(struct glue_cspace *cspace,
		struct kobject_container *kobject_container,
		struct cptr *c_out);
int glue_cap_insert_module_type(struct glue_cspace *cspace,
		struct module_container *module_container,
		struct cptr *c_out);
int glue_cap_insert_notifier_block_type(struct glue_cspace *cspace,
		struct notifier_block_container *notifier_block_container,
		struct cptr *c_out);
int glue_cap_insert_pci_dev_type(struct glue_cspace *cspace,
		struct pci_dev_container *pci_dev_container,
		struct cptr *c_out);
int glue_cap_insert_platform_device_type(struct glue_cspace *cspace,
		struct platform_device_container *platform_device_container,
		struct cptr *c_out);
int glue_cap_insert_platform_driver_type(struct glue_cspace *cspace,
		struct platform_driver_container *platform_driver_container,
		struct cptr *c_out);
int glue_cap_insert_x86_cpu_id_type(struct glue_cspace *cspace,
		struct x86_cpu_id_container *x86_cpu_id_container,
		struct cptr *c_out);
int glue_cap_lookup_attribute_group_type(struct glue_cspace *cspace,
		struct cptr c,
		struct attribute_group_container **attribute_group_container);
int glue_cap_lookup_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct device_container **device_container);
int glue_cap_lookup_kobject_type(struct glue_cspace *cspace,
		struct cptr c,
		struct kobject_container **kobject_container);
int glue_cap_lookup_module_type(struct glue_cspace *cspace,
		struct cptr c,
		struct module_container **module_container);
int glue_cap_lookup_notifier_block_type(struct glue_cspace *cspace,
		struct cptr c,
		struct notifier_block_container **notifier_block_container);
int glue_cap_lookup_pci_dev_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_dev_container **pci_dev_container);
int glue_cap_lookup_platform_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct platform_device_container **platform_device_container);
int glue_cap_lookup_platform_driver_type(struct glue_cspace *cspace,
		struct cptr c,
		struct platform_driver_container **platform_driver_container);
int glue_cap_lookup_x86_cpu_id_type(struct glue_cspace *cspace,
		struct cptr c,
		struct x86_cpu_id_container **x86_cpu_id_container);

#endif	/* __CORETEMP_GLUE_HELPER_H__ */
