#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

struct mei_device* global_mei_dev;

int trmp_impl_pci_driver_probe(fptr_pci_driver_probe target, struct pci_dev* pdev, struct pci_device_id* ent)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	struct pci_device_id** ent_ptr = &ent;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_driver_probe_call_ctx call_ctx = {pdev, ent};
	__maybe_unused const struct pci_driver_probe_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}
	printk("%s, pdev %p\n", __func__, pdev);

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *pdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_driver_probe__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		const void* __adjusted = *ent_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ent_ptr) {
			caller_marshal_kernel__pci_driver_probe__ent__in(__pos, msg, ext, ctx, *ent_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_pci_driver_probe);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_driver_probe__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		if (*ent_ptr) {
			caller_unmarshal_kernel__pci_driver_probe__ent__in(__pos, msg, ext, ctx, *ent_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_pci_driver_probe)
int LCD_TRAMPOLINE_LINKAGE(trmp_pci_driver_probe) trmp_pci_driver_probe(struct pci_dev* pdev, struct pci_device_id* ent)
{
	volatile fptr_impl_pci_driver_probe impl;
	fptr_pci_driver_probe target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_pci_driver_probe);
	impl = trmp_impl_pci_driver_probe;
	return impl(target, pdev, ent);
}

void trmp_impl_pci_driver_remove(fptr_pci_driver_remove target, struct pci_dev* pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused const struct pci_driver_remove_call_ctx call_ctx = {pdev};
	__maybe_unused const struct pci_driver_remove_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *pdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_driver_remove__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_pci_driver_remove);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_driver_remove__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_pci_driver_remove)
void LCD_TRAMPOLINE_LINKAGE(trmp_pci_driver_remove) trmp_pci_driver_remove(struct pci_dev* pdev)
{
	volatile fptr_impl_pci_driver_remove impl;
	fptr_pci_driver_remove target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_pci_driver_remove);
	impl = trmp_impl_pci_driver_remove;
	return impl(target, pdev);
}

void trmp_impl_pci_driver_shutdown(fptr_pci_driver_shutdown target, struct pci_dev* pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused const struct pci_driver_shutdown_call_ctx call_ctx = {pdev};
	__maybe_unused const struct pci_driver_shutdown_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *pdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_driver_shutdown__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_pci_driver_shutdown);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_driver_shutdown__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_pci_driver_shutdown)
void LCD_TRAMPOLINE_LINKAGE(trmp_pci_driver_shutdown) trmp_pci_driver_shutdown(struct pci_dev* pdev)
{
	volatile fptr_impl_pci_driver_shutdown impl;
	fptr_pci_driver_shutdown target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_pci_driver_shutdown);
	impl = trmp_impl_pci_driver_shutdown;
	return impl(target, pdev);
}

