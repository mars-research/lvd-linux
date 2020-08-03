#include <lcd_config/pre_hook.h>

#include "../mei_caller.h"
#include <asm/lcd_domains/liblcd.h>
#include <liblcd/sync_ipc_poll.h>

#include <lcd_config/post_hook.h>

static struct glue_cspace *c_cspace;

#define CPTR_HASH_BITS      5
static DEFINE_HASHTABLE(mei_hash, CPTR_HASH_BITS);
static DEFINE_HASHTABLE(dev_table, CPTR_HASH_BITS);
static DEFINE_HASHTABLE(pdev_hash, CPTR_HASH_BITS);

/* device for IOMMU assignment */
struct pcidev_info dev_assign = { 0x0000, 0x04, 0x00, 0x0 };

int glue_mei_init(void)
{
	int ret;
	ret = glue_cap_init();
	if (ret) {
		LIBLCD_ERR("cap init");
		goto fail1;
	}
	ret = glue_cap_create(&c_cspace);
	if (ret) {
		LIBLCD_ERR("cap create");
		goto fail2;
	}
	return 0;
fail2:
	glue_cap_exit();
fail1:
	return ret;

}

void glue_mei_exit(void)
{
	glue_cap_destroy(c_cspace);
	glue_cap_exit();
}

int glue_insert_mei_device(struct mei_device_container *dev_c)
{
	BUG_ON(!dev_c->mei);

	dev_c->my_ref = __cptr((unsigned long)dev_c->mei);

	hash_add(mei_hash, &dev_c->hentry, (unsigned long) dev_c->mei);

	return 0;
}

int glue_lookup_mei_device(struct cptr c, struct
		mei_device_container **dev_cout) {
	struct mei_device_container *dev_c;
 
	hash_for_each_possible(mei_hash, dev_c,
			hentry, (unsigned long) cptr_val(c)) {
		if (dev_c->mei == (struct mei_device*) c.cptr)
			*dev_cout = dev_c;
	}
	return 0;
}

void glue_remove_mei_device(struct mei_device_container *dev_c)
{
	hash_del(&dev_c->hentry);
}

int glue_insert_device(struct device_container *dev_c)
{
	BUG_ON(!dev_c->dev);

	dev_c->my_ref = __cptr((unsigned long)dev_c->dev);

	hash_add(dev_table, &dev_c->hentry, (unsigned long) dev_c->dev);

	return 0;
}

int glue_lookup_device(struct cptr c, struct
		device_container **dev_cout) {
	struct device_container *dev_c;

	hash_for_each_possible(dev_table, dev_c,
			hentry, (unsigned long) cptr_val(c)) {
		if (dev_c->dev == (struct device*) c.cptr)
			*dev_cout = dev_c;
	}
	return 0;
}

void glue_remove_device(struct device_container *dev_c)
{
	hash_del(&dev_c->hentry);
}

int inline glue_insert_pdev_hash(struct pci_dev_container *dev_container)
{
	BUG_ON(!dev_container->pdev);

	dev_container->my_ref = __cptr((unsigned long)dev_container->pdev);

	hash_add(pdev_hash, &dev_container->hentry,
				(unsigned long) dev_container->pdev);

	return 0;
}

int inline glue_lookup_pdev_hash(struct cptr c, struct pci_dev_container
		**dev_container_out)
{
        struct pci_dev_container *dev_container;

        hash_for_each_possible(pdev_hash, dev_container,
				hentry, (unsigned long) cptr_val(c)) {
		if (!dev_container) {
			WARN_ON(!dev_container);
			continue;
		}
		if (dev_container->pdev == (struct pci_dev*) c.cptr) {
	                *dev_container_out = dev_container;
			break;
		}
        }
        return 0;
}

void inline glue_remove_pdev_hash(struct pci_dev_container *dev_container)
{
	hash_del(&dev_container->hentry);
}


