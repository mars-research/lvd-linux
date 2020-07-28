#include <lcd_config/pre_hook.h>

#include <linux/bio.h>
#include <linux/blkdev.h>
#include <linux/uio.h>

#include <lcd_config/post_hook.h>

/*
 * if you change this list, also change bvec_alloc or things will
 * break badly! cannot be bigger than what you can fit into an
 * unsigned short
 */
#define BV(x) { .nr_vecs = x, .name = "biovec-"__stringify(x) }
static struct biovec_slab bvec_slabs[BVEC_POOL_NR] __read_mostly = {
	BV(1), BV(4), BV(16), BV(64), BV(128), BV(BIO_MAX_PAGES),
};
#undef BV

static inline bool bio_remaining_done(struct bio *bio)
{
	/*
	 * If we're not chaining, then ->__bi_remaining is always 1 and
	 * we always end io on the first invocation.
	 */
	if (!bio_flagged(bio, BIO_CHAIN))
		return true;

	BUG_ON(atomic_read(&bio->__bi_remaining) <= 0);

	if (atomic_dec_and_test(&bio->__bi_remaining)) {
		bio_clear_flag(bio, BIO_CHAIN);
		return true;
	}

	return false;
}

static struct bio *__bio_chain_endio(struct bio *bio)
{
	struct bio *parent = bio->bi_private;

	if (!parent->bi_error)
		parent->bi_error = bio->bi_error;
	bio_put(bio);
	return parent;
}

static void bio_chain_endio(struct bio *bio)
{
	bio_endio(__bio_chain_endio(bio));
}


void bvec_free(mempool_t *pool, struct bio_vec *bv, unsigned int idx)
{
	if (!idx)
		return;
	idx--;

	BIO_BUG_ON(idx >= BVEC_POOL_NR);

#ifndef LCD_ISOLATE
	if (idx == BVEC_POOL_MAX) {
		mempool_free(bv, pool);
	} else
#endif
	{
		struct biovec_slab *bvs = bvec_slabs + idx;

		kmem_cache_free(bvs->slab, bv);
	}
}

struct bio_vec *bvec_alloc(gfp_t gfp_mask, int nr, unsigned long *idx,
			   mempool_t *pool)
{
	struct bio_vec *bvl;

	/*
	 * see comment near bvec_array define!
	 */
	switch (nr) {
	case 1:
		*idx = 0;
		break;
	case 2 ... 4:
		*idx = 1;
		break;
	case 5 ... 16:
		*idx = 2;
		break;
	case 17 ... 64:
		*idx = 3;
		break;
	case 65 ... 128:
		*idx = 4;
		break;
#ifndef LCD_ISOLATE
	/*
	 * nr represents number of pages. Block layer resorts to pre-created
	 * mempool if asked for more than 128 pages, which is a huge number of
	 * pages. Inside LCD, we restrict it to a single page atmost. Anyways,
	 * let's keep this case commented to not dig into mempools, as we do
	 * not have support for it yet, inside LCDs.
	 */
	case 129 ... BIO_MAX_PAGES:
		*idx = 5;
		break;
#endif
	default:
		return NULL;
	}

	/*
	 * idx now points to the pool we want to allocate from. only the
	 * 1-vec entry pool is mempool backed.
	 */
#ifndef LCD_ISOLATE
	if (*idx == BVEC_POOL_MAX) {
fallback:
		bvl = mempool_alloc(pool, gfp_mask);
	} else
#endif	
	{
		struct biovec_slab *bvs = bvec_slabs + *idx;
		gfp_t __gfp_mask = gfp_mask & ~(__GFP_DIRECT_RECLAIM | __GFP_IO);

		/*
		 * Make this allocation restricted and don't dump info on
		 * allocation failures, since we'll fallback to the mempool
		 * in case of failure.
		 */
		__gfp_mask |= __GFP_NOMEMALLOC | __GFP_NORETRY | __GFP_NOWARN;

		/*
		 * Try a slab allocation. If this fails and __GFP_DIRECT_RECLAIM
		 * is set, retry with the 1-entry mempool
		 */
		bvl = kmem_cache_alloc(bvs->slab, __gfp_mask);
		if (unlikely(!bvl && (gfp_mask & __GFP_DIRECT_RECLAIM))) {
			*idx = BVEC_POOL_MAX;
			goto fail;
		}
	}

	(*idx)++;
	return bvl;
fail:
	return NULL;
}

