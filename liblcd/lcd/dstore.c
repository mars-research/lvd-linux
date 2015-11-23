/**
 * dstore.c - data store subsystem
 *
 * Authors:
 *     Charles Jacobsen <charlesj@cs.utah.edu>
 * Copyright: University of Utah
 *
 * Adapted from the capability subsystem in the microkernel.
 */

#include <lcd-domains/liblcd-config.h>

#include <linux/slab.h>
#include <linux/delay.h>
#include <lcd-domains/liblcd.h>

#include <lcd-domains/liblcd-hacks.h>

/* CONFIGURATION ---------------------------------------- */

/* These control the shape of the data store - its depth, width 
 * of tables, etc.
 */

#define LCD_DPTR_DEPTH_BITS  2    /* max depth of 3, zero indexed         */
#define LCD_DPTR_FANOUT_BITS 2    /* each level fans out by a factor of 4 */
#define LCD_DPTR_SLOT_BITS   2    /* each node contains 4 slots           */
#define LCD_DSTORE_NODE_TABLE_NUM_SLOTS ((1 << LCD_DPTR_SLOT_BITS) +	\
						(1 << LCD_DPTR_FANOUT_BITS))
#define LCD_DPTR_LEVEL_SHIFT (((1 << LCD_DPTR_DEPTH_BITS) - 1) * \
				LCD_DPTR_FANOUT_BITS + LCD_DPTR_SLOT_BITS)

/* DPTR DEFS -------------------------------------------------- */


static inline unsigned long lcd_dptr_slot(dptr_t d)
{
	/*
	 * Mask off low bits
	 */ 
	return dptr_val(d) & ((1 << LCD_DPTR_SLOT_BITS) - 1);
}

/* 
 * Gives fanout index for going *from* lvl to lvl + 1, where 
 * 0 <= lvl < 2^LCD_DPTR_DEPTH_BITS - 1 (i.e., we can't go anywhere
 * if lvl = 2^LCD_DPTR_DEPTH_BITS - 1, because we are at the deepest
 * level).
 */
static inline unsigned long lcd_dptr_fanout(dptr_t d, int lvl)
{
	unsigned long i;

	i = dptr_val(d);
	/*
	 * Shift and mask off bits at correct section
	 */
	i >>= (lvl * LCD_DPTR_FANOUT_BITS + LCD_DPTR_SLOT_BITS);
	i &= ((1 << LCD_DPTR_FANOUT_BITS) - 1);

	return i;
}
/*
 * Gives depth/level of cptr, zero indexed (0 means the root cnode table)
 */
static inline unsigned long lcd_dptr_level(dptr_t d)
{
	unsigned long i;

	i = dptr_val(d);
	/*
	 * Shift and mask
	 */
	i >>= LCD_DPTR_LEVEL_SHIFT;
	i &= ((1 << LCD_DPTR_DEPTH_BITS) - 1);

	return i;
}

/* 
 * DPTR CACHE
 */
struct dptr_cache {
	unsigned long *bmaps[1 << LCD_DPTR_DEPTH_BITS];
};

/* DATA STORE DEFS ---------------------------------------- */

enum allocation_state {
	ALLOCATION_INVALID,
	ALLOCATION_VALID,
	ALLOCATION_MARKED_FOR_DELETE,
	ALLOCATION_REMOVED,
};

/*
 * (The dstore derivation tree, the analogue of the cdt, may be pointless
 * right now since we don't allow grant, etc. with data stores. But maybe
 * in the future ...)
 */
struct dstore_node;
struct ddt_root_node {
	struct mutex lock;
	struct dstore_node *root_node;
	enum allocation_state state;
};

struct dstore;
struct dstore_node {
	struct mutex lock;
	/*
	 * dstore node data
	 */
	void *object;
	int tag;
	struct dstore *dstore;
	/*
	 * ddt data
	 */
	struct ddt_root_node *ddt_root;
	struct list_head children;
	struct list_head siblings;
};

struct dstore_node_table {
	struct dstore_node dstore_node[LCD_DSTORE_NODE_TABLE_NUM_SLOTS];
	uint8_t table_level;
	struct list_head table_list;
};

