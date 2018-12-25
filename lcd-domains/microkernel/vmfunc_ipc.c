#include <linux/types.h>
#include <linux/pci.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <libfipc.h>
#include <lcd_domains/types.h>
#include <lcd_domains/microkernel.h>
#include <lcd_domains/lcd_iommu.h>
#include <asm/lcd_domains/run.h>
#include <asm/lcd_domains/create.h>
#include <asm/lcd_domains/libvmfunc.h>

struct lcd *lcd = NULL;

/* SYSCALL HANDLERS -------------------------------------------------- */

static int handle_syscall_create_sync_ep(struct fipc_message *msg)
{
	cptr_t slot;
	/*
	 * Get slot where to store new sync endpoint
	 */
	slot = __cptr(msg->regs[0]);
	/*
	 * Create sync ipc endpoint
	 */
	return __lcd_create_sync_endpoint(lcd, slot);
}

static int handle_syscall_create(struct fipc_message *msg)
{
	cptr_t lcd_slot;
	/*
	 * Args
	 */
	lcd_slot = __cptr(msg->regs[0]);
	/*
	 * Do create
	 */
	return __lcd_create(lcd, lcd_slot);
}

static int handle_syscall_cap_grant(struct fipc_message *msg)
{
	cptr_t dest_lcd, src, dest;
	/*
	 * Args
	 */
	dest_lcd = __cptr(msg->regs[0]);
	src = __cptr(msg->regs[1]);
	dest = __cptr(msg->regs[2]);
	/*
	 * Do grant
	 */
	return __lcd_cap_grant(lcd, dest_lcd, src, dest);
}

static int handle_syscall_cap_revoke(struct fipc_message *msg)
{
	cptr_t object;
	/*
	 * Get object cptr
	 */
	object = __cptr(msg->regs[0]);
	/*
	 * Do revoke
	 */
	return cap_revoke(lcd->cspace, object);
}

static int handle_syscall_cap_delete(struct fipc_message *msg)
{
	cptr_t object;
	/*
	 * Get object cptr
	 */
	object = __cptr(msg->regs[0]);
	/*
	 * Delete from lcd's cspace
	 */
	cap_delete(lcd->cspace, object);

	return 0;
}

static int handle_syscall_config_registers(struct fipc_message *msg)
{
	cptr_t dest_lcd;
	gva_t pc, sp;
	gpa_t gva_root, utcb_page;
	/*
	 * Args
	 */
	dest_lcd = __cptr(msg->regs[0]);
	pc = __gva(msg->regs[1]);
	sp = __gva(msg->regs[2]);
	gva_root = __gpa(msg->regs[3]);
	utcb_page = __gpa(msg->regs[4]);
	/*
	 * Do config
	 */
	return __lcd_config(lcd, dest_lcd, pc, sp, gva_root,
			utcb_page);
}

static int handle_syscall_memory_grant_and_map(struct fipc_message *msg)
{
	cptr_t dest_lcd, mo_cptr, dest_slot;
	gpa_t base;
	/*
	 * Args
	 */
	dest_lcd = __cptr(msg->regs[0]);
	mo_cptr = __cptr(msg->regs[1]);
	dest_slot = __cptr(msg->regs[2]);
	base = __gpa(msg->regs[3]);
	/*
	 * Do grant map
	 */
	return __lcd_memory_grant_and_map(lcd, dest_lcd, mo_cptr,
					dest_slot, base);
}

static int handle_syscall_munmap(struct fipc_message *msg)
{
	cptr_t mem_object;
	/*
	 * Get memory object cptr
	 */
	mem_object = __cptr(msg->regs[0]);
	/*
	 * Do unmap
	 */
	__lcd_unmap_memory_object(lcd, mem_object);

	return 0;
}

static int handle_syscall_mmap(struct fipc_message *msg)
{
	cptr_t mem_object;
	gpa_t base;
	/*
	 * Get memory object cptr and the base gpa where the memory
	 * object should be mapped.
	 */
	mem_object = __cptr(msg->regs[0]);
	base = __gpa(msg->regs[1]);
	/*
	 * Do map
	 */
	return __lcd_map_memory_object(lcd, mem_object, base);
}

static int handle_syscall_vmalloc(struct fipc_message *msg)
{
	cptr_t slot;
	unsigned long nr_pages;
	/*
	 * Get slot where to store alloc'd pages and order
	 */
	slot = __cptr(msg->regs[0]);
	nr_pages = msg->regs[1];
	/*
	 * Do vmalloc
	 */

	msg->regs[0] = (unsigned long)__lcd_vmalloc(lcd, slot, nr_pages);

	return 0;
}

