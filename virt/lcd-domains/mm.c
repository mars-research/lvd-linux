/*
 * Memory management for LCDs
 */

#include <lcd-domains/lcd-domains.h>
#include <asm/lcd-domains-arch.h>
#include "mm.h"


int lcd_mm_gpa_map_range(struct lcd *lcd, u64 gpa_start, u64 hpa_start, 
			u64 npages)
{
	u64 off;
	u64 len;

	len = npages * PAGE_SIZE;
	for (off = 0; off < len; off += PAGE_SIZE) {
		if (lcd_arch_ept_map_gpa_to_hpa(lcd->lcd_arch,
							/* gpa */
							gpa_start + off,
							/* hpa */
							hpa_start + off,
							/* create */
							1,
							/* no overwrite */
							0)) {
			printk(KERN_ERR "lcd_mm_gpa_map_range: error mapping gpa %lx to hpa %lx\n",
				(unsigned long)(gpa_start + off),
				(unsigned long)(hpa_start + off));
			return -EIO;
		}
	}

	return 0;
}
