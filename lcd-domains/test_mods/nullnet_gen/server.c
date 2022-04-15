#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void netif_carrier_on_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netif_carrier_on__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	netif_carrier_on(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netif_carrier_on__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_carrier_off_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netif_carrier_off__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	netif_carrier_off(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netif_carrier_off__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void register_netdevice_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct register_netdevice_call_ctx call_ctx = {dev};
	__maybe_unused struct register_netdevice_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__register_netdevice__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	ret = register_netdevice(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__register_netdevice__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

  printk("%s, dev->flags 0x%x\n", __func__, dev->flags);
	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void consume_skb_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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
		*skb_ptr = glue_unpack(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__consume_skb__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	consume_skb(skb);

	*__pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel__consume_skb__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void alloc_netdev_mqs_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int sizeof_priv = 0;
	char const* name = 0;
	unsigned char name_assign_type = 0;
	fptr_setup setup = 0;
	unsigned int txqs = 0;
	unsigned int rxqs = 0;
	int* sizeof_priv_ptr = &sizeof_priv;
	char const** name_ptr = &name;
	unsigned char* name_assign_type_ptr = &name_assign_type;
	fptr_setup* setup_ptr = &setup;
	unsigned int* txqs_ptr = &txqs;
	unsigned int* rxqs_ptr = &rxqs;
	struct net_device* ret = 0;
	struct net_device** ret_ptr = &ret;
	
	__maybe_unused struct alloc_netdev_mqs_call_ctx call_ctx = {sizeof_priv, name, name_assign_type, setup, txqs, rxqs};
	__maybe_unused struct alloc_netdev_mqs_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sizeof_priv_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*name_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*name_ptr) {
			char* writable = (char*)*name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(__pos, __msg, __ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	{
		*name_assign_type_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
		*setup_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, setup);
	}

	{
		*txqs_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*rxqs_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	ret = alloc_netdev_mqs(sizeof_priv, name, name_assign_type, setup, txqs, rxqs);

	*__pos = 0;
	{
	}

	{
		(void)name_ptr;
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
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__alloc_netdev_mqs__ret_net_device__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void free_netdev_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__free_netdev__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	free_netdev(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__free_netdev__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_lock_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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
	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_unlock_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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
	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_link_unregister_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct rtnl_link_ops* _global_rtnl_link_ops = 0;
	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	
	__maybe_unused struct rtnl_link_unregister_call_ctx call_ctx = {_global_rtnl_link_ops};
	__maybe_unused struct rtnl_link_unregister_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*_global_rtnl_link_ops_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct rtnl_link_ops*);
		if (*_global_rtnl_link_ops_ptr) {
			callee_unmarshal_kernel___global_rtnl_link_ops__in(__pos, __msg, __ext, *_global_rtnl_link_ops_ptr);
		}

	}

	rtnl_link_unregister(_global_rtnl_link_ops);

	*__pos = 0;
	{
		if (*_global_rtnl_link_ops_ptr) {
			callee_marshal_kernel___global_rtnl_link_ops__in(__pos, __msg, __ext, *_global_rtnl_link_ops_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __rtnl_link_unregister_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct rtnl_link_ops* _global_rtnl_link_ops = 0;
	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	
	__maybe_unused struct __rtnl_link_unregister_call_ctx call_ctx = {_global_rtnl_link_ops};
	__maybe_unused struct __rtnl_link_unregister_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*_global_rtnl_link_ops_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct rtnl_link_ops*);
		if (*_global_rtnl_link_ops_ptr) {
			callee_unmarshal_kernel___global_rtnl_link_ops__in(__pos, __msg, __ext, *_global_rtnl_link_ops_ptr);
		}

	}

	__rtnl_link_unregister(_global_rtnl_link_ops);

	*__pos = 0;
	{
		if (*_global_rtnl_link_ops_ptr) {
			callee_marshal_kernel___global_rtnl_link_ops__in(__pos, __msg, __ext, *_global_rtnl_link_ops_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __rtnl_link_register_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct rtnl_link_ops* _global_rtnl_link_ops = 0;
	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct __rtnl_link_register_call_ctx call_ctx = {_global_rtnl_link_ops};
	__maybe_unused struct __rtnl_link_register_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct rtnl_link_ops);
		*_global_rtnl_link_ops_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct rtnl_link_ops*, (__size), (DEFAULT_GFP_FLAGS));
		if (*_global_rtnl_link_ops_ptr) {
			callee_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(__pos, __msg, __ext, ctx, *_global_rtnl_link_ops_ptr);
		}

	}

	ret = __rtnl_link_register(_global_rtnl_link_ops);

	*__pos = 0;
	{
		if (*_global_rtnl_link_ops_ptr) {
			callee_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(__pos, __msg, __ext, ctx, *_global_rtnl_link_ops_ptr);
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

void ether_setup_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	__maybe_unused struct ether_setup_call_ctx call_ctx = {dev};
	__maybe_unused struct ether_setup_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ether_setup__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	ether_setup(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ether_setup__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void eth_validate_addr_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__eth_validate_addr__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	ret = eth_validate_addr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__eth_validate_addr__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned %d!\n", __func__, __LINE__, ret);
	}
}

void eth_mac_addr_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct sockaddr* p = 0;
	struct net_device** dev_ptr = &dev;
	struct sockaddr** p_ptr = &p;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct eth_mac_addr_call_ctx call_ctx = {dev, p};
	__maybe_unused struct eth_mac_addr_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__eth_mac_addr__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*p_ptr = glue_unpack(__pos, __msg, __ext, struct sockaddr*);
		if (*p_ptr) {
			callee_unmarshal_kernel__eth_mac_addr__p__in(__pos, __msg, __ext, ctx, *p_ptr);
		}

	}

	ret = eth_mac_addr(dev, p);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__eth_mac_addr__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*p_ptr) {
			callee_marshal_kernel__eth_mac_addr__p__in(__pos, __msg, __ext, ctx, *p_ptr);
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

void trmp_impl_setup(fptr_setup target, struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct setup_call_ctx call_ctx = {dev};
	__maybe_unused const struct setup_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__setup__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_setup);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__setup__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		const struct net_device_ops *ops = dev->netdev_ops;
    char eth_addr[] = { 0xa6, 0x4f, 0xf0, 0x41, 0xdf, 0x07 };
	printk("%s:%d ndo_ops %p, ops->ndo_init %p, ndo_uinit %p, start_xmit %p, validate_addr %p, set_rx_mode %p, set_mac_addr %p, get_stats64 %p, change_carrier %p\n",
		      __func__, __LINE__, ops, ops->ndo_init,
			ops->ndo_uninit, ops->ndo_start_xmit,
			ops->ndo_validate_addr, ops->ndo_set_rx_mode,
			ops->ndo_set_mac_address, ops->ndo_get_stats64,
			ops->ndo_change_carrier);

    dev->dev_addr = kzalloc(ETH_ALEN, GFP_KERNEL);

    memcpy(dev->dev_addr, eth_addr, ETH_ALEN);

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_setup)
void LCD_TRAMPOLINE_LINKAGE(trmp_setup) trmp_setup(struct net_device* dev)
{
	volatile fptr_impl_setup impl;
	fptr_setup target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_setup);
	impl = trmp_impl_setup;
	return impl(target, dev);
}

void trmp_impl_get_drvinfo(fptr_get_drvinfo target, struct net_device* dev, struct ethtool_drvinfo* info)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	struct ethtool_drvinfo** info_ptr = &info;
	
	__maybe_unused const struct get_drvinfo_call_ctx call_ctx = {dev, info};
	__maybe_unused const struct get_drvinfo_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__get_drvinfo__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *info_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*info_ptr) {
			caller_marshal_kernel__get_drvinfo__info__out(__pos, __msg, __ext, ctx, *info_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_get_drvinfo);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__get_drvinfo__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*info_ptr) {
			caller_unmarshal_kernel__get_drvinfo__info__out(__pos, __msg, __ext, ctx, *info_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_get_drvinfo)
void LCD_TRAMPOLINE_LINKAGE(trmp_get_drvinfo) trmp_get_drvinfo(struct net_device* dev, struct ethtool_drvinfo* info)
{
	volatile fptr_impl_get_drvinfo impl;
	fptr_get_drvinfo target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_get_drvinfo);
	impl = trmp_impl_get_drvinfo;
	return impl(target, dev, info);
}

int trmp_impl_ndo_init(fptr_ndo_init target, struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ndo_init_call_ctx call_ctx = {dev};
	__maybe_unused const struct ndo_init_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_init__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_ndo_init);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_init__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ndo_init)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_init) trmp_ndo_init(struct net_device* dev)
{
	volatile fptr_impl_ndo_init impl;
	fptr_ndo_init target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_init);
	impl = trmp_impl_ndo_init;
	return impl(target, dev);
}

