#include <lcd_domains/lcd_iommu.h>

/* Map the whole of heap and rammap regions for now.
 * Once the allocation logic is figured out, one can even introduce
 * a new region and map only that region inside the iommu.
 */
struct region {
	char *name;
	u64 start_gfn;
	u64 end_gfn;
} regions[] = {
	{
		.name = "heap",
		.start_gfn = gpa_to_gfn(HEAP_START_ADDR),
		.end_gfn = gpa_to_gfn(HEAP_END_ADDR),
	},
	{
		.name = "rammap",
		.start_gfn = gpa_to_gfn(RAMMAP_START_ADDR),
		.end_gfn = gpa_to_gfn(RAMMAP_END_ADDR),
	}
};

/* XXX: We are indeed walking the ept *without* a lock!
 * HINT: A mutex is initialized during ept init and never used.
 * What are the consequences?
 * - Be prepared for some stale entries and bugs
 */
int lcd_iommu_map_page(struct lcd *lcd, gpa_t gpa, unsigned int order,
		       bool force)
{
	hpa_t hpa;
	int ret;
	int i;
	phys_addr_t phys;
	u64 gfn_start, gfn_end = 1 << order;

	gfn_start = gpa_to_gfn(gpa_val(gpa));

	/* If domain pointer is null */
	if (!lcd->domain)
		return -EINVAL;

	for (i = 0; i < gfn_end; ++i) {
		gpa_t ga = gfn_to_gpa(gfn_start + i);
		ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, ga, &hpa, false);

		/* no mapping, continue */
		if (ret) {
			/* reset ret as this might be the last
			 * one in the loop. If there is no mapping
			 * we should not return error
			 */
			ret = 0;
			continue;
		}

		/* check for an existing mapping */
		phys = iommu_iova_to_phys(lcd->domain, gpa_val(ga));

		/* if an entry is already present and is not equal to the current
		 * requested mapping and if force is set, proceed to delete the
		 * old mapping to proceed with a new mapping.
		 */
		/* old == new, no change is required */
		if (phys && phys == hpa_val(hpa))
			continue;
		else if (phys && phys != hpa_val(hpa) && force) {
			/* XXX: This won't work as expected as unmapped entries are
			 * not immediately flushed to the hardware. It is just queued
			 * for flushing. There might be a race if we need to remove
			 * an old mapping and program a new one immediately. If that
			 * occurs, dmar will warn us.
			 */
			printk("%s, already mapped? old %llx | new %lx\n",
			       __func__, phys, hpa_val(hpa));
			/* unmap old mapping */
			ret = iommu_unmap(lcd->domain, gpa_val(gpa), PAGE_SIZE);
			BUG_ON(ret != PAGE_SIZE);
		}

		printk("%s, mapping gpa:hpa %lx:%lx pair\n", __func__,
		       gpa_val(ga), hpa_val(hpa));

		/* map */
		ret = iommu_map(lcd->domain, gpa_val(ga), hpa_val(hpa),
				PAGE_SIZE, IOMMU_READ | IOMMU_WRITE);

		if (ret)
			LIBLCD_ERR("%s: iommu_map failed for hpa:gpa %p:%p",
				   __func__, hpa_val(hpa), gpa_val(ga));
	}

	return ret;
}

int lcd_iommu_unmap_page(struct lcd *lcd, gpa_t gpa, unsigned int order)
{
	hpa_t hpa;
	int ret;
	int i;
	u64 gfn_start, gfn_end = 1 << order;

	gfn_start = gpa_to_gfn(gpa_val(gpa));

	if (!lcd->domain)
		return -EINVAL;

	for (i = 0; i < gfn_end; ++i) {
		gpa_t ga = gfn_to_gpa(gfn_start + i);

		ret = lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, ga, &hpa, false);

		/* no mapping. Just return */
		/* TODO: it might be possible that the LCD has freed the memory
		 * without freeing the iommu mapping. in that case, it is
		 * messed up, but we can still go ahead and try to unmap it.
		 * This is has to be handled in a more cleaner way.
		 */
		if (ret) {
			LIBLCD_WARN("%s, spurious unmap to gpa:hpa %lx:%lx pair",
				    __func__, gpa_val(ga), hpa_val(hpa));
			continue;
		}

		ret = iommu_iova_to_phys(lcd->domain, gpa_val(ga));

		printk("%s, unmapping gpa:hpa %lx:%lx pair\n", __func__,
		       gpa_val(ga), hpa_val(hpa));

		if (ret) {
			/* A valid mapping is present */
			ret = iommu_unmap(lcd->domain, gpa_val(ga), PAGE_SIZE);
			/* bug if not a page */
			BUG_ON(ret != PAGE_SIZE);
		}
	}
	/* return success */
	return 0;
}

