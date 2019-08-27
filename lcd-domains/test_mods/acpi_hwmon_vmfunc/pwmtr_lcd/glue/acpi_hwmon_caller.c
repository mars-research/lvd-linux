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
extern struct acpi_driver_container acpi_power_meter_driver_container;
struct acpi_driver *driver = &acpi_power_meter_driver_container.acpi_driver;

#define CPTR_HASH_BITS      5
static DEFINE_HASHTABLE(cptr_table, CPTR_HASH_BITS);
static DEFINE_HASHTABLE(acpi_table, CPTR_HASH_BITS);

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
	hash_init(acpi_table);

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

int glue_insert_device(struct device_container *dev_c)
{
	BUG_ON(!dev_c->dev);

	dev_c->my_ref = __cptr((unsigned long)dev_c->dev);

	hash_add(cptr_table, &dev_c->hentry, (unsigned long) dev_c->dev);

	return 0;
}

int glue_lookup_device(struct cptr c, struct
		device_container **dev_cout) {
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

int glue_insert_acpi_handle(struct acpi_handle_container *acpi_hc)
{
	BUG_ON(!acpi_hc->acpi_handle);

	acpi_hc->my_ref = __cptr((unsigned long)acpi_hc->acpi_handle);

	hash_add(cptr_table, &acpi_hc->hentry, (unsigned long) acpi_hc->acpi_handle);

	return 0;
}

int glue_lookup_acpi_handle(struct cptr c, struct
		acpi_handle_container **acpi_hcout) {
	struct acpi_handle_container *acpi_hc;

	hash_for_each_possible(cptr_table, acpi_hc,
			hentry, (unsigned long) cptr_val(c)) {
		if (acpi_hc->acpi_handle == (void *) c.cptr)
			*acpi_hcout = acpi_hc;
	}
	return 0;
}

void glue_remove_acpi_handle(struct acpi_handle_container *acpi_hc)
{
	hash_del(&acpi_hc->hentry);
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
	unsigned 	long dc_mem_sz;
	unsigned 	long dc_offset;
	cptr_t dc_cptr;
	unsigned 	long bus_mem_sz;
	unsigned 	long bus_offset;
	cptr_t bus_cptr;

	ret = lcd_virt_to_cptr(__gva((unsigned long)device_class), &dc_cptr,
			&dc_mem_sz, &dc_offset);

	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}

	ret = lcd_virt_to_cptr(__gva((unsigned long)bus_id), &bus_cptr,
			&bus_mem_sz, &bus_offset);

	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}

	async_msg_set_fn_type(_request, ACPI_BUS_GENERATE_NETLINK_EVENT);
	fipc_set_reg0(_request, ilog2((dc_mem_sz) >> (PAGE_SHIFT)));
	fipc_set_reg1(_request, dc_offset);
	fipc_set_reg2(_request, cptr_val(dc_cptr));
	fipc_set_reg3(_request, ilog2((bus_mem_sz) >> (PAGE_SHIFT)));
	fipc_set_reg4(_request, bus_offset);
	fipc_set_reg5(_request, cptr_val(bus_cptr));
	fipc_set_reg6(_request, type | (data << 8));

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
fail_virt:
	return ret;
}