void trmp_impl_ndo_uninit(fptr_ndo_uninit target, struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct ndo_uninit_call_ctx call_ctx = {dev};
	__maybe_unused const struct ndo_uninit_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_uninit__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_ndo_uninit);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_uninit__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_ndo_uninit)
void LCD_TRAMPOLINE_LINKAGE(trmp_ndo_uninit) trmp_ndo_uninit(struct net_device* dev)
{
	volatile fptr_impl_ndo_uninit impl;
	fptr_ndo_uninit target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_uninit);
	impl = trmp_impl_ndo_uninit;
	return impl(target, dev);
}

long long trmp_impl_ndo_start_xmit(fptr_ndo_start_xmit target, struct sk_buff* skb, struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	struct net_device** dev_ptr = &dev;
	long long ret = 0;
	long long* ret_ptr = &ret;
	
	__maybe_unused const struct ndo_start_xmit_call_ctx call_ctx = {skb, dev};
	__maybe_unused const struct ndo_start_xmit_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__ndo_start_xmit__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_start_xmit__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_ndo_start_xmit);

	*__pos = 0;
	{
		if (*skb_ptr) {
			caller_unmarshal_kernel__ndo_start_xmit__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_start_xmit__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, long long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_ndo_start_xmit)
long long LCD_TRAMPOLINE_LINKAGE(trmp_ndo_start_xmit) trmp_ndo_start_xmit(struct sk_buff* skb, struct net_device* dev)
{
	volatile fptr_impl_ndo_start_xmit impl;
	fptr_ndo_start_xmit target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_start_xmit);
	impl = trmp_impl_ndo_start_xmit;
	return impl(target, skb, dev);
}

