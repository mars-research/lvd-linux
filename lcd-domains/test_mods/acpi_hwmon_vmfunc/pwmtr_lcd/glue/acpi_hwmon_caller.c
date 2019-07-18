#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include "../../acpi_hwmon_glue_helper.h"
#include "../acpi_hwmon_caller.h"

#include <lcd_config/post_hook.h>

static struct glue_cspace *c_cspace;
extern struct acpi_driver acpi_power_meter_driver;
struct acpi_driver *driver = &acpi_power_meter_driver;

int glue_acpi_hwmon_init(void)
{
	int ret;
	ret = glue_cap_init();
	if (ret) {
		LIBLCD_ERR("cap init");
		goto fail1;
	}
	ret = glue_cap_create(&c_cspace);
	if (ret) {
		LIBLCD_ERR("cap create");
		goto fail2;
	}
	return 0;
fail2:
	glue_cap_exit();
fail1:
	return ret;

}

void glue_acpi_hwmon_exit(void)
{
	glue_cap_destroy(c_cspace);
	glue_cap_exit();

}

int acpi_bus_generate_netlink_event(const char *device_class,
		const char *bus_id,
		unsigned char type,
		int data)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	int ret;

	async_msg_set_fn_type(_request, ACPI_BUS_GENERATE_NETLINK_EVENT);
	fipc_set_reg3(_request, type);
	fipc_set_reg4(_request, data);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

int acpi_bus_get_device(void *handle,
		struct acpi_device **device)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int sync_ret;
	unsigned 	long handle_mem_sz;
	unsigned 	long handle_offset;
	cptr_t handle_cptr;
	int func_ret;
	int ret;

	async_msg_set_fn_type(_request, ACPI_BUS_GET_DEVICE);
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )handle),
		&handle_cptr,
		&handle_mem_sz,
		&handle_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

int acpi_bus_register_driver(struct acpi_driver *driver)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret, ret;

	async_msg_set_fn_type(_request, ACPI_BUS_REGISTER_DRIVER);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;

}

void acpi_bus_unregister_driver(struct acpi_driver *driver)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	async_msg_set_fn_type(_request,
			ACPI_BUS_UNREGISTER_DRIVER);
	ret = vmfunc_wrapper(_request);
	return;
}

unsigned int acpi_evaluate_integer(void *handle,
		char *pathname,
		struct acpi_object_list *arguments,
		unsigned long long *data)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int sync_ret;
	unsigned 	long handle_mem_sz;
	unsigned 	long handle_offset;
	cptr_t handle_cptr;
	int func_ret, ret;
	async_msg_set_fn_type(_request,
			ACPI_EVALUATE_INTEGER);
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )handle),
		&handle_cptr,
		&handle_mem_sz,
		&handle_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
	fipc_set_reg2(_request,
			*data);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

unsigned int acpi_extract_package(union acpi_object *package,
		struct acpi_buffer *format,
		struct acpi_buffer *buffer)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret, ret;
	async_msg_set_fn_type(_request,
			ACPI_EXTRACT_PACKAGE);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;

}

unsigned int acpi_evaluate_object(void *handle,
		char *pathname,
		struct acpi_object_list *external_params,
		struct acpi_buffer *return_buffer)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int sync_ret;
	unsigned 	long handle_mem_sz;
	unsigned 	long handle_offset;
	cptr_t handle_cptr;
	int func_ret, ret;
	async_msg_set_fn_type(_request,
			ACPI_EVALUATE_OBJECT);
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )handle),
		&handle_cptr,
		&handle_mem_sz,
		&handle_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

void acpi_exception(const char *module_name,
		unsigned int line_number,
		unsigned int status,
		const char *format, ...)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	async_msg_set_fn_type(_request,
			ACPI_EXCEPTION);
	fipc_set_reg2(_request,
			line_number);
	fipc_set_reg3(_request,
			status);
	ret = vmfunc_wrapper(_request);
	return;

}

int device_create_file(struct device *dev,
		const struct device_attribute *attr)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret, ret;

	async_msg_set_fn_type(_request,
			DEVICE_CREATE_FILE);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

void device_remove_file(struct device *dev,
		const struct device_attribute *attr)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	async_msg_set_fn_type(_request,
			DEVICE_REMOVE_FILE);
	ret = vmfunc_wrapper(_request);
	return;

}

struct device *get_device(struct device *dev)
{
	struct device_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	async_msg_set_fn_type(_request,
			GET_DEVICE);
	ret = vmfunc_wrapper(_request);
	return &func_ret_container->device;
}

void put_device(struct device *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	async_msg_set_fn_type(_request,
			PUT_DEVICE);
	ret = vmfunc_wrapper(_request);
	return;

}

int dmi_check_system(const struct dmi_system_id *list)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret, func_ret;

	async_msg_set_fn_type(_request,
			DMI_CHECK_SYSTEM);
	ret = vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;

}

struct device *hwmon_device_register(struct device *dev)
{
	struct device_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	async_msg_set_fn_type(_request,
			HWMON_DEVICE_REGISTER);
	ret = vmfunc_wrapper(_request);

	return &func_ret_container->device;
}

void hwmon_device_unregister(struct device *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	async_msg_set_fn_type(_request,
			HWMON_DEVICE_UNREGISTER);
	ret = vmfunc_wrapper(_request);
	return;

}

struct kobject *kobject_create_and_add(const char *name,
		struct kobject *parent)
{
	struct kobject_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	async_msg_set_fn_type(_request,
			KOBJECT_CREATE_AND_ADD);
	ret = vmfunc_wrapper(_request);
	return &func_ret_container->kobject;
}

void kobject_put(struct kobject *kobj)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	async_msg_set_fn_type(_request,
			KOBJECT_PUT);
	ret = vmfunc_wrapper(_request);
	return;

}

int sysfs_create_link(struct kobject *kobj,
		struct kobject *target,
		const char *name)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret, func_ret;

	async_msg_set_fn_type(_request,
			SYSFS_CREATE_LINK);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

void sysfs_remove_link(struct kobject *kobj,
		const char *name)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	async_msg_set_fn_type(_request,
			SYSFS_REMOVE_LINK);
	ret = vmfunc_wrapper(_request);
	return;

}

void sysfs_notify(struct kobject *kobj,
		const char *dir,
		const char *attr)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	async_msg_set_fn_type(_request,
			SYSFS_NOTIFY);
	ret = vmfunc_wrapper(_request);
	return;

}

int acpi_op_add_callee(struct fipc_message *_request)
{
	struct acpi_device *device = NULL;
	int ret = 0;
	int func_ret = 0;
	device = kzalloc(sizeof( *device ),
		GFP_KERNEL);
	if (!device) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = driver->ops.add(device);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int acpi_op_remove_callee(struct fipc_message *_request)
{
	struct acpi_device *device = NULL;
	int ret = 0;
	int func_ret = 0;
	device = kzalloc(sizeof( *device ),
		GFP_KERNEL);
	if (!device) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = driver->ops.remove(device);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int acpi_op_notify_callee(struct fipc_message *_request)
{
	struct acpi_device *device = NULL;
	unsigned 	int event = 0;
	int ret = 0;
	device = kzalloc(sizeof( *device ),
		GFP_KERNEL);
	if (!device) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	event = fipc_get_reg1(_request);
	driver->ops.notify(device,
			event);
fail_alloc:
	return ret;

}
