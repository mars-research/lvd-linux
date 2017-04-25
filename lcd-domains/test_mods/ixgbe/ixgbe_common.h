#ifndef __IXGBE_COMMON_H__
#define __IXGBE_COMMON_H__

#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/rtnetlink.h>

#include <libcap.h>
#include <libfipc.h>
#include <thc_ipc.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>

#include "ixgbe_glue_helper.h"

#define PCI_REGIONS
#define IOMMU_ASSIGN

enum dispatch_t {
	__PCI_REGISTER_DRIVER,
	PCI_UNREGISTER_DRIVER,
	ALLOC_ETHERDEV_MQS,
	REGISTER_NETDEV,
	ETHER_SETUP,
	ETH_MAC_ADDR,
	ETH_VALIDATE_ADDR,
	FREE_NETDEV,
	NETIF_CARRIER_OFF,
	NETIF_CARRIER_ON,
	NETIF_DEVICE_ATTACH,
	NETIF_DEVICE_DETACH,
	NETIF_SET_REAL_NUM_RX_QUEUES,
	NETIF_SET_REAL_NUM_TX_QUEUES,
	CONSUME_SKB,
	UNREGISTER_NETDEV,
	ETH_PLATFORM_GET_MAC_ADDRESS,
	DEV_ADDR_ADD,
	DEV_ADDR_DEL,
	DEVICE_SET_WAKEUP_ENABLE,
	ETH_GET_HEADLEN,
	NETIF_TX_STOP_ALL_QUEUES,
	NETIF_TX_WAKE_ALL_QUEUES,
	PCI_DISABLE_PCIE_ERROR_REPORTING,
	PCI_BUS_READ_CONFIG_WORD,
	PCI_BUS_WRITE_CONFIG_WORD,
	PCI_CLEANUP_AER_UNCORRECT_ERROR_STATUS,
	PCI_DISABLE_DEVICE,
	PCI_DISABLE_MSIX,
	PCI_ENABLE_MSIX_RANGE,
	PCI_ENABLE_PCIE_ERROR_REPORTING,
	PCIE_CAPABILITY_READ_WORD,
	PCIE_GET_MINIMUM_LINK,
	PCI_ENABLE_DEVICE_MEM,
	PCI_REQUEST_SELECTED_REGIONS,
	PCI_REQUEST_SELECTED_REGIONS_EXCLUSIVE,
	PCI_SET_MASTER,
	PCI_SAVE_STATE,
	PCI_RESTORE_STATE,
	PCI_RELEASE_SELECTED_REGIONS,
	PCI_SELECT_BARS,
	PCI_WAKE_FROM_D3,
	__HW_ADDR_SYNC_DEV,
	__HW_ADDR_UNSYNC_DEV,
	NDO_OPEN,
	NDO_STOP,
	NDO_START_XMIT,
	NDO_SELECT_QUEUE,
	NDO_SET_RX_MODE,
	NDO_VALIDATE_ADDR,
	NDO_SET_MAC_ADDRESS,
	NDO_CHANGE_MTU,
	NDO_TX_TIMEOUT,
	NDO_SET_TX_MAXRATE,
	NDO_GET_STATS64,
	PROBE,
	REMOVE,
	SYNC,
	UNSYNC,
	TRIGGER_EXIT,
	SERVICE_EVENT_SCHED,
	TRIGGER_DUMP,
};

typedef enum {
	MC_LIST = 1,
	UC_LIST = 2,
} addr_list;

#define MAC_ADDR_DUMP(addr)	\
	do { \
	int _i; \
	for (_i = 0; _i < ETH_ALEN - 1; _i++) \
		printk("%02x:", addr[_i]); \
	printk("%02x\n", addr[_i]); \
	} while(0)

#define ASYNC_RPC_BUFFER_ORDER 12

struct pcidev_info {
	unsigned int domain, bus, slot, fn;
};

/* CSPACES ------------------------------------------------------------ */
int glue_cap_init(void);

int glue_cap_create(struct glue_cspace **cspace);

void glue_cap_destroy(struct glue_cspace *cspace);

void glue_cap_exit(void);

void glue_cap_remove(
	struct glue_cspace *cspace,
	cptr_t c);

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


#endif /* __IXGBE_COMMON_H__ */
