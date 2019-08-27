#include <lcd_config/pre_hook.h>

#include <linux/bio.h>
#include <linux/blkdev.h>
#include <linux/uio.h>

#include <lcd_config/post_hook.h>

void blk_rq_bio_prep(struct request_queue *q, struct request *rq,
			struct bio *bio);
/*
 * Append a bio to a passthrough request.  Only works can be merged into
 * the request based on the driver constraints.
 */
int blk_rq_append_bio(struct request *rq, struct bio *bio)
{
	if (!rq->bio) {
		blk_rq_bio_prep(rq->q, rq, bio);
	}
#ifndef LCD_ISOLATE
	else {
		if (!ll_back_merge_fn(rq->q, rq, bio))
			return -EINVAL;

		rq->biotail->bi_next = bio;
		rq->biotail = bio;
		rq->__data_len += bio->bi_iter.bi_size;
	}
#endif

	return 0;
}
EXPORT_SYMBOL(blk_rq_append_bio);


/**
 * blk_rq_map_kern - map kernel data to a request, for REQ_TYPE_BLOCK_PC usage
 * @q:		request queue where request should be inserted
 * @rq:		request to fill
 * @kbuf:	the kernel buffer
 * @len:	length of user data
 * @gfp_mask:	memory allocation flags
 *
 * Description:
 *    Data will be mapped directly if possible. Otherwise a bounce
 *    buffer is used. Can be called multiple times to append multiple
 *    buffers.
 */
int blk_rq_map_kern(struct request_queue *q, struct request *rq, void *kbuf,
		    unsigned int len, gfp_t gfp_mask)
{
	int reading = rq_data_dir(rq) == READ;
#ifndef LCD_ISOLATE
	unsigned long addr = (unsigned long) kbuf;
#endif
	int do_copy = 0;
	struct bio *bio;
	int ret;

	LIBLCD_MSG("%s, called", __func__);

	if (len > (queue_max_hw_sectors(q) << 9)) {
		LIBLCD_ERR("len (%d) > max_hw_sectors << 9 (%d)", len, (queue_max_hw_sectors(q) << 9));
		return -EINVAL;
	}
	if (!len || !kbuf) {
		LIBLCD_ERR("kbuf :%s ", kbuf);
		return -EINVAL;
	}
#ifndef LCD_ISOLATE
	/*
	 * XXX: object_is_on_stack checks if a pointer is in the range of current's
	 * stack, which we do not have access to inside LCD
	 */
	do_copy = !blk_rq_aligned(q, addr, len) || object_is_on_stack(kbuf);
	if (do_copy)
		bio = bio_copy_kern(q, kbuf, len, gfp_mask, reading);
	else
#endif
		bio = bio_map_kern(q, kbuf, len, gfp_mask);

	if (IS_ERR(bio)) {
		LIBLCD_ERR("bio_map_kern returned: %p", bio);
		return PTR_ERR(bio);
	}
	if (!reading)
		bio_set_op_attrs(bio, REQ_OP_WRITE, 0);

	if (do_copy)
		rq->cmd_flags |= REQ_COPY_USER;

	ret = blk_rq_append_bio(rq, bio);

	LIBLCD_MSG("%s, blk_rq_append_bio returned %d", __func__, ret);

	if (unlikely(ret)) {
		LIBLCD_ERR("blk_rq_append_bio returned %d", ret);
		/* request is too big */
		bio_put(bio);
		return ret;
	}

#if 0
	blk_queue_bounce(q, &rq->bio);
#endif

	LIBLCD_MSG("%s, returned %d", __func__, ret);
	return 0;
}
//EXPORT_SYMBOL(blk_rq_map_kern);
