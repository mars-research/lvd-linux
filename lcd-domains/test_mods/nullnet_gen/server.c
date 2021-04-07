#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void netif_carrier_on_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netif_carrier_on__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	netif_carrier_on(dev);

	*pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netif_carrier_on__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void netif_carrier_off_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__netif_carrier_off__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	netif_carrier_off(dev);

	*pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__netif_carrier_off__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void register_netdevice_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__register_netdevice__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	ret = register_netdevice(dev);

	*pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__register_netdevice__dev__in(pos, msg, ext, *dev_ptr);
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

void consume_skb_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct sk_buff* skb = 0;
	struct sk_buff** skb_ptr = &skb;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack(pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__consume_skb__skb__in(pos, msg, ext, *skb_ptr);
		}

	}

	consume_skb(skb);

	*pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel__consume_skb__skb__in(pos, msg, ext, *skb_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void alloc_netdev_mqs_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

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
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sizeof_priv_ptr = glue_unpack(pos, msg, ext, int);
	}

	{
		*name_ptr = glue_unpack_new_shadow(pos, msg, ext, char const*, sizeof(char) * glue_peek(pos, msg, ext));
		if (*name_ptr) {
			char* writable = (char*)*name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(pos, msg, ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(pos, msg, ext, char);
			}

		}

	}

	{
		*name_assign_type_ptr = glue_unpack(pos, msg, ext, unsigned char);
	}

	{
		*setup_ptr = glue_unpack_rpc_ptr(pos, msg, ext, setup);
	}

	{
		*txqs_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*rxqs_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	ret = alloc_netdev_mqs(sizeof_priv, name, name_assign_type, setup, txqs, rxqs);

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
		glue_pack(pos, msg, ext, *ret_ptr);
		if (*ret_ptr) {
			callee_marshal_kernel__alloc_netdev_mqs__ret_net_device__out(pos, msg, ext, *ret_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void free_netdev_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__free_netdev__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	free_netdev(dev);

	*pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__free_netdev__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_lock_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	rtnl_lock();

	*pos = 0;
	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_unlock_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	rtnl_unlock();

	*pos = 0;
	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rtnl_link_unregister_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct rtnl_link_ops* _global_rtnl_link_ops = 0;
	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*_global_rtnl_link_ops_ptr = glue_unpack_shadow(pos, msg, ext, struct rtnl_link_ops*);
		if (*_global_rtnl_link_ops_ptr) {
			callee_unmarshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, *_global_rtnl_link_ops_ptr);
		}

	}

	rtnl_link_unregister(_global_rtnl_link_ops);

	*pos = 0;
	{
		if (*_global_rtnl_link_ops_ptr) {
			callee_marshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, *_global_rtnl_link_ops_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __rtnl_link_unregister_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct rtnl_link_ops* _global_rtnl_link_ops = 0;
	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*_global_rtnl_link_ops_ptr = glue_unpack_shadow(pos, msg, ext, struct rtnl_link_ops*);
		if (*_global_rtnl_link_ops_ptr) {
			callee_unmarshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, *_global_rtnl_link_ops_ptr);
		}

	}

	__rtnl_link_unregister(_global_rtnl_link_ops);

	*pos = 0;
	{
		if (*_global_rtnl_link_ops_ptr) {
			callee_marshal_kernel___global_rtnl_link_ops__in(pos, msg, ext, *_global_rtnl_link_ops_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __rtnl_link_register_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct rtnl_link_ops* _global_rtnl_link_ops = 0;
	struct rtnl_link_ops** _global_rtnl_link_ops_ptr = &_global_rtnl_link_ops;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*_global_rtnl_link_ops_ptr = glue_unpack_new_shadow(pos, msg, ext, struct rtnl_link_ops*, sizeof(struct rtnl_link_ops));
		if (*_global_rtnl_link_ops_ptr) {
			callee_unmarshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(pos, msg, ext, *_global_rtnl_link_ops_ptr);
		}

	}

	ret = __rtnl_link_register(_global_rtnl_link_ops);

	*pos = 0;
	{
		if (*_global_rtnl_link_ops_ptr) {
			callee_marshal_kernel____rtnl_link_register___global_rtnl_link_ops__in(pos, msg, ext, *_global_rtnl_link_ops_ptr);
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

void ether_setup_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__ether_setup__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	ether_setup(dev);

	*pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__ether_setup__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void eth_validate_addr_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__eth_validate_addr__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	ret = eth_validate_addr(dev);

	*pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__eth_validate_addr__dev__in(pos, msg, ext, *dev_ptr);
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

void eth_mac_addr_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device* dev = 0;
	struct sockaddr* p = 0;
	struct net_device** dev_ptr = &dev;
	struct sockaddr** p_ptr = &p;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__eth_mac_addr__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		*p_ptr = glue_unpack(pos, msg, ext, struct sockaddr*);
		if (*p_ptr) {
			callee_unmarshal_kernel__eth_mac_addr__p__in(pos, msg, ext, *p_ptr);
		}

	}

	ret = eth_mac_addr(dev, p);

	*pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__eth_mac_addr__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		if (*p_ptr) {
			callee_marshal_kernel__eth_mac_addr__p__in(pos, msg, ext, *p_ptr);
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

void trmp_impl_setup(fptr_setup target, struct net_device* dev)
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

	glue_pack(pos, msg, ext, target);
	{
		glue_pack(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__setup__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	glue_call_client(pos, msg, RPC_ID_setup);

	*pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__setup__dev__in(pos, msg, ext, *dev_ptr);
		}

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
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	struct ethtool_drvinfo** info_ptr = &info;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(pos, msg, ext, target);
	{
		glue_pack(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__get_drvinfo__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *info_ptr);
		if (*info_ptr) {
			caller_marshal_kernel__get_drvinfo__info__out(pos, msg, ext, *info_ptr);
		}

	}

	glue_call_client(pos, msg, RPC_ID_get_drvinfo);

	*pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__get_drvinfo__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		if (*info_ptr) {
			caller_unmarshal_kernel__get_drvinfo__info__out(pos, msg, ext, *info_ptr);
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

	glue_pack(pos, msg, ext, target);
	{
		glue_pack(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_init__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	glue_call_client(pos, msg, RPC_ID_ndo_init);

	*pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_init__dev__in(pos, msg, ext, *dev_ptr);
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

	glue_pack(pos, msg, ext, target);
	{
		glue_pack(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_uninit__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	glue_call_client(pos, msg, RPC_ID_ndo_uninit);

	*pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_uninit__dev__in(pos, msg, ext, *dev_ptr);
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
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	struct net_device** dev_ptr = &dev;
	long long ret = 0;
	long long* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(pos, msg, ext, target);
	{
		glue_pack(pos, msg, ext, *skb_ptr);
		if (*skb_ptr) {
			caller_marshal_kernel__ndo_start_xmit__skb__in(pos, msg, ext, *skb_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_start_xmit__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	glue_call_client(pos, msg, RPC_ID_ndo_start_xmit);

	*pos = 0;
	{
		if (*skb_ptr) {
			caller_unmarshal_kernel__ndo_start_xmit__skb__in(pos, msg, ext, *skb_ptr);
		}

	}

	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_start_xmit__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(pos, msg, ext, long long);
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

	glue_pack(pos, msg, ext, target);
	{
		glue_pack(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_set_rx_mode__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	glue_call_client(pos, msg, RPC_ID_ndo_set_rx_mode);

	*pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_set_rx_mode__dev__in(pos, msg, ext, *dev_ptr);
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

struct rtnl_link_stats64* trmp_impl_ndo_get_stats64(fptr_ndo_get_stats64 target, struct net_device* dev, struct rtnl_link_stats64* stats)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	struct rtnl_link_stats64** stats_ptr = &stats;
	struct rtnl_link_stats64* ret = 0;
	struct rtnl_link_stats64** ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(pos, msg, ext, target);
	{
		glue_pack(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_get_stats64__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *stats_ptr);
		if (*stats_ptr) {
			caller_marshal_kernel__ndo_get_stats64__stats__in(pos, msg, ext, *stats_ptr);
		}

	}

	glue_call_client(pos, msg, RPC_ID_ndo_get_stats64);

	*pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_get_stats64__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		if (*stats_ptr) {
			caller_unmarshal_kernel__ndo_get_stats64__stats__in(pos, msg, ext, *stats_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(pos, msg, ext, struct rtnl_link_stats64*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(pos, msg, ext, *ret_ptr);
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
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	bool* new_carrier_ptr = &new_carrier;
	int ret = 0;
	int* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(pos, msg, ext, target);
	{
		glue_pack(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__ndo_change_carrier__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *new_carrier_ptr);
	}

	glue_call_client(pos, msg, RPC_ID_ndo_change_carrier);

	*pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__ndo_change_carrier__dev__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(pos, msg, ext, int);
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
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct nlattr*** tb_ptr = &tb;
	struct nlattr*** data_ptr = &data;
	int ret = 0;
	int* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(pos, msg, ext, target);
	{
		glue_pack_shadow(pos, msg, ext, *tb_ptr);
		if (*tb_ptr) {
			glue_pack_shadow(pos, msg, ext, **tb_ptr);
			if (**tb_ptr) {
				caller_marshal_kernel__validate__tb__in(pos, msg, ext, **tb_ptr);
			}

		}

	}

	{
		glue_pack_shadow(pos, msg, ext, *data_ptr);
		if (*data_ptr) {
			glue_pack_shadow(pos, msg, ext, **data_ptr);
			if (**data_ptr) {
				caller_marshal_kernel__validate__data__in(pos, msg, ext, **data_ptr);
			}

		}

	}

	glue_call_client(pos, msg, RPC_ID_validate);

	*pos = 0;
	{
		if (*tb_ptr) {
			if (**tb_ptr) {
				caller_unmarshal_kernel__validate__tb__in(pos, msg, ext, **tb_ptr);
			}

		}

	}

	{
		if (*data_ptr) {
			if (**data_ptr) {
				caller_unmarshal_kernel__validate__data__in(pos, msg, ext, **data_ptr);
			}

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

LCD_TRAMPOLINE_DATA(trmp_validate)
int LCD_TRAMPOLINE_LINKAGE(trmp_validate) trmp_validate(struct nlattr** tb, struct nlattr** data)
{
	volatile fptr_impl_validate impl;
	fptr_validate target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_validate);
	impl = trmp_impl_validate;
	return impl(target, tb, data);
}

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext)
{
	switch(id) {
	case RPC_ID_shared_mem_init:
		glue_user_trace("shared_mem_init\n");
		shared_mem_init_callee(msg, ext);
		break;

	case RPC_ID_netif_carrier_on:
		glue_user_trace("netif_carrier_on\n");
		netif_carrier_on_callee(msg, ext);
		break;

	case RPC_ID_netif_carrier_off:
		glue_user_trace("netif_carrier_off\n");
		netif_carrier_off_callee(msg, ext);
		break;

	case RPC_ID_register_netdevice:
		glue_user_trace("register_netdevice\n");
		register_netdevice_callee(msg, ext);
		break;

	case RPC_ID_consume_skb:
		glue_user_trace("consume_skb\n");
		consume_skb_callee(msg, ext);
		break;

	case RPC_ID_alloc_netdev_mqs:
		glue_user_trace("alloc_netdev_mqs\n");
		alloc_netdev_mqs_callee(msg, ext);
		break;

	case RPC_ID_free_netdev:
		glue_user_trace("free_netdev\n");
		free_netdev_callee(msg, ext);
		break;

	case RPC_ID_rtnl_lock:
		glue_user_trace("rtnl_lock\n");
		rtnl_lock_callee(msg, ext);
		break;

	case RPC_ID_rtnl_unlock:
		glue_user_trace("rtnl_unlock\n");
		rtnl_unlock_callee(msg, ext);
		break;

	case RPC_ID_rtnl_link_unregister:
		glue_user_trace("rtnl_link_unregister\n");
		rtnl_link_unregister_callee(msg, ext);
		break;

	case RPC_ID___rtnl_link_unregister:
		glue_user_trace("__rtnl_link_unregister\n");
		__rtnl_link_unregister_callee(msg, ext);
		break;

	case RPC_ID___rtnl_link_register:
		glue_user_trace("__rtnl_link_register\n");
		__rtnl_link_register_callee(msg, ext);
		break;

	case RPC_ID_ether_setup:
		glue_user_trace("ether_setup\n");
		ether_setup_callee(msg, ext);
		break;

	case RPC_ID_eth_validate_addr:
		glue_user_trace("eth_validate_addr\n");
		eth_validate_addr_callee(msg, ext);
		break;

	case RPC_ID_eth_mac_addr:
		glue_user_trace("eth_mac_addr\n");
		eth_mac_addr_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

