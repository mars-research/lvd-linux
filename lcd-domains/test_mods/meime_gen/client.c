#include <lcd_config/pre_hook.h>

#include "common.h"

#include <asm/lcd_domains/liblcd.h>
#include <lcd_config/post_hook.h>

struct mei_device* global_mei_dev;

void pci_driver_probe_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_pci_driver_probe function_ptr = glue_unpack(__pos, msg, ext, fptr_pci_driver_probe);
	struct pci_dev* pdev = 0;
	struct pci_device_id* ent = 0;
	struct pci_dev** pdev_ptr = &pdev;
	struct pci_device_id** ent_ptr = &ent;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_driver_probe_call_ctx call_ctx = {pdev, ent};
	__maybe_unused struct pci_driver_probe_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct pci_dev*, (sizeof(struct pci_dev)), (DEFAULT_GFP_FLAGS));
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_driver_probe__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		*ent_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct pci_device_id*, (sizeof(struct pci_device_id)), (DEFAULT_GFP_FLAGS));
		if (*ent_ptr) {
			callee_unmarshal_kernel__pci_driver_probe__ent__in(__pos, msg, ext, ctx, *ent_ptr);
		}

	}

	ret = function_ptr(pdev, ent);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_driver_probe__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		if (*ent_ptr) {
			callee_marshal_kernel__pci_driver_probe__ent__in(__pos, msg, ext, ctx, *ent_ptr);
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

void pci_driver_remove_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_pci_driver_remove function_ptr = glue_unpack(__pos, msg, ext, fptr_pci_driver_remove);
	struct pci_dev* pdev = 0;
	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused struct pci_driver_remove_call_ctx call_ctx = {pdev};
	__maybe_unused struct pci_driver_remove_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_driver_remove__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	function_ptr(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_driver_remove__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_driver_shutdown_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_pci_driver_shutdown function_ptr = glue_unpack(__pos, msg, ext, fptr_pci_driver_shutdown);
	struct pci_dev* pdev = 0;
	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused struct pci_driver_shutdown_call_ctx call_ctx = {pdev};
	__maybe_unused struct pci_driver_shutdown_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_driver_shutdown__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	function_ptr(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_driver_shutdown__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_set_master(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_set_master_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_set_master_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_set_master__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_pci_set_master);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_set_master__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int pci_request_regions(struct pci_dev* pdev, char const* res_name)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	char const** res_name_ptr = &res_name;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_request_regions_call_ctx call_ctx = {pdev, res_name};
	__maybe_unused const struct pci_request_regions_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_request_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		const void* __adjusted = *res_name_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*res_name_ptr) {
			size_t i, len;
			char const* array = *res_name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	glue_call_server(__pos, msg, RPC_ID_pci_request_regions);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_request_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		(void)res_name_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void pci_release_regions(struct pci_dev* pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused const struct pci_release_regions_call_ctx call_ctx = {pdev};
	__maybe_unused const struct pci_release_regions_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_release_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_pci_release_regions);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_release_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

bool pci_dev_run_wake(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct pci_dev_run_wake_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_dev_run_wake_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_dev_run_wake__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_pci_dev_run_wake);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_dev_run_wake__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

int pci_enable_device(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_device_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_enable_device_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_device__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_pci_enable_device);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_device__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void pci_unregister_driver(struct pci_driver* drv)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_driver** drv_ptr = &drv;
	
	__maybe_unused const struct pci_unregister_driver_call_ctx call_ctx = {drv};
	__maybe_unused const struct pci_unregister_driver_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *drv_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*drv_ptr) {
			caller_marshal_kernel__pci_unregister_driver__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_pci_unregister_driver);

	*__pos = 0;
	{
		if (*drv_ptr) {
			caller_unmarshal_kernel__pci_unregister_driver__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int __pci_register_driver(struct pci_driver* drv, struct module* owner, char const* mod_name)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_driver** drv_ptr = &drv;
	struct module** owner_ptr = &owner;
	char const** mod_name_ptr = &mod_name;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct __pci_register_driver_call_ctx call_ctx = {drv, owner, mod_name};
	__maybe_unused const struct __pci_register_driver_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *drv_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*drv_ptr) {
			caller_marshal_kernel____pci_register_driver__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	{
		const void* __adjusted = *owner_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*owner_ptr) {
			caller_marshal_kernel____pci_register_driver__owner__in(__pos, msg, ext, ctx, *owner_ptr);
		}

	}

	{
		const void* __adjusted = *mod_name_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*mod_name_ptr) {
			size_t i, len;
			char const* array = *mod_name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	glue_call_server(__pos, msg, RPC_ID___pci_register_driver);

	*__pos = 0;
	{
		if (*drv_ptr) {
			caller_unmarshal_kernel____pci_register_driver__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	{
		if (*owner_ptr) {
			caller_unmarshal_kernel____pci_register_driver__owner__in(__pos, msg, ext, ctx, *owner_ptr);
		}

	}

	{
		(void)mod_name_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void mei_hbm_pg_resume(struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused const struct mei_hbm_pg_resume_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_hbm_pg_resume_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hbm_pg_resume__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_mei_hbm_pg_resume);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hbm_pg_resume__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int mei_hbm_pg(struct mei_device* dev, unsigned char pg_cmd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	unsigned char* pg_cmd_ptr = &pg_cmd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mei_hbm_pg_call_ctx call_ctx = {dev, pg_cmd};
	__maybe_unused const struct mei_hbm_pg_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hbm_pg__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *pg_cmd_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_mei_hbm_pg);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hbm_pg__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void mei_device_init(struct mei_device* dev, struct device* device, struct mei_hw_ops const* ops)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	struct device** device_ptr = &device;
	struct mei_hw_ops const** ops_ptr = &ops;
	
	__maybe_unused const struct mei_device_init_call_ctx call_ctx = {dev, device, ops};
	__maybe_unused const struct mei_device_init_call_ctx *ctx = &call_ctx;

	(void)ext;

	global_mei_dev = *dev_ptr;
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_device_init__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		struct pci_dev* __adjusted = container_of(*device_ptr, struct pci_dev, dev);
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*device_ptr) {
			caller_marshal_kernel__mei_device_init__device__in(__pos, msg, ext, ctx, *device_ptr);
		}

	}

	{
		const void* __adjusted = *ops_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ops_ptr) {
			caller_marshal_kernel___global_mei_hw_ops__in(__pos, msg, ext, *ops_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_mei_device_init);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_device_init__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*device_ptr) {
			caller_unmarshal_kernel__mei_device_init__device__in(__pos, msg, ext, ctx, *device_ptr);
		}

	}

	{
		if (*ops_ptr) {
			struct mei_hw_ops* writable = (struct mei_hw_ops*)*ops_ptr;
			caller_unmarshal_kernel___global_mei_hw_ops__in(__pos, msg, ext, writable);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_stop(struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused const struct mei_stop_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_stop_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_stop__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_mei_stop);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_stop__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_cancel_work(struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused const struct mei_cancel_work_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_cancel_work_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_cancel_work__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_mei_cancel_work);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_cancel_work__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int mei_start(struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mei_start_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_start_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_start__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	printk("%s, mei_device %p | hw %p | hw->mem_addr %p\n", __func__,
			dev, to_me_hw(dev), (to_me_hw(dev))->mem_addr);
	glue_call_server(__pos, msg, RPC_ID_mei_start);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_start__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void* pci_iomap(struct pci_dev* pdev, int bar, unsigned long maxlen)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	int* bar_ptr = &bar;
	unsigned long* maxlen_ptr = &maxlen;
	cptr_t ioremap_cptr;
	gpa_t ioremap_gpa;
	uint64_t ioremap_len;
	void* ret = 0;
	void** ret_ptr = &ret;
	
	__maybe_unused const struct pci_iomap_call_ctx call_ctx = {pdev, bar, maxlen};
	__maybe_unused const struct pci_iomap_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		lcd_cptr_alloc(&ioremap_cptr);
		glue_pack(__pos, msg, ext, cptr_val(ioremap_cptr));
	}

	{
		const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_iomap__dev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *bar_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *maxlen_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_pci_iomap);

	*__pos = 0;
	{
		int _ret;
		ioremap_len = glue_unpack(__pos, msg, ext, uint64_t);
		printk("%s, got len %llx\n", __func__, ioremap_len);
		if (ioremap_len < PAGE_SIZE)
			ioremap_len = PAGE_SIZE;
		_ret = lcd_ioremap_phys(ioremap_cptr, ioremap_len, &ioremap_gpa);

		if (_ret) {
			LIBLCD_ERR("failed to ioremap phys");
		}	
		printk("%s, calling lcd_ioremap %llx\n", __func__, ioremap_len);
		*ret_ptr = lcd_ioremap(gpa_val(ioremap_gpa), ioremap_len);
		printk("%s, lcd_ioremap returned %p\n", __func__, *ret_ptr);
		if (!*ret_ptr) {
			LIBLCD_ERR("failed to ioremap virt");
		}

	}

	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_iomap__dev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
	}

	{
		int _ret;
		_ret = lcd_syscall_assign_device(0, 0, PCI_DEVFN(0x16, 0));
		printk("%s, assign device returned %d\n", __func__, _ret);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return *ret_ptr;
}

int pci_bus_read_config_dword(struct pci_bus* bus, unsigned int devfn, int pos, unsigned int* value)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_bus** bus_ptr = &bus;
	unsigned int* devfn_ptr = &devfn;
	int* pos_ptr = &pos;
	unsigned int** value_ptr = &value;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_bus_read_config_dword_call_ctx call_ctx = {bus, devfn, pos, value};
	__maybe_unused const struct pci_bus_read_config_dword_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *bus_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__pci_bus_read_config_dword__bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *devfn_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *pos_ptr);
	}

	{
		const void* __adjusted = *value_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		(void)value_ptr;
	}

	glue_call_server(__pos, msg, RPC_ID_pci_bus_read_config_dword);

	*__pos = 0;
	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__pci_bus_read_config_dword__bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
	}

	{
	}

	{
		if (*value_ptr) {
			**value_ptr = glue_unpack(__pos, msg, ext, unsigned int);
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

void mei_hw_ops_host_is_ready_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_host_is_ready function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_host_is_ready);
	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct mei_hw_ops_host_is_ready_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hw_ops_host_is_ready_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_host_is_ready__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_host_is_ready__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void mei_hw_ops_hw_is_ready_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_hw_is_ready function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_hw_is_ready);
	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct mei_hw_ops_hw_is_ready_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hw_ops_hw_is_ready_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_hw_is_ready__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_hw_is_ready__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void mei_hw_ops_hw_reset_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_hw_reset function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_hw_reset);
	struct mei_device* dev = 0;
	bool intr_enable = 0;
	struct mei_device** dev_ptr = &dev;
	bool* intr_enable_ptr = &intr_enable;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mei_hw_ops_hw_reset_call_ctx call_ctx = {dev, intr_enable};
	__maybe_unused struct mei_hw_ops_hw_reset_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_hw_reset__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*intr_enable_ptr = glue_unpack(__pos, msg, ext, bool);
	}

	ret = function_ptr(dev, intr_enable);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_hw_reset__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void mei_hw_ops_hw_start_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_hw_start function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_hw_start);
	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mei_hw_ops_hw_start_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hw_ops_hw_start_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_hw_start__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_hw_start__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void mei_hw_ops_hw_config_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_hw_config function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_hw_config);
	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused struct mei_hw_ops_hw_config_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hw_ops_hw_config_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_hw_config__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_hw_config__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_hw_ops_fw_status_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_fw_status function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_fw_status);
	struct mei_device* dev = 0;
	struct mei_fw_status* fw_status = 0;
	struct mei_device** dev_ptr = &dev;
	struct mei_fw_status** fw_status_ptr = &fw_status;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mei_hw_ops_fw_status_call_ctx call_ctx = {dev, fw_status};
	__maybe_unused struct mei_hw_ops_fw_status_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_fw_status__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*fw_status_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct mei_fw_status*, (sizeof(struct mei_fw_status)), (DEFAULT_GFP_FLAGS));
		if (*fw_status_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_fw_status__fw_status__in(__pos, msg, ext, ctx, *fw_status_ptr);
		}

	}

	ret = function_ptr(dev, fw_status);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_fw_status__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*fw_status_ptr) {
			callee_marshal_kernel__mei_hw_ops_fw_status__fw_status__in(__pos, msg, ext, ctx, *fw_status_ptr);
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

void mei_hw_ops_pg_state_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_pg_state function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_pg_state);
	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct mei_hw_ops_pg_state_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hw_ops_pg_state_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_pg_state__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_pg_state__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void mei_hw_ops_pg_in_transition_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_pg_in_transition function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_pg_in_transition);
	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct mei_hw_ops_pg_in_transition_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hw_ops_pg_in_transition_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_pg_in_transition__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_pg_in_transition__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void mei_hw_ops_pg_is_enabled_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_pg_is_enabled function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_pg_is_enabled);
	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct mei_hw_ops_pg_is_enabled_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hw_ops_pg_is_enabled_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_pg_is_enabled__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_pg_is_enabled__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void mei_hw_ops_intr_clear_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_intr_clear function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_intr_clear);
	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused struct mei_hw_ops_intr_clear_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hw_ops_intr_clear_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_intr_clear__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_intr_clear__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_hw_ops_intr_enable_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_intr_enable function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_intr_enable);
	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused struct mei_hw_ops_intr_enable_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hw_ops_intr_enable_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_intr_enable__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_intr_enable__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_hw_ops_intr_disable_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_intr_disable function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_intr_disable);
	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused struct mei_hw_ops_intr_disable_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hw_ops_intr_disable_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_intr_disable__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_intr_disable__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_hw_ops_hbuf_free_slots_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_hbuf_free_slots function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_hbuf_free_slots);
	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mei_hw_ops_hbuf_free_slots_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hw_ops_hbuf_free_slots_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_hbuf_free_slots__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_hbuf_free_slots__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void mei_hw_ops_hbuf_is_ready_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_hbuf_is_ready function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_hbuf_is_ready);
	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct mei_hw_ops_hbuf_is_ready_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hw_ops_hbuf_is_ready_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_hbuf_is_ready__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_hbuf_is_ready__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void mei_hw_ops_hbuf_max_len_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_hbuf_max_len function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_hbuf_max_len);
	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused struct mei_hw_ops_hbuf_max_len_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hw_ops_hbuf_max_len_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_hbuf_max_len__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_hbuf_max_len__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void mei_hw_ops_write_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_write function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_write);
	struct mei_device* dev = 0;
	struct mei_msg_hdr* header = 0;
	unsigned char* buf = 0;
	struct mei_device** dev_ptr = &dev;
	struct mei_msg_hdr** header_ptr = &header;
	unsigned char** buf_ptr = &buf;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mei_hw_ops_write_call_ctx call_ctx = {dev, header, buf};
	__maybe_unused struct mei_hw_ops_write_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_write__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*header_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct mei_msg_hdr*, (sizeof(struct mei_msg_hdr)), (DEFAULT_GFP_FLAGS));
		if (*header_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_write__header__in(__pos, msg, ext, ctx, *header_ptr);
		}

	}

	{
		*buf_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned char*, (header->length), (DEFAULT_GFP_FLAGS));
		if (*buf_ptr) {
			**buf_ptr = glue_unpack(__pos, msg, ext, unsigned char);
		}

	}

	ret = function_ptr(dev, header, buf);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_write__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*header_ptr) {
			callee_marshal_kernel__mei_hw_ops_write__header__in(__pos, msg, ext, ctx, *header_ptr);
		}

	}

	{
		(void)buf_ptr;
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_hw_ops_rdbuf_full_slots_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_rdbuf_full_slots function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_rdbuf_full_slots);
	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mei_hw_ops_rdbuf_full_slots_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hw_ops_rdbuf_full_slots_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_rdbuf_full_slots__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_rdbuf_full_slots__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void mei_hw_ops_read_hdr_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_read_hdr function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_read_hdr);
	struct mei_device* dev = 0;
	struct mei_device** dev_ptr = &dev;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct mei_hw_ops_read_hdr_call_ctx call_ctx = {dev};
	__maybe_unused struct mei_hw_ops_read_hdr_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_read_hdr__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_read_hdr__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void mei_hw_ops_read_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mei_hw_ops_read function_ptr = glue_unpack(__pos, msg, ext, fptr_mei_hw_ops_read);
	struct mei_device* dev = 0;
	unsigned char* buffer = 0;
	unsigned long buffer_length = 0;
	struct mei_device** dev_ptr = &dev;
	unsigned char** buffer_ptr = &buffer;
	unsigned long* buffer_length_ptr = &buffer_length;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mei_hw_ops_read_call_ctx call_ctx = {dev, buffer, buffer_length};
	__maybe_unused struct mei_hw_ops_read_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hw_ops_read__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*buffer_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned char*, (buffer_length), (DEFAULT_GFP_FLAGS));
		if (*buffer_ptr) {
			**buffer_ptr = glue_unpack(__pos, msg, ext, unsigned char);
		}

	}

	{
		*buffer_length_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	ret = function_ptr(dev, buffer, buffer_length);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hw_ops_read__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		(void)buffer_ptr;
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

long schedule_timeout(long timeout)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	long* timeout_ptr = &timeout;
	long ret = 0;
	long* ret_ptr = &ret;
	
	__maybe_unused const struct schedule_timeout_call_ctx call_ctx = {timeout};
	__maybe_unused const struct schedule_timeout_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *timeout_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_schedule_timeout);

	*__pos = 0;
	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void finish_wait(struct __wait_queue_head* wq, struct __wait_queue* wait)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct __wait_queue_head** wq_ptr = &wq;
	struct __wait_queue** wait_ptr = &wait;
	
	__maybe_unused const struct finish_wait_call_ctx call_ctx = {wq, wait};
	__maybe_unused const struct finish_wait_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct mei_device* __adjusted = container_of(*wq_ptr, struct mei_device, wait_hw_ready);
		glue_pack(__pos, msg, ext, __adjusted);
		if (*wq_ptr) {
			caller_marshal_kernel__finish_wait__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
		const void* __adjusted = *wait_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*wait_ptr) {
			caller_marshal_kernel__finish_wait____wait_queue__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_finish_wait);

	*__pos = 0;
	{
		if (*wq_ptr) {
			caller_unmarshal_kernel__finish_wait__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
		if (*wait_ptr) {
			caller_unmarshal_kernel__finish_wait____wait_queue__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mei_deregister(struct mei_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	
	__maybe_unused const struct mei_deregister_call_ctx call_ctx = {dev};
	__maybe_unused const struct mei_deregister_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_deregister__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_mei_deregister);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_deregister__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int mei_register(struct mei_device* dev, struct device* parent)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	struct device** parent_ptr = &parent;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mei_register_call_ctx call_ctx = {dev, parent};
	__maybe_unused const struct mei_register_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_register__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		struct pci_dev* __adjusted = container_of(*parent_ptr, struct pci_dev, dev);
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*parent_ptr) {
			caller_marshal_kernel__mei_register__parent__in(__pos, msg, ext, ctx, *parent_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_mei_register);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_register__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*parent_ptr) {
			caller_unmarshal_kernel__mei_register__parent__in(__pos, msg, ext, ctx, *parent_ptr);
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

long prepare_to_wait_event(struct __wait_queue_head* wq, struct __wait_queue* wait, int state)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct __wait_queue_head** wq_ptr = &wq;
	struct __wait_queue** wait_ptr = &wait;
	int* state_ptr = &state;
	long ret = 0;
	long* ret_ptr = &ret;
	
	__maybe_unused const struct prepare_to_wait_event_call_ctx call_ctx = {wq, wait, state};
	__maybe_unused const struct prepare_to_wait_event_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct mei_device* __adjusted = container_of(*wq_ptr, struct mei_device, wait_hw_ready);
		glue_pack(__pos, msg, ext, __adjusted);
		if (*wq_ptr) {
			caller_marshal_kernel__prepare_to_wait_event__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
		const void* __adjusted = *wait_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*wait_ptr) {
			caller_marshal_kernel__prepare_to_wait_event____wait_queue__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_prepare_to_wait_event);

	*__pos = 0;
	{
		if (*wq_ptr) {
			caller_unmarshal_kernel__prepare_to_wait_event__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
		if (*wait_ptr) {
			caller_unmarshal_kernel__prepare_to_wait_event____wait_queue__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

unsigned long __msecs_to_jiffies(unsigned int m)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* m_ptr = &m;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused const struct __msecs_to_jiffies_call_ctx call_ctx = {m};
	__maybe_unused const struct __msecs_to_jiffies_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *m_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID___msecs_to_jiffies);

	*__pos = 0;
	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

bool queue_delayed_work_on(int cpu, struct workqueue_struct* wq, struct delayed_work* dwork, unsigned long delay)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* cpu_ptr = &cpu;
	struct delayed_work** dwork_ptr = &dwork;
	unsigned long* delay_ptr = &delay;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct queue_delayed_work_on_call_ctx call_ctx = {cpu, wq, dwork, delay};
	__maybe_unused const struct queue_delayed_work_on_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *cpu_ptr);
	}

	{
		struct mei_device* __adjusted = container_of(*dwork_ptr, struct mei_device, timer_work);
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dwork_ptr) {
			caller_marshal_kernel__queue_delayed_work_on__dwork__in(__pos, msg, ext, ctx, *dwork_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *delay_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_queue_delayed_work_on);

	*__pos = 0;
	{
	}

	{
		if (*dwork_ptr) {
			caller_unmarshal_kernel__queue_delayed_work_on__dwork__in(__pos, msg, ext, ctx, *dwork_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int mei_irq_write_handler(struct mei_device* dev, struct mei_cl_cb* cmpl_list)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	struct mei_cl_cb** cmpl_list_ptr = &cmpl_list;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mei_irq_write_handler_call_ctx call_ctx = {dev, cmpl_list};
	__maybe_unused const struct mei_irq_write_handler_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_irq_write_handler__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		const void* __adjusted = *cmpl_list_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*cmpl_list_ptr) {
			caller_marshal_kernel__mei_irq_write_handler__cmpl_list__out(__pos, msg, ext, ctx, *cmpl_list_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_mei_irq_write_handler);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_irq_write_handler__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*cmpl_list_ptr) {
			caller_unmarshal_kernel__mei_irq_write_handler__cmpl_list__out(__pos, msg, ext, ctx, *cmpl_list_ptr);
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

int mei_irq_read_handler(struct mei_device* dev, struct mei_cl_cb* cmpl_list, int* slots)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	struct mei_cl_cb** cmpl_list_ptr = &cmpl_list;
	int** slots_ptr = &slots;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mei_irq_read_handler_call_ctx call_ctx = {dev, cmpl_list, slots};
	__maybe_unused const struct mei_irq_read_handler_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_irq_read_handler__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		const void* __adjusted = *cmpl_list_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*cmpl_list_ptr) {
			caller_marshal_kernel__mei_irq_read_handler__cmpl_list__out(__pos, msg, ext, ctx, *cmpl_list_ptr);
		}

	}

	{
		const void* __adjusted = *slots_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*slots_ptr) {
			glue_pack(__pos, msg, ext, **slots_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_mei_irq_read_handler);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_irq_read_handler__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*cmpl_list_ptr) {
			caller_unmarshal_kernel__mei_irq_read_handler__cmpl_list__out(__pos, msg, ext, ctx, *cmpl_list_ptr);
		}

	}

	{
		if (*slots_ptr) {
			**slots_ptr = glue_unpack(__pos, msg, ext, int);
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

void mei_irq_compl_handler(struct mei_device* dev, struct mei_cl_cb* compl_list)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mei_device** dev_ptr = &dev;
	struct mei_cl_cb** compl_list_ptr = &compl_list;
	
	__maybe_unused const struct mei_irq_compl_handler_call_ctx call_ctx = {dev, compl_list};
	__maybe_unused const struct mei_irq_compl_handler_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_irq_compl_handler__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		const void* __adjusted = *compl_list_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*compl_list_ptr) {
			caller_marshal_kernel__mei_irq_compl_handler__compl_list__out(__pos, msg, ext, ctx, *compl_list_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_mei_irq_compl_handler);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_irq_compl_handler__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*compl_list_ptr) {
			caller_unmarshal_kernel__mei_irq_compl_handler__compl_list__out(__pos, msg, ext, ctx, *compl_list_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_disable_msi(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_disable_msi_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_disable_msi_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_disable_msi__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_pci_disable_msi);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_disable_msi__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

bool queue_work_on(int cpu, struct workqueue_struct* wq, struct work_struct* work)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* cpu_ptr = &cpu;
	struct workqueue_struct** wq_ptr = &wq;
	struct work_struct** work_ptr = &work;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct queue_work_on_call_ctx call_ctx = {cpu, wq, work};
	__maybe_unused const struct queue_work_on_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *cpu_ptr);
	}

	{
		const void* __adjusted = *wq_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*wq_ptr) {
			caller_marshal_kernel__queue_work_on__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
		const void* __adjusted = *work_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*work_ptr) {
			caller_marshal_kernel__queue_work_on__work__in(__pos, msg, ext, ctx, *work_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_queue_work_on);

	*__pos = 0;
	{
	}

	{
		if (*wq_ptr) {
			caller_unmarshal_kernel__queue_work_on__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
		if (*work_ptr) {
			caller_unmarshal_kernel__queue_work_on__work__in(__pos, msg, ext, ctx, *work_ptr);
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

void thread_fn_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_thread_fn function_ptr = glue_unpack(__pos, msg, ext, fptr_thread_fn);
	int irq = 0;
	void* id = 0;
	int* irq_ptr = &irq;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct thread_fn_call_ctx call_ctx = {irq, id};
	__maybe_unused struct thread_fn_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, msg, ext, int);
	}

	id = global_mei_dev;
	ret = function_ptr(irq, id);

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

void handler_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_handler function_ptr = glue_unpack(__pos, msg, ext, fptr_handler);
	int irq = 0;
	void* id = 0;
	int* irq_ptr = &irq;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct handler_call_ctx call_ctx = {irq, id};
	__maybe_unused struct handler_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, msg, ext, int);
	}

	id = global_mei_dev;
	ret = function_ptr(irq, id);

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

int request_threaded_irq(unsigned int irq, fptr_handler handler, fptr_thread_fn thread_fn, unsigned long irqflags, char const* devname, void* dev_id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* irq_ptr = &irq;
	fptr_handler* handler_ptr = &handler;
	fptr_thread_fn* thread_fn_ptr = &thread_fn;
	unsigned long* irqflags_ptr = &irqflags;
	char const** devname_ptr = &devname;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct request_threaded_irq_call_ctx call_ctx = {irq, handler, thread_fn, irqflags, devname, dev_id};
	__maybe_unused const struct request_threaded_irq_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *irq_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *handler_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *thread_fn_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *irqflags_ptr);
	}

	{
		const void* __adjusted = *devname_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*devname_ptr) {
			size_t i, len;
			char const* array = *devname_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	glue_call_server(__pos, msg, RPC_ID_request_threaded_irq);

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
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void free_irq(unsigned int irq, void* dev_id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* irq_ptr = &irq;
	
	__maybe_unused const struct free_irq_call_ctx call_ctx = {irq, dev_id};
	__maybe_unused const struct free_irq_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *irq_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_free_irq);

	*__pos = 0;
	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_disable_device(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_disable_device_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_disable_device_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_disable_device__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_pci_disable_device);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_disable_device__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int pci_enable_msi_range(struct pci_dev* dev, int minvec, int maxvec)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int* minvec_ptr = &minvec;
	int* maxvec_ptr = &maxvec;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_msi_range_call_ctx call_ctx = {dev, minvec, maxvec};
	__maybe_unused const struct pci_enable_msi_range_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_msi_range__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *minvec_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *maxvec_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_pci_enable_msi_range);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_msi_range__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
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

void pci_iounmap(struct pci_dev* dev, void* addr)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	void** addr_ptr = &addr;
	
	__maybe_unused const struct pci_iounmap_call_ctx call_ctx = {dev, addr};
	__maybe_unused const struct pci_iounmap_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_iounmap__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (0)
	{
		const void* __adjusted = *addr_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*addr_ptr) {
		}

	}

	glue_call_server(__pos, msg, RPC_ID_pci_iounmap);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_iounmap__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		(void)addr_ptr;
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __wake_up(struct __wait_queue_head* wq, unsigned int mode, int nr_exclusive, void* key)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct __wait_queue_head** wq_ptr = &wq;
	unsigned int* mode_ptr = &mode;
	int* nr_exclusive_ptr = &nr_exclusive;
	
	__maybe_unused const struct __wake_up_call_ctx call_ctx = {wq, mode, nr_exclusive, key};
	__maybe_unused const struct __wake_up_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct mei_device* __adjusted = container_of(*wq_ptr, struct mei_device, wait_hw_ready);
		glue_pack(__pos, msg, ext, __adjusted);
		if (*wq_ptr) {
			caller_marshal_kernel____wake_up__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *mode_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *nr_exclusive_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID___wake_up);

	*__pos = 0;
	{
		if (*wq_ptr) {
			caller_unmarshal_kernel____wake_up__wq__in(__pos, msg, ext, ctx, *wq_ptr);
		}

	}

	{
	}

	{
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

	case RPC_ID_pci_driver_probe:
		glue_user_trace("pci_driver_probe\n");
		pci_driver_probe_callee(msg, ext);
		break;

	case RPC_ID_pci_driver_remove:
		glue_user_trace("pci_driver_remove\n");
		pci_driver_remove_callee(msg, ext);
		break;

	case RPC_ID_pci_driver_shutdown:
		glue_user_trace("pci_driver_shutdown\n");
		pci_driver_shutdown_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_host_is_ready:
		glue_user_trace("mei_hw_ops_host_is_ready\n");
		mei_hw_ops_host_is_ready_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_hw_is_ready:
		glue_user_trace("mei_hw_ops_hw_is_ready\n");
		mei_hw_ops_hw_is_ready_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_hw_reset:
		glue_user_trace("mei_hw_ops_hw_reset\n");
		mei_hw_ops_hw_reset_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_hw_start:
		glue_user_trace("mei_hw_ops_hw_start\n");
		mei_hw_ops_hw_start_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_hw_config:
		glue_user_trace("mei_hw_ops_hw_config\n");
		mei_hw_ops_hw_config_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_fw_status:
		glue_user_trace("mei_hw_ops_fw_status\n");
		mei_hw_ops_fw_status_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_pg_state:
		glue_user_trace("mei_hw_ops_pg_state\n");
		mei_hw_ops_pg_state_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_pg_in_transition:
		glue_user_trace("mei_hw_ops_pg_in_transition\n");
		mei_hw_ops_pg_in_transition_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_pg_is_enabled:
		glue_user_trace("mei_hw_ops_pg_is_enabled\n");
		mei_hw_ops_pg_is_enabled_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_intr_clear:
		glue_user_trace("mei_hw_ops_intr_clear\n");
		mei_hw_ops_intr_clear_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_intr_enable:
		glue_user_trace("mei_hw_ops_intr_enable\n");
		mei_hw_ops_intr_enable_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_intr_disable:
		glue_user_trace("mei_hw_ops_intr_disable\n");
		mei_hw_ops_intr_disable_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_hbuf_free_slots:
		glue_user_trace("mei_hw_ops_hbuf_free_slots\n");
		mei_hw_ops_hbuf_free_slots_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_hbuf_is_ready:
		glue_user_trace("mei_hw_ops_hbuf_is_ready\n");
		mei_hw_ops_hbuf_is_ready_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_hbuf_max_len:
		glue_user_trace("mei_hw_ops_hbuf_max_len\n");
		mei_hw_ops_hbuf_max_len_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_write:
		glue_user_trace("mei_hw_ops_write\n");
		mei_hw_ops_write_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_rdbuf_full_slots:
		glue_user_trace("mei_hw_ops_rdbuf_full_slots\n");
		mei_hw_ops_rdbuf_full_slots_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_read_hdr:
		glue_user_trace("mei_hw_ops_read_hdr\n");
		mei_hw_ops_read_hdr_callee(msg, ext);
		break;

	case RPC_ID_mei_hw_ops_read:
		glue_user_trace("mei_hw_ops_read\n");
		mei_hw_ops_read_callee(msg, ext);
		break;

	case RPC_ID_thread_fn:
		glue_user_trace("thread_fn\n");
		thread_fn_callee(msg, ext);
		break;

	case RPC_ID_handler:
		glue_user_trace("handler\n");
		handler_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

