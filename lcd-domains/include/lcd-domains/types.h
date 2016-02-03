/*
 * types.h
 *
 * Common types and macros used throughout the interface and microkernel.
 *
 * Copyright: University of Utah
 */
#ifndef LCD_DOMAINS_TYPES_H
#define LCD_DOMAINS_TYPES_H

/**
 * Return value to signal that a host/non-isolated resource (like a page)
 * is not part of the capability system (i.e., is not in any thread's
 * cspace).
 */
#define LCD_ERR_NOT_VOLUNTEERED (-512)

/*
 * Extra reserved cnodes:
 *
 * cptr = 1 is the LCD's call endpoint
 * cptr = 2 points to an endpoint if the lcd did a receive, and the sender did
 *          a call (so the lcd can do a reply)
 *
 * So, if lcd A does a call on endpoint #1 and lcd B does a receive on endpoint
 * #1, the endpoint at LCD_CPTR_CALL_ENDPOINT in A's cspace will be granted
 * to lcd B in B's cspace at cptr LCD_CPTR_REPLY_ENDPOINT. lcd B can do a reply
 * (one time and then it's revoked).
 */
#define LCD_CPTR_CALL_ENDPOINT __cptr(1)
#define LCD_CPTR_REPLY_ENDPOINT __cptr(2)

/* ADDRESS SPACE TYPES ---------------------------------------- */

/* XXX: Assumes host and guest run in 64-bit mode */

typedef struct { unsigned long gva; } gva_t;
typedef struct { unsigned long hva; } hva_t;
typedef struct { unsigned long gpa; } gpa_t;
typedef struct { unsigned long hpa; } hpa_t;

static inline gva_t __gva(unsigned long gva)
{
	return (gva_t){ gva };
}
static inline unsigned long gva_val(gva_t gva)
{
	return gva.gva;
}
static inline unsigned long * gva_ptr(gva_t * gva)
{
	return &(gva->gva);
}
static inline gva_t gva_add(gva_t gva, unsigned long off)
{
	return __gva(gva_val(gva) + off);
}
static inline hva_t __hva(unsigned long hva)
{
	return (hva_t){ hva };
}
static inline unsigned long hva_val(hva_t hva)
{
	return hva.hva;
}
static inline unsigned long * hva_ptr(hva_t * hva)
{
	return &(hva->hva);
}
static inline hva_t hva_add(hva_t hva, unsigned long off)
{
	return __hva(hva_val(hva) + off);
}
static inline gpa_t __gpa(unsigned long gpa)
{
	return (gpa_t){ gpa };
}
static inline unsigned long gpa_val(gpa_t gpa)
{
	return gpa.gpa;
}
static inline unsigned long * gpa_ptr(gpa_t * gpa)
{
	return &(gpa->gpa);
}
static inline gpa_t gpa_add(gpa_t gpa, unsigned long off)
{
	return __gpa(gpa_val(gpa) + off);
}
static inline hpa_t __hpa(unsigned long hpa)
{
	return (hpa_t){ hpa };
}
static inline unsigned long hpa_val(hpa_t hpa)
{
	return hpa.hpa;
}
static inline unsigned long * hpa_ptr(hpa_t * hpa)
{
	return &(hpa->hpa);
}
static inline hpa_t hpa_add(hpa_t hpa, unsigned long off)
{
	return __hpa(hpa_val(hpa) + off);
}
static inline hpa_t pa2hpa(unsigned long pa)
{
	return (hpa_t){ pa };
}
static inline hpa_t va2hpa(void *va)
{
	return (hpa_t){ __pa(va) };
}
static inline void * hpa2va(hpa_t hpa)
{
	return __va(hpa_val(hpa));
}
static inline hva_t hpa2hva(hpa_t hpa)
{
	return (hva_t){ (unsigned long)__va(hpa.hpa) };
}
static inline void * hva2va(hva_t hva)
{
	return (void *)hva_val(hva);
}
static inline hva_t va2hva(void *va)
{
	return __hva((unsigned long)va);
}
static inline hpa_t hva2hpa(hva_t hva)
{
	return (hpa_t){ (unsigned long)__pa(hva2va(hva)) };
}

/* BOOT ADDRESS SPACE & INFO ------------------------------------------- */

