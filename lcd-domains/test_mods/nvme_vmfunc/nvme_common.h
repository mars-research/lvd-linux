#ifndef __NVME_COMMON_H__
#define __NVME_COMMON_H__

#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/rtnetlink.h>
#include <linux/blkdev.h>
#include <linux/blk-mq.h>

#include <libcap.h>
#include <libfipc.h>
#include <thc_ipc.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <linux/priv_mempool.h>
#include <asm/lcd_domains/libvmfunc.h>

#include "nvme_glue_helper.h"

#define PCI_REGIONS
#define IOMMU_ASSIGN
#define HOST_IRQ
#define SENDER_DISPATCH_LOOP
#define PASS_DEV_ADDR_IN_REG
//#define CONFIG_VMALLOC_SHARED_POOL

enum dispatch_t {
	__PCI_REGISTER_DRIVER,
	PCI_UNREGISTER_DRIVER,
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
	PROBE,
	REMOVE,
	SYNC,
	UNSYNC,
	POLL,
	REQUEST_THREADED_IRQ,
	FREE_IRQ,
	MSIX_IRQ_HANDLER,
	TRIGGER_EXIT,
	SERVICE_EVENT_SCHED,
	TRIGGER_DUMP,
	TRIGGER_CLEAN,
    
    BLK_MQ_ALLOC_TAG_SET,
        BLK_MQ_INIT_QUEUE,
        BLK_CLEANUP_QUEUE,
	BLK_MQ_END_REQUEST,
        BLK_MQ_FREE_TAG_SET,
        BLK_MQ_START_REQUEST,
        BLK_MQ_MAP_QUEUE,
        BLK_QUEUE_LOGICAL_BLOCK_SIZE,
        BLK_QUEUE_PHYSICAL_BLOCK_SIZE,
        ALLOC_DISK,
	DEVICE_ADD_DISK,
        PUT_DISK,
        DEL_GENDISK,
        DISK_NODE,
        REGISTER_BLKDEV,
        UNREGISTER_BLKDEV,
        REGISTER_CHARDEV,
	QUEUE_RQ_FN,
        MAP_QUEUE_FN,
        INIT_HCTX_FN,
	INIT_HCTX_SYNC,
        SOFTIRQ_DONE_FN,
        OPEN,
	RELEASE,
    
	MODULE_INIT,
	MODULE_EXIT,
	SYNC_PROBE,
	SYNC_NDO_SET_MAC_ADDRESS,
	SYNCHRONIZE_IRQ,
};

/*
typedef enum {
	MC_LIST = 1,
	UC_LIST = 2,
} addr_list;
*/

/*
typedef enum {
	VOLUNTEER_XMIT = 0x1,
	SHARED_DATA_XMIT = 0x2abcd,
} xmit_type_t;
*/

/*
typedef enum {
	IXGBE_POLL_RUNNING,
	IXGBE_POLL_STOPPED,
} ixgbe_poll_state_t;
*/

#define ASYNC_RPC_BUFFER_ORDER 15

#define INIT_FIPC_MSG(msg)		memset(msg, 0x0, sizeof(*msg))

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

#define INIT_IPC_MSG(m)		memset(m, 0x0, sizeof(*m))
/* ASYNC HELPERS -------------------------------------------------- */
static inline
int
async_msg_get_fn_type(struct fipc_message *msg)
{
	return msg->rpc_id;
}

static inline
void
async_msg_set_fn_type(struct fipc_message *msg, int type)
{
	msg->vmfunc_id = VMFUNC_RPC_CALL;
	msg->rpc_id = type;
}

#endif /* __NVME_COMMON_H__ */
