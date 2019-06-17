#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/trampoline.h>
#include <lcd_domains/microkernel.h>

#include "../acpi_hwmon_callee.h"

#include <lcd_config/post_hook.h>

#define ACPI_POWER_METER_DEVICE_NAME	"Power Meter"
#define ACPI_POWER_METER_CLASS		"pwr_meter_resource"

static struct glue_cspace *c_cspace;
struct acpi_device *g_acpi_device;
static int cap_in_hardware;

/* Module init/exit routines */
static int __init enable_cap_knobs(const struct dmi_system_id *d)
{
	cap_in_hardware = 1;
	return 0;
}

static struct dmi_system_id __initdata pm_dmi_table[] = {
	{
		enable_cap_knobs, "IBM Active Energy Manager",
		{
			DMI_MATCH(DMI_SYS_VENDOR, "IBM")
		},
	},
	{}
};

static const struct acpi_device_id power_meter_ids[] = {
	{"ACPI000D", 0},
	{"", 0},
};
MODULE_DEVICE_TABLE(acpi, power_meter_ids);

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


int acpi_bus_generate_netlink_event_callee(struct fipc_message *_request)
{
	const char *device_class;
	const char *bus_id;
	unsigned char type;
	int data;
	int ret = 0;
	int func_ret = 0;

	device_class = ACPI_POWER_METER_CLASS;
	bus_id = dev_name(&g_acpi_device->dev);
	type = fipc_get_reg3(_request);
	data = fipc_get_reg4(_request);

	func_ret = acpi_bus_generate_netlink_event(device_class, bus_id, type,
			data);

	fipc_set_reg1(_request, func_ret);

	return ret;
}

int acpi_bus_get_device_callee(struct fipc_message *_request)
{
	__maybe_unused void *handle = NULL;
	struct acpi_device **device = NULL;
	int ret = 0;
	int func_ret = 0;
	int sync_ret;
	cptr_t sync_ep;
	unsigned 	long mem_order;
	unsigned 	long handle_offset;
	cptr_t handle_cptr;
	gva_t handle_gva;
	device = kzalloc(sizeof( void  * ),
		GFP_KERNEL);
	if (!device) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc2;
	}
	( *device ) = kzalloc(sizeof( *( *device ) ),
		GFP_KERNEL);
	if (!( *device )) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	sync_ret = lcd_cptr_alloc(&handle_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}
	lcd_set_cr0(handle_cptr);
	sync_ret = lcd_sync_recv(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to recv");
		lcd_exit(-1);
	}
	mem_order = lcd_r0();
	handle_offset = lcd_r1();
	sync_ret = lcd_map_virt(handle_cptr,
		mem_order,
		&handle_gva);
	if (sync_ret) {
		LIBLCD_ERR("failed to map void *handle");
		lcd_exit(-1);
	}
	func_ret = acpi_bus_get_device(( void  * )( ( gva_val(handle_gva) ) + ( handle_offset ) ),
		( device ));
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
fail_alloc2:
	return ret;

}

