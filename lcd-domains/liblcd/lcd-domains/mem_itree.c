/*
 * mem_itree.c
 *
 * Resource trees used for address -> cptr translation.
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <linux/slab.h>
#include <linux/module.h>
#include <liblcd/resource_tree.h>
#include <liblcd/boot_info.h>
#include <lcd_domains/liblcd.h>

#include <lcd_config/post_hook.h>

static struct lcd_resource_tree itree;
static struct lcd_resource_node boot_nodes[LCD_NR_BOOT_RESOURCE_NODES];
static unsigned int boot_nodes_brk;
static int mem_itree_booted;

static void free_itree_node(struct lcd_resource_node *n)
{
	if (n->flags & LCD_RESOURCE_NODE_STATIC)
		return; /* node came from boot node array */
	else
		kfree(n);
}

static struct lcd_resource_node *early_alloc_itree_node(void)
{
	if (boot_nodes_brk >= LCD_NR_BOOT_RESOURCE_NODES) {
		LIBLCD_ERR("exhausted boot resource nodes");
		return NULL;
	} else {
		return &boot_nodes[++boot_nodes_brk];
	}
}

static struct lcd_resource_node *alloc_itree_node(void)
{
	struct lcd_resource_node *n;

	if (likely(mem_itree_booted)) {
		/*
		 * XXX: Be careful here. The heap may be calling into
		 * here when it is growing itself (alloc'ing fresh
		 * pages, and inserting the cptr into the resource tree).
		 * This call to kzalloc may recursively lead *back into*
		 * the heap to grow a slab cache for kmalloc. This
		 * recursion is OK because of how the heap maps the
		 * new pages (it maps them first *before* inserting them
		 * into the mem itree). See ram_map.c:_lcd_mmap.
		 */
		n = kzalloc(sizeof(struct lcd_resource_node), GFP_KERNEL);
		if (!n) {
			LIBLCD_ERR("kmalloc failed");
			return NULL;
		}
		n->flags = LCD_RESOURCE_NODE_KMALLOC;
		return n;
	} else {
		n = early_alloc_itree_node();
		if (!n)
			return NULL;
		n->flags = LCD_RESOURCE_NODE_STATIC;
		return n;
	}
}

/* INTERFACE -------------------------------------------------- */

int __liblcd_mem_itree_insert(gpa_t start, unsigned long size,
			cptr_t mem_cptr)
{
	struct lcd_resource_node *n;

	n = alloc_itree_node();
	if (!n) {
		LIBLCD_ERR("failed to get a node");
		goto fail1;
	}
	n->it_node.start = gpa_val(start);
	n->it_node.last = gpa_val(start) + size - 1;
	n->cptr = mem_cptr;
	lcd_resource_tree_insert(&itree, n);

	return 0;

fail1:
	return -ENOMEM;
}

void __liblcd_mem_itree_delete(struct lcd_resource_node *n)
{
	lcd_resource_tree_remove(&itree, n);
	free_itree_node(n);
}

void __liblcd_mem_itree_dump(void)
{
	lcd_resource_tree_dump(&itree);
}

int lcd_phys_to_resource_node(gpa_t paddr, struct lcd_resource_node **n_out)
{
	struct lcd_resource_node *n;
	int ret;
	/*
	 * Lookup
	 */
	ret = lcd_resource_tree_search(&itree, gpa_val(paddr), &n);
	if (ret) {
		LIBLCD_ERR("lookup failed");
		goto fail1;
	}
	*n_out = n;

	return 0;

fail1:
	return ret;
}

int lcd_phys_to_cptr(gpa_t paddr, cptr_t *c_out, unsigned long *size_out)
{
	struct lcd_resource_node *n;
	int ret;

	ret = lcd_phys_to_resource_node(paddr, &n);
	if (ret)
		return ret;
	*c_out = n->cptr;
	*size_out = lcd_resource_node_size(n);

	return 0;
}

int lcd_virt_to_cptr(gva_t vaddr, cptr_t *c_out, unsigned long *size_out)
{
	return lcd_phys_to_cptr(lcd_gva2gpa(vaddr), c_out, size_out);
}

/* INIT -------------------------------------------------- */

void __liblcd_mem_itree_booted(void)
{
	mem_itree_booted = 1;
}

static int add_boot_memory(void)
{
	struct lcd_resource_node *n;
	/*
	 * Add module init
	 */
	n = alloc_itree_node();
	if (!n)
		goto fail1;
	n->it_node.start = __liblcd_pa(THIS_MODULE->module_init);
	n->it_node.last = n->it_node.start + THIS_MODULE->init_size - 1;
	n->cptr = lcd_get_boot_info()->lcd_boot_cptrs.module_init;
	lcd_resource_tree_insert(&itree, n);
	/*
	 * Add module core
	 */
	n = alloc_itree_node();
	if (!n)
		goto fail2;
	n->it_node.start = __liblcd_pa(THIS_MODULE->module_core);
	n->it_node.last = n->it_node.start + THIS_MODULE->core_size - 1;
	n->cptr = lcd_get_boot_info()->lcd_boot_cptrs.module_core;
	lcd_resource_tree_insert(&itree, n);
	/*
	 * Add bootstrap pages
	 */
	n = alloc_itree_node();
	if (!n)
		goto fail3;
	n->it_node.start = gpa_val(LCD_BOOTSTRAP_PAGES_GP_ADDR);
	n->it_node.last = gpa_val(LCD_BOOTSTRAP_PAGES_GP_ADDR) + 
		LCD_BOOTSTRAP_PAGES_SIZE - 1;
	n->cptr = lcd_get_boot_info()->lcd_boot_cptrs.boot_pages;
	lcd_resource_tree_insert(&itree, n);
	/*
	 * Add stack pages
	 */
	n = alloc_itree_node();
	if (!n)
		goto fail4;
	n->it_node.start = gpa_val(LCD_STACK_GP_ADDR);
	n->it_node.last = gpa_val(LCD_STACK_GP_ADDR) + 
		LCD_STACK_SIZE - 1;
	n->cptr = lcd_get_boot_info()->lcd_boot_cptrs.stack;
	lcd_resource_tree_insert(&itree, n);
	/*
	 * Add boot guest virtual page tables
	 */
	n = alloc_itree_node();
	if (!n)
		goto fail5;
	n->it_node.start = gpa_val(LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR);
	n->it_node.last = gpa_val(LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR) + 
		LCD_BOOTSTRAP_PAGE_TABLES_SIZE - 1;
	n->cptr = lcd_get_boot_info()->lcd_boot_cptrs.gv;
	lcd_resource_tree_insert(&itree, n);

	return 0;

fail5:
fail4:
fail3:
fail2:
fail1:
	return -ENOMEM; /* we don't bother "freeing" nodes */
}

int __liblcd_mem_itree_init(void)
{
	int ret;
	/*
	 * Init resource tree root
	 */
	ret = lcd_resource_tree_init(&itree);
	if (ret) {
		LIBLCD_ERR("resource tree init failed");
		goto fail1;
	}
	/*
	 * Add existing memory (at boot) to resource tree
	 */
	ret = add_boot_memory();
	if (ret) {
		LIBLCD_ERR("failed to add resource nodes for boot mem");
		goto fail2;
	}

	return 0;

fail2:
fail1:
	return ret;
}
