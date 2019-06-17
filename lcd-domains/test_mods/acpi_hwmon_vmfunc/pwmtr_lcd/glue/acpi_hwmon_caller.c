#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include <linux/hashtable.h>

#include "../../acpi_hwmon_glue_helper.h"
#include "../acpi_hwmon_caller.h"

#include <lcd_config/post_hook.h>

static struct glue_cspace *lcd_cspace;
extern struct acpi_driver acpi_power_meter_driver;
struct acpi_driver *driver = &acpi_power_meter_driver;

#define CPTR_HASH_BITS      5
static DEFINE_HASHTABLE(cptr_table, CPTR_HASH_BITS);

int glue_acpi_hwmon_init(void)
{
	int ret;
	ret = glue_cap_init();
	if (ret) {
		LIBLCD_ERR("cap init");
		goto fail1;
	}
	ret = glue_cap_create(&lcd_cspace);
	if (ret) {
		LIBLCD_ERR("cap create");
		goto fail2;
	}

	hash_init(cptr_table);

	return 0;
fail2:
	glue_cap_exit();
fail1:
	return ret;

}

void glue_acpi_hwmon_exit(void)
{
	glue_cap_destroy(lcd_cspace);
	glue_cap_exit();

}

int glue_insert_device(struct hlist_head *htable, struct device_container *dev_c)
{
	BUG_ON(!dev_c->dev);

	dev_c->my_ref = __cptr((unsigned long)dev_c->dev);

	hash_add(cptr_table, &dev_c->hentry, (unsigned long) dev_c->dev);

	return 0;
}

int glue_lookup_device(struct hlist_head *htable, struct cptr c, struct device_container **dev_cout)
{
	struct device_container *dev_c;

	hash_for_each_possible(cptr_table, dev_c,
			hentry, (unsigned long) cptr_val(c)) {
		if (dev_c->dev == (struct device*) c.cptr)
			*dev_cout = dev_c;
	}
	return 0;
}

void glue_remove_device(struct device_container *dev_c)
{
	hash_del(&dev_c->hentry);
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
	struct acpi_driver_container *acpi_container;
	int func_ret, ret;

	acpi_container = container_of(driver, struct acpi_driver_container,
			acpi_driver);

	ret = glue_cap_insert_acpi_driver_type(lcd_cspace, acpi_container,
			&acpi_container->my_ref);

	if (ret) {
		LIBLCD_ERR("glue_cap_insert failed with ret %d", ret);
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, ACPI_BUS_REGISTER_DRIVER);
	fipc_set_reg0(_request, acpi_container->my_ref.cptr);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	acpi_container->other_ref.cptr = fipc_get_reg1(_request);

	return func_ret;

fail_insert:
	return ret;
}

void acpi_bus_unregister_driver(struct acpi_driver *driver)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct acpi_driver_container *acpi_container;
	int ret;

	acpi_container = container_of(driver, struct acpi_driver_container,
			acpi_driver);

	async_msg_set_fn_type(_request, ACPI_BUS_UNREGISTER_DRIVER);
	fipc_set_reg0(_request, acpi_container->my_ref.cptr);
	ret = vmfunc_wrapper(_request);

	glue_cap_remove(lcd_cspace, acpi_container->my_ref);

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

unsigned int _acpi_evaluate_object(struct acpi_device *acpi_device,
		const char *pathname,
		struct acpi_object_list *external_params,
		struct acpi_buffer *buffer)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct acpi_device_container *acpi_container;
	struct device_container *device_container = NULL;
	int func_ret, ret;
	struct page *p;
	cptr_t p_cptr;
	unsigned long p_offset;
	unsigned long p_mem_sz;

	acpi_container = container_of(acpi_device, struct acpi_device_container, acpi_device);

	glue_lookup_device(cptr_table, __cptr((unsigned long) &acpi_device->dev), &device_container);

	async_msg_set_fn_type(_request, ACPI_EVALUATE_OBJECT);

	p = lcd_alloc_pages(GFP_KERNEL, 0);

	if (!p) {
		LIBLCD_ERR("page alloc failed");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	buffer->pointer = lcd_page_address(p);

	ret = lcd_virt_to_cptr(__gva((unsigned long)buffer->pointer), &p_cptr,
			&p_mem_sz, &p_offset);

	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}

	/* We haven't exchanged our cptr yet */
	fipc_set_reg0(_request, acpi_container->my_ref.cptr);
	fipc_set_reg1(_request, acpi_container->other_ref.cptr);
	strncpy((char*)&_request->regs[2], pathname, sizeof(_request->regs[2]));
	fipc_set_reg3(_request, ilog2((p_mem_sz) >> (PAGE_SHIFT)));
	fipc_set_reg4(_request, device_container->other_ref.cptr);
	fipc_set_reg5(_request, cptr_val(p_cptr));
	fipc_set_reg6(_request, device_container->my_ref.cptr);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
fail_alloc:
fail_virt:
	return ret;
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
	return func_ret_container->dev;
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

	return func_ret_container->dev;
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
	struct acpi_device_container *acpi_container;
	struct device_container *device_container;
	struct acpi_device *acpi_device;
	int ret = 0;
	int func_ret = 0;

	acpi_container = kzalloc(sizeof(*acpi_container), GFP_KERNEL);

	if (!acpi_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	acpi_device = &acpi_container->acpi_device;

	ret = glue_cap_insert_acpi_device_type(lcd_cspace, acpi_container,
			&acpi_container->my_ref);

	if (ret) {
		LIBLCD_ERR("glue_cap_insert failed with ret %d", ret);
		goto fail_insert;
	}

	device_container = kzalloc(sizeof(*device_container), GFP_KERNEL);

	if (!device_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	device_container->dev = kzalloc(sizeof(struct device), GFP_KERNEL);

	if (!device_container->dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_insert_device(cptr_table, device_container);

	if (ret) {
		LIBLCD_ERR("glue_cap_insert failed with ret %d", ret);
		goto fail_insert;
	}

	acpi_container->other_ref.cptr = fipc_get_reg0(_request);
	device_container->other_ref.cptr = fipc_get_reg1(_request);

	func_ret = driver->ops.add(acpi_device);

	fipc_set_reg0(_request, func_ret);

fail_alloc:
fail_insert:
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
