/*
 * resource_tree.h
 *
 * Data structure for doing address -> cptr translation.
 *
 * We re-use interval trees from Linux.
 *
 * Copyright: University of Utah
 */

#include <libcap.h>
#include <linux/interval_tree.h>

struct lcd_resource_node {
	struct interval_tree_node resource_tree_node;
	unsigned long nr_pages_order;
	cptr_t cptr;
};

struct lcd_resource_tree {
	struct rb_root root;
};

/* what about when setting up heap? can't kmalloc data */

int lcd_resource_tree_alloc(struct lcd_resource_tree **t_out);

int lcd_resource_tree_init(struct lcd_resource_tree *t);

int lcd_resource_tree_destroy(struct lcd_resource_tree *t);

int lcd_resource_tree_free(struct lcd_resource_tree *t);

int lcd_resource_tree_insert(struct lcd_resource_tree *t,
			struct lcd_resource_node *n);

int lcd_resource_tree_search(struct lcd_resource_tree *t,
			unsigned long addr,
			struct lcd_resource_node **n_out);

void lcd_resource_tree_delete(struct lcd_resource_tree *t,
			struct lcd_resource_node *n);