int acpi_bus_get_device(void *handle,
		struct acpi_device **devices)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	unsigned 	long p_mem_sz;
	unsigned 	long p_offset;
	cptr_t p_cptr;
	int func_ret;
	int ret;
	struct acpi_handle_container *acpi_handle_container = NULL;
	struct device_container *device_container = NULL;

	ret = lcd_virt_to_cptr(__gva((unsigned long)devices), &p_cptr,
			&p_mem_sz, &p_offset);

	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}

	glue_lookup_acpi_handle(__cptr((unsigned long) handle),
			&acpi_handle_container);

	async_msg_set_fn_type(_request, ACPI_BUS_GET_DEVICE);
	fipc_set_reg0(_request, acpi_handle_container->other_ref.cptr);
	fipc_set_reg1(_request, ilog2((p_mem_sz) >> (PAGE_SHIFT)));
	fipc_set_reg2(_request, p_offset);
	fipc_set_reg3(_request, cptr_val(p_cptr));

	ret = vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	device_container = kzalloc(sizeof(*device_container), GFP_KERNEL);

	if (!device_container) {
		LIBLCD_ERR("alloc failed");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	device_container->dev = &devices[0]->dev;

	glue_insert_device(device_container);

	device_container->other_ref.cptr = fipc_get_reg1(_request);

	return func_ret;
fail_virt:
fail_alloc:
	return ret;
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
	fipc_set_reg0(_request, acpi_container->other_ref.cptr);
	ret = vmfunc_wrapper(_request);

	glue_cap_remove(lcd_cspace, acpi_container->my_ref);

	return;
}

unsigned int _acpi_evaluate_integer(struct acpi_device *acpi_device,
		char *pathname,
		struct acpi_object_list *args,
		unsigned long long *data)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret, ret;
	struct acpi_device_container *acpi_container;

	acpi_container = container_of(acpi_device, struct
			acpi_device_container, acpi_device);

	async_msg_set_fn_type(_request, ACPI_EVALUATE_INTEGER);
	fipc_set_reg0(_request, acpi_container->other_ref.cptr);
	strncpy((char*)&_request->regs[1], pathname,
			sizeof(_request->regs[1]));
	fipc_set_reg2(_request, args->pointer->type);
	fipc_set_reg3(_request, args->count);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	*data = fipc_get_reg1(_request);

	return func_ret;
}

unsigned int acpi_extract_package(union acpi_object *package,
		struct acpi_buffer *format,
		struct acpi_buffer *buffer)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret, ret;
	cptr_t p_cptr;
	unsigned long p_offset;
	unsigned long p_mem_sz;
	cptr_t pk_cptr;
	unsigned long pk_offset;
	unsigned long pk_mem_sz;

	ret = lcd_virt_to_cptr(__gva((unsigned long)buffer->pointer), &p_cptr,
			&p_mem_sz, &p_offset);

	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}

	ret = lcd_virt_to_cptr(__gva((unsigned long)package), &pk_cptr,
			&pk_mem_sz, &pk_offset);

	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}

	async_msg_set_fn_type(_request, ACPI_EXTRACT_PACKAGE);
	fipc_set_reg0(_request, ilog2((pk_mem_sz) >> (PAGE_SHIFT))
				| (pk_offset << 32));
	fipc_set_reg1(_request, cptr_val(pk_cptr));
	fipc_set_reg2(_request, ilog2((p_mem_sz) >> (PAGE_SHIFT))
				| (p_offset << 32));
	fipc_set_reg3(_request, cptr_val(p_cptr));
	fipc_set_reg4(_request, format->length);
	fipc_set_reg5(_request, buffer->length);
	fipc_set_reg6(_request, (unsigned long) package->package.elements -
			(unsigned long) package);

	printk("%s, package: %p type: %x package_count: %u", __func__, package,
			package->type, package->package.count);


	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	{
		union acpi_object *pss = package;

		pss->package.elements = (union acpi_object*)((char *) pss + fipc_get_reg1(_request));

		printk("%s, package: %p type: %x package_count: %u", __func__, pss, pss->type, pss->package.count);
		if (pss && pss->package.count == 14) {
			int i;
			for (i = 11; i < 14; i++) {
				union acpi_object *element = &(pss->package.elements[i]);
				printk("%s, element[%d]: %p", __func__, i, element);
			}
		}
	}
	return func_ret;
fail_virt:
	return ret;
}

