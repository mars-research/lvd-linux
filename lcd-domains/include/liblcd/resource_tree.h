/*
 * resource_tree.h
 *
 * Data structure for tracking memory objects tracked
 * by the microkernel and accessible to a thread. This
 * is primarily used for doing address -> cptr translation.
 *
 * We use red-black trees from Linux. Intervals stored in
 * the tree are assumed to be non-overlapping.
 *
 * No locking (just like the other bare bones data structures).
 *
 * Copyright: University of Utah
 */
#ifndef LCD_DOMAINS_RESOURCE_TREE_H
#define LCD_DOMAINS_RESOURCE_TREE_H

#include <libcap.h>
#include <linux/rbtree.h>

/**
 * struct lcd_resource_tree
 *
 * Represents root of resource tree. You are responsible
 * for allocating this struct in any way you like. But you
 * should call lcd_resource_tree_init before using it.
 */
struct lcd_resource_tree {
	struct rb_root root;
};

/**
 * struct lcd_resource_node
 *
 * Represents node in LCD resource interval tree. Before
 * you insert this node into the tree, you should initialize:
 *
 *    -- start and last
 *    -- cptr is the cptr to the capability in the thread's cspace
 *    -- flags
 *
 * You are responsible for allocating nodes and freeing
 * them when you're done with them.
 *
 * You can use flags in any way you wish. Some common flags are
 * defined below.
 */
struct lcd_resource_node {
	struct rb_node rb_node;
	unsigned long start;
	unsigned long last;
	cptr_t cptr;
	unsigned int flags;
};

enum {
	LCD_RESOURCE_NODE_STATIC  = 1, /* resource node statically alloc'd */
	LCD_RESOURCE_NODE_KMALLOC = 2, /* resource node alloc'd via kmallc */
};

static inline unsigned long
lcd_resource_node_start(struct lcd_resource_node *n)
{
	return n->start;
}

static inline unsigned long
lcd_resource_node_last(struct lcd_resource_node *n)
{
	return n->last;
}

static inline unsigned long 
lcd_resource_node_size(struct lcd_resource_node *n)
{
	return lcd_resource_node_last(n) - lcd_resource_node_start(n) + 1;
}

static inline cptr_t
lcd_resource_node_cptr(struct lcd_resource_node *n)
{
	return n->cptr;
}

static inline unsigned int
lcd_resource_node_flags(struct lcd_resource_node *n)
{
	return n->flags;
}

/* INTERFACE -------------------------------------------------- */

/**
 * lcd_resource_tree_init -- Initialize tree
 * @t: the tree to init
 *
 * Call this before using any other functions on tree.
 */
int lcd_resource_tree_init(struct lcd_resource_tree *t);

/**
 * lcd_resource_tree_insert -- Insert interval into tree
 * @t: the tree to insert into
 * @n: the node to insert
 *
 * You should initialize the resource node before invoking
 * insert. See the doc for struct lcd_resource_node.
 * The tree will become invalid if you fail to do so.
 */
void lcd_resource_tree_insert(struct lcd_resource_tree *t,
			struct lcd_resource_node *n);

/**
 * lcd_resource_tree_search -- See if (@addr,@cptr) pair is contained in an 
 *                             interval in tree
 * @t: the tree to search in
 * @addr: the address to search for
 * @cptr: the cptr to search for
 * @n_out: out param, interval containing @addr if found
 *
 * Returns 0 if found, -1 if not found. @addr need not be the starting
 * address of the interval, just some address in the interval you are
 * searching for.
 */
int lcd_resource_tree_search(struct lcd_resource_tree *t,
			unsigned long addr,
			cptr_t cptr,
			struct lcd_resource_node **n_out);
/**
 * lcd_resource_tree_search_addr -- See if @addr is contained in an interval 
 *                                  in tree
 * @t: the tree to search in
 * @addr: the address to search for
 * @n_out: out param, interval containing @addr if found
 *
 * Returns 0 if found, -1 if not found. If more than one interval contains
 * @addr (this should only happen in the same interval has been inserted
 * more than once), the first node found is returned.
 */
int lcd_resource_tree_search(struct lcd_resource_tree *t,
			unsigned long addr,
			struct lcd_resource_node **n_out);

/**
 * lcd_resource_tree_first -- Return first node in tree
 * @t: the tree
 *
 * This is a wrapper around rb_first (*not* interval_tree_first). 
 * See its documentation. This is for iterating over the whole tree.
 *
 * Returns NULL if tree is empty.
 */
struct lcd_resource_node * 
lcd_resource_tree_first(struct lcd_resource_tree *t);

/**
 * lcd_resource_tree_next -- Return next node in iteration
 * @n: the node to compute next for
 *
 * This is a wrapper around rb_next (*not* interval_tree_next). See
 * its documentation.
 *
 * Returns NULL if there are no more nodes.
 *
 * Here is an iteration example:
 *
 *    struct lcd_resource_node *node;
 *    for (node = lcd_resource_tree_first(&mytree); 
 *         node; 
 *         node = lcd_resource_tree_next(node)) {
 *
 *           ... do something with node ...
 *    }
 */
struct lcd_resource_node *
lcd_resource_tree_next(struct lcd_resource_node *n);

/**
 * lcd_resource_tree_remove -- Remove node from tree
 * @t: tree to remove node from
 * @n: node to remove
 *
 * This will not free @n. You are responsible for the mem
 * mgt of @n.
 */
void lcd_resource_tree_remove(struct lcd_resource_tree *t,
			struct lcd_resource_node *n);

/**
 * lcd_resource_tree_dump -- Print resource tree to console, for debugging
 * @t: tree to dump
 */
void lcd_resource_tree_dump(struct lcd_resource_tree *t);

#endif /* LCD_DOMAINS_RESOURCE_TREE_H */
