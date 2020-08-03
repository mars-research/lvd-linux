#pragma once

#include <asm/cpu_device_id.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/hwmon.h>
#include <linux/sysfs.h>
#include <linux/hwmon-sysfs.h>
#include <linux/pci.h>
#include <libfipc.h>
#include "msr_common.h"

struct class_container {
	struct class class;
	struct cptr other_ref;
	struct cptr my_ref;
};

struct device_container {
	struct device device;
	struct cptr other_ref;
	struct cptr my_ref;
};

struct file_container {
	struct file file;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct file_operations_container {
	struct file_operations file_operations;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct lock_class_key_container {
	struct lock_class_key lock_class_key;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct module_container {
	struct module module;
	struct cptr other_ref;
	struct cptr my_ref;
};
int glue_cap_insert_class_type(struct glue_cspace *cspace,
		struct class_container *class_container,
		struct cptr *c_out);
int glue_cap_insert_device_type(struct glue_cspace *cspace,
		struct device_container *device_container,
		struct cptr *c_out);
int glue_cap_insert_file_type(struct glue_cspace *cspace,
		struct file_container *file_container,
		struct cptr *c_out);
int glue_cap_insert_file_operations_type(struct glue_cspace *cspace,
		struct file_operations_container *file_operations_container,
		struct cptr *c_out);
int glue_cap_insert_lock_class_key_type(struct glue_cspace *cspace,
		struct lock_class_key_container *lock_class_key_container,
		struct cptr *c_out);
int glue_cap_insert_module_type(struct glue_cspace *cspace,
		struct module_container *module_container,
		struct cptr *c_out);
int glue_cap_lookup_class_type(struct glue_cspace *cspace,
		struct cptr c,
		struct class_container **class_container);
int glue_cap_lookup_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct device_container **device_container);
int glue_cap_lookup_file_type(struct glue_cspace *cspace,
		struct cptr c,
		struct file_container **file_container);
int glue_cap_lookup_file_operations_type(struct glue_cspace *cspace,
		struct cptr c,
		struct file_operations_container **file_operations_container);
int glue_cap_lookup_lock_class_key_type(struct glue_cspace *cspace,
		struct cptr c,
		struct lock_class_key_container **lock_class_key_container);
int glue_cap_lookup_module_type(struct glue_cspace *cspace,
		struct cptr c,
		struct module_container **module_container);