#ifdef CONFIG_LVD
int add(struct acpi_device *device)
#else
int add(struct acpi_device *device,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	int ret;
	struct acpi_device_ptr_container *acpi_ptr_c;
	struct device_container *device_container;

	acpi_ptr_c = kzalloc(sizeof(*acpi_ptr_c), GFP_KERNEL);
	if (!acpi_ptr_c) {
		LIBLCD_ERR("kzalloc");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	acpi_ptr_c->acpi_device_ptr = device;

	ret = glue_cap_insert_acpi_device_ptr_type(c_cspace, acpi_ptr_c,
			&acpi_ptr_c->my_ref);

	if (ret) {
		LIBLCD_ERR("glue_cap_insert failed with ret %d", ret);
		goto fail_insert;
	}

	device_container = kzalloc(sizeof(*device_container), GFP_KERNEL);
	if (!device_container) {
		LIBLCD_ERR("kzalloc");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	ret = glue_cap_insert_device_type(c_cspace, device_container,
			&device_container->my_ref);

	if (ret) {
		LIBLCD_ERR("glue_cap_insert failed with ret %d", ret);
		goto fail_insert;
	}

	device_container->dev = &device->dev;
	async_msg_set_fn_type(_request, ACPI_OP_ADD);
	fipc_set_reg0(_request, acpi_ptr_c->my_ref.cptr);
	fipc_set_reg1(_request, device_container->my_ref.cptr);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg1(_request);

	return func_ret;
fail_alloc:
fail_insert:
	return ret;
}

#ifdef CONFIG_LVD
int remove(struct acpi_device *device)
#else
int remove(struct acpi_device *device,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;

	async_msg_set_fn_type(_request, ACPI_OP_REMOVE);
	vmfunc_wrapper(_request);
	func_ret = fipc_get_reg1(_request);
	return func_ret;
}

#ifdef CONFIG_LVD
void notify(struct acpi_device *device,
		unsigned int event)
#else
void notify(struct acpi_device *device,
		unsigned int event,
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	async_msg_set_fn_type(_request, ACPI_OP_NOTIFY);
	fipc_set_reg1(_request, event);

	vmfunc_wrapper(_request);
	return;
}

int acpi_bus_register_driver_callee(struct fipc_message *_request)
{
	struct acpi_driver_container *acpi_container;
	struct acpi_driver *driver;
	int ret = 0;
	int func_ret = 0;

	acpi_container = kzalloc(sizeof(*acpi_container), GFP_KERNEL);
	if (!acpi_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	driver = &acpi_container->acpi_driver;

	strncpy(driver->name, "power_meter", sizeof(driver->name));
	strncpy(driver->class, ACPI_POWER_METER_CLASS, sizeof(driver->class));
	driver->ids = power_meter_ids;
	driver->ops.add = add;
	driver->ops.remove = remove;
	driver->ops.notify = notify;

	acpi_container->other_ref.cptr = fipc_get_reg0(_request);

	ret = glue_cap_insert_acpi_driver_type(c_cspace, acpi_container,
			&acpi_container->my_ref);

	if (ret) {
		LIBLCD_ERR("glue_cap_insert failed with ret %d", ret);
		goto fail_insert;
	}

	func_ret = acpi_bus_register_driver(driver);
	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, acpi_container->my_ref.cptr);

fail_alloc:
fail_insert:
	return ret;
}

int acpi_bus_unregister_driver_callee(struct fipc_message *_request)
{
	struct acpi_driver_container *acpi_container;
	struct acpi_driver *driver;
	int ret = 0;
	cptr_t my_ref;

	my_ref.cptr = fipc_get_reg0(_request);
	ret = glue_cap_lookup_acpi_driver_type(c_cspace, my_ref,
			&acpi_container);

	if (ret) {
		LIBLCD_ERR("glue_cap_lookup failed with ret %d", ret);
		goto fail_lookup;
	}
	driver = &acpi_container->acpi_driver;

	acpi_bus_unregister_driver(driver);

	glue_cap_remove(c_cspace, acpi_container->my_ref);
fail_lookup:
	return ret;
}

int acpi_evaluate_integer_callee(struct fipc_message *_request)
{
	__maybe_unused void *handle = NULL;
	char *pathname;
	struct acpi_object_list *arguments = NULL;
	unsigned long long *data = 0;
	int ret = 0;
	int func_ret = 0;
	int sync_ret;
	cptr_t sync_ep;
	unsigned 	long mem_order;
	unsigned 	long handle_offset;
	cptr_t handle_cptr;
	gva_t handle_gva;
	pathname = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!pathname) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	arguments = kzalloc(sizeof( *arguments ),
		GFP_KERNEL);
	if (!arguments) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	data = kzalloc(sizeof( long   ),
		GFP_KERNEL);
	if (!data) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	sync_ret = lcd_cptr_alloc(&handle_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}
	lcd_set_cr0(handle_cptr);
	sync_ret = lcd_sync_recv(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to recv");
		lcd_exit(-1);
	}
	mem_order = lcd_r0();
	handle_offset = lcd_r1();
	sync_ret = lcd_map_virt(handle_cptr,
		mem_order,
		&handle_gva);
	if (sync_ret) {
		LIBLCD_ERR("failed to map void *handle");
		lcd_exit(-1);
	}
	pathname = (char*) fipc_get_reg1(_request);
	data = (unsigned long long *) fipc_get_reg2(_request);
	func_ret = acpi_evaluate_integer(( void  * )( ( gva_val(handle_gva) ) + ( handle_offset ) ),
		pathname,
		arguments,
		data);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int acpi_extract_package_callee(struct fipc_message *_request)
{
	union acpi_object *package = NULL;
	struct acpi_buffer *format = NULL;
	struct acpi_buffer *buffer = NULL;
	int ret = 0;
	int func_ret = 0;
	package = kzalloc(sizeof( *package ),
		GFP_KERNEL);
	if (!package) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	format = kzalloc(sizeof( *format ),
		GFP_KERNEL);
	if (!format) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	buffer = kzalloc(sizeof( *buffer ),
		GFP_KERNEL);
	if (!buffer) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = acpi_extract_package(package,
		format,
		buffer);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int acpi_evaluate_object_callee(struct fipc_message *_request)
{
	char *pathname;
	struct acpi_object_list *external_params = NULL;
	struct acpi_buffer buffer;
	struct acpi_device_ptr_container *acpi_ptr_c;
	struct device_container *device_container;
	struct acpi_device *acpi_device;
	int ret = 0;
	unsigned int func_ret = 0;
	unsigned 	long mem_order;
	unsigned 	long p_offset;
	cptr_t p_cptr, lcd_cptr;
	gva_t p_gva;

	pathname = kzalloc(sizeof(unsigned long), GFP_KERNEL);
	if (!pathname) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_lookup_acpi_device_ptr_type(c_cspace, __cptr(fipc_get_reg1(_request)),
			&acpi_ptr_c);

	if (ret) {
		LIBLCD_ERR("glue_cap_lookup failed with ret %d", ret);
		goto fail_lookup;
	}

	acpi_ptr_c->other_ref.cptr = fipc_get_reg0(_request);
	acpi_device = acpi_ptr_c->acpi_device_ptr;

	ret = glue_cap_lookup_device_type(c_cspace, __cptr(fipc_get_reg4(_request)),
			&device_container);

	if (ret) {
		LIBLCD_ERR("glue_cap_lookup failed with ret %d", ret);
		goto fail_lookup;
	}

	device_container->other_ref.cptr = fipc_get_reg6(_request);
	strncpy(pathname, (char*)&_request->regs[2], sizeof(_request->regs[2]));

	ret = lcd_cptr_alloc(&p_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_alloc;
	}

	mem_order = fipc_get_reg3(_request);
	/*
	 * FIXME: Due to lack of registers, let's assign zero as it is a page
	 * of data. The offset will be zero.
	 */
	p_offset = 0;
	lcd_cptr = __cptr(fipc_get_reg5(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_cptr,
			p_cptr);

	ret = lcd_map_virt(p_cptr, mem_order, &p_gva);

	if (ret) {
		LIBLCD_ERR("failed to map void *p");
		goto fail_virt;
	}
	buffer.length = (1 << mem_order) * PAGE_SIZE;
	buffer.pointer = (void*)(gva_val(p_gva) + p_offset);

	func_ret = acpi_evaluate_object(acpi_device,
			pathname,
			external_params,
			&buffer);

	fipc_set_reg0(_request, func_ret);
fail_alloc:
fail_lookup:
fail_virt:
	return ret;
}

int acpi_exception_callee(struct fipc_message *_request)
{
	char *module_name = 0;
	unsigned 	int line_number = 0;
	int status = 0;
	char *format = 0;
	int ret = 0;
	module_name = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!module_name) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	format = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!format) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	module_name = (char*) fipc_get_reg1(_request);
	line_number = fipc_get_reg2(_request);
	status = fipc_get_reg3(_request);
	format = (char*) fipc_get_reg4(_request);
	acpi_exception(module_name,
			line_number,
			status,
			format);
	return ret;

}

int device_create_file_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	struct device_attribute *attr = NULL;
	int ret = 0;
	int func_ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	attr = kzalloc(sizeof( *attr ),
		GFP_KERNEL);
	if (!attr) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = device_create_file(dev,
		attr);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int device_remove_file_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	struct device_attribute *attr = NULL;
	int ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	attr = kzalloc(sizeof( *attr ),
		GFP_KERNEL);
	if (!attr) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	device_remove_file(dev,
			attr);
fail_alloc:
	return ret;

}

int get_device_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	int ret = 0;
	struct device_container *func_ret_container = NULL;
	struct device *func_ret = NULL;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret_container->other_ref.cptr = fipc_get_reg2(_request);
	func_ret = get_device(dev);
	ret = glue_cap_insert_device_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fipc_set_reg1(_request,
			func_ret_container->other_ref.cptr);
fail_alloc:
fail_insert:
	return ret;

}

