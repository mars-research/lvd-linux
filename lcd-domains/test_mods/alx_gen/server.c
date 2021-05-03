#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

int trmp_impl_ethtool_ops_set_settings(fptr_ethtool_ops_set_settings target, struct net_device* netdev, struct ethtool_cmd* ecmd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	struct ethtool_cmd** ecmd_ptr = &ecmd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ethtool_ops_set_settings_call_ctx call_ctx = {netdev, ecmd};
	__maybe_unused const struct ethtool_ops_set_settings_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ethtool_ops_set_settings__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ecmd_ptr;
	}

	glue_call_client(__pos, msg, RPC_ID_ethtool_ops_set_settings);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ethtool_ops_set_settings__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*ecmd_ptr) {
			caller_unmarshal_kernel__ethtool_ops_set_settings__ecmd__in(__pos, msg, ext, ctx, *ecmd_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_set_settings)
int LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_set_settings) trmp_ethtool_ops_set_settings(struct net_device* netdev, struct ethtool_cmd* ecmd)
{
	volatile fptr_impl_ethtool_ops_set_settings impl;
	fptr_ethtool_ops_set_settings target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ethtool_ops_set_settings);
	impl = trmp_impl_ethtool_ops_set_settings;
	return impl(target, netdev, ecmd);
}

int trmp_impl_ethtool_ops_get_settings(fptr_ethtool_ops_get_settings target, struct net_device* netdev, struct ethtool_cmd* ecmd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	struct ethtool_cmd** ecmd_ptr = &ecmd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ethtool_ops_get_settings_call_ctx call_ctx = {netdev, ecmd};
	__maybe_unused const struct ethtool_ops_get_settings_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ethtool_ops_get_settings__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ecmd_ptr;
	}

	glue_call_client(__pos, msg, RPC_ID_ethtool_ops_get_settings);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ethtool_ops_get_settings__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*ecmd_ptr) {
			caller_unmarshal_kernel__ethtool_ops_get_settings__ecmd__out(__pos, msg, ext, ctx, *ecmd_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_get_settings)
int LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_get_settings) trmp_ethtool_ops_get_settings(struct net_device* netdev, struct ethtool_cmd* ecmd)
{
	volatile fptr_impl_ethtool_ops_get_settings impl;
	fptr_ethtool_ops_get_settings target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ethtool_ops_get_settings);
	impl = trmp_impl_ethtool_ops_get_settings;
	return impl(target, netdev, ecmd);
}

unsigned int trmp_impl_ethtool_ops_get_msglevel(fptr_ethtool_ops_get_msglevel target, struct net_device* netdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct ethtool_ops_get_msglevel_call_ctx call_ctx = {netdev};
	__maybe_unused const struct ethtool_ops_get_msglevel_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ethtool_ops_get_msglevel__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_ethtool_ops_get_msglevel);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ethtool_ops_get_msglevel__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_get_msglevel)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_get_msglevel) trmp_ethtool_ops_get_msglevel(struct net_device* netdev)
{
	volatile fptr_impl_ethtool_ops_get_msglevel impl;
	fptr_ethtool_ops_get_msglevel target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ethtool_ops_get_msglevel);
	impl = trmp_impl_ethtool_ops_get_msglevel;
	return impl(target, netdev);
}

void trmp_impl_ethtool_ops_set_msglevel(fptr_ethtool_ops_set_msglevel target, struct net_device* netdev, unsigned int data)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	unsigned int* data_ptr = &data;
	
	__maybe_unused const struct ethtool_ops_set_msglevel_call_ctx call_ctx = {netdev, data};
	__maybe_unused const struct ethtool_ops_set_msglevel_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ethtool_ops_set_msglevel__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *data_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_ethtool_ops_set_msglevel);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ethtool_ops_set_msglevel__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_set_msglevel)
void LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_set_msglevel) trmp_ethtool_ops_set_msglevel(struct net_device* netdev, unsigned int data)
{
	volatile fptr_impl_ethtool_ops_set_msglevel impl;
	fptr_ethtool_ops_set_msglevel target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ethtool_ops_set_msglevel);
	impl = trmp_impl_ethtool_ops_set_msglevel;
	return impl(target, netdev, data);
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
		*irqflags_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		size_t __size = sizeof(char) * glue_peek(__pos, msg, ext);
		*devname_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
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

	dev_id = kzalloc(sizeof(void*), GFP_KERNEL);
	ret = request_threaded_irq(irq, handler, thread_fn, irqflags, devname, dev_id);
	printk("%s, returned ret %d for irq %d (%s)\n", __func__, ret, irq, devname);

	*__pos = 0;
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
	void** dev_id_ptr = &dev_id;
	
	__maybe_unused struct free_irq_call_ctx call_ctx = {irq, dev_id};
	__maybe_unused struct free_irq_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*dev_id_ptr = glue_unpack(__pos, msg, ext, void*);
		if (*dev_id_ptr) {
		}

	}

	free_irq(irq, dev_id);

	*__pos = 0;
	{
	}

	{
		(void)dev_id_ptr;
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void synchronize_irq_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int irq = 0;
	unsigned int* irq_ptr = &irq;
	
	__maybe_unused struct synchronize_irq_call_ctx call_ctx = {irq};
	__maybe_unused struct synchronize_irq_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	synchronize_irq(irq);

	*__pos = 0;
	{
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_unlock_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	
	__maybe_unused struct rtnl_unlock_call_ctx call_ctx = {};
	__maybe_unused struct rtnl_unlock_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	rtnl_unlock();

	*__pos = 0;
	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_lock_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	
	__maybe_unused struct rtnl_lock_call_ctx call_ctx = {};
	__maybe_unused struct rtnl_lock_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	rtnl_lock();

	*__pos = 0;
	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void cancel_work_sync_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct work_struct* work = 0;
	struct work_struct** work_ptr = &work;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct cancel_work_sync_call_ctx call_ctx = {work};
	__maybe_unused struct cancel_work_sync_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*work_ptr = glue_unpack_shadow(__pos, msg, ext, struct work_struct*);
		if (*work_ptr) {
			callee_unmarshal_kernel__cancel_work_sync__work__in(__pos, msg, ext, ctx, *work_ptr);
		}

	}

	ret = cancel_work_sync(work);

	*__pos = 0;
	{
		if (*work_ptr) {
			callee_marshal_kernel__cancel_work_sync__work__in(__pos, msg, ext, ctx, *work_ptr);
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

int trmp_impl_probe(fptr_probe target, struct pci_dev* pdev, struct pci_device_id* ent)
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
	
	__maybe_unused const struct probe_call_ctx call_ctx = {pdev, ent};
	__maybe_unused const struct probe_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__probe__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ent_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ent_ptr) {
			caller_marshal_kernel__probe__ent__in(__pos, msg, ext, ctx, *ent_ptr);
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
		if (*ent_ptr) {
			caller_unmarshal_kernel__probe__ent__in(__pos, msg, ext, ctx, *ent_ptr);
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
int LCD_TRAMPOLINE_LINKAGE(trmp_probe) trmp_probe(struct pci_dev* pdev, struct pci_device_id* ent)
{
	volatile fptr_impl_probe impl;
	fptr_probe target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_probe);
	impl = trmp_impl_probe;
	return impl(target, pdev, ent);
}

void trmp_impl_remove(fptr_remove target, struct pci_dev* pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused const struct remove_call_ctx call_ctx = {pdev};
	__maybe_unused const struct remove_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
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

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_remove)
void LCD_TRAMPOLINE_LINKAGE(trmp_remove) trmp_remove(struct pci_dev* pdev)
{
	volatile fptr_impl_remove impl;
	fptr_remove target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_remove);
	impl = trmp_impl_remove;
	return impl(target, pdev);
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
	char const** mod_name_ptr = &mod_name;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct __pci_register_driver_call_ctx call_ctx = {drv, owner, mod_name};
	__maybe_unused struct __pci_register_driver_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct pci_driver);
		*drv_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct pci_driver*, (__size), (DEFAULT_GFP_FLAGS));
		if (*drv_ptr) {
			callee_unmarshal_kernel____pci_register_driver__drv__in(__pos, msg, ext, ctx, *drv_ptr);
		}

	}

	{
		size_t __size = sizeof(char) * glue_peek(__pos, msg, ext);
		*mod_name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
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

	ret = __pci_register_driver(drv, THIS_MODULE, mod_name);

	*__pos = 0;
	{
		if (*drv_ptr) {
			callee_marshal_kernel____pci_register_driver__drv__in(__pos, msg, ext, ctx, *drv_ptr);
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

unsigned int trmp_impl_ethtool_ops_get_link(fptr_ethtool_ops_get_link target, struct net_device* netdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct ethtool_ops_get_link_call_ctx call_ctx = {netdev};
	__maybe_unused const struct ethtool_ops_get_link_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ethtool_ops_get_link__dev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_ethtool_ops_get_link);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ethtool_ops_get_link__dev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_get_link)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_get_link) trmp_ethtool_ops_get_link(struct net_device* netdev)
{
	volatile fptr_impl_ethtool_ops_get_link impl;
	fptr_ethtool_ops_get_link target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ethtool_ops_get_link);
	impl = trmp_impl_ethtool_ops_get_link;
	return impl(target, netdev);
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

void trmp_impl_ethtool_ops_get_pauseparam(fptr_ethtool_ops_get_pauseparam target, struct net_device* netdev, struct ethtool_pauseparam* pause)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	struct ethtool_pauseparam** pause_ptr = &pause;
	
	__maybe_unused const struct ethtool_ops_get_pauseparam_call_ctx call_ctx = {netdev, pause};
	__maybe_unused const struct ethtool_ops_get_pauseparam_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ethtool_ops_get_pauseparam__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *pause_ptr;
	}

	glue_call_client(__pos, msg, RPC_ID_ethtool_ops_get_pauseparam);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ethtool_ops_get_pauseparam__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*pause_ptr) {
			caller_unmarshal_kernel__ethtool_ops_get_pauseparam__pause__out(__pos, msg, ext, ctx, *pause_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_get_pauseparam)
void LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_get_pauseparam) trmp_ethtool_ops_get_pauseparam(struct net_device* netdev, struct ethtool_pauseparam* pause)
{
	volatile fptr_impl_ethtool_ops_get_pauseparam impl;
	fptr_ethtool_ops_get_pauseparam target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ethtool_ops_get_pauseparam);
	impl = trmp_impl_ethtool_ops_get_pauseparam;
	return impl(target, netdev, pause);
}

