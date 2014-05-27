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
#include <lcd/ipc.h>
#include <lcd/lcd_defs.h>
#include <lcd/console.h>


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