#if 0
int _cond_resched(void)
{
	int func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;

	async_msg_set_fn_type(_request, _COND_RESCHED);

	vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

#endif
int request_threaded_irq(unsigned int irq, irq_handler_t handler,
				irq_handler_t thread_fn,
				unsigned long flags, const char *name, void *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct irqhandler_t_container *irqhandler_container;
	int func_ret;

	INIT_IPC_MSG(&r);
	printk("%s, irq # %d", __func__, irq);

	irqhandler_container = kzalloc(sizeof(struct irqhandler_t_container), GFP_KERNEL);

	if (!irqhandler_container) {
		LIBLCD_ERR("kzalloc");
		func_ret = -ENOMEM;
		goto fail_alloc;
	}

	irqhandler_container->irqhandler = handler;
	/*
	 * XXX: in irq context, we don't want to be doing a lookup.
	 * so save the handler_container pointer as myref
	 */
	irqhandler_container->my_ref.cptr = (unsigned long) irqhandler_container;
	irqhandler_container->data = dev;

	async_msg_set_fn_type(_request, REQUEST_THREADED_IRQ);
	fipc_set_reg0(_request, irq);
	fipc_set_reg1(_request, irqhandler_container->my_ref.cptr);
	fipc_set_reg2(_request, flags);
	/* reg3, reg4, reg5 */
	memcpy((void*)&_request->regs[3], name, sizeof(unsigned long) * 3);
	fipc_set_reg6(_request, (unsigned long)  dev);

	LIBLCD_MSG("%s, irq %d | data %p", __func__, irq, dev);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	LIBLCD_MSG("%s returned %d", __func__, func_ret);
fail_alloc:
	return func_ret;
}

void free_irq(unsigned int irq, void *dev_id)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	INIT_IPC_MSG(&r);
	async_msg_set_fn_type(_request, FREE_IRQ);

	fipc_set_reg0(_request, irq);
	fipc_set_reg1(_request, (unsigned long) dev_id);

	LIBLCD_MSG("%s, freeing irq %d | data %p", __func__, irq, dev_id);
	vmfunc_wrapper(_request);

	return;
}

void mei_cancel_work(struct mei_device *dev)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			MEI_CANCEL_WORK);
	ret = vmfunc_wrapper(_request);
	return;
}

void mei_deregister(struct mei_device *dev)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			MEI_DEREGISTER);
	ret = vmfunc_wrapper(_request);
	return;

}

void mei_device_init(struct mei_device *dev,
		struct device *device, const struct mei_hw_ops *hw_ops)
{
	struct mei_hw_ops_container *hw_ops_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct mei_device_container *meidev_container = NULL;
 	struct device_container *dev_container = NULL;
	int ret;
 	
	glue_lookup_device(__cptr((uint64_t) dev), &dev_container);

	meidev_container = kzalloc(sizeof(*meidev_container), GFP_KERNEL);
	if (!meidev_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	meidev_container->mei = dev;

	glue_insert_mei_device(meidev_container);

	hw_ops_container = container_of(hw_ops,
		struct mei_hw_ops_container,
		mei_hw_ops);
	ret = glue_cap_insert_mei_hw_ops_type(c_cspace,
		hw_ops_container,
		&hw_ops_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	INIT_IPC_MSG(&r);

	async_msg_set_fn_type(_request,
			MEI_DEVICE_INIT);

	fipc_set_reg0(_request, meidev_container->my_ref.cptr);
	fipc_set_reg1(_request, dev_container->other_ref.cptr);
	fipc_set_reg2(_request, hw_ops_container->my_ref.cptr);

	ret = vmfunc_wrapper(_request);

	hw_ops_container->other_ref.cptr = fipc_get_reg0(_request);
fail_alloc:
fail_insert:
	return;
}

int mei_hbm_pg(struct mei_device *dev, u8 pg_cmd)
{
	int ret;
	int func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			MEI_HBM_PG);
	fipc_set_reg1(_request,
			pg_cmd);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

void mei_hbm_pg_resume(struct mei_device *dev)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			MEI_HBM_PG_RESUME);
	ret = vmfunc_wrapper(_request);
	return;

}

