#include <lcd_config/pre_hook.h>

#include "../coretemp_hwmon_caller.h"

#include <lcd_config/post_hook.h>

static struct glue_cspace *c_cspace;

#define CPTR_HASH_BITS      5
static DEFINE_HASHTABLE(pdev_hash, CPTR_HASH_BITS);
struct cpuinfo_x86* cpudata[nr_cpu_ids] = { 0 };

int glue_coretemp_hwmon_init(void)
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

void glue_coretemp_hwmon_exit(void)
{
	glue_cap_destroy(c_cspace);
	glue_cap_exit();

}

int glue_insert_pdevice(struct platform_device_ptr_container *pdev_c)
{
	BUG_ON(!pdev_c->pdev);

	pdev_c->my_ref = __cptr((unsigned long)pdev_c->pdev);

	hash_add(pdev_hash, &pdev_c->hentry, (unsigned long) pdev_c->pdev);

	return 0;
}

int glue_lookup_pdevice(struct cptr c, struct platform_device_ptr_container **pdev_cout) {
	struct platform_device_ptr_container *pdev_c;

	hash_for_each_possible(pdev_hash, pdev_c,
			hentry, (unsigned long) cptr_val(c)) {
		if (pdev_c->pdev == (struct platform_device*) c.cptr)
			*pdev_cout = pdev_c;
	}
	return 0;
}

void glue_remove_pdevice(struct platform_device_ptr_container *pdev_c)
{
	hash_del(&pdev_c->hentry);
}

const struct x86_cpu_id *x86_match_cpu(const struct x86_cpu_id *match)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	struct x86_cpu_id *func_ret;
	const struct x86_cpu_id *_match = &match[0];

	INIT_FIPC_MSG(_request);

	func_ret = kzalloc(sizeof(struct x86_cpu_id), GFP_KERNEL);
	if (!func_ret) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	async_msg_set_fn_type(_request, X86_MATCH_CPU);
	fipc_set_reg0(_request, _match->vendor);
	fipc_set_reg1(_request, _match->family);
	fipc_set_reg2(_request, _match->model);
	fipc_set_reg3(_request, _match->feature);

	ret = vmfunc_wrapper(_request);
	func_ret->vendor = fipc_get_reg0(_request);
	func_ret->family = fipc_get_reg1(_request);
	func_ret->model = fipc_get_reg2(_request);
	func_ret->feature = fipc_get_reg3(_request);

fail_alloc:
	return func_ret;
}

int probe_callee(struct fipc_message *_request)
{
	struct platform_device_container *pdev_container;
	struct platform_driver_container *pdrv_container;
	int func_ret;
	int ret = 0;

	LIBLCD_MSG("%s called", __func__);

	pdev_container = kzalloc(sizeof(struct platform_device_container),
		GFP_KERNEL);
	if (!pdev_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_insert_platform_device_type(c_cspace, pdev_container,
				&pdev_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	pdev_container->other_ref = __cptr(fipc_get_reg0(_request));

	ret = glue_cap_lookup_platform_driver_type(c_cspace,
			__cptr(fipc_get_reg1(_request)),
			&pdrv_container);

	if (ret) {
		LIBLCD_ERR("lcd lookup");
		goto fail_lookup;
	}

	pdrv_container->other_ref = __cptr(fipc_get_reg2(_request));

	func_ret = pdrv_container->platform_driver.probe(
			&pdev_container->platform_device);

	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, pdev_container->my_ref.cptr);

fail_alloc:
fail_lookup:
fail_insert:
	return ret;
}

int remove_callee(struct fipc_message *_request)
{
	struct platform_device_container *pdev_container;
	struct platform_driver_container *pdrv_container;
	int ret = 0;
	int func_ret = 0;

	ret = glue_cap_lookup_platform_device_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&pdev_container);

	if (ret) {
		LIBLCD_ERR("lcd lookup");
		goto fail_lookup;
	}

	ret = glue_cap_lookup_platform_driver_type(c_cspace,
			__cptr(fipc_get_reg1(_request)),
			&pdrv_container);

	if (ret) {
		LIBLCD_ERR("lcd lookup");
		goto fail_lookup;
	}

	func_ret = pdrv_container->platform_driver.remove(
				&pdev_container->platform_device);
	fipc_set_reg0(_request, func_ret);
fail_lookup:
	return ret;
}

