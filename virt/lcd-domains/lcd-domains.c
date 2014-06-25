/**
 * lcd-domains.c - Main file for the LCD module
 *
 *
 * Authors:
 *   Anton Burtsev   <aburtsev@flux.utah.edu>
 */

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/miscdevice.h>
#include <linux/compat.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include <linux/lcd-domains.h>
#include <asm/lcd-domains-arch.h>
#include <lcd-domains/lcd-domains.h>
#include <lcd-domains/syscall.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LCD driver");

/* lcd create / destroy ---------------------------------------- */

static int lcd_create(struct lcd **lcd_out)
{
	struct lcd *lcd;
	int r;

	/*
	 * Alloc and init lcd
	 */
	lcd = (struct lcd *)kmalloc(sizeof(*lcd), GFP_KERNEL);
	if (!lcd) {
		printk(KERN_ERR "lcd_create: error alloc'ing lcd\n");
		r = -ENOMEM;
		goto fail1;
	}
	lcd->lcd_arch = lcd_arch_create();
	if(!lcd->lcd_arch) {
		printk(KERN_ERR "lcd_create: error creating lcd_arch\n");
		r = -ENOMEM;
		goto fail2;
	}

	/*
	 * Point utcb to arch-dependent utcb.
	 */
	lcd->utcb = lcd_arch->utcb;

	*lcd_out = lcd;

fail3:
	lcd_arch_destroy(lcd->lcd_arch);
fail2:
	kfree(lcd);
fail1:
	return r;

}

static void lcd_destroy(struct lcd *lcd)
{
	/*
	 * All memory mapped in lcd's ept will be automatically
	 * freed when the ept is teared down. This currently
	 * includes:
	 *
	 *   -- utcb
	 *   -- guest virtual paging memory
	 */
	lcd_arch_destroy(lcd->lcd_arch);
	kfree(lcd);
}

/* Memory Management ---------------------------------------- */

/**
 * Maps 
 *
 *    gpa_start --> gpa_start + npages * PAGE_SIZE
 *
 * to
 *
 *    hpa_start --> hpa_start + npages * PAGE_SIZE
 *
 * in lcd's ept.
 */
static int lcd_mm_gpa_map_range(struct lcd *lcd, u64 gpa_start, u64 hpa_start, 
			u64 npages)
{
	u64 off;
	u64 len;

	len = npages * PAGE_SIZE;
	for (off = 0; off < len; off += PAGE_SIZE) {
		if (lcd_arch_ept_map_gpa_to_hpa(lcd->lcd_arch,
							/* gpa */
							gpa_start + off,
							/* hpa */
							hpa_start + off,
							/* create */
							1,
							/* no overwrite */
							0)) {
			printk(KERN_ERR "lcd_mm_gpa_map_range: error mapping gpa %lx to hpa %lx\n",
				(unsigned long)(gpa_start + off),
				(unsigned long)(hpa_start + off));
			return -EIO;
		}
	}

	return 0;
}

/**
 * Initializes guest virtual address space info in lcd, and
 * sets gva root pointer (e.g., x86 %cr3).
 *
 * Must be called before mapping any gva's.
 */
int lcd_mm_gva_init(struct lcd *lcd, u64 gv_paging_mem_gpa_start,
		u64 gv_paging_mem_end)
{
	u64 root;
	int ret;

	/*
	 * Set start / end (we will use one page for the
	 * global page dir)
	 */
	lcd->gv.paging_mem_bot = gv_paging_mem_start_gpa;
	lcd->gv.paging_mem_brk = gv_paging_mem_start_gpa + PAGE_SIZE;
	lcd->gv.paging_mem_top = gv_paging_mem_end_gpa;
	
	if (lcd->gv.paging_mem_brk > lcd->gv.paging_mem_top) {
		printk(KERN_ERR "lcd_mm_gva_init: not enough room in gpa for paging\n");
		return -EINVAL;			
	}

	/*
	 * Allocate a page for the global page dir.
	 */
	root = __get_free_page(GFP_KERNEL);
	if (!root) {
		printk(KERN_ERR "lcd_mm_gva_init: error allocating global page dir\n");
		return -ENOMEM;
	}
	memset((void *)root, 0, PAGE_SIZE);
	lcd->gv.root_hva = root;
	
	/*
	 * Map global page dir's page in lcd's ept
	 */
	ret = lcd_mm_gpa_map_range(lcd, gv_paging_mem_start_gpa, root, 1);
	if (ret) {
		printk("lcd_mm_gva_init: error mapping global page dir\n");
		free_page(root);
		return ret;
	}

	return 0;
}

/**
 * Maps 
 *
 *    gva_start --> gva_start + npages * PAGE_SIZE
 *
 * to
 *
 *    gpa_start --> gpa_start + npages * PAGE_SIZE
 *
 * in lcd's guest virtual paging tables.
 *
 * Note! Call lcd_mm_gva_init before mapping any gva's.
 */
