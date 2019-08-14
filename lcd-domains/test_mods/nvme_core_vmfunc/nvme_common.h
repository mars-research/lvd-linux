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
	PCI_DISABLE_MSI,
	PCI_ENABLE_MSIX,
	PCI_ENABLE_MSIX_RANGE,
	PCI_ENABLE_MSI_RANGE,
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
	PCI_DEVICE_IS_PRESENT,
	PCI_VFS_ASSIGNED,
	PUT_DEVICE,
	DMA_POOL_ALLOC,
	DMA_POOL_CREATE,
	DMA_POOL_DESTROY,
	DMA_POOL_FREE,
	PROBE,
	REMOVE,
	SYNC,
	UNSYNC,
	POLL,
	REQUEST_THREADED_IRQ,
	FREE_IRQ,
	MSIX_IRQ_HANDLER,
	IRQ_SET_AFFINITY_HINT,
	TRIGGER_EXIT,
	SERVICE_EVENT_SCHED,
	TRIGGER_DUMP,
	TRIGGER_CLEAN,
	BLK_EXECUTE_RQ_NOWAIT,
	BLK_GET_QUEUE,
	BLK_MQ_COMPLETE_REQUEST,
	BLK_MQ_FREE_REQUEST,
	BLK_MQ_START_STOPPED_HW_QUEUES,
	BLK_MQ_STOP_HW_QUEUES,
	BLK_MQ_TAGS_CPUMASK,
	BLK_MQ_TAGSET_BUSY_ITER,
	BLK_MQ_TAG_TO_RQ,
	BLK_MQ_UPDATE_NR_HW_QUEUES,
	BLK_PUT_QUEUE,
	BLK_RQ_COUNT_INTEGRITY_SG,
	BLK_RQ_MAP_INTEGRITY_SG,
	BLK_RQ_MAP_SG,
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
	GET_DEVICE,
	DEVICE_RELEASE_DRIVER,
	QUEUE_RQ_FN,
	SOFTIRQ_DONE_FN,
	MAP_QUEUE_FN,
	INIT_HCTX_FN,
	INIT_HCTX_SYNC,
	EXIT_HCTX_FN,
	INIT_REQUEST_FN,
	TIMEOUT_FN,
	POLL_FN,
	OPEN,
	RELEASE,
	NVME_ALLOC_REQUEST,
	NVME_CANCEL_REQUEST,
	NVME_CHANGE_CTRL_STATE,
	NVME_COMPLETE_ASYNC_EVENT,
	NVME_DISABLE_CTRL,
	NVME_ENABLE_CTRL,
	NVME_INIT_CTRL,
	NVME_INIT_IDENTIFY,
	NVME_IO_TIMEOUT_DISPATCH, //NVME_IO_TIMEOUT is a macro
	NVME_KILL_QUEUES,
	NVME_MAX_RETRIES,
	NVME_PUT_CTRL,
	NVME_QUEUE_ASYNC_EVENTS,
	NVME_QUEUE_SCAN,
	NVME_REMOVE_NAMESPACES,
	NVME_REQUEUE_REQ,
	NVME_SET_QUEUE_COUNT,
	NVME_SETUP_CMD,
	NVME_SHUTDOWN_CTRL,
	NVME_START_QUEUES,
	NVME_STOP_QUEUES,
	NVME_SUBMIT_SYNC_CMD,
	NVME_UNINIT_CTRL,
	NVME_PCI_REG_READ32,
	NVME_PCI_REG_WRITE32,
	NVME_PCI_REG_READ64,
	NVME_PCI_RESET_CTRL,
	NVME_PCI_FREE_CTRL,
	NVME_PCI_POST_SCAN,
	NVME_PCI_SUBMIT_ASYNC_EVENT,
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

#define LOWER32_BITS    32
#define LOWER_HALF(x)   (x & ((1ULL << LOWER32_BITS) - 1))
#define UPPER_HALF(x)   (x >> LOWER32_BITS)

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