int trmp_impl_ethtool_ops_set_pauseparam(fptr_ethtool_ops_set_pauseparam target, struct net_device* netdev, struct ethtool_pauseparam* pause)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	struct ethtool_pauseparam** pause_ptr = &pause;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ethtool_ops_set_pauseparam_call_ctx call_ctx = {netdev, pause};
	__maybe_unused const struct ethtool_ops_set_pauseparam_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ethtool_ops_set_pauseparam__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *pause_ptr;
	}

	glue_call_client(__pos, msg, RPC_ID_ethtool_ops_set_pauseparam);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ethtool_ops_set_pauseparam__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*pause_ptr) {
			caller_unmarshal_kernel__ethtool_ops_set_pauseparam__pause__in(__pos, msg, ext, ctx, *pause_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_set_pauseparam)
int LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_set_pauseparam) trmp_ethtool_ops_set_pauseparam(struct net_device* netdev, struct ethtool_pauseparam* pause)
{
	volatile fptr_impl_ethtool_ops_set_pauseparam impl;
	fptr_ethtool_ops_set_pauseparam target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ethtool_ops_set_pauseparam);
	impl = trmp_impl_ethtool_ops_set_pauseparam;
	return impl(target, netdev, pause);
}

void trmp_impl_ethtool_ops_get_strings(fptr_ethtool_ops_get_strings target, struct net_device* netdev, unsigned int stringset, unsigned char* buf)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	unsigned int* stringset_ptr = &stringset;
	unsigned char** buf_ptr = &buf;
	
	__maybe_unused const struct ethtool_ops_get_strings_call_ctx call_ctx = {netdev, stringset, buf};
	__maybe_unused const struct ethtool_ops_get_strings_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ethtool_ops_get_strings__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *stringset_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *buf_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		(void)buf_ptr;
	}

	glue_call_client(__pos, msg, RPC_ID_ethtool_ops_get_strings);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ethtool_ops_get_strings__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
	}

	{
		if (*buf_ptr) {
			int i;
			unsigned char* array = *buf_ptr;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, unsigned char);
			}

		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_get_strings)
void LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_get_strings) trmp_ethtool_ops_get_strings(struct net_device* netdev, unsigned int stringset, unsigned char* buf)
{
	volatile fptr_impl_ethtool_ops_get_strings impl;
	fptr_ethtool_ops_get_strings target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ethtool_ops_get_strings);
	impl = trmp_impl_ethtool_ops_get_strings;
	return impl(target, netdev, stringset, buf);
}

void trmp_impl_ethtool_ops_get_ethtool_stats(fptr_ethtool_ops_get_ethtool_stats target, struct net_device* netdev, struct ethtool_stats* estats, unsigned long long* data)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	unsigned long long** data_ptr = &data;
	
	__maybe_unused const struct ethtool_ops_get_ethtool_stats_call_ctx call_ctx = {netdev, estats, data};
	__maybe_unused const struct ethtool_ops_get_ethtool_stats_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ethtool_ops_get_ethtool_stats__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		(void)data_ptr;
	}

	glue_call_client(__pos, msg, RPC_ID_ethtool_ops_get_ethtool_stats);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ethtool_ops_get_ethtool_stats__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*data_ptr) {
			int i;
			unsigned long long* array = *data_ptr;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned long long* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, unsigned long long);
			}

		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_get_ethtool_stats)
void LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_get_ethtool_stats) trmp_ethtool_ops_get_ethtool_stats(struct net_device* netdev, struct ethtool_stats* estats, unsigned long long* data)
{
	volatile fptr_impl_ethtool_ops_get_ethtool_stats impl;
	fptr_ethtool_ops_get_ethtool_stats target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ethtool_ops_get_ethtool_stats);
	impl = trmp_impl_ethtool_ops_get_ethtool_stats;
	return impl(target, netdev, estats, data);
}

int trmp_impl_ethtool_ops_get_sset_count(fptr_ethtool_ops_get_sset_count target, struct net_device* netdev, int sset)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	int* sset_ptr = &sset;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ethtool_ops_get_sset_count_call_ctx call_ctx = {netdev, sset};
	__maybe_unused const struct ethtool_ops_get_sset_count_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ethtool_ops_get_sset_count__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *sset_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_ethtool_ops_get_sset_count);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ethtool_ops_get_sset_count__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ethtool_ops_get_sset_count)
int LCD_TRAMPOLINE_LINKAGE(trmp_ethtool_ops_get_sset_count) trmp_ethtool_ops_get_sset_count(struct net_device* netdev, int sset)
{
	volatile fptr_impl_ethtool_ops_get_sset_count impl;
	fptr_ethtool_ops_get_sset_count target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ethtool_ops_get_sset_count);
	impl = trmp_impl_ethtool_ops_get_sset_count;
	return impl(target, netdev, sset);
}

