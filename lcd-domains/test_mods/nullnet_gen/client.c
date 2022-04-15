#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

unsigned long volatile jiffies;

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

int register_netdevice(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct register_netdevice_call_ctx call_ctx = {dev};
	__maybe_unused const struct register_netdevice_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__register_netdevice__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_register_netdevice);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__register_netdevice__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

struct net_device* alloc_netdev_mqs(int sizeof_priv, char const* name, unsigned char name_assign_type, fptr_setup setup, unsigned int txqs, unsigned int rxqs)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* sizeof_priv_ptr = &sizeof_priv;
	char const** name_ptr = &name;
	unsigned char* name_assign_type_ptr = &name_assign_type;
	fptr_setup* setup_ptr = &setup;
	unsigned int* txqs_ptr = &txqs;
	unsigned int* rxqs_ptr = &rxqs;
	struct net_device* ret = 0;
	struct net_device** ret_ptr = &ret;
	
	__maybe_unused const struct alloc_netdev_mqs_call_ctx call_ctx = {sizeof_priv, name, name_assign_type, setup, txqs, rxqs};
	__maybe_unused const struct alloc_netdev_mqs_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *sizeof_priv_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *name_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*name_ptr) {
			size_t i, len;
			char const* array = *name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *name_assign_type_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *setup_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *txqs_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rxqs_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_alloc_netdev_mqs);

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
		*ret_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__alloc_netdev_mqs__ret_net_device__out(__pos, __msg, __ext, ctx, *ret_ptr);
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
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__free_netdev__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_free_netdev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__free_netdev__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

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

