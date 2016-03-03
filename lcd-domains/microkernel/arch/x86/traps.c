/******************************************************************************
 * arch/x86/traps.c
 * 
 * Modifications to Linux original are copyright (c) 2002-2004, K A Fraser
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  Pentium III FXSR, SSE support
 * Gareth Hughes <gareth@valinux.com>, May 2000
 */

#include <linux/module.h>
#include <linux/kallsyms.h>
#include <asm/lcd_domains/main.h>
#include <liblcd/address_spaces.h>

static int debug_stack_lines = 20;

#define _p(_x) ((void *)(unsigned long)(_x))
#define stack_words_per_line 4
#define ESP_BEFORE_EXCEPTION(regs) ((unsigned long)regs->rsp)

#if 0

static void show_guest_stack(struct vcpu *v, const struct cpu_user_regs *regs)
{
    int i;
    unsigned long *stack, addr;
    unsigned long mask = STACK_SIZE;

    /* Avoid HVM as we don't know what the stack looks like. */
    if ( is_hvm_vcpu(v) )
        return;

    if ( is_pv_32bit_vcpu(v) )
    {
        compat_show_guest_stack(v, regs, debug_stack_lines);
        return;
    }

    if ( vm86_mode(regs) )
    {
        stack = (unsigned long *)((regs->ss << 4) + (regs->esp & 0xffff));
        printk("Guest stack trace from ss:sp = %04x:%04x (VM86)\n  ",
               regs->ss, (uint16_t)(regs->esp & 0xffff));
    }
    else
    {
        stack = (unsigned long *)regs->esp;
        printk("Guest stack trace from "__OP"sp=%p:\n  ", stack);
    }

    if ( !access_ok(stack, sizeof(*stack)) )
    {
        printk("Guest-inaccessible memory.\n");
        return;
    }

    if ( v != current )
    {
        struct vcpu *vcpu;

        ASSERT(guest_kernel_mode(v, regs));
        vcpu = maddr_get_owner(read_cr3()) == v->domain ? v : NULL;
        if ( !vcpu )
        {
            stack = do_page_walk(v, (unsigned long)stack);
            if ( (unsigned long)stack < PAGE_SIZE )
            {
                printk("Inaccessible guest memory.\n");
                return;
            }
            mask = PAGE_SIZE;
        }
    }

    for ( i = 0; i < (debug_stack_lines*stack_words_per_line); i++ )
    {
        if ( (((long)stack - 1) ^ ((long)(stack + 1) - 1)) & mask )
            break;
        if ( __get_user(addr, stack) )
        {
            if ( i != 0 )
                printk("\n    ");
            printk("Fault while accessing guest memory.");
            i = 1;
            break;
        }
        if ( (i != 0) && ((i % stack_words_per_line) == 0) )
            printk("\n  ");
        printk(" %p", _p(addr));
        stack++;
    }
    if ( mask == PAGE_SIZE )
    {
        BUILD_BUG_ON(PAGE_SIZE == STACK_SIZE);
        unmap_domain_page(stack);
    }
    if ( i == 0 )
        printk("Stack empty.");
    printk("\n");
}

#endif

/*
 * Notes for get_stack_trace_bottom() and get_stack_dump_bottom()
 *
 * Stack pages 0, 1 and 2:
 *   These are all 1-page IST stacks.  Each of these stacks have an exception
 *   frame and saved register state at the top.  The interesting bound for a
 *   trace is the word adjacent to this, while the bound for a dump is the
 *   very top, including the exception frame.
 *
 * For the cases where the stack should not be inspected, pretend that the
 * passed stack pointer is already out of reasonable bounds.
 */
unsigned long get_stack_trace_bottom(unsigned long sp)
{
    switch ( get_stack_page(sp) )
    {
    case 0 ... 2:
        return ROUNDUP(sp, PAGE_SIZE) - sizeof(unsigned long);
    default:
        return sp - sizeof(unsigned long);
    }
}

unsigned long get_stack_dump_bottom(unsigned long sp)
{
    switch ( get_stack_page(sp) )
    {
    case 0 ... 2:
        return ROUNDUP(sp, PAGE_SIZE) - sizeof(unsigned long);

    default:
        return sp - sizeof(unsigned long);
    }
}

