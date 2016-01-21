/*
 * mem.h
 *
 * Memory allocation, memory "volunteering", mapping,
 * and so on. This contains e.g.:
 *
 *    - Low-level functions for calling into the microkernel
 *      to allocate/free pages and map/unmap pages.
 *    - Higher-level functions that allocate pages and find free 
 *      places in the caller's address spaces to map them.
 *
 * Copyright: University of Utah
 */
#ifndef LCD_DOMAINS_MEM_H
#define LCD_DOMAINS_MEM_H

#include <linux/mm_types.h>
#include <libcap.h>
#include <lcd-domains/types.h>

/* LOW-LEVEL PAGE ALLOC -------------------------------------------------- */

/**
 * _lcd_pages_alloc_exact_node -- Low-level page allocation on numa node,
 *                               calls into microkernel
 * @nid: NUMA node ID on which to allocate pages
 * @flags: GFP flags to control allocation
 * @order: allocate 2^order pages
 * @slot_out: cptr to capability to allocated pages
 *
 * This function first allocates a slot in the caller's cspace. It then
 * calls into the microkernel to allocate 2^order pages. The microkernel
 * will store the capability to the pages in the allocated slot. The
 * slot cptr is returned in slot_out.
 *
 * XXX: This is translated into a call to alloc_pages_node with arguments
 * @nid, @flags, and @order in the microkernel. There are probably security
 * issues with that.
 *
 */ 
int _lcd_alloc_pages_exact_node(int nid, unsigned int flags, unsigned int order,
				cptr_t *slot_out);

/**
 * _lcd_alloc_pages -- Allocate pages, low-level call into microkernel
 * @flags: GFP flags to control allocation
 * @order: allocate 2^order pages
 * @slot_out: cptr to capability to allocated pages
 *
 * Similar to lcd_alloc_pages_exact_node, but uses the numa node the
 * LCD is currently running on / associated with.
 *
 * Note: You should use this lower-level function if you are setting up a
 * memory region for sharing with another LCD. Why? Because you will need
 * to grant the other LCD access to the memory, and the capability you
 * get when you alloc is for the whole chunk of pages (you can't grant
 * individual pages within the chunk, that is not supported right now).
 */
int _lcd_alloc_pages(unsigned int flags, unsigned int order,
		cptr_t *slot_out);

/* 
 * What about free pages? Use lcd_cap_revoke and/or lcd_cap_delete with 
 * the pages cptr. All capabilities to the pages must go away for the
 * pages to be returned to the host page allocator.
 */

/* LOW-LEVEL MAP -------------------------------------------------- */

/**
 * _lcd_mmap -- Low-level memory mapping, calls into microkernel
 * @mo: cptr to memory object capability (e.g., pages)
 * @base: starting guest physical address where memory should be mapped
 *
 * Maps the memory referred to by the @mo cptr_t in the caller's
 * guest physical address space, starting at @base. If @mo is
 * already mapped, @base has an existing mapping, or the @mo won't
 * fit, this call fails.
 *
 * If @mo or @gpa is invalid, returns non-zero (e.g., @mo doesn't
 * refer to a memory object capability).
 *
 * Note: If the caller is running in the non-isolated host (not inside
 * an LCD), this function is a no-op. (All host addresses are already
 * accessible to kLCDs. You can use lcd_map_phys to get the host physical
 * address where the memory is.)
 *
 * Note: If @mo is already mapped, the call will fail (multiple mappings
 * is currently not supported).
 */
int _lcd_mmap(cptr_t mo, gpa_t base);

/**
 * _lcd_munmap -- Low-level unmapping, calls into microkernel
 * @mo: cptr to memory object capability
 *
 * Unmaps the memory referred to by the @mo cptr_t in the caller's
 * guest physical address space. If the pages aren't mapped, silently
 * fails / does nothing.
 *
 * Note: If the caller is a non-isolated thread, this function is a no-op.
 */
void _lcd_munmap(cptr_t mo);

/* HIGH-LEVEL ALLOC/FREE ---------------------------------------- */

/**
 * lcd_alloc_pages_exact_node -- Get free pages from the heap
 * @nid: NUMA node id from which to allocate from
 * @flags: GFP flags to control allocation
 * @order: allocate 2^order pages
 *
 * Allocate 2^order pages from the heap. Returns pointer to the struct
 * page for the first page in the chunk.
 *
 * Note: If you want to share pages with another LCD, it's recommended
 * that you use the lower level _lcd_alloc_pages. See its documentation.
 *
 * TODO: If/when LCDs become multicore, it would be cool to have separate
 * pools of memory for different NUMA nodes.
 */
