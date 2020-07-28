#include <lcd_config/pre_hook.h>

#include <linux/bio.h>
#include <linux/blkdev.h>
#include <linux/scatterlist.h>

#include <lcd_config/post_hook.h>

static inline void
__blk_segment_map_sg(struct request_queue *q, struct bio_vec *bvec,
		     struct scatterlist *sglist, struct bio_vec *bvprv,
		     struct scatterlist **sg, int *nsegs, int *cluster)
{

	int nbytes = bvec->bv_len;

	if (*sg && *cluster) {
		if ((*sg)->length + nbytes > queue_max_segment_size(q))
			goto new_segment;

		if (!BIOVEC_PHYS_MERGEABLE(bvprv, bvec))
			goto new_segment;
		if (!BIOVEC_SEG_BOUNDARY(q, bvprv, bvec))
			goto new_segment;

		(*sg)->length += nbytes;
		printk("%s, no new segment!", __func__);
	} else {
new_segment:
		if (!*sg)
			*sg = sglist;
		else {
			/*
			 * If the driver previously mapped a shorter
			 * list, we could see a termination bit
			 * prematurely unless it fully inits the sg
			 * table on each mapping. We KNOW that there
			 * must be more entries here or the driver
			 * would be buggy, so force clear the
			 * termination bit to avoid doing a full
			 * sg_init_table() in drivers for each command.
			 */
			sg_unmark_end(*sg);
			*sg = sg_next(*sg);
		}

		sg_set_page(*sg, bvec->bv_page, nbytes, bvec->bv_offset);
		(*nsegs)++;
	}
	*bvprv = *bvec;
}

static int __blk_bios_map_sg(struct request_queue *q, struct bio *bio,
			     struct scatterlist *sglist,
			     struct scatterlist **sg)
{
	struct bio_vec bvec, bvprv = { NULL };
	struct bvec_iter iter;
	int nsegs, cluster;
	struct bio *_bio = bio;

	nsegs = 0;
	cluster = blk_queue_cluster(q);

	if (!bio) {
		printk("%s:%d bio %p", __func__, __LINE__, bio);
	}

	switch (bio_op(bio)) {
	case REQ_OP_DISCARD:
	case REQ_OP_SECURE_ERASE:
		/*
		 * This is a hack - drivers should be neither modifying the
		 * biovec, nor relying on bi_vcnt - but because of
		 * blk_add_request_payload(), a discard bio may or may not have
		 * a payload we need to set up here (thank you Christoph) and
		 * bi_vcnt is really the only way of telling if we need to.
		 */
		if (!bio->bi_vcnt) {
			printk("%s bi_vcnt is zero! return", __func__);
			return 0;
		}
		/* Fall through */
	case REQ_OP_WRITE_SAME:
		*sg = sglist;
		bvec = bio_iovec(bio);
		sg_set_page(*sg, bvec.bv_page, bvec.bv_len, bvec.bv_offset);
		return 1;
	default:
		break;
	}

	for_each_bio(bio)
		bio_for_each_segment(bvec, bio, iter)
			__blk_segment_map_sg(q, &bvec, sglist, &bvprv, sg,
					     &nsegs, &cluster);

	/* XXX: nsegs is zero. Something's wrong! */
	if (!nsegs) {
		struct bio *bio = _bio;
		for_each_bio(bio)
			bio_for_each_segment(bvec, bio, iter)
				printk("%s, bvec.{bv_page %p offset %d len %d}",
						__func__, bvec.bv_page, bvec.bv_offset, bvec.bv_len);


		bio = _bio;

		if (bio) {
			printk("%s, bio %p {.bi_io_vec %p .bi_vcnt %d .bi_phys_segments %d .bi_iter %p}",
					__func__, bio, bio->bi_io_vec, bio->bi_vcnt, bio->bi_phys_segments,
					&bio->bi_iter);
		} else {
			printk("%s bio %p _bio %p", __func__, bio, _bio);
		}
	}

	return nsegs;
}

/*
 * map a request to scatterlist, return number of sg entries setup. Caller
 * must make sure sg can hold rq->nr_phys_segments entries
 */
int blk_rq_map_sg(struct request_queue *q, struct request *rq,
		  struct scatterlist *sglist)
{
	struct scatterlist *sg = NULL;
	int nsegs = 0;

	if (rq->bio) {
		nsegs = __blk_bios_map_sg(q, rq->bio, sglist, &sg);
		WARN_ON(!nsegs);
	} else {
		printk("%s rq %p rq->bio %p", __func__, rq, rq->bio);
	}