void mei_irq_compl_handler(struct mei_device *dev,
		struct mei_cl_cb *compl_list)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			MEI_IRQ_COMPL_HANDLER);
	ret = vmfunc_wrapper(_request);
	return;

}

int mei_irq_read_handler(struct mei_device *dev,
		struct mei_cl_cb *cmpl_list,
		int *slots)
{
	int ret;
	int func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			MEI_IRQ_READ_HANDLER);
	/* TODO: Marshal slots */
	//fipc_set_reg1(_request,
	//		slots);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;

}

int mei_irq_write_handler(struct mei_device *dev,
		struct mei_cl_cb *cmpl_list)
{
	int ret;
	int func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			MEI_IRQ_WRITE_HANDLER);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

int mei_register(struct mei_device *dev,
		struct device *parent)
{
	int ret;
	int func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			MEI_REGISTER);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

int mei_restart(struct mei_device *dev)
{
	int ret;
	int func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			MEI_RESTART);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

int mei_start(struct mei_device *dev)
{
	int ret;
	int func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			MEI_START);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

void mei_stop(struct mei_device *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			MEI_STOP);
	vmfunc_wrapper(_request);
	return;
}

bool mei_write_is_idle(struct mei_device *dev)
{
	bool func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			MEI_WRITE_IS_IDLE);
	vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