struct page *lcd_alloc_pages_exact_node(int nid, unsigned int flags, 
					unsigned int order);
/**
 * lcd_alloc_pages -- Get free pages from the heap
 * @flags: GFP flags to control allocation
 * @order: allocate 2^order pages
 *
 * Similar to lcd_alloc_pages_exact_node.
 */
struct page *lcd_alloc_pages(unsigned int flags, unsigned int order);
/**
 * lcd_free_pages -- Free pages allocated via lcd_alloc_pages
 * @base: pointer to first struct page in the chunk
 * @order: chunk is of size 2^order pages
 */
void lcd_free_pages(struct page *base);

/* HIGH-LEVEL MAP/UNMAP ---------------------------------------- */

/**
 * lcd_map_phys -- Map pages in a free part of the physical address space
 * @pages: cptr to pages capability
 * @order: there are 2^order pages to map
 * @base_out: out param, guest physical address where pages were mapped
 *
 * IMPORTANT: This should not be used for ioremap-like functionality. This
 * function will not set up the memory types / cache behavior properly for 
 * device memory.
 *
 * Map pages in a free part of the physical address space. Returns
 * the (guest) physical address where the pages were mapped. (This is
 * kind of like kmap, but for the guest physical address space.)
 *
 * If @order is not the true order of @pages, you may get odd behavior.
 * (Internally, @order is going to be used to find a free spot in the
 * address space.)
 *
 * Note: For non-isolated code, guest physical == host physical (by convention),
 * and the pages are already technically accessible to the caller (but the
 * caller probably won't know the physical address of the pages - they just
 * have the opaque cptr_t for the pages). So, for non-isolated code, this
 * function doesn't really do much. The guest physical address returned
 * is actually the host physical address of the pages.
 *
 * Note: If pages is already mapped, this call will fail (the microkernel
 * does not support multiple mappings).
 */
int lcd_map_phys(cptr_t pages, unsigned int order, gpa_t *base_out);
/**
 * lcd_map_virt -- Map a physical address range in caller's virtual address
 *                 space
 * @base: base guest physical address of memory to map
 * @order: physical address range is 2^order pages
 * @gva_out: out param, where the memory was mapped in the virtual address 
 *           space
 *
 * IMPORTANT: This function should not be used for ioremap-like functionality.
 * It will not set up memory types for io mem (e.g., marking memory as
 * uncacheable).
 *
 * This is like kmap. You already have guest physical memory, and you just
 * want to map it in a free spot in the caller's virtual address space.
 *
 * Note: For non-isolated code, at least on x86_64, all physical memory
 * is already mapped, so this function won't really do much. Further
 * note that guest virtual == host virtual, by convention - so the guest
 * virtual address returned is actually a host virtual address for
 * non-isolated code.
 *
 * Note: Mapping the same chunk of memory more than once can lead to
 * undefined behavior inside the LCD. (The LCD data structures do not
 * support more than one mapping, and we don't try to check if there
 * already is a mapping. Linux doesn't support multiple mappings of RAM
 * either, fyi. Doing so would break page_address, for example, as
 * it would for us.)
 */
int lcd_map_virt(gpa_t base, unsigned int order, gva_t *gva_out);
/**
 * lcd_map_both -- Map pages in a free place in caller's physical and 
 *                 virtual address spaces.
 * @pages: cptr to pages capability
 * @order: 2^order pages
 * @gva_out: out param, (guest) virtual address where pages were mapped
 *
 * Map @pages in both address spaces so you can start using the memory.
 *
 * This is lcd_map_phys and lcd_map_virt put together, with the same
 * caveats.
 */
int lcd_map_both(cptr_t pages, unsigned int order, gva_t *gva_out);
/**
 * lcd_unmap_phys -- Unmap pages from physical address space
 * @base: guest physical address where pages are mapped
 * @order: 2^order pages are mapped there
 *
 * Unmap pages from caller's physical address space. (This is like kunmap,
 * but for physical memory.)
 *
 * Note: For non-isolated code, this is a no-op.
 */
void lcd_unmap_phys(gpa_t base, unsigned int order);
/**
 * lcd_unmap_virt -- Unmap physical memory from virtual address space
 * @base: guest virtual address where pages are mapped
 * @order: 2^order pages are mapped there
 *
 * Unmap memory from caller's virtual address space. (This is like kunmap.)
 *
 * Note: For non-isolated code, this is a no-op.
 */
