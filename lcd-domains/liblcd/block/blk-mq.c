#include <lcd_config/pre_hook.h>

#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include "blk-mq-tag.h"

#include <lcd_config/post_hook.h>

struct request *blk_mq_tag_to_rq(struct blk_mq_tags *tags, unsigned int tag)
{

//	printk("%s tag: %d nr_tags: %d", __func__, tag,
//			tags->nr_tags);
	if (tag < tags->nr_tags)
		return tags->rqs[tag];

	return NULL;
}
EXPORT_SYMBOL(blk_mq_tag_to_rq);

struct cpumask *blk_mq_tags_cpumask(struct blk_mq_tags *tags)
{
	return tags->cpumask;
}
EXPORT_SYMBOL_GPL(blk_mq_tags_cpumask);


/*
 * This helper should only be used for flush request to share tag
 * with the request cloned from, and both the two requests can't be
 * in flight at the same time. The caller has to make sure the tag
 * can't be freed.
 */
void blk_mq_tag_set_rq(struct blk_mq_hw_ctx *hctx,
		unsigned int tag, struct request *rq)
{
	hctx->tags->rqs[tag] = rq;
}

int create_rq_maps(struct blk_mq_tag_set *set)
{
	int ret = 0;
	int i;
	/* Account for the flush_rq request as well */
	int depth = set->queue_depth + 1;

	set->tags = kzalloc_node(set->nr_hw_queues * sizeof(struct blk_mq_tags *),
				 GFP_KERNEL, set->numa_node);

	if (!set->tags) {
		LIBLCD_ERR("alloc failed");
		ret = -ENOMEM;
		goto fail_alloc;
	}

	for (i = 0; i < set->nr_hw_queues; i++) {
		struct blk_mq_tags *tags;
		int j;

		tags = set->tags[i] = kzalloc(sizeof(struct blk_mq_tags), GFP_KERNEL);

		printk("%s setting nr_tags to %d", __func__, depth);
		tags->nr_tags = depth;

		if (!set->tags[i]) {
			LIBLCD_ERR("fail alloc");
			ret = -ENOMEM;
			goto fail_alloc;
		}
		tags->rqs = kzalloc(depth * sizeof(struct request*),
					GFP_KERNEL);
		if (!tags->rqs) {
			LIBLCD_ERR("alloc failed");
			ret = -ENOMEM;
			goto fail_alloc;
		}

		for (j = 0; j < depth; j++) {
			tags->rqs[j] = kzalloc(sizeof(struct request) + set->cmd_size,
						GFP_KERNEL);
			if (!tags->rqs[j]) {
				LIBLCD_ERR("alloc failed");
				ret = -ENOMEM;
				goto fail_alloc;
			}
		}
	}
fail_alloc:
	return ret;
}
EXPORT_SYMBOL(create_rq_maps);

void assign_q_to_rqs(struct blk_mq_tag_set *set, struct request_queue *q)
{
	int i;

	BUG_ON(set->tags == NULL);
	for (i = 0; i < set->nr_hw_queues; i++) {
		struct blk_mq_tags *tags;
		int j;

		tags = set->tags[i];

		BUG_ON(tags == NULL);
		BUG_ON(tags->rqs == NULL);

		for (j = 0; j < set->queue_depth + 1; j++) {
			struct request *rq = tags->rqs[j];
			BUG_ON(rq == NULL);
			rq->q = q;
		}
	}
}
EXPORT_SYMBOL(assign_q_to_rqs);
