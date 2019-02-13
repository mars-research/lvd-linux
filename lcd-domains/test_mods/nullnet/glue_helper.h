#ifndef _GLUE_HELPER_H_
#define _GLUE_HELPER_H_

#include <linux/kthread.h>

#include <linux/netdevice.h>
#include <net/rtnetlink.h>
#include <uapi/linux/if_link.h>
#include <uapi/linux/netlink.h>
#include <linux/skbuff.h>

#include <linux/priv_mempool.h>

#include <libcap.h>
#include <libfipc.h>
#include <thc_ipc.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>

enum dispatch_t {
	REGISTER_NETDEVICE = 0,
	UNREGISTER_NETDEVICE = 1,
	ETHER_SETUP = 2,
	ETH_MAC_ADDR =3,
	ETH_VALIDATE_ADDR = 4,
	FREE_NETDEV= 5,
	NETIF_CARRIER_OFF=6,
	NETIF_CARRIER_ON,
	__RTNL_LINK_REGISTER = 8,
	__RTNL_LINK_UNREGISTER,
	RTNL_LINK_UNREGISTER,
	ALLOC_NETDEV_MQS,
	CONSUME_SKB = 12,
	NDO_INIT,
	NDO_UNINIT,
	NDO_START_XMIT,
	NDO_VALIDATE_ADDR = 16,
	NDO_SET_RX_MODE,
	NDO_SET_MAC_ADDRESS,
	NDO_GET_STATS64,
	NDO_CHANGE_CARRIER = 20,
	VALIDATE = 21,
	SETUP = 22,
	TRIGGER_EXIT =23,
	TRIGGER_CLEAN,
	PREP_CHANNEL=25,
};

typedef enum {
	VOLUNTEER_XMIT = 1,
	SHARED_DATA_XMIT = 2,
} xmit_type_t;


struct skbuff_members {
	unsigned int        len, data_len;
	__u16               queue_mapping;
	__u8                xmit_more:1,
			ip_summed:2;
	__u16		network_header;
	__u16			transport_header;
	union {
		__wsum		csum;
		struct {
			__u16	csum_start;
			__u16	csum_offset;
		};
	};
	sk_buff_data_t      tail;
	sk_buff_data_t      end;
	unsigned int 	head_data_off;
	unsigned int        truesize;
} __attribute__((aligned));

#define SKB_LCD_MEMBERS(SKB)	((struct skbuff_members*)((char*)skb_end_pointer(SKB) - (char*)SKB_LCD_MEMBERS_SZ))

#define C(x)	skb_lcd->x = skb->x
#define P(x)	skb->x = skb_lcd->x

#define CONSUME_SKB_SEND_ONLY
#define CONSUME_SKB_NO_HASHING
#define SENDER_DISPATCH_LOOP

//#define NO_AWE
//#define NO_HASHING
//#define NO_MARSHAL

//#define DOUBLE_HASHING
//#define ONE_SLOT

//#define CACHE_ALIGNED __attribute__((aligned(64)))

#define fipc_test_pause()    asm volatile ( "pause\n": : :"memory" );
#define lcd_id		(lcd_get_boot_info()->lcd_id)

#define PMFS_ASYNC_RPC_BUFFER_ORDER 12
// LCD_DEBUG
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
} CACHE_ALIGNED;

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
	/* just store the pointer */
	struct sk_buff *skb;
	/* store the order when volunteered. comes handy during unmap */
	unsigned int skb_ord, skbd_ord;
	cptr_t skb_cptr, skbh_cptr;
	/*
	 * as head, data pointer is different in LCD and KLCD, store it
	 * while crossing the boundary
	 */
	unsigned char *head, *data;
	/* for hashtable insertion */
	struct hlist_node hentry;
	struct cptr other_ref;
	struct cptr my_ref;
	struct task_struct *tsk;
} CACHE_ALIGNED;


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

static inline
int
async_msg_blocking_send_start_0(struct thc_channel *chnl, 
			struct fipc_message **out)
{
	int ret;
	for (;;) {
		/* Poll until we get a free slot or error */
		ret = fipc_send_msg_start_0(thc_channel_to_fipc(chnl), out);
		if (!ret || ret != -EWOULDBLOCK)
			return ret;
		cpu_relax();
		if (kthread_should_stop())
			return -EIO;
	}
}

static inline
int
async_msg_blocking_send_start_inc(struct thc_channel *chnl, 
			struct fipc_message **out, u64 __maybe_unused *counter)
{
	int ret;
	for (;;) {
		(*counter)++;
		/* Poll until we get a free slot or error */
		ret = fipc_send_msg_start(thc_channel_to_fipc(chnl), out);
		if (!ret || ret != -EWOULDBLOCK)
			return ret;
		if (kthread_should_stop())
			return -EIO;
	}
}
extern void *_thc_allocstack(void);
extern int  _thc_schedulecont(struct awe_t *awe) __attribute__((returns_twice));
#if 0
inline
int thc_ipc_recv_response_new(struct thc_channel* channel,
			struct fipc_message** out, uint64_t id )
{
	int ret;
	int received_cookie;
retry:
	while ( 1 )
	{
		// Poll until we get a message or error
		*out = get_current_rx_slot( channel);

		if ( ! check_rx_slot_msg_waiting( *out ) )
		{
			// No messages to receive, yield to next async
			//printf("No messages to recv, yield and save into id:%llu\n", id);
			THCYieldAndSave(id);
			continue; 
		}

		break;
	}

	received_cookie = thc_get_msg_id(*out);
	if (received_cookie == id) {
		//printf("Message is ours id:%llu\n", (*out)->regs[0]);
		inc_rx_slot( channel ); 
		return 0;
	}
	
	//printf("Message is not ours yielding to id:%llu\n", (*out)->regs[0]);
	ret = THCYieldToIdAndSave(received_cookie, id);
	 
	//ret = THCYieldToId((*out)->regs[0]);
	if (ret) {
		printk("ALERT: wrong id\n");
		return ret;
	}

	// We came back here but maybe we're the last AWE and 
        // we're re-started by do finish
	goto retry; 
	return 0;
}
#endif /* #if 0 */

static inline
int fipc_test_blocking_recv_start ( struct thc_channel *chnl, struct fipc_message** out)
{
	int ret;

	while ( 1 )
	{
#ifdef ONE_SLOT
		// Poll until we get a message or error
		ret = fipc_recv_msg_start_0( thc_channel_to_fipc(chnl),
						out );
#else
		// Poll until we get a message or error
		ret = fipc_recv_msg_start( thc_channel_to_fipc(chnl),
						out );
#endif
		if ( !ret || ret != -EWOULDBLOCK )
		{
			return ret;
		}	
		fipc_test_pause();
	}
	return 0;
}

/**
 * This function will block until a message slot is available and stored in out.
 */
static inline
int fipc_test_blocking_send_start ( struct thc_channel * chnl, struct fipc_message** out )
{
	int ret;

	while ( 1 )
	{
		// Poll until we get a free slot or error
		ret = fipc_send_msg_start( thc_channel_to_fipc(chnl), out );

		if ( !ret || ret != -EWOULDBLOCK )
		{
			return ret;
		}

		fipc_test_pause();
	}

	return 0;
}


#endif /* _GLUE_HELPER_H_ */