void rtnl_link_unregister(struct rtnl_link_ops* _global_rtnl_link_ops)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	
	__maybe_unused const struct rtnl_link_unregister_call_ctx call_ctx = {_global_rtnl_link_ops};
	__maybe_unused const struct rtnl_link_unregister_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *_global_rtnl_link_ops_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*_global_rtnl_link_ops_ptr) {
			caller_marshal_kernel___global_rtnl_link_ops__in(__pos, __msg, __ext, *_global_rtnl_link_ops_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_rtnl_link_unregister);

	*__pos = 0;
	{
		if (*_global_rtnl_link_ops_ptr) {
			caller_unmarshal_kernel___global_rtnl_link_ops__in(__pos, __msg, __ext, *_global_rtnl_link_ops_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __rtnl_link_unregister(struct rtnl_link_ops* _global_rtnl_link_ops)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	
	__maybe_unused const struct __rtnl_link_unregister_call_ctx call_ctx = {_global_rtnl_link_ops};
	__maybe_unused const struct __rtnl_link_unregister_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *_global_rtnl_link_ops_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*_global_rtnl_link_ops_ptr) {
			caller_marshal_kernel___global_rtnl_link_ops__in(__pos, __msg, __ext, *_global_rtnl_link_ops_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID___rtnl_link_unregister);

	*__pos = 0;
	{
		if (*_global_rtnl_link_ops_ptr) {
			caller_unmarshal_kernel___global_rtnl_link_ops__in(__pos, __msg, __ext, *_global_rtnl_link_ops_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int __rtnl_link_register(struct rtnl_link_ops* _global_rtnl_link_ops)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct __rtnl_link_register_call_ctx call_ctx = {_global_rtnl_link_ops};
	__maybe_unused const struct __rtnl_link_register_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *_global_rtnl_link_ops_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*_global_rtnl_link_ops_ptr) {
			caller_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(__pos, __msg, __ext, ctx, *_global_rtnl_link_ops_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID___rtnl_link_register);

	*__pos = 0;
	{
		if (*_global_rtnl_link_ops_ptr) {
			caller_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(__pos, __msg, __ext, ctx, *_global_rtnl_link_ops_ptr);
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

void ether_setup(struct net_device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	__maybe_unused const struct ether_setup_call_ctx call_ctx = {dev};
	__maybe_unused const struct ether_setup_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__ether_setup__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_ether_setup);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ether_setup__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
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

int eth_mac_addr(struct net_device* dev, void* p)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	struct sockaddr** p_ptr = (struct sockaddr **) &p;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct eth_mac_addr_call_ctx call_ctx = {dev, p};
	__maybe_unused const struct eth_mac_addr_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__eth_mac_addr__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *p_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*p_ptr) {
			caller_marshal_kernel__eth_mac_addr__p__in(__pos, __msg, __ext, ctx, *p_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_eth_mac_addr);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__eth_mac_addr__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*p_ptr) {
			caller_unmarshal_kernel__eth_mac_addr__p__in(__pos, __msg, __ext, ctx, *p_ptr);
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

void setup_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_setup function_ptr = glue_unpack(__pos, __msg, __ext, fptr_setup);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	__maybe_unused struct setup_call_ctx call_ctx = {dev};
	__maybe_unused struct setup_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct net_device);
		*dev_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct net_device*, (__size), (DEFAULT_GFP_FLAGS));
		if (*dev_ptr) {
			callee_unmarshal_kernel__setup__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__setup__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void get_drvinfo_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_get_drvinfo function_ptr = glue_unpack(__pos, __msg, __ext, fptr_get_drvinfo);
	struct net_device* dev = 0;
	struct ethtool_drvinfo* info = 0;
	struct net_device** dev_ptr = &dev;
	struct ethtool_drvinfo** info_ptr = &info;
	
	__maybe_unused struct get_drvinfo_call_ctx call_ctx = {dev, info};
	__maybe_unused struct get_drvinfo_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__get_drvinfo__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		size_t __size = sizeof(struct ethtool_drvinfo);
		*info_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct ethtool_drvinfo*, (__size), (DEFAULT_GFP_FLAGS));
		if (*info_ptr) {
			callee_unmarshal_kernel__get_drvinfo__info__out(__pos, __msg, __ext, ctx, *info_ptr);
		}

	}

	function_ptr(dev, info);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__get_drvinfo__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*info_ptr) {
			callee_marshal_kernel__get_drvinfo__info__out(__pos, __msg, __ext, ctx, *info_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_init_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_init function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_init);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ndo_init_call_ctx call_ctx = {dev};
	__maybe_unused struct ndo_init_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_init__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_init__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

void ndo_uninit_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_uninit function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_uninit);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	__maybe_unused struct ndo_uninit_call_ctx call_ctx = {dev};
	__maybe_unused struct ndo_uninit_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_uninit__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_uninit__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

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
	struct net_device* dev = 0;
	struct sk_buff** skb_ptr = &skb;
	struct net_device** dev_ptr = &dev;
	long long ret = 0;
	long long* ret_ptr = &ret;
	
	__maybe_unused struct ndo_start_xmit_call_ctx call_ctx = {skb, dev};
	__maybe_unused struct ndo_start_xmit_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct sk_buff);
		*skb_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct sk_buff*, (__size), (DEFAULT_GFP_FLAGS));
		if (*skb_ptr) {
			callee_unmarshal_kernel__ndo_start_xmit__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_start_xmit__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(skb, dev);

	*__pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel__ndo_start_xmit__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_start_xmit__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

void ndo_set_rx_mode_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_set_rx_mode function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_set_rx_mode);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	__maybe_unused struct ndo_set_rx_mode_call_ctx call_ctx = {dev};
	__maybe_unused struct ndo_set_rx_mode_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_set_rx_mode__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	function_ptr(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_set_rx_mode__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
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

void ndo_set_mac_address_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_set_mac_address function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_set_mac_address);
	struct net_device* dev = 0;
	void* addr = 0;
	struct net_device** dev_ptr = &dev;
	void** addr_ptr = &addr;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ndo_set_mac_address_call_ctx call_ctx = {dev, addr};
	__maybe_unused struct ndo_set_mac_address_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_set_mac_address__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*addr_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*addr_ptr) {
		}

	}

	ret = function_ptr(dev, addr);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_set_mac_address__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

void ndo_get_stats64_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_get_stats64 function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_get_stats64);
	struct net_device* dev = 0;
	struct rtnl_link_stats64* stats = 0;
	struct net_device** dev_ptr = &dev;
	struct rtnl_link_stats64** stats_ptr = &stats;
	struct rtnl_link_stats64* ret = 0;
	struct rtnl_link_stats64** ret_ptr = &ret;
	
	__maybe_unused struct ndo_get_stats64_call_ctx call_ctx = {dev, stats};
	__maybe_unused struct ndo_get_stats64_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_get_stats64__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		size_t __size = sizeof(struct rtnl_link_stats64);
		*stats_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct rtnl_link_stats64*, (__size), (DEFAULT_GFP_FLAGS));
		if (*stats_ptr) {
			callee_unmarshal_kernel__ndo_get_stats64__stats__in(__pos, __msg, __ext, ctx, *stats_ptr);
		}

	}

	ret = function_ptr(dev, stats);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_get_stats64__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

