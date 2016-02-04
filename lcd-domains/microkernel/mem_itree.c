/**
 * mem_itree.c
 *
 * Global interval tree for tracking what host memory is
 * in the microkernel's capability system.
 *
 * Copyright: University of Utah
 */

#include <linux/interval_tree.h>
#include <linux/mutex.h>
#include "internal.h"

static struct mutex lcd_physical_mem_itree_lock;
static struct mutex lcd_vmalloc_mem_itree_lock;
static struct lcd_mem_itree lcd_physical_mem_itree;
static struct lcd_mem_itree lcd_vmalloc_mem_itree;

static int do_mem_itree_insert(struct lcd_mem_itree *tree,
			struct mutex *tree_lock,
			struct lcd_memory_object *mo, unsigned int flags)
{
	struct lcd_memory_itree_node *n;
	/*
	 * Create a new node
	 */
	n = kzalloc(sizeof(*n), GFP_KERNEL);
	if (!n) {
		LCD_ERR("kzalloc node");
		return -ENOMEM;
	}
	/*
	 * Insert into interval tree
	 */
	n->mo = mo;
	n->flags = flags;

	mutex_lock(tree_lock);
	interval_tree_insert(&n->it_node, &tree->root);
	mutex_unlock(tree_lock);

	return 0;
}

int __lcd_mem_itree_insert(struct lcd_memory_object *mo, unsigned int flags)
{
	switch (mo->sub_type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
		return do_mem_itree_insert(&lcd_physical_mem_itree,
					&lcd_physical_mem_itree_lock,
					mo,
					flags);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM:
		return do_mem_itree_insert(&lcd_vmalloc_mem_itree, 
					&lcd_vmalloc_mem_itree_lock,
					mo,
					flags);
	default:
		LCD_ERR("unexpected memory object type %d", 
			mo->sub_type);
	}
}

static int do_mem_itree_lookup(struct lcd_mem_itree *tree,
			struct mutex *tree_lock,
			unsigned long addr,
			struct lcd_mem_itree_node **node_out)
{
	/*
	 * Stab the intervals to see if any contain addr. Because
	 * our intervals are non-overlapping, there will be
	 * at most one match.
	 */
	struct interval_tree_node *match;

	mutex_lock(tree_lock);
	match = interval_tree_first(&tree->root, addr, addr);
	mutex_unlock(tree_lock);
	if (match) {
		n_out = container_of(match, struct lcd_mem_itree_node,
				it_node);
		return 0;
	} else {
		return -1; /* not found */
	}
}

int __lcd_mem_itree_lookup(unsigned long addr,
			enum lcd_microkernel_type_id type,
			struct lcd_mem_itree_node **node_out)
{
	switch (type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
		return do_mem_itree_lookup(&lcd_physical_mem_itree, 
					&lcd_physical_mem_itree_lock,
					addr,
					node_out);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM:
		return do_mem_itree_lookup(&lcd_vmalloc_mem_itree,
					&lcd_vmalloc_mem_itree_lock,
					addr,
					node_out);
	default:
		LCD_ERR("unexpected memory object type %d", type);
		return -EINVAL;
	}
}

static void do_mem_itree_delete(struct lcd_mem_itree *tree,
				struct mutex *tree_lock,
				struct lcd_mem_itree_node *node)
{
	mutex_lock(tree_lock);
	interval_tree_remove(&node->it_node, &tree->root);
	mutex_unlock(tree_lock);
}

void __lcd_mem_itree_delete(struct lcd_mem_itree_node *node)
{
	switch (node->mo->sub_type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
		return do_mem_itree_delete(&lcd_physical_mem_itree, 
					&lcd_physical_mem_itree_lock,
					node);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM:
		return do_mem_itree_delete(&lcd_vmalloc_mem_itree, 
					&lcd_vmalloc_mem_itree_lock,
					node);
	default:
		LCD_ERR("unexpected memory object type %d", 
			node->mo->sub_type);
	}
}

int __lcd_mem_itree_init(void)
{
	/*
	 * Initialize the two trees and locks
	 */
	lcd_physical_mem_itree.root = RB_ROOT;
	lcd_vmalloc_mem_itree.root = RB_ROOT;
	mutex_init(&lcd_physical_mem_itree_lock);
	mutex_init(&lcd_vmalloc_mem_itree_lock);

	return 0;
}

void __lcd_mem_itree_exit(void)
{
	/*
	 * For now, just check if the trees are non-empty.
	 *
	 * They should be empty if all LCDs (isolated and non-isolated)
	 * were torn down properly.
	 */
	BUG_ON(!RB_EMPTY_ROOT(&lcd_physical_mem_itree.root));
	BUG_ON(!RB_EMPTY_ROOT(&lcd_vmalloc_mem_itree.root));
}