void lcd_unmap_virt(gva_t base, unsigned int order);
/**
 * lcd_unmap_both -- Kill the guest virtual and physical mappings
 * @base: guest virtual address where pages are mapped
 * @order: 2^order pages are mapped there
 *
 * This is lcd_unmap_phys and lcd_unmap_virt put together, and will
 * kill the mappings set up by lcd_map_both.
 *
 * Note: For non-isolated code, this is a no-op.
 */
void lcd_unmap_both(gva_t base, unsigned int order);

/* "VOLUNTEERING" PAGES ---------------------------------------- */

/**
 * lcd_volunteer_pages -- Bring pages into the cspace access control system
 * @base: the first struct page of the chunk of physically contiguous pages
 * @order: there are 2^order pages
 * @slot_out: out param, where the page was stored in the caller's cspace
 *
 * Note: This only makes sense for non-isolated code. For isolated code,
 * this function is equivalent to lcd_phys_to_cptr (see its documentation).
 *
 * Inserts pages into the caller's cspace. Motivation: You allocated a page
 * by some other means, external to the microkernel (e.g., via
 * the host's alloc_pages), and you want to share the page with an LCD.
 * To share the page with an LCD, it needs to be part of the microkernel's
 * access control system. Hence, you need to insert this resource into
 * your cspace so you can then grant access to it to the LCD. This is what
 * this function does.
 *
 * To remove the pages from the capability system, you should revoke access to 
 * them and then call lcd_unvolunteer_pages (this is, for now, just
 * a wrapper around lcd_cap_delete).
 *
 * The microkernel *will not* free such pages when the last capability
 * to them goes away - the caller is responsible for freeing them. (This is
 * because the pages could have come from a non-isolated subsystem, like the
 * host module loader. If the microkernel always freed such pages, we would
 * have a double-free, since the module loader always tries to free the
 * pages as well.)
 *
 * IMPORTANT: No checks are in place to ensure you don't insert the same
 * pages multiple times. kLCDs are trusted to do the right thing.
 */
int lcd_volunteer_pages(struct page *base, unsigned int order,
			cptr_t *slot_out);
/**
 * lcd_unvolunteer_pages -- Remove pages from the capability system
 * @pages: cptr to pages capability to be removed
 *
 * Deletes pages from caller's cspace. (This is just a wrapper around
 * lcd_cap_delete.)
 *
 * IMPORTANT: This function does not attempt to remove the pages from
 * any other cspaces. It is the caller's responsibility to track and
 * revoke access. Also, this will not cause the pages to be freed. The
 * caller is responsible for doing that after they have removed the
 * pages from the capability system (i.e., revoke access, then call this
 * function, and then free the pages).
 *
 * NOTE: This doesn't do any type checking. You could potentially pass in
 * a cptr to a synchronous endpoint capability, for example, and it would
 * get deleted from the caller's cspace.
 */
int lcd_unvolunteer_pages(cptr_t pages);

/* "VOLUNTEERING" DEVICE MEMORY ---------------------------------------- */

/**
 * lcd_volunteer_dev_mem -- Bring device memory into the cspace access control 
 *                          system
 * @base: the physical adress at the start of the chunk of physically 
 *        contiguous device memory
 * @order: there are 2^order pages of device memory being volunteered
 * @slot_out: out param, where the page was stored in the caller's cspace
 *
 * Note: This only makes sense for non-isolated code. For isolated code,
 * all memory objects it has access to are in its cspace, and so
 * this function is equivalent to lcd_phys_to_cptr (see its documentation).
 *
 * Inserts device memory into the caller's cspace. Motivation: You want to 
 * grant an LCD access to a device's BAR regions, so that it can map those 
 * in its address space and interact with the device.
 *
 * The rest of the notes are equivalent to lcd_volunteer_pages; same ideas.
 *
 * Note: We use a gpa_t here so that isolated code can call this as well
 * without error. Recall that for non-isolated code, 
 * guest physical == host physical by convention (so a non-isolated thread
 * can cast an hpa to a gpa and it will do the right thing).
 */
int lcd_volunteer_dev_mem(gpa_t base, unsigned int order,
			cptr_t *slot_out);
