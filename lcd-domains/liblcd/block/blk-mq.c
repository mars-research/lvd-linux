#include <lcd_config/pre_hook.h>

#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include "blk-mq-tag.h"

#include <lcd_config/post_hook.h>

struct request *blk_mq_tag_to_rq(struct blk_mq_tags *tags, unsigned int tag)
{
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

		tags->nr_tags = set->queue_depth;

		if (!set->tags[i]) {
			LIBLCD_ERR("fail alloc");
			ret = -ENOMEM;
			goto fail_alloc;
		}
		tags->rqs = kzalloc(set->queue_depth * sizeof(struct request*),
					GFP_KERNEL);
		if (!tags->rqs) {
			LIBLCD_ERR("alloc failed");
			ret = -ENOMEM;
			goto fail_alloc;
		}

		for (j = 0; j < set->queue_depth; j++) {
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