int lcd_mm_gva_map_range(struct lcd *lcd, u64 gva_start, u64 gpa_start, 
			u64 npages)
{
	/* unimplemented */
	return -1;
}

/* BLOBs -------------------------------------------------- */

static int lcd_do_run_blob_once(struct lcd *lcd)
{
	int r;
	int syscall_id;

	r = lcd_arch_run(lcd->lcd_arch);
	if (r < 0) {
		printk(KERN_ERR "lcd_do_run_blob_once: error running blob\n");
		goto out;
	}

	switch(r) {
	case LCD_ARCH_STATUS_PAGE_FAULT:
		/*
		 * Paging shouldn't be needed for blob (blob shouldn't
		 * use instructions that access memory, for now)
		 */
		printk(KERN_ERR "lcd_run_blob: page fault\n");
		r = -EIO;
		goto out;
		break;
	case LCD_ARCH_STATUS_EXT_INTR:
		/*
		 * Continue
		 */
		printk(KERN_ERR "lcd_run_blob: got external intr\n");
		r = 0;
		goto out;
	case LCD_ARCH_STATUS_EPT_FAULT:
		/*
		 * EPT should everything mapped the blob needs, so
		 * there's a problem. Quit.
		 */
		printk(KERN_ERR "lcd_run_blob: ept fault\n");
		r = -EIO;
		goto out;
	case LCD_ARCH_STATUS_CR3_ACCESS:
		/*
		 * %cr3 shouldn't be accessed for simple blobs (for
		 * now). Quit.
		 */
		printk(KERN_ERR "lcd_run_blob: cr3 access\n");
		r = -EIO;
		goto out;
	case LCD_ARCH_STATUS_SYSCALL:
		/*
		 * Only allow yield syscalls for now
		 */
		syscall_id = LCD_ARCH_GET_SYSCALL_NUM(lcd->lcd_arch);
		printk(KERN_ERR "lcd_run_blob: handling syscall %d\n",
			syscall_id);
		if (syscall_id != LCD_SYSCALL_YIELD) {
			printk(KERN_ERR "lcd_run_blob: unexpected syscall id %d\n",
				syscall_id);
			r = -EIO;
			goto out;
		} else {
			printk(KERN_ERR "lcd_run_blob: lcd yielded, exiting lcd...\n");
			r = 0;
			goto out;
		}
	}

out:
	return r;
}

static int lcd_do_run_blob(struct lcd *lcd)
{
	int r;
	while (1) {
		r = lcd_do_run_blob_once(lcd);
		if (r)
			return r;
	}
}

static int lcd_init_blob(struct lcd *lcd, unsigned char *blob,
			unsigned int blob_order)
{
	unsigned int blob_size;
	int r;
	u64 paging_mem_size;
	u64 npages;

	/*
	 * (initial)
	 * Blob Memory Layout
	 * ==================
	 *
	 * The layout below reflects the guest physical *and* virtual memory
	 * layout with the exception that not all paging memory is mapped in
	 * in the guest physical address space (for efficiency). 
	 *
	 * Guest physical addresses are mapped one-to-one to the same guest 
	 * virtual addresses.
	 *
	 * All allocated guest physical memory--including the arch-dependent 
	 * region, guest virtual page tables, and the lcd's code--is mapped
	 * in the guest virtual address space.
	 *
	 *                   +---------------------------+
	 *                   |                           |
	 *                   :                           :
	 *                   :      Free / Unmapped      :
	 *                   |                           |
	 *                   +---------------------------+
	 *                   |           Blob            | (max 16 pgs)	 
	 * blob entry------> +---------------------------+ 
	 *                   |       Guest Virtual       | (4 MBs)
	 *                   |       Paging Memory       |
	 * LCD_ARCH_FREE---> +---------------------------+
	 *                   |                           |
	 *                   :   Reserved Arch Memory    :
	 *                   |                           |
	 *                   +---------------------------+ 0x0000 0000 0000 0000
	 */
	
	paging_mem_size = 4 * (1 << 20); /* 4 MBs */

	/*
	 * Initialize guest virtual paging
	 */
	r = lcd_mm_gva_init(lcd, LCD_ARCH_FREE, 
			LCD_ARCH_FREE + paging_mem_size);
	if (r) {
		printk(KERN_ERR "lcd_init_blob: error setting up gva\n");
		goto fail1;
	}

	/*
	 * Map blob in guest physical, after paging mem
	 */
	r = lcd_mm_gpa_range(lcd, LCD_ARCH_FREE + paging_mem_size, 
			__pa((u64)blob), (1 << blob_order));
	if (r) {
		printk(KERN_ERR "lcd_init_blob: error mapping blob in gpa\n");
		goto fail2;
	}

	/*
	 * Map gpa from 0 to top of blob in lcd's gva
	 */
	npages = (LCD_ARCH_FREE + paging_mem_size) >> PAGE_SHIFT;
	npages += (1 << blob_order);
	r = lcd_mm_gva_range(lcd, 
			/* gva start */
			0, 
			/* gpa start */
			0, 
			/* num pages */
			npages);
	if (r) {
		printk(KERN_ERR "lcd_init_blob: error setting up initial gva\n");
		goto fail3;
	}

	/*
	 * Initialize program counter to blob entry point (just after
	 * guest virtual paging mem).
	 */
	r = lcd_arch_set_pc(lcd, LCD_ARCH_FREE + paging_mem_size);
	if (r) {
		printk(KERN_ERR "lcd_init_blob: error setting prgm counter\n");
		goto fail4;
	}

	return 0;

fail4:
fail3:
fail2:
fail1:
	/* 
	 * For now, relying on the fact that all paging memory allocated
	 * for lcd is mapped in ept, so it will be freed eventually
	 * when the lcd_arch is destroyed.
	 *
	 * Probably better to unmap and free instead, in the future ...
	 */
	return r;
}