struct dstore {
	struct mutex lock;
	enum allocation_state state;
	struct dstore_node_table *root_table;
	struct kmem_cache *dstore_node_table_cache;
	struct dptr_cache *dptr_cache;
	struct list_head table_list;
};

/* DDT CACHE -------------------------------------------------- */

struct ddt_cache_t {
	struct mutex lock;
	struct kmem_cache *ddt_root_cache;
};

static struct ddt_cache_t ddt_cache;

/**
 * Allocates a new ddt root node using the ddt cache.
 */
struct ddt_root_node * get_ddt_root(void)
{
	int ret;
	struct ddt_root_node *ddt_node = NULL;
	ret = mutex_lock_interruptible(&ddt_cache.lock);
	if (ret) {
		LIBLCD_ERR("interrupted");
		goto out;
	}

	ddt_node = kmem_cache_alloc(ddt_cache.ddt_root_cache, 0);
	ddt_node->state = ALLOCATION_VALID;
	mutex_init(&ddt_node->lock);

	mutex_unlock(&ddt_cache.lock);

out:	
	return ddt_node;
}

void free_ddt_root(struct ddt_root_node *ddt_node)
{
	int ret;
	ret = mutex_lock_interruptible(&ddt_cache.lock);
	if (ret) {
		LIBLCD_ERR("interrupted");
		goto out;
	}

	ddt_node->state = ALLOCATION_REMOVED;	
	kmem_cache_free(ddt_cache.ddt_root_cache, ddt_node);

	mutex_unlock(&ddt_cache.lock);

out:
	return;
}

/* DPTR CACHE -------------------------------------------------- */

static int dptr_cache_init(struct dptr_cache **out)
{
	struct dptr_cache *cache;
	int ret;
	int i, j;
	int nbits;
	/*
	 * Allocate the container
	 */
	cache = kzalloc(sizeof(*cache), GFP_KERNEL);
	if (!cache) {
		ret = -ENOMEM;
		goto fail1;
	}
	/*
	 * Allocate the bitmaps
	 */
	for (i = 0; i < (1 << LCD_DPTR_DEPTH_BITS); i++) {
		/*
		 * For level i, we use the slot bits plus i * fanout bits
		 *
		 * So e.g. for level 0, we use only slot bits, so there
		 * are only 2^(num slot bits) cap slots at level 0.
		 */
		nbits = 1 << (LCD_DPTR_SLOT_BITS + i * LCD_DPTR_FANOUT_BITS);
		/*
		 * Alloc bitmap
		 */
		cache->bmaps[i] = kzalloc(sizeof(unsigned long) *
					BITS_TO_LONGS(nbits),
					GFP_KERNEL);
		if (!cache->bmaps[i]) {
			ret = -ENOMEM;
			goto fail2; /* i = level we failed at */
		}
	}
	/*
	 * Mark reserved dptr's as allocated
	 */
	set_bit(0, cache->bmaps[0]);

	*out = cache;

	return 0;

fail2:
	for (j = 0; j < i; j++)
		kfree(cache->bmaps[j]);
	kfree(cache);
fail1:
	return ret;
}

static void dptr_cache_destroy(struct dptr_cache *cache)
{
	int i;
	/*
	 * Free bitmaps
	 */
	for (i = 0; i < (1 << LCD_DPTR_DEPTH_BITS); i++)
		kfree(cache->bmaps[i]);
	/*
	 * Free container
	 */
	kfree(cache);
}

static int __lcd_alloc_dptr_from_bmap(unsigned long *bmap, int size,
				unsigned long *out)
{
	unsigned long idx;
	/*
	 * Find next zero bit
	 */
	idx = find_first_zero_bit(bmap, size);
	if (idx >= size)
		return 0; /* signal we are full */
	/*
	 * Set bit to mark cptr as in use
	 */
	set_bit(idx, bmap);

	*out = idx;

	return 1; /* signal we are done */
}

