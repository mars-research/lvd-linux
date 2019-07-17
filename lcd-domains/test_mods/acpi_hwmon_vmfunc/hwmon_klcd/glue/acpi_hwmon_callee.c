#include <lcd_config/pre_hook.h>

#include <linux/mm.h>
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

#define CPTR_HASH_BITS      5
static DEFINE_HASHTABLE(dev_hash, CPTR_HASH_BITS);
static DEFINE_HASHTABLE(acpidev_hash, CPTR_HASH_BITS);
static DEFINE_HASHTABLE(dmi_hash, CPTR_HASH_BITS);

static struct glue_cspace *c_cspace;
struct acpi_device *g_acpi_device;

/* Module init/exit routines */
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

	hash_init(dev_hash);
	hash_init(acpidev_hash);
	hash_init(dmi_hash);

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

int glue_insert_device(struct hlist_head *htable, struct device_container *dev_c)
{
	BUG_ON(!dev_c->dev);

	dev_c->my_ref = __cptr((unsigned long)dev_c->dev);

	hash_add(dev_hash, &dev_c->hentry, (unsigned long) dev_c->dev);

	return 0;
}

int glue_lookup_device(struct hlist_head *htable, struct cptr c, struct
		device_container **dev_cout) {
	struct device_container *dev_c;

	hash_for_each_possible(dev_hash, dev_c,
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

int glue_insert_acpi_device(struct hlist_head *htable, struct acpi_device_ptr_container *acpi_dev_c)
{
	BUG_ON(!acpi_dev_c->acpi_device_ptr);

	acpi_dev_c->my_ref = __cptr((unsigned
				long)acpi_dev_c->acpi_device_ptr);

	hash_add(dev_hash, &acpi_dev_c->hentry, (unsigned long)
			acpi_dev_c->acpi_device_ptr);

	return 0;
}

int glue_lookup_acpi_device(struct hlist_head *htable, struct cptr c, struct
		acpi_device_ptr_container **dev_cout) {
	struct acpi_device_ptr_container *dev_c;

	hash_for_each_possible(dev_hash, dev_c, hentry, (unsigned long)
			cptr_val(c)) {
		if (dev_c->acpi_device_ptr == (struct acpi_device*) c.cptr)
			*dev_cout = dev_c;
	}
	return 0;
}

void glue_remove_acpi_device(struct acpi_device_ptr_container *dev_c)
{
	hash_del(&dev_c->hentry);
}

int glue_insert_dmi_system_id(struct hlist_head *htable, struct dmi_system_id_ptr_container *dmi_id_c)
{
	BUG_ON(!dmi_id_c->dmi_system_id_ptr);

	dmi_id_c->my_ref = __cptr((unsigned
				long)dmi_id_c->dmi_system_id_ptr);

	hash_add(dev_hash, &dmi_id_c->hentry, (unsigned long)
			dmi_id_c->dmi_system_id_ptr);

	return 0;
}

int glue_lookup_dmi_system_id(struct hlist_head *htable, struct cptr c, struct
		dmi_system_id_ptr_container **dev_cout) {
	struct dmi_system_id_ptr_container *dev_c;

	hash_for_each_possible(dev_hash, dev_c, hentry, (unsigned long)
			cptr_val(c)) {
		if (dev_c->dmi_system_id_ptr == (struct dmi_system_id*) c.cptr)
			*dev_cout = dev_c;
	}
	return 0;
}

void glue_remove_dmi_system_id(struct dmi_system_id_ptr_container *dev_c)
{
	hash_del(&dev_c->hentry);
}

int sync_setup_memory(void *data, size_t sz, unsigned long *order, cptr_t *data_cptr, unsigned long *data_offset)
{
        int ret;
        struct page *p;
        unsigned long data_len;
        unsigned long mem_len;
        /*
         * Determine page that contains (start of) data
         */
        p = virt_to_head_page(data);
        if (!p) {
                LIBLCD_ERR("failed to translate to page");
                ret = -EINVAL;
                goto fail1;
        }
        data_len = sz;
        mem_len = ALIGN(data + data_len - page_address(p), PAGE_SIZE);
        *order = ilog2(roundup_pow_of_two(mem_len >> PAGE_SHIFT));
        /*
         * Volunteer memory
         */
        *data_offset = data - page_address(p);
        ret = lcd_volunteer_pages(p, *order, data_cptr);
        if (ret) {
                LIBLCD_ERR("failed to volunteer memory");
                goto fail2;
        }
        /*
         * Done
         */
        return 0;
fail2:
fail1:
        return ret;
}

int acpi_bus_generate_netlink_event_callee(struct fipc_message *_request)
{
	const char *device_class;
	const char *bus_id;
	unsigned char type;
	int data;
	int ret = 0;
	int func_ret = 0;
	unsigned 	long dc_mem_order;
	unsigned 	long dc_offset;
	cptr_t dc_cptr, lcd_dc_cptr;
	gva_t dc_gva;
	unsigned 	long bus_mem_order;
	unsigned 	long bus_offset;
	cptr_t bus_cptr, lcd_bus_cptr;
	gva_t bus_gva;

	LIBLCD_MSG("%s, called", __func__);


	ret = lcd_cptr_alloc(&dc_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_alloc;
	}

	dc_mem_order = fipc_get_reg0(_request);
	dc_offset = fipc_get_reg1(_request);
	lcd_dc_cptr = __cptr(fipc_get_reg2(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_dc_cptr,
			dc_cptr);

	ret = lcd_map_virt(dc_cptr, dc_mem_order, &dc_gva);

	if (ret) {
		LIBLCD_ERR("failed to map void *p");
		goto fail_virt;
	}

	ret = lcd_cptr_alloc(&bus_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_alloc;
	}

	bus_mem_order = fipc_get_reg3(_request);
	bus_offset = fipc_get_reg4(_request);
	lcd_bus_cptr = __cptr(fipc_get_reg5(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_bus_cptr,
			bus_cptr);

	ret = lcd_map_virt(bus_cptr, bus_mem_order, &bus_gva);

	if (ret) {
		LIBLCD_ERR("failed to map void *p");
		goto fail_virt;
	}

	device_class = (void*)(gva_val(dc_gva) + dc_offset);
	bus_id = (void*)(gva_val(bus_gva) + bus_offset);
	type = fipc_get_reg6(_request) & 0xFF;
	data = fipc_get_reg6(_request) >> 8;

	func_ret = acpi_bus_generate_netlink_event(device_class, bus_id, type,
			data);

	fipc_set_reg0(_request, func_ret);
fail_alloc:
fail_virt:
	return ret;
}

int acpi_bus_get_device_callee(struct fipc_message *_request)
{
	void *handle = NULL;
	struct acpi_device **devices = NULL;
	int ret = 0;
	int func_ret = 0;
	unsigned 	long p_mem_order;
	unsigned 	long p_offset;
	cptr_t p_cptr, lcd_cptr;
	gva_t p_gva;
	struct acpi_handle_container *acpi_handle_container = NULL;

	LIBLCD_MSG("%s, called", __func__);


	p_mem_order = fipc_get_reg1(_request);
	p_offset = fipc_get_reg2(_request);
	lcd_cptr = __cptr(fipc_get_reg3(_request));

	ret = lcd_cptr_alloc(&p_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_alloc;
	}

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_cptr,
			p_cptr);

	ret = lcd_map_virt(p_cptr, p_mem_order, &p_gva);

	if (ret) {
		LIBLCD_ERR("failed to map void *p");
		goto fail_virt;
	}

	ret = glue_cap_lookup_acpi_handle_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&acpi_handle_container);

	if (ret) {
		LIBLCD_ERR("glue_cap_lookup failed with ret %d", ret);
		goto fail_lookup;
	}

	handle = acpi_handle_container->acpi_handle;
	devices = (struct acpi_device**)(gva_val(p_gva) + p_offset);

	func_ret = acpi_bus_get_device(handle, devices);
	fipc_set_reg0(_request, func_ret);

	{
		struct device_container *device_container = NULL;
		struct acpi_device *acpi_dev;

		device_container = kzalloc(sizeof(*device_container),
				GFP_KERNEL);

		if (!device_container) {
			LIBLCD_ERR("kzalloc");
			ret = -ENOMEM;
			goto fail_alloc;
		}

		acpi_dev = devices[0];

		device_container->dev = &acpi_dev->dev;

		glue_insert_device(dev_hash, device_container);

		fipc_set_reg1(_request, device_container->my_ref.cptr);
	}

fail_alloc:
fail_lookup:
fail_virt:
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

	LIBLCD_MSG("%s, called! handle: %p", __func__, device->handle);

	acpi_ptr_c = kzalloc(sizeof(*acpi_ptr_c), GFP_KERNEL);
	if (!acpi_ptr_c) {
		LIBLCD_ERR("kzalloc");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	acpi_ptr_c->acpi_device_ptr = device;

	glue_insert_acpi_device(acpidev_hash, acpi_ptr_c);

	device_container = kzalloc(sizeof(*device_container), GFP_KERNEL);
	if (!device_container) {
		LIBLCD_ERR("kzalloc");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	device_container->dev = &device->dev;

	glue_insert_device(dev_hash, device_container);

	async_msg_set_fn_type(_request, ACPI_OP_ADD);
	fipc_set_reg0(_request, acpi_ptr_c->my_ref.cptr);
	fipc_set_reg1(_request, device_container->my_ref.cptr);

	vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg0(_request);

	printk("%s, add returns %d\n", __func__, func_ret);

	return func_ret;
fail_alloc:
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
	struct acpi_device_ptr_container *acpi_ptr_c = NULL;

	LIBLCD_MSG("%s, called", __func__);

	glue_lookup_acpi_device(acpidev_hash, __cptr((unsigned long) device),
			&acpi_ptr_c);

	async_msg_set_fn_type(_request, ACPI_OP_REMOVE);
	fipc_set_reg0(_request, acpi_ptr_c->other_ref.cptr);
	vmfunc_klcd_wrapper(_request, 1);
	func_ret = fipc_get_reg0(_request);
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

	LIBLCD_MSG("%s, called", __func__);

	async_msg_set_fn_type(_request, ACPI_OP_NOTIFY);
	fipc_set_reg1(_request, event);

	vmfunc_klcd_wrapper(_request, 1);
	return;
}

int acpi_bus_register_driver_callee(struct fipc_message *_request)
{
	struct acpi_driver_container *acpi_container;
	struct acpi_driver *driver;
	int ret = 0;
	int func_ret = 0;

	LIBLCD_MSG("%s, called", __func__);


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

	LIBLCD_MSG("%s, called", __func__);

	ret = glue_cap_lookup_acpi_driver_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &acpi_container);
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
	void *handle = NULL;
	struct acpi_handle_container *acpi_handle_container = NULL;
	char *pathname;
	union acpi_object arg0;
	struct acpi_object_list args;
	unsigned long long data;
	int ret = 0;
	int func_ret = 0;

	LIBLCD_MSG("%s, called", __func__);


	pathname = kzalloc(sizeof(unsigned long), GFP_KERNEL);
	if (!pathname) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_lookup_acpi_handle_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&acpi_handle_container);

	if (ret) {
		LIBLCD_ERR("glue_cap_lookup failed with ret %d", ret);
		goto fail_lookup;
	}

	handle = acpi_handle_container->acpi_handle;

	strncpy(pathname, (char*)&_request->regs[1],
			sizeof(_request->regs[1]));

	arg0.type = fipc_get_reg2(_request);
	args.count = fipc_get_reg3(_request);
	args.pointer = &arg0;

	func_ret = acpi_evaluate_integer(handle, pathname, &args, &data);
	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, data);
fail_lookup:
fail_alloc:
	return ret;
}

int acpi_extract_package_callee(struct fipc_message *_request)
{
	union acpi_object *package;
	struct acpi_buffer format;
	struct acpi_buffer buffer;
	unsigned 	long p_mem_order;
	unsigned 	long p_offset;
	cptr_t p_cptr, lcd_cptr;
	gva_t p_gva;
	unsigned 	long pk_mem_order;
	unsigned 	long pk_offset;
	cptr_t pk_cptr, lcd_pk_cptr;
	gva_t pk_gva;
	int ret;
	int func_ret;
	char *format_buf;

	LIBLCD_MSG("%s, called", __func__);


	ret = lcd_cptr_alloc(&pk_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_alloc;
	}

	pk_mem_order = LOWER_HALF(fipc_get_reg0(_request));
	pk_offset = UPPER_HALF(fipc_get_reg0(_request));
	lcd_pk_cptr = __cptr(fipc_get_reg1(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_pk_cptr,
			pk_cptr);

	ret = lcd_map_virt(pk_cptr, pk_mem_order, &pk_gva);

	if (ret) {
		LIBLCD_ERR("failed to map package");
		goto fail_virt;
	}

	package = (void*)(gva_val(pk_gva) + pk_offset);

	/* map buffer */
	ret = lcd_cptr_alloc(&p_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_alloc;
	}

	p_mem_order = LOWER_HALF(fipc_get_reg2(_request));
	p_offset = UPPER_HALF(fipc_get_reg2(_request));
	lcd_cptr = __cptr(fipc_get_reg3(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_cptr,
			p_cptr);

	ret = lcd_map_virt(p_cptr, p_mem_order, &p_gva);

	if (ret) {
		LIBLCD_ERR("failed to map void *p");
		goto fail_virt;
	}

	format.length = fipc_get_reg4(_request);

	format_buf = kzalloc(format.length, GFP_KERNEL);

	if (!format_buf) {
		LIBLCD_ERR("alloc failed");
		goto fail_alloc;
	}

	memset(format_buf, 'N', format.length);
	format.pointer = format_buf;

	buffer.pointer = (void*)(gva_val(p_gva) + p_offset);
	buffer.length = fipc_get_reg5(_request);

	printk("%s, package: %p type: %x package_count: %u\n", __func__, package,
			package->type, package->package.count);

	package->package.elements = (union acpi_object *)((char*) package +
			fipc_get_reg6(_request));

	func_ret = acpi_extract_package(package, &format, &buffer);
	printk("%s, acpi_extract_package returns %d\n", __func__,
				func_ret);
	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, (unsigned long) package->package.elements -
			(unsigned long) package);

	kfree(format_buf);
fail_alloc:
fail_virt:
	return ret;
}

int acpi_evaluate_object_sync_callee(struct fipc_message *_request)
{
	unsigned 	long mem_order;
	unsigned 	long p_offset;
	cptr_t p_cptr, lcd_cptr;
	gva_t p_gva;
	char *strings[3];
	cptr_t p_cptr[3];
	unsigned long p_offset[3];
	unsigned long p_mem_sz[3];

	ret = lcd_cptr_alloc(&p_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_alloc;
	}

	mem_order = LOWER_HALF(fipc_get_reg3(_request));
	p_offset = UPPER_HALF(fipc_get_reg3(_request));
	lcd_cptr = __cptr(fipc_get_reg5(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_cptr,
			p_cptr);

	ret = lcd_map_virt(p_cptr, mem_order, &p_gva);

	if (ret) {
		LIBLCD_ERR("failed to map void *p");
		goto fail_virt;
	}
	buffer.length = (1 << 2) * PAGE_SIZE;
	buffer.pointer = (void*)(gva_val(p_gva) + p_offset);



}

int acpi_evaluate_object_callee(struct fipc_message *_request)
{
	char *pathname;
	struct acpi_object_list *external_params = NULL;
	struct acpi_buffer buffer;
	struct acpi_device_ptr_container *acpi_ptr_c = NULL;
	struct device_container *device_container = NULL;
	struct acpi_device *acpi_device;
	struct acpi_object_container *acpi_obj_container;
	void *handle;
	int ret = 0;
	unsigned int func_ret = 0;

	LIBLCD_MSG("%s, called", __func__);

	pathname = kzalloc(sizeof(unsigned long), GFP_KERNEL);
	if (!pathname) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	acpi_obj_container = kzalloc(sizeof(*acpi_obj_container), GFP_KERNEL);
	if (!acpi_obj_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	glue_lookup_acpi_device(acpidev_hash, __cptr(fipc_get_reg1(_request)),
			&acpi_ptr_c);

	if (ret) {
		LIBLCD_ERR("glue_cap_lookup failed with ret %d", ret);
		goto fail_lookup;
	}

	acpi_ptr_c->other_ref.cptr = fipc_get_reg0(_request);
	acpi_device = acpi_ptr_c->acpi_device_ptr;

	glue_lookup_device(dev_hash, __cptr(fipc_get_reg4(_request)),
			&device_container);

	device_container->other_ref.cptr = fipc_get_reg6(_request);
	strncpy(pathname, (char*)&_request->regs[2], sizeof(_request->regs[2]));

	handle = acpi_device->handle;
	buffer.length = fipc_get_reg3(_request);
	buffer.pointer = fipc_get_reg5(_request);

	printk("%s calling acpi_eval_obj with handle: %p, pathname: %s, "
			"ext_params: %p, buffer.p: %p\n", __func__, handle,
			pathname, external_params, buffer.pointer);

	func_ret = acpi_evaluate_object(handle, pathname,
			external_params, &buffer);

	printk("%s, returned %u\n", __func__, func_ret);

	{
		union acpi_object *pss = buffer.pointer;

		printk("%s, pss: %p type: %x package_count: %u elements: %p "
				"(elements - pss): %lx\n", __func__, pss,
				pss->type, pss->package.count,
				pss->package.elements, (unsigned long)
				pss->package.elements - (unsigned long) pss);

		fipc_set_reg3(_request, pss->package.count - 11);
		if (pss && pss->package.count == 14) {
			int i;
			for (i = 11; i < 14; i++) {
				union acpi_object *element = &(pss->package.elements[i]);
				printk("%s, element[%d]: %p | length: %u\n",
						__func__, i, element,
						element->string.length);
				_request->regs[i-7] = element->string.length;
			}
		}
		if (pss && pss->package.count == 1) {
			union acpi_object *element = &(pss->package.elements[0]);
			void *acpi_handle = element->reference.handle;

			struct acpi_handle_container *acpi_handle_container;
			acpi_handle_container = kzalloc(sizeof(*acpi_handle_container), GFP_KERNEL);

			if (!acpi_handle_container) {
				LIBLCD_ERR("alloc failed");
				goto fail_alloc;
			}
			acpi_handle_container->acpi_handle = acpi_handle;
			ret = glue_cap_insert_acpi_handle_type(c_cspace,
					acpi_handle_container,
					&acpi_handle_container->my_ref);
			if (ret) {
				LIBLCD_ERR("lcd insert");
				goto fail_insert;
			}

			fipc_set_reg1(_request,
					acpi_handle_container->my_ref.cptr);
		}
	}
	fipc_set_reg0(_request, func_ret);
	fipc_set_reg2(_request, buffer.length);

fail_alloc:
fail_lookup:
fail_virt:
fail_insert:
	return ret;
}

#ifdef CONFIG_LVD
ssize_t attr_show(struct device *dev, struct device_attribute *attr, char *buf)
#else
ssize_t attr_show(struct device *dev, struct device_attribute *attr, char *buf
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	ssize_t func_ret;
	struct device_container *device_container = NULL;
	struct sensor_device_attribute *s_attr;
	struct sensor_device_attribute_container *s_attr_cnt;

	LIBLCD_MSG("%s, called", __func__);


	s_attr = container_of(attr, struct sensor_device_attribute,
			dev_attr);
	s_attr_cnt = container_of(s_attr, struct
			sensor_device_attribute_container, sensor_attr);

	glue_lookup_device(dev_hash, __cptr((unsigned long) dev),
			&device_container);

	async_msg_set_fn_type(_request, ATTR_SHOW);

	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, s_attr_cnt->other_ref.cptr);

	vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg1(_request);

	/*
	 * copy from our shared page to the kernel provided buffer
	 */
	memcpy(buf, s_attr_cnt->buf, func_ret);

	return func_ret;
}

#ifdef CONFIG_LVD
ssize_t attr_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
#else
ssize_t attr_store(struct device *dev, struct device_attribute *attr, char *buf
		struct trampoline_hidden_args *hidden_args)
#endif
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	ssize_t func_ret;
	struct device_container *device_container = NULL;
	struct sensor_device_attribute *s_attr;
	struct sensor_device_attribute_container *s_attr_cnt;

	LIBLCD_MSG("%s, called", __func__);


	s_attr = container_of(attr, struct sensor_device_attribute,
			dev_attr);
	s_attr_cnt = container_of(s_attr, struct
			sensor_device_attribute_container, sensor_attr);

	glue_lookup_device(dev_hash, __cptr((unsigned long) dev),
			&device_container);

	async_msg_set_fn_type(_request, ATTR_STORE);

	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, s_attr_cnt->other_ref.cptr);
	fipc_set_reg2(_request, count);

	memcpy(s_attr_cnt->buf, buf, count);
	vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int device_create_file_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	struct device_container *device_container = NULL;
	struct device_attribute *dev_attr;
	struct sensor_device_attribute_container *s_attr_cnt;
	int ret = 0;
	int func_ret = 0;
	unsigned 	long mem_order;
	unsigned 	long p_offset;
	cptr_t p_cptr, lcd_cptr;
	gva_t p_gva;
	unsigned 	long buf_order;
	unsigned 	long buf_offset;
	cptr_t buf_cptr, lcd_buf_cptr;
	gva_t buf_gva;

	LIBLCD_MSG("%s, called", __func__);


	glue_lookup_device(dev_hash, __cptr(fipc_get_reg0(_request)),
			&device_container);

	dev = device_container->dev;

	s_attr_cnt = kzalloc(sizeof(*s_attr_cnt), GFP_KERNEL);

	if (!s_attr_cnt) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	s_attr_cnt->other_ref.cptr = fipc_get_reg1(_request);

	ret = lcd_cptr_alloc(&p_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_alloc;
	}

	ret = lcd_cptr_alloc(&buf_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_alloc;
	}

	mem_order = LOWER_HALF(fipc_get_reg2(_request));
	p_offset = UPPER_HALF(fipc_get_reg2(_request));
	lcd_cptr = __cptr(fipc_get_reg3(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_cptr,
			p_cptr);

	ret = lcd_map_virt(p_cptr, mem_order, &p_gva);

	if (ret) {
		LIBLCD_ERR("failed to map void *p");
		goto fail_virt;
	}

	lcd_buf_cptr = __cptr(fipc_get_reg5(_request));

	buf_order = LOWER_HALF(fipc_get_reg6(_request));
	buf_offset = UPPER_HALF(fipc_get_reg6(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_buf_cptr,
			buf_cptr);

	ret = lcd_map_virt(buf_cptr, buf_order, &buf_gva);

	if (ret) {
		LIBLCD_ERR("failed to map void *p");
		goto fail_virt;
	}

	s_attr_cnt->buf = (char*)(gva_val(buf_gva) + buf_offset);

	dev_attr = &s_attr_cnt->sensor_attr.dev_attr;
	dev_attr->attr.mode = fipc_get_reg5(_request);
	dev_attr->attr.name = (char*)(gva_val(p_gva) + p_offset);

	dev_attr->show = attr_show;

	if (dev_attr->attr.mode & S_IWUSR) {
		dev_attr->store = attr_store;
	}

	func_ret = device_create_file(dev, dev_attr);

	fipc_set_reg0(_request, func_ret);
fail_alloc:
fail_virt:
	return ret;
}

int device_remove_file_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	int ret = 0;
	struct device_container *device_container = NULL;
	struct sensor_device_attribute_container *s_attr_cnt = NULL;

	LIBLCD_MSG("%s, called", __func__);


	glue_lookup_device(dev_hash, __cptr(fipc_get_reg0(_request)),
			&device_container);

	dev = device_container->dev;

	ret = glue_cap_lookup_sensor_device_attribute_type(c_cspace,
			__cptr(fipc_get_reg1(_request)), &s_attr_cnt);

	if (ret) {
		LIBLCD_ERR("lookup failed %d", ret);
		goto fail_lookup;
	}

	device_remove_file(dev, &s_attr_cnt->sensor_attr.dev_attr);
fail_lookup:
	return ret;
}

int get_device_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	int ret = 0;
	struct device_container *device_container = NULL;
	struct device *func_ret;

	LIBLCD_MSG("%s, called", __func__);


	glue_lookup_device(dev_hash, __cptr(fipc_get_reg0(_request)),
			&device_container);

	dev = device_container->dev;

	func_ret = get_device(dev);

	return ret;
}

int put_device_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	int ret = 0;
	struct device_container *device_container = NULL;

	LIBLCD_MSG("%s, called", __func__);


	glue_lookup_device(dev_hash, __cptr(fipc_get_reg0(_request)),
			&device_container);

	dev = device_container->dev;

	put_device(dev);

	return ret;
}

