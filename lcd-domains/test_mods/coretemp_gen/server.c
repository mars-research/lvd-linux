#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void sysfs_remove_group_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct kobject* dev = 0;
	struct attribute_group const* grp = 0;
	struct kobject** dev_ptr = &dev;
	struct attribute_group const** grp_ptr = &grp;
	
	__maybe_unused struct sysfs_remove_group_call_ctx call_ctx = {dev, grp};
	__maybe_unused struct sysfs_remove_group_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct device* __device = glue_unpack(__pos, msg, ext, struct device*);
		*dev_ptr = &__device->kobj;
		if (*dev_ptr) {
			callee_unmarshal_kernel__sysfs_remove_group__kobj__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*grp_ptr = glue_unpack_shadow(__pos, msg, ext, struct attribute_group const*);
		if (*grp_ptr) {
			struct attribute_group* writable = (struct attribute_group*)*grp_ptr;
			callee_unmarshal_kernel__sysfs_remove_group__grp__in(__pos, msg, ext, ctx, writable);
		}

	}

	sysfs_remove_group(dev, grp);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__sysfs_remove_group__kobj__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*grp_ptr) {
			callee_marshal_kernel__sysfs_remove_group__grp__in(__pos, msg, ext, ctx, *grp_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int trmp_impl_probe(fptr_probe target, struct platform_device* pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct platform_device** pdev_ptr = &pdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct probe_call_ctx call_ctx = {pdev};
	__maybe_unused const struct probe_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *pdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__probe__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_probe);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__probe__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_probe)
int LCD_TRAMPOLINE_LINKAGE(trmp_probe) trmp_probe(struct platform_device* pdev)
{
	volatile fptr_impl_probe impl;
	fptr_probe target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_probe);
	impl = trmp_impl_probe;
	return impl(target, pdev);
}

int trmp_impl_remove(fptr_remove target, struct platform_device* pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct platform_device** pdev_ptr = &pdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct remove_call_ctx call_ctx = {pdev};
	__maybe_unused const struct remove_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *pdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__remove__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_remove);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__remove__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_remove)
int LCD_TRAMPOLINE_LINKAGE(trmp_remove) trmp_remove(struct platform_device* pdev)
{
	volatile fptr_impl_remove impl;
	fptr_remove target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_remove);
	impl = trmp_impl_remove;
	return impl(target, pdev);
}