static void __bio_free(struct bio *bio)
{

#ifndef LCD_ISOLATE
	/* We do not perform bio_associate_task */
	bio_disassociate_task(bio);
#endif

#ifndef LCD_ISOLATE
	/* We disable BLK_DEV_INTEGRITY */
	if (bio_integrity(bio))
		bio_integrity_free(bio);
#endif
}

static void bio_free(struct bio *bio)
{
	struct bio_set *bs = bio->bi_pool;
	void *p;

	__bio_free(bio);

	if (bs) {
		bvec_free(bs->bvec_pool, bio->bi_io_vec, BVEC_POOL_IDX(bio));

		/*
		 * If we have front padding, adjust the bio pointer before freeing
		 */
		p = bio;
		p -= bs->front_pad;

#ifndef LCD_ISOLATE
		mempool_free(p, bs->bio_pool);
#endif
	} else {
		/* Bio was allocated by bio_kmalloc() */
		kfree(bio);
	}
}

/**
 * bio_put - release a reference to a bio
 * @bio:   bio to release reference to
 *
 * Description:
 *   Put a reference to a &struct bio, either one you have gotten with
 *   bio_alloc, bio_get or bio_clone. The last put of a bio will free it.
 **/
void bio_put(struct bio *bio)
{
	if (!bio_flagged(bio, BIO_REFFED))
		bio_free(bio);
	else {
		BIO_BUG_ON(!atomic_read(&bio->__bi_cnt));

		/*
		 * last put frees it
		 */
		if (atomic_dec_and_test(&bio->__bi_cnt))
			bio_free(bio);
	}
}
EXPORT_SYMBOL(bio_put);

void bio_init(struct bio *bio)
{
	memset(bio, 0, sizeof(*bio));
	atomic_set(&bio->__bi_remaining, 1);
	atomic_set(&bio->__bi_cnt, 1);
}
EXPORT_SYMBOL(bio_init);

/**
 * bio_alloc_bioset - allocate a bio for I/O
 * @gfp_mask:   the GFP_ mask given to the slab allocator
 * @nr_iovecs:	number of iovecs to pre-allocate
 * @bs:		the bio_set to allocate from.
 *
 * Description:
 *   If @bs is NULL, uses kmalloc() to allocate the bio; else the allocation is
 *   backed by the @bs's mempool.
 *
 *   When @bs is not NULL, if %__GFP_DIRECT_RECLAIM is set then bio_alloc will
 *   always be able to allocate a bio. This is due to the mempool guarantees.
 *   To make this work, callers must never allocate more than 1 bio at a time
 *   from this pool. Callers that need to allocate more than 1 bio must always
 *   submit the previously allocated bio for IO before attempting to allocate
 *   a new one. Failure to do so can cause deadlocks under memory pressure.
 *
 *   Note that when running under generic_make_request() (i.e. any block
 *   driver), bios are not submitted until after you return - see the code in
 *   generic_make_request() that converts recursion into iteration, to prevent
 *   stack overflows.
 *
 *   This would normally mean allocating multiple bios under
 *   generic_make_request() would be susceptible to deadlocks, but we have
 *   deadlock avoidance code that resubmits any blocked bios from a rescuer
 *   thread.
 *
 *   However, we do not guarantee forward progress for allocations from other
 *   mempools. Doing multiple allocations from the same mempool under
 *   generic_make_request() should be avoided - instead, use bio_set's front_pad
 *   for per bio allocations.
 *
 *   RETURNS:
 *   Pointer to new bio on success, NULL on failure.
 */
struct bio *bio_alloc_bioset(gfp_t gfp_mask, int nr_iovecs, struct bio_set *bs)
{
#ifndef LCD_ISOLATE
	gfp_t saved_gfp = gfp_mask;
#endif
	unsigned front_pad;
	unsigned inline_vecs;
	struct bio_vec *bvl = NULL;
	struct bio *bio;
	void *p = NULL;