static int enable_cap_knobs(const struct dmi_system_id *d)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct dmi_system_id_ptr_container *dmi_id_container = NULL;

	LIBLCD_MSG("%s, called", __func__);


	glue_lookup_dmi_system_id(dmi_hash, __cptr((unsigned long)d), &dmi_id_container);

	async_msg_set_fn_type(_request, DMI_CALLBACK);
	fipc_set_reg0(_request, dmi_id_container->other_ref.cptr);

	vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

static struct dmi_system_id pm_dmi_table[] = {
	{
		.callback = enable_cap_knobs,
		.ident = "IBM Active Energy Manager",
	},
	{}
};


int dmi_check_system_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret = 0;
	struct dmi_system_id_ptr_container *dmi_id_container;
#if 0
	cptr_t id_cptr, lcd_id_cptr;
	unsigned long id_offset, id_mem_order;
	gva_t id_gva;
#endif

	LIBLCD_MSG("%s, called", __func__);


	dmi_id_container = kzalloc(sizeof(*dmi_id_container), GFP_KERNEL);

	if (!dmi_id_container) {
		LIBLCD_ERR("kzalloc");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	dmi_id_container->dmi_system_id_ptr = pm_dmi_table;

	glue_insert_dmi_system_id(dmi_hash, dmi_id_container);

#if 0
	id_mem_order = fipc_get_reg1(_request);
	id_offset = fipc_get_reg2(_request);
	lcd_id_cptr = __cptr(fipc_get_reg3(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_id_cptr,
			id_cptr);

	ret = lcd_map_virt(id_cptr, id_mem_order, &id_gva);

	if (ret) {
		LIBLCD_ERR("failed to map void *p");
		goto fail_virt;
	}

	pm_dmi_table->ident = (char*)(gva_val(id_gva) + id_offset);
#endif
	strncpy(pm_dmi_table->matches[0].substr, (char*)&_request->regs[4],
			sizeof(_request->regs[4]));

	pm_dmi_table->matches[0].slot = fipc_get_reg5(_request) & 0x7F;
	pm_dmi_table->matches[0].exact_match = (fipc_get_reg5(_request) >> 7) & 0x1;

	pm_dmi_table->callback = enable_cap_knobs;

	dmi_id_container->other_ref.cptr = fipc_get_reg0(_request);

	func_ret = dmi_check_system(pm_dmi_table);

	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, dmi_id_container->my_ref.cptr);

	return func_ret;
fail_alloc:
#if 0
fail_virt:
#endif
	return ret;
}

