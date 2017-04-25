#ifndef _LCD_IOMMU_H
#define _LCD_IOMMU_H

#include <linux/iommu.h>
#include <asm/lcd_domains/ept.h>
#include <asm/liblcd/address_spaces.h>
#include <lcd_domains/microkernel.h>

/**
 * Map the whole heap/rammap region of LCDs in IOMMU hardware
 * There are a couple of things to note here,
 * 1) Not all pages in the heap region are backed by physical pages
 * 2) liblcd allocator supports ondemand allocation
 *
 * We should either allocate a set of pages for recv skb's during boot,
 * and then invoke IOMMU map as all the necessary gpa pages would have
 * a valid hpa. As we got a (gpa, hpa) pair, we can insert map it in the
 * IOMMU hardware.
 *
 * volunteered memory get mapped to inside the LCDs and are mapped to the 
 * RAM map region. Tx buffers are in a shared region which is volunteered
 * by the KLCD to a particular LCD (and eventually mapped using mmap).
 *
 * @param lcd pointer to the lcd structure
 * @return 0 on success and -errno on failure
 */
int lcd_iommu_map_memory(struct lcd *lcd);

/**
 * Program a single gpa:hpa pair into the iommu
 *
 * @param lcd pointer to the lcd structure
 * @param gpa guest physical address of the page to be mapped
 * @param force whether to force a remap if already mapped
 * @return 0 on success and -errno on failure
 */
int lcd_iommu_map_page(struct lcd *lcd, gpa_t gpa, unsigned int, bool force);

int lcd_iommu_unmap_page(struct lcd *lcd, gpa_t gpa);

/**
 * unmap the mappings from iommu. The logic is similar to mapping, except
 * that the mappings are removed from the iommu.
 * @param lcd pointer to the lcd structure
 * @return 0 on success and -errno on failure
 */
int lcd_iommu_unmap_memory(struct lcd *lcd);

/**
 *  Top level function for mapping a domain.
 * @param lcd pointer to the lcd structure
 * @param pdev pointer to the pci_dev structure
 * @return 0 on success and -errno on failure
 */
int lcd_iommu_map_domain(struct lcd *lcd, struct pci_dev *pdev);

int lcd_iommu_unmap_domain(struct lcd *lcd, struct pci_dev *pdev);

/**
 * Region start and end in the guest physical layout to be mapped
 */
#define HEAP_START_ADDR      LCD_HEAP_OFFSET
#define HEAP_END_ADDR        (HEAP_START_ADDR + (1 << 30) - 1)

/* XXX: There is a hole of 1GB in between the heap and rammap. */
#define RAMMAP_START_ADDR      LCD_RAM_MAP_OFFSET
#define RAMMAP_END_ADDR        (RAMMAP_START_ADDR + (1 << 30) - 1)

#define gpa_to_gfn(gpa) ((gpa) >> PAGE_SHIFT)

static inline gpa_t gfn_to_gpa(u64 gfn)
{
	return __gpa(gfn << PAGE_SHIFT);
}

#endif /* _LCD_IOMMU_H */