	//LIBLCD_MSG("%s bs: %p", __func__, bs);
	if (!bs) {
		if (nr_iovecs > UIO_MAXIOV)
			return NULL;

		p = kmalloc(sizeof(struct bio) +
			    nr_iovecs * sizeof(struct bio_vec),
			    gfp_mask);

		//LIBLCD_MSG("%s p: %p", __func__, p);
		front_pad = 0;
		inline_vecs = nr_iovecs;
	} else {
#ifndef LCD_ISOLATE
		/* should not use nobvec bioset for nr_iovecs > 0 */
		if (WARN_ON_ONCE(!bs->bvec_pool && nr_iovecs > 0))
			return NULL;
		/*
		 * generic_make_request() converts recursion to iteration; this
		 * means if we're running beneath it, any bios we allocate and
		 * submit will not be submitted (and thus freed) until after we
		 * return.
		 *
		 * This exposes us to a potential deadlock if we allocate
		 * multiple bios from the same bio_set() while running
		 * underneath generic_make_request(). If we were to allocate
		 * multiple bios (say a stacking block driver that was splitting
		 * bios), we would deadlock if we exhausted the mempool's
		 * reserve.
		 *
		 * We solve this, and guarantee forward progress, with a rescuer
		 * workqueue per bio_set. If we go to allocate and there are
		 * bios on current->bio_list, we first try the allocation
		 * without __GFP_DIRECT_RECLAIM; if that fails, we punt those
		 * bios we would be blocking to the rescuer workqueue before
		 * we retry with the original gfp_flags.
		 */

		if (current->bio_list && !bio_list_empty(current->bio_list))
			gfp_mask &= ~__GFP_DIRECT_RECLAIM;

		p = mempool_alloc(bs->bio_pool, gfp_mask);
		if (!p && gfp_mask != saved_gfp) {
			punt_bios_to_rescuer(bs);
			gfp_mask = saved_gfp;
			p = mempool_alloc(bs->bio_pool, gfp_mask);
		}

		front_pad = bs->front_pad;
		inline_vecs = BIO_INLINE_VECS;
#endif
	}

	if (unlikely(!p))
		return NULL;

	bio = p + front_pad;
	bio_init(bio);

	if (nr_iovecs > inline_vecs) {
		unsigned long idx = 0;

		bvl = bvec_alloc(gfp_mask, nr_iovecs, &idx, bs->bvec_pool);
		//LIBLCD_MSG("%s bvl: %p", __func__, bvl);
#ifndef LCD_ISOLATE
		/* We assume the allocation never fails to avoid calling punt_bios_xx */
		if (!bvl && gfp_mask != saved_gfp) {
			punt_bios_to_rescuer(bs);
			gfp_mask = saved_gfp;
			bvl = bvec_alloc(gfp_mask, nr_iovecs, &idx, bs->bvec_pool);
		}
#endif

		if (unlikely(!bvl))
			goto err_free;

		bio->bi_flags |= idx << BVEC_POOL_OFFSET;
	} else if (nr_iovecs) {
		bvl = bio->bi_inline_vecs;
	}

	bio->bi_pool = bs;
	bio->bi_max_vecs = nr_iovecs;
	bio->bi_io_vec = bvl;

	//LIBLCD_MSG("%s returned bio: %p", __func__, bio);
	return bio;

err_free:
#ifndef LCD_ISOLATE
	mempool_free(p, bs->bio_pool);
#endif
	return NULL;
}
EXPORT_SYMBOL(bio_alloc_bioset);

/**
 *	bio_add_pc_page	-	attempt to add page to bio
 *	@q: the target queue
 *	@bio: destination bio
 *	@page: page to add
 *	@len: vec entry length
 *	@offset: vec entry offset
 *
 *	Attempt to add a page to the bio_vec maplist. This can fail for a
 *	number of reasons, such as the bio being full or target block device
 *	limitations. The target block device must allow bio's up to PAGE_SIZE,
 *	so it is always possible to add a single page to an empty bio.
 *
 *	This should only be used by REQ_PC bios.
 */
