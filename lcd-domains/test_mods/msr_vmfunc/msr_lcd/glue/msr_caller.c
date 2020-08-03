#include <lcd_config/pre_hook.h>
#include "../msr_caller.h"
#include <lcd_config/post_hook.h>

static struct cptr c;
static struct glue_cspace *c_cspace;
static struct lcd_sync_channel_group *msr_group;

int glue_msr_init(void)
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

void glue_msr_exit(void)
{
	glue_cap_destroy(c_cspace);
	glue_cap_exit();

}

int glue_insert_file(struct file_container *file_c)
{
	BUG_ON(!file_c->file);

	file_c->my_ref = __cptr((unsigned long)file_c->file);

	hash_add(file_hash, &file_c->hentry, (unsigned long) file_c->file);

	return 0;
}

int glue_lookup_file(struct cptr c, struct
		file_container **file_cout) {
	struct file_container *file_c;
 
	hash_for_each_possible(file_hash, file_c,
			hentry, (unsigned long) cptr_val(c)) {
		if (file_c->file == (struct file*) c.cptr)
			*file_cout = file_c;
	}
	return 0;
}

void glue_remove_file(struct file_container *file_c)
{
	hash_del(&file_c->hentry);
}



struct class *__class_create(struct module *owner, const char *name,
		struct lock_class_key *key)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct class_container *func_ret_container;
	struct class *func_ret;

	func_ret_container = kzalloc(sizeof(struct class_container),
			GFP_KERNEL);

	if (!func_ret_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_insert_class_type(c_cspace, func_ret_container,
			&func_ret_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, __CLASS_CREATE);
	fipc_set_reg0(_request, func_ret_container->my_ref.cptr);

	ret = vmfunc_wrapper(_request);

	func_ret_container->other_ref.cptr = fipc_get_reg0(_request);
	func_ret = &func_ret_container->class;

	return func_ret;
fail_alloc:
fail_insert:
	return NULL;
}

void class_destroy(struct class *cls)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct class_container *class_container;

	class_container = container_of(cls, struct class_container, class);

	async_msg_set_fn_type(_request, CLASS_DESTROY);
	fipc_set_reg0(_request, class_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	return;
}

bool capable(int cap)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;

	async_msg_set_fn_type(_request,
			CAPABLE);

	fipc_set_reg0(_request,	cap);
	ret = vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);
	return func_ret;

}

struct device *device_create(struct class *class,
		struct device *parent,
		dev_t devt,
		void *drvdata,
		const char *fmt,
		...)
{
	int ret;
	struct device_container *func_ret_container = NULL;
	struct device *func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct class_container *class_container;
	va_list valist;
	int instance;
	struct device_container *device_container = NULL;

	class_container = container_of(class, struct class_container, class);

	func_ret_container = kzalloc(sizeof( struct device_container   ),
		GFP_KERNEL);
	if (!func_ret_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	glue_lookup_device(__cptr((unsigned long) parent), &device_container);

	func_ret = &func_ret_container->device;
	func_ret_container->dev = &func_ret_container->device;

	glue_insert_device(func_ret_container);

	va_start(valist, fmt);
	instance = va_arg(valist, int);
	va_end(valist);

	async_msg_set_fn_type(_request, DEVICE_CREATE);
	fipc_set_reg0(_request, class_container->other_ref.cptr);
	fipc_set_reg1(_request, device_container->other_ref.cptr);
	fipc_set_reg2(_request, devt);
	fipc_set_reg3(_request, func_ret_container->my_ref.cptr);
	fipc_set_reg4(_request, instance);

	ret = vmfunc_wrapper(_request);

	func_ret_container->other_ref.cptr = fipc_get_reg0(_request);

	return func_ret;
fail_alloc:
	return NULL;
}

void device_destroy(struct class *class,
		dev_t devt)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct class_container *class_container;

	class_container = container_of(class, struct class_container, class);
	async_msg_set_fn_type(_request, DEVICE_DESTROY);
	fipc_set_reg0(_request, class_container->other_ref.cptr);
	fipc_set_reg1(_request, devt);
	ret = vmfunc_wrapper(_request);
	return;
}

long long no_seek_end_llseek(struct file *file,
		long long offset,
		int whence)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			NO_SEEK_END_LLSEEK);
	fipc_set_reg1(_request,
			offset);
	fipc_set_reg2(_request,
			whence);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

void cpu_maps_update_begin(void)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			CPU_MAPS_UPDATE_BEGIN);
	ret = vmfunc_wrapper(_request);
	return;

}

void cpu_maps_update_done()
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			CPU_MAPS_UPDATE_DONE);
	ret = vmfunc_wrapper(_request);
	return;
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

int wrmsr_safe_on_cpu(unsigned int cpu,
		unsigned int msr_no,
		unsigned int l,
		unsigned int h)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret; 
	int func_ret;

	async_msg_set_fn_type(_request,
			WRMSR_SAFE_ON_CPU);
	
	fipc_set_reg0(_request, cpu);
	fipc_set_reg1(_request,	msr_no);
	fipc_set_reg2(_request, l);
	fipc_set_reg3(_request, h);
	ret = vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);
	return func_ret;

}


int rdmsr_safe_regs_on_cpu(unsigned int cpu,
		unsigned int *regs)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	int len = 8;
	int i;

	async_msg_set_fn_type(_request, RDMSR_SAFE_ON_CPU);

	fipc_set_reg0(_request, cpu);
	for (i = 0; i < len; i ++)
		fipc_set_reg(i + 1, _request);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	for (i = 0; i < len; i ++)
		regs[i] = fipc_get_reg(i + 1, _request);

	return func_ret;

}

