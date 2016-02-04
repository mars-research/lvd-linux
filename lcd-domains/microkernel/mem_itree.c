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

static struct lcd_mem_itree lcd_physical_mem_itree;
static struct lcd_mem_itree lcd_vmalloc_mem_itree;

static int do_mem_itree_insert(struct lcd_mem_itree *tree,
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
	mutex_init(&n->lock);
	n->mo = mo;
	n->flags = flags;
	/*
	 * Set up the start and end address, using memory object
	 */
	n->it_node.start = __lcd_memory_object_start(mo);
	n->it_node.last = __lcd_memory_object_last(mo);
	/*
	 * Insert into tree
	 */
	mutex_lock(&tree->lock);
	interval_tree_insert(&n->it_node, &tree->root);
	mutex_unlock(&tree->lock);

	return 0;
}

int __lcd_mem_itree_insert(struct lcd_memory_object *mo, unsigned int flags)
{
	switch (mo->sub_type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
		return do_mem_itree_insert(&lcd_physical_mem_itree,
					mo,
					flags);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM:
		return do_mem_itree_insert(&lcd_vmalloc_mem_itree, 
					mo,
					flags);
	default:
		LCD_ERR("unexpected memory object type %d", 
			mo->sub_type);
	}
}

static int do_mem_itree_lookup_nolock(struct lcd_mem_itree *tree,
			unsigned long addr,
			struct lcd_mem_itree_node **node_out)
{
	/*
	 * Stab the intervals to see if any contain addr. Because
	 * our intervals are non-overlapping, there will be
	 * at most one match.
	 */
	struct interval_tree_node *match;

	match = interval_tree_first(&tree->root, addr, addr);
	if (match) {
		node_out = container_of(match, struct lcd_mem_itree_node,
					it_node);
		mutex_lock(&(*node_out)->lock);
		return 0;
	} else {
		return -1; /* not found */
	}
}

static int do_mem_itree_lookup(struct lcd_mem_itree *tree,
			unsigned long addr,
			struct lcd_mem_itree_node **node_out)
{
	int ret;

	mutex_lock(&tree->lock);

	ret = do_mem_itree_lookup(tree, addr, node_out);

	mutex_unlock(&tree->lock);

	return ret;
}

int __lcd_mem_itree_get(unsigned long addr,
			enum lcd_microkernel_type_id type,
			struct lcd_mem_itree_node **node_out)
{
	switch (type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
		return do_mem_itree_lookup(&lcd_physical_mem_itree, 
					addr,
					node_out);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM:
		return do_mem_itree_lookup(&lcd_vmalloc_mem_itree,
					addr,
					node_out);
	default:
		LCD_ERR("unexpected memory object type %d", type);
		return -EINVAL;
	}
}

int __lcd_mem_itree_put(struct lcd_mem_itree_node *node)
{
	mutex_unlock(&node->lock);
}

static void do_mem_itree_delete(struct lcd_mem_itree *tree,
				struct lcd_memory_object *mo)
{
	struct lcd_mem_itree_node *node;
	int ret;

	mutex_lock(&tree->lock);

	/*
	 * Find the tree node that contains the memory object
	 */
	ret = do_mem_itree_lookup_nolock(tree,
					__lcd_memory_object_start(mo),
					&node);
	if (ret) {
		LCD_ERR("couldn't find memory object in tree, skipping delete");
		mutex_unlock(&tree->lock);
		BUG();
		return;
	}
	/*
	 * We now have the node locked and the tree locked, so no one
	 * can have a reference. Free to unlock and delete tree node.
	 */
	interval_tree_remove(&node->it_node, &tree->root);

	kfree(node);

	mutex_unlock(&tree->lock);
}

void __lcd_mem_itree_delete(struct lcd_memory_object *mo)
{
	switch (mo->sub_type) {
	case LCD_MICROKERNEL_TYPE_ID_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE:
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_DEV_MEM:
		return do_mem_itree_delete(&lcd_physical_mem_itree, mo);
	case LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_VMALLOC_MEM:
		return do_mem_itree_delete(&lcd_vmalloc_mem_itree, mo);
	default:
		LCD_ERR("unexpected memory object type %d", 
			mo->sub_type);
	}
}

int __lcd_mem_itree_init(void)
{
	/*
	 * Initialize the two trees and locks
	 */
	lcd_physical_mem_itree.root = RB_ROOT;
	lcd_vmalloc_mem_itree.root = RB_ROOT;
	mutex_init(&lcd_physical_mem_itree.lock);
	mutex_init(&lcd_vmalloc_mem_itree.lock);

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