int lcd_iommu_map_memory(struct lcd *lcd)
{
	int i, ret;
	u64 p, gfn_end;

	for (i = 0; i < ARRAY_SIZE(regions); i++) {
		p = regions[i].start_gfn;
		gfn_end = regions[i].end_gfn;

		for (; p < gfn_end; ++p) {
			gpa_t ga = gfn_to_gpa(p);
			hpa_t hpa;

			ret =
			    lcd_arch_ept_gpa_to_hpa(lcd->lcd_arch, ga, &hpa,
						    false);

			/* no entry. Just continue */
			if (ret)
				continue;

			/* A valid entry is present.
			 * Program its gpa:hpa into our IOMMU domain.
			 * For now, let's have flags as IOMMU_RW.
			 * TODO: Separate READ/WRITE
			 */
			ret = iommu_map(lcd->domain, gpa_val(ga), hpa_val(hpa),
					PAGE_SIZE, IOMMU_READ | IOMMU_WRITE);

			if (ret) {
				LIBLCD_ERR
				    ("%s: iommu_map failed for hpa:gpa %p:%p",
				     __func__, hpa_val(hpa), gpa_val(ga));
				goto fail;
			}
		}
	}

	return 0;
fail:
	lcd_iommu_unmap_memory(lcd);
	return ret;
}

int lcd_iommu_unmap_memory(struct lcd *lcd)
{
	int i, ret;
	u64 p, gfn_end;

	for (i = 0; i < ARRAY_SIZE(regions); i++) {
		p = regions[i].start_gfn;
		gfn_end = regions[i].end_gfn;

		for (; p < gfn_end; ++p) {
			gpa_t ga = gfn_to_gpa(p);

			ret = iommu_iova_to_phys(lcd->domain, gpa_val(ga));
			/* no entry. Just continue */
			if (!ret)
				continue;

			/* A valid mapping is present */
			ret = iommu_unmap(lcd->domain, gpa_val(ga), PAGE_SIZE);
			/* bug if not a page */
			BUG_ON(ret != PAGE_SIZE);
		}
	}
	/* return success */
	return 0;
}

int lcd_iommu_map_domain(struct lcd *lcd, struct pci_dev *pdev)
{
	int ret;

	/* step 0. Check if iommu is present */
	if (!iommu_present(&pci_bus_type)) {
		ret = -ENODEV;
		goto out;
	}

	if (lcd->domain) {
		LIBLCD_ERR("Domain mapping %p already found!",
			   __func__, lcd->domain);
		ret = -EEXIST;
		goto out;
	}

	/* step 1. alloc domain */
	lcd->domain = iommu_domain_alloc(&pci_bus_type);

	if (!lcd->domain) {
		ret = -ENOMEM;
		goto out;
	}

	/* We might not use IRQ remapping in the near future, atleast until
	 * someone makes it work inside LCDs. It's ok to continue.
	 */
	if (!iommu_capable(&pci_bus_type, IOMMU_CAP_INTR_REMAP)) {
		printk(KERN_WARNING "%s: No interrupt remapping support\n",
		       __func__);
	}

	/* step 2. map memory */
	ret = lcd_iommu_map_memory(lcd);
	if (ret) {
		LIBLCD_ERR("%s: assign device failed! ret %d", __func__, ret);
		goto fail_map;
	}

	/* step 3. attach domain */
	ret = iommu_attach_device(lcd->domain, &pdev->dev);
	if (ret) {
		LIBLCD_ERR("%s: attach device failed! ret %d", __func__, ret);
		goto fail_attach;
	}

	return ret;

fail_attach:
	lcd_iommu_unmap_memory(lcd);
fail_map:
	iommu_domain_free(lcd->domain);
out:
	return ret;
}

EXPORT_SYMBOL(lcd_iommu_map_domain);

int lcd_iommu_unmap_domain(struct lcd *lcd, struct pci_dev *pdev)
{
	if (!lcd->domain) {
		LIBLCD_ERR("%s Domain mapping not found!", __func__);
		return -EINVAL;
	}

	/* step 1. detach device */
	iommu_detach_device(lcd->domain, &pdev->dev);

	/* step 2. free domain */
	iommu_domain_free(lcd->domain);

	return 0;
}

EXPORT_SYMBOL(lcd_iommu_unmap_domain);
