/*
 * resource_tree.c
 *
 * Implementation of liblcd resource tree. This is
 * utilized for doing address -> cptr translation.
 *
 * Copyright: University of Utah
 */

#include <linux/slab.h>
#include <linux/interval_tree.h>
#include <linux/rb_tree.h>
#include <libcap.h>
#include <lcd-domains/liblcd.h>

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

	match = interval_tree_first(&t->root, addr, addr);
	if (match) {
		n_out = container_of(match, struct lcd_resource_node,
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
	return container_of(
		container_of(rb_next(n), struct interval_tree_node, rb),
		struct lcd_resource_node,
		it_node);
}

void lcd_resource_tree_remove(struct lcd_resource_tree *t,
			struct lcd_resource_node *n)
{
	interval_tree_remove(&n->it_node, &t->root);
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(lcd_resource_tree_init);
EXPORT_SYMBOL(lcd_resource_tree_insert);
EXPORT_SYMBOL(lcd_resource_tree_search);
EXPORT_SYMBOL(lcd_resource_tree_first);
EXPORT_SYMBOL(lcd_resource_tree_next);
EXPORT_SYMBOL(lcd_resource_tree_remove);
