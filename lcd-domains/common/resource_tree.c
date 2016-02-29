/*
 * resource_tree.c
 *
 * Implementation of liblcd resource tree. This is
 * utilized for doing address -> cptr translation.
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <linux/slab.h>
#include <linux/rbtree.h>
#include <linux/interval_tree.h>
#include <linux/interval_tree_generic.h>
#include <libcap.h>
#include <liblcd/liblcd.h>

#include <lcd_config/post_hook.h>

/*
 * Interval tree functions are not exported (in lib/interval_tree.c), 
 * so we just make our own (this is necessary for liblcd anyway).
 */

#define START(node) ((node)->start)
#define LAST(node)  ((node)->last)

INTERVAL_TREE_DEFINE(struct interval_tree_node, rb,
		     unsigned long, __subtree_last,
		     START, LAST,, interval_tree)

/* RESOURCE TREES -------------------------------------------------- */ 

int lcd_resource_tree_init(struct lcd_resource_tree *t)
{
	/*
	 * Init interval tree.
	 */
	t->root = RB_ROOT;
	return 0;
}

void lcd_resource_tree_insert(struct lcd_resource_tree *t,
			struct lcd_resource_node *n)
{
	/*
	 * Insert into interval tree
	 */
	interval_tree_insert(&n->it_node, &t->root);
}

int lcd_resource_tree_search(struct lcd_resource_tree *t,
			unsigned long addr,
			struct lcd_resource_node **n_out)
{
	/*
	 * Stab the intervals to see if any contain addr. Because
	 * our intervals are non-overlapping, there will be
	 * at most one match.
	 */
	struct interval_tree_node *match;

	match = interval_tree_iter_first(&t->root, addr, addr);
	if (match) {
		*n_out = container_of(match, struct lcd_resource_node,
				it_node);
		return 0;
	} else {
		return -1; /* not found */
	}
}

struct lcd_resource_node * 
lcd_resource_tree_first(struct lcd_resource_tree *t)
{
	struct rb_node *n;
	
	n = rb_first(&t->root);
	if (n)
		return container_of(
			container_of(n, struct interval_tree_node, rb),
			struct lcd_resource_node,
			it_node);
	else
		return NULL; /* empty tree */
}

struct lcd_resource_node *
lcd_resource_tree_next(struct lcd_resource_node *n)
{
	struct rb_node *rb_node = rb_next(&n->it_node.rb);
	if (rb_node) {
		return container_of(
			container_of(rb_node, struct interval_tree_node, rb),
			struct lcd_resource_node,
			it_node);
	} else
		return NULL;
}

void lcd_resource_tree_remove(struct lcd_resource_tree *t,
			struct lcd_resource_node *n)
{
	interval_tree_remove(&n->it_node, &t->root);
}

void lcd_resource_tree_dump(struct lcd_resource_tree *t)
{
	struct lcd_resource_node *cursor;

	LIBLCD_MSG("  DUMPING RESOURCE TREE 0x%p:", t);

	cursor = lcd_resource_tree_first(t);
	while (cursor) {
		printk("  node:\n");
		printk("    start: 0x%lx\n",
			lcd_resource_node_start(cursor));
		printk("    last: 0x%lx\n",
			lcd_resource_node_last(cursor));
		printk("    size: 0x%lx\n",
			lcd_resource_node_size(cursor));
		printk("    cptr: 0x%lx\n",
			cptr_val(lcd_resource_node_cptr(cursor)));
		printk("    flags: 0x%x\n",
			lcd_resource_node_flags(cursor));

		cursor = lcd_resource_tree_next(cursor);
	}

	LIBLCD_MSG("  END RESOURCE TREE DUMP");
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(lcd_resource_tree_init);
EXPORT_SYMBOL(lcd_resource_tree_insert);
EXPORT_SYMBOL(lcd_resource_tree_search);
EXPORT_SYMBOL(lcd_resource_tree_first);
EXPORT_SYMBOL(lcd_resource_tree_next);
EXPORT_SYMBOL(lcd_resource_tree_remove);
EXPORT_SYMBOL(lcd_resource_tree_dump);
