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

/* 
 * HEAP CONFIGURATION --------------------------------------------------
 *
 * Heap is 2^12 pages, or 16 MBs.
 *
 * The minimum you can allocate from the heap is 4 KBs (2^0 = 1 page).
 *
 * The maximum you can allocate from the heap is 4 MBs (2^10 = 1024 pages).
 */
#define LCD_HEAP_NR_PAGES_ORDER 12
#define LCD_HEAP_SIZE (1UL << (LCD_HEAP_NR_PAGES_ORDER + PAGE_SHIFT))
#define LCD_HEAP_MIN_ORDER 0
#define LCD_HEAP_MAX_ORDER 10

/* MEMORY SUBSYSTEM INTERNALS ------------------------------ */

/**
 * __liblcd_mem_init -- Call during boot after mem itree initialized
 *
 * This initializes the heap allocator.
 */
int __liblcd_mem_init(void);

#endif /* LCD_DOMAINS_LIBLCD_H */