int __platform_driver_register(struct platform_driver *driver,
		struct module *mod)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	struct platform_driver_container *pdrv_container;

	INIT_FIPC_MSG(_request);

	pdrv_container = container_of(driver, struct platform_driver_container,
					platform_driver);

	ret = glue_cap_insert_platform_driver_type(c_cspace, pdrv_container,
				&pdrv_container->my_ref);
	if (ret) {
		LIBLCD_MSG("cap insert failed %d", ret);
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, __PLATFORM_DRIVER_REGISTER);
	fipc_set_reg0(_request, pdrv_container->my_ref.cptr);
	ret = vmfunc_wrapper(_request);
	pdrv_container->other_ref = __cptr(fipc_get_reg0(_request));
	func_ret = fipc_get_reg1(_request);

	return func_ret;
fail_insert:
	return ret;
}

int __register_cpu_notifier(struct notifier_block *nb)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	struct notifier_block_container *nb_container;

	INIT_FIPC_MSG(_request);

	nb_container = container_of(nb, struct notifier_block_container,
					notifier_block);

	ret = glue_cap_insert_notifier_block_type(c_cspace, nb_container,
				&nb_container->my_ref);
	if (ret) {
		LIBLCD_MSG("cap insert failed %d", ret);
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, __REGISTER_CPU_NOTIFIER);
	fipc_set_reg0(_request, nb_container->my_ref.cptr);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	nb_container->other_ref = __cptr(fipc_get_reg1(_request));
	return func_ret;
fail_insert:
	return ret;
}

void platform_driver_unregister(struct platform_driver *driver)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct platform_driver_container *pdrv_container;
	int ret;

	pdrv_container = container_of(driver, struct platform_driver_container,
					platform_driver);

	async_msg_set_fn_type(_request, PLATFORM_DRIVER_UNREGISTER);
	fipc_set_reg0(_request, pdrv_container->my_ref.cptr);
	ret = vmfunc_wrapper(_request);
	return;
}

void __unregister_cpu_notifier(struct notifier_block *nb)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct notifier_block_container *nb_container;
	int ret;

	nb_container = container_of(nb, struct notifier_block_container,
					notifier_block);

	async_msg_set_fn_type(_request, __UNREGISTER_CPU_NOTIFIER);
	fipc_set_reg0(_request, nb_container->other_ref.cptr);
	ret = vmfunc_wrapper(_request);
	return;
}