int acpi_evaluate_object_sync(unsigned long buf_len, unsigned long num_elements,
			unsigned long *lengths)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	char *strings[3];
	cptr_t p_cptr[3];
	unsigned long p_offset[3];
	unsigned long p_mem_sz[3];
	int ret = 0;
	void *buf_ptr;
	cptr_t _p_cptr;
	unsigned long _p_offset;
	unsigned long _p_mem_sz;
	struct acpi_buffer *buffer = NULL;

	async_msg_set_fn_type(_request, SYNC_ACPI_EVALUATE_OBJECT);

	buf_ptr = kzalloc(buf_len, GFP_KERNEL);

	if (!buf_ptr) {
		LIBLCD_ERR("failed to alloc");
		goto fail_alloc;
	}

	ret = lcd_virt_to_cptr(__gva((unsigned long)buf_ptr), &_p_cptr,
			&_p_mem_sz, &_p_offset);

	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}
	fipc_set_reg0(_request, ilog2((_p_mem_sz) >> (PAGE_SHIFT))
				| (_p_offset << 32));
	fipc_set_reg1(_request, cptr_val(_p_cptr));


	{
		union acpi_object *pss = buffer->pointer;
		//unsigned long elements_offset = (unsigned long)((char *) pss + fipc_get_reg2(_request));

		printk("%s, pss: %p type: %x package_count: %u elements: %p (elements - pss): %lx reg2: %lx", __func__,
				pss, pss->type, pss->package.count, pss->package.elements,
				(unsigned long) pss->package.elements - (unsigned long) pss,
				fipc_get_reg2(_request));

		if (pss && pss->package.count == 14) {
			int i;
			//acpi_evaluate_object_sync(pss);

			pss->package.elements = (union acpi_object*)((char *) pss + fipc_get_reg2(_request));

			for (i = 11; i < 14; i++) {
				union acpi_object *element = &(pss->package.elements[i]);
				printk("%s, element[%d]: %p | length: %u",
						__func__, i, element,
						element->string.length);
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
			glue_insert_acpi_handle(acpi_handle_container);

			acpi_handle_container->acpi_handle = acpi_handle;
			acpi_handle_container->other_ref.cptr = fipc_get_reg1(_request);
		}
	}

	{

	union acpi_object *pss = buffer->pointer;
	if (pss && pss->package.count == 14) {
		int i, j;
		int idx;
		for (i = 11; i < 14; i++) {
			union acpi_object *element = &(pss->package.elements[i]);
			idx = i - 11;
			j = idx * 2;
			if (element->type == ACPI_TYPE_STRING) {
				strings[idx] = (char*) kzalloc(element->string.length, GFP_KERNEL);
				if (!strings[idx]) {
					LIBLCD_ERR("alloc failed");
					goto fail_alloc;
				}

				ret = lcd_virt_to_cptr(__gva((unsigned long)strings[idx]),
							&p_cptr[idx], &p_mem_sz[idx],
							&p_offset[idx]);

				if (ret) {
					LIBLCD_ERR("virt to cptr failed");
					goto fail_virt;
				}

				_request->regs[j] = ilog2((p_mem_sz[idx]) >> (PAGE_SHIFT))
							| (p_offset[idx] << 32);

				_request->regs[j+1] = cptr_val(p_cptr[idx]); 
			}
		}
	}
	}
	vmfunc_wrapper(_request);
fail_virt:
fail_alloc:
	return ret;
}