/*
 * Guest Physical Memory Layout
 * ============================
 *
 * No gdt/tss/idt for now (easier). See Documentation/lcd-domains/vmx.txt.
 *
 * From bottom to top,
 *
 *   -- The bottom 1 MB is unmapped / reserved in case the module is expecting 
 *      the standard physical memory layout of a PC. (Of course, it or its 
 *      creator would need to map something there to emulate that memory.) No
 *      memory mapped here for the gcc stack protector, so make sure you have
 *      that turned off when building the code for the lcd.
 *
 *   -- Guest virtual page tables come next, 4 MBs. This puts a (big) upper 
 *      limit on the size of the module that can be mapped. The page tables
 *      in the hierarchy are allocated on demand as the module is mapped.
 *
 *   -- The UTCB used by the initial thread when the lcd boots. (The
 *      microkernel manages this page.)
 *
 *   -- The stack
 *
 *   -- Heap. The module is put at the beginning of the heap.
 *
 *   -- A huge chunk of free/unmapped guest physical memory available to the
 *      module.
 *
 *   -- The upper part is unusable (see Intel SDM V3 28.2.2). The last
 *      usable byte is at 0x0000 FFFF FFFF FFFF.
 *
 *                   +---------------------------+ 0xFFFF FFFF FFFF FFFF
 *                   |         Unusable          |
 *                   +---------------------------+ 0x0000 FFFF FFFF FFFF
 *                   |	        Free		 |
 *                   | 				 |
 *                   +---------------------------+ 0x0000 E8FF FFFF FFFF
 *                   |				 |
 *		     |	     Ioremap space	 |
 *		     |		(32 TB)		 |
 *		     |				 |
 *		     +---------------------------+ 0x0000 C900 0000 0000	
 *		     |				 |
 *                   :           Free            :
 *                   |                           |
 *                   +---------------------------+ 0x0000 0000 0150 8000
 *                   |                           |
 *                   |           Heap            |
 *                   |      (module mapped       |
 *                   |       at the bottom)      |
 *                   :         (16 MBs)          :
 *                   |                           |
 *                   +---------------------------+ 0x0000 0000 0050 8000
 *                   |           Stack           |
 *                   |          (8 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 6000
 *                   |         Guard page        |
 *                   |        (not mapped)       |
 *                   |          (4 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 5000
 *                   |           UTCB            |
 *                   |          (4 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 4000
 *                   |        Boot info          |
 *                   |         (16 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 0000
 *                   | Guest Virtual Page Tables | 
 *                   |        (4 MBs max)        |
 *                   +---------------------------+ 0x0000 0000 0010 0000
 *                   |       Free / Unmapped     | 
 *                   |          (1 MB)           |
 *                   +---------------------------+ 0x0000 0000 0000 0000
 *
 * Guest Virtual Memory Layout
 * ===========================
 *
 * The lower part has the same layout as the guest physical.
 *
 * The module is mapped per the guest virtual addresses in the lcd_module_page
 * list returned from the module loader, so that relinking is unnecessary.
 * 
 *                   +---------------------------+ 0xFFFF FFFF FFFF FFFF
 *		     |	      	Unused	 	 |
 *		     |		(free)		 |
 *		     +---------------------------+ 0x0000 E8FF FFFF FFFF
 *		     |				 |
 *		     |	     Ioremap Space	 |
 *		     :	        (32 TB)		 :
 *		     |				 |
 *  The module       +---------------------------+ 0x0000 C900 0000 0000
 *  gets mapped      |                           |
 *  somewhere in     :         	Unused	 	 :
 *  here  -------->  |          (free)           |
 *                   |                           |
 *                   +---------------------------+ 0x0000 0000 0150 8000
 *                   |                           |
 *                   |           Heap            |
 *                   :         (16 MBs)          :
 *                   |                           |
 *                   +---------------------------+ 0x0000 0000 0050 8000
 *                   |           Stack           |
 *                   |          (8 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 6000
 *                   |         Guard page        |
 *                   |        (not mapped)       |
 *                   |          (4 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 5000
 *                   |           UTCB            |
 *                   |          (4 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 4000
 *                   |        Boot info          |
 *                   |         (16 KBs)          |
 *                   +---------------------------+ 0x0000 0000 0050 0000
 *                   | Guest Virtual Page Tables | 
 *                   |        (4 MBs max)        |
 *                   +---------------------------+ 0x0000 0000 0010 0000
 *                   |       Free / Unmapped     | 
 *                   |          (1 MB)           |
 *                   +---------------------------+ 0x0000 0000 0000 0000
 */

#define LCD_BOOT_PAGES_ORDER 2
#define LCD_STACK_PAGES_ORDER 1

/* guest physical addresses */
#define LCD_GV_PAGING_MEM_GPA __gpa(1 << 20) /* low 1 MB is empty */
#define LCD_GV_PAGING_MEM_SIZE (4 << 20)
#define LCD_BOOT_PAGES_GPA gpa_add(LCD_GV_PAGING_MEM_GPA, \
					LCD_GV_PAGING_MEM_SIZE)
