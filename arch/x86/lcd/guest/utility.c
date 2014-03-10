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
#include "../lcd_defs.h"

typedef unsigned int   uint;


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

static int putc(int fd, const char c)
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

static inline void
lcd_putc(char c)
{
    asm volatile("movzx %0,%%rax" : : "r" (c));
    asm volatile("vmcall");
}

static void
printint(int fd, int xx, int base, int sgn)
{
    static char digits[] = "0123456789ABCDEF";
    char buf[16];
    int i, neg;
    uint x;
    
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
    }while((x /= base) != 0);
    if(neg)
    buf[i++] = '-';
    
    while(--i >= 0)
    lcd_putc(buf[i]);
}

void
printf(int fd, char *fmt, ...)
{
    char *s;
    int c, i, state;
    uint *ap;
    
    state = 0;
    ap = (uint*)(void*)&fmt + 1;
    for(i = 0; fmt[i]; i++){
        c = fmt[i] & 0xff;
        if(state == 0){
            if(c == '%'){
                state = '%';
            } else {
              //  lcd_putc(fd, c);
                lcd_putc(c);
            }
        } else if(state == '%'){
            if(c == 'd'){
                printk(KERN_ERR "printk The int is %d\n", *ap);
                printint(fd, *ap, 10, 1);
                ap++;
            } else if(c == 'x' || c == 'p'){
                printint(fd, *ap, 16, 0);
                ap++;
            } else if(c == 's'){
                s = (char*)*ap;
                ap++;
                if(s == 0)
                s = "(null)";
                while(*s != 0){
                    lcd_putc(*s);
                    s++;
                }
            } else if(c == 'c'){
                lcd_putc( *ap);
                ap++;
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

void temp_fn() {
  int check = 107;
    printk(KERN_ERR "printk2 %d\n", check);
  printf(1, "temp func \n");
    
}

static volatile int shared_var = 0;

static int  hello_2_init(void)
{
    int check = 107;
//printk(KERN_INFO "Hello, world 2\n");
    if (shared_var == 0) {
        shared_var = 1;
        lcd_struct *lcd = lcd_create();
        lcd_move_module(lcd, &__this_module);
        lcd_run(lcd);
        printk("%d\n", shared_var);
      //  lcd_destroy(lcd);
        return 0;
    }
    // guest
    shared_var = 2;
    //printk(KERN_ERR "printk2 %d\n", check);
    printf(1, "Hellow  World\n");
    temp_fn();
    
	return 0;
}

static void __exit hello_2_exit(void)
{
   printk(KERN_INFO "Goodbye, world 2\n");
   //  printf(1, "goodbye World\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);