unsigned int _acpi_evaluate_object(struct acpi_device *acpi_device,
		char *pathname,
		struct acpi_object_list *external_params,
		struct acpi_buffer *buffer)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct acpi_device_container *acpi_container;
	int ret;
	unsigned int func_ret;
	unsigned long buffer_length;
	unsigned long num_elements;
	unsigned long lengths[20] = {0};
	unsigned long p_mem_sz = 0, p_offset = 0;
	int i;

	acpi_container = container_of(acpi_device, struct
			acpi_device_container, acpi_device);

	async_msg_set_fn_type(_request, ACPI_EVALUATE_OBJECT);

	printk("%s, buffer->pointer: %p p_mem_sz: %lx p_offset: %lx", __func__,
			buffer->pointer, p_mem_sz, p_offset);

	/* We haven't exchanged our cptr yet */
	fipc_set_reg0(_request, acpi_container->my_ref.cptr);
	fipc_set_reg1(_request, acpi_container->other_ref.cptr);
	strncpy((char*)&_request->regs[2], pathname, sizeof(_request->regs[2]));
	fipc_set_reg3(_request, buffer->length);
	//fipc_set_reg5(_request, buffer->pointer);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	buffer_length = fipc_get_reg2(_request);
	num_elements = fipc_get_reg3(_request);

	if (num_elements == 3) {
		for (i = 0; i < num_elements; i++) {
			lengths[i] = _request->regs[i+4];
		}

		acpi_evaluate_object_sync(buffer_length, num_elements, lengths);
	}

	return func_ret;
}

int device_create_file(struct device *dev,
		const struct device_attribute *devattr)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct device_container *device_container = NULL;
	struct sensor_device_attribute *s_attr;
	struct sensor_device_attribute_container *s_attr_cnt;
	int func_ret, ret;
	struct page *p;
	cptr_t p_cptr;
	unsigned long p_offset;
	unsigned long p_mem_sz;
	cptr_t buf_cptr;
	unsigned long buf_offset;
	unsigned long buf_mem_sz;

	s_attr = container_of(devattr, struct sensor_device_attribute,
			dev_attr);
	s_attr_cnt = container_of(s_attr, struct
			sensor_device_attribute_container, sensor_attr);

	glue_lookup_device(__cptr((unsigned long) dev),
			&device_container);

	ret = glue_cap_insert_sensor_device_attribute_type(lcd_cspace,
			s_attr_cnt, &s_attr_cnt->my_ref);
	if (ret) {
		LIBLCD_ERR("glue_cap_insert failed with ret: %d", ret);
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, DEVICE_CREATE_FILE);

	ret = lcd_virt_to_cptr(__gva((unsigned long)devattr->attr.name), &p_cptr,
			&p_mem_sz, &p_offset);

	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}

	if (!s_attr_cnt->buf) {
		p = lcd_alloc_pages(GFP_KERNEL, 0);

		if (!p) {
			LIBLCD_ERR("page alloc failed");
			ret = -ENOMEM;
			goto fail_alloc;
		}

		s_attr_cnt->buf = lcd_page_address(p);
	}

	ret = lcd_virt_to_cptr(__gva((unsigned long)s_attr_cnt->buf), &buf_cptr,
			&buf_mem_sz, &buf_offset);

	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}

	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, s_attr_cnt->my_ref.cptr);
	fipc_set_reg2(_request, (ilog2((p_mem_sz) >> (PAGE_SHIFT)))
				| (p_offset << 32));
	fipc_set_reg3(_request, cptr_val(p_cptr));
	fipc_set_reg4(_request, devattr->attr.mode);
	fipc_set_reg5(_request, cptr_val(buf_cptr));
	fipc_set_reg6(_request, (ilog2((buf_mem_sz) >> (PAGE_SHIFT)))
				| (buf_offset << 32));

	ret = vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);
	return func_ret;
fail_virt:
fail_alloc:
fail_insert:
	return ret;
}

void device_remove_file(struct device *dev,
		const struct device_attribute *devattr)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	struct device_container *device_container = NULL;
	struct sensor_device_attribute *s_attr;
	struct sensor_device_attribute_container *s_attr_cnt;

	s_attr = container_of(devattr, struct sensor_device_attribute,
			dev_attr);
	s_attr_cnt = container_of(s_attr, struct
			sensor_device_attribute_container, sensor_attr);

	glue_lookup_device(__cptr((unsigned long) dev),
			&device_container);

	async_msg_set_fn_type(_request, DEVICE_REMOVE_FILE);

	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, s_attr_cnt->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	return;

}