static int lcd_alloc_dptr(struct dptr_cache *cptr_cache, dptr_t *free_dptr)
{
	int ret;
	int depth;
	int done;
	unsigned long *bmap;
	unsigned long idx;
	int size;

	depth = 0;
	do {
		bmap = cptr_cache->bmaps[depth];
		size = 1 << (LCD_DPTR_SLOT_BITS + 
			depth * LCD_DPTR_FANOUT_BITS);
		done = __lcd_alloc_dptr_from_bmap(bmap, size, &idx);
		depth++;
	} while (!done && depth < (1 << LCD_DPTR_DEPTH_BITS));

	if (!done) {
		/*
		 * Didn't find one
		 */
		LIBLCD_ERR("out of cptrs");
		ret = -ENOMEM;
		goto fail2;
	}
	/*
	 * Found one; dec depth back to what it was, and encode
	 * depth in cptr
	 */
	depth--;
	idx |= (depth << LCD_DPTR_LEVEL_SHIFT);
	*free_dptr = __dptr(idx);

	return 0; 

fail2:
	return ret;
}

static void lcd_free_dptr(struct dptr_cache *dptr_cache, dptr_t d)
{
	unsigned long *bmap;
	unsigned long bmap_idx;
	unsigned long level;

	/*
	 * Get the correct level bitmap
	 */
	level = lcd_dptr_level(d);
	bmap = dptr_cache->bmaps[level];
	/*
	 * The bitmap index includes all fanout bits and the slot bits
	 */
	bmap_idx = ((1 << (LCD_DPTR_FANOUT_BITS * level + LCD_DPTR_SLOT_BITS))
		- 1) & dptr_val(d);
	/*
	 * Clear the bit in the bitmap
	 */
	clear_bit(bmap_idx, bmap);

	return; 
}

/* DATA STORE -------------------------------------------------- */

static int make_empty_dstore_node_table(struct dstore *dstore, 
					uint8_t level,
					struct dstore_node_table **new_out)
{
	struct dstore_node_table *new;
	int i;
	/*
	 * Allocate
	 */
	new = kmem_cache_zalloc(dstore->dstore_node_table_cache, 0);
	if (!new)
		goto fail1;
	/*
	 * Mark all cnodes in new table as free, and init spin lock.
	 * We delay some of the other set up until the cnode is
	 * actually used.
	 */
	for (i = 0; i < LCD_DSTORE_NODE_TABLE_NUM_SLOTS; i++) {
		new->dstore_node[i].tag = 0;
		INIT_LIST_HEAD(&new->dstore_node[i].children);
		INIT_LIST_HEAD(&new->dstore_node[i].siblings);
		mutex_init(&new->dstore_node[i].lock);
	}
	
	new->table_level = level;

	/*
	 * Add to dstore's list of tables. (This is used when we
	 * tear down the whole dstore. For now, we don't greedily free
	 * tables as they become empty, so we don't delete them from
	 * the list until the dstore is destroyed.)
	 */
	list_add(&new->table_list, &dstore->table_list);

	*new_out = new;

	return 0;
fail1:
	return -ENOMEM;
}

/**
 * Initializes the dstore's fields.
 */
int lcd_dstore_init_dstore(struct dstore **out)
{
	int ret;
	struct dstore *dstore;

	/*
	 * Alloc
	 */
	dstore = kzalloc(sizeof(*dstore), GFP_KERNEL);
	if (!dstore) {
		LIBLCD_ERR("alloc dstore");
		ret = -ENOMEM;
		goto fail1;
	}

	/*
	 * Init ...
	 */

	dstore->state = ALLOCATION_VALID;

	mutex_init(&dstore->lock);

	/*
	 * Initialize list of dstore tables
	 */
	INIT_LIST_HEAD(&dstore->table_list);

	/*
	 * Initialize the dstore node table cache
	 */
	dstore->dstore_node_table_cache = KMEM_CACHE(dstore_node_table, 0);
	if(!dstore->dstore_node_table_cache) {
		LIBLCD_ERR("failed to allocate dstore_node_table slab");
		ret = -ENOMEM;
		goto fail2;
	}

	/*
	 * Initialize the root dstore table
	 */
	ret = make_empty_dstore_node_table(dstore, 0, &dstore->root_table);
	if (ret) {
		LIBLCD_ERR("error initializing root dstore table");
		goto fail3;
	}

	/*
	 * Initialize the dptr cache
	 */
	ret = dptr_cache_init(&dstore->dptr_cache);
	if (ret) {
		LIBLCD_ERR("setting up dptr cache");
		goto fail4;
	}

	*out = dstore;
	
	return 0;

fail4:
fail3:
fail2:
	/* This can handle a partially set up dstore. */
	lcd_dstore_destroy(dstore);
fail1:
	return ret;
}