#if !defined(CONFIG_FRAME_POINTER)

/*
 * Stack trace from pointers found in stack, unaided by frame pointers.  For
 * caller convenience, this has the same prototype as its alternative, and
 * simply ignores the base pointer parameter.
 */
static void _show_trace(struct lcd_arch *lcd, unsigned long sp, unsigned long __maybe_unused bp)
{
    unsigned long *stack = (unsigned long *)sp, stack_hva, addr, addr_hva;
    unsigned long *bottom = (unsigned long *)get_stack_trace_bottom(sp);
    char symname[KSYM_SYMBOL_LEN];
   

    printk("Warrning: no frame pointers, stack might be innacurate\n");

    while ( stack <= bottom )
    {

	ret = lcd_arch_ept_gpa_to_hva(lcd, (unsigned long) stack, &stack_hva);
        if (ret) { 
	    LCD_ARCH_ERR("Failed to resolve guest stack gpa into hpa\n");
	    return;
        };

        addr = *(unsigned long *)stack_hva;
	stack++;

	ret = lcd_arch_ept_gpa_to_hva(lcd, (unsigned long) addr, &addr_hva);
        if (ret) { 
	    LCD_ARCH_ERR("Failed to resolve guest stack gpa into hpa\n");
	    return;
        };

       	//if (lookup_symbol_name((unsigned long)addr_hva, symname)>= 0)
	if (sprint_symbol(symname, (unsigned long)addr_hva)>= 0)

		printk("[<%p>] %pS, %s", _p(addr), _p(addr), symname);

    }
}

#else
/* Stack trace from frames in the stack, using frame pointers */
static void _show_trace(struct lcd_arch *lcd, unsigned long sp, unsigned long bp)
{
    unsigned long *frame, next, addr, addr_hva, hva;
    int ret; 

    /* Bounds for range of valid frame pointer. */
    unsigned long low = sp, high = get_stack_trace_bottom(sp);

    /* The initial frame pointer. */
    next = bp;

    for ( ; ; )
    {
	//printk("next:%p, low:%p, high:%p\n", _p(next), _p(low), _p(high));

        /* Valid frame pointer? */
        if ( (next < low) || (next >= high) )
        {
	    //printk("Exception frame\n");
            /*
             * Exception stack frames have a different layout, denoted by an
             * inverted frame pointer.
             */
            next = ~next;
            if ( (next < low) || (next >= high) ) {
                break;
	    };
            frame = (unsigned long *)next;
            
	    //next  = frame[0];
            ret = lcd_arch_ept_gpa_to_hva(lcd, (unsigned long) &frame[0], &hva);
            if (ret) { 
	        LCD_ARCH_ERR("Failed to resolve guest stack gpa into hpa\n");
	        return;
            };
            next = *(unsigned long *)hva;

	    //addr  = frame[(offsetof(struct cpu_user_regs, eip) -
            //               offsetof(struct cpu_user_regs, ebp))
            //             / BYTES_PER_LONG];
            ret = lcd_arch_ept_gpa_to_hva(lcd, (unsigned long) &frame[(offsetof(struct cpu_user_regs, eip) - offsetof(struct cpu_user_regs, ebp)) / BYTES_PER_LONG], &hva);
            if (ret) { 
	        LCD_ARCH_ERR("Failed to resolve guest stack gpa into hpa\n");
	        return;
            };
            addr = *(unsigned long *)hva;

        }
        else
        {
            /* Ordinary stack frame. */
	    //printk("Normal frame\n");
            frame = (unsigned long *)next;
            //next  = frame[0];
            ret = lcd_arch_ept_gpa_to_hva(lcd, (unsigned long) &frame[0], &hva);
            if (ret) { 
	        LCD_ARCH_ERR("Failed to resolve guest stack gpa into hpa\n");
	        return;
            };
            next = *(unsigned long *)hva;
            //addr  = frame[1];
            ret = lcd_arch_ept_gpa_to_hva(lcd, (unsigned long) &frame[1], &hva);
            if (ret) { 
	        LCD_ARCH_ERR("Failed to resolve guest stack gpa into hpa\n");
	        return;
            };
            addr = *(unsigned long *)hva;

        }

	//ret = lcd_arch_ept_gpa_to_hva(lcd, (unsigned long) addr, &addr_hva);
        //if (ret) { 
	//    LCD_ARCH_ERR("Failed to resolve guest stack gpa into hpa\n");
	//    return;
        //};
        addr_hva = addr; 
        printk("   [<%p>] %pS\n", _p(addr), _p(addr));


        low = (unsigned long)&frame[2];
    }
}