int bio_add_pc_page(struct request_queue *q, struct bio *bio, struct page
		    *page, unsigned int len, unsigned int offset)
{
	int retried_segments = 0;
	struct bio_vec *bvec;

	/*
	 * cloned bio must not modify vec list
	 */
	if (unlikely(bio_flagged(bio, BIO_CLONED))) {
		LIBLCD_ERR("%s BIO_CLONED", __func__);
		return 0;
	}

	if (((bio->bi_iter.bi_size + len) >> 9) > queue_max_hw_sectors(q)) {
		LIBLCD_ERR("%s bi_size > queue_max_hw_sectors ", __func__);
		return 0;
	}

	/*
	 * For filesystems with a blocksize smaller than the pagesize
	 * we will often be called with the same page as last time and
	 * a consecutive offset.  Optimize this special case.
	 */
	if (bio->bi_vcnt > 0) {
		struct bio_vec *prev = &bio->bi_io_vec[bio->bi_vcnt - 1];

		if (page == prev->bv_page &&
		    offset == prev->bv_offset + prev->bv_len) {
			prev->bv_len += len;
			bio->bi_iter.bi_size += len;
			printk("%s adding to existing bio_vec %p", __func__, prev);
			goto done;
		}

		/*
		 * If the queue doesn't support SG gaps and adding this
		 * offset would create a gap, disallow it.
		 */
		if (bvec_gap_to_prev(q, prev, offset)) {
			printk("%s, bvec_gap_to_prev failed", __func__);
			return 0;
		}
	}

	if (bio->bi_vcnt >= bio->bi_max_vecs) {
		printk("%s, bi_vcnt %d bi_max_vecs %d", __func__, bio->bi_vcnt,
					bio->bi_max_vecs);
		return 0;
	}

	/*
	 * setup the new entry, we might clear it again later if we
	 * cannot add the page
	 */
	bvec = &bio->bi_io_vec[bio->bi_vcnt];
	bvec->bv_page = page;
	bvec->bv_len = len;
	bvec->bv_offset = offset;
	bio->bi_vcnt++;
	bio->bi_phys_segments++;
	bio->bi_iter.bi_size += len;

	/*
	 * Perform a recount if the number of segments is greater
	 * than queue_max_segments(q).
	 */

	while (bio->bi_phys_segments > queue_max_segments(q)) {

		if (retried_segments)
			goto failed;

		retried_segments = 1;
		blk_recount_segments(q, bio);
	}

	/* If we may be able to merge these biovecs, force a recount */
	if (bio->bi_vcnt > 1 && (BIOVEC_PHYS_MERGEABLE(bvec-1, bvec)))
		bio_clear_flag(bio, BIO_SEG_VALID);

 done:
	//LIBLCD_MSG("%s len: %d", __func__, len);
	return len;

 failed:
	bvec->bv_page = NULL;
	bvec->bv_len = 0;
	bvec->bv_offset = 0;
	bio->bi_vcnt--;
	bio->bi_iter.bi_size -= len;
	blk_recount_segments(q, bio);
	return 0;
}
EXPORT_SYMBOL(bio_add_pc_page);

static void bio_map_kern_endio(struct bio *bio)
{
	bio_put(bio);
}

/**
 *	bio_map_kern	-	map kernel address into bio
 *	@q: the struct request_queue for the bio
 *	@data: pointer to buffer to map
 *	@len: length in bytes
 *	@gfp_mask: allocation flags for bio allocation
 *
 *	Map the kernel address into a bio suitable for io to a block
 *	device. Returns an error pointer in case of error.
 */
struct bio *bio_map_kern(struct request_queue *q, void *data, unsigned int len,
			 gfp_t gfp_mask)
{
	unsigned long kaddr = (unsigned long)data;
	unsigned long end = (kaddr + len + PAGE_SIZE - 1) >> PAGE_SHIFT;
	unsigned long start = kaddr >> PAGE_SHIFT;
	const int nr_pages = end - start;
	int offset, i;
	struct bio *bio;

	bio = bio_kmalloc(gfp_mask, nr_pages);
	if (!bio) {
		LIBLCD_ERR("%s bio_kmalloc ret %p", __func__, bio);
		return ERR_PTR(-ENOMEM);
	}

	offset = offset_in_page(kaddr);
	if (!nr_pages) {
		printk("%s, nr_pages %d", __func__, nr_pages);
	}

	for (i = 0; i < nr_pages; i++) {
		unsigned int bytes = PAGE_SIZE - offset;

		if (len <= 0)
			break;

		if (bytes > len)
			bytes = len;

		if (bio_add_pc_page(q, bio, virt_to_page(data), bytes,
				    offset) < bytes) {
			/* we don't support partial mappings */
			printk("%s, bio_add_pc_page failed", __func__);
			bio_put(bio);
			return ERR_PTR(-EINVAL);
		}

		data += bytes;
		len -= bytes;
		offset = 0;
	}

	bio->bi_end_io = bio_map_kern_endio;
	//LIBLCD_MSG("%s retu bio %p", __func__, bio);
	return bio;
}
EXPORT_SYMBOL(bio_map_kern);

inline int bio_phys_segments(struct request_queue *q, struct bio *bio)
{
	if (unlikely(!bio_flagged(bio, BIO_SEG_VALID)))
		blk_recount_segments(q, bio);

	return bio->bi_phys_segments;
}
EXPORT_SYMBOL(bio_phys_segments);

static void bio_free_pages(struct bio *bio)
{
	struct bio_vec *bvec;
	int i;

	bio_for_each_segment_all(bvec, bio, i)
		__free_page(bvec->bv_page);
}