static int update_dstore_node_table(struct dstore *dstore,
			struct dstore_node_table *old, unsigned long level_id, 
			bool alloc, struct dstore_node_table **new)
{
	unsigned long index;
	/*
	 * The first half of the slots contain objects, the second half table
	 * pointers. Skip over cap slots by adding half the number of slots
	 * to level_id.
	 */
	index = level_id + (LCD_DSTORE_NODE_TABLE_NUM_SLOTS >> 1);

	if (old->dstore_node[index].tag == LCD_DSTORE_TAG_DSTORE_NODE) {
		/*
		 * Slot contains a dstore node that points to the next table.
		 */
		*new = old->dstore_node[index].object;

		return 0; /* signal we are not done yet */

	} else if (old->dstore_node[index].tag == 
		LCD_DSTORE_TAG_NULL && alloc) {
		/*
		 * The dstore node is free, and we can alloc.
		 *
		 * Allocate and init a new dstore node table
		 */
		make_empty_dstore_node_table(dstore, old->table_level + 1, new);
		/*
		 * Set up dstore node that points to it
		 */
		old->dstore_node[index].tag = LCD_DSTORE_TAG_DSTORE_NODE;
		old->dstore_node[index].object = *new;

		return 0; /* signal we are not done yet */
	} else {
		/*
		 * cnode free, invalid, etc.
		 */
		return -EINVAL; /* signal error in look up */
	}
}

static int find_dstore_node(struct dstore *dstore, 
			struct dstore_node_table *old, 
			unsigned long level_id, bool alloc, 
			struct dstore_node **dstore_node)
{
	/*
	 * The first slots contain capabilities
	 */
	if (old->dstore_node[level_id].tag != LCD_DSTORE_TAG_NULL && !alloc) {
		/*
		 * dstore node contains an object. We want to catch the case 
		 * when we expected to alloc.
		 */
		*dstore_node = &old->dstore_node[level_id];

		return 1; /* signal we found the slot and are done */

	} else if (old->dstore_node[level_id].tag == LCD_DSTORE_TAG_NULL &&
		alloc) {
		/*
		 * dstore node is empty, but we expected that (alloc is true).
		 *
		 * Initialize it.
		 */
		*dstore_node = &old->dstore_node[level_id];
		mutex_init(&(*dstore_node)->lock);
		(*dstore_node)->dstore = dstore;
		
		return 1; /* signal we found the slot and are done */
	} else {
		/*
		 * invalid indexing, etc.
		 */
		return -EINVAL; /* signal an error in look up */
	}
}

static int get_level_index(int table_level, dptr_t d, 
			unsigned long *level_id)
{
	/*
	 * Calculate the depth of the index
	 */
	if (lcd_dptr_level(d) == table_level) {
		/*
		 * We're at the final level - we're done, and need to look in 
		 * the object slots in the dstore node table
		 */
		*level_id = lcd_dptr_slot(d);
		return 0; /* signal no more levels to traverse */
	} else {
		/*
		 * More levels to go; determine index of next table to
		 * look at
		 */
		*level_id = lcd_dptr_fanout(d, table_level);
		return 1; /* signal more levels to traverse */
	}
}

static int walk_one_level(struct dstore *dstore, dptr_t d, bool alloc, 
			struct dstore_node_table *old, 
			struct dstore_node_table **new, 
			struct dstore_node **dstore_node)
{
	int more_levels;
	unsigned long level_id;

	more_levels = get_level_index(old->table_level, d, &level_id);
	if (more_levels)
		return update_dstore_node_table(dstore, old, level_id, alloc, 
						new);
	else
		return find_dstore_node(dstore, old, level_id, alloc, 
					dstore_node);
}

/**
 * Finds the dstore node in dstore indexed by dptr d. If alloc is true,
 * allocates dstore node tables on the fly while looking up d.
 *
 * ** Expects the dstore to be locked by the caller. **
 */
static int __lcd_dstore_node_lookup(struct dstore *dstore, dptr_t d, bool alloc,
				struct dstore_node **dstore_node) 
{

	int ret;
	struct dstore_node_table *old;
	struct dstore_node_table *new;

	/*
	 * If cptr is null, fail
	 */
	if (dptr_val(d) == dptr_val(LCD_DPTR_NULL))
		return -EINVAL;

