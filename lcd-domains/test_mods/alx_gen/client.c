#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void ethtool_ops_set_settings_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ethtool_ops_set_settings function_ptr = glue_unpack(__pos, msg, ext, fptr_ethtool_ops_set_settings);
	struct net_device* netdev = 0;
	struct net_device** netdev_ptr = &netdev;
	struct ethtool_cmd __ecmd;
	struct ethtool_cmd* ecmd = &__ecmd;
	struct ethtool_cmd** ecmd_ptr = &ecmd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ethtool_ops_set_settings_call_ctx call_ctx = {netdev, ecmd};
	__maybe_unused struct ethtool_ops_set_settings_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ethtool_ops_set_settings__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*ecmd_ptr) {
			callee_unmarshal_kernel__ethtool_ops_set_settings__ecmd__in(__pos, msg, ext, ctx, *ecmd_ptr);
		}

	}

	ret = function_ptr(netdev, ecmd);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ethtool_ops_set_settings__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*ecmd_ptr) {
			callee_marshal_kernel__ethtool_ops_set_settings__ecmd__in(__pos, msg, ext, ctx, *ecmd_ptr);
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

void ethtool_ops_get_settings_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ethtool_ops_get_settings function_ptr = glue_unpack(__pos, msg, ext, fptr_ethtool_ops_get_settings);
	struct net_device* netdev = 0;
	struct net_device** netdev_ptr = &netdev;
	struct ethtool_cmd __ecmd;
	struct ethtool_cmd* ecmd = &__ecmd;
	struct ethtool_cmd** ecmd_ptr = &ecmd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ethtool_ops_get_settings_call_ctx call_ctx = {netdev, ecmd};
	__maybe_unused struct ethtool_ops_get_settings_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ethtool_ops_get_settings__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*ecmd_ptr) {
			callee_unmarshal_kernel__ethtool_ops_get_settings__ecmd__out(__pos, msg, ext, ctx, *ecmd_ptr);
		}

	}

	ret = function_ptr(netdev, ecmd);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ethtool_ops_get_settings__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*ecmd_ptr) {
			callee_marshal_kernel__ethtool_ops_get_settings__ecmd__out(__pos, msg, ext, ctx, *ecmd_ptr);
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

void ethtool_ops_get_msglevel_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ethtool_ops_get_msglevel function_ptr = glue_unpack(__pos, msg, ext, fptr_ethtool_ops_get_msglevel);
	struct net_device* netdev = 0;
	struct net_device** netdev_ptr = &netdev;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct ethtool_ops_get_msglevel_call_ctx call_ctx = {netdev};
	__maybe_unused struct ethtool_ops_get_msglevel_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ethtool_ops_get_msglevel__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	ret = function_ptr(netdev);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ethtool_ops_get_msglevel__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

void ethtool_ops_set_msglevel_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ethtool_ops_set_msglevel function_ptr = glue_unpack(__pos, msg, ext, fptr_ethtool_ops_set_msglevel);
	struct net_device* netdev = 0;
	unsigned int data = 0;
	struct net_device** netdev_ptr = &netdev;
	unsigned int* data_ptr = &data;
	
	__maybe_unused struct ethtool_ops_set_msglevel_call_ctx call_ctx = {netdev, data};
	__maybe_unused struct ethtool_ops_set_msglevel_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ethtool_ops_set_msglevel__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		*data_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	function_ptr(netdev, data);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ethtool_ops_set_msglevel__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void thread_fn_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_thread_fn function_ptr = glue_unpack(__pos, msg, ext, fptr_thread_fn);
	int irq = 0;
	void* id = 0;
	int* irq_ptr = &irq;
	void** id_ptr = &id;
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

	{
		*id_ptr = glue_unpack(__pos, msg, ext, void*);
		if (*id_ptr) {
		}

	}

	ret = function_ptr(irq, id);

	*__pos = 0;
	{
	}

	{
		(void)id_ptr;
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
	void** id_ptr = &id;
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

	{
		*id_ptr = glue_unpack(__pos, msg, ext, void*);
		if (*id_ptr) {
		}

	}

	ret = function_ptr(irq, id);

	*__pos = 0;
	{
	}

	{
		(void)id_ptr;
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
	void** dev_id_ptr = &dev_id;
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
		__maybe_unused const void* __adjusted = *devname_ptr;
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

	{
		__maybe_unused const void* __adjusted = *dev_id_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_id_ptr) {
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
		(void)dev_id_ptr;
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
	void** dev_id_ptr = &dev_id;
	
	__maybe_unused const struct free_irq_call_ctx call_ctx = {irq, dev_id};
	__maybe_unused const struct free_irq_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *irq_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *dev_id_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_id_ptr) {
		}

	}

	glue_call_server(__pos, msg, RPC_ID_free_irq);

	*__pos = 0;
	{
	}

	{
		(void)dev_id_ptr;
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void synchronize_irq(unsigned int irq)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* irq_ptr = &irq;
	
	__maybe_unused const struct synchronize_irq_call_ctx call_ctx = {irq};
	__maybe_unused const struct synchronize_irq_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *irq_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_synchronize_irq);

	*__pos = 0;
	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_unlock(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	
	__maybe_unused const struct rtnl_unlock_call_ctx call_ctx = {};
	__maybe_unused const struct rtnl_unlock_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, msg, RPC_ID_rtnl_unlock);

	*__pos = 0;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_lock(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	
	__maybe_unused const struct rtnl_lock_call_ctx call_ctx = {};
	__maybe_unused const struct rtnl_lock_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, msg, RPC_ID_rtnl_lock);

	*__pos = 0;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

bool cancel_work_sync(struct work_struct* work)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct work_struct** work_ptr = &work;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct cancel_work_sync_call_ctx call_ctx = {work};
	__maybe_unused const struct cancel_work_sync_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *work_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*work_ptr) {
			caller_marshal_kernel__cancel_work_sync__work__in(__pos, msg, ext, ctx, *work_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_cancel_work_sync);

	*__pos = 0;
	{
		if (*work_ptr) {
			caller_unmarshal_kernel__cancel_work_sync__work__in(__pos, msg, ext, ctx, *work_ptr);
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

void probe_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_probe function_ptr = glue_unpack(__pos, msg, ext, fptr_probe);
	struct pci_dev* pdev = 0;
	struct pci_dev** pdev_ptr = &pdev;
	struct pci_device_id __ent;
	struct pci_device_id* ent = &__ent;
	struct pci_device_id** ent_ptr = &ent;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct probe_call_ctx call_ctx = {pdev, ent};
	__maybe_unused struct probe_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct pci_dev*, (sizeof(struct pci_dev)), (DEFAULT_GFP_FLAGS));
		if (*pdev_ptr) {
			callee_unmarshal_kernel__probe__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		if (*ent_ptr) {
			callee_unmarshal_kernel__probe__ent__in(__pos, msg, ext, ctx, *ent_ptr);
		}

	}

	ret = function_ptr(pdev, ent);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__probe__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		if (*ent_ptr) {
			callee_marshal_kernel__probe__ent__in(__pos, msg, ext, ctx, *ent_ptr);
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
	struct pci_dev* pdev = 0;
	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused struct remove_call_ctx call_ctx = {pdev};
	__maybe_unused struct remove_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__remove__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	function_ptr(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__remove__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
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
		__maybe_unused const void* __adjusted = *drv_ptr;
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
		__maybe_unused const void* __adjusted = *drv_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*drv_ptr) {
			caller_marshal_kernel____pci_register_driver__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *mod_name_ptr;
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

void ethtool_ops_get_link_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ethtool_ops_get_link function_ptr = glue_unpack(__pos, msg, ext, fptr_ethtool_ops_get_link);
	struct net_device* netdev = 0;
	struct net_device** netdev_ptr = &netdev;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct ethtool_ops_get_link_call_ctx call_ctx = {netdev};
	__maybe_unused struct ethtool_ops_get_link_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ethtool_ops_get_link__dev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	ret = function_ptr(netdev);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ethtool_ops_get_link__dev__in(__pos, msg, ext, ctx, *netdev_ptr);
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
		__maybe_unused const void* __adjusted = *dev_ptr;
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

void ethtool_ops_get_pauseparam_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ethtool_ops_get_pauseparam function_ptr = glue_unpack(__pos, msg, ext, fptr_ethtool_ops_get_pauseparam);
	struct net_device* netdev = 0;
	struct net_device** netdev_ptr = &netdev;
	struct ethtool_pauseparam __pause;
	struct ethtool_pauseparam* pause = &__pause;
	struct ethtool_pauseparam** pause_ptr = &pause;
	
	__maybe_unused struct ethtool_ops_get_pauseparam_call_ctx call_ctx = {netdev, pause};
	__maybe_unused struct ethtool_ops_get_pauseparam_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ethtool_ops_get_pauseparam__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*pause_ptr) {
			callee_unmarshal_kernel__ethtool_ops_get_pauseparam__pause__out(__pos, msg, ext, ctx, *pause_ptr);
		}

	}

	function_ptr(netdev, pause);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ethtool_ops_get_pauseparam__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*pause_ptr) {
			callee_marshal_kernel__ethtool_ops_get_pauseparam__pause__out(__pos, msg, ext, ctx, *pause_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ethtool_ops_set_pauseparam_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ethtool_ops_set_pauseparam function_ptr = glue_unpack(__pos, msg, ext, fptr_ethtool_ops_set_pauseparam);
	struct net_device* netdev = 0;
	struct net_device** netdev_ptr = &netdev;
	struct ethtool_pauseparam __pause;
	struct ethtool_pauseparam* pause = &__pause;
	struct ethtool_pauseparam** pause_ptr = &pause;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ethtool_ops_set_pauseparam_call_ctx call_ctx = {netdev, pause};
	__maybe_unused struct ethtool_ops_set_pauseparam_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ethtool_ops_set_pauseparam__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*pause_ptr) {
			callee_unmarshal_kernel__ethtool_ops_set_pauseparam__pause__in(__pos, msg, ext, ctx, *pause_ptr);
		}

	}

	ret = function_ptr(netdev, pause);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ethtool_ops_set_pauseparam__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*pause_ptr) {
			callee_marshal_kernel__ethtool_ops_set_pauseparam__pause__in(__pos, msg, ext, ctx, *pause_ptr);
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

void ethtool_ops_get_strings_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ethtool_ops_get_strings function_ptr = glue_unpack(__pos, msg, ext, fptr_ethtool_ops_get_strings);
	struct net_device* netdev = 0;
	unsigned int stringset = 0;
	unsigned char* buf = 0;
	struct net_device** netdev_ptr = &netdev;
	unsigned int* stringset_ptr = &stringset;
	unsigned char** buf_ptr = &buf;
	
	__maybe_unused struct ethtool_ops_get_strings_call_ctx call_ctx = {netdev, stringset, buf};
	__maybe_unused struct ethtool_ops_get_strings_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ethtool_ops_get_strings__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		*stringset_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*buf_ptr = glue_unpack(__pos, msg, ext, unsigned char*);
		(void)buf_ptr;
	}

	function_ptr(netdev, stringset, buf);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ethtool_ops_get_strings__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
	}

	{
		if (*buf_ptr) {
			size_t i, len = (sizeof(__alx_gstrings_stats));
			unsigned char* array = *buf_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ethtool_ops_get_ethtool_stats_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ethtool_ops_get_ethtool_stats function_ptr = glue_unpack(__pos, msg, ext, fptr_ethtool_ops_get_ethtool_stats);
	struct net_device* netdev = 0;
	struct ethtool_stats* estats = 0;
	unsigned long long* data = 0;
	struct net_device** netdev_ptr = &netdev;
	unsigned long long** data_ptr = &data;
	
	__maybe_unused struct ethtool_ops_get_ethtool_stats_call_ctx call_ctx = {netdev, estats, data};
	__maybe_unused struct ethtool_ops_get_ethtool_stats_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ethtool_ops_get_ethtool_stats__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		*data_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned long long*, (sizeof(unsigned long long) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
		(void)data_ptr;
	}

	function_ptr(netdev, estats, data);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ethtool_ops_get_ethtool_stats__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*data_ptr) {
			size_t i, len = (__ALX_NUM_STATS * sizeof(u64));
			unsigned long long* array = *data_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned long long* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ethtool_ops_get_sset_count_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ethtool_ops_get_sset_count function_ptr = glue_unpack(__pos, msg, ext, fptr_ethtool_ops_get_sset_count);
	struct net_device* netdev = 0;
	int sset = 0;
	struct net_device** netdev_ptr = &netdev;
	int* sset_ptr = &sset;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ethtool_ops_get_sset_count_call_ctx call_ctx = {netdev, sset};
	__maybe_unused struct ethtool_ops_get_sset_count_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ethtool_ops_get_sset_count__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		*sset_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = function_ptr(netdev, sset);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ethtool_ops_get_sset_count__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

void net_device_ops_ndo_open_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_net_device_ops_ndo_open function_ptr = glue_unpack(__pos, msg, ext, fptr_net_device_ops_ndo_open);
	struct net_device* netdev = 0;
	struct net_device** netdev_ptr = &netdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct net_device_ops_ndo_open_call_ctx call_ctx = {netdev};
	__maybe_unused struct net_device_ops_ndo_open_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__net_device_ops_ndo_open__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	ret = function_ptr(netdev);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_open__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

void net_device_ops_ndo_stop_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_net_device_ops_ndo_stop function_ptr = glue_unpack(__pos, msg, ext, fptr_net_device_ops_ndo_stop);
	struct net_device* netdev = 0;
	struct net_device** netdev_ptr = &netdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct net_device_ops_ndo_stop_call_ctx call_ctx = {netdev};
	__maybe_unused struct net_device_ops_ndo_stop_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__net_device_ops_ndo_stop__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	ret = function_ptr(netdev);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_stop__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

void net_device_ops_ndo_start_xmit_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_net_device_ops_ndo_start_xmit function_ptr = glue_unpack(__pos, msg, ext, fptr_net_device_ops_ndo_start_xmit);
	struct sk_buff* skb = 0;
	struct net_device* netdev = 0;
	struct sk_buff** skb_ptr = &skb;
	struct net_device** netdev_ptr = &netdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct net_device_ops_ndo_start_xmit_call_ctx call_ctx = {skb, netdev};
	__maybe_unused struct net_device_ops_ndo_start_xmit_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct sk_buff*, (sizeof(struct sk_buff)), (DEFAULT_GFP_FLAGS));
		if (*skb_ptr) {
			callee_unmarshal_kernel__net_device_ops_ndo_start_xmit__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__net_device_ops_ndo_start_xmit__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	ret = function_ptr(skb, netdev);

	*__pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_start_xmit__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		if (*netdev_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_start_xmit__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

void net_device_ops_ndo_set_rx_mode_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_net_device_ops_ndo_set_rx_mode function_ptr = glue_unpack(__pos, msg, ext, fptr_net_device_ops_ndo_set_rx_mode);
	struct net_device* netdev = 0;
	struct net_device** netdev_ptr = &netdev;
	
	__maybe_unused struct net_device_ops_ndo_set_rx_mode_call_ctx call_ctx = {netdev};
	__maybe_unused struct net_device_ops_ndo_set_rx_mode_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	function_ptr(netdev);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_set_rx_mode__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void net_device_ops_ndo_set_mac_address_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_net_device_ops_ndo_set_mac_address function_ptr = glue_unpack(__pos, msg, ext, fptr_net_device_ops_ndo_set_mac_address);
	struct net_device* netdev = 0;
	struct net_device** netdev_ptr = &netdev;
	struct sockaddr __data;
	struct sockaddr* data = &__data;
	struct sockaddr** data_ptr = &data;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct net_device_ops_ndo_set_mac_address_call_ctx call_ctx = {netdev, data};
	__maybe_unused struct net_device_ops_ndo_set_mac_address_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__net_device_ops_ndo_set_mac_address__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*data_ptr) {
			callee_unmarshal_kernel__net_device_ops_ndo_set_mac_address__sockaddr__in(__pos, msg, ext, ctx, *data_ptr);
		}

	}

	ret = function_ptr(netdev, data);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_set_mac_address__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*data_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_set_mac_address__sockaddr__in(__pos, msg, ext, ctx, *data_ptr);
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

void net_device_ops_ndo_do_ioctl_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_net_device_ops_ndo_do_ioctl function_ptr = glue_unpack(__pos, msg, ext, fptr_net_device_ops_ndo_do_ioctl);
	struct net_device* netdev = 0;
	int cmd = 0;
	struct net_device** netdev_ptr = &netdev;
	struct ifreq __ifr;
	struct ifreq* ifr = &__ifr;
	struct ifreq** ifr_ptr = &ifr;
	int* cmd_ptr = &cmd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct net_device_ops_ndo_do_ioctl_call_ctx call_ctx = {netdev, ifr, cmd};
	__maybe_unused struct net_device_ops_ndo_do_ioctl_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__net_device_ops_ndo_do_ioctl__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*ifr_ptr) {
			callee_unmarshal_kernel__net_device_ops_ndo_do_ioctl__ifr__in(__pos, msg, ext, ctx, *ifr_ptr);
		}

	}

	{
		*cmd_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = function_ptr(netdev, ifr, cmd);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_do_ioctl__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*ifr_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_do_ioctl__ifr__in(__pos, msg, ext, ctx, *ifr_ptr);
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

void net_device_ops_ndo_change_mtu_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_net_device_ops_ndo_change_mtu function_ptr = glue_unpack(__pos, msg, ext, fptr_net_device_ops_ndo_change_mtu);
	struct net_device* netdev = 0;
	int mtu = 0;
	struct net_device** netdev_ptr = &netdev;
	int* mtu_ptr = &mtu;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct net_device_ops_ndo_change_mtu_call_ctx call_ctx = {netdev, mtu};
	__maybe_unused struct net_device_ops_ndo_change_mtu_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__net_device_ops_ndo_change_mtu__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		*mtu_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = function_ptr(netdev, mtu);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_change_mtu__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

void net_device_ops_ndo_tx_timeout_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_net_device_ops_ndo_tx_timeout function_ptr = glue_unpack(__pos, msg, ext, fptr_net_device_ops_ndo_tx_timeout);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	__maybe_unused struct net_device_ops_ndo_tx_timeout_call_ctx call_ctx = {dev};
	__maybe_unused struct net_device_ops_ndo_tx_timeout_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__net_device_ops_ndo_tx_timeout__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_tx_timeout__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void net_device_ops_ndo_get_stats64_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_net_device_ops_ndo_get_stats64 function_ptr = glue_unpack(__pos, msg, ext, fptr_net_device_ops_ndo_get_stats64);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	struct rtnl_link_stats64 __net_stats;
	struct rtnl_link_stats64* net_stats = &__net_stats;
	struct rtnl_link_stats64** net_stats_ptr = &net_stats;
	struct rtnl_link_stats64* ret = 0;
	struct rtnl_link_stats64** ret_ptr = &ret;
	
	__maybe_unused struct net_device_ops_ndo_get_stats64_call_ctx call_ctx = {dev, net_stats};
	__maybe_unused struct net_device_ops_ndo_get_stats64_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__net_device_ops_ndo_get_stats64__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*net_stats_ptr) {
			callee_unmarshal_kernel__net_device_ops_ndo_get_stats64__net_stats__in(__pos, msg, ext, ctx, *net_stats_ptr);
		}

	}

	ret = function_ptr(dev, net_stats);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_get_stats64__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*net_stats_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_get_stats64__net_stats__in(__pos, msg, ext, ctx, *net_stats_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_get_stats64__ret_rtnl_link_stats64__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void net_device_ops_ndo_fix_features_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_net_device_ops_ndo_fix_features function_ptr = glue_unpack(__pos, msg, ext, fptr_net_device_ops_ndo_fix_features);
	struct net_device* netdev = 0;
	unsigned long long features = 0;
	struct net_device** netdev_ptr = &netdev;
	unsigned long long* features_ptr = &features;
	unsigned long long ret = 0;
	unsigned long long* ret_ptr = &ret;
	
	__maybe_unused struct net_device_ops_ndo_fix_features_call_ctx call_ctx = {netdev, features};
	__maybe_unused struct net_device_ops_ndo_fix_features_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__net_device_ops_ndo_fix_features__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		*features_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	ret = function_ptr(netdev, features);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_fix_features__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

int pci_bus_write_config_word(struct pci_bus* bus, unsigned int devfn, int pos, unsigned short value)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_bus** bus_ptr = &bus;
	unsigned int* devfn_ptr = &devfn;
	int* pos_ptr = &pos;
	unsigned short* value_ptr = &value;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_bus_write_config_word_call_ctx call_ctx = {bus, devfn, pos, value};
	__maybe_unused const struct pci_bus_write_config_word_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *bus_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__pci_bus_write_config_word__bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *devfn_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *pos_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *value_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_pci_bus_write_config_word);

	*__pos = 0;
	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__pci_bus_write_config_word__bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
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