void trmp_impl_ndo_set_rx_mode(fptr_ndo_set_rx_mode target, struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct ndo_set_rx_mode_call_ctx call_ctx = {dev};
	__maybe_unused const struct ndo_set_rx_mode_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_set_rx_mode__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_ndo_set_rx_mode);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_set_rx_mode__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_ndo_set_rx_mode)
void LCD_TRAMPOLINE_LINKAGE(trmp_ndo_set_rx_mode) trmp_ndo_set_rx_mode(struct net_device* dev)
{
	volatile fptr_impl_ndo_set_rx_mode impl;
	fptr_ndo_set_rx_mode target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_set_rx_mode);
	impl = trmp_impl_ndo_set_rx_mode;
	return impl(target, dev);
}

int trmp_impl_ndo_validate_addr(fptr_ndo_validate_addr target, struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ndo_validate_addr_call_ctx call_ctx = {dev};
	__maybe_unused const struct ndo_validate_addr_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
    printk("%s, dev->flags 0x%x\n", __func__, dev->flags);
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_validate_addr__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_ndo_validate_addr);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_validate_addr__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ndo_validate_addr)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_validate_addr) trmp_ndo_validate_addr(struct net_device* dev)
{
	volatile fptr_impl_ndo_validate_addr impl;
	fptr_ndo_validate_addr target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_validate_addr);
	impl = trmp_impl_ndo_validate_addr;
	return impl(target, dev);
}

