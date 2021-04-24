#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

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
		const void* __adjusted = *pdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__probe__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	{
		const void* __adjusted = *ent_ptr;
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

void trmp_impl_shutdown(fptr_shutdown target, struct pci_dev* pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused const struct shutdown_call_ctx call_ctx = {pdev};
	__maybe_unused const struct shutdown_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *pdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__shutdown__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_shutdown);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__shutdown__pdev__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_shutdown)
void LCD_TRAMPOLINE_LINKAGE(trmp_shutdown) trmp_shutdown(struct pci_dev* pdev)
{
	volatile fptr_impl_shutdown impl;
	fptr_shutdown target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_shutdown);
	impl = trmp_impl_shutdown;
	return impl(target, pdev);
}

void trmp_impl_ndo_set_rx_mode(fptr_ndo_set_rx_mode target, struct net_device* netdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	
	__maybe_unused const struct ndo_set_rx_mode_call_ctx call_ctx = {netdev};
	__maybe_unused const struct ndo_set_rx_mode_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ndo_set_rx_mode__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_ndo_set_rx_mode);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ndo_set_rx_mode__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_ndo_set_rx_mode)
void LCD_TRAMPOLINE_LINKAGE(trmp_ndo_set_rx_mode) trmp_ndo_set_rx_mode(struct net_device* netdev)
{
	volatile fptr_impl_ndo_set_rx_mode impl;
	fptr_ndo_set_rx_mode target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_set_rx_mode);
	impl = trmp_impl_ndo_set_rx_mode;
	return impl(target, netdev);
}

int trmp_impl_ndo_stop(fptr_ndo_stop target, struct net_device* netdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ndo_stop_call_ctx call_ctx = {netdev};
	__maybe_unused const struct ndo_stop_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ndo_stop__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_ndo_stop);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ndo_stop__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ndo_stop)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_stop) trmp_ndo_stop(struct net_device* netdev)
{
	volatile fptr_impl_ndo_stop impl;
	fptr_ndo_stop target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_stop);
	impl = trmp_impl_ndo_stop;
	return impl(target, netdev);
}

int trmp_impl_ndo_open(fptr_ndo_open target, struct net_device* netdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ndo_open_call_ctx call_ctx = {netdev};
	__maybe_unused const struct ndo_open_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ndo_open__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_ndo_open);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ndo_open__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ndo_open)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_open) trmp_ndo_open(struct net_device* netdev)
{
	volatile fptr_impl_ndo_open impl;
	fptr_ndo_open target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_open);
	impl = trmp_impl_ndo_open;
	return impl(target, netdev);
}

int trmp_impl_ndo_start_xmit(fptr_ndo_start_xmit target, struct sk_buff* skb, struct net_device* netdev)
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
	
	__maybe_unused const struct ndo_start_xmit_call_ctx call_ctx = {skb, netdev};
	__maybe_unused const struct ndo_start_xmit_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__ndo_start_xmit__skb__io(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		const void* __adjusted = *netdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ndo_start_xmit__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_ndo_start_xmit);

	*__pos = 0;
	{
		*skb_ptr = glue_unpack_shadow(__pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			caller_unmarshal_kernel__ndo_start_xmit__skb__io(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ndo_start_xmit__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ndo_start_xmit)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_start_xmit) trmp_ndo_start_xmit(struct sk_buff* skb, struct net_device* netdev)
{
	volatile fptr_impl_ndo_start_xmit impl;
	fptr_ndo_start_xmit target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_start_xmit);
	impl = trmp_impl_ndo_start_xmit;
	return impl(target, skb, netdev);
}

unsigned long long trmp_impl_ndo_features_check(fptr_ndo_features_check target, struct sk_buff* skb, struct net_device* dev, unsigned long long features)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	struct net_device** dev_ptr = &dev;
	unsigned long long* features_ptr = &features;
	unsigned long long ret = 0;
	unsigned long long* ret_ptr = &ret;
	
	__maybe_unused const struct ndo_features_check_call_ctx call_ctx = {skb, dev, features};
	__maybe_unused const struct ndo_features_check_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__ndo_features_check__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_features_check__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *features_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_ndo_features_check);

	*__pos = 0;
	{
		if (*skb_ptr) {
			caller_unmarshal_kernel__ndo_features_check__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_features_check__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ndo_features_check)
unsigned long long LCD_TRAMPOLINE_LINKAGE(trmp_ndo_features_check) trmp_ndo_features_check(struct sk_buff* skb, struct net_device* dev, unsigned long long features)
{
	volatile fptr_impl_ndo_features_check impl;
	fptr_ndo_features_check target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_features_check);
	impl = trmp_impl_ndo_features_check;
	return impl(target, skb, dev, features);
}

int trmp_impl_ndo_set_mac_address(fptr_ndo_set_mac_address target, struct net_device* netdev, void* p)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	void** p_ptr = &p;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ndo_set_mac_address_call_ctx call_ctx = {netdev, p};
	__maybe_unused const struct ndo_set_mac_address_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ndo_set_mac_address__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		const void* __adjusted = *p_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*p_ptr) {
		}

	}

	glue_call_client(__pos, msg, RPC_ID_ndo_set_mac_address);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ndo_set_mac_address__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		(void)p_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_ndo_set_mac_address)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_set_mac_address) trmp_ndo_set_mac_address(struct net_device* netdev, void* p)
{
	volatile fptr_impl_ndo_set_mac_address impl;
	fptr_ndo_set_mac_address target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_set_mac_address);
	impl = trmp_impl_ndo_set_mac_address;
	return impl(target, netdev, p);
}

int trmp_impl_ndo_do_ioctl(fptr_ndo_do_ioctl target, struct net_device* netdev, struct ifreq* req, int cmd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	struct ifreq** req_ptr = &req;
	int* cmd_ptr = &cmd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ndo_do_ioctl_call_ctx call_ctx = {netdev, req, cmd};
	__maybe_unused const struct ndo_do_ioctl_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ndo_do_ioctl__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		const void* __adjusted = *req_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*req_ptr) {
			caller_marshal_kernel__ndo_do_ioctl__req__in(__pos, msg, ext, ctx, *req_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *cmd_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_ndo_do_ioctl);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ndo_do_ioctl__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*req_ptr) {
			caller_unmarshal_kernel__ndo_do_ioctl__req__in(__pos, msg, ext, ctx, *req_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ndo_do_ioctl)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_do_ioctl) trmp_ndo_do_ioctl(struct net_device* netdev, struct ifreq* req, int cmd)
{
	volatile fptr_impl_ndo_do_ioctl impl;
	fptr_ndo_do_ioctl target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_do_ioctl);
	impl = trmp_impl_ndo_do_ioctl;
	return impl(target, netdev, req, cmd);
}

int trmp_impl_ndo_change_mtu(fptr_ndo_change_mtu target, struct net_device* netdev, int new_mtu)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	int* new_mtu_ptr = &new_mtu;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ndo_change_mtu_call_ctx call_ctx = {netdev, new_mtu};
	__maybe_unused const struct ndo_change_mtu_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *netdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ndo_change_mtu__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *new_mtu_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_ndo_change_mtu);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ndo_change_mtu__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ndo_change_mtu)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_change_mtu) trmp_ndo_change_mtu(struct net_device* netdev, int new_mtu)
{
	volatile fptr_impl_ndo_change_mtu impl;
	fptr_ndo_change_mtu target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_change_mtu);
	impl = trmp_impl_ndo_change_mtu;
	return impl(target, netdev, new_mtu);
}

void trmp_impl_ndo_tx_timeout(fptr_ndo_tx_timeout target, struct net_device* netdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	
	__maybe_unused const struct ndo_tx_timeout_call_ctx call_ctx = {netdev};
	__maybe_unused const struct ndo_tx_timeout_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *netdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ndo_tx_timeout__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_ndo_tx_timeout);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ndo_tx_timeout__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_ndo_tx_timeout)
