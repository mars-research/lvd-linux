#include <lcd_config/pre_hook.h>

#include <lcd_domains/microkernel.h>
#include <liblcd/trampoline.h>
#include <asm/cacheflush.h>
#include <linux/interrupt.h>

#include "../shpchp_callee.h"

#include <lcd_config/post_hook.h>

#define CPTR_HASH_BITS      5
static DEFINE_HASHTABLE(pdev_hash, CPTR_HASH_BITS);

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


int add_timer_callee(struct fipc_message *_request)
{
	struct timer_list *timer = NULL;
	int ret = 0;
	timer = kzalloc(sizeof( *timer ),
		GFP_KERNEL);
	if (!timer) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	add_timer(timer);
fail_alloc:
	return ret;
}

int del_timer_callee(struct fipc_message *_request)
{
	struct timer_list *timer = NULL;
	int ret = 0;
	int func_ret = 0;
	timer = kzalloc(sizeof( *timer ),
		GFP_KERNEL);
	if (!timer) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = del_timer(timer);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int init_timer_key_callee(struct fipc_message *_request)
{
	struct timer_list *timer = NULL;
	unsigned 	int flags = 0;
	char *name = 0;
	struct lock_class_key *key = NULL;
	int ret = 0;
	timer = kzalloc(sizeof( *timer ),
		GFP_KERNEL);
	if (!timer) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	name = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!name) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	key = kzalloc(sizeof( *key ),
		GFP_KERNEL);
	if (!key) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	flags = fipc_get_reg1(_request);
	name = fipc_get_reg2(_request);
	init_timer_key(timer,
			flags,
			name,
			key);
fail_alloc:
	return ret;

}

int device_create_file_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	struct device_attribute *attr = NULL;
	int ret = 0;
	int func_ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	attr = kzalloc(sizeof( *attr ),
		GFP_KERNEL);
	if (!attr) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = device_create_file(dev,
		attr);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int device_remove_file_callee(struct fipc_message *_request)
{
	struct device *dev = NULL;
	struct device_attribute *attr = NULL;
	int ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	attr = kzalloc(sizeof( *attr ),
		GFP_KERNEL);
	if (!attr) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	device_remove_file(dev,
			attr);
fail_alloc:
	return ret;

}

#if 0
int ioremap_nocache_callee(struct fipc_message *_request)
{
	unsigned 	long phys_addr = 0;
	unsigned 	long size = 0;
	int ret = 0;
	phys_addr = fipc_get_reg1(_request);
	size = fipc_get_reg2(_request);
	ioremap_nocache(phys_addr,
			size);
fail_alloc:
	return ret;

}

int iounmap_callee(struct fipc_message *_request)
{
	void *addr = NULL;
	int ret = 0;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long addr_offset;
	cptr_t addr_cptr;
	gva_t addr_gva;
	sync_ret = lcd_cptr_alloc(&addr_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}
	lcd_set_cr0(addr_cptr);
	sync_ret = lcd_sync_recv(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to recv");
		lcd_exit(-1);
	}
	mem_order = lcd_r0();
	addr_offset = lcd_r1();
	sync_ret = lcd_map_virt(addr_cptr,
		mem_order,
		&addr_gva);
	if (sync_ret) {
		LIBLCD_ERR("failed to map void *addr");
		lcd_exit(-1);
	}
	iounmap(( void  * )( ( gva_val(addr_gva) ) + ( addr_offset ) ));
fail_alloc:
	return ret;

}
#endif

