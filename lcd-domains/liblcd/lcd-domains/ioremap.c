/*
 * ioremap - code for mapping device memory to LCD
 */ 

#include <linux/slab.h>
#include <liblcd/allocator.h>
#include <liblcd/mem.h>
#include <asm/lcd_domains/liblcd.h>
#include <lcd_domains/liblcd.h>

struct lcd_page_allocator *ioremap_allocator;

void *lcd_ioremap(unsigned long phys_addr, unsigned int size)
{
	return (void *)(gva_val(lcd_gpa2gva(__gpa(phys_addr))));
}

static inline gpa_t ioremap_page_block_to_addr(struct lcd_page_block *pb)
{
        return gpa_add(LCD_IOREMAP_GP_ADDR,
                lcd_page_block_to_offset(ioremap_allocator, pb));
}

static inline struct lcd_page_block *ioremap_addr_to_page_block(gpa_t addr)
{
        return lcd_offset_to_page_block(
                ioremap_allocator,
                gpa_val(addr) - gpa_val(LCD_IOREMAP_GP_ADDR));
}

int lcd_ioremap_phys(cptr_t phys_addr, unsigned int size, gpa_t *base) {
	
	unsigned int order = 0;
	struct lcd_page_block *pb = NULL;
	int ret = 0;
	gpa_t addr;

	order = ilog2(size >> PAGE_SHIFT);	
	if(order < LCD_IOREMAP_MIN_ORDER) {
		order = LCD_IOREMAP_MIN_ORDER;
	}
	
	pb = lcd_page_allocator_alloc(ioremap_allocator, order);
	if(!pb) {
                LIBLCD_ERR("failed to get free ioremap region");
                ret = -ENOMEM;
		goto exit;
	}
	
	addr = ioremap_page_block_to_addr(pb);
	
	ret = lcd_syscall_mmap(phys_addr, addr);
        if (ret) {
                LIBLCD_ERR("low level mmap failed");
                goto exit_mmap;
        }

        ret = __liblcd_mem_itree_insert(addr, (1UL << (PAGE_SHIFT + order)),
                                        phys_addr);
        if (ret) {
                LIBLCD_ERR("error inserting into mem itree");
                goto exit_itree_insert;
        }

	*base = addr;
	return ret;

exit_itree_insert:
	lcd_syscall_munmap(phys_addr);

exit_mmap:
	lcd_page_allocator_free(ioremap_allocator, pb, order);
	
exit:
	return ret;
}

void lcd_iounmap_phys(gpa_t phys_addr, unsigned long size)
{
        int ret;
        struct lcd_resource_node *n; 
	unsigned int order = 0;
	cptr_t phys;

	order = ilog2(size >> PAGE_SHIFT);

        if (order < LCD_RAM_MAP_MIN_ORDER)
                order = LCD_RAM_MAP_MIN_ORDER;

        ret = lcd_phys_to_resource_node(phys_addr, &n);
        if (ret) { 
                LIBLCD_ERR("error looking up resource node");
                return;
        }

	phys = n->cptr;

        lcd_page_allocator_free(
                ioremap_allocator,
                ioremap_addr_to_page_block(phys_addr),
                order);

        __liblcd_mem_itree_delete(n);
        
	lcd_syscall_munmap(phys);
}

static int
alloc_ioremap_metadata_memory(const struct lcd_page_allocator_cbs *cbs,
                        unsigned int alloc_order,
                        unsigned long metadata_sz,
                        void **metadata_addr)
{
        int ret = 0;
        
	*metadata_addr = kmalloc(metadata_sz, GFP_KERNEL);
        if (!*metadata_addr) {
                LIBLCD_ERR("error getting metadata mem for ioremap");
                ret = -ENOMEM;
        }

        return ret;
}

static void
free_ioremap_metadata_memory(const struct lcd_page_allocator_cbs *cbs,
                        void *metadata_addr,
                        unsigned long metadata_sz,
                        unsigned int alloc_order)
{
        kfree(metadata_addr);
}

struct lcd_page_allocator_cbs ioremap_page_allocator_cbs = {
        .alloc_map_metadata_memory = alloc_ioremap_metadata_memory,
        .free_unmap_metadata_memory = free_ioremap_metadata_memory,
};

int __liblcd__ioremap_init(void)
{
        int ret;
        ret = lcd_page_allocator_create(LCD_IOREMAP_NR_PAGES_ORDER,
                                        LCD_IOREMAP_MIN_ORDER,
                                        LCD_IOREMAP_MAX_ORDER,
                                        &ioremap_page_allocator_cbs,
                                        0, /* don't embed metadata */
                                        &ioremap_allocator);
        if (ret) {
                LIBLCD_ERR("failed to initialize ioremap map allocator");
                goto exit;
        }

        return 0;

exit:
        return ret;
}

