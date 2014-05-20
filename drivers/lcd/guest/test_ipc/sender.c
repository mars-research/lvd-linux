//
//  tesla.c
//  
//
//  Created by jithu joseph on 2/28/14.
//  some utility routines to be used by guest
//  compile using -static -fno-builtin
//

//#include <stdio.h>
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <asm/vmx.h>
#include "../ipc.h"
#include "../lcd_defs.h"
//#include "../ipc_common_defs.h"



//#define va_arg(v,l)     __builtin_va_arg(v,l)
//#define va_start(v,l)   __builtin_va_start(v,l)
//#define  va_list        __builtin_va_list


typedef unsigned int   uint;


// The idea is to get the shared page which is
// to be used for IPC . It is the lowest page
// the bottom of the 4 Page Stack block.
// The address of which can be obtained by
// clearing 14 bits from stack pointer
static inline char* get_shared(void)
{
    char *shared;
    __asm__("and %%rsp,%0; ":"=r" (shared) : "0" (0xFFFFFFFFFFFFC000));
    return shared;
}

unsigned long
strlen(const char *s)
{
    int n;
    
    for(n = 0; s[n]; n++)
        ;
    return n;
}

static int lcd_puts(const char *buf)
{
	long ret;
    
	asm volatile("movq $1, %%rax \n\t" // SYS_write
                 "movq $1, %%rdi \n\t" // STDOUT
                 "movq %1, %%rsi \n\t" // string
                 "movq %2, %%rdx \n\t" // string len
                 "vmcall \n\t"
                 "movq %%rax, %0 \n\t" :
                 "=r" (ret) : "r" (buf), "r" (strlen(buf)) :
                 "rax", "rdi", "rsi", "rdx");
    
	return ret;
}

static int abc_putc(int fd, const char c)
{
	long ret;
    
	asm volatile("movq $1, %%rax \n\t" // SYS_write
                 "movq $1, %%rdi \n\t" // STDOUT
                 "movq %1, %%rsi \n\t" // string
                 "movq $1, %%rdx \n\t" // string len
                 "vmcall \n\t"
                 "movq %%rax, %0 \n\t" :
                 "=r" (ret) : "r" (&c) :
                 "rax", "rdi", "rsi", "rdx");
    
	return ret;
}

// %rax not getting saved automatically
static void
lcd_putc(char c)
{
    asm volatile("push %rax");
    asm volatile("movzx %0,%%rax" : : "r" (c));
    asm volatile("vmcall");
    asm volatile("pop %rax");
//    printk(KERN_ERR "%c", c);

}

static void
lcd_ipc()
{
    asm volatile("push %rax");
    //asm volatile("mov $0xdeadbeef, %rax");
    // DIR : 1  (SND) and peer capid 1
    asm volatile("mov $0x0000000100000001, %rax");
    asm volatile("vmcall");
    asm volatile("pop %rax");
    //    printk(KERN_ERR "%c", c);
    
}


static void
printint(long xx, int base, int sgn)
{
    static char digits[] = "0123456789ABCDEF";
    char buf[16];
    int i, neg;
    u64 x;
    
    neg = 0;
    if(sgn && xx < 0){
        neg = 1;
        x = -xx;
    } else {
        x = xx;
    }
    
    i = 0;
    do{
        buf[i++] = digits[x % base];
       // lcd_putc('*');
    }while((x /= base) != 0);
    if(neg)
        buf[i++] = '-';
    
    while(--i >= 0)
        lcd_putc(buf[i]);
}


// Print to the given fd. Only understands %d, %x, %p, %s.
void
my_printf(char *fmt, ...)
{
    va_list ap;
    char *s;
    int c, i, state;
    va_start(ap, fmt);
    
    state = 0;
    for(i = 0; fmt[i]; i++){
        c = fmt[i] & 0xff;
        if(state == 0){
            if(c == '%'){
                state = '%';
            } else {
                lcd_putc(c);
            }
        } else if(state == '%'){
            if(c == 'd'){
                printint(va_arg(ap, long), 10, 1);
            } else if(c == 'x' || c == 'p'){
                printint(va_arg(ap, long), 16, 0);
            } else if(c == 's'){
                s = va_arg(ap, char*);
                if(s == 0)
                    s = "(null)";
                while(*s != 0){
                    lcd_putc(*s);
                    s++;
                }
            } else if(c == 'c'){
                lcd_putc(va_arg(ap, uint));
            } else if(c == '%'){
                lcd_putc(c);
            } else {
                // Unknown % sequence.  Print it to draw attention.
                lcd_putc('%');
                lcd_putc(c);
            }
            state = 0;
        }
    }
}


void temp_fn(int var) {
    utcb_t *p_utcb = (utcb_t *) get_shared();
    int hex = 0xdeadbeef;
    char my_str[] = "lcdstr";
    
    p_utcb->mr[0] = 20;
    p_utcb->mr[1] = 21;
    p_utcb->mr[2] = 22;
    p_utcb->mr[3] = 23;
    p_utcb->mr[4] = 24;
    p_utcb->mr[5] = 25;

  //  my_printf("Temp %d and %s n %x\n", var, my_str, hex);
    //my_printf("shared pg 0x%x\n", p_utcb);
    
    lcd_ipc();
    
    
    
}

static volatile int shared_var = 0;

static int  hello_2_init(void)
{
    lcd_struct *lcd;
    
      if (shared_var == 0) {
        shared_var = 1;
        lcd = lcd_create();
        lcd_move_module(lcd, &__this_module);
        lcd_run(lcd);
        printk("%d\n", shared_var);
      //  lcd_destroy(lcd);
        return 0;
    }
    // guest
    shared_var = 2;
 
    temp_fn(69);
    
	return 0;
}

static void __exit hello_2_exit(void)
{
   printk(KERN_INFO "Goodbye, world 2\n");
   //  printf(1, "goodbye World\n");
    ;
}

module_init(hello_2_init);
module_exit(hello_2_exit);