void ndo_change_carrier_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_ndo_change_carrier function_ptr = glue_unpack(__pos, __msg, __ext, fptr_ndo_change_carrier);
	struct net_device* dev = 0;
	bool new_carrier = 0;
	struct net_device** dev_ptr = &dev;
	bool* new_carrier_ptr = &new_carrier;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct ndo_change_carrier_call_ctx call_ctx = {dev, new_carrier};
	__maybe_unused struct ndo_change_carrier_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_change_carrier__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*new_carrier_ptr = glue_unpack(__pos, __msg, __ext, bool);
	}

	ret = function_ptr(dev, new_carrier);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_change_carrier__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

void validate_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_validate function_ptr = glue_unpack(__pos, __msg, __ext, fptr_validate);
	struct nlattr** tb = 0;
	struct nlattr** data = 0;
	struct nlattr*** tb_ptr = &tb;
	struct nlattr*** data_ptr = &data;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct validate_call_ctx call_ctx = {tb, data};
	__maybe_unused struct validate_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*tb_ptr = glue_unpack(__pos, __msg, __ext, struct nlattr**);
		if (*tb_ptr) {
			**tb_ptr = glue_unpack(__pos, __msg, __ext, struct nlattr*);
			if (**tb_ptr) {
				callee_unmarshal_kernel__validate__tb__in(__pos, __msg, __ext, ctx, **tb_ptr);
			}

		}

	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, struct nlattr**);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, struct nlattr*);
			if (**data_ptr) {
				callee_unmarshal_kernel__validate__data__in(__pos, __msg, __ext, ctx, **data_ptr);
			}

		}

	}

	ret = function_ptr(tb, data);

	*__pos = 0;
	{
		if (*tb_ptr) {
			if (**tb_ptr) {
				callee_marshal_kernel__validate__tb__in(__pos, __msg, __ext, ctx, **tb_ptr);
			}

		}

	}

	{
		if (*data_ptr) {
			if (**data_ptr) {
				callee_marshal_kernel__validate__data__in(__pos, __msg, __ext, ctx, **data_ptr);
			}

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
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, __msg, RPC_ID_get_jiffies);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void get_random_bytes(void* buf, int nbytes)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	char** buf_ptr = (char**) &buf;
	int* nbytes_ptr = &nbytes;
	
	__maybe_unused const struct get_random_bytes_call_ctx call_ctx = {buf, nbytes};
	__maybe_unused const struct get_random_bytes_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *buf_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		(void)buf_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *nbytes_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_get_random_bytes);

	*__pos = 0;
	{
		if (*buf_ptr) {
			int i;
			char* array = *buf_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __init_globals(void) {

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

	case RPC_ID_setup:
		glue_user_trace("setup\n");
		setup_callee(__msg, __ext);
		break;

	case RPC_ID_get_drvinfo:
		glue_user_trace("get_drvinfo\n");
		get_drvinfo_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_init:
		glue_user_trace("ndo_init\n");
		ndo_init_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_uninit:
		glue_user_trace("ndo_uninit\n");
		ndo_uninit_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_start_xmit:
		glue_user_trace("ndo_start_xmit\n");
		ndo_start_xmit_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_set_rx_mode:
		glue_user_trace("ndo_set_rx_mode\n");
		ndo_set_rx_mode_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_validate_addr:
		glue_user_trace("ndo_validate_addr\n");
		ndo_validate_addr_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_set_mac_address:
		glue_user_trace("ndo_set_mac_address\n");
		ndo_set_mac_address_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_get_stats64:
		glue_user_trace("ndo_get_stats64\n");
		ndo_get_stats64_callee(__msg, __ext);
		break;

	case RPC_ID_ndo_change_carrier:
		glue_user_trace("ndo_change_carrier\n");
		ndo_change_carrier_callee(__msg, __ext);
		break;

	case RPC_ID_validate:
		glue_user_trace("validate\n");
		validate_callee(__msg, __ext);
		break;

	default:
		return 0;
	}

	return 1;
}