	/*
	 * Initialize to root dstore node table
	 */
	old = dstore->root_table;
	do {
		/*
		 * Walk one level. Table gets updated to the next level's
		 * table, and dstore node will be non-NULL and point to the 
		 * final dstore node when we get there.
		 *
		 * walk_one_level returns 1 when we find the node, 0 when we
		 * should keep going, and < 0 on error.
		 */
		ret = walk_one_level(dstore, d, alloc, old, &new, 
				dstore_node);
		old = new;
	} while (!ret);

	/*
	 * only return non zero if we had an error
	 */
	return (ret < 0 ? ret : 0);
}

static int __lcd_dstore_node_get__(struct dstore *dstore, dptr_t d, 
			bool alloc, struct dstore_node **dstore_node)
{
	int ret;

	/*
	 * Look up and lock dstore node
	 */

	ret = mutex_lock_interruptible(&dstore->lock);
	if (ret) {
		LIBLCD_ERR("interrupted");
		goto fail1;
	}

	if (dstore->state != ALLOCATION_VALID) {
		LIBLCD_ERR("dstore state is not valid, state is %d", 
			dstore->state);
		ret = -EIDRM;
		goto fail2;
	}
	ret = __lcd_dstore_node_lookup(dstore, d, alloc, dstore_node);
	if(ret) {
		ret = -ENOMEM;
		goto fail2;
	}
	
	ret = mutex_lock_interruptible(&(*dstore_node)->lock);
	if (ret) {
		LIBLCD_ERR("interrupted");
		goto fail2;
	}

	mutex_unlock(&dstore->lock);

	return 0;

fail2:
	mutex_unlock(&dstore->lock);
fail1:
	return ret;
}

int lcd_dstore_insert(struct dstore *dstore, void *object, int tag,
		dptr_t *out)
{
	struct dstore_node *dstore_node;
	int ret;
	dptr_t d;

	/* Ensure tag is not one of the reserved tags. */
	if (tag == LCD_DSTORE_TAG_NULL ||
		tag == LCD_DSTORE_TAG_DSTORE_NODE) {
		return -EINVAL;
	}

	/*
	 * Alloc a dptr
	 */
	ret = lcd_alloc_dptr(dstore->dptr_cache, &d);
	if (ret) {
		LIBLCD_ERR("error alloc dptr");
		goto fail1;
	}

	/*
	 * Get cnode
	 */
	ret = __lcd_dstore_node_get__(dstore, d, true, &dstore_node);
	if (ret) {
		LIBLCD_ERR("error getting dstore node");
		goto fail2;
	}
	/*
	 * Set data
	 */
 	dstore_node->dstore = dstore;
	dstore_node->object = object; 	
	dstore_node->tag    = tag;
	/*
	 * Set up ddt
	 */
	dstore_node->ddt_root = get_ddt_root();
	dstore_node->ddt_root->root_node = dstore_node;
	/*
	 * Release node
	 */
	lcd_dstore_put(dstore_node);

	*out = d;

	return 0; 

fail2:
	lcd_free_dptr(dstore->dptr_cache, d);
fail1:
	return ret;
}

void *lcd_dstore_node_object(struct dstore_node *n)
{
	return n->object;
}

int lcd_dstore_get(struct dstore *dstore, dptr_t d, int tag, 
		struct dstore_node **out)
{
	struct dstore_node *dstore_node;
	int ret;
	/*
	 * Get cnode
	 */
	ret = __lcd_dstore_node_get__(dstore, d, false, &dstore_node);
	if (ret) {
		LIBLCD_ERR("error getting dstore node");
		return ret;
	}
	/*
	 * Check tag, and return object
	 */
	if (dstore_node->tag != tag) {
		LIBLCD_ERR("expected tag %d, node has tag %d",
			tag, dstore_node->tag);
		return -EINVAL;
	}

	*out = dstore_node;

	return 0;
}

void lcd_dstore_put(struct dstore_node *n)
{
	mutex_unlock(&n->lock);
}

/**
 * Does actual removal from ddt. Returns non-zero if dstore node is the last 
 * one in the ddt.
 *
 * ** Expects dstore node and ddt_root to be locked, in that order. **
 */