	if (unlikely(rq->cmd_flags & REQ_COPY_USER) &&
	    (blk_rq_bytes(rq) & q->dma_pad_mask)) {
		unsigned int pad_len =
			(q->dma_pad_mask & ~blk_rq_bytes(rq)) + 1;

		sg->length += pad_len;
		rq->extra_len += pad_len;
	}

	if (q->dma_drain_size && q->dma_drain_needed(rq)) {
		if (op_is_write(req_op(rq)))
			memset(q->dma_drain_buffer, 0, q->dma_drain_size);

		sg_unmark_end(sg);
		sg = sg_next(sg);
		sg_set_page(sg, virt_to_page(q->dma_drain_buffer),
			    q->dma_drain_size,
			    ((unsigned long)q->dma_drain_buffer) &
			    (PAGE_SIZE - 1));
		nsegs++;
		rq->extra_len += q->dma_drain_size;
	}

	if (sg)
		sg_mark_end(sg);

	/*
	 * Something must have been wrong if the figured number of
	 * segment is bigger than number of req's physical segments
	 */
	WARN_ON(nsegs > rq->nr_phys_segments);

	return nsegs;
}
EXPORT_SYMBOL(blk_rq_map_sg);

static unsigned int __blk_recalc_rq_segments(struct request_queue *q,
					     struct bio *bio,
					     bool no_sg_merge)
{
	struct bio_vec bv, bvprv = { NULL };
	int cluster, prev = 0;
	unsigned int seg_size, nr_phys_segs;
	struct bio *fbio, *bbio;
	struct bvec_iter iter;

	if (!bio)
		return 0;

	/*
	 * This should probably be returning 0, but blk_add_request_payload()
	 * (Christoph!!!!)
	 */
	if (bio_op(bio) == REQ_OP_DISCARD || bio_op(bio) == REQ_OP_SECURE_ERASE)
		return 1;

	if (bio_op(bio) == REQ_OP_WRITE_SAME)
		return 1;

	fbio = bio;
	cluster = blk_queue_cluster(q);
	seg_size = 0;
	nr_phys_segs = 0;
	for_each_bio(bio) {
		bio_for_each_segment(bv, bio, iter) {
			/*
			 * If SG merging is disabled, each bio vector is
			 * a segment
			 */
			if (no_sg_merge)
				goto new_segment;

			if (prev && cluster) {
				if (seg_size + bv.bv_len
				    > queue_max_segment_size(q))
					goto new_segment;
				if (!BIOVEC_PHYS_MERGEABLE(&bvprv, &bv))
					goto new_segment;
				if (!BIOVEC_SEG_BOUNDARY(q, &bvprv, &bv))
					goto new_segment;

				seg_size += bv.bv_len;
				bvprv = bv;
				continue;
			}
new_segment:
			if (nr_phys_segs == 1 && seg_size >
			    fbio->bi_seg_front_size)
				fbio->bi_seg_front_size = seg_size;

			nr_phys_segs++;
			bvprv = bv;
			prev = 1;
			seg_size = bv.bv_len;
		}
		bbio = bio;
	}

	if (nr_phys_segs == 1 && seg_size > fbio->bi_seg_front_size)
		fbio->bi_seg_front_size = seg_size;
	if (seg_size > bbio->bi_seg_back_size)
		bbio->bi_seg_back_size = seg_size;

	return nr_phys_segs;
}

void blk_recalc_rq_segments(struct request *rq)
{
	bool no_sg_merge = !!test_bit(QUEUE_FLAG_NO_SG_MERGE,
			&rq->q->queue_flags);

	rq->nr_phys_segments = __blk_recalc_rq_segments(rq->q, rq->bio,
			no_sg_merge);
}

void blk_recount_segments(struct request_queue *q, struct bio *bio)
{
	unsigned short seg_cnt;

	/* estimate segment number by bi_vcnt for non-cloned bio */
	if (bio_flagged(bio, BIO_CLONED))
		seg_cnt = bio_segments(bio);
	else
		seg_cnt = bio->bi_vcnt;

	if (test_bit(QUEUE_FLAG_NO_SG_MERGE, &q->queue_flags) &&
			(seg_cnt < queue_max_segments(q)))
		bio->bi_phys_segments = seg_cnt;
	else {
		struct bio *nxt = bio->bi_next;

		bio->bi_next = NULL;
		bio->bi_phys_segments = __blk_recalc_rq_segments(q, bio, false);
		bio->bi_next = nxt;
	}

	bio_set_flag(bio, BIO_SEG_VALID);
}
EXPORT_SYMBOL(blk_recount_segments);