static int handle_syscall_pages_alloc(struct fipc_message *msg)
{
	cptr_t slot;
	unsigned int flags;
	unsigned int order;
	/*
	 * Get slot where to store alloc'd pages, flags,
	 * and order
	 */
	slot = __cptr(msg->regs[0]);
	flags = (unsigned int)msg->regs[1];
	order = (unsigned int)msg->regs[2];
	/*
	 * Do page alloc
	 */
	msg->regs[0] = (unsigned long)__lvd_alloc_pages(lcd, slot, flags, order);
	return 0;
}

static int handle_syscall_pages_alloc_exact_node(struct fipc_message *msg)
{
	cptr_t slot;
	int nid;
	unsigned int flags;
	unsigned int order;
	/*
	 * Get slot where to store alloc'd pages, node id, flags,
	 * and order
	 */
	slot = __cptr(msg->regs[0]);
	nid = (int)msg->regs[1];
	flags = (unsigned int)msg->regs[2];
	order = (unsigned int)msg->regs[3];
	/*
	 * Do page alloc
	 */
	msg->regs[0] = (unsigned long)__lvd_alloc_pages_exact_node(lcd, slot, nid, flags, order);
	return 0;
}

static int handle_syscall_sync_reply(struct fipc_message *msg)
{
	/*
	 * No endpoint arg; just do reply
	 */
	return __lcd_reply(lcd);
}

static int handle_syscall_sync_call(struct fipc_message *msg)
{
	cptr_t endpoint;
	/*
	 * Get endpoint
	 */
	endpoint = __cptr(msg->regs[0]);
	/*
	 * Do synchronous ipc call
	 */
	return __lcd_call(lcd, endpoint);
}

static int handle_syscall_sync_recv(struct fipc_message *msg)
{
	cptr_t endpoint;
	/*
	 * Get endpoint
	 */
	endpoint = __cptr(msg->regs[0]);
	/*
	 * Do synchronous ipc recv
	 */
	return __lcd_recv(lcd, endpoint);
}

static int handle_syscall_sync_send(struct fipc_message *msg)
{
	cptr_t endpoint;
	/*
	 * Get endpoint
	 */
	endpoint = __cptr(msg->regs[0]);
	/*
	 * Do synchronous ipc send
	 */
	return __lcd_send(lcd, endpoint);
}

static int handle_syscall_run(struct fipc_message *msg)
{
	cptr_t dest_lcd;
	/*
	 * Args
	 */
	dest_lcd = __cptr(msg->regs[0]);
	/*
	 * Run it
	 */
	return __lcd_run(lcd, dest_lcd);
}

static int handle_syscall_putchar(struct fipc_message *msg)
{
	char c;
	/*
	 * Put char and possibly print on host
	 */
	c = (char)msg->regs[0];
	return __lcd_put_char(lcd, c);
}

static int handle_syscall_dump_stack(struct fipc_message *msg)
{
	lcd_arch_dump_lcd(lcd->lcd_arch);

	return 0;
}

static int handle_syscall_exit(struct fipc_message *msg, int *lcd_ret)
{
	/*
	 * LCD's exit value in arg0
	 */
	*lcd_ret = (int)msg->regs[0];
	return 1; /* signal LCD has exited */
}

static int handle_syscall_irq_disable(struct fipc_message *msg)
{
	lcd_arch_irq_disable(lcd->lcd_arch);
	return 0;
}

static int handle_syscall_irq_enable(struct fipc_message *msg)
{
	lcd_arch_irq_enable(lcd->lcd_arch);
	return 0;
}

static int handle_syscall_iommu_map_page(struct fipc_message *msg)
{
	int ret;
	bool force;
	gpa_t gpa;
	unsigned int order;

	gpa = __gpa(msg->regs[0]);
	order = msg->regs[1];
	force = msg->regs[2];

	ret = lcd_iommu_map_page(lcd, gpa, order, force);
	return ret;
}

static int handle_syscall_iommu_unmap_page(struct fipc_message *msg)
{
	int ret;
	gpa_t gpa;
	int order;

	gpa = __gpa(msg->regs[0]);
	order = msg->regs[1];

	ret = lcd_iommu_unmap_page(lcd, gpa, order);
	return ret;
}

static int handle_syscall_assign_device(struct fipc_message *msg)
{
	int domain, bus, devfn;
	struct pci_dev *dev;
	int ret;

	domain = msg->regs[0];
	bus = msg->regs[1];
	devfn = msg->regs[2];

	dev = pci_get_domain_bus_and_slot(domain, bus, devfn);
	if (dev) {
		LCD_MSG("Device found %x:%x:%x. mapping into iommu domain",
			bus, domain, devfn);
		ret = lcd_iommu_map_domain(lcd, dev);
		LCD_MSG("lcd iommu map returned %d", ret);
	} else {
		LCD_ERR("couldn't get device %x:%x:%x", bus, domain, devfn);
		ret = -ENODEV;
	}
	return ret;
}