int wrmsr_safe_regs_on_cpu(unsigned int cpu,
		unsigned int *regs)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	int len = 8;
	int i;

	async_msg_set_fn_type(_request,
			WRMSR_SAFE_REGS_ON_CPU);

	fipc_set_reg0(_request, cpu);
	for (i = 0; i < len; i ++)
		fipc_set_reg(i + 1, _request);


	ret = vmfunc_wrapper(_request);
	
	func_ret = fipc_get_reg0(_request);
	for (i = 0; i < len; i ++)
		regs[i] = fipc_get_reg(i + 1, _request);


	return func_ret;

}

void __unregister_chrdev(unsigned int major, unsigned int baseminor, unsigned
		int count, const char *name)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request, __UNREGISTER_CHRDEV);
	fipc_set_reg0(_request, major);
	fipc_set_reg1(_request, baseminor);
	fipc_set_reg2(_request, count);
	ret = vmfunc_wrapper(_request);
	return;
}

int __register_chrdev(unsigned int major,
		unsigned int baseminor,
		unsigned int count,
		const char *name,
		const struct file_operations *fops)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct file_operations_container *fops_container;
	int func_ret;
	int ret;

	fops_container = container_of(fops, struct file_operations_container, fops);

	ret = glue_cap_insert_file_operations_type(c_cspace, fops_container,
				&fops_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, __REGISTER_CHRDEV);
	fipc_set_reg0(_request, fops_container->my_ref.cptr);
	fipc_set_reg1(_request, major);
	fipc_set_reg2(_request, baseminor);
	fipc_set_reg3(_request, count);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	fops_container->other_ref.cptr = fipc_get_reg0(_request);
	return func_ret;
fail_insert:
	ret;
}

int llseek_callee(struct fipc_message *_request)
{
	struct file *file = NULL;
	long offset = 0;
	int whence = 0;
	int ret = 0;
	long func_ret = 0;
	
	ret = glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}


	
	offset = fipc_get_reg1(_request);
	whence = fipc_get_reg2(_request);
	func_ret = fops->llseek(file,
		offset,
		whence);
	fipc_set_reg1(_response,
			func_ret);
fail_alloc:
	return ret;

}

int read_callee(struct fipc_message *_request)
{
	struct file *file = NULL;
	char buf = 0;
	unsigned 	int len = 0;
	long ptr = 0;
	int ret = 0;
	unsigned 	int func_ret = 0;
	file = kzalloc(sizeof( *file ),
		GFP_KERNEL);
	if (!file) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	buf = fipc_get_reg1(_request);
	len = fipc_get_reg2(_request);
	ptr = fipc_get_reg3(_request);
	func_ret = fops->read(file,
		buf,
		len,
		ptr);
	fipc_set_reg1(_response,
			func_ret);
fail_alloc:
	return ret;

}

int write_callee(struct fipc_message *_request)
{
	struct file *file = NULL;
	char buf = 0;
	unsigned 	int len = 0;
	long ptr = 0;
	int ret = 0;
	unsigned 	int func_ret = 0;
	file = kzalloc(sizeof( *file ),
		GFP_KERNEL);
	if (!file) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	buf = fipc_get_reg1(_request);
	len = fipc_get_reg2(_request);
	ptr = fipc_get_reg3(_request);
	func_ret = fops->write(file,
		buf,
		len,
		ptr);
	fipc_set_reg1(_response,
			func_ret);
fail_alloc:
	return ret;

}

int unlocked_ioctl_callee(struct fipc_message *_request)
{
	struct file *file = NULL;
	unsigned 	int ioc = 0;
	unsigned 	long arg = 0;
	int ret = 0;
	long func_ret = 0;
	file = kzalloc(sizeof( *file ),
		GFP_KERNEL);
	if (!file) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	ioc = fipc_get_reg1(_request);
	arg = fipc_get_reg2(_request);
	func_ret = fops->unlocked_ioctl(file,
		ioc,
		arg);
	fipc_set_reg1(_response,
			func_ret);
fail_alloc:
	return ret;

}

int compat_ioctl_callee(struct fipc_message *_request)
{
	struct file *file = NULL;
	unsigned 	int ioc = 0;
	unsigned 	long arg = 0;
	int ret = 0;
	long func_ret = 0;
	file = kzalloc(sizeof( *file ),
		GFP_KERNEL);
	if (!file) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	ioc = fipc_get_reg1(_request);
	arg = fipc_get_reg2(_request);
	func_ret = fops->compat_ioctl(file,
		ioc,
		arg);
	fipc_set_reg1(_response,
			func_ret);
fail_alloc:
	return ret;

}

int open_callee(struct fipc_message *_request)
{
	struct inode inode;
	struct file *file = NULL;
	int ret = 0;
	int func_ret = 0;

	file_container = kzalloc(sizeof(*file_container), GFP_KERNEL);
	if (!file_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	file = kzalloc(sizeof( *file ),	GFP_KERNEL);
	if (!file) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	file_container->file = file;
	file_container->other_ref.cptr = fipc_get_reg0(_request);

	glue_insert_file(file_container);

	func_ret = fops->open(&inode, file);
	fipc_set_reg0(_response, func_ret);
	fipc_set_reg1(_response, file_container->my_ref.cptr);

fail_alloc:
	return ret;

}

