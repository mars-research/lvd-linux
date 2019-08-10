#ifndef NVME_DEV_H
#define NVME_DEV_H
/*
 * Represents an NVM Express device.  Each nvme_dev is a PCI function.
 */
struct nvme_dev {
	struct nvme_queue **queues;
	struct blk_mq_tag_set tagset;
	struct blk_mq_tag_set admin_tagset;
	u32 __iomem *dbs;
	struct device *dev;
	struct dma_pool *prp_page_pool;
	struct dma_pool *prp_small_pool;
	unsigned queue_count;
	unsigned online_queues;
	unsigned max_qid;
	int q_depth;
	u32 db_stride;
	struct msix_entry *entry;
	void __iomem *bar;
	struct work_struct reset_work;
	struct work_struct remove_work;
	struct timer_list watchdog_timer;
	struct mutex shutdown_lock;
	bool subsystem;
	void __iomem *cmb;
	dma_addr_t cmb_dma_addr;
	u64 cmb_size;
	u32 cmbsz;
	struct nvme_ctrl ctrl;
    
	struct completion ioq_wait;
};
#endif