struct device *get_device(struct device *dev)
{
	struct device_container *device_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	glue_lookup_device(__cptr((unsigned long) dev),
			&device_container);

	async_msg_set_fn_type(_request, GET_DEVICE);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	return device_container->dev;
}

void put_device(struct device *dev)
{
	struct device_container *device_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	glue_lookup_device(__cptr((unsigned long) dev),
			&device_container);

	async_msg_set_fn_type(_request, PUT_DEVICE);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	return;
}

int dmi_check_system(const struct dmi_system_id *list)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret, func_ret;
	struct dmi_system_id_container *dmi_id_container;
#if 0
	cptr_t id_cptr;
	unsigned long id_offset;
	unsigned long id_mem_sz;

	ret = lcd_virt_to_cptr(__gva((unsigned long)list->ident), &id_cptr,
			&id_mem_sz, &id_offset);

	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}
#endif
	dmi_id_container = kzalloc(sizeof(*dmi_id_container), GFP_KERNEL);

	if (!dmi_id_container) {
		LIBLCD_ERR("kzalloc");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	dmi_id_container->dmi_system_id = (struct dmi_system_id*) list;

	ret = glue_cap_insert_dmi_system_id_type(lcd_cspace, dmi_id_container,
			&dmi_id_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}


	async_msg_set_fn_type(_request, DMI_CHECK_SYSTEM);
	fipc_set_reg0(_request, dmi_id_container->my_ref.cptr);
#if 0
	fipc_set_reg1(_request, ilog2((id_mem_sz) >> (PAGE_SHIFT)));
	fipc_set_reg2(_request, id_offset);
	fipc_set_reg3(_request, cptr_val(id_cptr));
#endif
	strncpy((char*)&_request->regs[4], list->matches[0].substr,
			sizeof(_request->regs[4]));
	fipc_set_reg5(_request, list->matches[0].slot |
			(list->matches[0].exact_match << 7));

	ret = vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);
	dmi_id_container->other_ref.cptr = fipc_get_reg1(_request);

	return func_ret;
fail_insert:
fail_alloc:
//fail_virt:
	return ret;
}

struct device *hwmon_device_register(struct device *dev)
{
	struct device_container *hw_dev_cnt = NULL;
	struct device_container *device_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	glue_lookup_device(__cptr((unsigned long) dev),
			&device_container);

	hw_dev_cnt = kzalloc(sizeof(*hw_dev_cnt), GFP_KERNEL);

	if (!hw_dev_cnt) {
		LIBLCD_ERR("alloc failed");
		ret = -ENOMEM;
		goto fail_alloc;
	}


	hw_dev_cnt->dev = kzalloc(sizeof(struct device), GFP_KERNEL);

	if (!hw_dev_cnt->dev) {
		LIBLCD_ERR("alloc failed");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	hw_dev_cnt->dev->parent = dev;

	glue_insert_device(hw_dev_cnt);

	async_msg_set_fn_type(_request, HWMON_DEVICE_REGISTER);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, hw_dev_cnt->my_ref.cptr);

	ret = vmfunc_wrapper(_request);

	hw_dev_cnt->other_ref.cptr = fipc_get_reg0(_request);

	return hw_dev_cnt->dev;

fail_alloc:
	return NULL;
}

void hwmon_device_unregister(struct device *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct device_container *hwdev_container = NULL;
	int ret;

	glue_lookup_device(__cptr((unsigned long) dev),
			&hwdev_container);

	async_msg_set_fn_type(_request, HWMON_DEVICE_UNREGISTER);

	fipc_set_reg0(_request, hwdev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);

	return;
}

struct kobject *kobject_create_and_add(const char *name,
		struct kobject *kobj)
{
	struct kobject_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	cptr_t p_cptr;
	unsigned long p_offset;
	unsigned long p_mem_sz;
	struct device *dev;
	struct acpi_device *acpi_dev;
	struct acpi_device_container *acpi_dev_container;

