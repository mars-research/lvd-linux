#include "lcd.h"
#include "utils.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mm.h>

#include <asm/vmx.h>

MODULE_AUTHOR("FluxVirt");
MODULE_LICENSE("GPL");

#include "_lcd.h"

static DECLARE_BITMAP(vmx_vpid_bitmap, VMX_NR_VPIDS);
static DEFINE_SPINLOCK(vmx_vpid_lock);

static DEFINE_PER_CPU(struct vmcs *, vmxarea);
static DEFINE_PER_CPU(struct desc_ptr, host_gdt);
static DEFINE_PER_CPU(int, vmx_enabled);
static DEFINE_PER_CPU(struct vmx_vcpu *, local_vcpu);


#include "_lcd_vmx_ops.c"


static void __init lcd_init(void) {
}

static void __exit lcd_exit(void) {
}

module_init(lcd_init);
module_exit(lcd_exit);