static int do_delete_from_ddt(struct dstore_node *dstore_node, 
			struct ddt_root_node *ddt_root)
{
	int last_node = 0;
	/*
	 * Make the dstore node's children the children of dstore node's 
	 * parent by putting them in the dstore node's siblings list. 
	 * (Note that list_splice does the right thing when children or 
	 * siblings is empty.)
	 */
	list_splice_init(&dstore_node->children, &dstore_node->siblings);
	/*
	 * If dstore node was the root node of the ddt, update the ddt's root.
	 */
	if (ddt_root->root_node == dstore_node) {
		/*
		 * See if anyone is in siblings - this will include node's
		 * children since we spliced lists. list_first_entry will
		 * look at the next list item, not the current node.
		 */
		if (!list_empty(&dstore_node->siblings)) {
			ddt_root->root_node = list_first_entry(
				&dstore_node->siblings, 
				struct dstore_node, siblings);
		} else {
			last_node = 1;
			ddt_root->root_node = NULL;
		}
	}
	/*
	 * Remove dstore node from list of siblings (won't break if siblings is
	 * empty).
	 */
	list_del_init(&dstore_node->siblings);

	return last_node;
}  

/**
 * Tries to lock ddt and remove dstore_node from it. Returns non-zero if 
 * dstore node successfully deleted.
 */
static int try_delete_dstore_node(struct dstore *dstore, 
				struct dstore_node *dstore_node)
{
	int last_node;
	struct ddt_root_node *ddt_node;
	/*
	 * Try to lock the cdt
	 */
	if (!mutex_trylock(&dstore_node->ddt_root->lock)) {
		/*
		 * Tell caller to retry
		 */
		return 0;
	}
	/*
	 * Get the ddt
	 */
	ddt_node = dstore_node->ddt_root;
	/*
	 * Delete the dstore node from the ddt
	 */
	last_node = do_delete_from_ddt(dstore_node, ddt_node);
	if (last_node)
		ddt_node->state = ALLOCATION_MARKED_FOR_DELETE;
	/*
	 * Unlock ddt
	 */
	mutex_unlock(&ddt_node->lock);

	if (last_node) {
		/*
		 * No more references to object in dstores. Tear down
		 * the ddt.
		 */
		free_ddt_root(ddt_node);
	}

	/*
	 * Mark dstore node as null, null out fields.
	 *
	 * XXX: We could possibly free up the dstore node table if all of its 
	 * nodes are free. For now, they just hang around and will be freed when
	 * the whole dstore is torn down. (Or reused if a new object is
	 * inserted there.)
	 */
	dstore_node->tag = LCD_DSTORE_TAG_NULL;
	dstore_node->object = NULL;
	dstore_node->ddt_root = NULL;
	/*
	 * Signal we are done
	 */
	return 1;
}

void lcd_dstore_delete(struct dstore *dstore, dptr_t d)
{
	struct dstore_node *dstore_node; 
	int done;
	int ret;

	/*
	 * Fail for null
	 */
	if (dptr_is_null(d)) {
		LIBLCD_ERR("trying to delete null dptr");
		return;
	}

	/*
	 * This entire thing has to go in a loop - we need to release
	 * the lock on node and try again until we can lock the ddt.
	 *
	 * (Note that until we get multithreading inside LCDs, this is
	 * probably unnecessary. Carried over from capability code.)
	 */
	do {
		/*
		 * Look up node
		 */
		ret = __lcd_dstore_node_get__(dstore, d, false, &dstore_node);
		if (ret) {
			LIBLCD_ERR("error getting dstore node");
			goto out1;
		}

		/*
		 * If the node is null, just return.
		 *
		 * (The node may not have been null the first time we tried
		 * to delete it, but when we reattempted, it was null'd by 
		 * someone else.)
		 */
		if (dstore_node->tag == LCD_DSTORE_TAG_NULL) {
			LIBLCD_MSG("dstore node already free");
			goto out2;
		}
	
		/*
		 * If dstore node is a table pointer, ignore and return.
		 */
		if (dstore_node->tag == LCD_DSTORE_TAG_DSTORE_NODE) {
			LIBLCD_MSG("ignoring attempt to delete invalid node");
			goto out2;
		}

		/*
		 * Delete the dstore node
		 */
		done = try_delete_dstore_node(dstore, dstore_node);
		/*
		 * Release node
		 */
		lcd_dstore_put(dstore_node);

		if (!done) {
			/*
			 * Someone else is using the ddt; wait 1 ms for him
			 * to finish.
			 */
			msleep(1);
		}

	} while (!done);

	/*
	 * Done. Return dptr to cache.
	 */
	lcd_free_dptr(dstore->dptr_cache, d);
		

	return;

out2:
out1:
	return;
}

