#ifndef _BLK_MQ_TAG_H
#define _BLK_MQ_TAG_H

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

#endif
