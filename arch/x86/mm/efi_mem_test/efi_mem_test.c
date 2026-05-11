/*
 * Copyright (C) 2023 Yinggang
 *
 * This file is released under the GPL.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <asm/io.h>

static unsigned long phys_addr = 0x000000003FAE9000;

// static void print_pte(phys_addr_t phys_addr) {
//     pgd_t *pgd;
//     p4d_t *p4d;
//     pud_t *pud;
//     pmd_t *pmd;
//     pte_t *pte;
//     unsigned long vaddr = (unsigned long)phys_to_virt(phys_addr);

//     pgd = pgd_offset(current->mm, vaddr);
//     if (pgd_none(*pgd) || pgd_bad(*pgd)) return;

//     p4d = p4d_offset(pgd, vaddr);
//     if (p4d_none(*p4d) || p4d_bad(*p4d)) return;

//     pud = pud_offset(p4d, vaddr);
//     if (pud_none(*pud) || pud_bad(*pud)) return;

//     pmd = pmd_offset(pud, vaddr);
//     if (pmd_none(*pmd) || pmd_bad(*pmd)) return;

//     pte = pte_offset_map(pmd, vaddr);
//     if (!pte) return;

//     printk(KERN_INFO "PTE before modification: %llx\n", (long long)pte_val(*pte));
//     pte_unmap(pte);
// }

// static void attempt_write(phys_addr_t phys_addr) {
//     void __iomem *vaddr;
//     vaddr = ioremap(phys_addr, PAGE_SIZE);
//     if (!vaddr) {
//         printk(KERN_INFO "Failed to map physical address\n");
//         return;
//     }

//     printk(KERN_INFO "Attempting to write to address...\n");
//     // This is a dangerous operation; it can crash your system
//     iowrite32(0xDEADBEEF, vaddr);

//     iounmap(vaddr);
// }

// static void make_writable(phys_addr_t phys_addr) {
//     // Similar to print_pte, find the PTE for the address
//     // Then modify it to make the page writable
//     // This is a simplified view; actual implementation may vary based on your setup
//     // This operation is risky and can destabilize your system
// }

static int __init memory_test_init(void) {
    void *virt_addr;
    pte_t *pte;
    unsigned int level;

    pr_info("Loading memory test module...\n");

    // Map the physical address to a virtual address
    virt_addr = ioremap(phys_addr, PAGE_SIZE);
    // virt_addr = phys_to_virt(phys_addr);
    if (!virt_addr) {
        pr_err("Failed to remap physical address\n");
        return -ENOMEM;
    }

    // Read the PTE and print permission bits
    pte = lookup_address((unsigned long)virt_addr, &level);
    if (!pte) {
        pr_info("Failed to lookup address\n");
        return -1;
    } else {
        pr_info("PTE for physical address %lx at %d: %llx\n", phys_addr, level, pte_val(*pte));
    }
    
    pr_info("Attempting to write to virtual address %lx\n", (unsigned long)virt_addr);
    *(volatile unsigned long *)virt_addr = 66;
    // iowrite32(0xDEADBEEF, virt_addr);

    iounmap(virt_addr);

    // print_pte(TEST_PHYS_ADDR);
    // attempt_write(TEST_PHYS_ADDR);
    // make_writable(TEST_PHYS_ADDR);
    // print_pte(TEST_PHYS_ADDR);
    // attempt_write(TEST_PHYS_ADDR);

    return 0; // Non-zero return means that the module couldn't be loaded.
}

static void __exit memory_test_exit(void) {
    pr_info("Unloading memory test module.\n");
}

module_init(memory_test_init);
module_exit(memory_test_exit);

MODULE_AUTHOR("Yinggang");
MODULE_DESCRIPTION("Test OVMF Memory Attributes");
MODULE_LICENSE("GPL");
