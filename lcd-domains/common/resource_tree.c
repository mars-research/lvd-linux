/*
 * resource_tree.c
 *
 * Implementation of liblcd resource tree. This is
 * utilized for doing address -> cptr translation.
 *
 * ------------------------------------------------------------
 *
 * We use Linux red-black trees. Each node in the tree
 * contains a (start address, last address, cptr) triple. No
 * two nodes in the same tree have the same triple, and nodes
 * are not allowed to have overlapping (start, last) intervals - UNLESS
 * they are identical. More precisely, given X and Y, the following
 * is *NOT* allowed (and will be rejected by the code below):
 *
 *   -- [X.start, X.last] and [Y.start, Y.last] overlap but the intervals
 *      are not the same.
 *
 * If the intervals are the same, the cptr's will differentiate
 * the nodes. (This ugliness is necessary for non-isolated code
 * usage - the same address interval may be inserted multiple times
 * but with different cptr's.)
 *
 * Given all of this, nodes are ordered in the following way: Given 
 * nodes X and Y:
 *
 *          X < Y iff  X.start < Y.start OR
 *                       X.start == Y.start && 
 *                       X.last  == Y.last  && 
 *                       X.cptr < Y.cptr
 *
 *         X == Y iff  the triples are equal
 *
 *          X > Y iff  X.start > Y.start OR
 *                       X.start == Y.start &&
 *                       X.last  == Y.last  &&
 *                       X.cptr > Y.cptr
 *
 * Put differently, 
 *
 *   -- a parent's left children have either smaller starting values,
 *      or their intervals are identical but they have smaller cptr
 *      values
 *
 *   -- a parent's right children have either bigger starting values,
 *      or their intervals are identical but they have bigger cptr
 *      values
 *
 * Also, the following is assumed to be avoided, but won't break anything
 * if violated:
 *
 *   -- X.cptr == Y.cptr && X.start != Y.start && X.last != Y.last. The
 *      code below will not prevent such an insert from happening.
 *
 * There are *TWO* search algorithms:
 *
 *      1 - Search by (addr, cptr) pair. The algorithm finds the
 *          (unique) node X such that addr is in [X.start, X.last] and
 *          cptr = X.cptr.
 *
 *          Given the current node N (starting with the root),
 *
 *              if addr < N.start, go left
 *
 *              if addr > N.last, go right
 *
 *              if addr in [N.start, N.last] and cptr == N.cptr we're done
 *
 *              if addr in [N.start, N.last] but cptr < N.cptr, go left
 *
 *              if addr in [N.start, N.last] but cptr > N.cptr, go right
 *
 *      2 - Search by addr only. The algorithm finds the first matching
 *          node X such that addr is in [X.start, X.last]. The algorithm
 *          works just like (1), but when we find a node N such that
 *          addr in [N.start, N.last], we return N (don't look at cptr).
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <linux/slab.h>
#include <linux/rbtree.h>
#include <libcap.h>
#include <liblcd/liblcd.h>

#include <lcd_config/post_hook.h>

/* RESOURCE TREES -------------------------------------------------- */ 

int lcd_resource_tree_init(struct lcd_resource_tree *t)
{
	/*
	 * Init interval tree.
	 */
	t->root = RB_ROOT;
	return 0;
}

static inline int addr_node_cmp(unsigned long addr,
				struct lcd_resource_node *n)
{
	/*
	 * Check if addr is in the node's interval
	 */
	if (addr < lcd_resource_node_start(n))
		return -1;
	if (addr > lcd_resource_node_last(n))
		return 1;
	return 0; /* in interval */
}

static inline int addr_cptr_node_cmp(unsigned long addr, cptr_t cptr,
				struct lcd_resource_node *n)
{
	/*
	 * Check if addr is even in the node's interval
	 */
	if (addr < lcd_resource_node_start(n))
		return -1;
	if (addr > lcd_resource_node_last(n))
		return 1;
	/*
	 * Addr is in interval; compare cptr's now
	 */
	if (cptr_val(cptr) < cptr_val(lcd_resource_node_cptr(n)))
		return -1;
	if (cptr_val(cptr) > cptr_val(lcd_resource_node_cptr(n)))
		return 1;
	/*
	 * Addr is in interval, and cptr's are equal; we have a match.
	 */
	return 0;
}

static inline int node_cmp(struct lcd_resource_node *n1,
			struct lcd_resource_node *n2)
{
	unsigned long left_addr = lcd_resource_node_start(n1);
	unsigned long right_addr = lcd_resource_node_start(n2);
	unsigned long left_cptr = cptr_val(lcd_resource_node_cptr(n1));
	unsigned long right_cptr = cptr_val(lcd_resource_node_cptr(n2));

	/* Sort in dictionary order on (start addr, cptr val) */
	if (left_addr < right_addr)
		return -1;
	if (right_addr < left_addr)
		return 1;
	if (left_cptr < right_cptr)
		return -1;
	if (left_cptr > right_cptr)
		return 1;
	return 0; /* equal */
}

static inline int in_interval(unsigned long addr,
			unsigned long i_lo, unsigned long i_hi)
{
	return addr >= i_lo && addr <= i_hi;
}