int trmp_impl_net_device_ops_ndo_open(fptr_net_device_ops_ndo_open target, struct net_device* netdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct net_device_ops_ndo_open_call_ctx call_ctx = {netdev};
	__maybe_unused const struct net_device_ops_ndo_open_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__net_device_ops_ndo_open__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_net_device_ops_ndo_open);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__net_device_ops_ndo_open__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_open)
int LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_open) trmp_net_device_ops_ndo_open(struct net_device* netdev)
{
	volatile fptr_impl_net_device_ops_ndo_open impl;
	fptr_net_device_ops_ndo_open target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_net_device_ops_ndo_open);
	impl = trmp_impl_net_device_ops_ndo_open;
	return impl(target, netdev);
}

int trmp_impl_net_device_ops_ndo_stop(fptr_net_device_ops_ndo_stop target, struct net_device* netdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct net_device_ops_ndo_stop_call_ctx call_ctx = {netdev};
	__maybe_unused const struct net_device_ops_ndo_stop_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__net_device_ops_ndo_stop__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_net_device_ops_ndo_stop);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__net_device_ops_ndo_stop__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_stop)
int LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_stop) trmp_net_device_ops_ndo_stop(struct net_device* netdev)
{
	volatile fptr_impl_net_device_ops_ndo_stop impl;
	fptr_net_device_ops_ndo_stop target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_net_device_ops_ndo_stop);
	impl = trmp_impl_net_device_ops_ndo_stop;
	return impl(target, netdev);
}

int trmp_impl_net_device_ops_ndo_start_xmit(fptr_net_device_ops_ndo_start_xmit target, struct sk_buff* skb, struct net_device* netdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	struct net_device** netdev_ptr = &netdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct net_device_ops_ndo_start_xmit_call_ctx call_ctx = {skb, netdev};
	__maybe_unused const struct net_device_ops_ndo_start_xmit_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
		return 0;
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__net_device_ops_ndo_start_xmit__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__net_device_ops_ndo_start_xmit__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_net_device_ops_ndo_start_xmit);

	*__pos = 0;
	{
		if (*skb_ptr) {
			caller_unmarshal_kernel__net_device_ops_ndo_start_xmit__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__net_device_ops_ndo_start_xmit__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_start_xmit)
int LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_start_xmit) trmp_net_device_ops_ndo_start_xmit(struct sk_buff* skb, struct net_device* netdev)
{
	volatile fptr_impl_net_device_ops_ndo_start_xmit impl;
	fptr_net_device_ops_ndo_start_xmit target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_net_device_ops_ndo_start_xmit);
	impl = trmp_impl_net_device_ops_ndo_start_xmit;
	return impl(target, skb, netdev);
}

void trmp_impl_net_device_ops_ndo_set_rx_mode(fptr_net_device_ops_ndo_set_rx_mode target, struct net_device* netdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	
	__maybe_unused const struct net_device_ops_ndo_set_rx_mode_call_ctx call_ctx = {netdev};
	__maybe_unused const struct net_device_ops_ndo_set_rx_mode_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__net_device_ops_ndo_set_rx_mode__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_net_device_ops_ndo_set_rx_mode);

	*__pos = 0;
	if (0)
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_set_rx_mode)
void LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_set_rx_mode) trmp_net_device_ops_ndo_set_rx_mode(struct net_device* netdev)
{
	volatile fptr_impl_net_device_ops_ndo_set_rx_mode impl;
	fptr_net_device_ops_ndo_set_rx_mode target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_net_device_ops_ndo_set_rx_mode);
	impl = trmp_impl_net_device_ops_ndo_set_rx_mode;
	return impl(target, netdev);
}

int trmp_impl_net_device_ops_ndo_set_mac_address(fptr_net_device_ops_ndo_set_mac_address target, struct net_device* netdev, struct sockaddr* data)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	struct sockaddr** data_ptr = &data;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct net_device_ops_ndo_set_mac_address_call_ctx call_ctx = {netdev, data};
	__maybe_unused const struct net_device_ops_ndo_set_mac_address_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__net_device_ops_ndo_set_mac_address__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
	}

	glue_call_client(__pos, msg, RPC_ID_net_device_ops_ndo_set_mac_address);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__net_device_ops_ndo_set_mac_address__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*data_ptr) {
			caller_unmarshal_kernel__net_device_ops_ndo_set_mac_address__sockaddr__in(__pos, msg, ext, ctx, *data_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_set_mac_address)
int LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_set_mac_address) trmp_net_device_ops_ndo_set_mac_address(struct net_device* netdev, struct sockaddr* data)
{
	volatile fptr_impl_net_device_ops_ndo_set_mac_address impl;
	fptr_net_device_ops_ndo_set_mac_address target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_net_device_ops_ndo_set_mac_address);
	impl = trmp_impl_net_device_ops_ndo_set_mac_address;
	return impl(target, netdev, data);
}

int trmp_impl_net_device_ops_ndo_do_ioctl(fptr_net_device_ops_ndo_do_ioctl target, struct net_device* netdev, struct ifreq* ifr, int cmd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	struct ifreq** ifr_ptr = &ifr;
	int* cmd_ptr = &cmd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct net_device_ops_ndo_do_ioctl_call_ctx call_ctx = {netdev, ifr, cmd};
	__maybe_unused const struct net_device_ops_ndo_do_ioctl_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__net_device_ops_ndo_do_ioctl__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ifr_ptr;
	}

	{
		glue_pack(__pos, msg, ext, *cmd_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_net_device_ops_ndo_do_ioctl);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__net_device_ops_ndo_do_ioctl__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*ifr_ptr) {
			caller_unmarshal_kernel__net_device_ops_ndo_do_ioctl__ifr__in(__pos, msg, ext, ctx, *ifr_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_do_ioctl)
int LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_do_ioctl) trmp_net_device_ops_ndo_do_ioctl(struct net_device* netdev, struct ifreq* ifr, int cmd)
{
	volatile fptr_impl_net_device_ops_ndo_do_ioctl impl;
	fptr_net_device_ops_ndo_do_ioctl target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_net_device_ops_ndo_do_ioctl);
	impl = trmp_impl_net_device_ops_ndo_do_ioctl;
	return impl(target, netdev, ifr, cmd);
}

int trmp_impl_net_device_ops_ndo_change_mtu(fptr_net_device_ops_ndo_change_mtu target, struct net_device* netdev, int mtu)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	int* mtu_ptr = &mtu;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct net_device_ops_ndo_change_mtu_call_ctx call_ctx = {netdev, mtu};
	__maybe_unused const struct net_device_ops_ndo_change_mtu_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__net_device_ops_ndo_change_mtu__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *mtu_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_net_device_ops_ndo_change_mtu);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__net_device_ops_ndo_change_mtu__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_change_mtu)
int LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_change_mtu) trmp_net_device_ops_ndo_change_mtu(struct net_device* netdev, int mtu)
{
	volatile fptr_impl_net_device_ops_ndo_change_mtu impl;
	fptr_net_device_ops_ndo_change_mtu target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_net_device_ops_ndo_change_mtu);
	impl = trmp_impl_net_device_ops_ndo_change_mtu;
	return impl(target, netdev, mtu);
}

void trmp_impl_net_device_ops_ndo_tx_timeout(fptr_net_device_ops_ndo_tx_timeout target, struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct net_device_ops_ndo_tx_timeout_call_ctx call_ctx = {dev};
	__maybe_unused const struct net_device_ops_ndo_tx_timeout_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__net_device_ops_ndo_tx_timeout__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_net_device_ops_ndo_tx_timeout);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__net_device_ops_ndo_tx_timeout__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_tx_timeout)
void LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_tx_timeout) trmp_net_device_ops_ndo_tx_timeout(struct net_device* dev)
{
	volatile fptr_impl_net_device_ops_ndo_tx_timeout impl;
	fptr_net_device_ops_ndo_tx_timeout target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_net_device_ops_ndo_tx_timeout);
	impl = trmp_impl_net_device_ops_ndo_tx_timeout;
	return impl(target, dev);
}

struct rtnl_link_stats64* trmp_impl_net_device_ops_ndo_get_stats64(fptr_net_device_ops_ndo_get_stats64 target, struct net_device* dev, struct rtnl_link_stats64* net_stats)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	struct rtnl_link_stats64** net_stats_ptr = &net_stats;
	
	__maybe_unused const struct net_device_ops_ndo_get_stats64_call_ctx call_ctx = {dev, net_stats};
	__maybe_unused const struct net_device_ops_ndo_get_stats64_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__net_device_ops_ndo_get_stats64__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *net_stats_ptr;
	}

	glue_call_client(__pos, msg, RPC_ID_net_device_ops_ndo_get_stats64);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__net_device_ops_ndo_get_stats64__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*net_stats_ptr) {
			caller_unmarshal_kernel__net_device_ops_ndo_get_stats64__net_stats__in(__pos, msg, ext, ctx, *net_stats_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return net_stats;
}

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_get_stats64)
struct rtnl_link_stats64* LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_get_stats64) trmp_net_device_ops_ndo_get_stats64(struct net_device* dev, struct rtnl_link_stats64* net_stats)
{
	volatile fptr_impl_net_device_ops_ndo_get_stats64 impl;
	fptr_net_device_ops_ndo_get_stats64 target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_net_device_ops_ndo_get_stats64);
	impl = trmp_impl_net_device_ops_ndo_get_stats64;
	return impl(target, dev, net_stats);
}

unsigned long long trmp_impl_net_device_ops_ndo_fix_features(fptr_net_device_ops_ndo_fix_features target, struct net_device* netdev, unsigned long long features)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	unsigned long long* features_ptr = &features;
	unsigned long long ret = 0;
	unsigned long long* ret_ptr = &ret;
	
	__maybe_unused const struct net_device_ops_ndo_fix_features_call_ctx call_ctx = {netdev, features};
	__maybe_unused const struct net_device_ops_ndo_fix_features_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__net_device_ops_ndo_fix_features__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *features_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_net_device_ops_ndo_fix_features);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__net_device_ops_ndo_fix_features__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_fix_features)
unsigned long long LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_fix_features) trmp_net_device_ops_ndo_fix_features(struct net_device* netdev, unsigned long long features)
{
	volatile fptr_impl_net_device_ops_ndo_fix_features impl;
	fptr_net_device_ops_ndo_fix_features target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_net_device_ops_ndo_fix_features);
	impl = trmp_impl_net_device_ops_ndo_fix_features;
	return impl(target, netdev, features);
}

void pci_bus_write_config_word_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_bus* bus = 0;
	unsigned int devfn = 0;
	int pos = 0;
	unsigned short value = 0;
	struct pci_bus** bus_ptr = &bus;
	unsigned int* devfn_ptr = &devfn;
	int* pos_ptr = &pos;
	unsigned short* value_ptr = &value;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_bus_write_config_word_call_ctx call_ctx = {bus, devfn, pos, value};
	__maybe_unused struct pci_bus_write_config_word_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*bus_ptr = glue_unpack(__pos, msg, ext, struct pci_bus*);
		if (*bus_ptr) {
			callee_unmarshal_kernel__pci_bus_write_config_word__bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		*devfn_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*pos_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*value_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	ret = pci_bus_write_config_word(bus, devfn, pos, value);

	*__pos = 0;
	{
		if (*bus_ptr) {
			callee_marshal_kernel__pci_bus_write_config_word__bus__in(__pos, msg, ext, ctx, *bus_ptr);
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

void pci_bus_read_config_word_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_bus* bus = 0;
	unsigned int devfn = 0;
	int pos = 0;
	struct pci_bus** bus_ptr = &bus;
	unsigned int* devfn_ptr = &devfn;
	int* pos_ptr = &pos;
	unsigned short __value;
	unsigned short* value = &__value;
	unsigned short** value_ptr = &value;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_bus_read_config_word_call_ctx call_ctx = {bus, devfn, pos, value};
	__maybe_unused struct pci_bus_read_config_word_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*bus_ptr = glue_unpack(__pos, msg, ext, struct pci_bus*);
		if (*bus_ptr) {
			callee_unmarshal_kernel__pci_bus_read_config_word__bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		*devfn_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*pos_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		(void)value_ptr;
	}

	ret = pci_bus_read_config_word(bus, devfn, pos, value);

	*__pos = 0;
	{
		if (*bus_ptr) {
			callee_marshal_kernel__pci_bus_read_config_word__bus__in(__pos, msg, ext, ctx, *bus_ptr);
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

int trmp_impl_mdio_read(fptr_mdio_read target, struct net_device* dev, int prtad, int devad, unsigned short addr)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	int* prtad_ptr = &prtad;
	int* devad_ptr = &devad;
	unsigned short* addr_ptr = &addr;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mdio_read_call_ctx call_ctx = {dev, prtad, devad, addr};
	__maybe_unused const struct mdio_read_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mdio_read__net_device__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *prtad_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *devad_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *addr_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_mdio_read);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mdio_read__net_device__in(__pos, msg, ext, ctx, *dev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_mdio_read)
int LCD_TRAMPOLINE_LINKAGE(trmp_mdio_read) trmp_mdio_read(struct net_device* dev, int prtad, int devad, unsigned short addr)
{
	volatile fptr_impl_mdio_read impl;
	fptr_mdio_read target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mdio_read);
	impl = trmp_impl_mdio_read;
	return impl(target, dev, prtad, devad, addr);
}

int trmp_impl_mdio_write(fptr_mdio_write target, struct net_device* dev, int prtad, int devad, unsigned short addr, unsigned short val)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	int* prtad_ptr = &prtad;
	int* devad_ptr = &devad;
	unsigned short* addr_ptr = &addr;
	unsigned short* val_ptr = &val;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mdio_write_call_ctx call_ctx = {dev, prtad, devad, addr, val};
	__maybe_unused const struct mdio_write_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mdio_write__net_device__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *prtad_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *devad_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *addr_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *val_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_mdio_write);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mdio_write__net_device__in(__pos, msg, ext, ctx, *dev_ptr);
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
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_mdio_write)
int LCD_TRAMPOLINE_LINKAGE(trmp_mdio_write) trmp_mdio_write(struct net_device* dev, int prtad, int devad, unsigned short addr, unsigned short val)
{
	volatile fptr_impl_mdio_write impl;
	fptr_mdio_write target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_mdio_write);
	impl = trmp_impl_mdio_write;
	return impl(target, dev, prtad, devad, addr, val);
}

