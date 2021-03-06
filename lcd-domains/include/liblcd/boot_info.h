/*
 * bootinfo.h
 *
 * This is the data that is stored in the LCD's bootstrap
 * pages.
 *
 * Copyright: University of Utah
 */
#ifndef LIBLCD_BOOTINFO_H
#define LIBLCD_BOOTINFO_H

#include <libcap.h>
#include <lcd_domains/types.h>
#include <liblcd/console.h>

#define LCD_NUM_BOOT_CPTRS 8

struct lcd_boot_cptrs {
	cptr_t module_init;
	cptr_t module_core;
	cptr_t boot_pages;
	cptr_t stack;
	cptr_t pcpu_stacks[NR_CPUS];
	cptr_t vmfunc_tr_page;
	cptr_t vmfunc_sb_page;
	cptr_t vmfunc_state_page;
	cptr_t gv;
	cptr_t entry_text_pages;
	cptr_t idt_page;
	cptr_t idtr_base;
	cptr_t percpu_pages;
};

struct lcd_boot_info {
	/*
	 * Bootstrap cptr cache
	 */
	struct cptr_cache cptr_cache;
	/*
	 * Module init and core addresses and sizes
	 */
	gva_t module_init_base;
	gva_t module_core_base;
	gva_t module_vmfunc_tr_base;
	gva_t module_vmfunc_sb_base;
	gva_t entry_text_base;

	unsigned long module_init_size;
	unsigned long module_core_size;
	unsigned long module_vmfunc_tr_size;
	unsigned long module_vmfunc_sb_size;
	/*
	 * Capabilities to memory objects
	 */
	struct lcd_boot_cptrs lcd_boot_cptrs;
	/*
	 * Other capabilities (e.g., endpoints)
	 */
	cptr_t cptrs[LCD_NUM_BOOT_CPTRS];
};

/**
 * lcd_get_boot_info -- Return the bootstrap info
 *
 * When executing inside an LCD, invoke this function, and it will
 * return a pointer to the LCD's bootstrap data.
 *
 * For non-isolated code, this function returns NULL.
 */
struct lcd_boot_info * lcd_get_boot_info(void);
/**
 * lcd_dump_boot_info -- Print the bits in the lcd_boot_info
 *
 * This is for debugging.
 */
static inline void lcd_dump_boot_info(struct lcd_boot_info *b)
{
	unsigned long idx;
	unsigned char *bits = (unsigned char *)b;
	/*
	 * This isn't ideal for printing, but it needs to be useable
	 * before kmalloc etc. is available. (Can't alloc a string
	 * buffer and sprintf into it.)
	 */
	LIBLCD_MSG("  DUMPING lcd_boot_info bits:");
	for (idx = 0; idx < sizeof(struct lcd_boot_info); idx++) {
		if (idx % 10 == 0)
			printk(KERN_ERR "\n    ");
		/* We do the hex manually because vsnprintf if used inside
		 * the isolated liblcd, and it outputs the data in a weird
		 * way. */
		printk(KERN_ERR "%02x ", bits[idx]);
	}
	printk(KERN_ERR "\n");  
}

struct ext_registers {
	uint64_t regs[PAGE_SIZE/8];
};

struct ext_registers *get_register_page(int cpu);

#endif /* LIBLCD_BOOTINFO_H */