void LCD_TRAMPOLINE_LINKAGE(trmp_ndo_tx_timeout) trmp_ndo_tx_timeout(struct net_device* netdev)
{
	volatile fptr_impl_ndo_tx_timeout impl;
	fptr_ndo_tx_timeout target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_tx_timeout);
	impl = trmp_impl_ndo_tx_timeout;
	return impl(target, netdev);
}

struct rtnl_link_stats64* trmp_impl_ndo_get_stats64(fptr_ndo_get_stats64 target, struct net_device* netdev, struct rtnl_link_stats64* stats)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	struct rtnl_link_stats64** stats_ptr = &stats;
	struct rtnl_link_stats64* ret = 0;
	struct rtnl_link_stats64** ret_ptr = &ret;
	
	__maybe_unused const struct ndo_get_stats64_call_ctx call_ctx = {netdev, stats};
	__maybe_unused const struct ndo_get_stats64_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *netdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ndo_get_stats64__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		const void* __adjusted = *stats_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*stats_ptr) {
			caller_marshal_kernel__ndo_get_stats64__stats__in(__pos, msg, ext, ctx, *stats_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_ndo_get_stats64);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ndo_get_stats64__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*stats_ptr) {
			caller_unmarshal_kernel__ndo_get_stats64__stats__in(__pos, msg, ext, ctx, *stats_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, struct rtnl_link_stats64*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_ndo_get_stats64)
struct rtnl_link_stats64* LCD_TRAMPOLINE_LINKAGE(trmp_ndo_get_stats64) trmp_ndo_get_stats64(struct net_device* netdev, struct rtnl_link_stats64* stats)
{
	volatile fptr_impl_ndo_get_stats64 impl;
	fptr_ndo_get_stats64 target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_get_stats64);
	impl = trmp_impl_ndo_get_stats64;
	return impl(target, netdev, stats);
}

int trmp_impl_ndo_setup_tc(fptr_ndo_setup_tc target, struct net_device* dev, unsigned int handle, unsigned short proto, struct tc_to_netdev* tc)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	unsigned int* handle_ptr = &handle;
	unsigned short* proto_ptr = &proto;
	struct tc_to_netdev** tc_ptr = &tc;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ndo_setup_tc_call_ctx call_ctx = {dev, handle, proto, tc};
	__maybe_unused const struct ndo_setup_tc_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_setup_tc__dev__io(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *handle_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *proto_ptr);
	}

	{
		const void* __adjusted = *tc_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*tc_ptr) {
			caller_marshal_kernel__ndo_setup_tc__tc__in(__pos, msg, ext, ctx, *tc_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_ndo_setup_tc);

	*__pos = 0;
	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_setup_tc__dev__io(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
	}

	{
		if (*tc_ptr) {
			caller_unmarshal_kernel__ndo_setup_tc__tc__in(__pos, msg, ext, ctx, *tc_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ndo_setup_tc)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_setup_tc) trmp_ndo_setup_tc(struct net_device* dev, unsigned int handle, unsigned short proto, struct tc_to_netdev* tc)
{
	volatile fptr_impl_ndo_setup_tc impl;
	fptr_ndo_setup_tc target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_setup_tc);
	impl = trmp_impl_ndo_setup_tc;
	return impl(target, dev, handle, proto, tc);
}

unsigned long long trmp_impl_ndo_fix_features(fptr_ndo_fix_features target, struct net_device* netdev, unsigned long long features)
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
	
	__maybe_unused const struct ndo_fix_features_call_ctx call_ctx = {netdev, features};
	__maybe_unused const struct ndo_fix_features_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *netdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ndo_fix_features__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *features_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_ndo_fix_features);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ndo_fix_features__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ndo_fix_features)
unsigned long long LCD_TRAMPOLINE_LINKAGE(trmp_ndo_fix_features) trmp_ndo_fix_features(struct net_device* netdev, unsigned long long features)
{
	volatile fptr_impl_ndo_fix_features impl;
	fptr_ndo_fix_features target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_fix_features);
	impl = trmp_impl_ndo_fix_features;
	return impl(target, netdev, features);
}

int trmp_impl_ndo_set_features(fptr_ndo_set_features target, struct net_device* netdev, unsigned long long features)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	unsigned long long* features_ptr = &features;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ndo_set_features_call_ctx call_ctx = {netdev, features};
	__maybe_unused const struct ndo_set_features_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *netdev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ndo_set_features__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *features_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_ndo_set_features);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ndo_set_features__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ndo_set_features)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_set_features) trmp_ndo_set_features(struct net_device* netdev, unsigned long long features)
{
	volatile fptr_impl_ndo_set_features impl;
	fptr_ndo_set_features target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_set_features);
	impl = trmp_impl_ndo_set_features;
	return impl(target, netdev, features);
}

int trmp_impl_ndo_set_tx_maxrate(fptr_ndo_set_tx_maxrate target, struct net_device* netdev, int queue_index, unsigned int maxrate)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** netdev_ptr = &netdev;
	int* queue_index_ptr = &queue_index;
	unsigned int* maxrate_ptr = &maxrate;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ndo_set_tx_maxrate_call_ctx call_ctx = {netdev, queue_index, maxrate};
	__maybe_unused const struct ndo_set_tx_maxrate_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *netdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ptr) {
			caller_marshal_kernel__ndo_set_tx_maxrate__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *queue_index_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *maxrate_ptr);
	}

	glue_call_client(__pos, msg, RPC_ID_ndo_set_tx_maxrate);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			caller_unmarshal_kernel__ndo_set_tx_maxrate__netdev__in(__pos, msg, ext, ctx, *netdev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ndo_set_tx_maxrate)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_set_tx_maxrate) trmp_ndo_set_tx_maxrate(struct net_device* netdev, int queue_index, unsigned int maxrate)
{
	volatile fptr_impl_ndo_set_tx_maxrate impl;
	fptr_ndo_set_tx_maxrate target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_set_tx_maxrate);
	impl = trmp_impl_ndo_set_tx_maxrate;
	return impl(target, netdev, queue_index, maxrate);
}

