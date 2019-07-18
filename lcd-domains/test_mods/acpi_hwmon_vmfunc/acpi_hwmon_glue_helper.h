#ifndef __ACPI_HWMON_GLUE_HELPER_H__
#define __ACPI_HWMON_GLUE_HELPER_H__


#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/dmi.h>
#include <linux/acpi.h>
#include <libcap.h>
#include <libfipc.h>
#include <thc_ipc.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <asm/lcd_domains/libvmfunc.h>

struct acpi_buffer_container {
	struct acpi_buffer acpi_buffer;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct acpi_device_container {
	struct acpi_device acpi_device;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct acpi_driver_container {
	struct acpi_driver acpi_driver;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct acpi_device_ops_container {
	struct acpi_device_ops acpi_device_ops;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct acpi_object_container {
	union acpi_object acpi_object;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct acpi_object_list_container {
	struct acpi_object_list acpi_object_list;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct device_attribute_container {
	struct device_attribute device_attribute;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct device_container {
	struct device device;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct dmi_system_id_container {
	struct dmi_system_id dmi_system_id;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct kobject_container {
	struct kobject kobject;
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
int glue_cap_insert_acpi_buffer_type(struct glue_cspace *cspace,
		struct acpi_buffer_container *acpi_buffer_container,
		struct cptr *c_out);
int glue_cap_insert_acpi_device_type(struct glue_cspace *cspace,
		struct acpi_device_container *acpi_device_container,
		struct cptr *c_out);
int glue_cap_insert_acpi_driver_type(struct glue_cspace *cspace,
		struct acpi_driver_container *acpi_driver_container,
		struct cptr *c_out);
int glue_cap_insert_acpi_device_ops_type(struct glue_cspace *cspace,
		struct acpi_device_ops_container *acpi_device_ops_container,
		struct cptr *c_out);
int glue_cap_insert_acpi_object_type(struct glue_cspace *cspace,
		struct acpi_object_container *acpi_object_container,
		struct cptr *c_out);
int glue_cap_insert_acpi_object_list_type(struct glue_cspace *cspace,
		struct acpi_object_list_container *acpi_object_list_container,
		struct cptr *c_out);
int glue_cap_insert_device_attribute_type(struct glue_cspace *cspace,
		struct device_attribute_container *device_attribute_container,
		struct cptr *c_out);
int glue_cap_insert_device_type(struct glue_cspace *cspace,
		struct device_container *device_container,
		struct cptr *c_out);
int glue_cap_insert_dmi_system_id_type(struct glue_cspace *cspace,
		struct dmi_system_id_container *dmi_system_id_container,
		struct cptr *c_out);
int glue_cap_insert_kobject_type(struct glue_cspace *cspace,
		struct kobject_container *kobject_container,
		struct cptr *c_out);
int glue_cap_lookup_acpi_buffer_type(struct glue_cspace *cspace,
		struct cptr c,
		struct acpi_buffer_container **acpi_buffer_container);
int glue_cap_lookup_acpi_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct acpi_device_container **acpi_device_container);
int glue_cap_lookup_acpi_driver_type(struct glue_cspace *cspace,
		struct cptr c,
		struct acpi_driver_container **acpi_driver_container);
int glue_cap_lookup_acpi_device_ops_type(struct glue_cspace *cspace,
		struct cptr c,
		struct acpi_device_ops_container **acpi_device_ops_container);
int glue_cap_lookup_acpi_object_type(struct glue_cspace *cspace,
		struct cptr c,
		struct acpi_object_container **acpi_object_container);
int glue_cap_lookup_acpi_object_list_type(struct glue_cspace *cspace,
		struct cptr c,
		struct acpi_object_list_container **acpi_object_list_container);
int glue_cap_lookup_device_attribute_type(struct glue_cspace *cspace,
		struct cptr c,
		struct device_attribute_container **device_attribute_container);
int glue_cap_lookup_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct device_container **device_container);
int glue_cap_lookup_dmi_system_id_type(struct glue_cspace *cspace,
		struct cptr c,
		struct dmi_system_id_container **dmi_system_id_container);
int glue_cap_lookup_kobject_type(struct glue_cspace *cspace,
		struct cptr c,
		struct kobject_container **kobject_container);

#endif	/* __ACPI_HWMON_GLUE_HELPER_H__ */