int pci_bus_read_config_word(struct pci_bus* bus, unsigned int devfn, int pos, unsigned short* value)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_bus** bus_ptr = &bus;
	unsigned int* devfn_ptr = &devfn;
	int* pos_ptr = &pos;
	unsigned short** value_ptr = &value;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_bus_read_config_word_call_ctx call_ctx = {bus, devfn, pos, value};
	__maybe_unused const struct pci_bus_read_config_word_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *bus_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__pci_bus_read_config_word__bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *devfn_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *pos_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *value_ptr;
	}

	glue_call_server(__pos, msg, RPC_ID_pci_bus_read_config_word);

	*__pos = 0;
	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__pci_bus_read_config_word__bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
	}

	{
	}

	{
		if (*value_ptr) {
			**value_ptr = glue_unpack(__pos, msg, ext, unsigned short);
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

void mdio_read_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mdio_read function_ptr = glue_unpack(__pos, msg, ext, fptr_mdio_read);
	struct net_device* dev = 0;
	int prtad = 0;
	int devad = 0;
	unsigned short addr = 0;
	struct net_device** dev_ptr = &dev;
	int* prtad_ptr = &prtad;
	int* devad_ptr = &devad;
	unsigned short* addr_ptr = &addr;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mdio_read_call_ctx call_ctx = {dev, prtad, devad, addr};
	__maybe_unused struct mdio_read_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mdio_read__net_device__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*prtad_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*devad_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*addr_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	ret = function_ptr(dev, prtad, devad, addr);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mdio_read__net_device__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
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

void mdio_write_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mdio_write function_ptr = glue_unpack(__pos, msg, ext, fptr_mdio_write);
	struct net_device* dev = 0;
	int prtad = 0;
	int devad = 0;
	unsigned short addr = 0;
	unsigned short val = 0;
	struct net_device** dev_ptr = &dev;
	int* prtad_ptr = &prtad;
	int* devad_ptr = &devad;
	unsigned short* addr_ptr = &addr;
	unsigned short* val_ptr = &val;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mdio_write_call_ctx call_ctx = {dev, prtad, devad, addr, val};
	__maybe_unused struct mdio_write_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mdio_write__net_device__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*prtad_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*devad_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*addr_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*val_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	ret = function_ptr(dev, prtad, devad, addr, val);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mdio_write__net_device__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
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

int mdio_mii_ioctl(struct mdio_if_info const* mdio, struct mii_ioctl_data* mii_data, int cmd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mdio_if_info const** mdio_ptr = &mdio;
	struct mii_ioctl_data** mii_data_ptr = &mii_data;
	int* cmd_ptr = &cmd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mdio_mii_ioctl_call_ctx call_ctx = {mdio, mii_data, cmd};
	__maybe_unused const struct mdio_mii_ioctl_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *mdio_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*mdio_ptr) {
			caller_marshal_kernel__mdio_mii_ioctl__mdio__in(__pos, msg, ext, ctx, *mdio_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *mii_data_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*mii_data_ptr) {
			caller_marshal_kernel__mdio_mii_ioctl__mii_data__in(__pos, msg, ext, ctx, *mii_data_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *cmd_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_mdio_mii_ioctl);

	*__pos = 0;
	{
		if (*mdio_ptr) {
			struct mdio_if_info* writable = (struct mdio_if_info*)*mdio_ptr;
			caller_unmarshal_kernel__mdio_mii_ioctl__mdio__in(__pos, msg, ext, ctx, writable);
		}

	}

	{
		if (*mii_data_ptr) {
			caller_unmarshal_kernel__mdio_mii_ioctl__mii_data__in(__pos, msg, ext, ctx, *mii_data_ptr);
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
		__maybe_unused const void* __adjusted = *dev_ptr;
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
		__maybe_unused const void* __adjusted = *dev_ptr;
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

bool queue_work_on(int cpu, struct workqueue_struct* wq, struct work_struct* work)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* cpu_ptr = &cpu;
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
		__maybe_unused const void* __adjusted = *work_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*work_ptr) {
			caller_marshal_kernel__queue_work_on__work__in(__pos, msg, ext, ctx, *work_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_queue_work_on);

	*__pos = 0;
	{
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

void consume_skb(struct sk_buff* skb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	
	__maybe_unused const struct consume_skb_call_ctx call_ctx = {skb};
	__maybe_unused const struct consume_skb_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__consume_skb__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_consume_skb);

	*__pos = 0;
	{
		glue_remove_shadow(*skb_ptr);
		if (*skb_ptr) {
			caller_unmarshal_kernel__consume_skb__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void napi_complete_done(struct napi_struct* n, int work_done)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** n_ptr = &n;
	int* work_done_ptr = &work_done;
	
	__maybe_unused const struct napi_complete_done_call_ctx call_ctx = {n, work_done};
	__maybe_unused const struct napi_complete_done_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *n_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*n_ptr) {
			caller_marshal_kernel__napi_complete_done__n__in(__pos, msg, ext, ctx, *n_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *work_done_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_napi_complete_done);

	*__pos = 0;
	{
		if (*n_ptr) {
			caller_unmarshal_kernel__napi_complete_done__n__in(__pos, msg, ext, ctx, *n_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct net_device* alloc_etherdev_mqs(int sizeof_priv, unsigned int txqs, unsigned int rxqs)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* sizeof_priv_ptr = &sizeof_priv;
	unsigned int* txqs_ptr = &txqs;
	unsigned int* rxqs_ptr = &rxqs;
	struct net_device* ret = 0;
	struct net_device** ret_ptr = &ret;
	
	__maybe_unused const struct alloc_etherdev_mqs_call_ctx call_ctx = {sizeof_priv, txqs, rxqs};
	__maybe_unused const struct alloc_etherdev_mqs_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *sizeof_priv_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *txqs_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rxqs_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_alloc_etherdev_mqs);

	*__pos = 0;
	{
	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct net_device*, (sizeof(struct net_device) + ctx->sizeof_priv), (DEFAULT_GFP_FLAGS));
		if (*ret_ptr) {
			caller_unmarshal_kernel__alloc_etherdev_mqs__ret_net_device__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void free_netdev(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct free_netdev_call_ctx call_ctx = {dev};
	__maybe_unused const struct free_netdev_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__free_netdev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_free_netdev);

	*__pos = 0;
	{
		glue_remove_shadow(*dev_ptr);
		if (*dev_ptr) {
			caller_unmarshal_kernel__free_netdev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_napi_del(struct napi_struct* napi)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** napi_ptr = &napi;
	
	__maybe_unused const struct netif_napi_del_call_ctx call_ctx = {napi};
	__maybe_unused const struct netif_napi_del_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *napi_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*napi_ptr) {
			caller_marshal_kernel__netif_napi_del__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_netif_napi_del);

	*__pos = 0;
	{
		if (*napi_ptr) {
			caller_unmarshal_kernel__netif_napi_del__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int rtnl_is_locked(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct rtnl_is_locked_call_ctx call_ctx = {};
	__maybe_unused const struct rtnl_is_locked_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, msg, RPC_ID_rtnl_is_locked);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void net_device_ops_ndo_validate_addr_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_net_device_ops_ndo_validate_addr function_ptr = glue_unpack(__pos, msg, ext, fptr_net_device_ops_ndo_validate_addr);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct net_device_ops_ndo_validate_addr_call_ctx call_ctx = {dev};
	__maybe_unused struct net_device_ops_ndo_validate_addr_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__net_device_ops_ndo_validate_addr__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__net_device_ops_ndo_validate_addr__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void napi_disable(struct napi_struct* n)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** n_ptr = &n;
	
	__maybe_unused const struct napi_disable_call_ctx call_ctx = {n};
	__maybe_unused const struct napi_disable_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *n_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*n_ptr) {
			caller_marshal_kernel__napi_disable__n__in(__pos, msg, ext, ctx, *n_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_napi_disable);

	*__pos = 0;
	{
		if (*n_ptr) {
			caller_unmarshal_kernel__napi_disable__n__in(__pos, msg, ext, ctx, *n_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void poll_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_poll function_ptr = glue_unpack(__pos, msg, ext, fptr_poll);
	struct napi_struct* napi = 0;
	int weight = 0;
	struct napi_struct** napi_ptr = &napi;
	int* weight_ptr = &weight;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct poll_call_ctx call_ctx = {napi, weight};
	__maybe_unused struct poll_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*napi_ptr = glue_unpack(__pos, msg, ext, struct napi_struct*);
		if (*napi_ptr) {
			callee_unmarshal_kernel__poll__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	{
		*weight_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = function_ptr(napi, weight);

	*__pos = 0;
	{
		if (*napi_ptr) {
			callee_marshal_kernel__poll__napi__in(__pos, msg, ext, ctx, *napi_ptr);
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

void netif_napi_add(struct net_device* dev, struct napi_struct* napi, fptr_poll poll, int weight)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	struct napi_struct** napi_ptr = &napi;
	fptr_poll* poll_ptr = &poll;
	int* weight_ptr = &weight;
	
	__maybe_unused const struct netif_napi_add_call_ctx call_ctx = {dev, napi, poll, weight};
	__maybe_unused const struct netif_napi_add_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_napi_add__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *napi_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*napi_ptr) {
			caller_marshal_kernel__netif_napi_add__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *poll_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *weight_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_netif_napi_add);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_napi_add__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*napi_ptr) {
			caller_unmarshal_kernel__netif_napi_add__napi__in(__pos, msg, ext, ctx, *napi_ptr);
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

void __napi_schedule(struct napi_struct* n)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** n_ptr = &n;
	
	__maybe_unused const struct __napi_schedule_call_ctx call_ctx = {n};
	__maybe_unused const struct __napi_schedule_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *n_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*n_ptr) {
			caller_marshal_kernel____napi_schedule__n__in(__pos, msg, ext, ctx, *n_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID___napi_schedule);

	*__pos = 0;
	{
		if (*n_ptr) {
			caller_unmarshal_kernel____napi_schedule__n__in(__pos, msg, ext, ctx, *n_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __dev_kfree_skb_any(struct sk_buff* skb, unsigned int reason)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	unsigned int* reason_ptr = &reason;
	
	__maybe_unused const struct __dev_kfree_skb_any_call_ctx call_ctx = {skb, reason};
	__maybe_unused const struct __dev_kfree_skb_any_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel____dev_kfree_skb_any__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *reason_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID___dev_kfree_skb_any);

	*__pos = 0;
	{
		glue_remove_shadow(*skb_ptr);
		if (*skb_ptr) {
			caller_unmarshal_kernel____dev_kfree_skb_any__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_device_attach(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct netif_device_attach_call_ctx call_ctx = {dev};
	__maybe_unused const struct netif_device_attach_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_device_attach__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_netif_device_attach);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_device_attach__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_tx_wake_queue(struct netdev_queue* dev_queue)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct netdev_queue** dev_queue_ptr = &dev_queue;
	
	__maybe_unused const struct netif_tx_wake_queue_call_ctx call_ctx = {dev_queue};
	__maybe_unused const struct netif_tx_wake_queue_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_queue_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_queue_ptr) {
			caller_marshal_kernel__netif_tx_wake_queue__dev_queue__in(__pos, msg, ext, ctx, *dev_queue_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_netif_tx_wake_queue);

	*__pos = 0;
	{
		if (*dev_queue_ptr) {
			caller_unmarshal_kernel__netif_tx_wake_queue__dev_queue__in(__pos, msg, ext, ctx, *dev_queue_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_device_detach(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct netif_device_detach_call_ctx call_ctx = {dev};
	__maybe_unused const struct netif_device_detach_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_device_detach__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_netif_device_detach);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_device_detach__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_schedule_queue(struct netdev_queue* txq)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct netdev_queue** txq_ptr = &txq;
	
	__maybe_unused const struct netif_schedule_queue_call_ctx call_ctx = {txq};
	__maybe_unused const struct netif_schedule_queue_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *txq_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*txq_ptr) {
			caller_marshal_kernel__netif_schedule_queue__txq__in(__pos, msg, ext, ctx, *txq_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_netif_schedule_queue);

	*__pos = 0;
	{
		if (*txq_ptr) {
			caller_unmarshal_kernel__netif_schedule_queue__txq__in(__pos, msg, ext, ctx, *txq_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void unregister_netdev(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct unregister_netdev_call_ctx call_ctx = {dev};
	__maybe_unused const struct unregister_netdev_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__unregister_netdev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_unregister_netdev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__unregister_netdev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int register_netdev(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct register_netdev_call_ctx call_ctx = {dev};
	__maybe_unused const struct register_netdev_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__register_netdev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_register_netdev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__register_netdev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void netdev_update_features(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct netdev_update_features_call_ctx call_ctx = {dev};
	__maybe_unused const struct netdev_update_features_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netdev_update_features__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_netdev_update_features);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netdev_update_features__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_carrier_on(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct netif_carrier_on_call_ctx call_ctx = {dev};
	__maybe_unused const struct netif_carrier_on_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_carrier_on__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_netif_carrier_on);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_carrier_on__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_carrier_off(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct netif_carrier_off_call_ctx call_ctx = {dev};
	__maybe_unused const struct netif_carrier_off_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_carrier_off__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_netif_carrier_off);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_carrier_off__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

unsigned int napi_gro_receive(struct napi_struct* napi, struct sk_buff* skb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** napi_ptr = &napi;
	struct sk_buff** skb_ptr = &skb;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct napi_gro_receive_call_ctx call_ctx = {napi, skb};
	__maybe_unused const struct napi_gro_receive_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *napi_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*napi_ptr) {
			caller_marshal_kernel__napi_gro_receive__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__napi_gro_receive__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_napi_gro_receive);

	*__pos = 0;
	{
		if (*napi_ptr) {
			caller_unmarshal_kernel__napi_gro_receive__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	{
		if (*skb_ptr) {
			caller_unmarshal_kernel__napi_gro_receive__skb__in(__pos, msg, ext, ctx, *skb_ptr);
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

unsigned char* skb_put(struct sk_buff* skb, unsigned int len)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	unsigned int* len_ptr = &len;
	unsigned char* ret = 0;
	unsigned char** ret_ptr = &ret;
	
	__maybe_unused const struct skb_put_call_ctx call_ctx = {skb, len};
	__maybe_unused const struct skb_put_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__skb_put__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *len_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_skb_put);

	*__pos = 0;
	{
		if (*skb_ptr) {
			caller_unmarshal_kernel__skb_put__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, unsigned char*);
		if (*ret_ptr) {
			**ret_ptr = glue_unpack(__pos, msg, ext, unsigned char);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct sk_buff* __netdev_alloc_skb(struct net_device* dev, unsigned int len, unsigned int gfp_mask)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	unsigned int* len_ptr = &len;
	unsigned int* gfp_mask_ptr = &gfp_mask;
	struct sk_buff* ret = 0;
	struct sk_buff** ret_ptr = &ret;
	
	__maybe_unused const struct __netdev_alloc_skb_call_ctx call_ctx = {dev, len, gfp_mask};
	__maybe_unused const struct __netdev_alloc_skb_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel____netdev_alloc_skb__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *len_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *gfp_mask_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID___netdev_alloc_skb);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel____netdev_alloc_skb__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, struct sk_buff*);
		if (*ret_ptr) {
			caller_unmarshal_kernel____netdev_alloc_skb__ret_sk_buff__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pci_select_bars(struct pci_dev* dev, unsigned long flags)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	unsigned long* flags_ptr = &flags;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_select_bars_call_ctx call_ctx = {dev, flags};
	__maybe_unused const struct pci_select_bars_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_select_bars__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *flags_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_pci_select_bars);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_select_bars__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

int pcie_set_readrq(struct pci_dev* dev, int rq)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int* rq_ptr = &rq;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pcie_set_readrq_call_ctx call_ctx = {dev, rq};
	__maybe_unused const struct pcie_set_readrq_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pcie_set_readrq__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *rq_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_pcie_set_readrq);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pcie_set_readrq__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

int pcie_get_readrq(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pcie_get_readrq_call_ctx call_ctx = {dev};
	__maybe_unused const struct pcie_get_readrq_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pcie_get_readrq__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_pcie_get_readrq);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pcie_get_readrq__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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
		__maybe_unused const void* __adjusted = *dev_ptr;
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

int pci_enable_pcie_error_reporting(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_pcie_error_reporting_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_enable_pcie_error_reporting_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_pci_enable_pcie_error_reporting);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(__pos, msg, ext, ctx, *dev_ptr);
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

int pci_disable_pcie_error_reporting(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_disable_pcie_error_reporting_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_disable_pcie_error_reporting_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_pci_disable_pcie_error_reporting);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(__pos, msg, ext, ctx, *dev_ptr);
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

int pci_request_selected_regions(struct pci_dev* pdev, int bars, char const* res_name)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	int* bars_ptr = &bars;
	char const** res_name_ptr = &res_name;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_request_selected_regions_call_ctx call_ctx = {pdev, bars, res_name};
	__maybe_unused const struct pci_request_selected_regions_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_request_selected_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *bars_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *res_name_ptr;
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

	glue_call_server(__pos, msg, RPC_ID_pci_request_selected_regions);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_request_selected_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
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

void pci_release_selected_regions(struct pci_dev* pdev, int bars)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	int* bars_ptr = &bars;
	
	__maybe_unused const struct pci_release_selected_regions_call_ctx call_ctx = {pdev, bars};
	__maybe_unused const struct pci_release_selected_regions_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_release_selected_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *bars_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_pci_release_selected_regions);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_release_selected_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
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
		__maybe_unused const void* __adjusted = *dev_ptr;
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

int pci_enable_device_mem(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_device_mem_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_enable_device_mem_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_device_mem__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_pci_enable_device_mem);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_device_mem__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void* pci_ioremap_bar(struct pci_dev* pdev, int bar)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	int* bar_ptr = &bar;
	cptr_t ioremap_cptr;
	gpa_t ioremap_gpa;
	uint64_t ioremap_len;
	void* ret = 0;
	void** ret_ptr = &ret;
	
	__maybe_unused const struct pci_ioremap_bar_call_ctx call_ctx = {pdev, bar};
	__maybe_unused const struct pci_ioremap_bar_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		lcd_cptr_alloc(&ioremap_cptr);
		glue_pack(__pos, msg, ext, cptr_val(ioremap_cptr));
	}

	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_ioremap_bar__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *bar_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_pci_ioremap_bar);

	*__pos = 0;
	{
		ioremap_len = glue_unpack(__pos, msg, ext, uint64_t);
		lcd_ioremap_phys(ioremap_cptr, ioremap_len, &ioremap_gpa);
		*ret_ptr = lcd_ioremap(gpa_val(ioremap_gpa), ioremap_len);
	}

	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_ioremap_bar__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
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

	case RPC_ID_ethtool_ops_set_settings:
		glue_user_trace("ethtool_ops_set_settings\n");
		ethtool_ops_set_settings_callee(msg, ext);
		break;

	case RPC_ID_ethtool_ops_get_settings:
		glue_user_trace("ethtool_ops_get_settings\n");
		ethtool_ops_get_settings_callee(msg, ext);
		break;

	case RPC_ID_ethtool_ops_get_msglevel:
		glue_user_trace("ethtool_ops_get_msglevel\n");
		ethtool_ops_get_msglevel_callee(msg, ext);
		break;

	case RPC_ID_ethtool_ops_set_msglevel:
		glue_user_trace("ethtool_ops_set_msglevel\n");
		ethtool_ops_set_msglevel_callee(msg, ext);
		break;

	case RPC_ID_thread_fn:
		glue_user_trace("thread_fn\n");
		thread_fn_callee(msg, ext);
		break;

	case RPC_ID_handler:
		glue_user_trace("handler\n");
		handler_callee(msg, ext);
		break;

	case RPC_ID_probe:
		glue_user_trace("probe\n");
		probe_callee(msg, ext);
		break;

	case RPC_ID_remove:
		glue_user_trace("remove\n");
		remove_callee(msg, ext);
		break;

	case RPC_ID_ethtool_ops_get_link:
		glue_user_trace("ethtool_ops_get_link\n");
		ethtool_ops_get_link_callee(msg, ext);
		break;

	case RPC_ID_ethtool_ops_get_pauseparam:
		glue_user_trace("ethtool_ops_get_pauseparam\n");
		ethtool_ops_get_pauseparam_callee(msg, ext);
		break;

	case RPC_ID_ethtool_ops_set_pauseparam:
		glue_user_trace("ethtool_ops_set_pauseparam\n");
		ethtool_ops_set_pauseparam_callee(msg, ext);
		break;

	case RPC_ID_ethtool_ops_get_strings:
		glue_user_trace("ethtool_ops_get_strings\n");
		ethtool_ops_get_strings_callee(msg, ext);
		break;

	case RPC_ID_ethtool_ops_get_ethtool_stats:
		glue_user_trace("ethtool_ops_get_ethtool_stats\n");
		ethtool_ops_get_ethtool_stats_callee(msg, ext);
		break;

	case RPC_ID_ethtool_ops_get_sset_count:
		glue_user_trace("ethtool_ops_get_sset_count\n");
		ethtool_ops_get_sset_count_callee(msg, ext);
		break;

	case RPC_ID_net_device_ops_ndo_open:
		glue_user_trace("net_device_ops_ndo_open\n");
		net_device_ops_ndo_open_callee(msg, ext);
		break;

	case RPC_ID_net_device_ops_ndo_stop:
		glue_user_trace("net_device_ops_ndo_stop\n");
		net_device_ops_ndo_stop_callee(msg, ext);
		break;

	case RPC_ID_net_device_ops_ndo_start_xmit:
		glue_user_trace("net_device_ops_ndo_start_xmit\n");
		net_device_ops_ndo_start_xmit_callee(msg, ext);
		break;

	case RPC_ID_net_device_ops_ndo_set_rx_mode:
		glue_user_trace("net_device_ops_ndo_set_rx_mode\n");
		net_device_ops_ndo_set_rx_mode_callee(msg, ext);
		break;

	case RPC_ID_net_device_ops_ndo_set_mac_address:
		glue_user_trace("net_device_ops_ndo_set_mac_address\n");
		net_device_ops_ndo_set_mac_address_callee(msg, ext);
		break;

	case RPC_ID_net_device_ops_ndo_do_ioctl:
		glue_user_trace("net_device_ops_ndo_do_ioctl\n");
		net_device_ops_ndo_do_ioctl_callee(msg, ext);
		break;

	case RPC_ID_net_device_ops_ndo_change_mtu:
		glue_user_trace("net_device_ops_ndo_change_mtu\n");
		net_device_ops_ndo_change_mtu_callee(msg, ext);
		break;

	case RPC_ID_net_device_ops_ndo_tx_timeout:
		glue_user_trace("net_device_ops_ndo_tx_timeout\n");
		net_device_ops_ndo_tx_timeout_callee(msg, ext);
		break;

	case RPC_ID_net_device_ops_ndo_get_stats64:
		glue_user_trace("net_device_ops_ndo_get_stats64\n");
		net_device_ops_ndo_get_stats64_callee(msg, ext);
		break;

	case RPC_ID_net_device_ops_ndo_fix_features:
		glue_user_trace("net_device_ops_ndo_fix_features\n");
		net_device_ops_ndo_fix_features_callee(msg, ext);
		break;

	case RPC_ID_mdio_read:
		glue_user_trace("mdio_read\n");
		mdio_read_callee(msg, ext);
		break;

	case RPC_ID_mdio_write:
		glue_user_trace("mdio_write\n");
		mdio_write_callee(msg, ext);
		break;

	case RPC_ID_net_device_ops_ndo_validate_addr:
		glue_user_trace("net_device_ops_ndo_validate_addr\n");
		net_device_ops_ndo_validate_addr_callee(msg, ext);
		break;

	case RPC_ID_poll:
		glue_user_trace("poll\n");
		poll_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