/**
 * lcd_unvolunteer_dev_mem -- Remove device memory from the capability system
 * @devmem: cptr to device memory capability to be removed
 *
 * Deletes device mem from caller's cspace. (This is just a wrapper around
 * lcd_cap_delete.)
 *
 * IMPORTANT: This function does not attempt to remove the device memory from
 * any other cspaces. It is the caller's responsibility to track and
 * revoke access.
 *
 * NOTE: This doesn't do any type checking. You could potentially pass in
 * a cptr to a synchronous endpoint capability, for example, and it would
 * get deleted from the caller's cspace.
 */
int lcd_unvolunteer_dev_mem(cptr_t devmem);

/* ADDRESS -> CPTR TRANSLATION ---------------------------------------- */

/**
 * lcd_phys_to_cptr -- Look up the cptr for memory object that contains
 *                     guest physical address @paddr
 * @paddr: the physical address to search for
 * @c_out: out param containing cptr to memory capability that contains @paddr
 * @order_out: the memory object has size 2^order pages
 *
 * If @paddr meets these conditions:
 *
 *    1 - it is contained in a chunk of physical memory that has been
 *        mapped to real host memory (for non-isolated code, this will 
 *        always be true; but for isolated code, it may not have mapped 
 *        the host memory yet)
 *    2 - this host memory is under the capability system (for isolated
 *        code, this will always be true; but for non-isolated code,
 *        it need not be true - if the memory has not been volunteered
 *        yet)
 *
 * then this function will return the cptr to the caller's memory object
 * capability that corresponds to this host memory, along with the
 * order (size) of the memory.
 *
 * Failure conditions:
 *
 *    1 fails:  This can happen if an isolated thread invokes this
 *              function for a guest physical address that is not mapped
 *              to any host physical address
 *
 *    2 fails:  This can happen if a non-isolated thread does a query
 *              for a physical address that hasn't been introduced/volunteered
 *              yet into the capability system (for a page of host memory,
 *              for example). In this case, LCD_ERR_NOT_VOLUNTEERED is
 *              returned. Note that this failure case cannot happen for
 *              isolated threads (all of their memory objects are tracked
 *              through capabilities).
 *
 * (There is no way to get a cptr to a capability to individual pages
 * in a chunk, since the microkernel doesn't currently support this.)
 */
int lcd_phys_to_cptr(gpa_t paddr, cptr_t *c_out, unsigned int *order_out);
/**
 * lcd_virt_to_cptr -- Look up the cptr for memory object that contains
 *                     guest virtual address @vaddr
 * @vaddr: the virtual address to search for
 * @c_out: out param containing cptr to memory capability that contains @paddr
 * @order_out: the memory object has size 2^order pages
 *
 * Similar to lcd_phys_to_cptr. Similar error conditions.
 */
int lcd_virt_to_cptr(gva_t vaddr, cptr_t *c_out, unsigned int *order_out);

/* MISCELLANEOUS -------------------------------------------------- */

/**
 * lcd_virt_to_head_page -- Return struct page associated with addr
 * @addr: guest virtual address, as a void *
 *
 * Similar to Linux's virt_to_head_page (but we don't ever use 
 * compound pages).
 *
 * Necessary to satisfy slab dependency.
 *
 * NOTE: For isolated code, this function fails (and returns NULL) for 
 * addresses outside the heap. For non-isolated code, this function
 * is equivalent to virt_to_head_page.
 */
struct page *lcd_virt_to_head_page(const void *addr);
/**
 * lcd_page_address -- Return guest virtual address (as void *) associated 
 *                     with struct page
 * @page: the struct page pointer
 *
 * Recall that we only allow one virtual -> physical mapping (so that
 * we don't have to do reverse map lookups as Linux does). So for each 
 * struct page, there is at most one mapping.
 *
 * Similar to Linux's page_address.
 *
 * If there is no mapping associated with the struct page, returns NULL.
 *
 * Necessary to satisfy slab dependency.
 */
void *lcd_page_address(const struct page *page);
/**
 * lcd_free_memcg_kmem_pages -- Free 2^order pages at addr
 * @addr: starting address of pages
 * @order: 2^order pages
 *
 * We don't use cgroups inside LCDs, so this is equivalent to:
 *
 *   1 - lcd_virt_to_head_page to translate addr into a page
 *   2 - lcd_free_pages to free the pages
 *
 * Necessary to satisfy slab dependency.
 */
void lcd_free_memcg_kmem_pages(unsigned long addr, unsigned int order);

#endif /* LCD_DOMAINS_MEM_H */
