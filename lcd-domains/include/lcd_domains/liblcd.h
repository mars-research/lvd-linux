/*
 * liblcd.h
 *
 * Some liblcd-internal functions and configuration.
 *
 * Copyright: University of Utah
 */
#ifndef LCD_DOMAINS_LIBLCD_H
#define LCD_DOMAINS_LIBLCD_H

#include <linux/mmzone.h>
#include <libcap.h>
#include <lcd_domains/types.h>
#include <liblcd/resource_tree.h>
#include <liblcd/address_spaces.h>

/* LIBCAP DEBUG -------------------------------------------------- */

/* Controls debug msg output for liblcd's libcap */
#define LIBLCD_LIBCAP_DEBUG_LVL 0

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
/**
 * __liblcd_mem_itree_dump -- Print memory itree to console, for debugging
 */
void __liblcd_mem_itree_dump(void);

/* 
 * HEAP --------------------------------------------------
 *
 * Heap is 2^12 pages, or 16 MBs.
 *
 * The minimum you can allocate from the heap is 4 KBs (2^0 = 1 page).
 *
 * The maximum you can allocate from the heap is determined by the
 * host's configuration (MAX_ORDER - 1; MAX_ORDER is defined in 
 * linux/mmzone.h at the top). For x86_64, this is 4 MBs (2^10 = 1024 pages).
 */
#define LCD_HEAP_NR_PAGES_ORDER 12
#define LCD_HEAP_SIZE (1UL << (LCD_HEAP_NR_PAGES_ORDER + PAGE_SHIFT))
#define LCD_HEAP_MIN_ORDER 0
#define LCD_HEAP_MAX_ORDER (MAX_ORDER - 1)

/**
 * __liblcd_heap_init -- Call during boot after mem itree initialized
 *
 * This initializes the heap allocator.
 */
int __liblcd_heap_init(void);

/* 
 * RAM MAPPING --------------------------------------------------
 *
 * RAM mapping area is 1 GB (2^18 pages).
 *
 * The minimum address space block you can allocate from the RAM map
 * area is 2 MBs (2^9 = 512 pages). This leads to a lot of internal
 * fragmentation, but this is tolerable, so long as we don't need to
 * map RAM more than 200 or so times. (The actual memory object will
 * likely be considerably smaller than 2 MBs, so only the first
 * few KBs may actually be backed/mapped in guest physical. But the
 * entire 2 MB region will be considered occupied by the internal
 * RAM map allocator.)
 *
 * The maximum address space block you can allocate from the RAM map
 * area is 256 MB (2^16 = 65,536 pages).
 *
 * In hindsight, maybe a simple bitmap would have been just as good
 * here since there are only 1GB/64MB = 512 allocation blocks. Oh well. (These
 * mapping regions were originally gonna be a lot bigger, but I had
 * to switch over to 2MB guest virtual pages instead of 1GB pages. Etc.
 * etc.)
 */
#define LCD_RAM_MAP_NR_PAGES_ORDER \
	(ilog2(LCD_RAM_MAP_REGION_SIZE >> PAGE_SHIFT))
#define LCD_RAM_MAP_SIZE LCD_RAM_MAP_REGION_SIZE
#define LCD_RAM_MAP_MIN_ORDER 9
#define LCD_RAM_MAP_MAX_ORDER 16

/**
 * __liblcd_ram_map_init -- Call during boot after heap initialized
 *
 * This initializes the RAM map region allocator.
 */
int __liblcd_ram_map_init(void);

/* MISC -------------------------------------------------- */

/**
 * __liblcd_pa -- Convert void* guest virtual to an unsigned long physical addr
 * @addr: address to convert
 */
static inline unsigned long __liblcd_pa(void *addr)
{
	return (unsigned long)(addr) - LCD_VIRT_BASE;
}

#endif /* LCD_DOMAINS_LIBLCD_H */