int platform_device_add(struct platform_device *pdev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	struct platform_device_container *pdev_container;

	pdev_container = container_of(pdev, struct platform_device_container,
					platform_device);

	async_msg_set_fn_type(_request, PLATFORM_DEVICE_ADD);

	fipc_set_reg0(_request, pdev_container->other_ref.cptr);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

struct platform_device *platform_device_alloc(const char *name,
		int id)
{
	struct platform_device_container *func_ret_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct platform_device *func_ret;
	int ret;

	func_ret_container = kzalloc(sizeof(*func_ret_container), GFP_KERNEL);

	if (!func_ret_container) {
		LIBLCD_ERR("kzalloc failed");
		goto fail_alloc;
	}

	func_ret = &func_ret_container->platform_device;

	async_msg_set_fn_type(_request, PLATFORM_DEVICE_ALLOC);
	fipc_set_reg0(_request, func_ret_container->my_ref.cptr);
	memcpy(&_request->regs[1], name, sizeof(unsigned long));
	fipc_set_reg2(_request, id);

	ret = vmfunc_wrapper(_request);
	func_ret_container->other_ref.cptr = fipc_get_reg0(_request);
	return func_ret;
fail_alloc:
	return NULL;
}

void platform_device_put(struct platform_device *pdev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	struct platform_device_container *pdev_container;

	pdev_container = container_of(pdev, struct platform_device_container,
					platform_device);

	async_msg_set_fn_type(_request, PLATFORM_DEVICE_PUT);

	fipc_set_reg0(_request, pdev_container->other_ref.cptr);
	ret = vmfunc_wrapper(_request);
	return;
}

struct pci_dev *pci_get_domain_bus_and_slot(int domain,
		unsigned int bus,
		unsigned int devfn)
{
	struct pci_dev_container *pci_dev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev *pci_dev;
	int ret;
	int func_ret;

	pci_dev_container = kzalloc(sizeof(*pci_dev_container), GFP_KERNEL);

	if (!pci_dev_container) {
		LIBLCD_ERR("kzalloc failed");
		goto fail_alloc;
	}
	pci_dev = &pci_dev_container->pci_dev;

	async_msg_set_fn_type(_request, PCI_GET_DOMAIN_BUS_AND_SLOT);
	fipc_set_reg0(_request, domain);
	fipc_set_reg1(_request, bus);
	fipc_set_reg2(_request, devfn);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	if (func_ret) {
		pci_dev->vendor = fipc_get_reg1(_request);
		pci_dev->device = fipc_get_reg2(_request);
		return pci_dev;
	}
fail_alloc:
	return NULL;
}

struct device *devm_hwmon_device_register_with_groups(struct device *dev,
		const char *name,
		void *drvdata,
		const struct attribute_group **groups)
{
	struct device_container *func_ret_container = NULL;
	struct kobject_container *kobj_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	struct device *func_ret;
	struct platform_device_container *pdev_container;
	struct platform_device *pdev;

	func_ret_container = kzalloc(sizeof(struct device_container), GFP_KERNEL);

	if (!func_ret_container) {
		LIBLCD_ERR("kzalloc func_ret_container");
		goto fail_alloc;
	}

	kobj_container = kzalloc(sizeof(struct kobject_container), GFP_KERNEL);

	if (!kobj_container) {
		LIBLCD_ERR("kzalloc kobj_container");
		goto fail_alloc;
	}

	func_ret = &func_ret_container->device;

	kobj_container->kobj = &func_ret->kobj;

	ret = glue_cap_insert_kobject_type(c_cspace, kobj_container,
			&kobj_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	ret = glue_cap_insert_device_type(c_cspace,
				func_ret_container,
				&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	pdev = container_of(dev, struct platform_device, dev);
	pdev_container = container_of(pdev, struct platform_device_container,
			platform_device);
	async_msg_set_fn_type(_request,
			DEVM_HWMON_DEVICE_REGISTER_WITH_GROUPS);
	fipc_set_reg0(_request, func_ret_container->my_ref.cptr);
	fipc_set_reg1(_request, (u64) name);
	fipc_set_reg2(_request, pdev_container->other_ref.cptr);
	ret = vmfunc_wrapper(_request);
	func_ret_container->other_ref.cptr = fipc_get_reg0(_request);
	kobj_container->other_ref.cptr = fipc_get_reg1(_request);

	return func_ret;
fail_insert:
fail_alloc:
	return NULL;
}

int rdmsr_safe_on_cpu(unsigned int cpu,
		unsigned int msr_no,
		unsigned int *l,
		unsigned int *h)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	async_msg_set_fn_type(_request, RDMSR_SAFE_ON_CPU);

	fipc_set_reg0(_request, cpu);
	fipc_set_reg1(_request, msr_no);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	*l = fipc_get_reg1(_request);
	*h = fipc_get_reg2(_request);

	return func_ret;
}

int rdmsr_on_cpu(unsigned int cpu,
		unsigned int msr_no,
		unsigned int *l,
		unsigned int *h)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	async_msg_set_fn_type(_request, RDMSR_ON_CPU);
	fipc_set_reg0(_request, cpu);
	fipc_set_reg1(_request, msr_no);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	*l = fipc_get_reg1(_request);
	*h = fipc_get_reg2(_request);
	return func_ret;
}

int sysfs_create_group(struct kobject *kobj,
		const struct attribute_group *grp)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	struct device *dev;
	struct device_container *device_container;
	struct attribute **attrs = grp->attrs;
	struct device_attribute *dev_attr;
	struct device_attribute_container *dev_attr_container;
	struct attribute_group_container *attr_grp_ctr;

	dev = container_of(kobj, struct device, kobj);
	device_container = container_of(dev, struct device_container, device);
	attr_grp_ctr = container_of(grp, struct attribute_group_container, attr_group);

	dev_attr = container_of(attrs[0], struct device_attribute, attr);
	dev_attr_container = container_of(dev_attr, struct device_attribute_container,
				dev_attr);

	ret = glue_cap_insert_device_attribute_type(c_cspace, dev_attr_container,
				&dev_attr_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	ret = glue_cap_insert_attribute_group_type(c_cspace, attr_grp_ctr,
				&attr_grp_ctr->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, SYSFS_CREATE_GROUP);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, attrs[0]->mode);
	memcpy(&_request->regs[2], attrs[0]->name, sizeof(unsigned long));
	fipc_set_reg3(_request, dev_attr_container->my_ref.cptr);
	fipc_set_reg4(_request, attr_grp_ctr->my_ref.cptr);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	attr_grp_ctr->other_ref.cptr = fipc_get_reg1(_request);
	return func_ret;
fail_insert:
	return ret;
}

void sysfs_remove_group(struct kobject *kobj,
		const struct attribute_group *grp)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	struct device_container *device_container;
	struct attribute_group_container *attr_grp_ctr;
	struct device *dev;

	dev = container_of(kobj, struct device, kobj);
	device_container = container_of(dev, struct device_container, device);

	attr_grp_ctr = container_of(grp, struct attribute_group_container, attr_group);

	async_msg_set_fn_type(_request, SYSFS_REMOVE_GROUP);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, attr_grp_ctr->other_ref.cptr);
	ret = vmfunc_wrapper(_request);
	return;
}