int trmp_impl_ndo_set_mac_address(fptr_ndo_set_mac_address target, struct net_device* dev, void* addr)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	void** addr_ptr = &addr;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ndo_set_mac_address_call_ctx call_ctx = {dev, addr};
	__maybe_unused const struct ndo_set_mac_address_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_set_mac_address__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *addr_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*addr_ptr) {
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_ndo_set_mac_address);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_set_mac_address__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		(void)addr_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_ndo_set_mac_address)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_set_mac_address) trmp_ndo_set_mac_address(struct net_device* dev, void* addr)
{
	volatile fptr_impl_ndo_set_mac_address impl;
	fptr_ndo_set_mac_address target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_set_mac_address);
	impl = trmp_impl_ndo_set_mac_address;
	return impl(target, dev, addr);
}

struct rtnl_link_stats64* trmp_impl_ndo_get_stats64(fptr_ndo_get_stats64 target, struct net_device* dev, struct rtnl_link_stats64* stats)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	struct rtnl_link_stats64** stats_ptr = &stats;
	struct rtnl_link_stats64* ret = 0;
	struct rtnl_link_stats64** ret_ptr = &ret;
	
	__maybe_unused const struct ndo_get_stats64_call_ctx call_ctx = {dev, stats};
	__maybe_unused const struct ndo_get_stats64_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_get_stats64__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *stats_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*stats_ptr) {
			caller_marshal_kernel__ndo_get_stats64__stats__in(__pos, __msg, __ext, ctx, *stats_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_ndo_get_stats64);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_get_stats64__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*stats_ptr) {
			caller_unmarshal_kernel__ndo_get_stats64__stats__in(__pos, __msg, __ext, ctx, *stats_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, struct rtnl_link_stats64*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_ndo_get_stats64)
struct rtnl_link_stats64* LCD_TRAMPOLINE_LINKAGE(trmp_ndo_get_stats64) trmp_ndo_get_stats64(struct net_device* dev, struct rtnl_link_stats64* stats)
{
	volatile fptr_impl_ndo_get_stats64 impl;
	fptr_ndo_get_stats64 target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_get_stats64);
	impl = trmp_impl_ndo_get_stats64;
	return impl(target, dev, stats);
}

int trmp_impl_ndo_change_carrier(fptr_ndo_change_carrier target, struct net_device* dev, bool new_carrier)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	bool* new_carrier_ptr = &new_carrier;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct ndo_change_carrier_call_ctx call_ctx = {dev, new_carrier};
	__maybe_unused const struct ndo_change_carrier_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_change_carrier__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *new_carrier_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_ndo_change_carrier);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_change_carrier__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_ndo_change_carrier)
int LCD_TRAMPOLINE_LINKAGE(trmp_ndo_change_carrier) trmp_ndo_change_carrier(struct net_device* dev, bool new_carrier)
{
	volatile fptr_impl_ndo_change_carrier impl;
	fptr_ndo_change_carrier target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_ndo_change_carrier);
	impl = trmp_impl_ndo_change_carrier;
	return impl(target, dev, new_carrier);
}