	dev = container_of(kobj, struct device, kobj);
	acpi_dev = container_of(dev, struct acpi_device, dev);
	acpi_dev_container = container_of(acpi_dev, struct acpi_device_container,
					acpi_device);

	ret = lcd_virt_to_cptr(__gva((unsigned long)name), &p_cptr,
			&p_mem_sz, &p_offset);

	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}

	func_ret_container = kzalloc(sizeof(*func_ret_container), GFP_KERNEL);

	if (!func_ret_container) {
		LIBLCD_ERR("alloc failed");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	ret = glue_cap_insert_kobject_type(lcd_cspace, func_ret_container,
			&func_ret_container->my_ref);

	if (ret) {
		LIBLCD_ERR("glue_cap_insert failed with ret %d", ret);
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, KOBJECT_CREATE_AND_ADD);
	fipc_set_reg0(_request, acpi_dev_container->other_ref.cptr);
	fipc_set_reg1(_request, ilog2((p_mem_sz) >> (PAGE_SHIFT)));
	fipc_set_reg2(_request, p_offset);
	fipc_set_reg3(_request, cptr_val(p_cptr));
	/* send our reference for the return kobject pointer */
	fipc_set_reg4(_request, func_ret_container->my_ref.cptr);

	ret = vmfunc_wrapper(_request);

	func_ret_container->other_ref.cptr = fipc_get_reg0(_request);

	return &func_ret_container->kobject;
fail_insert:
fail_alloc:
fail_virt:
	return NULL;
}

void kobject_put(struct kobject *kobj)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct kobject_container *kobj_cnt;
	int ret;

	kobj_cnt = container_of(kobj, struct kobject_container, kobject);

	async_msg_set_fn_type(_request, KOBJECT_PUT);
	fipc_set_reg0(_request, kobj_cnt->other_ref.cptr);

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
	struct kobject_container *kobj_cnt;
	struct device *dev;
	struct device_container *device_container = NULL;

	kobj_cnt = container_of(kobj, struct kobject_container, kobject);

	dev = container_of(target, struct device, kobj);

	glue_lookup_device(__cptr((unsigned long) dev),
			&device_container);

	async_msg_set_fn_type(_request, SYSFS_CREATE_LINK);
	fipc_set_reg0(_request, kobj_cnt->other_ref.cptr);
	fipc_set_reg1(_request, device_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

void sysfs_remove_link(struct kobject *kobj,
		const char *name)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct kobject_container *kobj_cnt;
	struct device *dev;
	struct device_container *device_container = NULL;
	int ret;

	kobj_cnt = container_of(kobj, struct kobject_container, kobject);

	dev = container_of(kobj, struct device, kobj);

	glue_lookup_device(__cptr((unsigned long) dev),
			&device_container);


	async_msg_set_fn_type(_request, SYSFS_REMOVE_LINK);
	fipc_set_reg0(_request, kobj_cnt->other_ref.cptr);
	fipc_set_reg1(_request, device_container->other_ref.cptr);

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
	struct device *dev;
	struct device_container *device_container = NULL;
	cptr_t p_cptr;
	unsigned long p_offset;
	unsigned long p_mem_sz;

	dev = container_of(kobj, struct device, kobj);

	glue_lookup_device(__cptr((unsigned long) dev),
			&device_container);

	ret = lcd_virt_to_cptr(__gva((unsigned long)attr), &p_cptr,
			&p_mem_sz, &p_offset);

	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}

	async_msg_set_fn_type(_request, SYSFS_NOTIFY);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, ilog2((p_mem_sz) >> (PAGE_SHIFT)));
	fipc_set_reg2(_request, p_offset);
	fipc_set_reg3(_request, cptr_val(p_cptr));

	ret = vmfunc_wrapper(_request);

fail_virt:
	return;
}

