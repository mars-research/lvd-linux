/*
 * trampoline_link.h
 *
 * These macros are used by linker scripts (.lds.S files)
 * for calculating trampoline sizes and so on.
 */
#ifndef LCD_DOMAINS_TRAMPOLINE_LINK_H
#define LCD_DOMAINS_TRAMPOLINE_LINK_H

#define LCD_TRAMPOLINE_LINKER_SECTION(name) \
	.##name##.text : {		  \
	   *(.##name##.text)		  \
        }	                          \
        name##_size = SIZEOF(.##name##.text); \
	name##_offset = name##_addr - name;



#endif /* LCD_DOMAINS_TRAMPOLINE_LINK_H */