int probe_callee(struct fipc_message *_request)
{
	struct pci_dev_container *dev_container;
	struct pci_driver_container *pdrv_container;
	struct pci_device_id *id;
	struct device_container *device_container;
	struct ext_registers *_regs = get_register_page(smp_processor_id());
	uint64_t *regs = &_regs->regs[0];
	int func_ret;
	int ret = 0;
	//unsigned int devfn;
	struct pci_dev *pdev;
	cptr_t pdrv_ref;
	u32 i = 0;

	LIBLCD_MSG("%s called", __func__);
 
	pdrv_ref = __cptr(regs[i++]);

	ret = glue_cap_lookup_pci_driver_type(c_cspace,
				pdrv_ref, &pdrv_container);

	if (ret) {
		LIBLCD_ERR("fail lookup");
		goto fail_lookup;
	}

	dev_container = kzalloc(sizeof( struct pci_dev_container   ),
		GFP_KERNEL);
	if (!dev_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	pdev = kzalloc(sizeof( struct pci_dev  ),
		GFP_KERNEL);
	if (!pdev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	dev_container->pdev = pdev;

	glue_insert_pdev_hash(dev_container);

	device_container = kzalloc(sizeof( struct device_container   ),
		GFP_KERNEL);
	if (!device_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	device_container->dev = &dev_container->pdev->dev;

	glue_insert_device(device_container);

	dev_container->other_ref.cptr = regs[i++];
	device_container->other_ref.cptr = regs[i++];
	pdev->dma_mask = regs[i++];
	/* needed for pci_is_enabled() check */
	atomic_set(&pdev->enable_cnt, regs[i++]);

	// unmarshal sentinel array
	{
		u32 num_elements = regs[i++];
		u32 e;
		id = kzalloc(sizeof(struct pci_device_id) * num_elements, GFP_KERNEL);
		
		if (!id) {
			LIBLCD_ERR("kzalloc");
			goto fail_alloc;
		}
#define ID(x)	id->x
		for (e = 0; e < num_elements; e++) {
			ID(vendor) = regs[i++];
			ID(device) = regs[i++];
			ID(subvendor) = regs[i++];
			ID(subdevice) = regs[i++];
			ID(class) = regs[i++];
			ID(class_mask) = regs[i++];
			ID(driver_data) = regs[i++];
		}
	}

	dev_container->pdev->dev.kobj.name = "mei_me_lcd";
	dev_container->pdev->vendor = PCI_ANY_ID;
	dev_container->pdev->device = PCI_ANY_ID;

	func_ret = pdrv_container->pci_driver.probe(dev_container->pdev, id);

	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, dev_container->my_ref.cptr);

fail_alloc:
fail_lookup:
	return ret;
}

int remove_callee(struct fipc_message *_request)
{
	struct pci_driver_container *pdrv_container;
	struct pci_dev_container *pdev_container = NULL;
	int ret;

	ret = glue_cap_lookup_pci_driver_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &pdrv_container);

	if (ret) {
		LIBLCD_ERR("fail lookup");
		goto fail_lookup;
	}

	glue_lookup_pdev_hash(__cptr(fipc_get_reg1(_request)), &pdev_container);

	pdrv_container->pci_driver.remove(pdev_container->pdev);

fail_lookup:
	return ret;
}

void pci_disable_device(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container = NULL;

	INIT_IPC_MSG(&r);

	glue_lookup_pdev_hash(__cptr((uint64_t)dev), &device_container);

	async_msg_set_fn_type(_request, PCI_DISABLE_DEVICE);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	return;
}

void pci_disable_msi(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container = NULL;

	INIT_IPC_MSG(&r);

	glue_lookup_pdev_hash(__cptr((uint64_t)dev), &device_container);

	async_msg_set_fn_type(_request, PCI_DISABLE_MSI);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	vmfunc_wrapper(_request);

	return;
}

int pci_enable_device(struct pci_dev *dev)
{
	int func_ret = 0;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container = NULL;

	INIT_IPC_MSG(&r);

	glue_lookup_pdev_hash(__cptr((uint64_t)dev), &device_container);

	async_msg_set_fn_type(_request, PCI_ENABLE_DEVICE);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int pci_enable_msi_range(struct pci_dev *dev, int minvec, int maxvec)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_dev_container *device_container = NULL;

	INIT_IPC_MSG(&r);

	glue_lookup_pdev_hash(__cptr((uint64_t)dev), &device_container);

	async_msg_set_fn_type(_request, PCI_ENABLE_MSI_RANGE);

	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, minvec);
	fipc_set_reg2(_request, maxvec);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

void __iomem * pci_iomap(struct pci_dev *dev,
		int bar,
		unsigned long maxlen)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	cptr_t res0_cptr;
	gpa_t gpa_addr;
	unsigned int res0_len;
	void __iomem *dev_resource_0;
	struct pci_dev_container *dev_container = NULL;
	int ret;
	unsigned int devfn;

	/* remap pci_resource 0 */
	ret = lcd_cptr_alloc(&res0_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_cptr;
	}

	glue_lookup_pdev_hash(__cptr((uint64_t)dev), &dev_container);

	LIBLCD_MSG("%s pdev: %p sending pdev_other.cptr %lx", __func__,
			dev,
			dev_container->other_ref.cptr);
	async_msg_set_fn_type(_request, PCI_IOMAP);
	fipc_set_reg0(_request, cptr_val(res0_cptr));
	fipc_set_reg1(_request, dev_container->other_ref.cptr);
	fipc_set_reg2(_request, bar); 
	fipc_set_reg3(_request, maxlen);

	vmfunc_wrapper(_request);

	res0_len = fipc_get_reg0(_request);

	ret = lcd_ioremap_phys(res0_cptr, res0_len, &gpa_addr);

	if (ret) {
		LIBLCD_ERR("failed to ioremap phys");
		goto fail_ioremap;
	}

	dev_resource_0 = lcd_ioremap(gpa_val(gpa_addr), res0_len);

	if (!dev_resource_0) {
		LIBLCD_ERR("failed to ioremap virt");
		goto fail_ioremap2;
	}
	dev_container->pdev->resource[0].start = (resource_size_t) dev_resource_0;
	dev_container->pdev->resource[0].end =
		(resource_size_t)((char*)dev_resource_0 + res0_len - 1);

	LIBLCD_MSG("%s: NVME_REG_VS: 0x%X", __func__,
			*((unsigned int *)dev_resource_0));

	devfn = PCI_DEVFN(dev_assign.slot, dev_assign.fn);

	/*
	 * assign the device after we map the memory so that
	 * all the rammapped pages are automatically keyed
	 * into the iommu hardware
	 */
	ret = lcd_syscall_assign_device(dev_assign.domain,
				dev_assign.bus,
				devfn);
	if (ret)
		LIBLCD_ERR("Could not assign pci device to LCD: ret %d",
				ret);

	return dev_resource_0;
fail_cptr:
fail_ioremap:
fail_ioremap2:
	return NULL;
}