#endif

static void show_trace(struct lcd_arch *lcd, const struct cpu_user_regs *regs)
{
    unsigned long *sp = ESP_BEFORE_EXCEPTION(regs);

    printk("LCD call trace:\n");

    /*
     * If RIP looks sensible, or the top of the stack doesn't, print RIP at
     * the top of the stack trace.
     */
    //if ( is_active_kernel_text(regs->rip) ||
    //     !is_active_kernel_text(*sp) )
        printk("   [<%p>] %pS\n", _p(regs->rip), _p(regs->rip));
    /*
     * Else RIP looks bad but the top of the stack looks good.  Perhaps we
     * followed a wild function pointer? Lets assume the top of the stack is a
     * return address; print it and skip past so _show_trace() doesn't print
     * it again.
     */
    //else
    //{
    //    printk("   [<%p>] %pS\n", _p(*sp), _p(*sp));
    //    sp++;
    //}

    _show_trace(lcd, (unsigned long)sp, regs->rbp);

    printk("\n");
}


void lcd_show_stack(struct lcd_arch *lcd, const struct cpu_user_regs *regs)
{
    /*
     * In order to handle async cactus stacks, the stack bottom is
     * always taken to be %rsp - wordsize.
     */
    gva_t stack_gva = __gva((unsigned long)regs->rsp);
    gpa_t stack_gpa;
    hva_t stack_hva;
    gva_t stack_bottom_gva = __gva(gva_val(stack) - sizeof(unsigned long));
    unsigned long data;
    
    int i, ret;

    printk("LCD stack trace from "__OP"sp=0x%lx:\n  ", gva_val(stack));

    for ( i = 0; i < (debug_stack_lines*stack_words_per_line) &&
              (gva_val(stack_gva) <= gva_val(stack_bottom_gva)); i++ )
    {
        if ( (i != 0) && ((i % stack_words_per_line) == 0) )
            printk("\n  ");
        /*
         * Resolve gva ==> gpa
         */
        stack_gpa = isolated_lcd_gva2gpa(stack_gva);
        /*
         * Resolve gpa ==> hva so we can read the memory there
         */
        ret = lcd_arch_ept_gpa_to_hva(lcd, stack_gpa, &stack_hva);
        if (ret) { 
            LCD_ARCH_ERR("Failed to resolve guest stack gpa into hpa\n");
            return;
        }
        /*
         * 

	//printk("hva_stack:%p, stack:%p\n", (unsigned long *)stack_hva, stack);

        data = *(unsigned long *)stack_hva;
	stack++;

        printk(" %016lx", data);
    }
    if ( i == 0 )
        printk("Stack empty.");

    printk("\n");

    show_trace(lcd, regs);
}

#if 0
void show_stack_overflow(unsigned int cpu, const struct cpu_user_regs *regs)
{
#ifdef MEMORY_GUARD
    unsigned long esp = regs->rsp;
    unsigned long esp_top, esp_bottom;

    esp_bottom = (esp | (STACK_SIZE - 1)) + 1;
    esp_top    = esp_bottom - PRIMARY_STACK_SIZE;

    printk("Valid stack range: %p-%p, sp=%p, tss.esp0=%p\n",
           (void *)esp_top, (void *)esp_bottom, (void *)esp,
           D

    /* Trigger overflow trace if %esp is within 512 bytes of the guard page. */
    if ( ((unsigned long)(esp - esp_top) > 512) &&
         ((unsigned long)(esp_top - esp) > 512) )
    {
        printk("No stack overflow detected. Skipping stack trace.\n");
        return;
    }

    if ( esp < esp_top )
        esp = esp_top;

    printk("Xen stack overflow (dumping trace %p-%p):\n",
           (void *)esp, (void *)esp_bottom);

    _show_trace(esp, regs->rbp);

    printk("\n");
#endif
}
#endif

static void _lcd_show_registers(
    const struct cpu_user_regs *regs, unsigned long crs[8])
{

    printk("RIP:    %04x:[<%016llx>]", regs->cs, regs->rip);
    //if ( context == CTXT_hypervisor )
    //    printk(" %pS", _p(regs->rip));
    printk("RFLAGS: %016llx   \n", regs->rflags);
   // if ( (context == CTXT_pv_guest) && v && v->vcpu_info )
    //    printk("EM: %d   ", !!vcpu_info(v, evtchn_upcall_mask));
    //printk("CONTEXT: %s", context_names[context]);
    //if ( v && !is_idle_vcpu(v) )
    //    printk(" (%pv)", v);

    printk("rax: %016llx   rbx: %016llx   rcx: %016llx\n",
           regs->rax, regs->rbx, regs->rcx);
    printk("rdx: %016llx   rsi: %016llx   rdi: %016llx\n",
           regs->rdx, regs->rsi, regs->rdi);
    printk("rbp: %016llx   rsp: %016llx   r8:  %016llx\n",
           regs->rbp, regs->rsp, regs->r8);
    printk("r9:  %016llx   r10: %016llx   r11: %016llx\n",
           regs->r9,  regs->r10, regs->r11);
    //if ( !(regs->entry_vector & TRAP_regs_partial) )
    {
        printk("r12: %016llx   r13: %016llx   r14: %016llx\n",
               regs->r12, regs->r13, regs->r14);
        printk("r15: %016llx   cr0: %016lx   cr4: %016lx\n",
               regs->r15, crs[0], crs[4]);
    }
      
    printk("cr3: %016lx   cr2: %016lx\n", crs[3], crs[2]);
    printk("ds: %04x   es: %04x   fs: %04x   gs: %04x   "
           "ss: %04x   cs: %04x\n",
           regs->ds, regs->es, regs->fs,
           regs->gs, regs->ss, regs->cs);
}

void lcd_show_registers(const struct cpu_user_regs *regs)
{
    struct cpu_user_regs fault_regs = *regs;
    unsigned long fault_crs[8];

    {
        //fault_crs[0] = read_cr0();
        //fault_crs[3] = read_cr3();
        //fault_crs[4] = read_cr4();
        //fault_regs.ds = read_sreg(ds);
        //fault_regs.es = read_sreg(es);
        //fault_regs.fs = read_sreg(fs);
        //fault_regs.gs = read_sreg(gs);
    }

    _lcd_show_registers(&fault_regs, fault_crs);

}

void lcd_show_execution_state(struct lcd_arch *lcd, 
                              const struct cpu_user_regs *regs)
{
    lcd_show_registers(regs);
    lcd_show_stack(lcd, regs);
}

#if 0
void vcpu_show_execution_state(struct vcpu *v)
{
    printk("*** Dumping Dom%d vcpu#%d state: ***\n",
           v->domain->domain_id, v->vcpu_id);

    if ( v == current )
    {
        show_execution_state(guest_cpu_user_regs());
        return;
    }

    vcpu_pause(v); /* acceptably dangerous */

    vcpu_show_registers(v);
    if ( guest_kernel_mode(v, &v->arch.user_regs) )
        show_guest_stack(v, &v->arch.user_regs);

    vcpu_unpause(v);
}
#endif

//static const char *trapstr(unsigned int trapnr)
//{
//    static const char * const strings[] = {
//        "divide error", "debug", "nmi", "bkpt", "overflow", "bounds",
//        "invalid opcode", "device not available", "double fault",
//        "coprocessor segment", "invalid tss", "segment not found",
//        "stack error", "general protection fault", "page fault",
//        "spurious interrupt", "coprocessor error", "alignment check",
//        "machine check", "simd error", "virtualisation exception"
//    };
//
//    return trapnr < ARRAY_SIZE(strings) ? strings[trapnr] : "???";
//}
