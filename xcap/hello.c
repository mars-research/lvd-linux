// Defining __KERNEL__ and MODULE allows us to access kernel-level code not usually available to userspace programs.
#undef __KERNEL__
#define __KERNEL__
 
#undef MODULE
#define MODULE
 
// Linux Kernel/LKM headers: module.h is needed by all modules and kernel.h is needed for KERN_INFO.
#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>        // included for __init and __exit macros
#include <linux/slab.h>        // included for kmalloc
#include <linux/kthread.h>
#include <linux/sched.h>
#include "cap.h"

struct task_struct *capTask;

int thread_fn(void* abc)
{
    LCD_Create_CSpace();
    LCD_Create_Object(TYPE_ENDPOINT);
	return 0;
}

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world!\n");
    capTask = kthread_create(thread_fn, NULL, "CapTest");
    return 0;    // Non-zero return means that the module couldn't be loaded.
}
 
static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

static int LCD_Create_CSpace(void)
{
    char *cspace;
    cspace = (char*) kmalloc(100, GFP_KERNEL);
    printk(KERN_INFO "CSpace is created\n");
    return 0;
}

static int LCD_Create_Object(objType_t type)
{
    return 0;
}
 
module_init(hello_init);
module_exit(hello_cleanup);