void pci_set_master_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused struct pci_set_master_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_set_master_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_set_master__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	pci_set_master(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_set_master__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_request_regions_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* pdev = 0;
	char const* res_name = 0;
	struct pci_dev** pdev_ptr = &pdev;
	char const** res_name_ptr = &res_name;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_request_regions_call_ctx call_ctx = {pdev, res_name};
	__maybe_unused struct pci_request_regions_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_request_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		*res_name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, (sizeof(char) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
		if (*res_name_ptr) {
			char* writable = (char*)*res_name_ptr;
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

	ret = pci_request_regions(pdev, res_name);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_request_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		(void)res_name_ptr;
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_release_regions_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* pdev = 0;
	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused struct pci_release_regions_call_ctx call_ctx = {pdev};
	__maybe_unused struct pci_release_regions_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_release_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	pci_release_regions(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_release_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_dev_run_wake_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct pci_dev_run_wake_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_dev_run_wake_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_dev_run_wake__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = pci_dev_run_wake(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_dev_run_wake__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void pci_enable_device_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_enable_device_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_enable_device_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_enable_device__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = pci_enable_device(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_enable_device__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void pci_unregister_driver_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_driver* drv = 0;
	struct pci_driver** drv_ptr = &drv;
	
	__maybe_unused struct pci_unregister_driver_call_ctx call_ctx = {drv};
	__maybe_unused struct pci_unregister_driver_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*drv_ptr = glue_unpack_shadow(__pos, msg, ext, struct pci_driver*);
		if (*drv_ptr) {
			callee_unmarshal_kernel__pci_unregister_driver__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	pci_unregister_driver(drv);

	*__pos = 0;
	{
		if (*drv_ptr) {
			callee_marshal_kernel__pci_unregister_driver__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

		glue_remove_shadow(*drv_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __pci_register_driver_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_driver* drv = 0;
	struct module* owner = 0;
	char const* mod_name = 0;
	struct pci_driver** drv_ptr = &drv;
	struct module** owner_ptr = &owner;
	char const** mod_name_ptr = &mod_name;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct __pci_register_driver_call_ctx call_ctx = {drv, owner, mod_name};
	__maybe_unused struct __pci_register_driver_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*drv_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct pci_driver*, (sizeof(struct pci_driver)), (DEFAULT_GFP_FLAGS));
		if (*drv_ptr) {
			callee_unmarshal_kernel____pci_register_driver__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	{
		*owner_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct module*, (sizeof(struct module)), (DEFAULT_GFP_FLAGS));
		if (*owner_ptr) {
			callee_unmarshal_kernel____pci_register_driver__owner__in(__pos, msg, ext, ctx, *owner_ptr);
		}

	}

	{
		*mod_name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, (sizeof(char) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
		if (*mod_name_ptr) {
			char* writable = (char*)*mod_name_ptr;
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

	ret = __pci_register_driver(drv, owner, mod_name);

	*__pos = 0;
	{
		if (*drv_ptr) {
			callee_marshal_kernel____pci_register_driver__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	{
		if (*owner_ptr) {
			callee_marshal_kernel____pci_register_driver__owner__in(__pos, msg, ext, ctx, *owner_ptr);
		}

	}

	{
		(void)mod_name_ptr;
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_hbm_pg_resume_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused struct mei_hbm_pg_resume_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hbm_pg_resume_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hbm_pg_resume__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	mei_hbm_pg_resume(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hbm_pg_resume__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_hbm_pg_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device* dev = 0;
	unsigned char pg_cmd = 0;
	struct mei_device** dev_ptr = &dev;
	unsigned char* pg_cmd_ptr = &pg_cmd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mei_hbm_pg_call_ctx call_ctx = {dev, pg_cmd};
	__maybe_unused struct mei_hbm_pg_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hbm_pg__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*pg_cmd_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	ret = mei_hbm_pg(dev, pg_cmd);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hbm_pg__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_device_init_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device* dev = 0;
	struct device* device = 0;
	struct mei_hw_ops const* ops = 0;
	struct mei_device** dev_ptr = &dev;
	struct device** device_ptr = &device;
	struct mei_hw_ops const** ops_ptr = &ops;
	
	__maybe_unused struct mei_device_init_call_ctx call_ctx = {dev, device, ops};
	__maybe_unused struct mei_device_init_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct mei_device*, (sizeof(struct mei_device) + sizeof(struct mei_me_hw)), (DEFAULT_GFP_FLAGS));
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_device_init__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}
		global_mei_dev = *dev_ptr;
	}

	{
		struct pci_dev* __pci_dev = glue_unpack(__pos, msg, ext, struct pci_dev*);
		*device_ptr = &__pci_dev->dev;
		if (*device_ptr) {
			callee_unmarshal_kernel__mei_device_init__device__in(__pos, msg, ext, ctx, *device_ptr);
		}

	}

	{
		*ops_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct mei_hw_ops const*, (sizeof(struct mei_hw_ops)), (DEFAULT_GFP_FLAGS));
		if (*ops_ptr) {
			struct mei_hw_ops* writable = (struct mei_hw_ops*)*ops_ptr;
			callee_unmarshal_kernel___global_mei_hw_ops__in(__pos, msg, ext, writable);
		}

	}

	mei_device_init(dev, device, ops);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_device_init__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*device_ptr) {
			callee_marshal_kernel__mei_device_init__device__in(__pos, msg, ext, ctx, *device_ptr);
		}

	}

	{
		if (*ops_ptr) {
			callee_marshal_kernel___global_mei_hw_ops__in(__pos, msg, ext, *ops_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_stop_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused struct mei_stop_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_stop_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_stop__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	mei_stop(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_stop__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_cancel_work_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused struct mei_cancel_work_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_cancel_work_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_cancel_work__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	mei_cancel_work(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_cancel_work__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_start_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mei_start_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_start_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_start__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = mei_start(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_start__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void pci_iomap_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* pdev = 0;
	int bar = 0;
	unsigned long maxlen = 0;
	struct pci_dev** pdev_ptr = &pdev;
	int* bar_ptr = &bar;
	unsigned long* maxlen_ptr = &maxlen;
	cptr_t resource_cptr;
	__maybe_unused unsigned int resource_len;
	cptr_t lcd_resource_cptr;
	void* ret = 0;
	//void** ret_ptr = &ret;
	
	__maybe_unused struct pci_iomap_call_ctx call_ctx = {pdev, bar, maxlen};
	__maybe_unused struct pci_iomap_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		lcd_resource_cptr.cptr = glue_unpack(__pos, msg, ext, uint64_t);
	}

	{
		*pdev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_iomap__dev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		*bar_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*maxlen_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	ret = pci_iomap(pdev, bar, maxlen);

	printk("%s, pci_iomap returned %p\n", __func__, ret);

	*__pos = 0;

	{
		lcd_volunteer_dev_mem(__gpa((uint64_t)pci_resource_start(pdev, bar)), get_order(pci_resource_len(pdev, bar)), &resource_cptr);
		copy_msg_cap_vmfunc(current->lcd, current->vmfunc_lcd, resource_cptr, lcd_resource_cptr);
		glue_pack(__pos, msg, ext, pci_resource_len(pdev, bar));
	}

	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_iomap__dev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
	}

	{
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_bus_read_config_dword_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_bus* bus = 0;
	unsigned int devfn = 0;
	int pos = 0;
	unsigned int* value = 0;
	struct pci_bus** bus_ptr = &bus;
	unsigned int* devfn_ptr = &devfn;
	int* pos_ptr = &pos;
	unsigned int** value_ptr = &value;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_bus_read_config_dword_call_ctx call_ctx = {bus, devfn, pos, value};
	__maybe_unused struct pci_bus_read_config_dword_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*bus_ptr = glue_unpack(__pos, msg, ext, struct pci_bus*);
		if (*bus_ptr) {
			callee_unmarshal_kernel__pci_bus_read_config_dword__bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		*devfn_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*pos_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*value_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned int*, (sizeof(unsigned int)), (DEFAULT_GFP_FLAGS));
		(void)value_ptr;
	}

	ret = pci_bus_read_config_dword(bus, devfn, pos, value);

	*__pos = 0;
	{
		if (*bus_ptr) {
			callee_marshal_kernel__pci_bus_read_config_dword__bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
	}

	{
	}

	{
		if (*value_ptr) {
			glue_pack(__pos, msg, ext, **value_ptr);
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

bool trmp_impl_mei_hw_ops_host_is_ready(fptr_mei_hw_ops_host_is_ready target, struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hw_ops_host_is_ready_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hw_ops_host_is_ready_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_host_is_ready__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_host_is_ready);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_host_is_ready__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_host_is_ready)
bool LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_host_is_ready) trmp_mei_hw_ops_host_is_ready(struct mei_device* dev)
{
	volatile fptr_impl_mei_hw_ops_host_is_ready impl;
	fptr_mei_hw_ops_host_is_ready target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_host_is_ready);
	impl = trmp_impl_mei_hw_ops_host_is_ready;
	return impl(target, dev);
}

bool trmp_impl_mei_hw_ops_hw_is_ready(fptr_mei_hw_ops_hw_is_ready target, struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hw_ops_hw_is_ready_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hw_ops_hw_is_ready_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_hw_is_ready__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_hw_is_ready);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_hw_is_ready__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_hw_is_ready)
bool LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_hw_is_ready) trmp_mei_hw_ops_hw_is_ready(struct mei_device* dev)
{
	volatile fptr_impl_mei_hw_ops_hw_is_ready impl;
	fptr_mei_hw_ops_hw_is_ready target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_hw_is_ready);
	impl = trmp_impl_mei_hw_ops_hw_is_ready;
	return impl(target, dev);
}

int trmp_impl_mei_hw_ops_hw_reset(fptr_mei_hw_ops_hw_reset target, struct mei_device* dev, bool intr_enable)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	bool* intr_enable_ptr = &intr_enable;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hw_ops_hw_reset_call_ctx call_ctx = {dev, intr_enable};
	__maybe_unused const struct mei_hw_ops_hw_reset_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_hw_reset__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *intr_enable_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_hw_reset);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_hw_reset__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_hw_reset)
int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_hw_reset) trmp_mei_hw_ops_hw_reset(struct mei_device* dev, bool intr_enable)
{
	volatile fptr_impl_mei_hw_ops_hw_reset impl;
	fptr_mei_hw_ops_hw_reset target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_hw_reset);
	impl = trmp_impl_mei_hw_ops_hw_reset;
	return impl(target, dev, intr_enable);
}

int trmp_impl_mei_hw_ops_hw_start(fptr_mei_hw_ops_hw_start target, struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hw_ops_hw_start_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hw_ops_hw_start_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_hw_start__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_hw_start);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_hw_start__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_hw_start)
int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_hw_start) trmp_mei_hw_ops_hw_start(struct mei_device* dev)
{
	volatile fptr_impl_mei_hw_ops_hw_start impl;
	fptr_mei_hw_ops_hw_start target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_hw_start);
	impl = trmp_impl_mei_hw_ops_hw_start;
	return impl(target, dev);
}

void trmp_impl_mei_hw_ops_hw_config(fptr_mei_hw_ops_hw_config target, struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused const struct mei_hw_ops_hw_config_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hw_ops_hw_config_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_hw_config__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_hw_config);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_hw_config__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_hw_config)
void LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_hw_config) trmp_mei_hw_ops_hw_config(struct mei_device* dev)
{
	volatile fptr_impl_mei_hw_ops_hw_config impl;
	fptr_mei_hw_ops_hw_config target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_hw_config);
	impl = trmp_impl_mei_hw_ops_hw_config;
	return impl(target, dev);
}

int trmp_impl_mei_hw_ops_fw_status(fptr_mei_hw_ops_fw_status target, struct mei_device* dev, struct mei_fw_status* fw_status)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	struct mei_fw_status** fw_status_ptr = &fw_status;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hw_ops_fw_status_call_ctx call_ctx = {dev, fw_status};
	__maybe_unused const struct mei_hw_ops_fw_status_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_fw_status__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		const void* __adjusted = *fw_status_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*fw_status_ptr) {
			caller_marshal_kernel__mei_hw_ops_fw_status__fw_status__in(__pos, msg, ext, ctx, *fw_status_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_fw_status);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_fw_status__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*fw_status_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_fw_status__fw_status__in(__pos, msg, ext, ctx, *fw_status_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_fw_status)
int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_fw_status) trmp_mei_hw_ops_fw_status(struct mei_device* dev, struct mei_fw_status* fw_status)
{
	volatile fptr_impl_mei_hw_ops_fw_status impl;
	fptr_mei_hw_ops_fw_status target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_fw_status);
	impl = trmp_impl_mei_hw_ops_fw_status;
	return impl(target, dev, fw_status);
}