int hwmon_device_register_callee(struct fipc_message *_request)
{
	struct device_container *hwdev_container = NULL;
	struct device_container *device_container = NULL;
	struct device *dev;
	int ret = 0;
	struct device *func_ret;

	LIBLCD_MSG("%s, called", __func__);


	glue_lookup_device(dev_hash, __cptr(fipc_get_reg0(_request)),
			&device_container);

	dev = device_container->dev;

	hwdev_container->other_ref.cptr = fipc_get_reg1(_request);

	func_ret = hwmon_device_register(dev);

	hwdev_container->dev = func_ret;

	glue_insert_device(dev_hash, hwdev_container);

	fipc_set_reg0(_request, hwdev_container->my_ref.cptr);

	return ret;
}

int hwmon_device_unregister_callee(struct fipc_message *_request)
{
	struct device_container *hwdev_container = NULL;
	struct device *hw_dev;
	int ret = 0;

	LIBLCD_MSG("%s, called", __func__);


	glue_lookup_device(dev_hash, __cptr(fipc_get_reg0(_request)),
			&hwdev_container);

	hw_dev = hwdev_container->dev;

	hwmon_device_unregister(hw_dev);
	return ret;
}

int kobject_create_and_add_callee(struct fipc_message *_request)
{
	char *name = 0;
	struct kobject *parent;
	int ret = 0;
	struct kobject_ptr_container *func_ret_container = NULL;
	struct kobject *func_ret = NULL;
	struct device_container *device_container = NULL;
	cptr_t p_cptr;
	cptr_t lcd_cptr;
	unsigned long p_offset;
	unsigned long mem_order;
	gva_t p_gva;

	LIBLCD_MSG("%s, called", __func__);


	glue_lookup_device(dev_hash, __cptr(fipc_get_reg0(_request)),
			&device_container);

	ret = lcd_cptr_alloc(&p_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_alloc;
	}

	mem_order = fipc_get_reg1(_request);
	p_offset = fipc_get_reg2(_request);
	lcd_cptr = __cptr(fipc_get_reg3(_request));

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_cptr,
			p_cptr);

	ret = lcd_map_virt(p_cptr, mem_order, &p_gva);

	if (ret) {
		LIBLCD_ERR("failed to map void *p");
		goto fail_virt;
	}

	name = (char*)(gva_val(p_gva) + p_offset);

	parent = &device_container->dev->kobj;

	func_ret_container->other_ref.cptr = fipc_get_reg4(_request);

	func_ret = kobject_create_and_add(name, parent);

	ret = glue_cap_insert_kobject_ptr_type(c_cspace, func_ret_container,
			&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	fipc_set_reg0(_request, func_ret_container->my_ref.cptr);
fail_alloc:
fail_insert:
fail_virt:
	return ret;

}