void mdio_mii_ioctl_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mdio_if_info const* mdio = 0;
	struct mii_ioctl_data* mii_data = 0;
	int cmd = 0;
	struct mdio_if_info const** mdio_ptr = &mdio;
	struct mii_ioctl_data** mii_data_ptr = &mii_data;
	int* cmd_ptr = &cmd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mdio_mii_ioctl_call_ctx call_ctx = {mdio, mii_data, cmd};
	__maybe_unused struct mdio_mii_ioctl_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct mdio_if_info);
		*mdio_ptr = glue_unpack_bind_or_new_shadow(__pos, msg, ext, struct mdio_if_info const*, __size);
		if (*mdio_ptr) {
			struct mdio_if_info* writable = (struct mdio_if_info*)*mdio_ptr;
			callee_unmarshal_kernel__mdio_mii_ioctl__mdio__in(__pos, msg, ext, ctx, writable);
		}

	}

	{
		*mii_data_ptr = glue_unpack(__pos, msg, ext, struct mii_ioctl_data*);
		if (*mii_data_ptr) {
			callee_unmarshal_kernel__mdio_mii_ioctl__mii_data__in(__pos, msg, ext, ctx, *mii_data_ptr);
		}

	}

	{
		*cmd_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = mdio_mii_ioctl(mdio, mii_data, cmd);

	*__pos = 0;
	{
		if (*mdio_ptr) {
			callee_marshal_kernel__mdio_mii_ioctl__mdio__in(__pos, msg, ext, ctx, *mdio_ptr);
		}

	}

	{
		if (*mii_data_ptr) {
			callee_marshal_kernel__mdio_mii_ioctl__mii_data__in(__pos, msg, ext, ctx, *mii_data_ptr);
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

void queue_work_on_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int cpu = 0;
	struct workqueue_struct* wq = 0;
	struct work_struct* work = 0;
	int* cpu_ptr = &cpu;
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
		*work_ptr = glue_unpack_shadow(__pos, msg, ext, struct work_struct*);
		if (*work_ptr) {
			callee_unmarshal_kernel__queue_work_on__work__in(__pos, msg, ext, ctx, *work_ptr);
		}

	}

	ret = queue_work_on(WORK_CPU_UNBOUND, system_wq, work);

	*__pos = 0;
	{
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

void consume_skb_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* skb = 0;
	struct sk_buff** skb_ptr = &skb;
	
	__maybe_unused struct consume_skb_call_ctx call_ctx = {skb};
	__maybe_unused struct consume_skb_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack(__pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__consume_skb__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	consume_skb(skb);

	*__pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel__consume_skb__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void napi_complete_done_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct* n = 0;
	int work_done = 0;
	struct napi_struct** n_ptr = &n;
	int* work_done_ptr = &work_done;
	
	__maybe_unused struct napi_complete_done_call_ctx call_ctx = {n, work_done};
	__maybe_unused struct napi_complete_done_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*n_ptr = glue_unpack_shadow(__pos, msg, ext, struct napi_struct*);
		if (*n_ptr) {
			callee_unmarshal_kernel__napi_complete_done__n__in(__pos, msg, ext, ctx, *n_ptr);
		}

	}

	{
		*work_done_ptr = glue_unpack(__pos, msg, ext, int);
	}

	napi_complete_done(n, work_done);

	*__pos = 0;
	{
		if (*n_ptr) {
			callee_marshal_kernel__napi_complete_done__n__in(__pos, msg, ext, ctx, *n_ptr);
		}

	}

	{
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void alloc_etherdev_mqs_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int sizeof_priv = 0;
	unsigned int txqs = 0;
	unsigned int rxqs = 0;
	int* sizeof_priv_ptr = &sizeof_priv;
	unsigned int* txqs_ptr = &txqs;
	unsigned int* rxqs_ptr = &rxqs;
	struct net_device* ret = 0;
	struct net_device** ret_ptr = &ret;
	
	__maybe_unused struct alloc_etherdev_mqs_call_ctx call_ctx = {sizeof_priv, txqs, rxqs};
	__maybe_unused struct alloc_etherdev_mqs_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sizeof_priv_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*txqs_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*rxqs_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = alloc_etherdev_mqs(sizeof_priv, txqs, rxqs);

	*__pos = 0;
	{
	}

	{
	}

	{
	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__alloc_etherdev_mqs__ret_net_device__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void free_netdev_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	__maybe_unused struct free_netdev_call_ctx call_ctx = {dev};
	__maybe_unused struct free_netdev_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__free_netdev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	free_netdev(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__free_netdev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_napi_del_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct* napi = 0;
	struct napi_struct** napi_ptr = &napi;
	
	__maybe_unused struct netif_napi_del_call_ctx call_ctx = {napi};
	__maybe_unused struct netif_napi_del_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*napi_ptr = glue_unpack_shadow(__pos, msg, ext, struct napi_struct*);
		if (*napi_ptr) {
			callee_unmarshal_kernel__netif_napi_del__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	netif_napi_del(napi);

	*__pos = 0;
	{
		if (*napi_ptr) {
			callee_marshal_kernel__netif_napi_del__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

		glue_remove_shadow(*napi_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_is_locked_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct rtnl_is_locked_call_ctx call_ctx = {};
	__maybe_unused struct rtnl_is_locked_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = rtnl_is_locked();

	*__pos = 0;
	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int trmp_impl_net_device_ops_ndo_validate_addr(fptr_net_device_ops_ndo_validate_addr target, struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct net_device_ops_ndo_validate_addr_call_ctx call_ctx = {dev};
	__maybe_unused const struct net_device_ops_ndo_validate_addr_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__net_device_ops_ndo_validate_addr__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_net_device_ops_ndo_validate_addr);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__net_device_ops_ndo_validate_addr__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_net_device_ops_ndo_validate_addr)
int LCD_TRAMPOLINE_LINKAGE(trmp_net_device_ops_ndo_validate_addr) trmp_net_device_ops_ndo_validate_addr(struct net_device* dev)
{
	volatile fptr_impl_net_device_ops_ndo_validate_addr impl;
	fptr_net_device_ops_ndo_validate_addr target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_net_device_ops_ndo_validate_addr);
	impl = trmp_impl_net_device_ops_ndo_validate_addr;
	return impl(target, dev);
}

void napi_disable_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct* n = 0;
	struct napi_struct** n_ptr = &n;
	
	__maybe_unused struct napi_disable_call_ctx call_ctx = {n};
	__maybe_unused struct napi_disable_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*n_ptr = glue_unpack_shadow(__pos, msg, ext, struct napi_struct*);
		if (*n_ptr) {
			callee_unmarshal_kernel__napi_disable__n__in(__pos, msg, ext, ctx, *n_ptr);
		}

	}

	napi_disable(n);

	*__pos = 0;
	{
		if (*n_ptr) {
			callee_marshal_kernel__napi_disable__n__in(__pos, msg, ext, ctx, *n_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int trmp_impl_poll(fptr_poll target, struct napi_struct* napi, int weight)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** napi_ptr = &napi;
	int* weight_ptr = &weight;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct poll_call_ctx call_ctx = {napi, weight};
	__maybe_unused const struct poll_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *napi_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*napi_ptr) {
			caller_marshal_kernel__poll__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *weight_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_poll);

	*__pos = 0;
	{
		if (*napi_ptr) {
			caller_unmarshal_kernel__poll__napi__in(__pos, msg, ext, ctx, *napi_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_poll)
int LCD_TRAMPOLINE_LINKAGE(trmp_poll) trmp_poll(struct napi_struct* napi, int weight)
{
	volatile fptr_impl_poll impl;
	fptr_poll target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_poll);
	impl = trmp_impl_poll;
	return impl(target, napi, weight);
}

void netif_napi_add_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct napi_struct* napi = 0;
	fptr_poll poll = 0;
	int weight = 0;
	struct net_device** dev_ptr = &dev;
	struct napi_struct** napi_ptr = &napi;
	fptr_poll* poll_ptr = &poll;
	int* weight_ptr = &weight;
	
	__maybe_unused struct netif_napi_add_call_ctx call_ctx = {dev, napi, poll, weight};
	__maybe_unused struct netif_napi_add_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netif_napi_add__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		size_t __size = sizeof(struct napi_struct);
		*napi_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct napi_struct*, (__size), (DEFAULT_GFP_FLAGS));
		if (*napi_ptr) {
			callee_unmarshal_kernel__netif_napi_add__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	{
		*poll_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, poll);
	}

	{
		*weight_ptr = glue_unpack(__pos, msg, ext, int);
	}

	netif_napi_add(dev, napi, poll, weight);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netif_napi_add__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*napi_ptr) {
			callee_marshal_kernel__netif_napi_add__napi__in(__pos, msg, ext, ctx, *napi_ptr);
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

void __napi_schedule_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct* n = 0;
	struct napi_struct** n_ptr = &n;
	
	__maybe_unused struct __napi_schedule_call_ctx call_ctx = {n};
	__maybe_unused struct __napi_schedule_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*n_ptr = glue_unpack_shadow(__pos, msg, ext, struct napi_struct*);
		if (*n_ptr) {
			callee_unmarshal_kernel____napi_schedule__n__in(__pos, msg, ext, ctx, *n_ptr);
		}

	}

	__napi_schedule(n);

	*__pos = 0;
	{
		if (*n_ptr) {
			callee_marshal_kernel____napi_schedule__n__in(__pos, msg, ext, ctx, *n_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __dev_kfree_skb_any_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* skb = 0;
	unsigned int reason = 0;
	struct sk_buff** skb_ptr = &skb;
	unsigned int* reason_ptr = &reason;
	
	__maybe_unused struct __dev_kfree_skb_any_call_ctx call_ctx = {skb, reason};
	__maybe_unused struct __dev_kfree_skb_any_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack(__pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel____dev_kfree_skb_any__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		*reason_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	__dev_kfree_skb_any(skb, reason);

	*__pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel____dev_kfree_skb_any__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_device_attach_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	__maybe_unused struct netif_device_attach_call_ctx call_ctx = {dev};
	__maybe_unused struct netif_device_attach_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netif_device_attach__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	netif_device_attach(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netif_device_attach__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_tx_wake_queue_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct netdev_queue* dev_queue = 0;
	struct netdev_queue** dev_queue_ptr = &dev_queue;
	
	__maybe_unused struct netif_tx_wake_queue_call_ctx call_ctx = {dev_queue};
	__maybe_unused struct netif_tx_wake_queue_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_queue_ptr = glue_unpack(__pos, msg, ext, struct netdev_queue*);
		if (*dev_queue_ptr) {
			callee_unmarshal_kernel__netif_tx_wake_queue__dev_queue__in(__pos, msg, ext, ctx, *dev_queue_ptr);
		}

	}

	netif_tx_wake_queue(dev_queue);

	*__pos = 0;
	{
		if (*dev_queue_ptr) {
			callee_marshal_kernel__netif_tx_wake_queue__dev_queue__in(__pos, msg, ext, ctx, *dev_queue_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_device_detach_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	__maybe_unused struct netif_device_detach_call_ctx call_ctx = {dev};
	__maybe_unused struct netif_device_detach_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netif_device_detach__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	netif_device_detach(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netif_device_detach__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_schedule_queue_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct netdev_queue* txq = 0;
	struct netdev_queue** txq_ptr = &txq;
	
	__maybe_unused struct netif_schedule_queue_call_ctx call_ctx = {txq};
	__maybe_unused struct netif_schedule_queue_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*txq_ptr = glue_unpack(__pos, msg, ext, struct netdev_queue*);
		if (*txq_ptr) {
			callee_unmarshal_kernel__netif_schedule_queue__txq__in(__pos, msg, ext, ctx, *txq_ptr);
		}

	}

	netif_schedule_queue(txq);

	*__pos = 0;
	{
		if (*txq_ptr) {
			callee_marshal_kernel__netif_schedule_queue__txq__in(__pos, msg, ext, ctx, *txq_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void unregister_netdev_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	__maybe_unused struct unregister_netdev_call_ctx call_ctx = {dev};
	__maybe_unused struct unregister_netdev_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__unregister_netdev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	unregister_netdev(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__unregister_netdev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void register_netdev_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct register_netdev_call_ctx call_ctx = {dev};
	__maybe_unused struct register_netdev_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__register_netdev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = register_netdev(dev);

	printk("%s:%d register_netdev returned %d\n", __func__, __LINE__, ret);
	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__register_netdev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void netdev_update_features_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	__maybe_unused struct netdev_update_features_call_ctx call_ctx = {dev};
	__maybe_unused struct netdev_update_features_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netdev_update_features__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	netdev_update_features(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netdev_update_features__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_carrier_on_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	__maybe_unused struct netif_carrier_on_call_ctx call_ctx = {dev};
	__maybe_unused struct netif_carrier_on_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netif_carrier_on__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	netif_carrier_on(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netif_carrier_on__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_carrier_off_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	__maybe_unused struct netif_carrier_off_call_ctx call_ctx = {dev};
	__maybe_unused struct netif_carrier_off_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netif_carrier_off__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	netif_carrier_off(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netif_carrier_off__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void napi_gro_receive_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct* napi = 0;
	struct sk_buff* skb = 0;
	struct napi_struct** napi_ptr = &napi;
	struct sk_buff** skb_ptr = &skb;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct napi_gro_receive_call_ctx call_ctx = {napi, skb};
	__maybe_unused struct napi_gro_receive_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*napi_ptr = glue_unpack_shadow(__pos, msg, ext, struct napi_struct*);
		if (*napi_ptr) {
			callee_unmarshal_kernel__napi_gro_receive__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	{
		size_t __size = sizeof(struct sk_buff);
		*skb_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct sk_buff*, (__size), (DEFAULT_GFP_FLAGS));
		if (*skb_ptr) {
			callee_unmarshal_kernel__napi_gro_receive__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	ret = napi_gro_receive(napi, skb);

	*__pos = 0;
	{
		if (*napi_ptr) {
			callee_marshal_kernel__napi_gro_receive__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	{
		if (*skb_ptr) {
			callee_marshal_kernel__napi_gro_receive__skb__in(__pos, msg, ext, ctx, *skb_ptr);
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

void skb_put_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* skb = 0;
	unsigned int len = 0;
	struct sk_buff** skb_ptr = &skb;
	unsigned int* len_ptr = &len;
	unsigned char* ret = 0;
	unsigned char** ret_ptr = &ret;
	
	__maybe_unused struct skb_put_call_ctx call_ctx = {skb, len};
	__maybe_unused struct skb_put_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack(__pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__skb_put__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = skb_put(skb, len);

	*__pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel__skb_put__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			glue_pack(__pos, msg, ext, **ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __netdev_alloc_skb_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	unsigned int len = 0;
	unsigned int gfp_mask = 0;
	struct net_device** dev_ptr = &dev;
	unsigned int* len_ptr = &len;
	unsigned int* gfp_mask_ptr = &gfp_mask;
	struct sk_buff* ret = 0;
	struct sk_buff** ret_ptr = &ret;
	
	__maybe_unused struct __netdev_alloc_skb_call_ctx call_ctx = {dev, len, gfp_mask};
	__maybe_unused struct __netdev_alloc_skb_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel____netdev_alloc_skb__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*gfp_mask_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = __netdev_alloc_skb(dev, len, gfp_mask);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel____netdev_alloc_skb__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel____netdev_alloc_skb__ret_sk_buff__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_select_bars_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	unsigned long flags = 0;
	struct pci_dev** dev_ptr = &dev;
	unsigned long* flags_ptr = &flags;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_select_bars_call_ctx call_ctx = {dev, flags};
	__maybe_unused struct pci_select_bars_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_select_bars__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*flags_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	ret = pci_select_bars(dev, flags);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_select_bars__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void pcie_set_readrq_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	int rq = 0;
	struct pci_dev** dev_ptr = &dev;
	int* rq_ptr = &rq;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pcie_set_readrq_call_ctx call_ctx = {dev, rq};
	__maybe_unused struct pcie_set_readrq_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pcie_set_readrq__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*rq_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = pcie_set_readrq(dev, rq);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pcie_set_readrq__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void pcie_get_readrq_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pcie_get_readrq_call_ctx call_ctx = {dev};
	__maybe_unused struct pcie_get_readrq_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pcie_get_readrq__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = pcie_get_readrq(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pcie_get_readrq__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void pci_enable_pcie_error_reporting_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_enable_pcie_error_reporting_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_enable_pcie_error_reporting_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = pci_enable_pcie_error_reporting(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void pci_disable_pcie_error_reporting_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_disable_pcie_error_reporting_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_disable_pcie_error_reporting_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = pci_disable_pcie_error_reporting(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void pci_request_selected_regions_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* pdev = 0;
	int bars = 0;
	char const* res_name = 0;
	struct pci_dev** pdev_ptr = &pdev;
	int* bars_ptr = &bars;
	char const** res_name_ptr = &res_name;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_request_selected_regions_call_ctx call_ctx = {pdev, bars, res_name};
	__maybe_unused struct pci_request_selected_regions_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_request_selected_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		*bars_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		size_t __size = sizeof(char) * glue_peek(__pos, msg, ext);
		*res_name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
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

	ret = pci_request_selected_regions(pdev, bars, res_name);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_request_selected_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
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

void pci_release_selected_regions_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* pdev = 0;
	int bars = 0;
	struct pci_dev** pdev_ptr = &pdev;
	int* bars_ptr = &bars;
	
	__maybe_unused struct pci_release_selected_regions_call_ctx call_ctx = {pdev, bars};
	__maybe_unused struct pci_release_selected_regions_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_release_selected_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		*bars_ptr = glue_unpack(__pos, msg, ext, int);
	}

	pci_release_selected_regions(pdev, bars);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_release_selected_regions__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
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

void pci_enable_device_mem_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_enable_device_mem_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_enable_device_mem_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_enable_device_mem__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = pci_enable_device_mem(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_enable_device_mem__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void pci_ioremap_bar_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* pdev = 0;
	int bar = 0;
	struct pci_dev** pdev_ptr = &pdev;
	int* bar_ptr = &bar;
	cptr_t resource_cptr;
	__maybe_unused unsigned int resource_len;
	cptr_t lcd_resource_cptr;
	
	__maybe_unused struct pci_ioremap_bar_call_ctx call_ctx = {pdev, bar};
	__maybe_unused struct pci_ioremap_bar_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		lcd_resource_cptr.cptr = glue_unpack(__pos, msg, ext, uint64_t);
	}

	{
		*pdev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__pci_ioremap_bar__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		*bar_ptr = glue_unpack(__pos, msg, ext, int);
	}

	//ret = pci_ioremap_bar(pdev, bar);

	*__pos = 0;

	{
		lcd_volunteer_dev_mem(__gpa((uint64_t)pci_resource_start(pdev, bar)), get_order(pci_resource_len(pdev, bar)), &resource_cptr);
		copy_msg_cap_vmfunc(current->lcd, current->vmfunc_lcd, resource_cptr, lcd_resource_cptr);
		glue_pack(__pos, msg, ext, pci_resource_len(pdev, bar));
	}

	{
		if (*pdev_ptr) {
			callee_marshal_kernel__pci_ioremap_bar__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void get_loops_per_jiffy_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused struct get_loops_per_jiffy_call_ctx call_ctx = {};
	__maybe_unused struct get_loops_per_jiffy_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = loops_per_jiffy;

	*__pos = 0;
	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void get_jiffies_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused struct get_jiffies_call_ctx call_ctx = {};
	__maybe_unused struct get_jiffies_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = jiffies;

	*__pos = 0;
	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void eth_validate_addr_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct eth_validate_addr_call_ctx call_ctx = {dev};
	__maybe_unused struct eth_validate_addr_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__eth_validate_addr__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = eth_validate_addr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__eth_validate_addr__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void ethtool_op_get_link_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct ethtool_op_get_link_call_ctx call_ctx = {dev};
	__maybe_unused struct ethtool_op_get_link_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ethtool_op_get_link__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = ethtool_op_get_link(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ethtool_op_get_link__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void lvd_netif_tx_start_all_queues_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	__maybe_unused struct lvd_netif_tx_start_all_queues_call_ctx call_ctx = {dev};
	__maybe_unused struct lvd_netif_tx_start_all_queues_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__lvd_netif_tx_start_all_queues__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	netif_tx_start_all_queues(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__lvd_netif_tx_start_all_queues__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void trmp_impl_work_fn(fptr_work_fn target, struct work_struct* work)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct work_struct** work_ptr = &work;
	
	__maybe_unused const struct work_fn_call_ctx call_ctx = {work};
	__maybe_unused const struct work_fn_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		__maybe_unused const void* __adjusted = *work_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*work_ptr) {
			caller_marshal_kernel__work_fn__work__in(__pos, msg, ext, ctx, *work_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_work_fn);

	*__pos = 0;
	{
		if (*work_ptr) {
			caller_unmarshal_kernel__work_fn__work__in(__pos, msg, ext, ctx, *work_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_work_fn)
void LCD_TRAMPOLINE_LINKAGE(trmp_work_fn) trmp_work_fn(struct work_struct* work)
{
	volatile fptr_impl_work_fn impl;
	fptr_work_fn target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_work_fn);
	impl = trmp_impl_work_fn;
	return impl(target, work);
}

void lvd_init_work_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct work_struct* work = 0;
	fptr_work_fn work_fn = 0;
	struct work_struct** work_ptr = &work;
	fptr_work_fn* work_fn_ptr = &work_fn;
	
	__maybe_unused struct lvd_init_work_call_ctx call_ctx = {work, work_fn};
	__maybe_unused struct lvd_init_work_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct work_struct);
		*work_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct work_struct*, (__size), (DEFAULT_GFP_FLAGS));
		if (*work_ptr) {
			callee_unmarshal_kernel__lvd_init_work__work__in(__pos, msg, ext, ctx, *work_ptr);
		}

	}

	{
		*work_fn_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, work_fn);
	}

	printk("%s, initializing work %p | work_fn %p\n", __func__, work, work_fn);

	INIT_WORK((work), (work_fn));

	printk("%s, initializing work %p | work->data %lx\n", __func__, work, work->data.counter);
	*__pos = 0;
	{
		if (*work_ptr) {
			callee_marshal_kernel__lvd_init_work__work__in(__pos, msg, ext, ctx, *work_ptr);
		}

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

	case RPC_ID_request_threaded_irq:
		glue_user_trace("request_threaded_irq\n");
		request_threaded_irq_callee(msg, ext);
		break;

	case RPC_ID_free_irq:
		glue_user_trace("free_irq\n");
		free_irq_callee(msg, ext);
		break;

	case RPC_ID_synchronize_irq:
		glue_user_trace("synchronize_irq\n");
		synchronize_irq_callee(msg, ext);
		break;

	case RPC_ID_rtnl_unlock:
		glue_user_trace("rtnl_unlock\n");
		rtnl_unlock_callee(msg, ext);
		break;

	case RPC_ID_rtnl_lock:
		glue_user_trace("rtnl_lock\n");
		rtnl_lock_callee(msg, ext);
		break;

	case RPC_ID_cancel_work_sync:
		glue_user_trace("cancel_work_sync\n");
		cancel_work_sync_callee(msg, ext);
		break;

	case RPC_ID_pci_unregister_driver:
		glue_user_trace("pci_unregister_driver\n");
		pci_unregister_driver_callee(msg, ext);
		break;

	case RPC_ID___pci_register_driver:
		glue_user_trace("__pci_register_driver\n");
		__pci_register_driver_callee(msg, ext);
		break;

	case RPC_ID_pci_enable_msi_range:
		glue_user_trace("pci_enable_msi_range\n");
		pci_enable_msi_range_callee(msg, ext);
		break;

	case RPC_ID_pci_bus_write_config_word:
		glue_user_trace("pci_bus_write_config_word\n");
		pci_bus_write_config_word_callee(msg, ext);
		break;

	case RPC_ID_pci_bus_read_config_word:
		glue_user_trace("pci_bus_read_config_word\n");
		pci_bus_read_config_word_callee(msg, ext);
		break;

	case RPC_ID_mdio_mii_ioctl:
		glue_user_trace("mdio_mii_ioctl\n");
		mdio_mii_ioctl_callee(msg, ext);
		break;

	case RPC_ID_pci_disable_msi:
		glue_user_trace("pci_disable_msi\n");
		pci_disable_msi_callee(msg, ext);
		break;

	case RPC_ID_pci_disable_device:
		glue_user_trace("pci_disable_device\n");
		pci_disable_device_callee(msg, ext);
		break;

	case RPC_ID_queue_work_on:
		glue_user_trace("queue_work_on\n");
		queue_work_on_callee(msg, ext);
		break;

	case RPC_ID_consume_skb:
		glue_user_trace("consume_skb\n");
		consume_skb_callee(msg, ext);
		break;

	case RPC_ID_napi_complete_done:
		glue_user_trace("napi_complete_done\n");
		napi_complete_done_callee(msg, ext);
		break;

	case RPC_ID_alloc_etherdev_mqs:
		glue_user_trace("alloc_etherdev_mqs\n");
		alloc_etherdev_mqs_callee(msg, ext);
		break;

	case RPC_ID_free_netdev:
		glue_user_trace("free_netdev\n");
		free_netdev_callee(msg, ext);
		break;

	case RPC_ID_netif_napi_del:
		glue_user_trace("netif_napi_del\n");
		netif_napi_del_callee(msg, ext);
		break;

	case RPC_ID_rtnl_is_locked:
		glue_user_trace("rtnl_is_locked\n");
		rtnl_is_locked_callee(msg, ext);
		break;

	case RPC_ID_napi_disable:
		glue_user_trace("napi_disable\n");
		napi_disable_callee(msg, ext);
		break;

	case RPC_ID_netif_napi_add:
		glue_user_trace("netif_napi_add\n");
		netif_napi_add_callee(msg, ext);
		break;

	case RPC_ID___napi_schedule:
		glue_user_trace("__napi_schedule\n");
		__napi_schedule_callee(msg, ext);
		break;

	case RPC_ID___dev_kfree_skb_any:
		glue_user_trace("__dev_kfree_skb_any\n");
		__dev_kfree_skb_any_callee(msg, ext);
		break;

	case RPC_ID_netif_device_attach:
		glue_user_trace("netif_device_attach\n");
		netif_device_attach_callee(msg, ext);
		break;

	case RPC_ID_netif_tx_wake_queue:
		glue_user_trace("netif_tx_wake_queue\n");
		netif_tx_wake_queue_callee(msg, ext);
		break;

	case RPC_ID_netif_device_detach:
		glue_user_trace("netif_device_detach\n");
		netif_device_detach_callee(msg, ext);
		break;

	case RPC_ID_netif_schedule_queue:
		glue_user_trace("netif_schedule_queue\n");
		netif_schedule_queue_callee(msg, ext);
		break;

	case RPC_ID_unregister_netdev:
		glue_user_trace("unregister_netdev\n");
		unregister_netdev_callee(msg, ext);
		break;

	case RPC_ID_register_netdev:
		glue_user_trace("register_netdev\n");
		register_netdev_callee(msg, ext);
		break;

	case RPC_ID_netdev_update_features:
		glue_user_trace("netdev_update_features\n");
		netdev_update_features_callee(msg, ext);
		break;

	case RPC_ID_netif_carrier_on:
		glue_user_trace("netif_carrier_on\n");
		netif_carrier_on_callee(msg, ext);
		break;

	case RPC_ID_netif_carrier_off:
		glue_user_trace("netif_carrier_off\n");
		netif_carrier_off_callee(msg, ext);
		break;

	case RPC_ID_napi_gro_receive:
		glue_user_trace("napi_gro_receive\n");
		napi_gro_receive_callee(msg, ext);
		break;

	case RPC_ID_skb_put:
		glue_user_trace("skb_put\n");
		skb_put_callee(msg, ext);
		break;

	case RPC_ID___netdev_alloc_skb:
		glue_user_trace("__netdev_alloc_skb\n");
		__netdev_alloc_skb_callee(msg, ext);
		break;

	case RPC_ID_pci_select_bars:
		glue_user_trace("pci_select_bars\n");
		pci_select_bars_callee(msg, ext);
		break;

	case RPC_ID_pcie_set_readrq:
		glue_user_trace("pcie_set_readrq\n");
		pcie_set_readrq_callee(msg, ext);
		break;

	case RPC_ID_pcie_get_readrq:
		glue_user_trace("pcie_get_readrq\n");
		pcie_get_readrq_callee(msg, ext);
		break;

	case RPC_ID_pci_set_master:
		glue_user_trace("pci_set_master\n");
		pci_set_master_callee(msg, ext);
		break;

	case RPC_ID_pci_enable_pcie_error_reporting:
		glue_user_trace("pci_enable_pcie_error_reporting\n");
		pci_enable_pcie_error_reporting_callee(msg, ext);
		break;

	case RPC_ID_pci_disable_pcie_error_reporting:
		glue_user_trace("pci_disable_pcie_error_reporting\n");
		pci_disable_pcie_error_reporting_callee(msg, ext);
		break;

	case RPC_ID_pci_request_selected_regions:
		glue_user_trace("pci_request_selected_regions\n");
		pci_request_selected_regions_callee(msg, ext);
		break;

	case RPC_ID_pci_release_selected_regions:
		glue_user_trace("pci_release_selected_regions\n");
		pci_release_selected_regions_callee(msg, ext);
		break;

	case RPC_ID_pci_enable_device:
		glue_user_trace("pci_enable_device\n");
		pci_enable_device_callee(msg, ext);
		break;

	case RPC_ID_pci_enable_device_mem:
		glue_user_trace("pci_enable_device_mem\n");
		pci_enable_device_mem_callee(msg, ext);
		break;

	case RPC_ID_pci_ioremap_bar:
		glue_user_trace("pci_ioremap_bar\n");
		pci_ioremap_bar_callee(msg, ext);
		break;

	case RPC_ID_get_loops_per_jiffy:
		glue_user_trace("get_loops_per_jiffy\n");
		get_loops_per_jiffy_callee(msg, ext);
		break;

	case RPC_ID_get_jiffies:
		glue_user_trace("get_jiffies\n");
		get_jiffies_callee(msg, ext);
		break;

	case RPC_ID_eth_validate_addr:
		glue_user_trace("eth_validate_addr\n");
		eth_validate_addr_callee(msg, ext);
		break;

	case RPC_ID_ethtool_op_get_link:
		glue_user_trace("ethtool_op_get_link\n");
		ethtool_op_get_link_callee(msg, ext);
		break;

	case RPC_ID_lvd_netif_tx_start_all_queues:
		glue_user_trace("lvd_netif_tx_start_all_queues\n");
		lvd_netif_tx_start_all_queues_callee(msg, ext);
		break;

	case RPC_ID_lvd_init_work:
		glue_user_trace("lvd_init_work\n");
		lvd_init_work_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

