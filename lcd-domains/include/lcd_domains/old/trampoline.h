/*
 * trampoline.h
 *
 * Helpers for dealing with trampolines.
 */
#ifndef LCD_DOMAINS_TRAMPOLINE_H
#define LCD_DOMAINS_TRAMPOLINE_H

#include <lcd-domains/liblcd-config.h>

#include <linux/slab.h>
#include <linux/kernel.h>

#include <lcd-domains/liblcd-hacks.h>

/**
 * This struct is literally the trampoline. In memory,
 * hidden_args comes first, followed by the code bits
 * for the trampoline.
 */
struct lcd_trampoline_handle {
	void *hidden_args;
	char trampoline[0];
};

/**
 * These variables are used by the other macros to determine the
 * trampoline's size, and offsets to other data.
 */
#define LCD_TRAMPOLINE_DATA(name)			 \
	extern char name##_size[];			 \
	extern char name##_offset[];			 \
	extern const char name##_addr[]; //_bits_loc[];	 

/**
 * This should be "attached" to the trampoline to put it in its
 * own section.
 */
#define LCD_TRAMPOLINE_LINKAGE(name) \
	__attribute__((section("." #name ".text")))	 

        /* ".pushsection .rodata \n\t"             \ */
        /* ".global " #fname "_addr_bits_loc \n\t"  \ */
        /* #fname "_addr_bits_loc: .quad " #fname "_addr \n\t" \ */
        /* ".popsection \n\t"                               \ */
	/* 						 \ */

/**
 * This macro should be placed near the beginning of the
 * trampoline you write.
 */
#define LCD_TRAMPOLINE_PROLOGUE(hidden_args, fname) {	\
    void **__ip;                       \
    asm(			      \
        "jmp 1f \n\t"		      \
				      \
        /* foo_trampoline_addr: */    \
        ".global " #fname "_addr \n\t" \
        #fname "_addr: \n\t"           \
        ".quad 0 \n\t" /* 64 bits, all 0 */	\
						\
        "1: \n\t"                                        \
        /* This next bit may not be portable. It assumes that */  \
        /* the following instruction is 7 bytes. RIP-relative */  \
        /* addressing is relative to the *next* instruction.  */  \
        "movq -0xf(%%rip), %0 \n\t" \
		                    \
		: "=g"(__ip)   \
		::);           \
    hidden_args = *(__ip - 1);	\
}


/**
 * You should "call" this macro when you want to duplicate a trampoline.
 */
#define LCD_DUP_TRAMPOLINE(name)					\
	lcd_dup_trampoline((char *)name, (unsigned long)name##_size,	\
			(unsigned long)name##_offset)

static inline struct lcd_trampoline_handle * 
lcd_dup_trampoline(char *t_bits,
		unsigned long t_size,
		unsigned long t_offset)
{
	struct lcd_trampoline_handle *t_handle;
	char *trampoline;
	unsigned long trampoline_addr;
	/*
	 * Allocate the trampoline
	 */
	t_handle = kzalloc(sizeof(*t_handle) + t_size, GFP_KERNEL);
	if (!t_handle)
		return NULL;
	trampoline = t_handle->trampoline;
	/*
	 * Copy trampoline bits
	 */
	memcpy(trampoline, t_bits, t_size);
	/*
	 * Patch trampoline's address into itself, at the offset (this is
	 * in the trampoline prologue; see LCD_TRAMPOLINE_PROLOGUE)
	 */
	trampoline_addr = (unsigned long)trampoline;
	memcpy(trampoline + t_offset, &trampoline_addr, sizeof(unsigned long));

	return t_handle;
}

#define LCD_TRAMPOLINE_TO_HIDDEN_ARGS(t) \
	(*(((void **)(t)) - 1))

#define LCD_HANDLE_TO_TRAMPOLINE(t) ((void *)((t)->trampoline))


#endif /* LCD_DOMAINS_TRAMPOLINE_H */
