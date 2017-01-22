#ifndef _GLUE_HELPER_H_
#define _GLUE_HELPER_H_

#include <linux/kthread.h>

#include <linux/netdevice.h>
#include <net/rtnetlink.h>
#include <uapi/linux/if_link.h>
#include <uapi/linux/netlink.h>
#include <linux/skbuff.h>

#include <libcap.h>
#include <libfipc.h>
#include <thc_ipc.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>

enum dispatch_t {
	REGISTER_NETDEVICE,
	UNREGISTER_NETDEVICE,
	ETHER_SETUP,
	ETH_MAC_ADDR,
	ETH_VALIDATE_ADDR,
	FREE_NETDEV,
	NETIF_CARRIER_OFF,
	NETIF_CARRIER_ON,
	__RTNL_LINK_REGISTER,
	__RTNL_LINK_UNREGISTER,
	RTNL_LINK_UNREGISTER,
	ALLOC_NETDEV_MQS,
	CONSUME_SKB,
	NDO_INIT,
	NDO_UNINIT,
	NDO_START_XMIT,
	NDO_VALIDATE_ADDR,
	NDO_SET_RX_MODE,
	NDO_SET_MAC_ADDRESS,
	NDO_GET_STATS64,
	NDO_CHANGE_CARRIER,
	VALIDATE,
	SETUP
};

#define PMFS_ASYNC_RPC_BUFFER_ORDER 12
/* CONTAINERS 	---------- */
struct net_device_container {
	struct net_device net_device;
	cptr_t other_ref;
	cptr_t my_ref;
};
struct net_device_ops_container {
	struct net_device_ops net_device_ops;
	cptr_t other_ref;
	cptr_t my_ref;
};
struct nlattr_container {
	struct nlattr nlattr;
	cptr_t other_ref;
	cptr_t my_ref;
};
struct rtnl_link_ops_container {
	struct rtnl_link_ops rtnl_link_ops;
	cptr_t other_ref;
	cptr_t my_ref;
	void *fs_info;
};
struct rtnl_link_stats64_container {
	struct rtnl_link_stats64 rtnl_link_stats64;
	cptr_t other_ref;
	cptr_t my_ref;
};
struct setup_container {
	void ( *setup )(struct net_device *);
	cptr_t other_ref;
	cptr_t my_ref;
};
struct sk_buff_container {
	struct sk_buff sk_buff;
	cptr_t other_ref;
	cptr_t my_ref;
};


/* CSPACES ------------------------------------------------------------ */
int glue_cap_init(void);

int glue_cap_create(struct glue_cspace **cspace);

void glue_cap_destroy(struct glue_cspace *cspace);

void glue_cap_exit(void);

void glue_cap_remove(
	struct glue_cspace *cspace, 
	cptr_t c);

int glue_cap_insert_net_device_type(
	struct glue_cspace *cspace, 
	struct net_device_container *net_device_container,
	cptr_t *c_out);

int glue_cap_insert_net_device_ops_type(
	struct glue_cspace *cspace, 
	struct net_device_ops_container *net_device_ops_container,
	cptr_t *c_out);

int glue_cap_insert_nlattr_type(
	struct glue_cspace *cspace, 
	struct nlattr_container *nlattr_container,
	cptr_t *c_out);

int glue_cap_insert_rtnl_link_ops_type(
	struct glue_cspace *cspace, 
	struct rtnl_link_ops_container *rtnl_link_ops_container,
	cptr_t *c_out);

int glue_cap_insert_rtnl_link_stats64_type(
	struct glue_cspace *cspace, 
	struct rtnl_link_stats64_container *rtnl_link_stats64_container,
	cptr_t *c_out);

int glue_cap_insert_sk_buff_type(
	struct glue_cspace *cspace, 
	struct sk_buff_container *sk_buff_container,
	cptr_t *c_out);

int glue_cap_insert_setup_type(
	struct glue_cspace *cspace, 
	struct setup_container *setup_container,
	cptr_t *c_out);

int glue_cap_lookup_net_device_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct net_device_container **net_device_container);

int glue_cap_lookup_net_device_ops_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct net_device_ops_container **net_device_ops_container);


int glue_cap_lookup_nlattr_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct nlattr_container **nlattr_container);

int glue_cap_lookup_rtnl_link_ops_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct rtnl_link_ops_container **rtnl_link_ops_container);

int glue_cap_lookup_rtnl_link_stats64_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct rtnl_link_stats64_container **rtnl_link_stats64_container);

int glue_cap_lookup_sk_buff_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct sk_buff_container **sk_buff_container);

int glue_cap_lookup_setup_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct setup_container **setup_container);

/* ASYNC HELPERS -------------------------------------------------- */

static inline
int
async_msg_get_fn_type(struct fipc_message *msg)
{
	return fipc_get_flags(msg) >> THC_RESERVED_MSG_FLAG_BITS;
}

static inline
void
async_msg_set_fn_type(struct fipc_message *msg, int type)
{
	uint32_t flags = fipc_get_flags(msg);
	/* ensure type is in range */
	type &= (1 << (32 - THC_RESERVED_MSG_FLAG_BITS)) - 1;
	/* erase old type */
	flags &= ((1 << THC_RESERVED_MSG_FLAG_BITS) - 1);
	/* install new type */
	flags |= (type << THC_RESERVED_MSG_FLAG_BITS);
	fipc_set_flags(msg, flags);
}

static inline
int
async_msg_blocking_send_start(struct thc_channel *chnl, 
			struct fipc_message **out)
{
	int ret;
	for (;;) {
		/* Poll until we get a free slot or error */
		ret = fipc_send_msg_start(thc_channel_to_fipc(chnl), out);
		if (!ret || ret != -EWOULDBLOCK)
			return ret;
		cpu_relax();
		if (kthread_should_stop())
			return -EIO;
	}
}

#endif /* _GLUE_HELPER_H_ */
