#include <lcd_config/pre_hook.h>

#include <linux/bio.h>
#include <linux/blkdev.h>
#include <linux/blk-mq.h>

#include <lcd_config/post_hook.h>

void blk_rq_bio_prep(struct request_queue *q, struct request *rq,
		     struct bio *bio)
{
	req_set_op(rq, bio_op(bio));

	if (bio_has_data(bio))
		rq->nr_phys_segments = bio_phys_segments(q, bio);

	rq->__data_len = bio->bi_iter.bi_size;
	rq->bio = rq->biotail = bio;

	if (bio->bi_bdev)
		rq->rq_disk = bio->bi_bdev->bd_disk;
}