void pci_iounmap(struct pci_dev *dev, void *addr)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container = NULL;
	
	glue_lookup_pdev_hash(__cptr((uint64_t)dev), &device_container);

	async_msg_set_fn_type(_request, PCI_IOUNMAP);

	fipc_set_reg0(_request, device_container->other_ref.cptr);
	/* TODO: Handle addr */
	//fipc_set_reg1(_request, addr);
	vmfunc_wrapper(_request);
	return;
}

int __pci_register_driver(struct pci_driver *drv,
		struct module *owner,
		const char *mod_name)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_driver_container *pci_driver_container;
	struct ext_registers *_regs = get_register_page(smp_processor_id());
	uint64_t *regs = &_regs->regs[0];
	unsigned i = 0;
	int ret;
	int func_ret;

	async_msg_set_fn_type(_request, __PCI_REGISTER_DRIVER);

	pci_driver_container = container_of(drv, struct pci_driver_container, pci_driver);

	ret = glue_cap_insert_pci_driver_type(c_cspace, pci_driver_container, &pci_driver_container->my_ref);	

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert1;
	}

	regs[i++] = pci_driver_container->my_ref.cptr;
	// marshall string
	{
		size_t slen = strlen(mod_name);
		u8 num_regs = DIV_ROUND_UP(slen, sizeof(uint64_t));

		regs[i++] = slen;
		memcpy(&regs[i], mod_name, slen);
		i += num_regs;
	}
	// marshal sentinel array
	{
		/*
		 * FIXME: write a macro that takes the array and type to figure
		 * out the length of the sentinel array
		 * get_sentinelarray_len(ptr, typeof(ptr));
		 */
		u32 num_elements = 45;
		u32 e;
		regs[i++] = num_elements;
#define ID_TBL(idx, x)	drv->id_table[idx].x
		for (e = 0; e < num_elements; e++) {
			regs[i++] = ID_TBL(e, vendor);
			regs[i++] = ID_TBL(e, device);
			regs[i++] = ID_TBL(e, subvendor);
			regs[i++] = ID_TBL(e, subdevice);
			regs[i++] = ID_TBL(e, class);
			regs[i++] = ID_TBL(e, class_mask);
			regs[i++] = ID_TBL(e, driver_data);
		}
	}

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);

	return func_ret;
fail_insert1:
	return 0;
}

void pci_unregister_driver(struct pci_driver *drv)
{
	struct pci_driver_container *drv_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	INIT_IPC_MSG(&r);

	drv_container = container_of(drv,
		struct pci_driver_container,
		pci_driver);

	async_msg_set_fn_type(_request, PCI_UNREGISTER_DRIVER);
	fipc_set_reg0(_request, drv_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	glue_cap_remove(c_cspace, drv_container->my_ref);

	return;
}

void pci_release_regions(struct pci_dev *dev)
{
	struct fipc_message req;
	struct fipc_message *_request = &req;
	struct pci_dev_container *device_container = NULL;
	
	glue_lookup_pdev_hash(__cptr((uint64_t)dev), &device_container);

	async_msg_set_fn_type(_request, PCI_RELEASE_REGIONS);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	return;
}

int pci_request_regions(struct pci_dev *dev,
		const char *reg)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_dev_container *device_container = NULL;
	
	glue_lookup_pdev_hash(__cptr((uint64_t)dev), &device_container);

	INIT_IPC_MSG(&r);

	async_msg_set_fn_type(_request, PCI_REQUEST_REGIONS);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

void pci_set_master(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container = NULL;
	
	glue_lookup_pdev_hash(__cptr((uint64_t)dev), &device_container);

	INIT_IPC_MSG(&r);

	async_msg_set_fn_type(_request, PCI_SET_MASTER);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	return;
}

#if 0
void finish_wait(struct __wait_queue_head *q,
		struct __wait_queue *wait)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			FINISH_WAIT);
	ret = vmfunc_wrapper(_request);
	return;

}