int pci_bus_add_devices_callee(struct fipc_message *_request)
{
	struct pci_bus *bus = NULL;
	int ret = 0;
	int func_ret = 0;
	bus = kzalloc(sizeof( *bus ),
		GFP_KERNEL);
	if (!bus) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = pci_bus_add_devices(bus);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int acpi_get_hp_hw_control_from_firmware_callee(struct fipc_message *_request)
{
	struct pci_dev *pdev = NULL;
	unsigned 	int flags = 0;
	int ret = 0;
	int func_ret = 0;
	pdev = kzalloc(sizeof( *pdev ),
		GFP_KERNEL);
	if (!pdev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	flags = fipc_get_reg1(_request);
	func_ret = acpi_get_hp_hw_control_from_firmware(pdev,
		flags);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int pci_assign_unassigned_bridge_resources_callee(struct fipc_message *_request)
{
	struct pci_dev *bridge = NULL;
	int ret = 0;
	bridge = kzalloc(sizeof( *bridge ),
		GFP_KERNEL);
	if (!bridge) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	pci_assign_unassigned_bridge_resources(bridge);
fail_alloc:
	return ret;

}

int pci_bus_resource_n_callee(struct fipc_message *_request)
{
	struct pci_bus *bus = NULL;
	int n = 0;
	int ret = 0;
	struct resource_container *func_ret_container = NULL;
	struct resource *func_ret = NULL;
	bus = kzalloc(sizeof( *bus ),
		GFP_KERNEL);
	if (!bus) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	n = fipc_get_reg1(_request);
	func_ret_container->other_ref.cptr = fipc_get_reg2(_request);
	func_ret = pci_bus_resource_n(bus,
		n);
	ret = glue_cap_insert_resource_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fipc_set_reg1(_request,
			func_ret_container->other_ref.cptr);
fail_alloc:
	return ret;

}

int pci_dev_get_callee(struct fipc_message *_request)
{
	struct pci_dev *dev = NULL;
	int ret = 0;
	struct pci_dev_container *func_ret_container = NULL;
	struct pci_dev *func_ret = NULL;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret_container->other_ref.cptr = fipc_get_reg2(_request);
	func_ret = pci_dev_get(dev);
	ret = glue_cap_insert_pci_dev_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fipc_set_reg1(_request,
			func_ret_container->other_ref.cptr);
fail_alloc:
	return ret;

}

int pci_dev_put_callee(struct fipc_message *_request)
{
	struct pci_dev *dev = NULL;
	int ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	pci_dev_put(dev);
fail_alloc:
	return ret;

}

int pcie_bus_configure_settings_callee(struct fipc_message *_request)
{
	struct pci_bus *bus = NULL;
	int ret = 0;
	bus = kzalloc(sizeof( *bus ),
		GFP_KERNEL);
	if (!bus) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	pcie_bus_configure_settings(bus);
fail_alloc:
	return ret;

}

int pci_enable_device_callee(struct fipc_message *_request)
{
	struct pci_dev *dev = NULL;
	int ret = 0;
	int func_ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = pci_enable_device(dev);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int pci_find_bus_callee(struct fipc_message *_request)
{
	int domain = 0;
	int busnr = 0;
	int ret = 0;
	struct pci_bus_container *func_ret_container = NULL;
	struct pci_bus *func_ret = NULL;
	domain = fipc_get_reg1(_request);
	busnr = fipc_get_reg2(_request);
	func_ret_container->other_ref.cptr = fipc_get_reg2(_request);
	func_ret = pci_find_bus(domain,
		busnr);
	ret = glue_cap_insert_pci_bus_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fipc_set_reg1(_request,
			func_ret_container->other_ref.cptr);
fail_alloc:
	return ret;

}

int pci_find_capability_callee(struct fipc_message *_request)
{
	struct pci_dev *dev = NULL;
	int cap = 0;
	int ret = 0;
	int func_ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	cap = fipc_get_reg1(_request);
	func_ret = pci_find_capability(dev,
		cap);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int pci_get_slot_callee(struct fipc_message *_request)
{
	struct pci_bus *bus = NULL;
	unsigned 	int devfn = 0;
	int ret = 0;
	struct pci_dev_container *func_ret_container = NULL;
	struct pci_dev *func_ret = NULL;
	bus = kzalloc(sizeof( *bus ),
		GFP_KERNEL);
	if (!bus) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	devfn = fipc_get_reg1(_request);
	func_ret_container->other_ref.cptr = fipc_get_reg2(_request);
	func_ret = pci_get_slot(bus,
		devfn);
	ret = glue_cap_insert_pci_dev_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fipc_set_reg1(_request,
			func_ret_container->other_ref.cptr);
fail_alloc:
	return ret;

}

int pci_hp_add_bridge_callee(struct fipc_message *_request)
{
	struct pci_dev *dev = NULL;
	int ret = 0;
	int func_ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = pci_hp_add_bridge(dev);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int pci_hp_change_slot_info_callee(struct fipc_message *_request)
{
	struct hotplug_slot *slot = NULL;
	struct hotplug_slot_info *info = NULL;
	int ret = 0;
	int func_ret = 0;
	slot = kzalloc(sizeof( *slot ),
		GFP_KERNEL);
	if (!slot) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	info = kzalloc(sizeof( *info ),
		GFP_KERNEL);
	if (!info) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = pci_hp_change_slot_info(slot,
		info);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int pci_hp_deregister_callee(struct fipc_message *_request)
{
	struct hotplug_slot *slot = NULL;
	int ret = 0;
	int func_ret = 0;
	slot = kzalloc(sizeof( *slot ),
		GFP_KERNEL);
	if (!slot) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = pci_hp_deregister(slot);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int __pci_hp_register_callee(struct fipc_message *_request)
{
	struct hotplug_slot *slot = NULL;
	struct pci_bus *bus = NULL;
	int devnr = 0;
	char *name = 0;
	struct module *owner = NULL;
	char *mod_name = 0;
	int ret = 0;
	int func_ret = 0;
	slot = kzalloc(sizeof( *slot ),
		GFP_KERNEL);
	if (!slot) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	bus = kzalloc(sizeof( *bus ),
		GFP_KERNEL);
	if (!bus) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	name = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!name) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	owner = kzalloc(sizeof( *owner ),
		GFP_KERNEL);
	if (!owner) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	mod_name = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!mod_name) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	devnr = fipc_get_reg1(_request);
	name = fipc_get_reg2(_request);
	mod_name = fipc_get_reg3(_request);
	func_ret = __pci_hp_register(slot,
		bus,
		devnr,
		name,
		owner,
		mod_name);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int pci_lock_rescan_remove_callee(struct fipc_message *_request)
{
	int a = 0;
	int ret = 0;
	a = fipc_get_reg1(_request);
	pci_lock_rescan_remove(a);
fail_alloc:
	return ret;

}

int __pci_register_driver_callee(struct fipc_message *_request)
{
	struct pci_driver *drv = NULL;
	struct module *owner = NULL;
	char *mod_name = 0;
	int ret = 0;
	int func_ret = 0;
	drv = kzalloc(sizeof( *drv ),
		GFP_KERNEL);
	if (!drv) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	owner = kzalloc(sizeof( *owner ),
		GFP_KERNEL);
	if (!owner) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	mod_name = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!mod_name) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	mod_name = fipc_get_reg1(_request);
	func_ret = __pci_register_driver(drv,
		owner,
		mod_name);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int pci_scan_slot_callee(struct fipc_message *_request)
{
	struct pci_bus *bus = NULL;
	int devfn = 0;
	int ret = 0;
	int func_ret = 0;
	bus = kzalloc(sizeof( *bus ),
		GFP_KERNEL);
	if (!bus) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	devfn = fipc_get_reg1(_request);
	func_ret = pci_scan_slot(bus,
		devfn);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int pci_stop_and_remove_bus_device_callee(struct fipc_message *_request)
{
	struct pci_dev *dev = NULL;
	int ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	pci_stop_and_remove_bus_device(dev);
fail_alloc:
	return ret;

}

int pci_unlock_rescan_remove_callee(struct fipc_message *_request)
{
	int b = 0;
	int ret = 0;
	b = fipc_get_reg1(_request);
	pci_unlock_rescan_remove(b);
fail_alloc:
	return ret;

}

int pci_unregister_driver_callee(struct fipc_message *_request)
{
	struct pci_driver *drv = NULL;
	int ret = 0;
	drv = kzalloc(sizeof( *drv ),
		GFP_KERNEL);
	if (!drv) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	pci_unregister_driver(drv);
fail_alloc:
	return ret;

}

int __release_region_callee(struct fipc_message *_request)
{
	struct resource *parent = NULL;
	unsigned 	long start = 0;
	unsigned 	long n = 0;
	int ret = 0;
	parent = kzalloc(sizeof( *parent ),
		GFP_KERNEL);
	if (!parent) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	start = fipc_get_reg1(_request);
	n = fipc_get_reg2(_request);
	__release_region(parent,
			start,
			n);
fail_alloc:
	return ret;

}

int __request_region_callee(struct fipc_message *_request)
{
	struct resource *parent = NULL;
	unsigned 	long start = 0;
	unsigned 	long n = 0;
	char *name = 0;
	int flags = 0;
	int ret = 0;
	struct resource_container *func_ret_container = NULL;
	struct resource *func_ret = NULL;
	parent = kzalloc(sizeof( *parent ),
		GFP_KERNEL);
	if (!parent) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	name = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!name) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	start = fipc_get_reg1(_request);
	n = fipc_get_reg2(_request);
	name = fipc_get_reg3(_request);
	flags = fipc_get_reg4(_request);
	func_ret_container->other_ref.cptr = fipc_get_reg2(_request);
	func_ret = __request_region(parent,
		start,
		n,
		name,
		flags);
	ret = glue_cap_insert_resource_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fipc_set_reg1(_request,
			func_ret_container->other_ref.cptr);
fail_alloc:
	return ret;

}

int request_threaded_irq_callee(struct fipc_message *_request)
{
	unsigned 	int irq = 0;
	struct irq_handler_container *irq_handler_container = NULL;
*irq_handler = NULL;
	struct irq_handler_tnread_container *irq_handler_tnread_container = NULL;
*irq_handler_tnread = NULL;
	unsigned 	long irqflags = 0;
	char *devname = 0;
	void *dev_id = NULL;
	int ret = 0;
	int func_ret = 0;
	irq_handler_container = kzalloc(sizeof( struct irq_handler_container   ),
		GFP_KERNEL);
	if (!irq_handler_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	ret = glue_cap_insert_irq_handler_type(c_cspace,
		irq_handler_container,
		&irq_handler_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	irq_handler_tnread_container = kzalloc(sizeof( struct irq_handler_tnread_container   ),
		GFP_KERNEL);
	if (!irq_handler_tnread_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	ret = glue_cap_insert_irq_handler_tnread_type(c_cspace,
		irq_handler_tnread_container,
		&irq_handler_tnread_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	devname = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!devname) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long dev_id_offset;
	cptr_t dev_id_cptr;
	gva_t dev_id_gva;
	irq = fipc_get_reg1(_request);
	irqflags = fipc_get_reg2(_request);
	devname = fipc_get_reg3(_request);
	sync_ret = lcd_cptr_alloc(&dev_id_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}
	lcd_set_cr0(dev_id_cptr);
	sync_ret = lcd_sync_recv(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to recv");
		lcd_exit(-1);
	}
	mem_order = lcd_r0();
	dev_id_offset = lcd_r1();
	sync_ret = lcd_map_virt(dev_id_cptr,
		mem_order,
		&dev_id_gva);
	if (sync_ret) {
		LIBLCD_ERR("failed to map void *dev_id");
		lcd_exit(-1);
	}
	func_ret = request_threaded_irq(irq,
		( &irq_handler_container->irq_handler ),
		( &irq_handler_tnread_container->irq_handler_tnread ),
		irqflags,
		devname,
		( void  * )( ( gva_val(dev_id_gva) ) + ( dev_id_offset ) ));
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int free_irq_callee(struct fipc_message *_request)
{
	unsigned 	int irq = 0;
	void *dev_id = NULL;
	int ret = 0;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long dev_id_offset;
	cptr_t dev_id_cptr;
	gva_t dev_id_gva;
	irq = fipc_get_reg1(_request);
	sync_ret = lcd_cptr_alloc(&dev_id_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}
	lcd_set_cr0(dev_id_cptr);
	sync_ret = lcd_sync_recv(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to recv");
		lcd_exit(-1);
	}
	mem_order = lcd_r0();
	dev_id_offset = lcd_r1();
	sync_ret = lcd_map_virt(dev_id_cptr,
		mem_order,
		&dev_id_gva);
	if (sync_ret) {
		LIBLCD_ERR("failed to map void *dev_id");
		lcd_exit(-1);
	}
	free_irq(irq,
			( void  * )( ( gva_val(dev_id_gva) ) + ( dev_id_offset ) ));
fail_alloc:
	return ret;

}

int schedule_timeout_callee(struct fipc_message *_request)
{
	long timeout = 0;
	int ret = 0;
	long func_ret = 0;
	timeout = fipc_get_reg1(_request);
	func_ret = schedule_timeout(timeout);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int request_threaded_irq_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret = 0;
	int irq;
	unsigned long flags;
	struct irqhandler_t_container *irqhandler_container;
	unsigned char *vector_name;
	void *data;

	irqhandler_container = kzalloc(sizeof(struct irqhandler_t_container),
					GFP_KERNEL);
	if (!irqhandler_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	vector_name = kzalloc(sizeof(unsigned long) * 3, GFP_KERNEL);

	if (!vector_name) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	irq = fipc_get_reg0(_request);
	irqhandler_container->other_ref.cptr = fipc_get_reg1(_request);
	flags = fipc_get_reg2(_request);
	data = (void*) fipc_get_reg6(_request);

	memcpy(vector_name, (void *)&_request->regs[3], sizeof(unsigned long) * 3);

	LIBLCD_MSG("%s, request_threaded_irq for %d | name: %s | data %p",
			__func__, irq, vector_name, data);

	func_ret = request_threaded_irq(irq, mei_irq_handler,
				NULL, flags,
				vector_name, (void*) irqhandler_container);

	printk("%s, storing in irq_map at idx: %d\n", __func__, reg_irqs);

	irq_map[reg_irqs].irq = irq;
	irq_map[reg_irqs].data = data;
	irq_map[reg_irqs].irqhandler_data = irqhandler_container;

	reg_irqs++;
	fipc_set_reg0(_request, func_ret);

fail_alloc:
	return ret;
}

int free_irq_callee(struct fipc_message *_request)
{
	unsigned int irq;
	struct irqhandler_t_container *irqhandler_container = NULL;
	int ret = 0;
	void *data;
	int i;

	irq = fipc_get_reg0(_request);
	data = (void*) fipc_get_reg1(_request);

	LIBLCD_MSG("%s, freeing irq %d | data %p", __func__, irq, data);

	for (i = 0; i < 32; i++) {
		if ((irq_map[i].irq == irq) && (irq_map[i].data == data)) {
			irqhandler_container = irq_map[i].irqhandler_data;
			break;
		}
	}

	if (!irqhandler_container) {
		printk("%s unable to retrieve container data for irq %d",
				__func__, irq);
		goto exit;
	}
	free_irq(irq, irqhandler_container);
	reg_irqs--;
exit:
	return ret;
}
