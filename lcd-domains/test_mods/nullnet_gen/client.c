#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void netif_carrier_on(struct net_device* dev)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack_shadow(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_carrier_on__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID_netif_carrier_on);

	*pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_carrier_on__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_carrier_off(struct net_device* dev)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack_shadow(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__netif_carrier_off__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID_netif_carrier_off);

	*pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__netif_carrier_off__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int register_netdevice(struct net_device* dev)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack_shadow(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__register_netdevice__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID_register_netdevice);

	*pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__register_netdevice__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void consume_skb(struct sk_buff* skb)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack_shadow(pos, msg, ext, *skb_ptr);
		if (*skb_ptr) {
			caller_marshal_kernel__consume_skb__skb__in(pos, msg, ext, *skb_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID_consume_skb);

	*pos = 0;
	{
		if (*skb_ptr) {
			caller_unmarshal_kernel__consume_skb__skb__in(pos, msg, ext, *skb_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct net_device* alloc_netdev_mqs(int sizeof_priv, char const* name, unsigned char name_assign_type, fptr_setup setup, unsigned int txqs, unsigned int rxqs)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	int* sizeof_priv_ptr = &sizeof_priv;
	char const** name_ptr = &name;
	unsigned char* name_assign_type_ptr = &name_assign_type;
	fptr_setup* setup_ptr = &setup;
	unsigned int* txqs_ptr = &txqs;
	unsigned int* rxqs_ptr = &rxqs;
	struct net_device* ret = 0;
	struct net_device** ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(pos, msg, ext, *sizeof_priv_ptr);
	}

	{
		glue_pack(pos, msg, ext, *name_ptr);
		if (*name_ptr) {
			size_t i, len;
			char const* array = *name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(pos, msg, ext, *element);
			}

		}

	}

	{
		glue_pack(pos, msg, ext, *name_assign_type_ptr);
	}

	{
		glue_pack(pos, msg, ext, *setup_ptr);
	}

	{
		glue_pack(pos, msg, ext, *txqs_ptr);
	}

	{
		glue_pack(pos, msg, ext, *rxqs_ptr);
	}

	glue_call_server(pos, msg, RPC_ID_alloc_netdev_mqs);

	*pos = 0;
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
		*ret_ptr = glue_unpack_shadow(pos, msg, ext, struct net_device*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__alloc_netdev_mqs__ret_net_device__out(pos, msg, ext, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void free_netdev(struct net_device* dev)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack_shadow(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__free_netdev__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID_free_netdev);

	*pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__free_netdev__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_lock(void)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(pos, msg, RPC_ID_rtnl_lock);

	*pos = 0;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_unlock(void)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(pos, msg, RPC_ID_rtnl_unlock);

	*pos = 0;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_link_unregister(struct rtnl_link_ops* _global_rtnl_link_ops)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(pos, msg, ext, *_global_rtnl_link_ops_ptr);
		if (*_global_rtnl_link_ops_ptr) {
			caller_marshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, *_global_rtnl_link_ops_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID_rtnl_link_unregister);

	*pos = 0;
	{
		if (*_global_rtnl_link_ops_ptr) {
			caller_unmarshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, *_global_rtnl_link_ops_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __rtnl_link_unregister(struct rtnl_link_ops* _global_rtnl_link_ops)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(pos, msg, ext, *_global_rtnl_link_ops_ptr);
		if (*_global_rtnl_link_ops_ptr) {
			caller_marshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, *_global_rtnl_link_ops_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID___rtnl_link_unregister);

	*pos = 0;
	{
		if (*_global_rtnl_link_ops_ptr) {
			caller_unmarshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, *_global_rtnl_link_ops_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int __rtnl_link_register(struct rtnl_link_ops* _global_rtnl_link_ops)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	int ret = 0;
	int* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(pos, msg, ext, *_global_rtnl_link_ops_ptr);
		if (*_global_rtnl_link_ops_ptr) {
			caller_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(pos, msg, ext, *_global_rtnl_link_ops_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID___rtnl_link_register);

	*pos = 0;
	{
		if (*_global_rtnl_link_ops_ptr) {
			caller_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(pos, msg, ext, *_global_rtnl_link_ops_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void ether_setup(struct net_device* dev)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack_shadow(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__ether_setup__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID_ether_setup);

	*pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ether_setup__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int eth_validate_addr(struct net_device* dev)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack_shadow(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__eth_validate_addr__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID_eth_validate_addr);

	*pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__eth_validate_addr__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int eth_mac_addr(struct net_device* dev, void* p)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	struct sockaddr** p_ptr = (struct sockaddr **) &p;
	int ret = 0;
	int* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack_shadow(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__eth_mac_addr__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		glue_pack_shadow(pos, msg, ext, *p_ptr);
		if (*p_ptr) {
			caller_marshal_kernel__eth_mac_addr__p__in(pos, msg, ext, *p_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID_eth_mac_addr);

	*pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__eth_mac_addr__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		if (*p_ptr) {
			caller_unmarshal_kernel__eth_mac_addr__p__in(pos, msg, ext, *p_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void setup_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	fptr_setup function_ptr = glue_unpack(pos, msg, ext, fptr_setup);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_new_shadow(pos, msg, ext, struct net_device*, sizeof(struct net_device));
		if (*dev_ptr) {
			callee_unmarshal_kernel__setup__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	function_ptr(dev);

	*pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__setup__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void get_drvinfo_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	fptr_get_drvinfo function_ptr = glue_unpack(pos, msg, ext, fptr_get_drvinfo);
	struct net_device* dev = 0;
	struct ethtool_drvinfo* info = 0;
	struct net_device** dev_ptr = &dev;
	struct ethtool_drvinfo** info_ptr = &info;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__get_drvinfo__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		*info_ptr = glue_unpack_new_shadow(pos, msg, ext, struct ethtool_drvinfo*, sizeof(struct ethtool_drvinfo));
		if (*info_ptr) {
			callee_unmarshal_kernel__get_drvinfo__info__out(pos, msg, ext, *info_ptr);
		}

	}

	function_ptr(dev, info);

	*pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__get_drvinfo__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		if (*info_ptr) {
			callee_marshal_kernel__get_drvinfo__info__out(pos, msg, ext, *info_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_init_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	fptr_ndo_init function_ptr = glue_unpack(pos, msg, ext, fptr_ndo_init);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_init__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	ret = function_ptr(dev);

	*pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_init__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_uninit_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	fptr_ndo_uninit function_ptr = glue_unpack(pos, msg, ext, fptr_ndo_uninit);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_uninit__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	function_ptr(dev);

	*pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_uninit__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_start_xmit_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	fptr_ndo_start_xmit function_ptr = glue_unpack(pos, msg, ext, fptr_ndo_start_xmit);
	struct sk_buff* skb = 0;
	struct net_device* dev = 0;
	struct sk_buff** skb_ptr = &skb;
	struct net_device** dev_ptr = &dev;
	long long ret = 0;
	long long* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack_new_shadow(pos, msg, ext, struct sk_buff*, sizeof(struct sk_buff));
		if (*skb_ptr) {
			callee_unmarshal_kernel__ndo_start_xmit__skb__in(pos, msg, ext, *skb_ptr);
		}

	}

	{
		*dev_ptr = glue_unpack_shadow(pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_start_xmit__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	ret = function_ptr(skb, dev);

	*pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel__ndo_start_xmit__skb__in(pos, msg, ext, *skb_ptr);
		}

	}

	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_start_xmit__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	// FIXME: Until dealloc is supported
	kfree(*skb_ptr);
}

void ndo_set_rx_mode_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	fptr_ndo_set_rx_mode function_ptr = glue_unpack(pos, msg, ext, fptr_ndo_set_rx_mode);
	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_set_rx_mode__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	function_ptr(dev);

	*pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_set_rx_mode__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_get_stats64_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	fptr_ndo_get_stats64 function_ptr = glue_unpack(pos, msg, ext, fptr_ndo_get_stats64);
	struct net_device* dev = 0;
	struct rtnl_link_stats64* stats = 0;
	struct net_device** dev_ptr = &dev;
	struct rtnl_link_stats64** stats_ptr = &stats;
	struct rtnl_link_stats64* ret = 0;
	struct rtnl_link_stats64** ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_get_stats64__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		*stats_ptr = glue_unpack_new_shadow(pos, msg, ext, struct rtnl_link_stats64*, sizeof(struct rtnl_link_stats64));
		if (*stats_ptr) {
			callee_unmarshal_kernel__ndo_get_stats64__stats__in(pos, msg, ext, *stats_ptr);
		}

	}

	ret = function_ptr(dev, stats);

	*pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_get_stats64__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		if (*stats_ptr) {
			callee_marshal_kernel__ndo_get_stats64__stats__in(pos, msg, ext, *stats_ptr);
		}

	}

	{
		glue_pack_shadow(pos, msg, ext, *ret_ptr);
		if (*ret_ptr) {
			callee_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(pos, msg, ext, *ret_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void ndo_change_carrier_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	fptr_ndo_change_carrier function_ptr = glue_unpack(pos, msg, ext, fptr_ndo_change_carrier);
	struct net_device* dev = 0;
	bool new_carrier = 0;
	struct net_device** dev_ptr = &dev;
	bool* new_carrier_ptr = &new_carrier;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ndo_change_carrier__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		*new_carrier_ptr = glue_unpack(pos, msg, ext, bool);
	}

	ret = function_ptr(dev, new_carrier);

	*pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ndo_change_carrier__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
	}

	{
		glue_pack(pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void validate_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	fptr_validate function_ptr = glue_unpack(pos, msg, ext, fptr_validate);
	struct nlattr** tb = 0;
	struct nlattr** data = 0;
	struct nlattr*** tb_ptr = &tb;
	struct nlattr*** data_ptr = &data;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*tb_ptr = glue_unpack(pos, msg, ext, struct nlattr**);
		if (*tb_ptr) {
			**tb_ptr = glue_unpack(pos, msg, ext, struct nlattr*);
			if (**tb_ptr) {
				callee_unmarshal_kernel__validate__tb__in(pos, msg, ext, **tb_ptr);
			}

		}

	}

	{
		*data_ptr = glue_unpack(pos, msg, ext, struct nlattr**);
		if (*data_ptr) {
			**data_ptr = glue_unpack(pos, msg, ext, struct nlattr*);
			if (**data_ptr) {
				callee_unmarshal_kernel__validate__data__in(pos, msg, ext, **data_ptr);
			}

		}

	}

	ret = function_ptr(tb, data);

	*pos = 0;
	{
		if (*tb_ptr) {
			if (**tb_ptr) {
				callee_marshal_kernel__validate__tb__in(pos, msg, ext, **tb_ptr);
			}

		}

	}

	{
		if (*data_ptr) {
			if (**data_ptr) {
				callee_marshal_kernel__validate__data__in(pos, msg, ext, **data_ptr);
			}

		}

	}

	{
		glue_pack(pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *pos;
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

	case RPC_ID_setup:
		glue_user_trace("setup\n");
		setup_callee(msg, ext);
		break;

	case RPC_ID_get_drvinfo:
		glue_user_trace("get_drvinfo\n");
		get_drvinfo_callee(msg, ext);
		break;

	case RPC_ID_ndo_init:
		glue_user_trace("ndo_init\n");
		ndo_init_callee(msg, ext);
		break;

	case RPC_ID_ndo_uninit:
		glue_user_trace("ndo_uninit\n");
		ndo_uninit_callee(msg, ext);
		break;

	case RPC_ID_ndo_start_xmit:
		glue_user_trace("ndo_start_xmit\n");
		ndo_start_xmit_callee(msg, ext);
		break;

	case RPC_ID_ndo_set_rx_mode:
		glue_user_trace("ndo_set_rx_mode\n");
		ndo_set_rx_mode_callee(msg, ext);
		break;

	case RPC_ID_ndo_get_stats64:
		glue_user_trace("ndo_get_stats64\n");
		ndo_get_stats64_callee(msg, ext);
		break;

	case RPC_ID_ndo_change_carrier:
		glue_user_trace("ndo_change_carrier\n");
		ndo_change_carrier_callee(msg, ext);
		break;

	case RPC_ID_validate:
		glue_user_trace("validate\n");
		validate_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