int kobject_put_callee(struct fipc_message *_request)
{
	struct kobject_ptr_container *kobjp_cnt = NULL;
	struct kobject *kobj = NULL;
	int ret = 0;

	LIBLCD_MSG("%s, called", __func__);


	ret = glue_cap_lookup_kobject_ptr_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&kobjp_cnt);

	if (ret) {
		LIBLCD_ERR("glue_cap_lookup failed with ret %d", ret);
		goto fail_lookup;
	}

	kobj = kobjp_cnt->kobject;

	kobject_put(kobj);

fail_lookup:
	return ret;
}

int sysfs_create_link_callee(struct fipc_message *_request)
{
	struct kobject *kobj = NULL;
	struct kobject *target = NULL;
	struct kobject_ptr_container *kobjp_cnt = NULL;
	struct device_container *device_container = NULL;
	struct device *dev;
	const char *name = 0;
	int ret = 0;
	int func_ret = 0;

	LIBLCD_MSG("%s, called", __func__);


	ret = glue_cap_lookup_kobject_ptr_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&kobjp_cnt);

	if (ret) {
		LIBLCD_ERR("glue_cap_lookup failed with ret %d", ret);
		goto fail_lookup;
	}

	kobj = kobjp_cnt->kobject;

	glue_lookup_device(dev_hash, __cptr(fipc_get_reg1(_request)),
			&device_container);

	dev = device_container->dev;
	target = &dev->kobj;
	name = kobject_name(target);
	LIBLCD_MSG("dev: %p, target_kobj: %p name: %s", __func__,
				dev, target, name);

	func_ret = sysfs_create_link(kobj, target, name);
	fipc_set_reg0(_request, func_ret);