int acpi_op_add_callee(struct fipc_message *_request)
{
	struct acpi_device_container *acpi_container;
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

	acpi_container->other_ref.cptr = fipc_get_reg0(_request);

	func_ret = driver->ops.add(acpi_device);

	fipc_set_reg0(_request, func_ret);

fail_alloc:
fail_insert:
	return ret;
}

int acpi_op_remove_callee(struct fipc_message *_request)
{
	struct acpi_device *device = NULL;
	struct acpi_device_container *acpi_container;
	int ret = 0;
	int func_ret = 0;

	ret = glue_cap_lookup_acpi_device_type(lcd_cspace,
			__cptr(fipc_get_reg0(_request)), &acpi_container);

	if (ret) {
		LIBLCD_ERR("glue_cap_lookup failed with ret %d", ret);
		goto fail_lookup;
	}

	device = &acpi_container->acpi_device;
	func_ret = driver->ops.remove(device);
	fipc_set_reg0(_request, func_ret);
fail_lookup:
	return ret;
}

int acpi_op_notify_callee(struct fipc_message *_request)
{
	struct acpi_device *device = NULL;
	struct acpi_device_container *acpi_container;
	unsigned 	int event = 0;
	int ret = 0;

	ret = glue_cap_lookup_acpi_device_type(lcd_cspace,
			__cptr(fipc_get_reg0(_request)), &acpi_container);

	if (ret) {
		LIBLCD_ERR("glue_cap_lookup failed with ret %d", ret);
		goto fail_lookup;
	}

	device = &acpi_container->acpi_device;
	event = fipc_get_reg1(_request);
	driver->ops.notify(device, event);
fail_lookup:
	return ret;
}

int attr_show_callee(struct fipc_message *_request)
{
	struct device_container *device_container = NULL;
	struct device *dev;
	struct sensor_device_attribute_container *s_attr_cnt;
	int ret = 0;
	int func_ret = 0;

	glue_lookup_device(__cptr(fipc_get_reg0(_request)),
			&device_container);

	dev = device_container->dev;

	ret = glue_cap_lookup_sensor_device_attribute_type(lcd_cspace,
			__cptr(fipc_get_reg1(_request)), &s_attr_cnt);
	if (ret) {
		LIBLCD_ERR("glue cap lookup failed ret: %d", ret);
		goto fail_lookup;
	}

	func_ret = s_attr_cnt->sensor_attr.dev_attr.show(dev,
			&s_attr_cnt->sensor_attr.dev_attr, s_attr_cnt->buf);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int attr_store_callee(struct fipc_message *_request)
{
	struct device_container *device_container = NULL;
	struct device *dev;
	struct sensor_device_attribute_container *s_attr_cnt;
	int ret = 0;
	int func_ret = 0;
	size_t count;

	glue_lookup_device(__cptr(fipc_get_reg0(_request)),
			&device_container);

	dev = device_container->dev;

	ret = glue_cap_lookup_sensor_device_attribute_type(lcd_cspace,
			__cptr(fipc_get_reg1(_request)), &s_attr_cnt);
	if (ret) {
		LIBLCD_ERR("glue cap lookup failed ret: %d", ret);
		goto fail_lookup;
	}

	count = fipc_get_reg2(_request);

	func_ret = s_attr_cnt->sensor_attr.dev_attr.store(dev,
			&s_attr_cnt->sensor_attr.dev_attr, s_attr_cnt->buf,
			count);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int dmi_callback_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct dmi_system_id_container *dmi_id_container = NULL;
	struct dmi_system_id *d;

	ret = glue_cap_lookup_dmi_system_id_type(lcd_cspace,
			__cptr(fipc_get_reg0(_request)), &dmi_id_container);

	if (ret) {
		LIBLCD_ERR("glue_cap_lookup failed with ret %d", ret);
		goto fail_lookup;
	}

	d = dmi_id_container->dmi_system_id;
	func_ret = d->callback(d);
	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}