void sysfs_create_group_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct kobject* dev = 0;
	struct attribute_group const* grp = 0;
	struct kobject** dev_ptr = &dev;
	struct attribute_group const** grp_ptr = &grp;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct sysfs_create_group_call_ctx call_ctx = {dev, grp};
	__maybe_unused struct sysfs_create_group_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct device* __device = glue_unpack(__pos, msg, ext, struct device*);
		*dev_ptr = &__device->kobj;
		if (*dev_ptr) {
			callee_unmarshal_kernel__sysfs_create_group__kobj__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*grp_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct attribute_group const*, sizeof(struct attribute_group));
		if (*grp_ptr) {
			struct attribute_group* writable = (struct attribute_group*)*grp_ptr;
			callee_unmarshal_kernel__sysfs_create_group__grp__in(__pos, msg, ext, ctx, writable);
		}

	}

	ret = sysfs_create_group(dev, grp);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__sysfs_create_group__kobj__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*grp_ptr) {
			callee_marshal_kernel__sysfs_create_group__grp__in(__pos, msg, ext, ctx, *grp_ptr);
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

void rdmsr_safe_on_cpu_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int cpu = 0;
	unsigned int msr_no = 0;
	unsigned int* l = 0;
	unsigned int* h = 0;
	unsigned int* cpu_ptr = &cpu;
	unsigned int* msr_no_ptr = &msr_no;
	unsigned int** l_ptr = &l;
	unsigned int** h_ptr = &h;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct rdmsr_safe_on_cpu_call_ctx call_ctx = {cpu, msr_no, l, h};
	__maybe_unused struct rdmsr_safe_on_cpu_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*cpu_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*msr_no_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*l_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned int*, sizeof(unsigned int));
		(void)l_ptr;
	}

	{
		*h_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned int*, sizeof(unsigned int));
		(void)h_ptr;
	}

	ret = rdmsr_safe_on_cpu(cpu, msr_no, l, h);

	*__pos = 0;
	{
	}

	{
	}

	{
		if (*l_ptr) {
			glue_pack(__pos, msg, ext, **l_ptr);
		}

	}

	{
		if (*h_ptr) {
			glue_pack(__pos, msg, ext, **h_ptr);
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

void rdmsr_on_cpu_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int cpu = 0;
	unsigned int msr_no = 0;
	unsigned int* l = 0;
	unsigned int* h = 0;
	unsigned int* cpu_ptr = &cpu;
	unsigned int* msr_no_ptr = &msr_no;
	unsigned int** l_ptr = &l;
	unsigned int** h_ptr = &h;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct rdmsr_on_cpu_call_ctx call_ctx = {cpu, msr_no, l, h};
	__maybe_unused struct rdmsr_on_cpu_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*cpu_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*msr_no_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*l_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned int*, sizeof(unsigned int));
		(void)l_ptr;
	}

	{
		*h_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned int*, sizeof(unsigned int));
		(void)h_ptr;
	}

	ret = rdmsr_on_cpu(cpu, msr_no, l, h);

	*__pos = 0;
	{
	}

	{
	}

	{
		if (*l_ptr) {
			glue_pack(__pos, msg, ext, **l_ptr);
		}

	}

	{
		if (*h_ptr) {
			glue_pack(__pos, msg, ext, **h_ptr);
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

void cpu_maps_update_begin_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	
	__maybe_unused struct cpu_maps_update_begin_call_ctx call_ctx = {};
	__maybe_unused struct cpu_maps_update_begin_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	cpu_maps_update_begin();

	*__pos = 0;
	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void cpu_maps_update_done_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	
	__maybe_unused struct cpu_maps_update_done_call_ctx call_ctx = {};
	__maybe_unused struct cpu_maps_update_done_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	cpu_maps_update_done();

	*__pos = 0;
	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void platform_driver_unregister_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct platform_driver* drv = 0;
	struct platform_driver** drv_ptr = &drv;
	
	__maybe_unused struct platform_driver_unregister_call_ctx call_ctx = {drv};
	__maybe_unused struct platform_driver_unregister_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*drv_ptr = glue_unpack_shadow(__pos, msg, ext, struct platform_driver*);
		if (*drv_ptr) {
			callee_unmarshal_kernel__platform_driver_unregister__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	platform_driver_unregister(drv);

	*__pos = 0;
	{
		if (*drv_ptr) {
			callee_marshal_kernel__platform_driver_unregister__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __platform_driver_register_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct platform_driver* drv = 0;
	struct module* owner = 0;
	struct platform_driver** drv_ptr = &drv;
	struct module** owner_ptr = &owner;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct __platform_driver_register_call_ctx call_ctx = {drv, owner};
	__maybe_unused struct __platform_driver_register_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*drv_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct platform_driver*, sizeof(struct platform_driver));
		if (*drv_ptr) {
			callee_unmarshal_kernel____platform_driver_register__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	{
		*owner_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct module*, sizeof(struct module));
		if (*owner_ptr) {
			callee_unmarshal_kernel____platform_driver_register__owner__in(__pos, msg, ext, ctx, *owner_ptr);
		}

	}

	ret = __platform_driver_register(drv, owner);

	*__pos = 0;
	{
		if (*drv_ptr) {
			callee_marshal_kernel____platform_driver_register__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	{
		if (*owner_ptr) {
			callee_marshal_kernel____platform_driver_register__owner__in(__pos, msg, ext, ctx, *owner_ptr);
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

void platform_device_alloc_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	char const* name = 0;
	int id = 0;
	char const** name_ptr = &name;
	int* id_ptr = &id;
	struct platform_device* ret = 0;
	struct platform_device** ret_ptr = &ret;
	
	__maybe_unused struct platform_device_alloc_call_ctx call_ctx = {name, id};
	__maybe_unused struct platform_device_alloc_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, sizeof(char) * glue_peek(__pos, msg, ext));
		if (*name_ptr) {
			char* writable = (char*)*name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(__pos, msg, ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, char);
			}

		}

	}

	{
		*id_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = platform_device_alloc(name, id);

	*__pos = 0;
	{
		(void)name_ptr;
	}

	{
	}

	{
		const void* __adjusted = *ret_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__platform_device_alloc__ret_platform_device__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void platform_device_add_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct platform_device* pdev = 0;
	struct platform_device** pdev_ptr = &pdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct platform_device_add_call_ctx call_ctx = {pdev};
	__maybe_unused struct platform_device_add_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack(__pos, msg, ext, struct platform_device*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__platform_device_add__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	ret = platform_device_add(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__platform_device_add__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
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

void platform_device_put_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct platform_device* pdev = 0;
	struct platform_device** pdev_ptr = &pdev;
	
	__maybe_unused struct platform_device_put_call_ctx call_ctx = {pdev};
	__maybe_unused struct platform_device_put_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack(__pos, msg, ext, struct platform_device*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__platform_device_put__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	platform_device_put(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__platform_device_put__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void platform_device_unregister_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct platform_device* pdev = 0;
	struct platform_device** pdev_ptr = &pdev;
	
	__maybe_unused struct platform_device_unregister_call_ctx call_ctx = {pdev};
	__maybe_unused struct platform_device_unregister_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack(__pos, msg, ext, struct platform_device*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__platform_device_unregister__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	platform_device_unregister(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__platform_device_unregister__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void x86_match_cpu_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct x86_cpu_id const* match = 0;
	struct x86_cpu_id const** match_ptr = &match;
	struct x86_cpu_id const* ret = 0;
	struct x86_cpu_id const** ret_ptr = &ret;
	
	__maybe_unused struct x86_match_cpu_call_ctx call_ctx = {match};
	__maybe_unused struct x86_match_cpu_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*match_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct x86_cpu_id const*, sizeof(struct x86_cpu_id) * glue_peek(__pos, msg, ext));
		if (*match_ptr) {
			struct x86_cpu_id* writable = (struct x86_cpu_id*)*match_ptr;
			size_t i, len;
			struct x86_cpu_id* array = writable;
			len = glue_unpack(__pos, msg, ext, size_t) - 1;
			memset(&array[len], 0x0, sizeof(array[len]));
			for (i = 0; i < len; ++i) {
				struct x86_cpu_id* element = &array[i];
				callee_unmarshal_kernel__x86_match_cpu__match__in(__pos, msg, ext, ctx, element);
			}

		}

	}

	ret = x86_match_cpu(match);

	*__pos = 0;
	{
		(void)match_ptr;
	}

	{
		const void* __adjusted = *ret_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__x86_match_cpu__ret_x86_cpu_id__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_get_domain_bus_and_slot_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int domain = 0;
	unsigned int bus = 0;
	unsigned int devfn = 0;
	int* domain_ptr = &domain;
	unsigned int* bus_ptr = &bus;
	unsigned int* devfn_ptr = &devfn;
	struct pci_dev* ret = 0;
	struct pci_dev** ret_ptr = &ret;
	
	__maybe_unused struct pci_get_domain_bus_and_slot_call_ctx call_ctx = {domain, bus, devfn};
	__maybe_unused struct pci_get_domain_bus_and_slot_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*domain_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*bus_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*devfn_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = pci_get_domain_bus_and_slot(domain, bus, devfn);

	*__pos = 0;
	{
	}

	{
	}

	{
	}

	{
		const void* __adjusted = *ret_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__pci_get_domain_bus_and_slot__ret_pci_dev__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void devm_hwmon_device_register_with_groups_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct device* dev = 0;
	char const* name = 0;
	void* drvdata = 0;
	struct attribute_group const** groups = 0;
	struct device** dev_ptr = &dev;
	char const** name_ptr = &name;
	void** drvdata_ptr = &drvdata;
	struct device* ret = 0;
	struct device** ret_ptr = &ret;
	
	__maybe_unused struct devm_hwmon_device_register_with_groups_call_ctx call_ctx = {dev, name, drvdata, groups};
	__maybe_unused struct devm_hwmon_device_register_with_groups_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct platform_device* __platform_device = glue_unpack(__pos, msg, ext, struct platform_device*);
		*dev_ptr = &__platform_device->dev;
		if (*dev_ptr) {
			callee_unmarshal_kernel__devm_hwmon_device_register_with_groups__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, sizeof(char) * glue_peek(__pos, msg, ext));
		if (*name_ptr) {
			char* writable = (char*)*name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(__pos, msg, ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, char);
			}

		}

	}

	{
		*drvdata_ptr = glue_unpack(__pos, msg, ext, void*);
		if (*drvdata_ptr) {
		}

	}

	ret = devm_hwmon_device_register_with_groups(dev, name, drvdata, groups);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__devm_hwmon_device_register_with_groups__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		(void)name_ptr;
	}

	{
		(void)drvdata_ptr;
	}

	{
		const void* __adjusted = *ret_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__devm_hwmon_device_register_with_groups__ret_device__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext)
{
	switch(id) {
	case RPC_ID_shared_mem_init:
		glue_user_trace("shared_mem_init\n");
		shared_mem_init_callee(msg, ext);
		break;

	case RPC_ID_sysfs_remove_group:
		glue_user_trace("sysfs_remove_group\n");
		sysfs_remove_group_callee(msg, ext);
		break;

	case RPC_ID_sysfs_create_group:
		glue_user_trace("sysfs_create_group\n");
		sysfs_create_group_callee(msg, ext);
		break;

	case RPC_ID_rdmsr_safe_on_cpu:
		glue_user_trace("rdmsr_safe_on_cpu\n");
		rdmsr_safe_on_cpu_callee(msg, ext);
		break;

	case RPC_ID_rdmsr_on_cpu:
		glue_user_trace("rdmsr_on_cpu\n");
		rdmsr_on_cpu_callee(msg, ext);
		break;

	case RPC_ID_cpu_maps_update_begin:
		glue_user_trace("cpu_maps_update_begin\n");
		cpu_maps_update_begin_callee(msg, ext);
		break;

	case RPC_ID_cpu_maps_update_done:
		glue_user_trace("cpu_maps_update_done\n");
		cpu_maps_update_done_callee(msg, ext);
		break;

	case RPC_ID_platform_driver_unregister:
		glue_user_trace("platform_driver_unregister\n");
		platform_driver_unregister_callee(msg, ext);
		break;

	case RPC_ID___platform_driver_register:
		glue_user_trace("__platform_driver_register\n");
		__platform_driver_register_callee(msg, ext);
		break;

	case RPC_ID_platform_device_alloc:
		glue_user_trace("platform_device_alloc\n");
		platform_device_alloc_callee(msg, ext);
		break;

	case RPC_ID_platform_device_add:
		glue_user_trace("platform_device_add\n");
		platform_device_add_callee(msg, ext);
		break;

	case RPC_ID_platform_device_put:
		glue_user_trace("platform_device_put\n");
		platform_device_put_callee(msg, ext);
		break;

	case RPC_ID_platform_device_unregister:
		glue_user_trace("platform_device_unregister\n");
		platform_device_unregister_callee(msg, ext);
		break;

	case RPC_ID_x86_match_cpu:
		glue_user_trace("x86_match_cpu\n");
		x86_match_cpu_callee(msg, ext);
		break;

	case RPC_ID_pci_get_domain_bus_and_slot:
		glue_user_trace("pci_get_domain_bus_and_slot\n");
		pci_get_domain_bus_and_slot_callee(msg, ext);
		break;

	case RPC_ID_devm_hwmon_device_register_with_groups:
		glue_user_trace("devm_hwmon_device_register_with_groups\n");
		devm_hwmon_device_register_with_groups_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