int put_device_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	int ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	put_device(dev);
fail_alloc:
	return ret;

}

int dmi_check_system_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret = 0;

	func_ret = dmi_check_system(pm_dmi_table);

	fipc_set_reg0(_request, func_ret);

	return ret;
}

int hwmon_device_register_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	int ret = 0;
	struct device_container *func_ret_container = NULL;
	struct device *func_ret = NULL;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret_container->other_ref.cptr = fipc_get_reg2(_request);
	func_ret = hwmon_device_register(dev);
	ret = glue_cap_insert_device_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fipc_set_reg1(_request,
			func_ret_container->other_ref.cptr);
fail_alloc:
fail_insert:
	return ret;

}

int hwmon_device_unregister_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	int ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	hwmon_device_unregister(dev);
fail_alloc:
	return ret;

}

int kobject_create_and_add_callee(struct fipc_message *_request)
{
	char *name = 0;
	struct kobject *parent = NULL;
	int ret = 0;
	struct kobject_container *func_ret_container = NULL;
	struct kobject *func_ret = NULL;
	name = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!name) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	parent = kzalloc(sizeof( *parent ),
		GFP_KERNEL);
	if (!parent) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	name = (char*) fipc_get_reg1(_request);
	func_ret_container->other_ref.cptr = fipc_get_reg2(_request);
	func_ret = kobject_create_and_add(name,
		parent);
	ret = glue_cap_insert_kobject_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fipc_set_reg1(_request,
			func_ret_container->other_ref.cptr);