static int lcd_run_blob(struct lcd_blob_info *bi)
{
	struct lcd *lcd;
	int r;
	unsigned char *blob;
	int i;

	/*
	 * Sanity check blob order
	 */
	if (bi->blob_order > 4) {
		printk(KERN_ERR "lcd_run_blob: blob is bigger than 16 pgs\n");
		r = -EINVAL;
		goto fail1;
	}
	
	/*
	 * Load blob mem
	 */
	blob = (unsigned char *)__get_free_pages(GFP_KERNEL, bi->blob_order);
	if (!blob) {
		printk(KERN_ERR "lcd_run_blob: couldn't alloc mem for blob\n");
		r = -ENOMEM;
		goto fail2;
	}

	/*
	 * Copy blob
	 */
	r = copy_from_user(blob, (void __user *)bi->blob, 
			(1 << bi->blob_order) * PAGE_SIZE);
	if (r) {
		printk(KERN_ERR "lcd_run_blob: error copying blob\n");
		goto fail3;
	}

	/*
	 * Alloc and init lcd
	 */
	r = lcd_create(&lcd);
	if (r) {
		printk(KERN_ERR "lcd_run_blob: error creating lcd\n");
		goto fail4;
	}

	/*
	 * Initialize lcd for blob
	 */
	r = lcd_init_blob(lcd, blob, bi->blob_order);
	if (r) {
		printk(KERN_ERR "lcd_run_blob: error loading blob in lcd\n");
		r = -EIO;
		goto fail5;
	}

	/*
	 * Run blob until it yields or fails
	 */
	r = lcd_do_run_blob(lcd);
	goto done;

done:
fail5:
	lcd_destroy(lcd);
	/* blob is freed via ept tear down ... kind of ugly for now ... */
	return r;
fail4:
fail3:
	free_pages((u64)blob, bi->blob_order);
fail2:
fail1:
	return r;
}


/* ioctl -------------------------------------------------- */

static long lcd_dev_ioctl(struct file *filp,
			  unsigned int ioctl, unsigned long arg)
{
	long r = -EINVAL;
	struct lcd_pv_kernel_config conf;
	struct lcd_blob_info bi;

	switch (ioctl) {
	case LCD_LOAD_PV_KERNEL:
		r = copy_from_user(&conf, (int __user *) arg,
				sizeof(struct lcd_pv_kernel_config));
		if (r) {
			r = -EIO;
			goto out;
		}
		/* create LCD with a PV Linux kernel */
		goto out;
		break;
	case LCD_RUN_BLOB:
		r = copy_from_user(&bi, (void __user *)arg, sizeof(bi));
		if (r) {
			printk(KERN_ERR "lcd: error loading blob info\n");
			r = -EIO;
			goto out;
		}
		r = lcd_run_blob(&bi);
		printk(KERN_ERR "lcd: addr=%lx, order=%d\n",
			(unsigned long)bi.blob, bi.blob_order);
		if (r) {
			printk(KERN_ERR "lcd: error running blob\n");
			goto out;
		}
		r = 0;
		goto out;
	default:
		return -ENOTTY;
	}

out:
	return r;
}

static const struct file_operations lcd_chardev_ops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl	= lcd_dev_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= lcd_dev_ioctl,
#endif
	.llseek		= noop_llseek,
};

static struct miscdevice lcd_dev = {
	LCD_MINOR,
	"lcd",
	&lcd_chardev_ops,
};

static int __init lcd_init(void)
{
	int r;

	printk(KERN_ERR "LCD module loaded\n");

	if ((r = lcd_arch_init())) {
		printk(KERN_ERR "lcd: failed to initialize vmx\n");
		return r;
	}

	r = misc_register(&lcd_dev);
	if (r) {
		printk(KERN_ERR "lcd: misc device register failed\n");
		
	}

	return r;
}

static void __exit lcd_exit(void)
{
	misc_deregister(&lcd_dev);
	lcd_arch_exit();
}

module_init(lcd_init);
module_exit(lcd_exit);