unsigned int trmp_impl_mei_hw_ops_pg_state(fptr_mei_hw_ops_pg_state target, struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hw_ops_pg_state_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hw_ops_pg_state_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_pg_state__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_pg_state);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_pg_state__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_pg_state)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_pg_state) trmp_mei_hw_ops_pg_state(struct mei_device* dev)
{
	volatile fptr_impl_mei_hw_ops_pg_state impl;
	fptr_mei_hw_ops_pg_state target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_pg_state);
	impl = trmp_impl_mei_hw_ops_pg_state;
	return impl(target, dev);
}

bool trmp_impl_mei_hw_ops_pg_in_transition(fptr_mei_hw_ops_pg_in_transition target, struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hw_ops_pg_in_transition_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hw_ops_pg_in_transition_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_pg_in_transition__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_pg_in_transition);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_pg_in_transition__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_pg_in_transition)
bool LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_pg_in_transition) trmp_mei_hw_ops_pg_in_transition(struct mei_device* dev)
{
	volatile fptr_impl_mei_hw_ops_pg_in_transition impl;
	fptr_mei_hw_ops_pg_in_transition target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_pg_in_transition);
	impl = trmp_impl_mei_hw_ops_pg_in_transition;
	return impl(target, dev);
}

bool trmp_impl_mei_hw_ops_pg_is_enabled(fptr_mei_hw_ops_pg_is_enabled target, struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hw_ops_pg_is_enabled_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hw_ops_pg_is_enabled_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_pg_is_enabled__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_pg_is_enabled);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_pg_is_enabled__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_pg_is_enabled)
bool LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_pg_is_enabled) trmp_mei_hw_ops_pg_is_enabled(struct mei_device* dev)
{
	volatile fptr_impl_mei_hw_ops_pg_is_enabled impl;
	fptr_mei_hw_ops_pg_is_enabled target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_pg_is_enabled);
	impl = trmp_impl_mei_hw_ops_pg_is_enabled;
	return impl(target, dev);
}

