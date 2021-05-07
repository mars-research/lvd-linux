#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void probe_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_probe function_ptr = glue_unpack(__pos, __msg, __ext, fptr_probe);
	struct pci_dev* pdev = 0;
	struct pci_device_id* ent = 0;
	struct pci_dev** pdev_ptr = &pdev;
	struct pci_device_id** ent_ptr = &ent;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct probe_call_ctx call_ctx = {pdev, ent};
	__maybe_unused struct probe_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct pci_dev);
		*pdev_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct pci_dev*, (__size), (DEFAULT_GFP_FLAGS));
		if (*pdev_ptr) {
			callee_unmarshal_kernel__probe__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		size_t __size = sizeof(struct pci_device_id);
		*ent_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct pci_device_id*, (__size), (DEFAULT_GFP_FLAGS));
		if (*ent_ptr) {
			callee_unmarshal_kernel__probe__ent__in(__pos, __msg, __ext, ctx, *ent_ptr);
		}

	}

	ret = function_ptr(pdev, ent);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__probe__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		if (*ent_ptr) {
			callee_marshal_kernel__probe__ent__in(__pos, __msg, __ext, ctx, *ent_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void remove_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_remove function_ptr = glue_unpack(__pos, __msg, __ext, fptr_remove);
	struct pci_dev* pdev = 0;
	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused struct remove_call_ctx call_ctx = {pdev};
	__maybe_unused struct remove_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__remove__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	function_ptr(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__remove__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void shutdown_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_shutdown function_ptr = glue_unpack(__pos, __msg, __ext, fptr_shutdown);
	struct pci_dev* pdev = 0;
	struct pci_dev** pdev_ptr = &pdev;
	
	__maybe_unused struct shutdown_call_ctx call_ctx = {pdev};
	__maybe_unused struct shutdown_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*pdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct pci_dev*);
		if (*pdev_ptr) {
			callee_unmarshal_kernel__shutdown__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	function_ptr(pdev);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			callee_marshal_kernel__shutdown__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_set_rx_mode_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_set_rx_mode function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_set_rx_mode);
	struct net_device* netdev = 0;
	struct net_device** netdev_ptr = &netdev;
	
	__maybe_unused struct ndo_set_rx_mode_call_ctx call_ctx = {netdev};
	__maybe_unused struct ndo_set_rx_mode_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ndo_set_rx_mode__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	function_ptr(netdev);

	*__pos = 0;
	if (0)
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ndo_set_rx_mode__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_stop_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_stop function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_stop);
	struct net_device* netdev = 0;
	struct net_device** netdev_ptr = &netdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ndo_stop_call_ctx call_ctx = {netdev};
	__maybe_unused struct ndo_stop_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ndo_stop__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	ret = function_ptr(netdev);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ndo_stop__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_open_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_open function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_open);
	struct net_device* netdev = 0;
	struct net_device** netdev_ptr = &netdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ndo_open_call_ctx call_ctx = {netdev};
	__maybe_unused struct ndo_open_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ndo_open__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	ret = function_ptr(netdev);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ndo_open__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_start_xmit_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_start_xmit function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_start_xmit);
	struct sk_buff* skb = 0;
	struct net_device* netdev = 0;
	struct sk_buff** skb_ptr = &skb;
	struct net_device** netdev_ptr = &netdev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ndo_start_xmit_call_ctx call_ctx = {skb, netdev};
	__maybe_unused struct ndo_start_xmit_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__ndo_start_xmit__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		*netdev_ptr = glue_unpack(__pos, __msg, __ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ndo_start_xmit__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	ret = function_ptr(skb, netdev);

	*__pos = 0;
	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			callee_marshal_kernel__ndo_start_xmit__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ndo_start_xmit__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_features_check_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_features_check function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_features_check);
	struct sk_buff* skb = 0;
	struct net_device* dev = 0;
	unsigned long long features = 0;
	struct sk_buff** skb_ptr = &skb;
	struct net_device** dev_ptr = &dev;
	unsigned long long* features_ptr = &features;
	unsigned long long ret = 0;
	unsigned long long* ret_ptr = &ret;
	
	__maybe_unused struct ndo_features_check_call_ctx call_ctx = {skb, dev, features};
	__maybe_unused struct ndo_features_check_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__ndo_features_check__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_features_check__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	ret = function_ptr(skb, dev, features);

	*__pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel__ndo_features_check__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_features_check__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_set_mac_address_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_set_mac_address function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_set_mac_address);
	struct net_device* netdev = 0;
	void* p = 0;
	struct net_device** netdev_ptr = &netdev;
	void** p_ptr = &p;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ndo_set_mac_address_call_ctx call_ctx = {netdev, p};
	__maybe_unused struct ndo_set_mac_address_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ndo_set_mac_address__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
		*p_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*p_ptr) {
		}

	}

	ret = function_ptr(netdev, p);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ndo_set_mac_address__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
		(void)p_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_do_ioctl_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_do_ioctl function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_do_ioctl);
	struct net_device* netdev = 0;
	struct ifreq* req = 0;
	int cmd = 0;
	struct net_device** netdev_ptr = &netdev;
	struct ifreq** req_ptr = &req;
	int* cmd_ptr = &cmd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ndo_do_ioctl_call_ctx call_ctx = {netdev, req, cmd};
	__maybe_unused struct ndo_do_ioctl_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ndo_do_ioctl__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
		size_t __size = sizeof(struct ifreq);
		*req_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct ifreq*, (__size), (DEFAULT_GFP_FLAGS));
		if (*req_ptr) {
			callee_unmarshal_kernel__ndo_do_ioctl__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	{
		*cmd_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = function_ptr(netdev, req, cmd);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ndo_do_ioctl__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*req_ptr) {
			callee_marshal_kernel__ndo_do_ioctl__req__in(__pos, __msg, __ext, ctx, *req_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_change_mtu_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_change_mtu function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_change_mtu);
	struct net_device* netdev = 0;
	int new_mtu = 0;
	struct net_device** netdev_ptr = &netdev;
	int* new_mtu_ptr = &new_mtu;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ndo_change_mtu_call_ctx call_ctx = {netdev, new_mtu};
	__maybe_unused struct ndo_change_mtu_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ndo_change_mtu__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
		*new_mtu_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = function_ptr(netdev, new_mtu);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ndo_change_mtu__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_tx_timeout_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_tx_timeout function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_tx_timeout);
	struct net_device* netdev = 0;
	struct net_device** netdev_ptr = &netdev;
	
	__maybe_unused struct ndo_tx_timeout_call_ctx call_ctx = {netdev};
	__maybe_unused struct ndo_tx_timeout_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ndo_tx_timeout__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	function_ptr(netdev);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ndo_tx_timeout__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_get_stats64_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_get_stats64 function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_get_stats64);
	struct net_device* netdev = 0;
	struct rtnl_link_stats64* stats = 0;
	struct net_device** netdev_ptr = &netdev;
	struct rtnl_link_stats64** stats_ptr = &stats;
	struct rtnl_link_stats64* ret = 0;
	struct rtnl_link_stats64** ret_ptr = &ret;
	
	__maybe_unused struct ndo_get_stats64_call_ctx call_ctx = {netdev, stats};
	__maybe_unused struct ndo_get_stats64_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ndo_get_stats64__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
		size_t __size = sizeof(struct rtnl_link_stats64);
		*stats_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct rtnl_link_stats64*, (__size), (DEFAULT_GFP_FLAGS));
		if (*stats_ptr) {
			callee_unmarshal_kernel__ndo_get_stats64__stats__in(__pos, __msg, __ext, ctx, *stats_ptr);
		}

	}

	ret = function_ptr(netdev, stats);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ndo_get_stats64__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
		if (*stats_ptr) {
			callee_marshal_kernel__ndo_get_stats64__stats__in(__pos, __msg, __ext, ctx, *stats_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_setup_tc_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_setup_tc function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_setup_tc);
	struct net_device* dev = 0;
	unsigned int handle = 0;
	unsigned short proto = 0;
	struct tc_to_netdev* tc = 0;
	struct net_device** dev_ptr = &dev;
	unsigned int* handle_ptr = &handle;
	unsigned short* proto_ptr = &proto;
	struct tc_to_netdev** tc_ptr = &tc;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ndo_setup_tc_call_ctx call_ctx = {dev, handle, proto, tc};
	__maybe_unused struct ndo_setup_tc_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_setup_tc__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*handle_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*proto_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*tc_ptr = glue_unpack(__pos, __msg, __ext, struct tc_to_netdev*);
		if (*tc_ptr) {
			callee_unmarshal_kernel__ndo_setup_tc__tc__in(__pos, __msg, __ext, ctx, *tc_ptr);
		}

	}

	ret = function_ptr(dev, handle, proto, tc);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_setup_tc__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
	}

	{
		if (*tc_ptr) {
			callee_marshal_kernel__ndo_setup_tc__tc__in(__pos, __msg, __ext, ctx, *tc_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_fix_features_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_fix_features function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_fix_features);
	struct net_device* netdev = 0;
	unsigned long long features = 0;
	struct net_device** netdev_ptr = &netdev;
	unsigned long long* features_ptr = &features;
	unsigned long long ret = 0;
	unsigned long long* ret_ptr = &ret;
	
	__maybe_unused struct ndo_fix_features_call_ctx call_ctx = {netdev, features};
	__maybe_unused struct ndo_fix_features_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ndo_fix_features__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
		*features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	ret = function_ptr(netdev, features);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ndo_fix_features__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_set_features_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_set_features function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_set_features);
	struct net_device* netdev = 0;
	unsigned long long features = 0;
	struct net_device** netdev_ptr = &netdev;
	unsigned long long* features_ptr = &features;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ndo_set_features_call_ctx call_ctx = {netdev, features};
	__maybe_unused struct ndo_set_features_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ndo_set_features__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
		*features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	ret = function_ptr(netdev, features);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ndo_set_features__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_set_tx_maxrate_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_set_tx_maxrate function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_set_tx_maxrate);
	struct net_device* netdev = 0;
	int queue_index = 0;
	unsigned int maxrate = 0;
	struct net_device** netdev_ptr = &netdev;
	int* queue_index_ptr = &queue_index;
	unsigned int* maxrate_ptr = &maxrate;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ndo_set_tx_maxrate_call_ctx call_ctx = {netdev, queue_index, maxrate};
	__maybe_unused struct ndo_set_tx_maxrate_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*netdev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*netdev_ptr) {
			callee_unmarshal_kernel__ndo_set_tx_maxrate__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
		*queue_index_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*maxrate_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	ret = function_ptr(netdev, queue_index, maxrate);

	*__pos = 0;
	{
		if (*netdev_ptr) {
			callee_marshal_kernel__ndo_set_tx_maxrate__netdev__in(__pos, __msg, __ext, ctx, *netdev_ptr);
		}

	}

	{
	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int del_timer_sync(struct timer_list* timer)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list** timer_ptr = &timer;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct del_timer_sync_call_ctx call_ctx = {timer};
	__maybe_unused const struct del_timer_sync_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *timer_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*timer_ptr) {
			caller_marshal_kernel__del_timer_sync__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_del_timer_sync);

	*__pos = 0;
	{
		if (*timer_ptr) {
			caller_unmarshal_kernel__del_timer_sync__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void timer_func_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_timer_func function_ptr = glue_unpack(__pos, __msg, __ext, fptr_timer_func);
	unsigned long data = 0;
	unsigned long* data_ptr = &data;
	
	__maybe_unused struct timer_func_call_ctx call_ctx = {data};
	__maybe_unused struct timer_func_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		//printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	printk("%s, calling %p with data %lx\n", __func__, function_ptr, data);
	function_ptr(data);

	*__pos = 0;
	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		//printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_setup_timer(struct timer_list* timer, fptr_timer_func func, unsigned long data)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list** timer_ptr = &timer;
	fptr_timer_func* func_ptr = &func;
	unsigned long* data_ptr = &data;
	
	__maybe_unused const struct lvd_setup_timer_call_ctx call_ctx = {timer, func, data};
	__maybe_unused const struct lvd_setup_timer_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *timer_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*timer_ptr) {
			caller_marshal_kernel__lvd_setup_timer__timer__io(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *func_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_ptr);
	}

	printk("%s, timer %p | func %p | data %lx\n", __func__, timer,
				func, data);
	glue_call_server(__pos, __msg, RPC_ID_lvd_setup_timer);

	*__pos = 0;
	{
		*timer_ptr = glue_unpack(__pos, __msg, __ext, struct timer_list*);
		if (*timer_ptr) {
			caller_unmarshal_kernel__lvd_setup_timer__timer__io(__pos, __msg, __ext, ctx, *timer_ptr);
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

void pci_disable_msi(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_disable_msi_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_disable_msi_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_disable_msi__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_disable_msi);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_disable_msi__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_disable_msix(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_disable_msix_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_disable_msix_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_disable_msix__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_disable_msix);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_disable_msix__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_disable_device(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_disable_device_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_disable_device_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_disable_device__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_disable_device);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_disable_device__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int pci_bus_read_config_word(struct pci_bus* bus, unsigned int devfn, int pos, unsigned short* value)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
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

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *bus_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__pci_bus_read_config_word__bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *devfn_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *pos_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *value_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		(void)value_ptr;
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_bus_read_config_word);

	*__pos = 0;
	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__pci_bus_read_config_word__bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
	}

	{
	}

	{
		if (*value_ptr) {
			**value_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pci_bus_write_config_word(struct pci_bus* bus, unsigned int devfn, int pos, unsigned short value)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
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

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *bus_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__pci_bus_write_config_word__bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *devfn_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *pos_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *value_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_bus_write_config_word);

	*__pos = 0;
	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__pci_bus_write_config_word__bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void pci_dev_put(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_dev_put_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_dev_put_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_dev_put__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_dev_put);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_dev_put__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

bool queue_work_on(int cpu, struct workqueue_struct* wq, struct work_struct* work)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* cpu_ptr = &cpu;
	struct workqueue_struct** wq_ptr = &wq;
	struct work_struct** work_ptr = &work;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct queue_work_on_call_ctx call_ctx = {cpu, wq, work};
	__maybe_unused const struct queue_work_on_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *cpu_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *wq_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*wq_ptr) {
			caller_marshal_kernel__queue_work_on__wq__in(__pos, __msg, __ext, ctx, *wq_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *work_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*work_ptr) {
			caller_marshal_kernel__queue_work_on__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_queue_work_on);

	*__pos = 0;
	{
	}

	{
		if (*wq_ptr) {
			caller_unmarshal_kernel__queue_work_on__wq__in(__pos, __msg, __ext, ctx, *wq_ptr);
		}

	}

	{
		if (*work_ptr) {
			caller_unmarshal_kernel__queue_work_on__work__in(__pos, __msg, __ext, ctx, *work_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pci_select_bars(struct pci_dev* dev, unsigned long flags)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	unsigned long* flags_ptr = &flags;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_select_bars_call_ctx call_ctx = {dev, flags};
	__maybe_unused const struct pci_select_bars_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_select_bars__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_select_bars);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_select_bars__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pcie_get_minimum_link(struct pci_dev* dev, unsigned int* speed, unsigned int* width)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	unsigned int** speed_ptr = &speed;
	unsigned int** width_ptr = &width;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pcie_get_minimum_link_call_ctx call_ctx = {dev, speed, width};
	__maybe_unused const struct pcie_get_minimum_link_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pcie_get_minimum_link__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *speed_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*speed_ptr) {
			glue_pack(__pos, __msg, __ext, **speed_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *width_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*width_ptr) {
			glue_pack(__pos, __msg, __ext, **width_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pcie_get_minimum_link);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pcie_get_minimum_link__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*speed_ptr) {
			**speed_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		if (*width_ptr) {
			**width_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pcie_capability_read_word(struct pci_dev* dev, int pos, unsigned short* val)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int* pos_ptr = &pos;
	unsigned short** val_ptr = &val;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pcie_capability_read_word_call_ctx call_ctx = {dev, pos, val};
	__maybe_unused const struct pcie_capability_read_word_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pcie_capability_read_word__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *pos_ptr);
	}

	{
		(void)val_ptr;
	}

	glue_call_server(__pos, __msg, RPC_ID_pcie_capability_read_word);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pcie_capability_read_word__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
		*val_ptr = glue_unpack(__pos, __msg, __ext, unsigned short*);
		if (*val_ptr) {
			**val_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void pci_restore_state(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_restore_state_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_restore_state_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_restore_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_restore_state);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_restore_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int pci_set_power_state(struct pci_dev* dev, int state)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int* state_ptr = &state;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_set_power_state_call_ctx call_ctx = {dev, state};
	__maybe_unused const struct pci_set_power_state_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_set_power_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_set_power_state);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_set_power_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pci_save_state(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_save_state_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_save_state_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_save_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_save_state);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_save_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void pci_set_master(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_set_master_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_set_master_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_set_master__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_set_master);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_set_master__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int pci_request_selected_regions(struct pci_dev* pdev, int bars, char const* res_name)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	int* bars_ptr = &bars;
	char const** res_name_ptr = &res_name;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_request_selected_regions_call_ctx call_ctx = {pdev, bars, res_name};
	__maybe_unused const struct pci_request_selected_regions_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_request_selected_regions__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *bars_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *res_name_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*res_name_ptr) {
			size_t __i, __len;
			char const* array = *res_name_ptr;
			for (__len = 0; array[__len]; ++__len);
			glue_pack(__pos, __msg, __ext, __len + 1);
			for (__i = 0; __i < __len; ++__i) {
				char const* element = &array[__i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_request_selected_regions);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_request_selected_regions__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
	}

	{
		(void)res_name_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void pci_release_selected_regions(struct pci_dev* pdev, int bars)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** pdev_ptr = &pdev;
	int* bars_ptr = &bars;
	
	__maybe_unused const struct pci_release_selected_regions_call_ctx call_ctx = {pdev, bars};
	__maybe_unused const struct pci_release_selected_regions_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *pdev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__pci_release_selected_regions__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *bars_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_release_selected_regions);

	*__pos = 0;
	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__pci_release_selected_regions__pdev__in(__pos, __msg, __ext, ctx, *pdev_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int mod_timer(struct timer_list* timer, unsigned long expires)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list** timer_ptr = &timer;
	unsigned long* expires_ptr = &expires;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mod_timer_call_ctx call_ctx = {timer, expires};
	__maybe_unused const struct mod_timer_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		//printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *timer_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*timer_ptr) {
			caller_marshal_kernel__mod_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *expires_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_mod_timer);

	*__pos = 0;
	{
		if (*timer_ptr) {
			caller_unmarshal_kernel__mod_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
	//	printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pci_wake_from_d3(struct pci_dev* dev, bool enable)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	bool* enable_ptr = &enable;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_wake_from_d3_call_ctx call_ctx = {dev, enable};
	__maybe_unused const struct pci_wake_from_d3_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_wake_from_d3__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *enable_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_wake_from_d3);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_wake_from_d3__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pci_enable_device_mem(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_device_mem_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_enable_device_mem_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_device_mem__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_enable_device_mem);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_device_mem__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void* ioremap_nocache(unsigned long long phys_addr, unsigned long size)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long long* phys_addr_ptr = &phys_addr;
	unsigned long* size_ptr = &size;
	cptr_t ioremap_cptr;
	gpa_t ioremap_gpa;
	uint64_t ioremap_len;
	void* ret = 0;
	void** ret_ptr = &ret;
	
	__maybe_unused const struct ioremap_nocache_call_ctx call_ctx = {phys_addr, size};
	__maybe_unused const struct ioremap_nocache_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		lcd_cptr_alloc(&ioremap_cptr);
		glue_pack(__pos, __msg, __ext, cptr_val(ioremap_cptr));
	}

	{
		glue_pack(__pos, __msg, __ext, *phys_addr_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *size_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_ioremap_nocache);

	*__pos = 0;
	{
		int __ret;
		ioremap_len = glue_unpack(__pos, __msg, __ext, uint64_t);
		__ret = lcd_ioremap_phys(ioremap_cptr, ioremap_len, &ioremap_gpa);
		if (__ret) {
			LIBLCD_ERR("failed to remap bar region");
		}
		*ret_ptr = lcd_ioremap(gpa_val(ioremap_gpa), ioremap_len);
		if (!*ret_ptr) {
			LIBLCD_ERR("failed to ioremap virt");
		}
		printk("%s, ioremap returned %p", __func__, *ret_ptr);
	}

	{
	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

unsigned long msleep_interruptible(unsigned int msecs)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* msecs_ptr = &msecs;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused const struct msleep_interruptible_call_ctx call_ctx = {msecs};
	__maybe_unused const struct msleep_interruptible_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *msecs_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_msleep_interruptible);

	*__pos = 0;
	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void netdev_rss_key_fill(void* buffer, unsigned long len)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	void** buffer_ptr = &buffer;
	unsigned long* len_ptr = &len;
	
	__maybe_unused const struct netdev_rss_key_fill_call_ctx call_ctx = {buffer, len};
	__maybe_unused const struct netdev_rss_key_fill_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		unsigned char* __casted = (unsigned char*)*buffer_ptr;
		unsigned char* const* __casted_ptr = &__casted;
		{
			__maybe_unused const void* __adjusted = *__casted_ptr;
			glue_pack(__pos, __msg, __ext, __adjusted);
			if (*__casted_ptr) {
				size_t __i, __len = (ctx->len);
				unsigned char* array = *__casted_ptr;
				glue_pack(__pos, __msg, __ext, __len);
				// Warning: see David if this breaks
				glue_user_trace("Warning: see David if this breaks");
				for (__i = 0; __i < __len; ++__i) {
					unsigned char* element = &array[__i];
					glue_pack(__pos, __msg, __ext, *element);
				}

			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_netdev_rss_key_fill);

	*__pos = 0;
	{
		unsigned char* __casted = (unsigned char*)*buffer_ptr;
		unsigned char** __casted_ptr = &__casted;
		{
			*__casted_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
			if (*__casted_ptr) {
				int __i;
				unsigned char* array = *__casted_ptr;
				size_t __len = glue_unpack(__pos, __msg, __ext, size_t);
				// Warning: see David if this breaks
				glue_user_trace("Warning: see David if this breaks");
				for (__i = 0; __i < __len; ++__i) {
					unsigned char* element = &array[__i];
					*element = glue_unpack(__pos, __msg, __ext, unsigned char);
				}

			}

		}

		*buffer_ptr = (void*)__casted;
	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int rtnl_is_locked(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct rtnl_is_locked_call_ctx call_ctx = {};
	__maybe_unused const struct rtnl_is_locked_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, __msg, RPC_ID_rtnl_is_locked);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int skb_copy_bits(struct sk_buff const* skb, int offset, void* to, int len)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff const** skb_ptr = &skb;
	int* offset_ptr = &offset;
	void** to_ptr = &to;
	int* len_ptr = &len;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct skb_copy_bits_call_ctx call_ctx = {skb, offset, to, len};
	__maybe_unused const struct skb_copy_bits_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__skb_copy_bits__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *offset_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *to_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*to_ptr) {
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_skb_copy_bits);

	*__pos = 0;
	{
		*skb_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sk_buff const*);
		if (*skb_ptr) {
			struct sk_buff* writable = (struct sk_buff*)*skb_ptr;
			caller_unmarshal_kernel__skb_copy_bits__skb__io(__pos, __msg, __ext, ctx, writable);
		}

	}

	{
	}

	{
		*to_ptr = glue_unpack_shadow(__pos, __msg, __ext, void*);
		if (*to_ptr) {
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int ___pskb_trim(struct sk_buff* skb, unsigned int len)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	unsigned int* len_ptr = &len;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ___pskb_trim_call_ctx call_ctx = {skb, len};
	__maybe_unused const struct ___pskb_trim_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel_____pskb_trim__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID____pskb_trim);

	*__pos = 0;
	{
		*skb_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			caller_unmarshal_kernel_____pskb_trim__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pskb_expand_head(struct sk_buff* skb, int nhead, int ntail, unsigned int gfp_mask)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	int* nhead_ptr = &nhead;
	int* ntail_ptr = &ntail;
	unsigned int* gfp_mask_ptr = &gfp_mask;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pskb_expand_head_call_ctx call_ctx = {skb, nhead, ntail, gfp_mask};
	__maybe_unused const struct pskb_expand_head_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__pskb_expand_head__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *nhead_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ntail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *gfp_mask_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pskb_expand_head);

	*__pos = 0;
	{
		*skb_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			caller_unmarshal_kernel__pskb_expand_head__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void consume_skb(struct sk_buff* skb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	
	__maybe_unused const struct consume_skb_call_ctx call_ctx = {skb};
	__maybe_unused const struct consume_skb_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__consume_skb__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_consume_skb);

	*__pos = 0;
	{
		if (*skb_ptr) {
			caller_unmarshal_kernel__consume_skb__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

unsigned char* __pskb_pull_tail(struct sk_buff* skb, int delta)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	int* delta_ptr = &delta;
	unsigned char* ret = 0;
	unsigned char** ret_ptr = &ret;
	
	__maybe_unused const struct __pskb_pull_tail_call_ctx call_ctx = {skb, delta};
	__maybe_unused const struct __pskb_pull_tail_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel____pskb_pull_tail__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *delta_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID___pskb_pull_tail);

	*__pos = 0;
	{
		*skb_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			caller_unmarshal_kernel____pskb_pull_tail__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*ret_ptr) {
			**ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

unsigned char* skb_put(struct sk_buff* skb, unsigned int len)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	unsigned int* len_ptr = &len;
	unsigned char* ret = 0;
	unsigned char** ret_ptr = &ret;
	
	__maybe_unused const struct skb_put_call_ctx call_ctx = {skb, len};
	__maybe_unused const struct skb_put_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__skb_put__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_skb_put);

	*__pos = 0;
	{
		if (*skb_ptr) {
			caller_unmarshal_kernel__skb_put__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
	}

	{
		(void)ret_ptr;
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void skb_tstamp_tx(struct sk_buff* orig_skb, struct skb_shared_hwtstamps* hwtstamps)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** orig_skb_ptr = &orig_skb;
	struct skb_shared_hwtstamps** hwtstamps_ptr = &hwtstamps;
	
	__maybe_unused const struct skb_tstamp_tx_call_ctx call_ctx = {orig_skb, hwtstamps};
	__maybe_unused const struct skb_tstamp_tx_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *orig_skb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*orig_skb_ptr) {
			caller_marshal_kernel__skb_tstamp_tx__orig_skb__io(__pos, __msg, __ext, ctx, *orig_skb_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *hwtstamps_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hwtstamps_ptr) {
			caller_marshal_kernel__skb_tstamp_tx__hwtstamps__in(__pos, __msg, __ext, ctx, *hwtstamps_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_skb_tstamp_tx);

	*__pos = 0;
	{
		if (*orig_skb_ptr) {
			caller_unmarshal_kernel__skb_tstamp_tx__orig_skb__io(__pos, __msg, __ext, ctx, *orig_skb_ptr);
		}

	}

	{
		if (*hwtstamps_ptr) {
			caller_unmarshal_kernel__skb_tstamp_tx__hwtstamps__in(__pos, __msg, __ext, ctx, *hwtstamps_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int skb_pad(struct sk_buff* skb, int pad)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	int* pad_ptr = &pad;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct skb_pad_call_ctx call_ctx = {skb, pad};
	__maybe_unused const struct skb_pad_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__skb_pad__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *pad_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_skb_pad);

	*__pos = 0;
	{
		*skb_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			caller_unmarshal_kernel__skb_pad__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void napi_consume_skb(struct sk_buff* skb, int budget)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	int* budget_ptr = &budget;
	
	__maybe_unused const struct napi_consume_skb_call_ctx call_ctx = {skb, budget};
	__maybe_unused const struct napi_consume_skb_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__napi_consume_skb__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *budget_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_napi_consume_skb);

	*__pos = 0;
	{
		if (*skb_ptr) {
			caller_unmarshal_kernel__napi_consume_skb__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __dev_kfree_skb_any(struct sk_buff* skb, unsigned int reason)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	unsigned int* reason_ptr = &reason;
	
	__maybe_unused const struct __dev_kfree_skb_any_call_ctx call_ctx = {skb, reason};
	__maybe_unused const struct __dev_kfree_skb_any_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel____dev_kfree_skb_any__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *reason_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID___dev_kfree_skb_any);

	*__pos = 0;
	{
		if (*skb_ptr) {
			caller_unmarshal_kernel____dev_kfree_skb_any__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct sk_buff* __napi_alloc_skb(struct napi_struct* napi, unsigned int len, unsigned int gfp_mask)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** napi_ptr = &napi;
	unsigned int* len_ptr = &len;
	unsigned int* gfp_mask_ptr = &gfp_mask;
	struct sk_buff* ret = 0;
	struct sk_buff** ret_ptr = &ret;
	
	__maybe_unused const struct __napi_alloc_skb_call_ctx call_ctx = {napi, len, gfp_mask};
	__maybe_unused const struct __napi_alloc_skb_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *napi_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*napi_ptr) {
			caller_marshal_kernel____napi_alloc_skb__napi__in(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *gfp_mask_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID___napi_alloc_skb);

	*__pos = 0;
	{
		if (*napi_ptr) {
			caller_unmarshal_kernel____napi_alloc_skb__napi__in(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	{
	}

	{
	}

	{
		if (*ret_ptr) {
			caller_unmarshal_kernel____napi_alloc_skb__ret_sk_buff__in(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void napi_gro_flush(struct napi_struct* napi, bool flush_old)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** napi_ptr = &napi;
	bool* flush_old_ptr = &flush_old;
	
	__maybe_unused const struct napi_gro_flush_call_ctx call_ctx = {napi, flush_old};
	__maybe_unused const struct napi_gro_flush_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *napi_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*napi_ptr) {
			caller_marshal_kernel__napi_gro_flush__napi__io(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *flush_old_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_napi_gro_flush);

	*__pos = 0;
	{
		*napi_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct napi_struct*);
		if (*napi_ptr) {
			caller_unmarshal_kernel__napi_gro_flush__napi__io(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void napi_complete_done(struct napi_struct* n, int work_done)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** n_ptr = &n;
	int* work_done_ptr = &work_done;
	
	__maybe_unused const struct napi_complete_done_call_ctx call_ctx = {n, work_done};
	__maybe_unused const struct napi_complete_done_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *n_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*n_ptr) {
			caller_marshal_kernel__napi_complete_done__n__in(__pos, __msg, __ext, ctx, *n_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *work_done_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_napi_complete_done);

	*__pos = 0;
	{
		if (*n_ptr) {
			caller_unmarshal_kernel__napi_complete_done__n__in(__pos, __msg, __ext, ctx, *n_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_unlock(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	
	__maybe_unused const struct rtnl_unlock_call_ctx call_ctx = {};
	__maybe_unused const struct rtnl_unlock_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, __msg, RPC_ID_rtnl_unlock);

	*__pos = 0;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_lock(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	
	__maybe_unused const struct rtnl_lock_call_ctx call_ctx = {};
	__maybe_unused const struct rtnl_lock_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, __msg, RPC_ID_rtnl_lock);

	*__pos = 0;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int call_netdevice_notifiers(unsigned long val, struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long* val_ptr = &val;
	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct call_netdevice_notifiers_call_ctx call_ctx = {val, dev};
	__maybe_unused const struct call_netdevice_notifiers_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *val_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__call_netdevice_notifiers__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_call_netdevice_notifiers);

	*__pos = 0;
	{
	}

	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__call_netdevice_notifiers__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct rtnl_link_stats64* dev_get_stats(struct net_device* dev, struct rtnl_link_stats64* storage)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	struct rtnl_link_stats64** storage_ptr = &storage;
	struct rtnl_link_stats64* ret = 0;
	struct rtnl_link_stats64** ret_ptr = &ret;
	
	__maybe_unused const struct dev_get_stats_call_ctx call_ctx = {dev, storage};
	__maybe_unused const struct dev_get_stats_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__dev_get_stats__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *storage_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*storage_ptr) {
			caller_marshal_kernel__dev_get_stats__storage__in(__pos, __msg, __ext, ctx, *storage_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_dev_get_stats);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__dev_get_stats__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*storage_ptr) {
			caller_unmarshal_kernel__dev_get_stats__storage__in(__pos, __msg, __ext, ctx, *storage_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, struct rtnl_link_stats64*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__dev_get_stats__ret_rtnl_link_stats64__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void unregister_netdev(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct unregister_netdev_call_ctx call_ctx = {dev};
	__maybe_unused const struct unregister_netdev_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__unregister_netdev__dev__io(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_unregister_netdev);

	*__pos = 0;
	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			caller_unmarshal_kernel__unregister_netdev__dev__io(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void free_netdev(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct free_netdev_call_ctx call_ctx = {dev};
	__maybe_unused const struct free_netdev_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__free_netdev__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_free_netdev);

	*__pos = 0;
	{
		glue_remove_shadow(*dev_ptr);
		if (*dev_ptr) {
			caller_unmarshal_kernel__free_netdev__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_napi_del(struct napi_struct* napi)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** napi_ptr = &napi;
	
	__maybe_unused const struct netif_napi_del_call_ctx call_ctx = {napi};
	__maybe_unused const struct netif_napi_del_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *napi_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*napi_ptr) {
			caller_marshal_kernel__netif_napi_del__napi__in(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_netif_napi_del);

	*__pos = 0;
	{
		if (*napi_ptr) {
			caller_unmarshal_kernel__netif_napi_del__napi__in(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

bool napi_hash_del(struct napi_struct* napi)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** napi_ptr = &napi;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct napi_hash_del_call_ctx call_ctx = {napi};
	__maybe_unused const struct napi_hash_del_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *napi_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*napi_ptr) {
			caller_marshal_kernel__napi_hash_del__napi__io(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_napi_hash_del);

	*__pos = 0;
	{
		*napi_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct napi_struct*);
		if (*napi_ptr) {
			caller_unmarshal_kernel__napi_hash_del__napi__io(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int register_netdev(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct register_netdev_call_ctx call_ctx = {dev};
	__maybe_unused const struct register_netdev_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__register_netdev__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_register_netdev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__register_netdev__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void netif_tx_stop_all_queues(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct netif_tx_stop_all_queues_call_ctx call_ctx = {dev};
	__maybe_unused const struct netif_tx_stop_all_queues_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_tx_stop_all_queues__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_netif_tx_stop_all_queues);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_tx_stop_all_queues__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_carrier_on(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct netif_carrier_on_call_ctx call_ctx = {dev};
	__maybe_unused const struct netif_carrier_on_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_carrier_on__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_netif_carrier_on);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_carrier_on__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_carrier_off(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct netif_carrier_off_call_ctx call_ctx = {dev};
	__maybe_unused const struct netif_carrier_off_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_carrier_off__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_netif_carrier_off);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_carrier_off__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct net_device* netdev_all_upper_get_next_dev_rcu(struct net_device* dev, struct list_head** iter)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	struct list_head*** iter_ptr = &iter;
	struct net_device* ret = 0;
	struct net_device** ret_ptr = &ret;
	
	__maybe_unused const struct netdev_all_upper_get_next_dev_rcu_call_ctx call_ctx = {dev, iter};
	__maybe_unused const struct netdev_all_upper_get_next_dev_rcu_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
		return NULL;
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netdev_all_upper_get_next_dev_rcu__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *iter_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*iter_ptr) {
			__maybe_unused const void* __adjusted = **iter_ptr;
			glue_pack_shadow(__pos, __msg, __ext, __adjusted);
			if (**iter_ptr) {
				caller_marshal_kernel__netdev_all_upper_get_next_dev_rcu__iter__in(__pos, __msg, __ext, ctx, **iter_ptr);
			}

		}

	}

	glue_call_server(__pos, __msg, RPC_ID_netdev_all_upper_get_next_dev_rcu);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*iter_ptr) {
			if (**iter_ptr) {
				caller_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__iter__in(__pos, __msg, __ext, ctx, **iter_ptr);
			}

		}

	}

	{
		if (*ret_ptr) {
			caller_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__ret_net_device__in(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void napi_disable(struct napi_struct* n)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** n_ptr = &n;
	
	__maybe_unused const struct napi_disable_call_ctx call_ctx = {n};
	__maybe_unused const struct napi_disable_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *n_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*n_ptr) {
			caller_marshal_kernel__napi_disable__n__in(__pos, __msg, __ext, ctx, *n_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_napi_disable);

	*__pos = 0;
	{
		if (*n_ptr) {
			caller_unmarshal_kernel__napi_disable__n__in(__pos, __msg, __ext, ctx, *n_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void poll_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_poll function_ptr = glue_unpack(__pos, __msg, __ext, fptr_poll);
	struct napi_struct* napi = 0;
	int budget = 0;
	struct napi_struct** napi_ptr = &napi;
	int* budget_ptr = &budget;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct poll_call_ctx call_ctx = {napi, budget};
	__maybe_unused struct poll_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*napi_ptr = glue_unpack(__pos, __msg, __ext, struct napi_struct*);
		if (*napi_ptr) {
			callee_unmarshal_kernel__poll__napi__in(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	{
		*budget_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = function_ptr(napi, budget);

	*__pos = 0;
	{
		if (*napi_ptr) {
			callee_marshal_kernel__poll__napi__in(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_napi_add(struct net_device* dev, struct napi_struct* napi, fptr_poll poll, int weight)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	struct napi_struct** napi_ptr = &napi;
	fptr_poll* poll_ptr = &poll;
	int* weight_ptr = &weight;
	
	__maybe_unused const struct netif_napi_add_call_ctx call_ctx = {dev, napi, poll, weight};
	__maybe_unused const struct netif_napi_add_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_napi_add__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *napi_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*napi_ptr) {
			caller_marshal_kernel__netif_napi_add__napi__in(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *poll_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *weight_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_netif_napi_add);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_napi_add__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*napi_ptr) {
			caller_unmarshal_kernel__netif_napi_add__napi__in(__pos, __msg, __ext, ctx, *napi_ptr);
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

void __napi_schedule_irqoff(struct napi_struct* n)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** n_ptr = &n;
	
	__maybe_unused const struct __napi_schedule_irqoff_call_ctx call_ctx = {n};
	__maybe_unused const struct __napi_schedule_irqoff_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *n_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*n_ptr) {
			caller_marshal_kernel____napi_schedule_irqoff__n__in(__pos, __msg, __ext, ctx, *n_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID___napi_schedule_irqoff);

	*__pos = 0;
	{
		if (*n_ptr) {
			caller_unmarshal_kernel____napi_schedule_irqoff__n__in(__pos, __msg, __ext, ctx, *n_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

unsigned int napi_gro_receive(struct napi_struct* napi, struct sk_buff* skb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** napi_ptr = &napi;
	struct sk_buff** skb_ptr = &skb;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct napi_gro_receive_call_ctx call_ctx = {napi, skb};
	__maybe_unused const struct napi_gro_receive_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
		{
			int i = 0;
			for (i = 0; i < skb->len; i++) {
				printk("%02x ", skb->data[i]);
			}
		}
		return 0;
	}

	{
		__maybe_unused const void* __adjusted = *napi_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*napi_ptr) {
			caller_marshal_kernel__napi_gro_receive__napi__in(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__napi_gro_receive__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_napi_gro_receive);

	*__pos = 0;
	{
		if (*napi_ptr) {
			caller_unmarshal_kernel__napi_gro_receive__napi__in(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	{
		*skb_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			caller_unmarshal_kernel__napi_gro_receive__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int netif_receive_skb(struct sk_buff* skb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct netif_receive_skb_call_ctx call_ctx = {skb};
	__maybe_unused const struct netif_receive_skb_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__netif_receive_skb__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_netif_receive_skb);

	*__pos = 0;
	{
		*skb_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			caller_unmarshal_kernel__netif_receive_skb__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int skb_checksum_help(struct sk_buff* skb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct skb_checksum_help_call_ctx call_ctx = {skb};
	__maybe_unused const struct skb_checksum_help_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__skb_checksum_help__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_skb_checksum_help);

	*__pos = 0;
	{
		*skb_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			caller_unmarshal_kernel__skb_checksum_help__skb__io(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void netif_device_attach(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct netif_device_attach_call_ctx call_ctx = {dev};
	__maybe_unused const struct netif_device_attach_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_device_attach__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_netif_device_attach);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_device_attach__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_tx_wake_queue(struct netdev_queue* dev_queue)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct netdev_queue** dev_queue_ptr = &dev_queue;
	
	__maybe_unused const struct netif_tx_wake_queue_call_ctx call_ctx = {dev_queue};
	__maybe_unused const struct netif_tx_wake_queue_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_queue_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_queue_ptr) {
			caller_marshal_kernel__netif_tx_wake_queue__dev_queue__in(__pos, __msg, __ext, ctx, *dev_queue_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_netif_tx_wake_queue);

	*__pos = 0;
	{
		if (*dev_queue_ptr) {
			caller_unmarshal_kernel__netif_tx_wake_queue__dev_queue__in(__pos, __msg, __ext, ctx, *dev_queue_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_device_detach(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct netif_device_detach_call_ctx call_ctx = {dev};
	__maybe_unused const struct netif_device_detach_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_device_detach__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_netif_device_detach);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_device_detach__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_wake_subqueue(struct net_device* dev, unsigned short queue_index)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	unsigned short* queue_index_ptr = &queue_index;
	
	__maybe_unused const struct netif_wake_subqueue_call_ctx call_ctx = {dev, queue_index};
	__maybe_unused const struct netif_wake_subqueue_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_wake_subqueue__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *queue_index_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_netif_wake_subqueue);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_wake_subqueue__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int netif_set_real_num_rx_queues(struct net_device* dev, unsigned int rxq)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	unsigned int* rxq_ptr = &rxq;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct netif_set_real_num_rx_queues_call_ctx call_ctx = {dev, rxq};
	__maybe_unused const struct netif_set_real_num_rx_queues_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_set_real_num_rx_queues__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *rxq_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_netif_set_real_num_rx_queues);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_set_real_num_rx_queues__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int netif_set_real_num_tx_queues(struct net_device* dev, unsigned int txq)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	unsigned int* txq_ptr = &txq;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct netif_set_real_num_tx_queues_call_ctx call_ctx = {dev, txq};
	__maybe_unused const struct netif_set_real_num_tx_queues_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_set_real_num_tx_queues__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *txq_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_netif_set_real_num_tx_queues);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_set_real_num_tx_queues__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int netif_set_xps_queue(struct net_device* dev, struct cpumask const* mask, unsigned short index)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	struct cpumask const** mask_ptr = &mask;
	unsigned short* index_ptr = &index;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct netif_set_xps_queue_call_ctx call_ctx = {dev, mask, index};
	__maybe_unused const struct netif_set_xps_queue_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_set_xps_queue__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *mask_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*mask_ptr) {
			caller_marshal_kernel__netif_set_xps_queue__mask__in(__pos, __msg, __ext, ctx, *mask_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *index_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_netif_set_xps_queue);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_set_xps_queue__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*mask_ptr) {
			struct cpumask* writable = (struct cpumask*)*mask_ptr;
			caller_unmarshal_kernel__netif_set_xps_queue__mask__in(__pos, __msg, __ext, ctx, writable);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void thread_fn_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_thread_fn function_ptr = glue_unpack(__pos, __msg, __ext, fptr_thread_fn);
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
		*irq_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = function_ptr(irq, id);

	*__pos = 0;
	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void handler_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_handler function_ptr = glue_unpack(__pos, __msg, __ext, fptr_handler);
	int irq = 0;
	void* id = 0;
	int* irq_ptr = &irq;
	void** id_ptr = &id;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct handler_call_ctx call_ctx = {irq, id};
	__maybe_unused struct handler_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		//printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*id_ptr = glue_unpack(__pos, __msg, __ext, void*);
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
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		//printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int request_threaded_irq(unsigned int irq, fptr_handler handler, fptr_thread_fn thread_fn, unsigned long irqflags, char const* devname, void* dev_id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* irq_ptr = &irq;
	fptr_handler* handler_ptr = &handler;
	unsigned long* irqflags_ptr = &irqflags;
	char const** devname_ptr = &devname;
	void** dev_id_ptr = &dev_id;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct request_threaded_irq_call_ctx call_ctx = {irq, handler, thread_fn, irqflags, devname, dev_id};
	__maybe_unused const struct request_threaded_irq_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *handler_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *irqflags_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *devname_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*devname_ptr) {
			size_t __i, __len;
			char const* array = *devname_ptr;
			for (__len = 0; array[__len]; ++__len);
			glue_pack(__pos, __msg, __ext, __len + 1);
			for (__i = 0; __i < __len; ++__i) {
				char const* element = &array[__i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		__maybe_unused const void* __adjusted = *dev_id_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_id_ptr) {
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_request_threaded_irq);

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
		(void)dev_id_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void free_irq(unsigned int irq, void* dev_id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* irq_ptr = &irq;
	void** dev_id_ptr = &dev_id;
	
	__maybe_unused const struct free_irq_call_ctx call_ctx = {irq, dev_id};
	__maybe_unused const struct free_irq_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *dev_id_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_id_ptr) {
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_free_irq);

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
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* irq_ptr = &irq;
	
	__maybe_unused const struct synchronize_irq_call_ctx call_ctx = {irq};
	__maybe_unused const struct synchronize_irq_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_synchronize_irq);

	*__pos = 0;
	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int irq_set_affinity_hint(unsigned int irq, struct cpumask const* m)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* irq_ptr = &irq;
	struct cpumask const** m_ptr = &m;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct irq_set_affinity_hint_call_ctx call_ctx = {irq, m};
	__maybe_unused const struct irq_set_affinity_hint_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *m_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*m_ptr) {
			caller_marshal_kernel__irq_set_affinity_hint__m__in(__pos, __msg, __ext, ctx, *m_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_irq_set_affinity_hint);

	*__pos = 0;
	{
	}

	{
		if (*m_ptr) {
			struct cpumask* writable = (struct cpumask*)*m_ptr;
			caller_unmarshal_kernel__irq_set_affinity_hint__m__in(__pos, __msg, __ext, ctx, writable);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pci_enable_msix_range(struct pci_dev* dev, struct msix_entry* entries, int minvec, int maxvec)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	struct msix_entry** entries_ptr = &entries;
	int* minvec_ptr = &minvec;
	int* maxvec_ptr = &maxvec;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_msix_range_call_ctx call_ctx = {dev, entries, minvec, maxvec};
	__maybe_unused const struct pci_enable_msix_range_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_msix_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *entries_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*entries_ptr) {
			size_t __i, __len = (ctx->maxvec);
			struct msix_entry* array = *entries_ptr;
			glue_pack(__pos, __msg, __ext, __len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (__i = 0; __i < __len; ++__i) {
				struct msix_entry* element = &array[__i];
				caller_marshal_kernel__pci_enable_msix_range__entries__io(__pos, __msg, __ext, ctx, element);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *minvec_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *maxvec_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_enable_msix_range);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_msix_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*entries_ptr = glue_unpack(__pos, __msg, __ext, struct msix_entry*);
		if (*entries_ptr) {
			int __i;
			struct msix_entry* array = *entries_ptr;
			size_t __len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (__i = 0; __i < __len; ++__i) {
				struct msix_entry* element = &array[__i];
				caller_unmarshal_kernel__pci_enable_msix_range__entries__io(__pos, __msg, __ext, ctx, element);
			}

		}

	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		int i = 0;
		for (i = 0; i < ret; i++) {
			printk("entry[%d]->vector %u\n", i, entries[i].vector);
		}
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pci_enable_msi_range(struct pci_dev* dev, int minvec, int maxvec)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int* minvec_ptr = &minvec;
	int* maxvec_ptr = &maxvec;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_msi_range_call_ctx call_ctx = {dev, minvec, maxvec};
	__maybe_unused const struct pci_enable_msi_range_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_msi_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *minvec_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *maxvec_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_enable_msi_range);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_msi_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int dev_addr_del(struct net_device* dev, unsigned char const* addr, unsigned char addr_type)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	unsigned char const** addr_ptr = &addr;
	unsigned char* addr_type_ptr = &addr_type;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct dev_addr_del_call_ctx call_ctx = {dev, addr, addr_type};
	__maybe_unused const struct dev_addr_del_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__dev_addr_del__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *addr_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*addr_ptr) {
			size_t __i, __len = (ETH_ALEN);
			unsigned char const* array = *addr_ptr;
			glue_pack(__pos, __msg, __ext, __len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (__i = 0; __i < __len; ++__i) {
				unsigned char const* element = &array[__i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *addr_type_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_dev_addr_del);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__dev_addr_del__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		(void)addr_ptr;
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int dev_addr_add(struct net_device* dev, unsigned char const* addr, unsigned char addr_type)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	unsigned char const** addr_ptr = &addr;
	unsigned char* addr_type_ptr = &addr_type;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct dev_addr_add_call_ctx call_ctx = {dev, addr, addr_type};
	__maybe_unused const struct dev_addr_add_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__dev_addr_add__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *addr_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*addr_ptr) {
			size_t __i, __len = (ETH_ALEN);
			unsigned char const* array = *addr_ptr;
			glue_pack(__pos, __msg, __ext, __len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (__i = 0; __i < __len; ++__i) {
				unsigned char const* element = &array[__i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *addr_type_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_dev_addr_add);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__dev_addr_add__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		(void)addr_ptr;
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void sync_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_sync function_ptr = glue_unpack(__pos, __msg, __ext, fptr_sync);
	struct net_device* dev = 0;
	unsigned char const* addr = 0;
	struct net_device** dev_ptr = &dev;
	unsigned char const** addr_ptr = &addr;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct sync_call_ctx call_ctx = {dev, addr};
	__maybe_unused struct sync_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__sync__net_device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		size_t __size = sizeof(unsigned char) * glue_peek(__pos, __msg, __ext);
		*addr_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, unsigned char const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*addr_ptr) {
			unsigned char* writable = (unsigned char*)*addr_ptr;
			int __i;
			unsigned char* array = writable;
			size_t __len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (__i = 0; __i < __len; ++__i) {
				unsigned char* element = &array[__i];
				*element = glue_unpack(__pos, __msg, __ext, unsigned char);
			}

		}

	}

	ret = function_ptr(dev, addr);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__sync__net_device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		(void)addr_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void unsync_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_unsync function_ptr = glue_unpack(__pos, __msg, __ext, fptr_unsync);
	struct net_device* dev = 0;
	unsigned char const* addr = 0;
	struct net_device** dev_ptr = &dev;
	unsigned char const** addr_ptr = &addr;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct unsync_call_ctx call_ctx = {dev, addr};
	__maybe_unused struct unsync_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__unsync__net_device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		size_t __size = sizeof(unsigned char) * glue_peek(__pos, __msg, __ext);
		*addr_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, unsigned char const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*addr_ptr) {
			unsigned char* writable = (unsigned char*)*addr_ptr;
			int __i;
			unsigned char* array = writable;
			size_t __len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (__i = 0; __i < __len; ++__i) {
				unsigned char* element = &array[__i];
				*element = glue_unpack(__pos, __msg, __ext, unsigned char);
			}

		}

	}

	ret = function_ptr(dev, addr);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__unsync__net_device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		(void)addr_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __hw_addr_unsync_dev(struct netdev_hw_addr_list* list, struct net_device* dev, fptr_unsync unsync)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct netdev_hw_addr_list** list_ptr = &list;
	struct net_device** dev_ptr = &dev;
	fptr_unsync* unsync_ptr = &unsync;
	
	__maybe_unused const struct __hw_addr_unsync_dev_call_ctx call_ctx = {list, dev, unsync};
	__maybe_unused const struct __hw_addr_unsync_dev_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *list_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*list_ptr) {
			caller_marshal_kernel____hw_addr_unsync_dev__list__in(__pos, __msg, __ext, ctx, *list_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel____hw_addr_unsync_dev__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *unsync_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID___hw_addr_unsync_dev);

	*__pos = 0;
	{
		if (*list_ptr) {
			caller_unmarshal_kernel____hw_addr_unsync_dev__list__in(__pos, __msg, __ext, ctx, *list_ptr);
		}

	}

	{
		if (*dev_ptr) {
			caller_unmarshal_kernel____hw_addr_unsync_dev__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int __hw_addr_sync_dev(struct netdev_hw_addr_list* list, struct net_device* dev, fptr_sync sync, fptr_unsync unsync)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct netdev_hw_addr_list** list_ptr = &list;
	struct net_device** dev_ptr = &dev;
	fptr_sync* sync_ptr = &sync;
	fptr_unsync* unsync_ptr = &unsync;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct __hw_addr_sync_dev_call_ctx call_ctx = {list, dev, sync, unsync};
	__maybe_unused const struct __hw_addr_sync_dev_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct net_device* __adjusted = container_of(*list_ptr, struct net_device, uc);
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*list_ptr) {
			caller_marshal_kernel____hw_addr_sync_dev__list__in(__pos, __msg, __ext, ctx, *list_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel____hw_addr_sync_dev__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *sync_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *unsync_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID___hw_addr_sync_dev);

	*__pos = 0;
	{
		if (*list_ptr) {
			caller_unmarshal_kernel____hw_addr_sync_dev__list__in(__pos, __msg, __ext, ctx, *list_ptr);
		}

	}

	{
		if (*dev_ptr) {
			caller_unmarshal_kernel____hw_addr_sync_dev__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int eth_platform_get_mac_address(struct device* dev, unsigned char* mac_addr)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct device** dev_ptr = &dev;
	unsigned char** mac_addr_ptr = &mac_addr;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct eth_platform_get_mac_address_call_ctx call_ctx = {dev, mac_addr};
	__maybe_unused const struct eth_platform_get_mac_address_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		struct pci_dev* __adjusted = container_of(*dev_ptr, struct pci_dev, dev);
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__eth_platform_get_mac_address__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *mac_addr_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*mac_addr_ptr) {
			size_t __i, __len = (ETH_ALEN);
			unsigned char* array = *mac_addr_ptr;
			glue_pack(__pos, __msg, __ext, __len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (__i = 0; __i < __len; ++__i) {
				unsigned char* element = &array[__i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	glue_call_server(__pos, __msg, RPC_ID_eth_platform_get_mac_address);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__eth_platform_get_mac_address__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*mac_addr_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*mac_addr_ptr) {
			int __i;
			unsigned char* array = *mac_addr_ptr;
			size_t __len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (__i = 0; __i < __len; ++__i) {
				unsigned char* element = &array[__i];
				*element = glue_unpack(__pos, __msg, __ext, unsigned char);
			}

		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct net_device* alloc_etherdev_mqs(int sizeof_priv, unsigned int txqs, unsigned int rxqs)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* sizeof_priv_ptr = &sizeof_priv;
	unsigned int* txqs_ptr = &txqs;
	unsigned int* rxqs_ptr = &rxqs;
	struct net_device* ret = 0;
	struct net_device** ret_ptr = &ret;
	
	__maybe_unused const struct alloc_etherdev_mqs_call_ctx call_ctx = {sizeof_priv, txqs, rxqs};
	__maybe_unused const struct alloc_etherdev_mqs_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *sizeof_priv_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *txqs_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rxqs_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_alloc_etherdev_mqs);

	*__pos = 0;
	{
	}

	{
	}

	{
	}

	{
		size_t alloc_size = sizeof(struct net_device);

		if (ctx->sizeof_priv) {
			/* ensure 32-byte alignment of private area */
			alloc_size = ALIGN(alloc_size, NETDEV_ALIGN);
			alloc_size += ctx->sizeof_priv;
		}
		/* ensure 32-byte alignment of whole construct */
		alloc_size += NETDEV_ALIGN - 1;

		*ret_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct net_device*, (alloc_size), (DEFAULT_GFP_FLAGS));
		if (*ret_ptr) {
			caller_unmarshal_kernel__alloc_etherdev_mqs__ret_net_device__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}
	{
		INIT_LIST_HEAD(&ret->mc.list);
		INIT_LIST_HEAD(&ret->uc.list);
		ret->mc.count = 0;
		ret->uc.count = 0;
	}
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void ndo_validate_addr_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_validate_addr function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_validate_addr);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ndo_validate_addr_call_ctx call_ctx = {dev};
	__maybe_unused struct ndo_validate_addr_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_validate_addr__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_validate_addr__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

unsigned long dev_trans_start(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused const struct dev_trans_start_call_ctx call_ctx = {dev};
	__maybe_unused const struct dev_trans_start_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__dev_trans_start__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_dev_trans_start);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__dev_trans_start__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void mdio_read_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mdio_read function_ptr = glue_unpack(__pos, __msg, __ext, fptr_mdio_read);
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
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mdio_read__net_device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*prtad_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*devad_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*addr_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	ret = function_ptr(dev, prtad, devad, addr);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mdio_read__net_device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mdio_write_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_mdio_write function_ptr = glue_unpack(__pos, __msg, __ext, fptr_mdio_write);
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
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mdio_write__net_device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*prtad_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*devad_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*addr_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*val_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	ret = function_ptr(dev, prtad, devad, addr, val);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mdio_write__net_device__in(__pos, __msg, __ext, ctx, *dev_ptr);
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
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int mdio_mii_ioctl(struct mdio_if_info const* mdio, struct mii_ioctl_data* mii_data, int cmd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mdio_if_info const** mdio_ptr = &mdio;
	struct mii_ioctl_data** mii_data_ptr = &mii_data;
	int* cmd_ptr = &cmd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mdio_mii_ioctl_call_ctx call_ctx = {mdio, mii_data, cmd};
	__maybe_unused const struct mdio_mii_ioctl_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *mdio_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*mdio_ptr) {
			caller_marshal_kernel__mdio_mii_ioctl__mdio__in(__pos, __msg, __ext, ctx, *mdio_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *mii_data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*mii_data_ptr) {
			caller_marshal_kernel__mdio_mii_ioctl__mii_data__io(__pos, __msg, __ext, ctx, *mii_data_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_mdio_mii_ioctl);

	*__pos = 0;
	{
		if (*mdio_ptr) {
			struct mdio_if_info* writable = (struct mdio_if_info*)*mdio_ptr;
			caller_unmarshal_kernel__mdio_mii_ioctl__mdio__in(__pos, __msg, __ext, ctx, writable);
		}

	}

	{
		*mii_data_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct mii_ioctl_data*);
		if (*mii_data_ptr) {
			caller_unmarshal_kernel__mdio_mii_ioctl__mii_data__io(__pos, __msg, __ext, ctx, *mii_data_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int mdio45_probe(struct mdio_if_info* mdio, int prtad)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mdio_if_info** mdio_ptr = &mdio;
	int* prtad_ptr = &prtad;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mdio45_probe_call_ctx call_ctx = {mdio, prtad};
	__maybe_unused const struct mdio45_probe_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *mdio_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*mdio_ptr) {
			caller_marshal_kernel__mdio45_probe__mdio__in(__pos, __msg, __ext, ctx, *mdio_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *prtad_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_mdio45_probe);

	*__pos = 0;
	{
		if (*mdio_ptr) {
			caller_unmarshal_kernel__mdio45_probe__mdio__in(__pos, __msg, __ext, ctx, *mdio_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void pci_unregister_driver(struct pci_driver* drv)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_driver** drv_ptr = &drv;
	
	__maybe_unused const struct pci_unregister_driver_call_ctx call_ctx = {drv};
	__maybe_unused const struct pci_unregister_driver_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *drv_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*drv_ptr) {
			caller_marshal_kernel__pci_unregister_driver__drv__in(__pos, __msg, __ext, ctx, *drv_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_unregister_driver);

	*__pos = 0;
	{
		if (*drv_ptr) {
			caller_unmarshal_kernel__pci_unregister_driver__drv__in(__pos, __msg, __ext, ctx, *drv_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int __pci_register_driver(struct pci_driver* drv, struct module* owner, char const* mod_name)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_driver** drv_ptr = &drv;
	struct module** owner_ptr = &owner;
	char const** mod_name_ptr = &mod_name;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct __pci_register_driver_call_ctx call_ctx = {drv, owner, mod_name};
	__maybe_unused const struct __pci_register_driver_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *drv_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*drv_ptr) {
			caller_marshal_kernel____pci_register_driver__drv__in(__pos, __msg, __ext, ctx, *drv_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *owner_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*owner_ptr) {
			caller_marshal_kernel____pci_register_driver__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *mod_name_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*mod_name_ptr) {
			size_t __i, __len;
			char const* array = *mod_name_ptr;
			for (__len = 0; array[__len]; ++__len);
			glue_pack(__pos, __msg, __ext, __len + 1);
			for (__i = 0; __i < __len; ++__i) {
				char const* element = &array[__i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	glue_call_server(__pos, __msg, RPC_ID___pci_register_driver);

	*__pos = 0;
	{
		if (*drv_ptr) {
			caller_unmarshal_kernel____pci_register_driver__drv__in(__pos, __msg, __ext, ctx, *drv_ptr);
		}

	}

	{
		if (*owner_ptr) {
			caller_unmarshal_kernel____pci_register_driver__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

	{
		(void)mod_name_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pci_enable_pcie_error_reporting(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_pcie_error_reporting_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_enable_pcie_error_reporting_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_enable_pcie_error_reporting);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pci_disable_pcie_error_reporting(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_disable_pcie_error_reporting_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_disable_pcie_error_reporting_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_disable_pcie_error_reporting);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void lvd_napi_enable(struct napi_struct* napi)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** napi_ptr = &napi;
	
	__maybe_unused const struct lvd_napi_enable_call_ctx call_ctx = {napi};
	__maybe_unused const struct lvd_napi_enable_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *napi_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*napi_ptr) {
			caller_marshal_kernel__lvd_napi_enable__napi__in(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_lvd_napi_enable);

	*__pos = 0;
	{
		if (*napi_ptr) {
			caller_unmarshal_kernel__lvd_napi_enable__napi__in(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_netif_trans_update(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct lvd_netif_trans_update_call_ctx call_ctx = {dev};
	__maybe_unused const struct lvd_netif_trans_update_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__lvd_netif_trans_update__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_lvd_netif_trans_update);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__lvd_netif_trans_update__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_netif_tx_disable(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct lvd_netif_tx_disable_call_ctx call_ctx = {dev};
	__maybe_unused const struct lvd_netif_tx_disable_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__lvd_netif_tx_disable__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_lvd_netif_tx_disable);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__lvd_netif_tx_disable__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_napi_schedule_irqoff(struct napi_struct* napi)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct napi_struct** napi_ptr = &napi;
	
	__maybe_unused const struct lvd_napi_schedule_irqoff_call_ctx call_ctx = {napi};
	__maybe_unused const struct lvd_napi_schedule_irqoff_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *napi_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*napi_ptr) {
			caller_marshal_kernel__lvd_napi_schedule_irqoff__napi__in(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_lvd_napi_schedule_irqoff);

	*__pos = 0;
	{
		if (*napi_ptr) {
			caller_unmarshal_kernel__lvd_napi_schedule_irqoff__napi__in(__pos, __msg, __ext, ctx, *napi_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_netif_tx_wake_all_queues(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct lvd_netif_tx_wake_all_queues_call_ctx call_ctx = {dev};
	__maybe_unused const struct lvd_netif_tx_wake_all_queues_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__lvd_netif_tx_wake_all_queues__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_lvd_netif_tx_wake_all_queues);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__lvd_netif_tx_wake_all_queues__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

unsigned long get_loops_per_jiffy(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused const struct get_loops_per_jiffy_call_ctx call_ctx = {};
	__maybe_unused const struct get_loops_per_jiffy_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, __msg, RPC_ID_get_loops_per_jiffy);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

unsigned long get_jiffies(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused const struct get_jiffies_call_ctx call_ctx = {};
	__maybe_unused const struct get_jiffies_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
	//	printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, __msg, RPC_ID_get_jiffies);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	if (verbose_debug) {
		//printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int eth_validate_addr(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct eth_validate_addr_call_ctx call_ctx = {dev};
	__maybe_unused const struct eth_validate_addr_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__eth_validate_addr__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_eth_validate_addr);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__eth_validate_addr__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

unsigned int ethtool_op_get_link(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct ethtool_op_get_link_call_ctx call_ctx = {dev};
	__maybe_unused const struct ethtool_op_get_link_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__ethtool_op_get_link__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_ethtool_op_get_link);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ethtool_op_get_link__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int ethtool_op_get_ts_info(struct net_device* dev, struct ethtool_ts_info* info)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	struct ethtool_ts_info** info_ptr = &info;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ethtool_op_get_ts_info_call_ctx call_ctx = {dev, info};
	__maybe_unused const struct ethtool_op_get_ts_info_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__ethtool_op_get_ts_info__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *info_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*info_ptr) {
			caller_marshal_kernel__ethtool_op_get_ts_info__info__in(__pos, __msg, __ext, ctx, *info_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_ethtool_op_get_ts_info);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ethtool_op_get_ts_info__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*info_ptr) {
			caller_unmarshal_kernel__ethtool_op_get_ts_info__info__in(__pos, __msg, __ext, ctx, *info_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int ipv6_find_hdr(struct sk_buff const* skb, unsigned int* offset, int target, unsigned short* fragoff, int* flags)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff const** skb_ptr = &skb;
	unsigned int** offset_ptr = &offset;
	int* target_ptr = &target;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ipv6_find_hdr_call_ctx call_ctx = {skb, offset, target, fragoff, flags};
	__maybe_unused const struct ipv6_find_hdr_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__ipv6_find_hdr__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *offset_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		(void)offset_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *target_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_ipv6_find_hdr);

	*__pos = 0;
	{
		if (*skb_ptr) {
			struct sk_buff* writable = (struct sk_buff*)*skb_ptr;
			caller_unmarshal_kernel__ipv6_find_hdr__skb__in(__pos, __msg, __ext, ctx, writable);
		}

	}

	{
		if (*offset_ptr) {
			**offset_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void __init_globals(void) {

	loops_per_jiffy = get_loops_per_jiffy();
	jiffies = get_jiffies();
}

int try_dispatch(enum RPC_ID id, struct fipc_message* __msg, struct ext_registers* __ext)
{
	switch(id) {
	case MODULE_INIT:
		glue_user_trace("MODULE_INIT");
		__init_globals();
		__module_lcd_init();
		shared_mem_init();
		break;

	case MODULE_EXIT:
		glue_user_trace("MODULE_EXIT");
		__module_lcd_exit();
		break;

	case RPC_ID_probe:
		glue_user_trace("probe\n");
		probe_callee(__msg, __ext);
		break;

	case RPC_ID_remove:
		glue_user_trace("remove\n");
		remove_callee(__msg, __ext);
		break;

	case RPC_ID_shutdown:
		glue_user_trace("shutdown\n");
		shutdown_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_set_rx_mode:
		glue_user_trace("ndo_set_rx_mode\n");
		ndo_set_rx_mode_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_stop:
		glue_user_trace("ndo_stop\n");
		ndo_stop_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_open:
		glue_user_trace("ndo_open\n");
		ndo_open_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_start_xmit:
		glue_user_trace("ndo_start_xmit\n");
		ndo_start_xmit_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_features_check:
		glue_user_trace("ndo_features_check\n");
		ndo_features_check_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_set_mac_address:
		glue_user_trace("ndo_set_mac_address\n");
		ndo_set_mac_address_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_do_ioctl:
		glue_user_trace("ndo_do_ioctl\n");
		ndo_do_ioctl_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_change_mtu:
		glue_user_trace("ndo_change_mtu\n");
		ndo_change_mtu_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_tx_timeout:
		glue_user_trace("ndo_tx_timeout\n");
		ndo_tx_timeout_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_get_stats64:
		glue_user_trace("ndo_get_stats64\n");
		ndo_get_stats64_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_setup_tc:
		glue_user_trace("ndo_setup_tc\n");
		ndo_setup_tc_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_fix_features:
		glue_user_trace("ndo_fix_features\n");
		ndo_fix_features_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_set_features:
		glue_user_trace("ndo_set_features\n");
		ndo_set_features_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_set_tx_maxrate:
		glue_user_trace("ndo_set_tx_maxrate\n");
		ndo_set_tx_maxrate_callee(__msg, __ext);
		break;

	case RPC_ID_timer_func:
		//glue_user_trace("timer_func\n");
		timer_func_callee(__msg, __ext);
		break;

	case RPC_ID_poll:
		glue_user_trace("poll\n");
		poll_callee(__msg, __ext);
		break;

	case RPC_ID_thread_fn:
		glue_user_trace("thread_fn\n");
		thread_fn_callee(__msg, __ext);
		break;

	case RPC_ID_handler:
		//glue_user_trace("handler\n");
		handler_callee(__msg, __ext);
		break;

	case RPC_ID_sync:
		glue_user_trace("sync\n");
		sync_callee(__msg, __ext);
		break;

	case RPC_ID_unsync:
		glue_user_trace("unsync\n");
		unsync_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_validate_addr:
		glue_user_trace("ndo_validate_addr\n");
		ndo_validate_addr_callee(__msg, __ext);
		break;

	case RPC_ID_mdio_read:
		glue_user_trace("mdio_read\n");
		mdio_read_callee(__msg, __ext);
		break;

	case RPC_ID_mdio_write:
		glue_user_trace("mdio_write\n");
		mdio_write_callee(__msg, __ext);
		break;

	default:
		return 0;
	}

	return 1;
}