long prepare_to_wait_event(struct __wait_queue_head *q,
		struct __wait_queue *wait,
		int state)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PREPARE_TO_WAIT_EVENT);
	fipc_set_reg1(_request,
			state);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

int queue_delayed_work_on(int cpu,
		struct workqueue_projection *wq,
		struct delayed_work *work,
		unsigned long delay)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			QUEUE_DELAYED_WORK_ON);
	fipc_set_reg1(_request,
			cpu);
	fipc_set_reg2(_request,
			delay);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

int queue_work_on(int cpu,
		struct workqueue_projection *wq,
		struct work_struct *work)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			QUEUE_WORK_ON);
	fipc_set_reg1(_request,
			cpu);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

long schedule_timeout(long timeout)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			SCHEDULE_TIMEOUT);
	fipc_set_reg1(_request,
			timeout);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

void __wake_up(struct __wait_queue_head *q,
		unsigned int mode,
		int nr,
		int key)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			__WAKE_UP);
	fipc_set_reg1(_request,
			mode);
	fipc_set_reg2(_request,
			nr);
	fipc_set_reg3(_request,
			key);
	ret = vmfunc_wrapper(_request);
	return;

}
#endif

int host_is_ready_callee(struct fipc_message *_request)
{
	struct mei_hw_ops *hw_ops;
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_device *dev = NULL;
	int ret = 0;
	int func_ret = 0;

	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	func_ret = hw_ops->host_is_ready(dev);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;

}

int hw_is_ready_callee(struct fipc_message *_request)
{
	struct mei_hw_ops *hw_ops;
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_device *dev = NULL;
	int ret = 0;
	int func_ret = 0;

	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	func_ret = hw_ops->hw_is_ready(dev);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int hw_reset_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	int enable = 0;
	int ret = 0;
	int func_ret = 0;

	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	enable = fipc_get_reg1(_request);

	func_ret = hw_ops->hw_reset(dev, enable);

	fipc_set_reg0(_request, func_ret);
fail_lookup:
	return ret;
}

int hw_start_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	int ret = 0;
	int func_ret = 0;

	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	func_ret = hw_ops->hw_start(dev);

	fipc_set_reg0(_request, func_ret);
fail_lookup:
	return ret;
}

int hw_config_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	int ret = 0;

	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;


	hw_ops->hw_config(dev);
fail_lookup:
	return ret;

}

int fw_status_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	struct mei_fw_status *fw_sts = NULL;
	int ret = 0;
	int func_ret = 0;

	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;


	fw_sts = kzalloc(sizeof( *fw_sts ),
		GFP_KERNEL);
	if (!fw_sts) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	func_ret = hw_ops->fw_status(dev, fw_sts);
	fipc_set_reg0(_request, func_ret);
fail_alloc:
fail_lookup:
	return ret;
}

int pg_state_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	int ret = 0;
	int func_ret = 0;

	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	func_ret = hw_ops->pg_state(dev);

	fipc_set_reg0(_request, func_ret);
fail_lookup:
	return ret;
}

int pg_in_transition_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	int ret = 0;
	int func_ret = 0;

	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	func_ret = hw_ops->pg_in_transition(dev);
	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pg_is_enabled_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	int ret = 0;
	int func_ret = 0;
	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	func_ret = hw_ops->pg_is_enabled(dev);

	fipc_set_reg0(_request, func_ret);