static inline int overlapping_non_equal_intervals(
	struct lcd_resource_node *n1,
	struct lcd_resource_node *n2)
{
	int ret = in_interval(lcd_resource_node_start(n1),
			lcd_resource_node_start(n2),
			lcd_resource_node_last(n2)) ||
		in_interval(lcd_resource_node_start(n2),
			lcd_resource_node_start(n1),
			lcd_resource_node_last(n1));
	if (unlikely(ret))
		ret = (lcd_resource_node_start(n1) != 
			lcd_resource_node_start(n2)) ||
			(lcd_resource_node_last(n1) != 
				lcd_resource_node_last(n2));
	return ret;
}

void lcd_resource_tree_insert(struct lcd_resource_tree *t,
			struct lcd_resource_node *n)
{
	/*
	 * This code was adapted from the rbtree documentation.
	 */
  	struct rb_node **new = &(t->root.rb_node), *parent = NULL;

  	/* Figure out where to put new node */
  	while (*new) {
  		struct lcd_resource_node *this = 
			container_of(*new, struct lcd_resource_node, rb_node);
		int result;

		/* We don't allow overlapping intervals *unless* the intervals
		 * are identical */
		if (unlikely(overlapping_non_equal_intervals(n, this))) {
			LIBLCD_ERR("OVERLAPPING INTERVALS:");
			lcd_resource_node_dump(n);
			lcd_resource_node_dump(this);
			BUG();
		}

  		result = node_cmp(n, this);

		parent = *new;
  		if (result < 0)
  			new = &((*new)->rb_left);
  		else if (result > 0)
  			new = &((*new)->rb_right);
		else {
			/* The caller shouldn't be trying to insert a 
			 * complete duplicate */
			BUG();
		}
  	}

  	/* Add new node and rebalance tree. */
  	rb_link_node(&n->rb_node, parent, new);
  	rb_insert_color(&n->rb_node, &t->root);
}

int lcd_resource_tree_search(struct lcd_resource_tree *t,
			unsigned long addr,
			cptr_t cptr,
			struct lcd_resource_node **n_out)
{
	struct rb_node *node = t->root.rb_node;

  	while (node) {
  		struct lcd_resource_node *this = 
			container_of(node, struct lcd_resource_node, rb_node);
		int result;

		result = addr_cptr_node_cmp(addr, cptr, this);

		if (result < 0)
  			node = node->rb_left;
		else if (result > 0)
  			node = node->rb_right;
		else {
  			*n_out = this;
			return 0;
		}
	}
	return -1; /* not found */
}

int lcd_resource_tree_search_addr(struct lcd_resource_tree *t,
			unsigned long addr,
			struct lcd_resource_node **n_out)
{
	struct rb_node *node = t->root.rb_node;

  	while (node) {
  		struct lcd_resource_node *this = 
			container_of(node, struct lcd_resource_node, rb_node);
		int result;

		result = addr_node_cmp(addr, this);

		if (result < 0)
  			node = node->rb_left;
		else if (result > 0)
  			node = node->rb_right;
		else {
  			*n_out = this;
			return 0;
		}
	}
	return -1; /* not found */
}

struct lcd_resource_node * 
lcd_resource_tree_first(struct lcd_resource_tree *t)
{
	struct rb_node *n;
	
	n = rb_first(&t->root);
	if (n)
		return container_of(n,
			struct lcd_resource_node,
			rb_node);
	else
		return NULL; /* empty tree */
}

struct lcd_resource_node *
lcd_resource_tree_next(struct lcd_resource_node *n)
{
	struct rb_node *rb_node = rb_next(&n->rb_node);
	if (rb_node) {
		return container_of(rb_node,
			struct lcd_resource_node,
				rb_node);
	} else
		return NULL;
}

void lcd_resource_tree_remove(struct lcd_resource_tree *t,
			struct lcd_resource_node *n)
{
	rb_erase(&n->rb_node, &t->root);
}

void lcd_resource_node_dump(struct lcd_resource_node *n)
{
	printk("  node:\n");
	printk("    start: 0x%lx\n",
		lcd_resource_node_start(n));
	printk("    last: 0x%lx\n",
		lcd_resource_node_last(n));
	printk("    size: 0x%lx\n",
		lcd_resource_node_size(n));
	printk("    cptr: 0x%lx\n",
		cptr_val(lcd_resource_node_cptr(n)));
	printk("    flags: 0x%x\n",
		lcd_resource_node_flags(n));
}

void lcd_resource_tree_dump(struct lcd_resource_tree *t)
{
	struct lcd_resource_node *cursor;

	LIBLCD_MSG("  DUMPING RESOURCE TREE 0x%p:", t);

	cursor = lcd_resource_tree_first(t);
	while (cursor) {

		lcd_resource_node_dump(cursor);

		cursor = lcd_resource_tree_next(cursor);
	}

	LIBLCD_MSG("  END RESOURCE TREE DUMP");
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(lcd_resource_tree_init);
EXPORT_SYMBOL(lcd_resource_tree_insert);
EXPORT_SYMBOL(lcd_resource_tree_search);
EXPORT_SYMBOL(lcd_resource_tree_search_addr);
EXPORT_SYMBOL(lcd_resource_tree_first);
EXPORT_SYMBOL(lcd_resource_tree_next);
EXPORT_SYMBOL(lcd_resource_tree_remove);
EXPORT_SYMBOL(lcd_resource_tree_dump);
