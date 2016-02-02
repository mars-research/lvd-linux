/*
 * resource_tree.h
 *
 * Data structure for tracking memory objects tracked
 * by the microkernel and accessible to a thread. This
 * is primarily used for doing address -> cptr translation.
 *
 * We re-use interval trees from Linux.
 *
 * No locking (just like the other bare bones data structures).
 *
 * Copyright: University of Utah
 */

#include <libcap.h>
#include <linux/rb_tree.h>
#include <linux/interval_tree.h>

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
 *    -- it_node's start and last (see linux/interval_tree.h)
 *    -- nr_pages_order: the size of the memory resource is 
 *       2^nr_pages_order
 *    -- cptr is the cptr to the capability in the thread's cspace
 *
 * You are responsible for allocating these things and freeing
 * them when you're done with them.
 *
 * You can use flags in any way you wish. Some common flags are
 * defined below.
 */
struct lcd_resource_node {
	struct interval_tree_node it_node;
	unsigned long nr_pages_order;
	cptr_t cptr;
	unsigned int flags;
};

enum {
	LCD_RESOURCE_NODE_STATIC  = 0, /* resource node statically alloc'd */
	LCD_RESOURCE_NODE_KMALLOC = 1, /* resource node alloc'd via kmallc */
};

static inline unsigned long
lcd_resource_node_start(struct lcd_resource_node *n)
{
	return n->it_node.start;
}

static inline unsigned long
lcd_resource_node_last(struct lcd_resource_node *n)
{
	return n->it_node.last;
}

static inline unsigned long 
lcd_resource_node_order(struct lcd_resource_node *n)
{
	return n->nr_pages_order;
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
 * lcd_resource_tree_search -- See if @addr is contained in an interval in tree
 * @t: the tree to search in
 * @addr: the address to search for
 * @n_out: out param, interval containing @addr if found
 *
 * Returns 0 if found, -1 if not found.
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