int notifier_fn_t_callee(struct fipc_message *_request)
{
	struct notifier_block_container *nb_container = NULL;
	struct notifier_block *nb;
	unsigned long action;
	void *hcpu = NULL;
	int ret = 0;
	int func_ret = 0;

	ret = glue_cap_lookup_notifier_block_type(c_cspace,
			__cptr(fipc_get_reg0(_request)),
			&nb_container);

	if (ret) {
		LIBLCD_ERR("lcd lookup");
		goto fail_lookup;
	}

	action = fipc_get_reg1(_request);
	hcpu = (void*)fipc_get_reg2(_request);
	nb = &nb_container->notifier_block;
	func_ret = nb->notifier_call(nb, action, hcpu);
	fipc_set_reg1(_request, func_ret);
fail_lookup:
	return ret;
}

void cpu_maps_update_begin(void)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	INIT_FIPC_MSG(_request);
	async_msg_set_fn_type(_request, CPU_MAPS_UPDATE_BEGIN);
	ret = vmfunc_wrapper(_request);
	return;
}

void cpu_maps_update_done(void)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	INIT_FIPC_MSG(_request);
	async_msg_set_fn_type(_request, CPU_MAPS_UPDATE_DONE);

	ret = vmfunc_wrapper(_request);
	return;
}

void platform_device_unregister(struct platform_device *pdev)
{
	struct platform_device_container *pdev_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	INIT_FIPC_MSG(_request);


	pdev_container = container_of(pdev, struct platform_device_container,
					platform_device);

	async_msg_set_fn_type(_request, PLATFORM_DEVICE_UNREGISTER);
	fipc_set_reg0(_request, pdev_container->other_ref.cptr);
	ret = vmfunc_wrapper(_request);
	return;
}

struct cpuinfo_x86 * __cpu_data(int cpu)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct cpuinfo_x86 *this_cpu;
	int ret;
	int sync_ret;
	unsigned 	long p_mem_sz;
	unsigned 	long p_offset;
	cptr_t p_cptr;

	assert(cpu < nr_cpu_ids);

	this_cpu = cpudata[cpu];

	if (this_cpu)
		goto exit;
	else {
		this_cpu = kzalloc(sizeof(*this_cpu), GFP_KERNEL);
		if (!this_cpu) {
			LIBLCD_ERR("allocation failed");
			goto fail_alloc;
		}

		INIT_FIPC_MSG(_request);

		async_msg_set_fn_type(_request, CPU_DATA);
		fipc_set_reg0(_request, cpu);

		sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long )this_cpu),
					&p_cptr, &p_mem_sz, &p_offset);

		if (sync_ret) {
			LIBLCD_ERR("virt to cptr failed");
			goto fail_cptr;
		}

		fipc_set_reg1(_request, ilog2((p_mem_sz) >> (PAGE_SHIFT)));
		fipc_set_reg2(_request, p_offset);
		fipc_set_reg3(_request, cptr_val(p_cptr));

		ret = vmfunc_wrapper(_request);
#if 0
		this_cpu->x86_model_id = fipc_get_reg0(_request);
		this_cpu->x86_model = fipc_get_reg1(_request);
		this_cpu->x86_mask = fipc_get_reg2(_request);
		this_cpu->x86_capability = fipc_get_reg3(_request);
		this_cpu->microcode = fipc_get_reg4(_request);
		this_cpu->phys_proc_id = fipc_get_reg5(_request);
		this_cpu->cpu_core_id = fipc_get_reg6(_request);
#endif
		cpudata[cpu] = this_cpu;
	}
fail_cptr:
fail_alloc:
exit:
	return this_cpu;
}

int attr_show_callee(struct fipc_message *_request)
{
	struct device_container *dev_container = NULL;
	struct device_attribute_container *dev_attr_container = NULL;
	struct device *dev;
	struct device_attribute *dev_attr;
	int ret;

	union {
		char buf[8];
		unsigned long _buf;
	} buffer;

	ret = glue_cap_lookup_device_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&dev_container);
	if (ret) {
		LIBLCD_ERR("lcd lookup");
		goto fail_lookup;
	}

	dev = &dev_container->device;

	ret = glue_cap_lookup_device_attribute_type(c_cspace,
				__cptr(fipc_get_reg1(_request)),
				&dev_attr_container);
	if (ret) {
		LIBLCD_ERR("lcd lookup");
		goto fail_lookup;
	}

	dev_attr = &dev_attr_container->dev_attr;
	dev_attr_container->dev_attr.show(dev, dev_attr, buffer.buf);

	fipc_set_reg0(_request, buffer._buf);
fail_lookup:
	return ret;
}
