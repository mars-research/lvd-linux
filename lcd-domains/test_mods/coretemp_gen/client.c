#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void sysfs_remove_group(struct kobject* dev, struct attribute_group const* grp)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct kobject** dev_ptr = &dev;
	struct attribute_group const** grp_ptr = &grp;
	
	__maybe_unused const struct sysfs_remove_group_call_ctx call_ctx = {dev, grp};
	__maybe_unused const struct sysfs_remove_group_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct device* __adjusted = container_of(*dev_ptr, struct device, kobj);
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__sysfs_remove_group__kobj__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		const void* __adjusted = *grp_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*grp_ptr) {
			caller_marshal_kernel__sysfs_remove_group__grp__in(__pos, msg, ext, ctx, *grp_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_sysfs_remove_group);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__sysfs_remove_group__kobj__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*grp_ptr) {
			struct attribute_group* writable = (struct attribute_group*)*grp_ptr;
			caller_unmarshal_kernel__sysfs_remove_group__grp__in(__pos, msg, ext, ctx, writable);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void probe_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_probe function_ptr = glue_unpack(__pos, msg, ext, fptr_probe);
	struct platform_device* pdev = 0;
	struct platform_device** pdev_ptr = &pdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct probe_call_ctx call_ctx = {pdev};
	__maybe_unused struct probe_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct platform_device*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__probe__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	ret = function_ptr(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__probe__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
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

void remove_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_remove function_ptr = glue_unpack(__pos, msg, ext, fptr_remove);
	struct platform_device* pdev = 0;
	struct platform_device** pdev_ptr = &pdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct remove_call_ctx call_ctx = {pdev};
	__maybe_unused struct remove_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct platform_device*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__remove__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	ret = function_ptr(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__remove__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
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

int sysfs_create_group(struct kobject* dev, struct attribute_group const* grp)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct kobject** dev_ptr = &dev;
	struct attribute_group const** grp_ptr = &grp;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct sysfs_create_group_call_ctx call_ctx = {dev, grp};
	__maybe_unused const struct sysfs_create_group_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct device* __adjusted = container_of(*dev_ptr, struct device, kobj);
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__sysfs_create_group__kobj__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		const void* __adjusted = *grp_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*grp_ptr) {
			caller_marshal_kernel__sysfs_create_group__grp__in(__pos, msg, ext, ctx, *grp_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_sysfs_create_group);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__sysfs_create_group__kobj__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*grp_ptr) {
			struct attribute_group* writable = (struct attribute_group*)*grp_ptr;
			caller_unmarshal_kernel__sysfs_create_group__grp__in(__pos, msg, ext, ctx, writable);
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
		const void* __adjusted = *l_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		(void)l_ptr;
	}

	{
		const void* __adjusted = *h_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		(void)h_ptr;
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
		const void* __adjusted = *l_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		(void)l_ptr;
	}

	{
		const void* __adjusted = *h_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		(void)h_ptr;
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
		const void* __adjusted = *drv_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
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
	struct module** owner_ptr = &owner;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct __platform_driver_register_call_ctx call_ctx = {drv, owner};
	__maybe_unused const struct __platform_driver_register_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *drv_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*drv_ptr) {
			caller_marshal_kernel____platform_driver_register__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	{
		const void* __adjusted = *owner_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*owner_ptr) {
			caller_marshal_kernel____platform_driver_register__owner__in(__pos, msg, ext, ctx, *owner_ptr);
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
		if (*owner_ptr) {
			caller_unmarshal_kernel____platform_driver_register__owner__in(__pos, msg, ext, ctx, *owner_ptr);
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
		const void* __adjusted = *name_ptr;
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
		*ret_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct platform_device*, sizeof(struct platform_device));
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
		const void* __adjusted = *pdev_ptr;
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
		const void* __adjusted = *pdev_ptr;
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
		const void* __adjusted = *pdev_ptr;
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
		const void* __adjusted = *match_ptr;
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
		*ret_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct x86_cpu_id const*, sizeof(struct x86_cpu_id));
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
		*ret_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct pci_dev*, sizeof(struct pci_dev));
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
		const void* __adjusted = *name_ptr;
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
		const void* __adjusted = *drvdata_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
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
		*ret_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct device*, sizeof(struct device));
		if (*ret_ptr) {
			caller_unmarshal_kernel__devm_hwmon_device_register_with_groups__ret_device__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
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

	case RPC_ID_probe:
		glue_user_trace("probe\n");
		probe_callee(msg, ext);
		break;

	case RPC_ID_remove:
		glue_user_trace("remove\n");
		remove_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

