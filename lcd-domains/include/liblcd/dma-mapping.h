

#define _dma_map_single(d, a, s, r) _dma_map_single_attrs(d, a, s, r, 0)

static inline dma_addr_t _dma_map_single_attrs(struct device *dev, void *ptr,
					      size_t size,
					      enum dma_data_direction dir,
					      unsigned long attrs)
{
	struct dma_map_ops *ops = get_dma_ops(dev);
	dma_addr_t addr;

	kmemcheck_mark_initialized(ptr, size);
	BUG_ON(!valid_dma_direction(dir));
	addr = ops->map_page(dev, virt_to_page(ptr),
			     offset_in_page(ptr), size,
			     dir, attrs);
	debug_dma_map_page(dev, virt_to_page(ptr),
			   offset_in_page(ptr), size,
			   dir, addr, true);
	return addr;
}