static void dstore_node_tear_down(struct dstore_node *dstore_node, 
				struct dstore *dstore)
{
	int ret;
	int done;

	do {
		/*
		 * Lock node
		 */
		ret = mutex_lock_interruptible(&dstore_node->lock);
		if (ret) {
			LIBLCD_ERR("interrupted, skipping this node");
			goto out1;
		}
		/*
		 * If the node is already marked as null, just return.
		 */
		if (dstore_node->tag == LCD_DSTORE_TAG_NULL)
			goto out2;
		/*
		 * Delete the node
		 */
		done = try_delete_dstore_node(dstore, dstore_node);

		/*
		 * Release node
		 */
		mutex_unlock(&dstore_node->lock);

		if (!done) {
			/*
			 * Someone else is using the ddt; wait 1 ms for him
			 * to finish.
			 */
			msleep(1);
		}

	} while (!done);

	return;

out2:
	mutex_unlock(&dstore_node->lock);
out1:
	return;
}

static void dstore_node_table_tear_down(struct dstore_node_table *t, 
					struct dstore *dstore)
{
	int i;
	struct dstore_node *dstore_node;

	/*
	 * Loop over cap slots (first half), and tear down each node
	 */
	for (i = 0; i < (LCD_DSTORE_NODE_TABLE_NUM_SLOTS >> 1); i++) {

		dstore_node = &t->dstore_node[i];

		dstore_node_tear_down(dstore_node, dstore);

	}

	return;
}

static void dstore_tear_down(struct dstore *dstore)
{
	struct list_head *cursor, *next;
	struct dstore_node_table *t;

	list_for_each_safe(cursor, next, &dstore->table_list) {

		t = list_entry(cursor, struct dstore_node_table, table_list);
		/*
		 * Tear down the table
		 */
		dstore_node_table_tear_down(t, dstore);
		/*
		 * Delete it from list and free it
		 */
		list_del(&t->table_list);
		kmem_cache_free(dstore->dstore_node_table_cache, t);
	}

	return;
}

void lcd_dstore_destroy(struct dstore *dstore)
{
	int ret;

	ret = mutex_lock_interruptible(&dstore->lock);
	if (ret) {
		LIBLCD_ERR("interrupted");
		goto fail1;
	}

	if (dstore->state != ALLOCATION_VALID) {
		LIBLCD_ERR("dstore already freed?");
		goto fail2;
	}
	/*
	 * Mark it as being deleted
	 */
	dstore->state = ALLOCATION_MARKED_FOR_DELETE;
	/*
	 * Unlock 
	 */
	mutex_unlock(&dstore->lock);
	/*
	 * Free all of the dstore node tables
	 */
	dstore_tear_down(dstore);
	/*
	 * Free the dstore node table cache, and the dptr cache,
	 * if they were allocated.
	 */
	if (dstore->dstore_node_table_cache)
		kmem_cache_destroy(dstore->dstore_node_table_cache);
	if (dstore->dptr_cache)
		dptr_cache_destroy(dstore->dptr_cache);
	/*
	 * Free the dstore object
	 */
	kfree(dstore);

	return;

fail2:
	mutex_unlock(&dstore->lock);
fail1:
	return;
}

/* INIT / EXIT -------------------------------------------------- */

int lcd_dstore_init(void)
{
	/*
	 * Initialize ddt cache
	 */
	mutex_init(&ddt_cache.lock);
	ddt_cache.ddt_root_cache = KMEM_CACHE(ddt_root_node, 0);
	if(!ddt_cache.ddt_root_cache) {
		LIBLCD_ERR("failed to initialize ddt_root_node allocator");
		return -ENOMEM;
	}

	return 0;
}

void lcd_dstore_exit(void)
{
	kmem_cache_destroy(ddt_cache.ddt_root_cache);
}