fail_lookup:
	return ret;
}

int intr_clear_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	int ret = 0;
	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	hw_ops->intr_clear(dev);
fail_lookup:
	return ret;

}

int intr_enable_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	int ret = 0;
	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	hw_ops->intr_enable(dev);
fail_lookup:
	return ret;

}

int intr_disable_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	int ret = 0;
	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	hw_ops->intr_disable(dev);
fail_lookup:
	return ret;

}

int hbuf_free_slots_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	int ret = 0;
	int func_ret = 0;
	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	func_ret = hw_ops->hbuf_free_slots(dev);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;

}

int hbuf_is_ready_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	int ret = 0;
	int func_ret = 0;

	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	func_ret = hw_ops->hbuf_is_ready(dev);

	fipc_set_reg0(_request, func_ret);
fail_lookup:
	return ret;

}

int hbuf_max_len_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	int ret = 0;
	unsigned 	int func_ret = 0;
	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	func_ret = hw_ops->hbuf_max_len(dev);

	fipc_set_reg0(_request,
			func_ret);
fail_lookup:
	return ret;

}

int write_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	struct mei_msg_hdr *hdr = NULL;
	unsigned 	char *buf = 0;
	int ret = 0;
	int func_ret = 0;

	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	/* TODO: handle buf and hdr */
	hdr = kzalloc(sizeof( *hdr ),
		GFP_KERNEL);
	if (!hdr) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	buf = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!buf) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	/* TODO: handle buf */
	//buf = fipc_get_reg1(_request);
	func_ret = hw_ops->write(dev, hdr, buf);
	fipc_set_reg0(_request, func_ret);

fail_alloc:
fail_lookup:
	return ret;
}

int rdbuf_full_slots_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	int ret = 0;
	int func_ret = 0;
	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	func_ret = hw_ops->rdbuf_full_slots(dev);
	fipc_set_reg0(_request,
			func_ret);
fail_lookup:
	return ret;

}

int read_hdr_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	int ret = 0;
	unsigned 	int func_ret = 0;
	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	func_ret = hw_ops->read_hdr(dev);
	fipc_set_reg0(_request,
			func_ret);
fail_lookup:
	return ret;

}

int read_callee(struct fipc_message *_request)
{
	struct mei_hw_ops_container *ops_container = NULL;
	struct mei_device_container *meidev_container = NULL;
	struct mei_hw_ops *hw_ops;
	struct mei_device *dev = NULL;
	unsigned 	char *buf = 0;
	unsigned 	long len = 0;
	int ret = 0;
	int func_ret = 0;

	ret = glue_cap_lookup_mei_hw_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container); 

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hw_ops = &ops_container->mei_hw_ops;

	glue_lookup_mei_device(__cptr(fipc_get_reg1(_request)), &meidev_container);

	dev = meidev_container->mei;

	/* TODO: handle buffer */
	buf = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!buf) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	/* TODO: handle buf */
	//buf = fipc_get_reg1(_request);
	len = fipc_get_reg2(_request);
	func_ret = hw_ops->read(dev, buf, len);
	fipc_set_reg0(_request, func_ret);
fail_alloc:
fail_lookup:
	return ret;

}

int mei_irq_handler_callee(struct fipc_message *_request)
{
	struct irqhandler_t_container *irqhandler_container;
	irqreturn_t irqret;
	int irq;
	int ret = 0;

	irq = fipc_get_reg0(_request);
	irqhandler_container = (struct irqhandler_t_container *)
		fipc_get_reg1(_request);

	/* call real irq handler */
	irqret = irqhandler_container->irqhandler(irq,
			irqhandler_container->data);

	//printk("%s, irq:%d irqret %d\n", __func__, irq, irqret);
	fipc_set_reg0(_request, irqret);
	return ret;
}
