#ifndef LCD_LCD_DEFS_H
#define LCD_LCD_DEFS_H

#include <linux/bitmap.h>
#include <uapi/asm/bootparam.h>
#include <xen/interface/xen.h>
#include <asm/vmx.h>
#include <lcd/ipc.h>
#include <lcd/lcd.h>


#if !defined(VMX_EPT_AD_BIT)
#define VMX_EPT_AD_BIT          (1ull << 21)
#define VMX_EPT_AD_ENABLE_BIT   (1ull << 6)
#endif

/* Where did this come from ? it's not documented in ia32 manual
#ifndef VMX_EPT_EXTENT_INDIVIDUAL_BIT
#define VMX_EPT_EXTENT_INDIVIDUAL_BIT           (1ull << 24)
#endif
*/

#ifndef X86_CR4_PCIDE
#define X86_CR4_PCIDE 0x00020000 /* enable PCID support */
#endif

#ifndef SECONDARY_EXEC_ENABLE_INVPCID
#define SECONDARY_EXEC_ENABLE_INVPCID 0x00001000
#endif

#ifndef SEG_TYPE_DATA
#define SEG_TYPE_DATA		(0 << 3)
#endif

#ifndef SEG_TYPE_READ_WRITE
#define SEG_TYPE_READ_WRITE	(1 << 1)
#endif

#ifndef SEG_TYPE_CODE
#define SEG_TYPE_CODE		(1 << 3)
#endif

#ifndef SEG_TYPE_EXEC_READ
#define SEG_TYPE_EXEC_READ	(1 << 1)
#endif

#ifndef SEG_TYPE_TSS
#define SEG_TYPE_TSS		((1 << 3) | (1 << 0))
#endif

#ifndef SEG_OP_SIZE_32BIT
#define SEG_OP_SIZE_32BIT	(1 << 0)
#endif

#ifndef SEG_GRANULARITY_4KB
#define SEG_GRANULARITY_4KB	(1 << 0)
#endif

#ifndef DESC_TYPE_CODE_DATA
#define DESC_TYPE_CODE_DATA	(1 << 0)
#endif

/* Memory management */

#define EPT_LEVELS 4

#define VMX_EPT_FAULT_READ  0x01

typedef unsigned long epte_t;

#define __EPTE_READ    0x01
#define __EPTE_WRITE   0x02
#define __EPTE_EXEC    0x04
#define __EPTE_IPAT    0x40
#define __EPTE_SZ      0x80
#define __EPTE_A       0x100
#define __EPTE_D       0x200
#define __EPTE_TYPE(n) (((n) & 0x7) << 3)

enum {
	EPTE_TYPE_UC = 0, /* uncachable */
	EPTE_TYPE_WC = 1, /* write combining */
	EPTE_TYPE_WT = 4, /* write through */
	EPTE_TYPE_WP = 5, /* write protected */
	EPTE_TYPE_WB = 6, /* write back */
};

#define __EPTE_NONE 0
#define __EPTE_FULL (__EPTE_READ | __EPTE_WRITE | __EPTE_EXEC)

#define EPTE_ADDR  (~(PAGE_SIZE - 1))
#define EPTE_FLAGS (PAGE_SIZE - 1)

#define ADDR_TO_IDX(la, n)						\
	((((unsigned long) (la)) >> (12 + 9 * (n))) & ((1 << 9) - 1))


/* VMCS related */

/* moved to lcd-vmx.h */
/* struct vmcs_config { */
/* 	int size; */
/* 	int order; */
/* 	u32 revision_id; */
/* 	u32 pin_based_exec_ctrl; */
/* 	u32 cpu_based_exec_ctrl; */
/* 	u32 cpu_based_2nd_exec_ctrl; */
/* 	u32 vmexit_ctrl; */
/* 	u32 vmentry_ctrl; */
/* }; */

/* struct vmcs { */
/* 	u32 revision_id; */
/* 	u32 abort; */
/* 	char data[0]; */
/* }; */

/* struct vmx_capability { */
/* 	u32 ept; */
/* 	u32 vpid; */
/* 	int has_load_efer:1; */
/* }; */

/* extern struct vmx_capability vmx_capability; */
/* extern struct vmcs_config vmcs_config; */


struct lcd_tss_struct {
	struct x86_hw_tss tss;
	u8 io_bitmap[1];
} __attribute__((packed));

struct ipc_waitq {
	u32 partner_id;
	struct list_head list;
};

/*
 * Guest Physical and Virtual Memory Layout
 * ========================================
 *
 * IA-32e paging is used, with a 4-level page table hierarchy (see
 * Intel Manual 4.5). IA-32e paging maps 48-bit guest virtual addresses
 * to 52-bit guest physical addresses. The upper 16 bits are ignored.
 *
 *
 *                         +---------------------------+ 0xFFFF FFFF FFFF FFFF
 *                         |                           |
 *                         :                           :
 *                         :       Kernel Module       :
 *                         :       Mapping Area        :
 *                         :                           :
 *                         |                           |
 * LCD_HEAP_END----------> +---------------------------+ TASK_SIZE (arch-dep)
 * LCD_MODULE_START        |                           |
 *                         |                           |
 *                         :                           :
 *                         :          HEAP             :
 *                         :       (grows up)          :
 *                         :                           :
 *                         :                           :
 *                         |                           |
 * LCD_HEAP_START--------> +---------------------------+ 0x0000 0000 0040 3000
 *                         |  Initial Guest Virtual    |
 *                         |      Paging Memory        | (4 MBs)
 * LCD_PAGING_MEM_START--> +---------------------------+ 0x0000 0000 0000 3000
 *                         |           GDT             | (4 KBs)
 * LCD_GDT---------------> +---------------------------+ 0x0000 0000 0000 2000
 *                         |                           |
 *                         |          Stack            |
 *                         :       (grows down)        : (4 KBs)
 *                         :                           :
 *                         |                           |
 *                         |   IPC Message Registers   |
 * LCD_ROOT_TCB----------> +---------------------------+ 0x0000 0000 0000 1000
 *                         |    Stack Canary Page      | (4 KBs)
 * LCD_STACK_CANARY,-----> +---------------------------+ 0x0000 0000 0000 0000
 * LCD_BOTTOM
 */
#define LCD_BOTTOM           0x0000000000000000UL
#define LCD_STACK_CANARY     LCD_BOTTOM
#define LCD_ROOT_TCB         0x0000000000001000UL
#define LCD_GDT              0x0000000000002000UL
#define LCD_PAGING_MEM_START 0x0000000000003000UL
#define LCD_HEAP_START       0x0000000000403000UL
#define LCD_HEAP_END         TASK_SIZE
#define LCD_MODULE_START     TASK_SIZE

#define LCD_PAGING_MEM_NUM_PAGES  ((LCD_HEAP_START - LCD_PAGING_MEM_START) \
					/ PAGE_SIZE)

#endif