void del_timer_sync_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list* timer = 0;
	struct timer_list** timer_ptr = &timer;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct del_timer_sync_call_ctx call_ctx = {timer};
	__maybe_unused struct del_timer_sync_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*timer_ptr = glue_unpack_shadow(__pos, msg, ext, struct timer_list*);
		if (*timer_ptr) {
			callee_unmarshal_kernel__del_timer_sync__timer__in(__pos, msg, ext, ctx, *timer_ptr);
		}

	}

	ret = del_timer_sync(timer);

	*__pos = 0;
	{
		if (*timer_ptr) {
			callee_marshal_kernel__del_timer_sync__timer__in(__pos, msg, ext, ctx, *timer_ptr);
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

void pci_disable_msix_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused struct pci_disable_msix_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_disable_msix_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_disable_msix__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	pci_disable_msix(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_disable_msix__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void pci_bus_read_config_word_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_bus* bus = 0;
	unsigned int devfn = 0;
	int pos = 0;
	unsigned short* value = 0;
	struct pci_bus** bus_ptr = &bus;
	unsigned int* devfn_ptr = &devfn;
	int* pos_ptr = &pos;
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
		*value_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned short*, (sizeof(unsigned short)), (DEFAULT_GFP_FLAGS));
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

void pci_dev_put_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused struct pci_dev_put_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_dev_put_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_dev_put__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	pci_dev_put(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_dev_put__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void pcie_get_minimum_link_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	unsigned int* speed = 0;
	unsigned int* width = 0;
	struct pci_dev** dev_ptr = &dev;
	unsigned int** speed_ptr = &speed;
	unsigned int** width_ptr = &width;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pcie_get_minimum_link_call_ctx call_ctx = {dev, speed, width};
	__maybe_unused struct pcie_get_minimum_link_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pcie_get_minimum_link__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*speed_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned int*, (sizeof(unsigned int)), (DEFAULT_GFP_FLAGS));
		if (*speed_ptr) {
			**speed_ptr = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

	{
		*width_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned int*, (sizeof(unsigned int)), (DEFAULT_GFP_FLAGS));
		if (*width_ptr) {
			**width_ptr = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

	ret = pcie_get_minimum_link(dev, speed, width);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pcie_get_minimum_link__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*speed_ptr) {
			glue_pack(__pos, msg, ext, **speed_ptr);
		}

	}

	{
		if (*width_ptr) {
			glue_pack(__pos, msg, ext, **width_ptr);
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

void pcie_capability_read_word_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	int pos = 0;
	unsigned short* val = 0;
	struct pci_dev** dev_ptr = &dev;
	int* pos_ptr = &pos;
	unsigned short** val_ptr = &val;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pcie_capability_read_word_call_ctx call_ctx = {dev, pos, val};
	__maybe_unused struct pcie_capability_read_word_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pcie_capability_read_word__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*pos_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		(void)val_ptr;
	}

	ret = pcie_capability_read_word(dev, pos, val);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pcie_capability_read_word__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
		const void* __adjusted = *val_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*val_ptr) {
			glue_pack(__pos, msg, ext, **val_ptr);
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

void pci_restore_state_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused struct pci_restore_state_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_restore_state_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_restore_state__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	pci_restore_state(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_restore_state__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_set_power_state_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	int state = 0;
	struct pci_dev** dev_ptr = &dev;
	int* state_ptr = &state;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_set_power_state_call_ctx call_ctx = {dev, state};
	__maybe_unused struct pci_set_power_state_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_set_power_state__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*state_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = pci_set_power_state(dev, state);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_set_power_state__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void pci_save_state_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_save_state_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_save_state_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_save_state__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = pci_save_state(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_save_state__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void mod_timer_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list* timer = 0;
	unsigned long expires = 0;
	struct timer_list** timer_ptr = &timer;
	unsigned long* expires_ptr = &expires;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mod_timer_call_ctx call_ctx = {timer, expires};
	__maybe_unused struct mod_timer_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*timer_ptr = glue_unpack_shadow(__pos, msg, ext, struct timer_list*);
		if (*timer_ptr) {
			callee_unmarshal_kernel__mod_timer__timer__in(__pos, msg, ext, ctx, *timer_ptr);
		}

	}

	{
		*expires_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	ret = mod_timer(timer, expires);

	*__pos = 0;
	{
		if (*timer_ptr) {
			callee_marshal_kernel__mod_timer__timer__in(__pos, msg, ext, ctx, *timer_ptr);
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

void pci_wake_from_d3_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	bool enable = 0;
	struct pci_dev** dev_ptr = &dev;
	bool* enable_ptr = &enable;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_wake_from_d3_call_ctx call_ctx = {dev, enable};
	__maybe_unused struct pci_wake_from_d3_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_wake_from_d3__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*enable_ptr = glue_unpack(__pos, msg, ext, bool);
	}

	ret = pci_wake_from_d3(dev, enable);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_wake_from_d3__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void ioremap_nocache_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long long phys_addr = 0;
	unsigned long size = 0;
	unsigned long long* phys_addr_ptr = &phys_addr;
	unsigned long* size_ptr = &size;
	cptr_t resource_cptr;
	__maybe_unused unsigned int resource_len;
	cptr_t lcd_resource_cptr;
	void* ret = 0;
	void** ret_ptr = &ret;
	
	__maybe_unused struct ioremap_nocache_call_ctx call_ctx = {phys_addr, size};
	__maybe_unused struct ioremap_nocache_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		lcd_resource_cptr.cptr = glue_unpack(__pos, msg, ext, uint64_t);
	}

	{
		*phys_addr_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*size_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	ret = ioremap_nocache(phys_addr, size);

	*__pos = 0;

	{
		lcd_volunteer_dev_mem(__gpa((uint64_t)*ret_ptr), get_order(size), &resource_cptr);
		copy_msg_cap_vmfunc(current->lcd, current->vmfunc_lcd, resource_cptr, lcd_resource_cptr);
		glue_pack(__pos, msg, ext, size);
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

void msleep_interruptible_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int msecs = 0;
	unsigned int* msecs_ptr = &msecs;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused struct msleep_interruptible_call_ctx call_ctx = {msecs};
	__maybe_unused struct msleep_interruptible_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*msecs_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = msleep_interruptible(msecs);

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

void netdev_rss_key_fill_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	void* buffer = 0;
	unsigned long len = 0;
	void** buffer_ptr = &buffer;
	unsigned long* len_ptr = &len;
	
	__maybe_unused struct netdev_rss_key_fill_call_ctx call_ctx = {buffer, len};
	__maybe_unused struct netdev_rss_key_fill_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		(void)buffer_ptr;
	}

	{
		*len_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	netdev_rss_key_fill(buffer, len);

	*__pos = 0;
	{
		const void* __adjusted = *buffer_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*buffer_ptr) {
		}

	}

	{
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

void skb_copy_bits_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff const* skb = 0;
	int offset = 0;
	void* to = 0;
	int len = 0;
	struct sk_buff const** skb_ptr = &skb;
	int* offset_ptr = &offset;
	void** to_ptr = &to;
	int* len_ptr = &len;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct skb_copy_bits_call_ctx call_ctx = {skb, offset, to, len};
	__maybe_unused struct skb_copy_bits_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack_shadow(__pos, msg, ext, struct sk_buff const*);
		if (*skb_ptr) {
			struct sk_buff* writable = (struct sk_buff*)*skb_ptr;
			callee_unmarshal_kernel__skb_copy_bits__skb__io(__pos, msg, ext, ctx, writable);
		}

	}

	{
		*offset_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*to_ptr = glue_unpack_shadow(__pos, msg, ext, void*);
		if (*to_ptr) {
		}

	}

	{
		*len_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = skb_copy_bits(skb, offset, to, len);

	*__pos = 0;
	{
		const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			callee_marshal_kernel__skb_copy_bits__skb__io(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
	}

	{
		const void* __adjusted = *to_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*to_ptr) {
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

void ___pskb_trim_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* skb = 0;
	unsigned int len = 0;
	struct sk_buff** skb_ptr = &skb;
	unsigned int* len_ptr = &len;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ___pskb_trim_call_ctx call_ctx = {skb, len};
	__maybe_unused struct ___pskb_trim_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack_shadow(__pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel_____pskb_trim__skb__io(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = ___pskb_trim(skb, len);

	*__pos = 0;
	{
		const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			callee_marshal_kernel_____pskb_trim__skb__io(__pos, msg, ext, ctx, *skb_ptr);
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

void pskb_expand_head_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* skb = 0;
	int nhead = 0;
	int ntail = 0;
	unsigned int gfp_mask = 0;
	struct sk_buff** skb_ptr = &skb;
	int* nhead_ptr = &nhead;
	int* ntail_ptr = &ntail;
	unsigned int* gfp_mask_ptr = &gfp_mask;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pskb_expand_head_call_ctx call_ctx = {skb, nhead, ntail, gfp_mask};
	__maybe_unused struct pskb_expand_head_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack_shadow(__pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__pskb_expand_head__skb__io(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		*nhead_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*ntail_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*gfp_mask_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = pskb_expand_head(skb, nhead, ntail, gfp_mask);

	*__pos = 0;
	{
		const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			callee_marshal_kernel__pskb_expand_head__skb__io(__pos, msg, ext, ctx, *skb_ptr);
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

void __pskb_pull_tail_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* skb = 0;
	int delta = 0;
	struct sk_buff** skb_ptr = &skb;
	int* delta_ptr = &delta;
	unsigned char* ret = 0;
	unsigned char** ret_ptr = &ret;
	
	__maybe_unused struct __pskb_pull_tail_call_ctx call_ctx = {skb, delta};
	__maybe_unused struct __pskb_pull_tail_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack_shadow(__pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel____pskb_pull_tail__skb__io(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		*delta_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = __pskb_pull_tail(skb, delta);

	*__pos = 0;
	{
		const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			callee_marshal_kernel____pskb_pull_tail__skb__io(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
	}

	{
		const void* __adjusted = *ret_ptr;
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
		(void)ret_ptr;
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void skb_tstamp_tx_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* orig_skb = 0;
	struct skb_shared_hwtstamps* hwtstamps = 0;
	struct sk_buff** orig_skb_ptr = &orig_skb;
	struct skb_shared_hwtstamps** hwtstamps_ptr = &hwtstamps;
	
	__maybe_unused struct skb_tstamp_tx_call_ctx call_ctx = {orig_skb, hwtstamps};
	__maybe_unused struct skb_tstamp_tx_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*orig_skb_ptr = glue_unpack(__pos, msg, ext, struct sk_buff*);
		if (*orig_skb_ptr) {
			callee_unmarshal_kernel__skb_tstamp_tx__orig_skb__io(__pos, msg, ext, ctx, *orig_skb_ptr);
		}

	}

	{
		*hwtstamps_ptr = glue_unpack(__pos, msg, ext, struct skb_shared_hwtstamps*);
		if (*hwtstamps_ptr) {
			callee_unmarshal_kernel__skb_tstamp_tx__hwtstamps__in(__pos, msg, ext, ctx, *hwtstamps_ptr);
		}

	}

	skb_tstamp_tx(orig_skb, hwtstamps);

	*__pos = 0;
	{
		if (*orig_skb_ptr) {
			callee_marshal_kernel__skb_tstamp_tx__orig_skb__io(__pos, msg, ext, ctx, *orig_skb_ptr);
		}

	}

	{
		if (*hwtstamps_ptr) {
			callee_marshal_kernel__skb_tstamp_tx__hwtstamps__in(__pos, msg, ext, ctx, *hwtstamps_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void skb_pad_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* skb = 0;
	int pad = 0;
	struct sk_buff** skb_ptr = &skb;
	int* pad_ptr = &pad;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct skb_pad_call_ctx call_ctx = {skb, pad};
	__maybe_unused struct skb_pad_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack_shadow(__pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__skb_pad__skb__io(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		*pad_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = skb_pad(skb, pad);

	*__pos = 0;
	{
		const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			callee_marshal_kernel__skb_pad__skb__io(__pos, msg, ext, ctx, *skb_ptr);
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

void napi_consume_skb_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* skb = 0;
	int budget = 0;
	struct sk_buff** skb_ptr = &skb;
	int* budget_ptr = &budget;
	
	__maybe_unused struct napi_consume_skb_call_ctx call_ctx = {skb, budget};
	__maybe_unused struct napi_consume_skb_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack(__pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__napi_consume_skb__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		*budget_ptr = glue_unpack(__pos, msg, ext, int);
	}

	napi_consume_skb(skb, budget);

	*__pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel__napi_consume_skb__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
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

void __napi_alloc_skb_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct* napi = 0;
	unsigned int len = 0;
	unsigned int gfp_mask = 0;
	struct napi_struct** napi_ptr = &napi;
	unsigned int* len_ptr = &len;
	unsigned int* gfp_mask_ptr = &gfp_mask;
	struct sk_buff* ret = 0;
	struct sk_buff** ret_ptr = &ret;
	
	__maybe_unused struct __napi_alloc_skb_call_ctx call_ctx = {napi, len, gfp_mask};
	__maybe_unused struct __napi_alloc_skb_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*napi_ptr = glue_unpack(__pos, msg, ext, struct napi_struct*);
		if (*napi_ptr) {
			callee_unmarshal_kernel____napi_alloc_skb__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*gfp_mask_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = __napi_alloc_skb(napi, len, gfp_mask);

	*__pos = 0;
	{
		if (*napi_ptr) {
			callee_marshal_kernel____napi_alloc_skb__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	{
	}

	{
	}

	{
		if (*ret_ptr) {
			callee_marshal_kernel____napi_alloc_skb__ret_sk_buff__in(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void napi_gro_flush_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct* napi = 0;
	bool flush_old = 0;
	struct napi_struct** napi_ptr = &napi;
	bool* flush_old_ptr = &flush_old;
	
	__maybe_unused struct napi_gro_flush_call_ctx call_ctx = {napi, flush_old};
	__maybe_unused struct napi_gro_flush_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*napi_ptr = glue_unpack_shadow(__pos, msg, ext, struct napi_struct*);
		if (*napi_ptr) {
			callee_unmarshal_kernel__napi_gro_flush__napi__io(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	{
		*flush_old_ptr = glue_unpack(__pos, msg, ext, bool);
	}

	napi_gro_flush(napi, flush_old);

	*__pos = 0;
	{
		const void* __adjusted = *napi_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*napi_ptr) {
			callee_marshal_kernel__napi_gro_flush__napi__io(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	{
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
			callee_unmarshal_kernel__napi_complete_done__n__io(__pos, msg, ext, ctx, *n_ptr);
		}

	}

	{
		*work_done_ptr = glue_unpack(__pos, msg, ext, int);
	}

	napi_complete_done(n, work_done);

	*__pos = 0;
	{
		const void* __adjusted = *n_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*n_ptr) {
			callee_marshal_kernel__napi_complete_done__n__io(__pos, msg, ext, ctx, *n_ptr);
		}

	}

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

void call_netdevice_notifiers_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long val = 0;
	struct net_device* dev = 0;
	unsigned long* val_ptr = &val;
	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct call_netdevice_notifiers_call_ctx call_ctx = {val, dev};
	__maybe_unused struct call_netdevice_notifiers_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*val_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__call_netdevice_notifiers__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = call_netdevice_notifiers(val, dev);

	*__pos = 0;
	{
	}

	{
		if (*dev_ptr) {
			callee_marshal_kernel__call_netdevice_notifiers__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void dev_get_stats_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct rtnl_link_stats64* storage = 0;
	struct net_device** dev_ptr = &dev;
	struct rtnl_link_stats64** storage_ptr = &storage;
	struct rtnl_link_stats64* ret = 0;
	struct rtnl_link_stats64** ret_ptr = &ret;
	
	__maybe_unused struct dev_get_stats_call_ctx call_ctx = {dev, storage};
	__maybe_unused struct dev_get_stats_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__dev_get_stats__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*storage_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct rtnl_link_stats64*, (sizeof(struct rtnl_link_stats64)), (DEFAULT_GFP_FLAGS));
		if (*storage_ptr) {
			callee_unmarshal_kernel__dev_get_stats__storage__in(__pos, msg, ext, ctx, *storage_ptr);
		}

	}

	ret = dev_get_stats(dev, storage);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__dev_get_stats__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*storage_ptr) {
			callee_marshal_kernel__dev_get_stats__storage__in(__pos, msg, ext, ctx, *storage_ptr);
		}

	}

	{
		const void* __adjusted = *ret_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__dev_get_stats__ret_rtnl_link_stats64__out(__pos, msg, ext, ctx, *ret_ptr);
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
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__unregister_netdev__dev__io(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	unregister_netdev(dev);

	*__pos = 0;
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			callee_marshal_kernel__unregister_netdev__dev__io(__pos, msg, ext, ctx, *dev_ptr);
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
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__free_netdev__dev__io(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	free_netdev(dev);

	*__pos = 0;
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			callee_marshal_kernel__free_netdev__dev__io(__pos, msg, ext, ctx, *dev_ptr);
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
		*napi_ptr = glue_unpack(__pos, msg, ext, struct napi_struct*);
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

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void napi_hash_del_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct* napi = 0;
	struct napi_struct** napi_ptr = &napi;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct napi_hash_del_call_ctx call_ctx = {napi};
	__maybe_unused struct napi_hash_del_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*napi_ptr = glue_unpack_shadow(__pos, msg, ext, struct napi_struct*);
		if (*napi_ptr) {
			callee_unmarshal_kernel__napi_hash_del__napi__io(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	ret = napi_hash_del(napi);

	*__pos = 0;
	{
		const void* __adjusted = *napi_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*napi_ptr) {
			callee_marshal_kernel__napi_hash_del__napi__io(__pos, msg, ext, ctx, *napi_ptr);
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

void netif_tx_stop_all_queues_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	__maybe_unused struct netif_tx_stop_all_queues_call_ctx call_ctx = {dev};
	__maybe_unused struct netif_tx_stop_all_queues_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netif_tx_stop_all_queues__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	netif_tx_stop_all_queues(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netif_tx_stop_all_queues__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netif_carrier_on__dev__io(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	netif_carrier_on(dev);

	*__pos = 0;
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			callee_marshal_kernel__netif_carrier_on__dev__io(__pos, msg, ext, ctx, *dev_ptr);
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

void netdev_all_upper_get_next_dev_rcu_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct list_head** iter = 0;
	struct net_device** dev_ptr = &dev;
	struct list_head*** iter_ptr = &iter;
	struct net_device* ret = 0;
	struct net_device** ret_ptr = &ret;
	
	__maybe_unused struct netdev_all_upper_get_next_dev_rcu_call_ctx call_ctx = {dev, iter};
	__maybe_unused struct netdev_all_upper_get_next_dev_rcu_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*iter_ptr = glue_unpack(__pos, msg, ext, struct list_head**);
		if (*iter_ptr) {
			**iter_ptr = glue_unpack(__pos, msg, ext, struct list_head*);
			if (**iter_ptr) {
				callee_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__iter__in(__pos, msg, ext, ctx, **iter_ptr);
			}

		}

	}

	ret = netdev_all_upper_get_next_dev_rcu(dev, iter);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netdev_all_upper_get_next_dev_rcu__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*iter_ptr) {
			if (**iter_ptr) {
				callee_marshal_kernel__netdev_all_upper_get_next_dev_rcu__iter__in(__pos, msg, ext, ctx, **iter_ptr);
			}

		}

	}

	{
		if (*ret_ptr) {
			callee_marshal_kernel__netdev_all_upper_get_next_dev_rcu__ret_net_device__in(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
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

int trmp_impl_poll(fptr_poll target, struct napi_struct* napi, int budget)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** napi_ptr = &napi;
	int* budget_ptr = &budget;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct poll_call_ctx call_ctx = {napi, budget};
	__maybe_unused const struct poll_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *napi_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*napi_ptr) {
			caller_marshal_kernel__poll__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *budget_ptr);
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
int LCD_TRAMPOLINE_LINKAGE(trmp_poll) trmp_poll(struct napi_struct* napi, int budget)
{
	volatile fptr_impl_poll impl;
	fptr_poll target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_poll);
	impl = trmp_impl_poll;
	return impl(target, napi, budget);
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
		*napi_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct napi_struct*, (sizeof(struct napi_struct)), (DEFAULT_GFP_FLAGS));
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

void __napi_schedule_irqoff_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct* n = 0;
	struct napi_struct** n_ptr = &n;
	
	__maybe_unused struct __napi_schedule_irqoff_call_ctx call_ctx = {n};
	__maybe_unused struct __napi_schedule_irqoff_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*n_ptr = glue_unpack_shadow(__pos, msg, ext, struct napi_struct*);
		if (*n_ptr) {
			callee_unmarshal_kernel____napi_schedule_irqoff__n__in(__pos, msg, ext, ctx, *n_ptr);
		}

	}

	__napi_schedule_irqoff(n);

	*__pos = 0;
	{
		if (*n_ptr) {
			callee_marshal_kernel____napi_schedule_irqoff__n__in(__pos, msg, ext, ctx, *n_ptr);
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
		*napi_ptr = glue_unpack(__pos, msg, ext, struct napi_struct*);
		if (*napi_ptr) {
			callee_unmarshal_kernel__napi_gro_receive__napi__in(__pos, msg, ext, ctx, *napi_ptr);
		}

	}

	{
		*skb_ptr = glue_unpack_shadow(__pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__napi_gro_receive__skb__io(__pos, msg, ext, ctx, *skb_ptr);
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
		const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			callee_marshal_kernel__napi_gro_receive__skb__io(__pos, msg, ext, ctx, *skb_ptr);
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

void netif_receive_skb_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* skb = 0;
	struct sk_buff** skb_ptr = &skb;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct netif_receive_skb_call_ctx call_ctx = {skb};
	__maybe_unused struct netif_receive_skb_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack_shadow(__pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__netif_receive_skb__skb__io(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	ret = netif_receive_skb(skb);

	*__pos = 0;
	{
		const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			callee_marshal_kernel__netif_receive_skb__skb__io(__pos, msg, ext, ctx, *skb_ptr);
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

void skb_checksum_help_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* skb = 0;
	struct sk_buff** skb_ptr = &skb;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct skb_checksum_help_call_ctx call_ctx = {skb};
	__maybe_unused struct skb_checksum_help_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack_shadow(__pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__skb_checksum_help__skb__io(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	ret = skb_checksum_help(skb);

	*__pos = 0;
	{
		const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			callee_marshal_kernel__skb_checksum_help__skb__io(__pos, msg, ext, ctx, *skb_ptr);
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

void netif_wake_subqueue_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	unsigned short queue_index = 0;
	struct net_device** dev_ptr = &dev;
	unsigned short* queue_index_ptr = &queue_index;
	
	__maybe_unused struct netif_wake_subqueue_call_ctx call_ctx = {dev, queue_index};
	__maybe_unused struct netif_wake_subqueue_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netif_wake_subqueue__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*queue_index_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	netif_wake_subqueue(dev, queue_index);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netif_wake_subqueue__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_set_real_num_rx_queues_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	unsigned int rxq = 0;
	struct net_device** dev_ptr = &dev;
	unsigned int* rxq_ptr = &rxq;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct netif_set_real_num_rx_queues_call_ctx call_ctx = {dev, rxq};
	__maybe_unused struct netif_set_real_num_rx_queues_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netif_set_real_num_rx_queues__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*rxq_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = netif_set_real_num_rx_queues(dev, rxq);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netif_set_real_num_rx_queues__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void netif_set_real_num_tx_queues_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	unsigned int txq = 0;
	struct net_device** dev_ptr = &dev;
	unsigned int* txq_ptr = &txq;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct netif_set_real_num_tx_queues_call_ctx call_ctx = {dev, txq};
	__maybe_unused struct netif_set_real_num_tx_queues_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netif_set_real_num_tx_queues__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*txq_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = netif_set_real_num_tx_queues(dev, txq);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netif_set_real_num_tx_queues__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void netif_set_xps_queue_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct cpumask const* mask = 0;
	unsigned short index = 0;
	struct net_device** dev_ptr = &dev;
	struct cpumask const** mask_ptr = &mask;
	unsigned short* index_ptr = &index;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct netif_set_xps_queue_call_ctx call_ctx = {dev, mask, index};
	__maybe_unused struct netif_set_xps_queue_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netif_set_xps_queue__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*mask_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct cpumask const*, (sizeof(struct cpumask)), (DEFAULT_GFP_FLAGS));
		if (*mask_ptr) {
			struct cpumask* writable = (struct cpumask*)*mask_ptr;
			callee_unmarshal_kernel__netif_set_xps_queue__mask__in(__pos, msg, ext, ctx, writable);
		}

	}

	{
		*index_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	ret = netif_set_xps_queue(dev, mask, index);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netif_set_xps_queue__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*mask_ptr) {
			callee_marshal_kernel__netif_set_xps_queue__mask__in(__pos, msg, ext, ctx, *mask_ptr);
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

unsigned int trmp_impl_thread_fn(fptr_thread_fn target, int irq, void* id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* irq_ptr = &irq;
	void** id_ptr = &id;
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

	{
		const void* __adjusted = *id_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*id_ptr) {
		}

	}

	glue_call_client(__pos, msg, RPC_ID_thread_fn);

	*__pos = 0;
	{
	}

	{
		(void)id_ptr;
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
	void** id_ptr = &id;
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

	{
		const void* __adjusted = *id_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*id_ptr) {
		}

	}

	glue_call_client(__pos, msg, RPC_ID_handler);

	*__pos = 0;
	{
	}

	{
		(void)id_ptr;
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
	void** dev_id_ptr = &dev_id;
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

	{
		*dev_id_ptr = glue_unpack_new_shadow(__pos, msg, ext, void*, (sizeof(void*)), (DEFAULT_GFP_FLAGS));
		if (*dev_id_ptr) {
		}

	}

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
		(void)dev_id_ptr;
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
		*dev_id_ptr = glue_unpack_shadow(__pos, msg, ext, void*);
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

void irq_set_affinity_hint_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int irq = 0;
	struct cpumask const* m = 0;
	unsigned int* irq_ptr = &irq;
	struct cpumask const** m_ptr = &m;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct irq_set_affinity_hint_call_ctx call_ctx = {irq, m};
	__maybe_unused struct irq_set_affinity_hint_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*m_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct cpumask const*, (sizeof(struct cpumask)), (DEFAULT_GFP_FLAGS));
		if (*m_ptr) {
			struct cpumask* writable = (struct cpumask*)*m_ptr;
			callee_unmarshal_kernel__irq_set_affinity_hint__m__in(__pos, msg, ext, ctx, writable);
		}

	}

	ret = irq_set_affinity_hint(irq, m);

	*__pos = 0;
	{
	}

	{
		if (*m_ptr) {
			callee_marshal_kernel__irq_set_affinity_hint__m__in(__pos, msg, ext, ctx, *m_ptr);
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

void pci_enable_msix_range_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct msix_entry* entries = 0;
	int minvec = 0;
	int maxvec = 0;
	struct pci_dev** dev_ptr = &dev;
	struct msix_entry** entries_ptr = &entries;
	int* minvec_ptr = &minvec;
	int* maxvec_ptr = &maxvec;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_enable_msix_range_call_ctx call_ctx = {dev, entries, minvec, maxvec};
	__maybe_unused struct pci_enable_msix_range_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_enable_msix_range__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*entries_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct msix_entry*, (sizeof(struct msix_entry) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
		if (*entries_ptr) {
			int i;
			struct msix_entry* array = *entries_ptr;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct msix_entry* element = &array[i];
				callee_unmarshal_kernel__pci_enable_msix_range__entries__in(__pos, msg, ext, ctx, element);
			}

		}

	}

	{
		*minvec_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*maxvec_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = pci_enable_msix_range(dev, entries, minvec, maxvec);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_enable_msix_range__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		(void)entries_ptr;
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

void dev_addr_del_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	unsigned char const* addr = 0;
	unsigned char addr_type = 0;
	struct net_device** dev_ptr = &dev;
	unsigned char const** addr_ptr = &addr;
	unsigned char* addr_type_ptr = &addr_type;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct dev_addr_del_call_ctx call_ctx = {dev, addr, addr_type};
	__maybe_unused struct dev_addr_del_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__dev_addr_del__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*addr_ptr = glue_unpack_shadow(__pos, msg, ext, unsigned char const*);
		if (*addr_ptr) {
			unsigned char* writable = (unsigned char*)*addr_ptr;
			int i;
			unsigned char* array = writable;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, unsigned char);
			}

		}

	}

	{
		*addr_type_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	ret = dev_addr_del(dev, addr, addr_type);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__dev_addr_del__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		(void)addr_ptr;
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

void dev_addr_add_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	unsigned char const* addr = 0;
	unsigned char addr_type = 0;
	struct net_device** dev_ptr = &dev;
	unsigned char const** addr_ptr = &addr;
	unsigned char* addr_type_ptr = &addr_type;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct dev_addr_add_call_ctx call_ctx = {dev, addr, addr_type};
	__maybe_unused struct dev_addr_add_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__dev_addr_add__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*addr_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned char const*, (sizeof(unsigned char) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
		if (*addr_ptr) {
			unsigned char* writable = (unsigned char*)*addr_ptr;
			int i;
			unsigned char* array = writable;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, unsigned char);
			}

		}

	}

	{
		*addr_type_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	ret = dev_addr_add(dev, addr, addr_type);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__dev_addr_add__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		(void)addr_ptr;
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

int trmp_impl_sync(fptr_sync target, struct net_device* dev, unsigned char const* addr)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	unsigned char const** addr_ptr = &addr;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct sync_call_ctx call_ctx = {dev, addr};
	__maybe_unused const struct sync_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__sync__net_device__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		const void* __adjusted = *addr_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*addr_ptr) {
			size_t i, len = (ETH_ALEN);
			unsigned char const* array = *addr_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	glue_call_client(__pos, msg, RPC_ID_sync);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__sync__net_device__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		(void)addr_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_sync)
int LCD_TRAMPOLINE_LINKAGE(trmp_sync) trmp_sync(struct net_device* dev, unsigned char const* addr)
{
	volatile fptr_impl_sync impl;
	fptr_sync target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_sync);
	impl = trmp_impl_sync;
	return impl(target, dev, addr);
}

int trmp_impl_unsync(fptr_unsync target, struct net_device* dev, unsigned char const* addr)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	unsigned char const** addr_ptr = &addr;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct unsync_call_ctx call_ctx = {dev, addr};
	__maybe_unused const struct unsync_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__unsync__net_device__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		const void* __adjusted = *addr_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*addr_ptr) {
			size_t i, len = (ETH_ALEN);
			unsigned char const* array = *addr_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	glue_call_client(__pos, msg, RPC_ID_unsync);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__unsync__net_device__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		(void)addr_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_unsync)
int LCD_TRAMPOLINE_LINKAGE(trmp_unsync) trmp_unsync(struct net_device* dev, unsigned char const* addr)
{
	volatile fptr_impl_unsync impl;
	fptr_unsync target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_unsync);
	impl = trmp_impl_unsync;
	return impl(target, dev, addr);
}

void __hw_addr_unsync_dev_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct netdev_hw_addr_list* list = 0;
	struct net_device* dev = 0;
	fptr_unsync unsync = 0;
	struct netdev_hw_addr_list** list_ptr = &list;
	struct net_device** dev_ptr = &dev;
	fptr_unsync* unsync_ptr = &unsync;
	
	__maybe_unused struct __hw_addr_unsync_dev_call_ctx call_ctx = {list, dev, unsync};
	__maybe_unused struct __hw_addr_unsync_dev_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*list_ptr = glue_unpack(__pos, msg, ext, struct netdev_hw_addr_list*);
		if (*list_ptr) {
			callee_unmarshal_kernel____hw_addr_unsync_dev__list__in(__pos, msg, ext, ctx, *list_ptr);
		}

	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel____hw_addr_unsync_dev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*unsync_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, unsync);
	}

	__hw_addr_unsync_dev(list, dev, unsync);

	*__pos = 0;
	{
		if (*list_ptr) {
			callee_marshal_kernel____hw_addr_unsync_dev__list__in(__pos, msg, ext, ctx, *list_ptr);
		}

	}

	{
		if (*dev_ptr) {
			callee_marshal_kernel____hw_addr_unsync_dev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __hw_addr_sync_dev_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct netdev_hw_addr_list* list = 0;
	struct net_device* dev = 0;
	fptr_sync sync = 0;
	fptr_unsync unsync = 0;
	struct netdev_hw_addr_list** list_ptr = &list;
	struct net_device** dev_ptr = &dev;
	fptr_sync* sync_ptr = &sync;
	fptr_unsync* unsync_ptr = &unsync;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct __hw_addr_sync_dev_call_ctx call_ctx = {list, dev, sync, unsync};
	__maybe_unused struct __hw_addr_sync_dev_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct net_device* __net_device = glue_unpack(__pos, msg, ext, struct net_device*);
		*list_ptr = &__net_device->uc;
		if (*list_ptr) {
			callee_unmarshal_kernel____hw_addr_sync_dev__list__in(__pos, msg, ext, ctx, *list_ptr);
		}

	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel____hw_addr_sync_dev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*sync_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, sync);
	}

	{
		*unsync_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, unsync);
	}

	ret = __hw_addr_sync_dev(list, dev, sync, unsync);

	*__pos = 0;
	{
		if (*list_ptr) {
			callee_marshal_kernel____hw_addr_sync_dev__list__in(__pos, msg, ext, ctx, *list_ptr);
		}

	}

	{
		if (*dev_ptr) {
			callee_marshal_kernel____hw_addr_sync_dev__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

void eth_platform_get_mac_address_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct device* dev = 0;
	unsigned char* mac_addr = 0;
	struct device** dev_ptr = &dev;
	unsigned char** mac_addr_ptr = &mac_addr;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct eth_platform_get_mac_address_call_ctx call_ctx = {dev, mac_addr};
	__maybe_unused struct eth_platform_get_mac_address_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct pci_dev* __pci_dev = glue_unpack(__pos, msg, ext, struct pci_dev*);
		*dev_ptr = &__pci_dev->dev;
		if (*dev_ptr) {
			callee_unmarshal_kernel__eth_platform_get_mac_address__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*mac_addr_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned char*, (sizeof(unsigned char) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
		(void)mac_addr_ptr;
	}

	ret = eth_platform_get_mac_address(dev, mac_addr);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__eth_platform_get_mac_address__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		if (*mac_addr_ptr) {
			size_t i, len = (ETH_ALEN);
			unsigned char* array = *mac_addr_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
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
		const void* __adjusted = *ret_ptr;
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

int trmp_impl_ndo_validate_addr(fptr_ndo_validate_addr target, struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ndo_validate_addr_call_ctx call_ctx = {dev};
	__maybe_unused const struct ndo_validate_addr_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_validate_addr__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, msg, RPC_ID_ndo_validate_addr);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_validate_addr__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ndo_validate_addr)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_validate_addr) trmp_ndo_validate_addr(struct net_device* dev)
{
	volatile fptr_impl_ndo_validate_addr impl;
	fptr_ndo_validate_addr target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_validate_addr);
	impl = trmp_impl_ndo_validate_addr;
	return impl(target, dev);
}

void dev_trans_start_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused struct dev_trans_start_call_ctx call_ctx = {dev};
	__maybe_unused struct dev_trans_start_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__dev_trans_start__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = dev_trans_start(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__dev_trans_start__dev__in(__pos, msg, ext, ctx, *dev_ptr);
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
		*mdio_ptr = glue_unpack_shadow(__pos, msg, ext, struct mdio_if_info const*);
		if (*mdio_ptr) {
			struct mdio_if_info* writable = (struct mdio_if_info*)*mdio_ptr;
			callee_unmarshal_kernel__mdio_mii_ioctl__mdio__in(__pos, msg, ext, ctx, writable);
		}

	}

	{
		*mii_data_ptr = glue_unpack_shadow(__pos, msg, ext, struct mii_ioctl_data*);
		if (*mii_data_ptr) {
			callee_unmarshal_kernel__mdio_mii_ioctl__mii_data__io(__pos, msg, ext, ctx, *mii_data_ptr);
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
		const void* __adjusted = *mii_data_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*mii_data_ptr) {
			callee_marshal_kernel__mdio_mii_ioctl__mii_data__io(__pos, msg, ext, ctx, *mii_data_ptr);
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

void mdio45_probe_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mdio_if_info* mdio = 0;
	int prtad = 0;
	struct mdio_if_info** mdio_ptr = &mdio;
	int* prtad_ptr = &prtad;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mdio45_probe_call_ctx call_ctx = {mdio, prtad};
	__maybe_unused struct mdio45_probe_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*mdio_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct mdio_if_info*, (sizeof(struct mdio_if_info)), (DEFAULT_GFP_FLAGS));
		if (*mdio_ptr) {
			callee_unmarshal_kernel__mdio45_probe__mdio__in(__pos, msg, ext, ctx, *mdio_ptr);
		}

	}

	{
		*prtad_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = mdio45_probe(mdio, prtad);

	*__pos = 0;
	{
		if (*mdio_ptr) {
			callee_marshal_kernel__mdio45_probe__mdio__in(__pos, msg, ext, ctx, *mdio_ptr);
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

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext)
{
	switch(id) {
	case RPC_ID_shared_mem_init:
		glue_user_trace("shared_mem_init\n");
		shared_mem_init_callee(msg, ext);
		break;

	case RPC_ID_del_timer_sync:
		glue_user_trace("del_timer_sync\n");
		del_timer_sync_callee(msg, ext);
		break;

	case RPC_ID_pci_disable_msi:
		glue_user_trace("pci_disable_msi\n");
		pci_disable_msi_callee(msg, ext);
		break;

	case RPC_ID_pci_disable_msix:
		glue_user_trace("pci_disable_msix\n");
		pci_disable_msix_callee(msg, ext);
		break;

	case RPC_ID_pci_disable_device:
		glue_user_trace("pci_disable_device\n");
		pci_disable_device_callee(msg, ext);
		break;

	case RPC_ID_pci_bus_read_config_word:
		glue_user_trace("pci_bus_read_config_word\n");
		pci_bus_read_config_word_callee(msg, ext);
		break;

	case RPC_ID_pci_bus_write_config_word:
		glue_user_trace("pci_bus_write_config_word\n");
		pci_bus_write_config_word_callee(msg, ext);
		break;

	case RPC_ID_pci_dev_put:
		glue_user_trace("pci_dev_put\n");
		pci_dev_put_callee(msg, ext);
		break;

	case RPC_ID_queue_work_on:
		glue_user_trace("queue_work_on\n");
		queue_work_on_callee(msg, ext);
		break;

	case RPC_ID_pci_select_bars:
		glue_user_trace("pci_select_bars\n");
		pci_select_bars_callee(msg, ext);
		break;

	case RPC_ID_pcie_get_minimum_link:
		glue_user_trace("pcie_get_minimum_link\n");
		pcie_get_minimum_link_callee(msg, ext);
		break;

	case RPC_ID_pcie_capability_read_word:
		glue_user_trace("pcie_capability_read_word\n");
		pcie_capability_read_word_callee(msg, ext);
		break;

	case RPC_ID_pci_restore_state:
		glue_user_trace("pci_restore_state\n");
		pci_restore_state_callee(msg, ext);
		break;

	case RPC_ID_pci_set_power_state:
		glue_user_trace("pci_set_power_state\n");
		pci_set_power_state_callee(msg, ext);
		break;

	case RPC_ID_pci_save_state:
		glue_user_trace("pci_save_state\n");
		pci_save_state_callee(msg, ext);
		break;

	case RPC_ID_pci_set_master:
		glue_user_trace("pci_set_master\n");
		pci_set_master_callee(msg, ext);
		break;

	case RPC_ID_pci_request_selected_regions:
		glue_user_trace("pci_request_selected_regions\n");
		pci_request_selected_regions_callee(msg, ext);
		break;

	case RPC_ID_pci_release_selected_regions:
		glue_user_trace("pci_release_selected_regions\n");
		pci_release_selected_regions_callee(msg, ext);
		break;

	case RPC_ID_mod_timer:
		glue_user_trace("mod_timer\n");
		mod_timer_callee(msg, ext);
		break;

	case RPC_ID_pci_wake_from_d3:
		glue_user_trace("pci_wake_from_d3\n");
		pci_wake_from_d3_callee(msg, ext);
		break;

	case RPC_ID_pci_enable_device_mem:
		glue_user_trace("pci_enable_device_mem\n");
		pci_enable_device_mem_callee(msg, ext);
		break;

	case RPC_ID_ioremap_nocache:
		glue_user_trace("ioremap_nocache\n");
		ioremap_nocache_callee(msg, ext);
		break;

	case RPC_ID_msleep_interruptible:
		glue_user_trace("msleep_interruptible\n");
		msleep_interruptible_callee(msg, ext);
		break;

	case RPC_ID_netdev_rss_key_fill:
		glue_user_trace("netdev_rss_key_fill\n");
		netdev_rss_key_fill_callee(msg, ext);
		break;

	case RPC_ID_rtnl_is_locked:
		glue_user_trace("rtnl_is_locked\n");
		rtnl_is_locked_callee(msg, ext);
		break;

	case RPC_ID_skb_copy_bits:
		glue_user_trace("skb_copy_bits\n");
		skb_copy_bits_callee(msg, ext);
		break;

	case RPC_ID____pskb_trim:
		glue_user_trace("___pskb_trim\n");
		___pskb_trim_callee(msg, ext);
		break;

	case RPC_ID_pskb_expand_head:
		glue_user_trace("pskb_expand_head\n");
		pskb_expand_head_callee(msg, ext);
		break;

	case RPC_ID_consume_skb:
		glue_user_trace("consume_skb\n");
		consume_skb_callee(msg, ext);
		break;

	case RPC_ID___pskb_pull_tail:
		glue_user_trace("__pskb_pull_tail\n");
		__pskb_pull_tail_callee(msg, ext);
		break;

	case RPC_ID_skb_put:
		glue_user_trace("skb_put\n");
		skb_put_callee(msg, ext);
		break;

	case RPC_ID_skb_tstamp_tx:
		glue_user_trace("skb_tstamp_tx\n");
		skb_tstamp_tx_callee(msg, ext);
		break;

	case RPC_ID_skb_pad:
		glue_user_trace("skb_pad\n");
		skb_pad_callee(msg, ext);
		break;

	case RPC_ID_napi_consume_skb:
		glue_user_trace("napi_consume_skb\n");
		napi_consume_skb_callee(msg, ext);
		break;

	case RPC_ID___dev_kfree_skb_any:
		glue_user_trace("__dev_kfree_skb_any\n");
		__dev_kfree_skb_any_callee(msg, ext);
		break;

	case RPC_ID___napi_alloc_skb:
		glue_user_trace("__napi_alloc_skb\n");
		__napi_alloc_skb_callee(msg, ext);
		break;

	case RPC_ID_napi_gro_flush:
		glue_user_trace("napi_gro_flush\n");
		napi_gro_flush_callee(msg, ext);
		break;

	case RPC_ID_napi_complete_done:
		glue_user_trace("napi_complete_done\n");
		napi_complete_done_callee(msg, ext);
		break;

	case RPC_ID_rtnl_unlock:
		glue_user_trace("rtnl_unlock\n");
		rtnl_unlock_callee(msg, ext);
		break;

	case RPC_ID_rtnl_lock:
		glue_user_trace("rtnl_lock\n");
		rtnl_lock_callee(msg, ext);
		break;

	case RPC_ID_call_netdevice_notifiers:
		glue_user_trace("call_netdevice_notifiers\n");
		call_netdevice_notifiers_callee(msg, ext);
		break;

	case RPC_ID_dev_get_stats:
		glue_user_trace("dev_get_stats\n");
		dev_get_stats_callee(msg, ext);
		break;

	case RPC_ID_unregister_netdev:
		glue_user_trace("unregister_netdev\n");
		unregister_netdev_callee(msg, ext);
		break;

	case RPC_ID_free_netdev:
		glue_user_trace("free_netdev\n");
		free_netdev_callee(msg, ext);
		break;

	case RPC_ID_netif_napi_del:
		glue_user_trace("netif_napi_del\n");
		netif_napi_del_callee(msg, ext);
		break;

	case RPC_ID_napi_hash_del:
		glue_user_trace("napi_hash_del\n");
		napi_hash_del_callee(msg, ext);
		break;

	case RPC_ID_register_netdev:
		glue_user_trace("register_netdev\n");
		register_netdev_callee(msg, ext);
		break;

	case RPC_ID_netif_tx_stop_all_queues:
		glue_user_trace("netif_tx_stop_all_queues\n");
		netif_tx_stop_all_queues_callee(msg, ext);
		break;

	case RPC_ID_netif_carrier_on:
		glue_user_trace("netif_carrier_on\n");
		netif_carrier_on_callee(msg, ext);
		break;

	case RPC_ID_netif_carrier_off:
		glue_user_trace("netif_carrier_off\n");
		netif_carrier_off_callee(msg, ext);
		break;

	case RPC_ID_netdev_all_upper_get_next_dev_rcu:
		glue_user_trace("netdev_all_upper_get_next_dev_rcu\n");
		netdev_all_upper_get_next_dev_rcu_callee(msg, ext);
		break;

	case RPC_ID_napi_disable:
		glue_user_trace("napi_disable\n");
		napi_disable_callee(msg, ext);
		break;

	case RPC_ID_netif_napi_add:
		glue_user_trace("netif_napi_add\n");
		netif_napi_add_callee(msg, ext);
		break;

	case RPC_ID___napi_schedule_irqoff:
		glue_user_trace("__napi_schedule_irqoff\n");
		__napi_schedule_irqoff_callee(msg, ext);
		break;

	case RPC_ID_napi_gro_receive:
		glue_user_trace("napi_gro_receive\n");
		napi_gro_receive_callee(msg, ext);
		break;

	case RPC_ID_netif_receive_skb:
		glue_user_trace("netif_receive_skb\n");
		netif_receive_skb_callee(msg, ext);
		break;

	case RPC_ID_skb_checksum_help:
		glue_user_trace("skb_checksum_help\n");
		skb_checksum_help_callee(msg, ext);
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

	case RPC_ID_netif_wake_subqueue:
		glue_user_trace("netif_wake_subqueue\n");
		netif_wake_subqueue_callee(msg, ext);
		break;

	case RPC_ID_netif_set_real_num_rx_queues:
		glue_user_trace("netif_set_real_num_rx_queues\n");
		netif_set_real_num_rx_queues_callee(msg, ext);
		break;

	case RPC_ID_netif_set_real_num_tx_queues:
		glue_user_trace("netif_set_real_num_tx_queues\n");
		netif_set_real_num_tx_queues_callee(msg, ext);
		break;

	case RPC_ID_netif_set_xps_queue:
		glue_user_trace("netif_set_xps_queue\n");
		netif_set_xps_queue_callee(msg, ext);
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

	case RPC_ID_irq_set_affinity_hint:
		glue_user_trace("irq_set_affinity_hint\n");
		irq_set_affinity_hint_callee(msg, ext);
		break;

	case RPC_ID_pci_enable_msix_range:
		glue_user_trace("pci_enable_msix_range\n");
		pci_enable_msix_range_callee(msg, ext);
		break;

	case RPC_ID_pci_enable_msi_range:
		glue_user_trace("pci_enable_msi_range\n");
		pci_enable_msi_range_callee(msg, ext);
		break;

	case RPC_ID_dev_addr_del:
		glue_user_trace("dev_addr_del\n");
		dev_addr_del_callee(msg, ext);
		break;

	case RPC_ID_dev_addr_add:
		glue_user_trace("dev_addr_add\n");
		dev_addr_add_callee(msg, ext);
		break;

	case RPC_ID___hw_addr_unsync_dev:
		glue_user_trace("__hw_addr_unsync_dev\n");
		__hw_addr_unsync_dev_callee(msg, ext);
		break;

	case RPC_ID___hw_addr_sync_dev:
		glue_user_trace("__hw_addr_sync_dev\n");
		__hw_addr_sync_dev_callee(msg, ext);
		break;

	case RPC_ID_eth_platform_get_mac_address:
		glue_user_trace("eth_platform_get_mac_address\n");
		eth_platform_get_mac_address_callee(msg, ext);
		break;

	case RPC_ID_alloc_etherdev_mqs:
		glue_user_trace("alloc_etherdev_mqs\n");
		alloc_etherdev_mqs_callee(msg, ext);
		break;

	case RPC_ID_dev_trans_start:
		glue_user_trace("dev_trans_start\n");
		dev_trans_start_callee(msg, ext);
		break;

	case RPC_ID_mdio_mii_ioctl:
		glue_user_trace("mdio_mii_ioctl\n");
		mdio_mii_ioctl_callee(msg, ext);
		break;

	case RPC_ID_mdio45_probe:
		glue_user_trace("mdio45_probe\n");
		mdio45_probe_callee(msg, ext);
		break;

	case RPC_ID_pci_unregister_driver:
		glue_user_trace("pci_unregister_driver\n");
		pci_unregister_driver_callee(msg, ext);
		break;

	case RPC_ID___pci_register_driver:
		glue_user_trace("__pci_register_driver\n");
		__pci_register_driver_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