void trmp_impl_mei_hw_ops_intr_clear(fptr_mei_hw_ops_intr_clear target, struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused const struct mei_hw_ops_intr_clear_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hw_ops_intr_clear_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_intr_clear__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_intr_clear);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_intr_clear__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_intr_clear)
void LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_intr_clear) trmp_mei_hw_ops_intr_clear(struct mei_device* dev)
{
	volatile fptr_impl_mei_hw_ops_intr_clear impl;
	fptr_mei_hw_ops_intr_clear target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_intr_clear);
	impl = trmp_impl_mei_hw_ops_intr_clear;
	return impl(target, dev);
}

void trmp_impl_mei_hw_ops_intr_enable(fptr_mei_hw_ops_intr_enable target, struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused const struct mei_hw_ops_intr_enable_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hw_ops_intr_enable_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_intr_enable__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_intr_enable);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_intr_enable__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_intr_enable)
void LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_intr_enable) trmp_mei_hw_ops_intr_enable(struct mei_device* dev)
{
	volatile fptr_impl_mei_hw_ops_intr_enable impl;
	fptr_mei_hw_ops_intr_enable target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_intr_enable);
	impl = trmp_impl_mei_hw_ops_intr_enable;
	return impl(target, dev);
}

void trmp_impl_mei_hw_ops_intr_disable(fptr_mei_hw_ops_intr_disable target, struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused const struct mei_hw_ops_intr_disable_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hw_ops_intr_disable_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_intr_disable__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_intr_disable);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_intr_disable__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_intr_disable)
void LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_intr_disable) trmp_mei_hw_ops_intr_disable(struct mei_device* dev)
{
	volatile fptr_impl_mei_hw_ops_intr_disable impl;
	fptr_mei_hw_ops_intr_disable target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_intr_disable);
	impl = trmp_impl_mei_hw_ops_intr_disable;
	return impl(target, dev);
}

int trmp_impl_mei_hw_ops_hbuf_free_slots(fptr_mei_hw_ops_hbuf_free_slots target, struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hw_ops_hbuf_free_slots_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hw_ops_hbuf_free_slots_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_hbuf_free_slots__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_hbuf_free_slots);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_hbuf_free_slots__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_hbuf_free_slots)
int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_hbuf_free_slots) trmp_mei_hw_ops_hbuf_free_slots(struct mei_device* dev)
{
	volatile fptr_impl_mei_hw_ops_hbuf_free_slots impl;
	fptr_mei_hw_ops_hbuf_free_slots target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_hbuf_free_slots);
	impl = trmp_impl_mei_hw_ops_hbuf_free_slots;
	return impl(target, dev);
}

bool trmp_impl_mei_hw_ops_hbuf_is_ready(fptr_mei_hw_ops_hbuf_is_ready target, struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hw_ops_hbuf_is_ready_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hw_ops_hbuf_is_ready_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_hbuf_is_ready__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_hbuf_is_ready);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_hbuf_is_ready__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_hbuf_is_ready)
bool LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_hbuf_is_ready) trmp_mei_hw_ops_hbuf_is_ready(struct mei_device* dev)
{
	volatile fptr_impl_mei_hw_ops_hbuf_is_ready impl;
	fptr_mei_hw_ops_hbuf_is_ready target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_hbuf_is_ready);
	impl = trmp_impl_mei_hw_ops_hbuf_is_ready;
	return impl(target, dev);
}

unsigned long trmp_impl_mei_hw_ops_hbuf_max_len(fptr_mei_hw_ops_hbuf_max_len target, struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hw_ops_hbuf_max_len_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hw_ops_hbuf_max_len_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_hbuf_max_len__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_hbuf_max_len);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_hbuf_max_len__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_hbuf_max_len)
unsigned long LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_hbuf_max_len) trmp_mei_hw_ops_hbuf_max_len(struct mei_device* dev)
{
	volatile fptr_impl_mei_hw_ops_hbuf_max_len impl;
	fptr_mei_hw_ops_hbuf_max_len target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_hbuf_max_len);
	impl = trmp_impl_mei_hw_ops_hbuf_max_len;
	return impl(target, dev);
}

int trmp_impl_mei_hw_ops_write(fptr_mei_hw_ops_write target, struct mei_device* dev, struct mei_msg_hdr* header, unsigned char* buf)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	struct mei_msg_hdr** header_ptr = &header;
	unsigned char** buf_ptr = &buf;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hw_ops_write_call_ctx call_ctx = {dev, header, buf};
	__maybe_unused const struct mei_hw_ops_write_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_write__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		const void* __adjusted = *header_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*header_ptr) {
			caller_marshal_kernel__mei_hw_ops_write__header__in(__pos, msg, ext, ctx, *header_ptr);
		}

	}

	{
		const void* __adjusted = *buf_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*buf_ptr) {
			glue_pack(__pos, msg, ext, **buf_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_write);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_write__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*header_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_write__header__in(__pos, msg, ext, ctx, *header_ptr);
		}

	}

	{
		(void)buf_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_write)
int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_write) trmp_mei_hw_ops_write(struct mei_device* dev, struct mei_msg_hdr* header, unsigned char* buf)
{
	volatile fptr_impl_mei_hw_ops_write impl;
	fptr_mei_hw_ops_write target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_write);
	impl = trmp_impl_mei_hw_ops_write;
	return impl(target, dev, header, buf);
}

