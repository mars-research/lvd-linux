/*
 * glue_cspace.h
 *
 * This combines the cspace and cptr cache into one object.
 *
 * Motivation: Glue code uses cspaces for remote references, but
 * it doesn't need the cnode slot allocation and object insertion
 * to be decoupled.
 *
 * This just saves writing the same code over and over for the different
 * glue code instances.
 *
 * Copyright: University of Utah
 */
#ifndef LIBLCD_GLUE_CSPACE_H
#define LIBLCD_GLUE_CSPACE_H

#include <linux/slab.h>
#include <libcap.h>

struct glue_cspace {
	struct cspace *cspace;
	struct cptr_cache *cptr_cache;
};

static inline int glue_cspace_alloc_init(struct cap_type_system *ts,
					struct glue_cspace **out)
{
	struct glue_cspace *gc;
	int ret;

	gc = kmalloc(sizeof(*gc), GFP_KERNEL);
	if (!gc) {
		ret = -ENOMEM;
		goto fail1;
	}
	gc->cspace = cap_alloc_cspace();
	if (!gc->cspace) {
		ret = -ENOMEM;
		goto fail2;
	}
	ret = cap_init_cspace_with_type_system(gc->cspace, ts);
	if (ret)
		goto fail3;
	ret = cptr_cache_alloc(&gc->cptr_cache);
	if (ret)
		goto fail4;
	ret = cptr_cache_init(gc->cptr_cache);
	if (ret)
		goto fail5;

	*out = gc;

	return 0;

fail5:
	cptr_cache_free(gc->cptr_cache);
fail4:
	cap_destroy_cspace(gc->cspace);
fail3:		
	cap_free_cspace(gc->cspace);
fail2:
	kfree(gc);
fail1:
	return ret;
}

static inline void glue_cspace_destroy_free(struct glue_cspace *gc)
{
	cptr_cache_destroy(gc->cptr_cache);
	cptr_cache_free(gc->cptr_cache);
	cap_destroy_cspace(gc->cspace);
	cap_free_cspace(gc->cspace);
	kfree(gc);
}

static inline int glue_cspace_insert(struct glue_cspace *gc, void *object,
				cap_type_t type, cptr_t **out)
{
	cptr_t c;
	int ret;

	ret = cptr_alloc(gc->cptr_cache, &c);
	if (ret)
		goto fail1;

	ret = cap_insert(gc->cspace, c, object, type);
	if (ret)
		goto fail2;

	*out = c;

	return 0;

fail2:
	cptr_free(gc->cptr_cache, c);
fail1:
	return ret;
}

static inline int glue_cspace_remove(struct glue_cspace *gc, cptr_t c)
{
	/* Assumes no grants, etc. were done. */
	cap_delete(gc->cspace, c);
	cptr_free(gc->cptr_cache, c);
}

#endif /* LIBLCD_GLUE_CSPACE_H */
