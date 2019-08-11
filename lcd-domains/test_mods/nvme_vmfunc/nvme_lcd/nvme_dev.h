#ifndef NVME_DEV_H
#define NVME_DEV_H

/*
 * Represents an NVM Express device.  Each nvme_dev is a PCI function.
 */
struct nvme_dev {
	struct nvme_queue **queues;
#ifdef LCD_ISOLATE
	union {
		struct blk_mq_tag_set_container tagset_container;
		struct blk_mq_tag_set tagset;
	};
#else
	struct blk_mq_tag_set tagset;
#endif

#ifdef LCD_ISOLATE
	union {
		struct blk_mq_tag_set_container admin_tagset_container;
		struct blk_mq_tag_set admin_tagset;
	};
#else
	struct blk_mq_tag_set admin_tagset;
#endif
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
#ifdef LCD_ISOLATE
	union {
		struct nvme_ctrl_container nvme_ctrl_c;
		struct nvme_ctrl ctrl;
	};
#else
	struct nvme_ctrl ctrl;
#endif
	struct completion ioq_wait;
};


enum {
	BT_WAIT_QUEUES	= 8,
	BT_WAIT_BATCH	= 8,
};

struct bt_wait_state {
	atomic_t wait_cnt;
	wait_queue_head_t wait;
} ____cacheline_aligned_in_smp;

#define TAG_TO_INDEX(bt, tag)	((tag) >> (bt)->bits_per_word)
#define TAG_TO_BIT(bt, tag)	((tag) & ((1 << (bt)->bits_per_word) - 1))

struct blk_mq_bitmap_tags {
	unsigned int depth;
	unsigned int wake_cnt;
	unsigned int bits_per_word;

	unsigned int map_nr;
	struct blk_align_bitmap *map;

	atomic_t wake_index;
	struct bt_wait_state *bs;
};

/*
 * Tag address space map.
 */
struct blk_mq_tags {
	unsigned int nr_tags;
	unsigned int nr_reserved_tags;

	atomic_t active_queues;

	struct blk_mq_bitmap_tags bitmap_tags;
	struct blk_mq_bitmap_tags breserved_tags;

	struct request **rqs;
	struct list_head page_list;

	int alloc_policy;
	cpumask_var_t cpumask;
};


#endif	/* NVME_DEV_H */