int trmp_impl_mei_hw_ops_rdbuf_full_slots(fptr_mei_hw_ops_rdbuf_full_slots target, struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hw_ops_rdbuf_full_slots_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hw_ops_rdbuf_full_slots_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_rdbuf_full_slots__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_rdbuf_full_slots);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_rdbuf_full_slots__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_rdbuf_full_slots)
int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_rdbuf_full_slots) trmp_mei_hw_ops_rdbuf_full_slots(struct mei_device* dev)
{
	volatile fptr_impl_mei_hw_ops_rdbuf_full_slots impl;
	fptr_mei_hw_ops_rdbuf_full_slots target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_rdbuf_full_slots);
	impl = trmp_impl_mei_hw_ops_rdbuf_full_slots;
	return impl(target, dev);
}

unsigned int trmp_impl_mei_hw_ops_read_hdr(fptr_mei_hw_ops_read_hdr target, struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hw_ops_read_hdr_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hw_ops_read_hdr_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_read_hdr__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_read_hdr);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_read_hdr__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_read_hdr)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_read_hdr) trmp_mei_hw_ops_read_hdr(struct mei_device* dev)
{
	volatile fptr_impl_mei_hw_ops_read_hdr impl;
	fptr_mei_hw_ops_read_hdr target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_read_hdr);
	impl = trmp_impl_mei_hw_ops_read_hdr;
	return impl(target, dev);
}

int trmp_impl_mei_hw_ops_read(fptr_mei_hw_ops_read target, struct mei_device* dev, unsigned char* buffer, unsigned long buffer_length)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	unsigned char** buffer_ptr = &buffer;
	unsigned long* buffer_length_ptr = &buffer_length;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hw_ops_read_call_ctx call_ctx = {dev, buffer, buffer_length};
	__maybe_unused const struct mei_hw_ops_read_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hw_ops_read__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		const void* __adjusted = *buffer_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*buffer_ptr) {
			glue_pack(__pos, msg, ext, **buffer_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *buffer_length_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_mei_hw_ops_read);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hw_ops_read__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		(void)buffer_ptr;
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_mei_hw_ops_read)
int LCD_TRAMPOLINE_LINKAGE(trmp_mei_hw_ops_read) trmp_mei_hw_ops_read(struct mei_device* dev, unsigned char* buffer, unsigned long buffer_length)
{
	volatile fptr_impl_mei_hw_ops_read impl;
	fptr_mei_hw_ops_read target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mei_hw_ops_read);
	impl = trmp_impl_mei_hw_ops_read;
	return impl(target, dev, buffer, buffer_length);
}

