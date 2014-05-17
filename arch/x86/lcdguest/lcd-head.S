/* LCD-specific pieces of head.S, intended to be included in the right
	place in head.S */

#ifdef CONFIG_LCD_GUEST

#include <linux/elfnote.h>
#include <linux/init.h>

#include <asm/boot.h>
#include <asm/asm.h>
#include <asm/page_types.h>

#include <xen/interface/elfnote.h>
#include <asm/xen/interface.h>

	__INIT
ENTRY(startup_lcd)
	cld
/*
#ifdef CONFIG_X86_32
	mov %esi,xen_start_info
	mov $init_thread_union+THREAD_SIZE,%esp
#else
	mov %rsi,xen_start_info
	mov $init_thread_union+THREAD_SIZE,%rsp
#endif*/
	jmp lcd_start_kernel

	__FINIT

ELFNOTE(LCD, LCD_ELFNOTE_ENTRY,          _ASM_PTR startup_lcd)
#endif /*CONFIG_LCD_GUEST */