fail_alloc:
fail_insert:
	return ret;

}

int kobject_put_callee(struct fipc_message *_request)
{
	struct kobject *kobj = NULL;
	int ret = 0;
	kobj = kzalloc(sizeof( *kobj ),
		GFP_KERNEL);
	if (!kobj) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	kobject_put(kobj);
fail_alloc:
	return ret;

}

int sysfs_create_link_callee(struct fipc_message *_request)
{
	struct kobject *kobj = NULL;
	struct kobject *target = NULL;
	char *name = 0;
	int ret = 0;
	int func_ret = 0;
	kobj = kzalloc(sizeof( *kobj ),
		GFP_KERNEL);
	if (!kobj) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	target = kzalloc(sizeof( *target ),
		GFP_KERNEL);
	if (!target) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	name = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!name) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	name = (char*) fipc_get_reg1(_request);
	func_ret = sysfs_create_link(kobj,
		target,
		name);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int sysfs_remove_link_callee(struct fipc_message *_request)
{
	struct kobject *kobj = NULL;
	char *name = 0;
	int ret = 0;
	kobj = kzalloc(sizeof( *kobj ),
		GFP_KERNEL);
	if (!kobj) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	name = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!name) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	name = (char*) fipc_get_reg1(_request);
	sysfs_remove_link(kobj,
			name);
fail_alloc:
	return ret;

}

int sysfs_notify_callee(struct fipc_message *_request)
{
	struct kobject *kobj = NULL;
	char *dir = 0;
	char *attr = 0;
	int ret = 0;
	kobj = kzalloc(sizeof( *kobj ),
		GFP_KERNEL);
	if (!kobj) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	dir = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!dir) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	attr = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!attr) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	dir = (char*) fipc_get_reg1(_request);
	attr = (char*) fipc_get_reg2(_request);
	sysfs_notify(kobj,
			dir,
			attr);
fail_alloc:
	return ret;

}