#define LCD_BOOT_PAGES_SIZE ((1 << LCD_BOOT_PAGES_ORDER) * (4 << 10))
#define LCD_UTCB_GPA gpa_add(LCD_BOOT_PAGES_GPA, LCD_BOOT_PAGES_SIZE)
#define LCD_UTCB_SIZE (4 << 10)
#define LCD_UTCB_STACK_GAP (4 << 10) /* guard page, not mapped */
#define LCD_STACK_GPA gpa_add(LCD_UTCB_GPA, LCD_UTCB_SIZE + LCD_UTCB_STACK_GAP)
#define LCD_STACK_SIZE ((1 << LCD_STACK_PAGES_ORDER) * (4 << 10))
#define LCD_MODULE_GPA gpa_add(LCD_STACK_GPA, LCD_STACK_SIZE)
/* Unused area in between heap and ioremap space */
#define LCD_IOREMAP_GPA_BASE __gpa(0xc9 << 40)
#define LCD_IOREMAP_GPA_SIZE (1 << 45) /* 32 TB set aside for ioremap */

/* guest virtual addresses */
#define LCD_GV_PAGING_MEM_GVA __gva(gpa_val(LCD_GV_PAGING_MEM_GPA))
#define LCD_BOOT_PAGES_GVA __gva(gpa_val(LCD_BOOT_PAGES_GPA))
#define LCD_UTCB_GVA __gva(gpa_val(LCD_UTCB_GPA))
#define LCD_STACK_GVA __gva(gpa_val(LCD_STACK_GPA))
#define LCD_IOREMAP_GVA __gva(gpa_val(LCD_IOREMAP_GPA))

#define LCD_NUM_BOOT_CPTRS 8

struct lcd_boot_info_for_page {
	cptr_t my_cptr;
	gpa_t page_gpa;
};

/* 
 * Hack for now to make boot easier, used in liblcd/lcd/cap.c for cptr
 * cache.
 */

#define LCD_BMAP0_SIZE (1 << (LCD_CPTR_SLOT_BITS + 0 * LCD_CPTR_FANOUT_BITS))
#define LCD_BMAP1_SIZE (1 << (LCD_CPTR_SLOT_BITS + 1 * LCD_CPTR_FANOUT_BITS))
#define LCD_BMAP2_SIZE (1 << (LCD_CPTR_SLOT_BITS + 2 * LCD_CPTR_FANOUT_BITS))
#define LCD_BMAP3_SIZE (1 << (LCD_CPTR_SLOT_BITS + 3 * LCD_CPTR_FANOUT_BITS))

#define LCD_BMAP0_NUM_LONGS BITS_TO_LONGS(LCD_BMAP0_SIZE)
#define LCD_BMAP1_NUM_LONGS BITS_TO_LONGS(LCD_BMAP1_SIZE)
#define LCD_BMAP2_NUM_LONGS BITS_TO_LONGS(LCD_BMAP2_SIZE)
#define LCD_BMAP3_NUM_LONGS BITS_TO_LONGS(LCD_BMAP3_SIZE)

struct lcd_boot_info {
	/*
	 * Bootstrap cptr cache --------------------
	 *
	 * level 0
	 */
	unsigned long bmap0[LCD_BMAP0_NUM_LONGS];
	/* level 1 */
	unsigned long bmap1[LCD_BMAP1_NUM_LONGS];
	/* level 2 */
	unsigned long bmap2[LCD_BMAP2_NUM_LONGS];
	/* level 3 */
	unsigned long bmap3[LCD_BMAP3_NUM_LONGS];
	/*
	 * Bootstrap page info --------------------
	 */
	unsigned num_boot_mem_pi;
	unsigned num_stack_mem_pi;
	unsigned num_paging_mem_pi;
	unsigned num_free_mem_pi;
	struct lcd_boot_info_for_page *boot_mem_pi_start;
	struct lcd_boot_info_for_page *stack_mem_pi_start;
	struct lcd_boot_info_for_page *paging_mem_pi_start;
	struct lcd_boot_info_for_page *free_mem_pi_start;
	/*
	 * Other capabilities (e.g., endpoints)
	 */
	cptr_t cptrs[LCD_NUM_BOOT_CPTRS];
};

/* MACRO HELPERS -------------------------------------------------- */

/* This may not be the ideal place for this stuff. */

/* Motivated by original module name max */
#define LCD_MODULE_NAME_MAX (64 - sizeof(unsigned long))

/* 
 * Max absolute path size to lcd kernel module .ko, i.e., a full
 * path like:
 *
 *    /foo/bar/baz/the_module.ko
 *
 * Note that the name part (the_module) must be less or equal to
 * LCD_MODULE_NAME_MAX.
 */
#define LCD_MPATH_SIZE 256

#define LCD_TEST_MODS_PATH \
	"/local/disk2/xcap-git/lcd-domains/test-mods"

#define LCD_DIR(subpath) \
	LCD_TEST_MODS_PATH "/" subpath

#define LCD_PRINTK_BUFF_SIZE 512

#endif /* LCD_DOMAINS_TYPES_H */
