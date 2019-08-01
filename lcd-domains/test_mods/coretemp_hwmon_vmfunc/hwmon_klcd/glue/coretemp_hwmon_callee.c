#include <lcd_config/pre_hook.h>

#include <asm/cacheflush.h>
#include <linux/cpu.h>
#include <linux/hashtable.h>
#include <liblcd/trampoline.h>
#include "../coretemp_hwmon_callee.h"

#include <lcd_config/post_hook.h>

#define DRVNAME	"coretemp"

#define CPTR_HASH_BITS      5
static DEFINE_HASHTABLE(pdev_hash, CPTR_HASH_BITS);
static DEFINE_HASHTABLE(dev_hash, CPTR_HASH_BITS);

static struct glue_cspace *c_cspace;

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

int glue_insert_device(struct device_ptr_container *dev_c)
{
	BUG_ON(!dev_c->dev);

	dev_c->my_ref = __cptr((unsigned long)dev_c->dev);

	hash_add(dev_hash, &dev_c->hentry, (unsigned long) dev_c->dev);

	return 0;
}

int glue_lookup_device(struct cptr c, struct
		device_ptr_container **dev_cout) {
	struct device_ptr_container *dev_c;

	hash_for_each_possible(dev_hash, dev_c,
			hentry, (unsigned long) cptr_val(c)) {
		if (dev_c->dev == (struct device*) c.cptr)
			*dev_cout = dev_c;
	}
	return 0;
}

void glue_remove_device(struct device_ptr_container *dev_c)
{
	hash_del(&dev_c->hentry);
}

int x86_match_cpu_callee(struct fipc_message *_request)
{
	struct x86_cpu_id *match;
	int ret = 0;
	const struct x86_cpu_id *func_ret = NULL;
	struct x86_cpu_id *_match;

	/* Array has one element and the other one is sentinel */
	match = kzalloc(sizeof(struct x86_cpu_id) * 2, GFP_KERNEL);
	if (!match) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	_match = &match[0];

	_match->vendor = fipc_get_reg0(_request);
	_match->family = fipc_get_reg1(_request);
	_match->model = fipc_get_reg2(_request);
	_match->feature = fipc_get_reg3(_request);

	func_ret = x86_match_cpu(match);
	fipc_set_reg0(_request, func_ret->vendor);
	fipc_set_reg1(_request, func_ret->family);
	fipc_set_reg2(_request, func_ret->model);
	fipc_set_reg3(_request, func_ret->feature);

fail_alloc:
	return ret;
}

int cpu_maps_update_begin_callee(struct fipc_message *_request)
{
	int ret = 0;
	cpu_maps_update_begin();
	return ret;
}

int cpu_maps_update_done_callee(struct fipc_message *_request)
{
	int ret = 0;
	cpu_maps_update_done();
	return ret;
}

int probe(struct platform_device *pdev,
		struct trampoline_hidden_args *hidden_args)
{
	struct platform_device_ptr_container *pdev_container;
	int ret = 0;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct platform_driver_container *pdrv_container;

	pdrv_container = hidden_args->struct_container;

	INIT_FIPC_MSG(&r);

	pdev_container = kzalloc(sizeof(struct platform_device_container),
			GFP_KERNEL);
	if (!pdev_container) {
		LIBLCD_ERR("kzalloc dev_container");
		goto fail_alloc;
	}

	pdev_container->pdev = pdev;
	async_msg_set_fn_type(_request, PROBE);

	glue_insert_pdevice(pdev_container);

	fipc_set_reg0(_request, pdev_container->my_ref.cptr);
	fipc_set_reg1(_request, pdrv_container->other_ref.cptr);
	fipc_set_reg2(_request, pdrv_container->my_ref.cptr);

	vmfunc_klcd_wrapper(_request, 1);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
fail_alloc:
	return ret;
}