static int handle_syscall_deassign_device(struct fipc_message *msg)
{
	int domain, bus, devfn;
	struct pci_dev *dev;
	int ret;

	domain = msg->regs[0];
	bus = msg->regs[1];
	devfn = msg->regs[2];

	dev = pci_get_domain_bus_and_slot(domain, bus, devfn);
	if (dev) {
		LCD_MSG("Device found %x:%x:%x. mapping into iommu domain",
			bus, domain, devfn);
		ret = lcd_iommu_unmap_domain(lcd, dev);
		LCD_MSG("lcd iommu map returned %d", ret);
	} else {
		LCD_ERR("couldn't get device %x:%x:%x", bus, domain, devfn);
		ret = -ENODEV;
	}
	return ret;
}

int handle_vmfunc_syscall(struct fipc_message *msg)
{
	int syscall_id;
	int ret;
	int lcd_ret;
	syscall_id = msg->syscall_nr;

	LCD_DEBUG(LCD_DEBUG_VERB,
		"got syscall %d", syscall_id);

	switch (syscall_id) {
	case LCD_SYSCALL_CAP_DELETE:
		ret = handle_syscall_cap_delete(msg);
		break;
	case LCD_SYSCALL_CAP_REVOKE:
		ret = handle_syscall_cap_revoke(msg);
		break;
	case LCD_SYSCALL_CAP_GRANT:
		ret = handle_syscall_cap_grant(msg);
		break;
	case LCD_SYSCALL_PUTCHAR:
		ret = handle_syscall_putchar(msg);
		break;
	case LCD_SYSCALL_CREATE:
		ret = handle_syscall_create(msg);
		break;
	case LCD_SYSCALL_CONFIG_REGISTERS:
		ret = handle_syscall_config_registers(msg);
		break;
	case LCD_SYSCALL_MEMORY_GRANT_AND_MAP:
		ret = handle_syscall_memory_grant_and_map(msg);
		break;
	case LCD_SYSCALL_RUN:
		ret = handle_syscall_run(msg);
		break;
	case LCD_SYSCALL_EXIT:
		ret = handle_syscall_exit(msg, &lcd_ret);
		break;
	case LCD_SYSCALL_SYNC_SEND:
		ret = handle_syscall_sync_send(msg);
		break;
	case LCD_SYSCALL_SYNC_RECV:
		ret = handle_syscall_sync_recv(msg);
		break;
	case LCD_SYSCALL_SYNC_CALL:
		ret = handle_syscall_sync_call(msg);
		break;
	case LCD_SYSCALL_SYNC_REPLY:
		ret = handle_syscall_sync_reply(msg);
		break;
	case LCD_SYSCALL_CREATE_SYNC_EP:
		ret = handle_syscall_create_sync_ep(msg);
		break;
	case LCD_SYSCALL_ALLOC_PAGES_EXACT_NODE:
		ret = handle_syscall_pages_alloc_exact_node(msg);
		break;
	case LCD_SYSCALL_ALLOC_PAGES:
		ret = handle_syscall_pages_alloc(msg);
		break;
	case LCD_SYSCALL_VMALLOC:
		ret = handle_syscall_vmalloc(msg);
		break;
	case LCD_SYSCALL_MMAP:
		ret = handle_syscall_mmap(msg);
		break;
	case LCD_SYSCALL_MUNMAP:
		ret = handle_syscall_munmap(msg);
		break;
	case LCD_SYSCALL_DUMP_STACK:
		ret = handle_syscall_dump_stack(msg);
		break;
	case LCD_SYSCALL_IRQ_DISABLE:
		ret = handle_syscall_irq_disable(msg);
		break;
	case LCD_SYSCALL_IRQ_ENABLE:
		ret = handle_syscall_irq_enable(msg);
		break;
	case LCD_SYSCALL_ASSIGN_DEVICE:
		ret = handle_syscall_assign_device(msg);
		break;
	case LCD_SYSCALL_DEASSIGN_DEVICE:
		ret = handle_syscall_deassign_device(msg);
		break;
	case LCD_SYSCALL_IOMMU_MAP_PAGE:
		ret = handle_syscall_iommu_map_page(msg);
		break;
	case LCD_SYSCALL_IOMMU_UNMAP_PAGE:
		ret = handle_syscall_iommu_unmap_page(msg);
		break;
	default:
		LCD_ERR("unimplemented syscall %d", syscall_id);
		ret = -ENOSYS;
	}
	/*
	 * Store return value so LCD can get it
	 */
	return ret;
}