static void bio_copy_kern_endio(struct bio *bio)
{
	bio_free_pages(bio);
	bio_put(bio);
}

static void bio_copy_kern_endio_read(struct bio *bio)
{
	char *p = bio->bi_private;
	struct bio_vec *bvec;
	int i;

	bio_for_each_segment_all(bvec, bio, i) {
		memcpy(p, page_address(bvec->bv_page), bvec->bv_len);
		p += bvec->bv_len;
	}

	bio_copy_kern_endio(bio);
}


/**
 *	bio_copy_kern	-	copy kernel address into bio
 *	@q: the struct request_queue for the bio
 *	@data: pointer to buffer to copy
 *	@len: length in bytes
 *	@gfp_mask: allocation flags for bio and page allocation
 *	@reading: data direction is READ
 *
 *	copy the kernel address into a bio suitable for io to a block
 *	device. Returns an error pointer in case of error.
 */
struct bio *bio_copy_kern(struct request_queue *q, void *data, unsigned int len,
			  gfp_t gfp_mask, int reading)
{
	unsigned long kaddr = (unsigned long)data;
	unsigned long end = (kaddr + len + PAGE_SIZE - 1) >> PAGE_SHIFT;
	unsigned long start = kaddr >> PAGE_SHIFT;
	struct bio *bio;
	void *p = data;
	int nr_pages = 0;

	/*
	 * Overflow, abort
	 */
	if (end < start)
		return ERR_PTR(-EINVAL);

	nr_pages = end - start;
	bio = bio_kmalloc(gfp_mask, nr_pages);
	if (!bio)
		return ERR_PTR(-ENOMEM);

	while (len) {
		struct page *page;
		unsigned int bytes = PAGE_SIZE;

		if (bytes > len)
			bytes = len;

#ifdef LCD_ISOLATE
		page = lcd_alloc_pages(0, q->bounce_gfp | gfp_mask);
#else
		page = alloc_page(q->bounce_gfp | gfp_mask);
#endif
		if (!page)
			goto cleanup;

		if (!reading)
			memcpy(page_address(page), p, bytes);

		if (bio_add_pc_page(q, bio, page, bytes, 0) < bytes)
			break;

		len -= bytes;
		p += bytes;
	}

	if (reading) {
		bio->bi_end_io = bio_copy_kern_endio_read;
		bio->bi_private = data;
	} else {
		bio->bi_end_io = bio_copy_kern_endio;
		bio_set_op_attrs(bio, REQ_OP_WRITE, 0);
	}

	return bio;

cleanup:
	bio_free_pages(bio);
	bio_put(bio);
	return ERR_PTR(-ENOMEM);
}

/**
 * bio_advance - increment/complete a bio by some number of bytes
 * @bio:	bio to advance
 * @bytes:	number of bytes to complete
 *
 * This updates bi_sector, bi_size and bi_idx; if the number of bytes to
 * complete doesn't align with a bvec boundary, then bv_len and bv_offset will
 * be updated on the last bvec as well.
 *
 * @bio will then represent the remaining, uncompleted portion of the io.
 */
void bio_advance(struct bio *bio, unsigned bytes)
{
	if (bio_integrity(bio))
		bio_integrity_advance(bio, bytes);

	bio_advance_iter(bio, &bio->bi_iter, bytes);
}
EXPORT_SYMBOL(bio_advance);

/**
 * bio_endio - end I/O on a bio
 * @bio:	bio
 *
 * Description:
 *   bio_endio() will end I/O on the whole bio. bio_endio() is the preferred
 *   way to end I/O on a bio. No one should call bi_end_io() directly on a
 *   bio unless they own it and thus know that it has an end_io function.
 **/
void bio_endio(struct bio *bio)
{
again:
	if (!bio_remaining_done(bio))
		return;

	/*
	 * Need to have a real endio function for chained bios, otherwise
	 * various corner cases will break (like stacking block devices that
	 * save/restore bi_end_io) - however, we want to avoid unbounded
	 * recursion and blowing the stack. Tail call optimization would
	 * handle this, but compiling with frame pointers also disables
	 * gcc's sibling call optimization.
	 */
	if (bio->bi_end_io == bio_chain_endio) {
		bio = __bio_chain_endio(bio);
		goto again;
	}

	if (bio->bi_end_io)
		bio->bi_end_io(bio);
}
EXPORT_SYMBOL(bio_endio);