fail_lookup:
	return ret;
}

int sysfs_remove_link_callee(struct fipc_message *_request)
{
	struct kobject *kobj = NULL;
	struct kobject_ptr_container *kobjp_cnt = NULL;
	struct device_container *device_container = NULL;
	struct device *dev;
	const char *name = 0;
	int ret = 0;

	LIBLCD_MSG("%s, called", __func__);


	ret = glue_cap_lookup_kobject_ptr_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&kobjp_cnt);

	if (ret) {
		LIBLCD_ERR("glue_cap_lookup failed with ret %d", ret);
		goto fail_lookup;
	}

	kobj = kobjp_cnt->kobject;

	glue_lookup_device(dev_hash, __cptr(fipc_get_reg1(_request)),
			&device_container);

	dev = device_container->dev;
	name = kobject_name(&dev->kobj);

	LIBLCD_MSG("dev: %p, name: %s", __func__,
				dev, name);

	sysfs_remove_link(kobj, name);
fail_lookup:
	return ret;
}

int sysfs_notify_callee(struct fipc_message *_request)
{
	struct kobject *kobj = NULL;
	struct kobject_ptr_container *kobjp_cnt = NULL;
	char *dir = 0;
	char *attr = 0;
	int ret = 0;
	cptr_t p_cptr;
	cptr_t lcd_cptr;
	unsigned long p_offset;
	unsigned long p_mem_order;
	gva_t p_gva;


	LIBLCD_MSG("%s, called", __func__);


	ret = glue_cap_lookup_kobject_ptr_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&kobjp_cnt);

	if (ret) {
		LIBLCD_ERR("glue_cap_lookup failed with ret %d", ret);
		goto fail_lookup;
	}

	kobj = kobjp_cnt->kobject;

	p_mem_order = fipc_get_reg1(_request);
	p_offset = fipc_get_reg2(_request);
	lcd_cptr = __cptr(fipc_get_reg3(_request));

	ret = lcd_cptr_alloc(&p_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_alloc;
	}

	copy_msg_cap_vmfunc(current->vmfunc_lcd, current->lcd, lcd_cptr,
			p_cptr);

	ret = lcd_map_virt(p_cptr, p_mem_order, &p_gva);

	if (ret) {
		LIBLCD_ERR("failed to map void *p");
		goto fail_virt;
	}

	dir = NULL;
	attr = (char*) (gva_val(p_gva) + p_offset);

	sysfs_notify(kobj, dir, attr);
fail_alloc:
fail_lookup:
fail_virt:
	return ret;
}
