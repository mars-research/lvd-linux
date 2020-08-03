#include "../shpchp_caller.h"
static struct cptr c;
static struct glue_cspace *c_cspace;
static struct lcd_sync_channel_group *shpchp_group;
int glue_shpchp_init(void)
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

void glue_shpchp_exit(void)
{
	glue_cap_destroy(c_cspace);
	glue_cap_exit();

}

void add_timer(struct timer_list *timer)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			ADD_TIMER);
	ret = vmfunc_wrapper(_request);
	return;

}

int del_timer(struct timer_list *timer)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			DEL_TIMER);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

void init_timer_key(struct timer_list *timer,
		unsigned int flags,
		char *name,
		struct lock_class_key *key)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			INIT_TIMER_KEY);
	fipc_set_reg1(_request,
			flags);
	fipc_set_reg2(_request,
			name);
	ret = vmfunc_wrapper(_request);
	return;

}

int device_create_file(struct device *dev,
		struct device_attribute *attr)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			DEVICE_CREATE_FILE);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

void device_remove_file(struct device *dev,
		struct device_attribute *attr)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			DEVICE_REMOVE_FILE);
	ret = vmfunc_wrapper(_request);
	return;

}

void *ioremap_nocache(unsigned long phys_addr,
		unsigned long size)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			IOREMAP_NOCACHE);
	fipc_set_reg1(_request,
			phys_addr);
	fipc_set_reg2(_request,
			size);
	ret = vmfunc_wrapper(_request);
	return;

}

void iounmap(void *addr)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int sync_ret;
	unsigned 	long addr_mem_sz;
	unsigned 	long addr_offset;
	cptr_t addr_cptr;
	async_msg_set_fn_type(_request,
			IOUNMAP);
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )addr),
		&addr_cptr,
		&addr_mem_sz,
		&addr_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
	ret = vmfunc_wrapper(_request);
	return;

}

int pci_bus_add_devices(struct pci_bus *bus)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_BUS_ADD_DEVICES);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

int acpi_get_hp_hw_control_from_firmware(struct pci_dev *pdev,
		unsigned int flags)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			ACPI_GET_HP_HW_CONTROL_FROM_FIRMWARE);
	fipc_set_reg1(_request,
			flags);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

void pci_assign_unassigned_bridge_resources(struct pci_dev *bridge)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_ASSIGN_UNASSIGNED_BRIDGE_RESOURCES);
	ret = vmfunc_wrapper(_request);
	return;

}

struct resource *pci_bus_resource_n(struct pci_bus *bus,
		int n)
{
	struct resource_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_BUS_RESOURCE_N);
	fipc_set_reg1(_request,
			n);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

struct pci_dev *pci_dev_get(struct pci_dev *dev)
{
	struct pci_dev_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_DEV_GET);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

void pci_dev_put(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_DEV_PUT);
	ret = vmfunc_wrapper(_request);
	return;

}

void pcie_bus_configure_settings(struct pci_bus *bus)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCIE_BUS_CONFIGURE_SETTINGS);
	ret = vmfunc_wrapper(_request);
	return;

}

int pci_enable_device(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_ENABLE_DEVICE);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

struct pci_bus *pci_find_bus(int domain,
		int busnr)
{
	struct pci_bus_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_FIND_BUS);
	fipc_set_reg1(_request,
			domain);
	fipc_set_reg2(_request,
			busnr);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

int pci_find_capability(struct pci_dev *dev,
		int cap)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_FIND_CAPABILITY);
	fipc_set_reg1(_request,
			cap);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

struct pci_dev *pci_get_slot(struct pci_bus *bus,
		unsigned int devfn)
{
	struct pci_dev_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_GET_SLOT);
	fipc_set_reg1(_request,
			devfn);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

int pci_hp_add_bridge(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_HP_ADD_BRIDGE);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

int pci_hp_change_slot_info(struct hotplug_slot *slot,
		struct hotplug_slot_info *info)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_HP_CHANGE_SLOT_INFO);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

int pci_hp_deregister(struct hotplug_slot *slot)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_HP_DEREGISTER);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

int __pci_hp_register(struct hotplug_slot *slot,
		struct pci_bus *bus,
		int devnr,
		char *name,
		struct module *owner,
		char *mod_name)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			__PCI_HP_REGISTER);
	fipc_set_reg1(_request,
			devnr);
	fipc_set_reg2(_request,
			name);
	fipc_set_reg3(_request,
			mod_name);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

void pci_lock_rescan_remove(int a)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_LOCK_RESCAN_REMOVE);
	fipc_set_reg1(_request,
			a);
	ret = vmfunc_wrapper(_request);
	return;

}

int __pci_register_driver(struct pci_driver *drv,
		struct module *owner,
		char *mod_name)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			__PCI_REGISTER_DRIVER);
	fipc_set_reg1(_request,
			mod_name);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

int pci_scan_slot(struct pci_bus *bus,
		int devfn)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_SCAN_SLOT);
	fipc_set_reg1(_request,
			devfn);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

void pci_stop_and_remove_bus_device(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_STOP_AND_REMOVE_BUS_DEVICE);
	ret = vmfunc_wrapper(_request);
	return;

}

void pci_unlock_rescan_remove(int b)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_UNLOCK_RESCAN_REMOVE);
	fipc_set_reg1(_request,
			b);
	ret = vmfunc_wrapper(_request);
	return;

}