void schedule_timeout_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	long timeout = 0;
	long* timeout_ptr = &timeout;
	long ret = 0;
	long* ret_ptr = &ret;
	
	__maybe_unused struct schedule_timeout_call_ctx call_ctx = {timeout};
	__maybe_unused struct schedule_timeout_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*timeout_ptr = glue_unpack(__pos, msg, ext, long);
	}

	ret = schedule_timeout(timeout);

	*__pos = 0;
	{
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void finish_wait_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct __wait_queue_head* wq = 0;
	struct __wait_queue* wait = 0;
	struct __wait_queue_head** wq_ptr = &wq;
	struct __wait_queue** wait_ptr = &wait;
	
	__maybe_unused struct finish_wait_call_ctx call_ctx = {wq, wait};
	__maybe_unused struct finish_wait_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct mei_device* __mei_device = glue_unpack_shadow(__pos, msg, ext, struct mei_device*);
		*wq_ptr = &__mei_device->wait_hw_ready;
		if (*wq_ptr) {
			callee_unmarshal_kernel__finish_wait__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
		*wait_ptr = glue_unpack_shadow(__pos, msg, ext, struct __wait_queue*);
		if (*wait_ptr) {
			callee_unmarshal_kernel__finish_wait____wait_queue__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	finish_wait(wq, wait);

	*__pos = 0;
	{
		if (*wq_ptr) {
			callee_marshal_kernel__finish_wait__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
		if (*wait_ptr) {
			callee_marshal_kernel__finish_wait____wait_queue__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_deregister_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused struct mei_deregister_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_deregister_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_deregister__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	mei_deregister(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_deregister__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_register_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device* dev = 0;
	struct device* parent = 0;
	struct mei_device** dev_ptr = &dev;
	struct device** parent_ptr = &parent;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mei_register_call_ctx call_ctx = {dev, parent};
	__maybe_unused struct mei_register_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_register__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		struct pci_dev* __pci_dev = glue_unpack(__pos, msg, ext, struct pci_dev*);
		*parent_ptr = &__pci_dev->dev;
		if (*parent_ptr) {
			callee_unmarshal_kernel__mei_register__parent__in(__pos, msg, ext, ctx, *parent_ptr);
		}

	}

	ret = mei_register(dev, parent);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_register__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*parent_ptr) {
			callee_marshal_kernel__mei_register__parent__in(__pos, msg, ext, ctx, *parent_ptr);
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

void prepare_to_wait_event_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct __wait_queue_head* wq = 0;
	struct __wait_queue* wait = 0;
	int state = 0;
	struct __wait_queue_head** wq_ptr = &wq;
	struct __wait_queue** wait_ptr = &wait;
	int* state_ptr = &state;
	long ret = 0;
	long* ret_ptr = &ret;
	
	__maybe_unused struct prepare_to_wait_event_call_ctx call_ctx = {wq, wait, state};
	__maybe_unused struct prepare_to_wait_event_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct mei_device* __mei_device = glue_unpack_shadow(__pos, msg, ext, struct mei_device*);
		*wq_ptr = &__mei_device->wait_hw_ready;
		if (*wq_ptr) {
			callee_unmarshal_kernel__prepare_to_wait_event__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
		*wait_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct __wait_queue*, (sizeof(struct __wait_queue)), (DEFAULT_GFP_FLAGS));
		if (*wait_ptr) {
			callee_unmarshal_kernel__prepare_to_wait_event____wait_queue__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	{
		*state_ptr = glue_unpack(__pos, msg, ext, int);
	}

	INIT_LIST_HEAD(&wait->task_list);
	ret = prepare_to_wait_event(wq, wait, state);

	*__pos = 0;
	{
		if (*wq_ptr) {
			callee_marshal_kernel__prepare_to_wait_event__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
		if (*wait_ptr) {
			callee_marshal_kernel__prepare_to_wait_event____wait_queue__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __msecs_to_jiffies_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int m = 0;
	unsigned int* m_ptr = &m;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused struct __msecs_to_jiffies_call_ctx call_ctx = {m};
	__maybe_unused struct __msecs_to_jiffies_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*m_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = __msecs_to_jiffies(m);

	*__pos = 0;
	{
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void queue_delayed_work_on_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int cpu = 0;
	struct workqueue_struct* wq = 0;
	struct delayed_work* dwork = 0;
	unsigned long delay = 0;
	int* cpu_ptr = &cpu;
	struct delayed_work** dwork_ptr = &dwork;
	unsigned long* delay_ptr = &delay;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct queue_delayed_work_on_call_ctx call_ctx = {cpu, wq, dwork, delay};
	__maybe_unused struct queue_delayed_work_on_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*cpu_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		struct mei_device* __mei_device = glue_unpack(__pos, msg, ext, struct mei_device*);
		*dwork_ptr = &__mei_device->timer_work;
		if (*dwork_ptr) {
			callee_unmarshal_kernel__queue_delayed_work_on__dwork__in(__pos, msg, ext, ctx, *dwork_ptr);
		}

	}

	{
		*delay_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	ret = queue_delayed_work_on(cpu, system_wq, dwork, delay);

	*__pos = 0;
	{
	}

	{
		if (*dwork_ptr) {
			callee_marshal_kernel__queue_delayed_work_on__dwork__in(__pos, msg, ext, ctx, *dwork_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_irq_write_handler_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device* dev = 0;
	struct mei_cl_cb* cmpl_list = 0;
	struct mei_device** dev_ptr = &dev;
	struct mei_cl_cb** cmpl_list_ptr = &cmpl_list;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mei_irq_write_handler_call_ctx call_ctx = {dev, cmpl_list};
	__maybe_unused struct mei_irq_write_handler_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_irq_write_handler__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*cmpl_list_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct mei_cl_cb*, (sizeof(struct mei_cl_cb)), (DEFAULT_GFP_FLAGS));
		if (*cmpl_list_ptr) {
			callee_unmarshal_kernel__mei_irq_write_handler__cmpl_list__out(__pos, msg, ext, ctx, *cmpl_list_ptr);
		}

	}

	ret = mei_irq_write_handler(dev, cmpl_list);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_irq_write_handler__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*cmpl_list_ptr) {
			callee_marshal_kernel__mei_irq_write_handler__cmpl_list__out(__pos, msg, ext, ctx, *cmpl_list_ptr);
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

void mei_irq_read_handler_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device* dev = 0;
	struct mei_cl_cb* cmpl_list = 0;
	int* slots = 0;
	struct mei_device** dev_ptr = &dev;
	struct mei_cl_cb** cmpl_list_ptr = &cmpl_list;
	int** slots_ptr = &slots;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mei_irq_read_handler_call_ctx call_ctx = {dev, cmpl_list, slots};
	__maybe_unused struct mei_irq_read_handler_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_irq_read_handler__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*cmpl_list_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct mei_cl_cb*, (sizeof(struct mei_cl_cb)), (DEFAULT_GFP_FLAGS));
		if (*cmpl_list_ptr) {
			callee_unmarshal_kernel__mei_irq_read_handler__cmpl_list__out(__pos, msg, ext, ctx, *cmpl_list_ptr);
		}

	}

	{
		*slots_ptr = glue_unpack_new_shadow(__pos, msg, ext, int*, (sizeof(int)), (DEFAULT_GFP_FLAGS));
		if (*slots_ptr) {
			**slots_ptr = glue_unpack(__pos, msg, ext, int);
		}

	}

	ret = mei_irq_read_handler(dev, cmpl_list, slots);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_irq_read_handler__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*cmpl_list_ptr) {
			callee_marshal_kernel__mei_irq_read_handler__cmpl_list__out(__pos, msg, ext, ctx, *cmpl_list_ptr);
		}

	}

	{
		if (*slots_ptr) {
			glue_pack(__pos, msg, ext, **slots_ptr);
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

void mei_irq_compl_handler_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device* dev = 0;
	struct mei_cl_cb* compl_list = 0;
	struct mei_device** dev_ptr = &dev;
	struct mei_cl_cb** compl_list_ptr = &compl_list;
	
	__maybe_unused struct mei_irq_compl_handler_call_ctx call_ctx = {dev, compl_list};
	__maybe_unused struct mei_irq_compl_handler_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_irq_compl_handler__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*compl_list_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct mei_cl_cb*, (sizeof(struct mei_cl_cb)), (DEFAULT_GFP_FLAGS));
		if (*compl_list_ptr) {
			callee_unmarshal_kernel__mei_irq_compl_handler__compl_list__out(__pos, msg, ext, ctx, *compl_list_ptr);
		}

	}

	mei_irq_compl_handler(dev, compl_list);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_irq_compl_handler__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*compl_list_ptr) {
			callee_marshal_kernel__mei_irq_compl_handler__compl_list__out(__pos, msg, ext, ctx, *compl_list_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_disable_msi_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused struct pci_disable_msi_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_disable_msi_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_disable_msi__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	pci_disable_msi(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_disable_msi__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void queue_work_on_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int cpu = 0;
	struct workqueue_struct* wq = 0;
	struct work_struct* work = 0;
	int* cpu_ptr = &cpu;
	struct workqueue_struct** wq_ptr = &wq;
	struct work_struct** work_ptr = &work;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct queue_work_on_call_ctx call_ctx = {cpu, wq, work};
	__maybe_unused struct queue_work_on_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*cpu_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*wq_ptr = glue_unpack(__pos, msg, ext, struct workqueue_struct*);
		if (*wq_ptr) {
			callee_unmarshal_kernel__queue_work_on__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
		*work_ptr = glue_unpack(__pos, msg, ext, struct work_struct*);
		if (*work_ptr) {
			callee_unmarshal_kernel__queue_work_on__work__in(__pos, msg, ext, ctx, *work_ptr);
		}

	}

	ret = queue_work_on(cpu, wq, work);

	*__pos = 0;
	{
	}

	{
		if (*wq_ptr) {
			callee_marshal_kernel__queue_work_on__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
		if (*work_ptr) {
			callee_marshal_kernel__queue_work_on__work__in(__pos, msg, ext, ctx, *work_ptr);
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

unsigned int trmp_impl_thread_fn(fptr_thread_fn target, int irq, void* id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* irq_ptr = &irq;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct thread_fn_call_ctx call_ctx = {irq, id};
	__maybe_unused const struct thread_fn_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		glue_pack(__pos, msg, ext, *irq_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_thread_fn);

	*__pos = 0;
	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_thread_fn)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_thread_fn) trmp_thread_fn(int irq, void* id)
{
	volatile fptr_impl_thread_fn impl;
	fptr_thread_fn target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_thread_fn);
	impl = trmp_impl_thread_fn;
	return impl(target, irq, id);
}

unsigned int trmp_impl_handler(fptr_handler target, int irq, void* id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* irq_ptr = &irq;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct handler_call_ctx call_ctx = {irq, id};
	__maybe_unused const struct handler_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		glue_pack(__pos, msg, ext, *irq_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_handler);

	*__pos = 0;
	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_handler)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_handler) trmp_handler(int irq, void* id)
{
	volatile fptr_impl_handler impl;
	fptr_handler target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_handler);
	impl = trmp_impl_handler;
	return impl(target, irq, id);
}

void request_threaded_irq_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int irq = 0;
	fptr_handler handler = 0;
	fptr_thread_fn thread_fn = 0;
	unsigned long irqflags = 0;
	char const* devname = 0;
	void* dev_id = 0;
	unsigned int* irq_ptr = &irq;
	fptr_handler* handler_ptr = &handler;
	fptr_thread_fn* thread_fn_ptr = &thread_fn;
	unsigned long* irqflags_ptr = &irqflags;
	char const** devname_ptr = &devname;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct request_threaded_irq_call_ctx call_ctx = {irq, handler, thread_fn, irqflags, devname, dev_id};
	__maybe_unused struct request_threaded_irq_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*handler_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, handler);
	}

	{
		*thread_fn_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, thread_fn);
	}

	{
		*irqflags_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		*devname_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, (sizeof(char) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
		if (*devname_ptr) {
			char* writable = (char*)*devname_ptr;
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

	dev_id = global_mei_dev;
	ret = request_threaded_irq(irq, handler, thread_fn, irqflags, devname, dev_id);

	*__pos = 0;
	{
	}

	{
	}

	{
	}

	{
	}

	{
		(void)devname_ptr;
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void free_irq_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int irq = 0;
	void* dev_id = 0;
	unsigned int* irq_ptr = &irq;
	
	__maybe_unused struct free_irq_call_ctx call_ctx = {irq, dev_id};
	__maybe_unused struct free_irq_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	dev_id = global_mei_dev;
	free_irq(irq, dev_id);

	*__pos = 0;
	{
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_disable_device_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused struct pci_disable_device_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_disable_device_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_disable_device__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	pci_disable_device(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_disable_device__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_enable_msi_range_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	int minvec = 0;
	int maxvec = 0;
	struct pci_dev** dev_ptr = &dev;
	int* minvec_ptr = &minvec;
	int* maxvec_ptr = &maxvec;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_enable_msi_range_call_ctx call_ctx = {dev, minvec, maxvec};
	__maybe_unused struct pci_enable_msi_range_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_enable_msi_range__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*minvec_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*maxvec_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = pci_enable_msi_range(dev, minvec, maxvec);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_enable_msi_range__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_iounmap_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	void* addr = 0;
	struct pci_dev** dev_ptr = &dev;
	void** addr_ptr = &addr;
	
	__maybe_unused struct pci_iounmap_call_ctx call_ctx = {dev, addr};
	__maybe_unused struct pci_iounmap_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_iounmap__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*addr_ptr = glue_unpack(__pos, msg, ext, void*);
		if (*addr_ptr) {
		}

	}

	pci_iounmap(dev, addr);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_iounmap__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		(void)addr_ptr;
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __wake_up_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct __wait_queue_head* wq = 0;
	unsigned int mode = 0;
	int nr_exclusive = 0;
	void* key = 0;
	struct __wait_queue_head** wq_ptr = &wq;
	unsigned int* mode_ptr = &mode;
	int* nr_exclusive_ptr = &nr_exclusive;
	
	__maybe_unused struct __wake_up_call_ctx call_ctx = {wq, mode, nr_exclusive, key};
	__maybe_unused struct __wake_up_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct mei_device* __mei_device = glue_unpack_shadow(__pos, msg, ext, struct mei_device*);
		*wq_ptr = &__mei_device->wait_hw_ready;
		if (*wq_ptr) {
			callee_unmarshal_kernel____wake_up__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
		*mode_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*nr_exclusive_ptr = glue_unpack(__pos, msg, ext, int);
	}

	__wake_up(wq, mode, nr_exclusive, key);

	*__pos = 0;
	{
		if (*wq_ptr) {
			callee_marshal_kernel____wake_up__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
	}

	{
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

	case RPC_ID_pci_set_master:
		glue_user_trace("pci_set_master\n");
		pci_set_master_callee(msg, ext);
		break;

	case RPC_ID_pci_request_regions:
		glue_user_trace("pci_request_regions\n");
		pci_request_regions_callee(msg, ext);
		break;

	case RPC_ID_pci_release_regions:
		glue_user_trace("pci_release_regions\n");
		pci_release_regions_callee(msg, ext);
		break;

	case RPC_ID_pci_dev_run_wake:
		glue_user_trace("pci_dev_run_wake\n");
		pci_dev_run_wake_callee(msg, ext);
		break;

	case RPC_ID_pci_enable_device:
		glue_user_trace("pci_enable_device\n");
		pci_enable_device_callee(msg, ext);
		break;

	case RPC_ID_pci_unregister_driver:
		glue_user_trace("pci_unregister_driver\n");
		pci_unregister_driver_callee(msg, ext);
		break;

	case RPC_ID___pci_register_driver:
		glue_user_trace("__pci_register_driver\n");
		__pci_register_driver_callee(msg, ext);
		break;

	case RPC_ID_mei_hbm_pg_resume:
		glue_user_trace("mei_hbm_pg_resume\n");
		mei_hbm_pg_resume_callee(msg, ext);
		break;

	case RPC_ID_mei_hbm_pg:
		glue_user_trace("mei_hbm_pg\n");
		mei_hbm_pg_callee(msg, ext);
		break;

	case RPC_ID_mei_device_init:
		glue_user_trace("mei_device_init\n");
		mei_device_init_callee(msg, ext);
		break;

	case RPC_ID_mei_stop:
		glue_user_trace("mei_stop\n");
		mei_stop_callee(msg, ext);
		break;

	case RPC_ID_mei_cancel_work:
		glue_user_trace("mei_cancel_work\n");
		mei_cancel_work_callee(msg, ext);
		break;

	case RPC_ID_mei_start:
		glue_user_trace("mei_start\n");
		mei_start_callee(msg, ext);
		break;

	case RPC_ID_pci_iomap:
		glue_user_trace("pci_iomap\n");
		pci_iomap_callee(msg, ext);
		break;

	case RPC_ID_pci_bus_read_config_dword:
		glue_user_trace("pci_bus_read_config_dword\n");
		pci_bus_read_config_dword_callee(msg, ext);
		break;

	case RPC_ID_schedule_timeout:
		glue_user_trace("schedule_timeout\n");
		schedule_timeout_callee(msg, ext);
		break;

	case RPC_ID_finish_wait:
		glue_user_trace("finish_wait\n");
		finish_wait_callee(msg, ext);
		break;

	case RPC_ID_mei_deregister:
		glue_user_trace("mei_deregister\n");
		mei_deregister_callee(msg, ext);
		break;

	case RPC_ID_mei_register:
		glue_user_trace("mei_register\n");
		mei_register_callee(msg, ext);
		break;

	case RPC_ID_prepare_to_wait_event:
		glue_user_trace("prepare_to_wait_event\n");
		prepare_to_wait_event_callee(msg, ext);
		break;

	case RPC_ID___msecs_to_jiffies:
		glue_user_trace("__msecs_to_jiffies\n");
		__msecs_to_jiffies_callee(msg, ext);
		break;

	case RPC_ID_queue_delayed_work_on:
		glue_user_trace("queue_delayed_work_on\n");
		queue_delayed_work_on_callee(msg, ext);
		break;

	case RPC_ID_mei_irq_write_handler:
		glue_user_trace("mei_irq_write_handler\n");
		mei_irq_write_handler_callee(msg, ext);
		break;

	case RPC_ID_mei_irq_read_handler:
		glue_user_trace("mei_irq_read_handler\n");
		mei_irq_read_handler_callee(msg, ext);
		break;

	case RPC_ID_mei_irq_compl_handler:
		glue_user_trace("mei_irq_compl_handler\n");
		mei_irq_compl_handler_callee(msg, ext);
		break;

	case RPC_ID_pci_disable_msi:
		glue_user_trace("pci_disable_msi\n");
		pci_disable_msi_callee(msg, ext);
		break;

	case RPC_ID_queue_work_on:
		glue_user_trace("queue_work_on\n");
		queue_work_on_callee(msg, ext);
		break;

	case RPC_ID_request_threaded_irq:
		glue_user_trace("request_threaded_irq\n");
		request_threaded_irq_callee(msg, ext);
		break;

	case RPC_ID_free_irq:
		glue_user_trace("free_irq\n");
		free_irq_callee(msg, ext);
		break;

	case RPC_ID_pci_disable_device:
		glue_user_trace("pci_disable_device\n");
		pci_disable_device_callee(msg, ext);
		break;

	case RPC_ID_pci_enable_msi_range:
		glue_user_trace("pci_enable_msi_range\n");
		pci_enable_msi_range_callee(msg, ext);
		break;

	case RPC_ID_pci_iounmap:
		glue_user_trace("pci_iounmap\n");
		pci_iounmap_callee(msg, ext);
		break;

	case RPC_ID___wake_up:
		glue_user_trace("__wake_up\n");
		__wake_up_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