int trmp_impl_validate(fptr_validate target, struct nlattr** tb, struct nlattr** data)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct nlattr*** tb_ptr = &tb;
	struct nlattr*** data_ptr = &data;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct validate_call_ctx call_ctx = {tb, data};
	__maybe_unused const struct validate_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *tb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*tb_ptr) {
			__maybe_unused const void* __adjusted = **tb_ptr;
			glue_pack_shadow(__pos, __msg, __ext, __adjusted);
			if (**tb_ptr) {
				caller_marshal_kernel__validate__tb__in(__pos, __msg, __ext, ctx, **tb_ptr);
			}

		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			__maybe_unused const void* __adjusted = **data_ptr;
			glue_pack_shadow(__pos, __msg, __ext, __adjusted);
			if (**data_ptr) {
				caller_marshal_kernel__validate__data__in(__pos, __msg, __ext, ctx, **data_ptr);
			}

		}

	}

	glue_call_client(__pos, __msg, RPC_ID_validate);

	*__pos = 0;
	{
		if (*tb_ptr) {
			if (**tb_ptr) {
				caller_unmarshal_kernel__validate__tb__in(__pos, __msg, __ext, ctx, **tb_ptr);
			}

		}

	}

	{
		if (*data_ptr) {
			if (**data_ptr) {
				caller_unmarshal_kernel__validate__data__in(__pos, __msg, __ext, ctx, **data_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_validate)
int LCD_TRAMPOLINE_LINKAGE(trmp_validate) trmp_validate(struct nlattr** tb, struct nlattr** data)
{
	volatile fptr_impl_validate impl;
	fptr_validate target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_validate);
	impl = trmp_impl_validate;
	return impl(target, tb, data);
}

void get_jiffies_callee(struct fipc_message* __msg, struct ext_registers* __ext)
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

	ret = get_jiffies();

	*__pos = 0;
	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void get_random_bytes_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	char* buf = 0;
	int nbytes = 0;
	char** buf_ptr = &buf;
	int* nbytes_ptr = &nbytes;
	
	__maybe_unused struct get_random_bytes_call_ctx call_ctx = {buf, nbytes};
	__maybe_unused struct get_random_bytes_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*buf_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char*, (__size), (DEFAULT_GFP_FLAGS));
		(void)buf_ptr;
	}

	{
		*nbytes_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	get_random_bytes(buf, nbytes);

	*__pos = 0;
	{
		if (*buf_ptr) {
			size_t i, len = (nbytes);
			char* array = *buf_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int try_dispatch(enum RPC_ID id, struct fipc_message* __msg, struct ext_registers* __ext)
{
	switch(id) {
	case RPC_ID_shared_mem_init:
		glue_user_trace("shared_mem_init\n");
		shared_mem_init_callee(__msg, __ext);
		break;

	case RPC_ID_netif_carrier_on:
		glue_user_trace("netif_carrier_on\n");
		netif_carrier_on_callee(__msg, __ext);
		break;

	case RPC_ID_netif_carrier_off:
		glue_user_trace("netif_carrier_off\n");
		netif_carrier_off_callee(__msg, __ext);
		break;

	case RPC_ID_register_netdevice:
		glue_user_trace("register_netdevice\n");
		register_netdevice_callee(__msg, __ext);
		break;

	case RPC_ID_consume_skb:
		glue_user_trace("consume_skb\n");
		consume_skb_callee(__msg, __ext);
		break;

	case RPC_ID_alloc_netdev_mqs:
		glue_user_trace("alloc_netdev_mqs\n");
		alloc_netdev_mqs_callee(__msg, __ext);
		break;

	case RPC_ID_free_netdev:
		glue_user_trace("free_netdev\n");
		free_netdev_callee(__msg, __ext);
		break;

	case RPC_ID_rtnl_lock:
		glue_user_trace("rtnl_lock\n");
		rtnl_lock_callee(__msg, __ext);
		break;

	case RPC_ID_rtnl_unlock:
		glue_user_trace("rtnl_unlock\n");
		rtnl_unlock_callee(__msg, __ext);
		break;

	case RPC_ID_rtnl_link_unregister:
		glue_user_trace("rtnl_link_unregister\n");
		rtnl_link_unregister_callee(__msg, __ext);
		break;

	case RPC_ID___rtnl_link_unregister:
		glue_user_trace("__rtnl_link_unregister\n");
		__rtnl_link_unregister_callee(__msg, __ext);
		break;

	case RPC_ID___rtnl_link_register:
		glue_user_trace("__rtnl_link_register\n");
		__rtnl_link_register_callee(__msg, __ext);
		break;

	case RPC_ID_ether_setup:
		glue_user_trace("ether_setup\n");
		ether_setup_callee(__msg, __ext);
		break;

	case RPC_ID_eth_validate_addr:
		glue_user_trace("eth_validate_addr\n");
		eth_validate_addr_callee(__msg, __ext);
		break;

	case RPC_ID_eth_mac_addr:
		glue_user_trace("eth_mac_addr\n");
		eth_mac_addr_callee(__msg, __ext);
		break;

	case RPC_ID_get_jiffies:
		glue_user_trace("get_jiffies\n");
		get_jiffies_callee(__msg, __ext);
		break;

	case RPC_ID_get_random_bytes:
		glue_user_trace("get_random_bytes\n");
		get_random_bytes_callee(__msg, __ext);
		break;

	default:
		return 0;
	}

	return 1;
}