void pci_unregister_driver(struct pci_driver *drv)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			PCI_UNREGISTER_DRIVER);
	ret = vmfunc_wrapper(_request);
	return;

}

void __release_region(struct resource *parent,
		unsigned long start,
		unsigned long n)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			__RELEASE_REGION);
	fipc_set_reg1(_request,
			start);
	fipc_set_reg2(_request,
			n);
	ret = vmfunc_wrapper(_request);
	return;

}

struct resource *__request_region(struct resource *parent,
		unsigned long start,
		unsigned long n,
		char *name,
		int flags)
{
	struct resource_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			__REQUEST_REGION);
	fipc_set_reg1(_request,
			start);
	fipc_set_reg2(_request,
			n);
	fipc_set_reg3(_request,
			name);
	fipc_set_reg4(_request,
			flags);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

int request_threaded_irq(unsigned int irq,
		int ( *irq_handler )(void),
		int ( *irq_handler_tnread )(void),
		unsigned long irqflags,
		char *devname,
		void *dev_id)
{
	struct irq_handler_container *irq_handler_container = NULL;
	struct irq_handler_tnread_container *irq_handler_tnread_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int sync_ret;
	unsigned 	long dev_id_mem_sz;
	unsigned 	long dev_id_offset;
	cptr_t dev_id_cptr;
	irq_handler_container = container_of(irq_handler,
		struct irq_handler_container,
		irq_handler);
	ret = glue_cap_insert_irq_handler_type(c_cspace,
		irq_handler_container,
		&irq_handler_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	irq_handler_container->irq_handler = irq_handler;
	irq_handler_tnread_container = container_of(irq_handler_tnread,
		struct irq_handler_tnread_container,
		irq_handler_tnread);
	ret = glue_cap_insert_irq_handler_tnread_type(c_cspace,
		irq_handler_tnread_container,
		&irq_handler_tnread_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	irq_handler_tnread_container->irq_handler_tnread = irq_handler_tnread;
	async_msg_set_fn_type(_request,
			REQUEST_THREADED_IRQ);
	fipc_set_reg1(_request,
			irq);
	fipc_set_reg2(_request,
			irqflags);
	fipc_set_reg3(_request,
			devname);
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )dev_id),
		&dev_id_cptr,
		&dev_id_mem_sz,
		&dev_id_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

void free_irq(unsigned int irq,
		void *dev_id)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int sync_ret;
	unsigned 	long dev_id_mem_sz;
	unsigned 	long dev_id_offset;
	cptr_t dev_id_cptr;
	async_msg_set_fn_type(_request,
			FREE_IRQ);
	fipc_set_reg1(_request,
			irq);
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )dev_id),
		&dev_id_cptr,
		&dev_id_mem_sz,
		&dev_id_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
	ret = vmfunc_wrapper(_request);
	return;

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

int irq_handler_callee(struct fipc_message *_request)
{
	int irq = 0;
	void *dev = NULL;
	int ret = 0;
	int func_ret = 0;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long dev_offset;
	cptr_t dev_cptr;
	gva_t dev_gva;
	irq = fipc_get_reg1(_request);
	sync_ret = lcd_cptr_alloc(&dev_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}
	lcd_set_cr0(dev_cptr);
	sync_ret = lcd_sync_recv(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to recv");
		lcd_exit(-1);
	}
	mem_order = lcd_r0();
	dev_offset = lcd_r1();
	sync_ret = lcd_map_virt(dev_cptr,
		mem_order,
		&dev_gva);
	if (sync_ret) {
		LIBLCD_ERR("failed to map void *dev");
		lcd_exit(-1);
	}
	func_ret = irq_handler_container->irq_handler(irq,
		( void  * )( ( gva_val(dev_gva) ) + ( dev_offset ) ));
	fipc_set_reg1(_response,
			func_ret);
fail_alloc:
	return ret;

}

int irq_handler_tnread_callee(struct fipc_message *_request)
{
	int irq = 0;
	void *dev = NULL;
	int ret = 0;
	int func_ret = 0;
	int sync_ret;
	unsigned 	long mem_order;
	unsigned 	long dev_offset;
	cptr_t dev_cptr;
	gva_t dev_gva;
	irq = fipc_get_reg1(_request);
	sync_ret = lcd_cptr_alloc(&dev_cptr);
	if (sync_ret) {
		LIBLCD_ERR("failed to get cptr");
		lcd_exit(-1);
	}
	lcd_set_cr0(dev_cptr);
	sync_ret = lcd_sync_recv(sync_ep);
	lcd_set_cr0(CAP_CPTR_NULL);
	if (sync_ret) {
		LIBLCD_ERR("failed to recv");
		lcd_exit(-1);
	}
	mem_order = lcd_r0();
	dev_offset = lcd_r1();
	sync_ret = lcd_map_virt(dev_cptr,
		mem_order,
		&dev_gva);
	if (sync_ret) {
		LIBLCD_ERR("failed to map void *dev");
		lcd_exit(-1);
	}
	func_ret = irq_handler_tnread_container->irq_handler_tnread(irq,
		( void  * )( ( gva_val(dev_gva) ) + ( dev_offset ) ));
	fipc_set_reg1(_response,
			func_ret);
fail_alloc:
	return ret;

}

