/*
 * liblcd.h
 *
 * Some liblcd-internal functions and configuration.
 *
 * Copyright: University of Utah
 */
#ifndef LCD_DOMAINS_LIBLCD_H
#define LCD_DOMAINS_LIBLCD_H

/* GLOBAL MEMORY TREE -------------------------------------------------- */

#define LCD_NR_BOOT_RESOURCE_NODES 16

/**
 * __liblcd_mem_itree_init -- Early boot initialization
 *
 * This should be called before the page allocator inits.
 */
int __liblcd_mem_itree_init(void);
/**
 * __liblcd_mem_itree_booted -- Call this when kmalloc is initialized
 *
 * This signals to mem itree code that it can safely start using kmalloc
 * to allocate tree nodes.
 */
void __liblcd_mem_itree_booted(void);
/**
 * __liblcd_mem_itree_insert -- Insert a new node into the memory tree
 * @start: the starting guest phys address of the memory object
 * @size: the size in bytes of the object
 * @mem_cptr: the cptr to the memory object capability in LCD's cspace
 */
int __liblcd_mem_itree_insert(gpa_t start, unsigned long size,
			cptr_t mem_cptr);
/**
 * __liblcd_mem_itree_delete -- Remove node from tree and free it
 * @n: the node to delete
 *
 * @n is no longer a valid pointer after this call.
 */
void __liblcd_mem_itree_delete(struct lcd_resource_node *n);

#endif /* LCD_DOMAINS_LIBLCD_H */