int remove(struct platform_device *pdev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	async_msg_set_fn_type(_request, REMOVE);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

LCD_TRAMPOLINE_DATA(probe_trampoline);
int LCD_TRAMPOLINE_LINKAGE(probe_trampoline)
probe_trampoline(struct platform_device *pdev)
{
	int ( *volatile probe_fp )(struct platform_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			probe_trampoline);
	probe_fp = probe;
	return probe_fp(pdev, hidden_args);

}

LCD_TRAMPOLINE_DATA(remove_trampoline);
int LCD_TRAMPOLINE_LINKAGE(remove_trampoline)
remove_trampoline(struct platform_device *pdev)
{
	int ( *volatile remove_fp )(struct platform_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			remove_trampoline);
	remove_fp = remove;
	return remove_fp(pdev, hidden_args);
}

int __platform_driver_register_callee(struct fipc_message *_request)
{
	struct platform_driver_container *pdrv_container;
	struct trampoline_hidden_args *drv_probe_hidden_args;
	struct trampoline_hidden_args *drv_remove_hidden_args;
	int ret = 0;
	int func_ret = 0;
	pdrv_container = kzalloc(sizeof(struct platform_driver_container),
			GFP_KERNEL);
	if (!pdrv_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	drv_probe_hidden_args = kzalloc(sizeof( *drv_probe_hidden_args ),
		GFP_KERNEL);
	if (!drv_probe_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc1;
	}
	drv_probe_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(probe_trampoline);
	if (!drv_probe_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	drv_probe_hidden_args->t_handle->hidden_args = drv_probe_hidden_args;
	drv_probe_hidden_args->struct_container = pdrv_container;
	pdrv_container->platform_driver.probe = LCD_HANDLE_TO_TRAMPOLINE(drv_probe_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_probe_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(probe_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	drv_remove_hidden_args = kzalloc(sizeof( *drv_remove_hidden_args ),
		GFP_KERNEL);
	if (!drv_remove_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc2;
	}
	drv_remove_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(remove_trampoline);
	if (!drv_remove_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup2;
	}
	drv_remove_hidden_args->t_handle->hidden_args = drv_remove_hidden_args;
	drv_remove_hidden_args->struct_container = pdrv_container;
	pdrv_container->platform_driver.remove = LCD_HANDLE_TO_TRAMPOLINE(drv_remove_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_remove_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(remove_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	pdrv_container->platform_driver.driver.name = DRVNAME;

	pdrv_container->other_ref = __cptr(fipc_get_reg0(_request));

	ret = glue_cap_insert_platform_driver_type(c_cspace, pdrv_container,
				&pdrv_container->my_ref);
	if (ret) {
		LIBLCD_MSG("cap insert failed %d", ret);
		goto fail_insert;
	}

	func_ret = __platform_driver_register(&pdrv_container->platform_driver,
			THIS_MODULE);

	fipc_set_reg0(_request, pdrv_container->my_ref.cptr);

	fipc_set_reg1(_request, func_ret);

fail_dup1:
fail_dup2:
fail_alloc:
fail_alloc1:
fail_alloc2:
fail_insert:
	return ret;
}

int __register_cpu_notifier_callee(struct fipc_message *_request)
{
	struct notifier_block *nb = NULL;
	int ret = 0;
	int func_ret = 0;
	nb = kzalloc(sizeof( *nb ),
		GFP_KERNEL);
	if (!nb) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = __register_cpu_notifier(nb);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int platform_driver_unregister_callee(struct fipc_message *_request)
{
	struct platform_driver *driver = NULL;
	int ret = 0;
	driver = kzalloc(sizeof( *driver ),
		GFP_KERNEL);
	if (!driver) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	platform_driver_unregister(driver);
fail_alloc:
	return ret;

}

int __unregister_cpu_notifier_callee(struct fipc_message *_request)
{
	struct notifier_block *nb = NULL;
	int ret = 0;
	nb = kzalloc(sizeof( *nb ),
		GFP_KERNEL);
	if (!nb) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	__unregister_cpu_notifier(nb);
fail_alloc:
	return ret;

}

int platform_device_add_callee(struct fipc_message *_request)
{
	struct platform_device *pdev = NULL;
	int ret = 0;
	int func_ret = 0;
	pdev = kzalloc(sizeof( *pdev ),
		GFP_KERNEL);
	if (!pdev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = platform_device_add(pdev);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int platform_device_alloc_callee(struct fipc_message *_request)
{
	char *name = 0;
	int id = 0;
	int ret = 0;
	struct platform_device_container *func_ret_container = NULL;
	struct platform_device *func_ret = NULL;
	name = kzalloc(sizeof( char   ),
		GFP_KERNEL);
	if (!name) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	//name = fipc_get_reg1(_request);
	id = fipc_get_reg2(_request);
	func_ret_container->other_ref.cptr = fipc_get_reg2(_request);
	func_ret = platform_device_alloc(name,
		id);
	ret = glue_cap_insert_platform_device_type(c_cspace,
		func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fipc_set_reg1(_request,
			func_ret_container->other_ref.cptr);
fail_alloc:
fail_insert:
	return ret;

}

int platform_device_put_callee(struct fipc_message *_request)
{
	struct platform_device *pdev = NULL;
	int ret = 0;
	pdev = kzalloc(sizeof( *pdev ),
		GFP_KERNEL);
	if (!pdev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	platform_device_put(pdev);
fail_alloc:
	return ret;

}

int pci_get_domain_bus_and_slot_callee(struct fipc_message *_request)
{
	int domain = 0;
	unsigned 	int bus = 0;
	unsigned 	int devfn = 0;
	int ret = 0;
	struct pci_dev_container *func_ret_container = NULL;
	struct pci_dev *func_ret = NULL;
	domain = fipc_get_reg1(_request);
	bus = fipc_get_reg2(_request);
	devfn = fipc_get_reg3(_request);
	func_ret_container->other_ref.cptr = fipc_get_reg2(_request);
	func_ret = pci_get_domain_bus_and_slot(domain,
		bus,
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
fail_insert:
	return ret;
}

int devm_hwmon_device_register_with_groups_callee(struct fipc_message *_request)
{
	struct platform_device_ptr_container *pdev_container = NULL;
	struct device *dev = NULL;
	char *name = 0;
	const struct attribute_group **groups = NULL;
	int ret = 0;
	struct device_ptr_container *devp_container = NULL;
	struct device *func_ret = NULL;	

	glue_lookup_pdevice(__cptr(fipc_get_reg2(_request)), &pdev_container);

	assert(!pdev_container);

	dev = &pdev_container->pdev->dev;

	devp_container = kzalloc(sizeof(struct device_ptr_container), GFP_KERNEL);

	if (!devp_container) {
		LIBLCD_ERR("kzalloc failed");
		goto fail_alloc;
	}

	devp_container->other_ref.cptr = fipc_get_reg0(_request);

	func_ret = devm_hwmon_device_register_with_groups(dev, name, NULL,
			groups);

	devp_container->dev = func_ret;

	glue_insert_device(devp_container);

	fipc_set_reg0(_request, devp_container->my_ref.cptr);
fail_alloc:
	return ret;
}

int rdmsr_safe_on_cpu_callee(struct fipc_message *_request)
{
	unsigned 	int cpu = 0;
	unsigned 	int msr_no = 0;
	unsigned 	int *l = 0;
	unsigned 	int *h = 0;
	int ret = 0;
	int func_ret = 0;
	l = kzalloc(sizeof( int   ),
		GFP_KERNEL);
	if (!l) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	h = kzalloc(sizeof( int   ),
		GFP_KERNEL);
	if (!h) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	cpu = fipc_get_reg1(_request);
	msr_no = fipc_get_reg2(_request);
	//l = fipc_get_reg3(_request);
	//h = fipc_get_reg4(_request);
	func_ret = rdmsr_safe_on_cpu(cpu,
		msr_no,
		l,
		h);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int rdmsr_on_cpu_callee(struct fipc_message *_request)
{
	unsigned 	int cpu = 0;
	unsigned 	int msr_no = 0;
	unsigned 	int *l = 0;
	unsigned 	int *h = 0;
	int ret = 0;
	int func_ret = 0;
	l = kzalloc(sizeof( int   ),
		GFP_KERNEL);
	if (!l) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	h = kzalloc(sizeof( int   ),
		GFP_KERNEL);
	if (!h) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	cpu = fipc_get_reg1(_request);
	msr_no = fipc_get_reg2(_request);
//	l = fipc_get_reg3(_request);
//	h = fipc_get_reg4(_request);
	func_ret = rdmsr_on_cpu(cpu,
		msr_no,
		l,
		h);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;
}

int sysfs_create_group_callee(struct fipc_message *_request)
{
	struct kobject *kobj = NULL;
	struct attribute_group *grp = NULL;
	int ret = 0;
	int func_ret = 0;
	kobj = kzalloc(sizeof( *kobj ),
		GFP_KERNEL);
	if (!kobj) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	grp = kzalloc(sizeof( *grp ),
		GFP_KERNEL);
	if (!grp) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = sysfs_create_group(kobj,
		grp);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int sysfs_remove_group_callee(struct fipc_message *_request)
{
	struct kobject *kobj = NULL;
	struct attribute_group *grp = NULL;
	int ret = 0;
	kobj = kzalloc(sizeof( *kobj ),
		GFP_KERNEL);
	if (!kobj) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	grp = kzalloc(sizeof( *grp ),
		GFP_KERNEL);
	if (!grp) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	sysfs_remove_group(kobj,
			grp);
fail_alloc:
	return ret;

}

int notifier_call(struct notifier_block *nb,
		unsigned long action,
		void *data,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int sync_ret;
	unsigned 	long data_mem_sz;
	unsigned 	long data_offset;
	cptr_t data_cptr;
	int ret;
	int func_ret;
	async_msg_set_fn_type(_request,
			NOTIFIER_FN_T);
	fipc_set_reg1(_request,
			action);
	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long   )data),
		&data_cptr,
		&data_mem_sz,
		&data_offset);
	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

LCD_TRAMPOLINE_DATA(notifier_call_trampoline);
int LCD_TRAMPOLINE_LINKAGE(notifier_call_trampoline)
notifier_call_trampoline(struct notifier_block *nb,
		unsigned long action,
		void *data)
{
	int ( *volatile notifier_call_fp )(struct notifier_block *,
			unsigned long, void *,
			struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			notifier_call_trampoline);
	notifier_call_fp = notifier_call;
	return notifier_call_fp(nb, action, data, hidden_args);
}
