#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

int rdmsr_safe_on_cpu(unsigned int cpu, unsigned int msr_no, unsigned int* l, unsigned int* h)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* cpu_ptr = &cpu;
	unsigned int* msr_no_ptr = &msr_no;
	unsigned int** l_ptr = &l;
	unsigned int** h_ptr = &h;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct rdmsr_safe_on_cpu_call_ctx call_ctx = {cpu, msr_no, l, h};
	__maybe_unused const struct rdmsr_safe_on_cpu_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *cpu_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *msr_no_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *l_ptr;
	}

	{
		__maybe_unused const void* __adjusted = *h_ptr;
	}

	glue_call_server(__pos, msg, RPC_ID_rdmsr_safe_on_cpu);

	*__pos = 0;
	{
	}

	{
	}

	{
		if (*l_ptr) {
			**l_ptr = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

	{
		if (*h_ptr) {
			**h_ptr = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void cpu_maps_update_begin(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	
	__maybe_unused const struct cpu_maps_update_begin_call_ctx call_ctx = {};
	__maybe_unused const struct cpu_maps_update_begin_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, msg, RPC_ID_cpu_maps_update_begin);

	*__pos = 0;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void cpu_maps_update_done(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	
	__maybe_unused const struct cpu_maps_update_done_call_ctx call_ctx = {};
	__maybe_unused const struct cpu_maps_update_done_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, msg, RPC_ID_cpu_maps_update_done);

	*__pos = 0;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void platform_device_unregister(struct platform_device* pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct platform_device** pdev_ptr = &pdev;
	
	__maybe_unused const struct platform_device_unregister_call_ctx call_ctx = {pdev};
	__maybe_unused const struct platform_device_unregister_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__platform_device_unregister__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_platform_device_unregister);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__platform_device_unregister__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void platform_driver_probe_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_platform_driver_probe function_ptr = glue_unpack(__pos, msg, ext, fptr_platform_driver_probe);
	struct platform_device* pdev = 0;
	struct platform_device** pdev_ptr = &pdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct platform_driver_probe_call_ctx call_ctx = {pdev};
	__maybe_unused struct platform_driver_probe_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct platform_device*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__platform_driver_probe__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	ret = function_ptr(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__platform_driver_probe__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void platform_driver_remove_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_platform_driver_remove function_ptr = glue_unpack(__pos, msg, ext, fptr_platform_driver_remove);
	struct platform_device* pdev = 0;
	struct platform_device** pdev_ptr = &pdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct platform_driver_remove_call_ctx call_ctx = {pdev};
	__maybe_unused struct platform_driver_remove_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct platform_device*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__platform_driver_remove__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	ret = function_ptr(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__platform_driver_remove__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_sysfs_remove_group(struct kobject* kobj, struct dev_attribute_group const* grp)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct kobject** kobj_ptr = &kobj;
	struct dev_attribute_group const** grp_ptr = &grp;
	
	__maybe_unused const struct lvd_sysfs_remove_group_call_ctx call_ctx = {kobj, grp};
	__maybe_unused const struct lvd_sysfs_remove_group_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *kobj_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*kobj_ptr) {
			caller_marshal_kernel__lvd_sysfs_remove_group__kobj__in(__pos, msg, ext, ctx, *kobj_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *grp_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*grp_ptr) {
			caller_marshal_kernel__lvd_sysfs_remove_group__grp__in(__pos, msg, ext, ctx, *grp_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_lvd_sysfs_remove_group);

	*__pos = 0;
	{
		if (*kobj_ptr) {
			caller_unmarshal_kernel__lvd_sysfs_remove_group__kobj__in(__pos, msg, ext, ctx, *kobj_ptr);
		}

	}

	{
		if (*grp_ptr) {
			struct dev_attribute_group* writable = (struct dev_attribute_group*)*grp_ptr;
			caller_unmarshal_kernel__lvd_sysfs_remove_group__grp__in(__pos, msg, ext, ctx, writable);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void dev_attr_show_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_dev_attr_show function_ptr = glue_unpack(__pos, msg, ext, fptr_dev_attr_show);
	struct device* dev = 0;
	struct device_attribute* attr = 0;
	struct device** dev_ptr = &dev;
	struct device_attribute** attr_ptr = &attr;
	char __buf;
	char const* buf = &__buf;
	char const** buf_ptr = &buf;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct dev_attr_show_call_ctx call_ctx = {dev, attr, buf};
	__maybe_unused struct dev_attr_show_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__dev_attr_show__device__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*attr_ptr = glue_unpack_shadow(__pos, msg, ext, struct device_attribute*);
		if (*attr_ptr) {
			callee_unmarshal_kernel__dev_attr_show__dev_attr__in(__pos, msg, ext, ctx, *attr_ptr);
		}

	}

	{
		(void)buf_ptr;
	}

	ret = function_ptr(dev, attr, buf);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__dev_attr_show__device__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*attr_ptr) {
			callee_marshal_kernel__dev_attr_show__dev_attr__in(__pos, msg, ext, ctx, *attr_ptr);
		}

	}

	{
		if (*buf_ptr) {
			size_t i, len;
			char const* array = *buf_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int lvd_sysfs_create_group(struct kobject* dev, struct dev_attribute_group const* grp)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct kobject** dev_ptr = &dev;
	struct dev_attribute_group const** grp_ptr = &grp;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct lvd_sysfs_create_group_call_ctx call_ctx = {dev, grp};
	__maybe_unused const struct lvd_sysfs_create_group_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct device* __adjusted = container_of(*dev_ptr, struct device, kobj);
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__lvd_sysfs_create_group__kobj__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *grp_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*grp_ptr) {
			caller_marshal_kernel__lvd_sysfs_create_group__grp__in(__pos, msg, ext, ctx, *grp_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_lvd_sysfs_create_group);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__lvd_sysfs_create_group__kobj__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*grp_ptr) {
			struct dev_attribute_group* writable = (struct dev_attribute_group*)*grp_ptr;
			caller_unmarshal_kernel__lvd_sysfs_create_group__grp__in(__pos, msg, ext, ctx, writable);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct pci_dev* pci_get_domain_bus_and_slot(int domain, unsigned int bus, unsigned int devfn)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* domain_ptr = &domain;
	unsigned int* bus_ptr = &bus;
	unsigned int* devfn_ptr = &devfn;
	struct pci_dev* ret = 0;
	struct pci_dev** ret_ptr = &ret;
	
	__maybe_unused const struct pci_get_domain_bus_and_slot_call_ctx call_ctx = {domain, bus, devfn};
	__maybe_unused const struct pci_get_domain_bus_and_slot_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *domain_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *bus_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *devfn_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_pci_get_domain_bus_and_slot);

	*__pos = 0;
	{
	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct pci_dev*, (sizeof(struct pci_dev)), (DEFAULT_GFP_FLAGS));
		if (*ret_ptr) {
			caller_unmarshal_kernel__pci_get_domain_bus_and_slot__ret_pci_dev__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct device* devm_hwmon_device_register_with_groups(struct device* dev, char const* name, void* drvdata, struct attribute_group const** groups)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct device** dev_ptr = &dev;
	char const** name_ptr = &name;
	void** drvdata_ptr = &drvdata;
	struct device* ret = 0;
	struct device** ret_ptr = &ret;
	
	__maybe_unused const struct devm_hwmon_device_register_with_groups_call_ctx call_ctx = {dev, name, drvdata, groups};
	__maybe_unused const struct devm_hwmon_device_register_with_groups_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct platform_device* __adjusted = container_of(*dev_ptr, struct platform_device, dev);
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__devm_hwmon_device_register_with_groups__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *name_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*name_ptr) {
			size_t i, len;
			char const* array = *name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	{
		__maybe_unused const void* __adjusted = *drvdata_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*drvdata_ptr) {
		}

	}

	glue_call_server(__pos, msg, RPC_ID_devm_hwmon_device_register_with_groups);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__devm_hwmon_device_register_with_groups__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		(void)name_ptr;
	}

	{
		(void)drvdata_ptr;
	}

	{
		*ret_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct device*, (sizeof(struct device)), (DEFAULT_GFP_FLAGS));
		if (*ret_ptr) {
			caller_unmarshal_kernel__devm_hwmon_device_register_with_groups__ret_device__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void platform_driver_unregister(struct platform_driver* drv)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct platform_driver** drv_ptr = &drv;
	
	__maybe_unused const struct platform_driver_unregister_call_ctx call_ctx = {drv};
	__maybe_unused const struct platform_driver_unregister_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *drv_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*drv_ptr) {
			caller_marshal_kernel__platform_driver_unregister__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_platform_driver_unregister);

	*__pos = 0;
	{
		if (*drv_ptr) {
			caller_unmarshal_kernel__platform_driver_unregister__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int __platform_driver_register(struct platform_driver* drv, struct module* owner)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct platform_driver** drv_ptr = &drv;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct __platform_driver_register_call_ctx call_ctx = {drv, owner};
	__maybe_unused const struct __platform_driver_register_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *drv_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*drv_ptr) {
			caller_marshal_kernel____platform_driver_register__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID___platform_driver_register);

	*__pos = 0;
	{
		if (*drv_ptr) {
			caller_unmarshal_kernel____platform_driver_register__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct platform_device* platform_device_alloc(char const* name, int id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	char const** name_ptr = &name;
	int* id_ptr = &id;
	struct platform_device* ret = 0;
	struct platform_device** ret_ptr = &ret;
	
	__maybe_unused const struct platform_device_alloc_call_ctx call_ctx = {name, id};
	__maybe_unused const struct platform_device_alloc_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *name_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*name_ptr) {
			size_t i, len;
			char const* array = *name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	{
		glue_pack(__pos, msg, ext, *id_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_platform_device_alloc);

	*__pos = 0;
	{
		(void)name_ptr;
	}

	{
	}

	{
		*ret_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct platform_device*, (sizeof(struct platform_device)), (DEFAULT_GFP_FLAGS));
		if (*ret_ptr) {
			caller_unmarshal_kernel__platform_device_alloc__ret_platform_device__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int platform_device_add(struct platform_device* pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct platform_device** pdev_ptr = &pdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct platform_device_add_call_ctx call_ctx = {pdev};
	__maybe_unused const struct platform_device_add_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__platform_device_add__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_platform_device_add);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__platform_device_add__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void platform_device_put(struct platform_device* pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct platform_device** pdev_ptr = &pdev;
	
	__maybe_unused const struct platform_device_put_call_ctx call_ctx = {pdev};
	__maybe_unused const struct platform_device_put_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__platform_device_put__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_platform_device_put);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__platform_device_put__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int rdmsr_on_cpu(unsigned int cpu, unsigned int msr_no, unsigned int* l, unsigned int* h)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* cpu_ptr = &cpu;
	unsigned int* msr_no_ptr = &msr_no;
	unsigned int** l_ptr = &l;
	unsigned int** h_ptr = &h;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct rdmsr_on_cpu_call_ctx call_ctx = {cpu, msr_no, l, h};
	__maybe_unused const struct rdmsr_on_cpu_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *cpu_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *msr_no_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *l_ptr;
	}

	{
		__maybe_unused const void* __adjusted = *h_ptr;
	}

	glue_call_server(__pos, msg, RPC_ID_rdmsr_on_cpu);

	*__pos = 0;
	{
	}

	{
	}

	{
		if (*l_ptr) {
			**l_ptr = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

	{
		if (*h_ptr) {
			**h_ptr = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct x86_cpu_id const* x86_match_cpu(struct x86_cpu_id const* match)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct x86_cpu_id const** match_ptr = &match;
	struct x86_cpu_id const* ret = 0;
	struct x86_cpu_id const** ret_ptr = &ret;
	
	__maybe_unused const struct x86_match_cpu_call_ctx call_ctx = {match};
	__maybe_unused const struct x86_match_cpu_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *match_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*match_ptr) {
			size_t i, len;
			struct x86_cpu_id sentinel = { 0 };
			struct x86_cpu_id const* array = *match_ptr;
			for (len = 0; memcmp(&array[len], &sentinel, sizeof(array[0])); ++len) ;
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				struct x86_cpu_id const* element = &array[i];
				caller_marshal_kernel__x86_match_cpu__match__in(__pos, msg, ext, ctx, element);
			}

		}

	}

	glue_call_server(__pos, msg, RPC_ID_x86_match_cpu);

	*__pos = 0;
	{
		(void)match_ptr;
	}

	{
		*ret_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct x86_cpu_id const*, (sizeof(struct x86_cpu_id)), (DEFAULT_GFP_FLAGS));
		if (*ret_ptr) {
			struct x86_cpu_id* writable = (struct x86_cpu_id*)*ret_ptr;
			caller_unmarshal_kernel__x86_match_cpu__ret_x86_cpu_id__out(__pos, msg, ext, ctx, writable);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct cpuinfo_x86* get_pcpu_cpu_data(int cpu)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* cpu_ptr = &cpu;
	struct cpuinfo_x86* ret = 0;
	struct cpuinfo_x86** ret_ptr = &ret;
	
	__maybe_unused const struct get_pcpu_cpu_data_call_ctx call_ctx = {cpu};
	__maybe_unused const struct get_pcpu_cpu_data_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *cpu_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_get_pcpu_cpu_data);

	*__pos = 0;
	{
	}

	{
		*ret_ptr = glue_unpack_bind_or_new_shadow(__pos, msg, ext, struct cpuinfo_x86*, sizeof(struct cpuinfo_x86));
		if (*ret_ptr) {
			caller_unmarshal_kernel__get_pcpu_cpu_data__cpuinfo_x86__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void notifier_call_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_notifier_call function_ptr = glue_unpack(__pos, msg, ext, fptr_notifier_call);
	struct notifier_block* nb = 0;
	unsigned long action = 0;
	void* data = 0;
	unsigned long* action_ptr = &action;
	void** data_ptr = &data;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct notifier_call_call_ctx call_ctx = {nb, action, data};
	__maybe_unused struct notifier_call_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*action_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		*data_ptr = glue_unpack_new_shadow(__pos, msg, ext, void*, (sizeof(unsigned long)), (DEFAULT_GFP_FLAGS));
		if (*data_ptr) {
		}

	}

	ret = function_ptr(nb, action, data);

	*__pos = 0;
	{
	}

	{
		(void)data_ptr;
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int __register_cpu_notifier(struct notifier_block* nb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct notifier_block** nb_ptr = &nb;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct __register_cpu_notifier_call_ctx call_ctx = {nb};
	__maybe_unused const struct __register_cpu_notifier_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *nb_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*nb_ptr) {
			caller_marshal_kernel____register_cpu_notifier__notifier_block__in(__pos, msg, ext, ctx, *nb_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID___register_cpu_notifier);

	*__pos = 0;
	{
		if (*nb_ptr) {
			caller_unmarshal_kernel____register_cpu_notifier__notifier_block__in(__pos, msg, ext, ctx, *nb_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void __unregister_cpu_notifier(struct notifier_block* nb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct notifier_block** nb_ptr = &nb;
	
	__maybe_unused const struct __unregister_cpu_notifier_call_ctx call_ctx = {nb};
	__maybe_unused const struct __unregister_cpu_notifier_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *nb_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*nb_ptr) {
			caller_marshal_kernel____unregister_cpu_notifier__notifier_block__in(__pos, msg, ext, ctx, *nb_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID___unregister_cpu_notifier);

	*__pos = 0;
	{
		if (*nb_ptr) {
			caller_unmarshal_kernel____unregister_cpu_notifier__notifier_block__in(__pos, msg, ext, ctx, *nb_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext)
{
	switch(id) {
	case MODULE_INIT:
		glue_user_trace("MODULE_INIT");
		__module_lcd_init();
		shared_mem_init();
		break;

	case MODULE_EXIT:
		glue_user_trace("MODULE_EXIT");
		__module_lcd_exit();
		break;

	case RPC_ID_platform_driver_probe:
		glue_user_trace("platform_driver_probe\n");
		platform_driver_probe_callee(msg, ext);
		break;

	case RPC_ID_platform_driver_remove:
		glue_user_trace("platform_driver_remove\n");
		platform_driver_remove_callee(msg, ext);
		break;

	case RPC_ID_dev_attr_show:
		glue_user_trace("dev_attr_show\n");
		dev_attr_show_callee(msg, ext);
		break;

	case RPC_ID_notifier_call:
		glue_user_trace("notifier_call\n");
		notifier_call_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

