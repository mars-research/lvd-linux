#include <lcd_config/pre_hook.h>

#include <linux/cpu.h>
#include "../msr_callee.h"
#include <liblcd/trampoline.h>

#include <lcd_config/post_hook.h>

static struct cptr c;
static struct glue_cspace *c_cspace;
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


int __class_create_callee(struct fipc_message *_request)
{
	struct class_container *cls_container = NULL;
	struct lock_class_key key;
	int ret = 0;
	struct class *class;

	cls_container = kzalloc(sizeof( *cls_container ), GFP_KERNEL);

	if (!cls_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	class = __class_create(THIS_MODULE, DRVNAME, &key);

	cls_container->class_p = class;
	cls_container->other_ref.cptr = fipc_get_reg0(_request);

	ret = glue_cap_insert_class_type(c_cspace, cls_container,
				&cls_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	fipc_set_reg0(_request, cls_container->my_ref.cptr);
fail_alloc:
fail_insert:
	return ret;
}


int capable_callee(struct fipc_message *_request)
{
	int cap = 0;
	int ret = 0;
	bool func_ret = 0;
	cap = fipc_get_reg0(_request);
	func_ret = capable(cap);
	fipc_set_reg0(_request,	func_ret);
	return ret;
}

int class_destroy_callee(struct fipc_message *_request)
{
	struct class_container *cls_container = NULL;
	int ret = 0;

	ret = glue_cap_lookup_class_type(c_cspace, __cptr(fipc_get_reg0(_request)),
				&cls_container);
	if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	class_destroy(cls_container->class_p);

fail_lookup:
	return ret;
}

int device_create_callee(struct fipc_message *_request)
{
	struct device *parent = NULL;
	unsigned long devt = 0;
	void *drvdata = NULL;
	struct device_container *func_ret_container = NULL;
	struct device *func_ret = NULL;
	struct class_container *cls_container = NULL;
	struct device_container *device_container = NULL;
	int ret = 0;
	struct class *class;
	int instance;

	ret = glue_cap_lookup_class_type(c_cspace, __cptr(fipc_get_reg0(_request)),
				&cls_container);
	if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	class = cls_container->class_p;
	func_ret_container = kzalloc(sizeof( struct device_container   ),
		GFP_KERNEL);
	if (!func_ret_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	func_ret_container->other_ref.cptr = fipc_get_reg3(_request);

	glue_lookup_device(__cptr(fipc_get_reg1(_request)), &device_container);

	parent = device_container->dev;

	instance = fipc_get_reg4(_request);

	func_ret = device_create(class, parent, devt, drvdata, "nvme_lcd%d", instance);
	func_ret_container->dev = func_ret;

	glue_insert_device(func_ret_container);

	fipc_set_reg0(_request, func_ret_container->my_ref.cptr);

fail_lookup:
fail_alloc:
	return ret;
}

int device_destroy_callee(struct fipc_message *_request)
{
	struct class *class = NULL;
	struct class_container *cls_container;
	unsigned long devt = 0;
	int ret = 0;

	ret = glue_cap_lookup_class_type(c_cspace, __cptr(fipc_get_reg0(_request)),
				&cls_container);
	if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	class = cls_container->class_p;

	devt = fipc_get_reg1(_request);

	device_destroy(class, devt);

fail_lookup:
	return ret;
}

/* AB: Check */
int no_seek_end_llseek_callee(struct fipc_message *_request)
{
	struct file *file = NULL;
	long 	long offset = 0;
	int whence = 0;
	int ret = 0;
	long 	long func_ret = 0;
	file = kzalloc(sizeof( *file ),
		GFP_KERNEL);
	if (!file) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	offset = fipc_get_reg1(_request);
	whence = fipc_get_reg2(_request);
	func_ret = no_seek_end_llseek(file,
		offset,
		whence);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int cpu_maps_update_begin_callee(struct fipc_message *_request)
{
	cpu_maps_update_begin();
	return 0;
}

int cpu_maps_update_done_callee(struct fipc_message *_request)
{
	cpu_maps_update_done();
	return 0;
}

int rdmsr_safe_on_cpu_callee(struct fipc_message *_request)
{
	unsigned int cpu;
	u32 msr_no;
	u32 l;
	u32 h;
	int ret = 0;
	int func_ret = 0;

	cpu = fipc_get_reg0(_request);
	msr_no = fipc_get_reg1(_request);
	func_ret = rdmsr_safe_on_cpu(cpu, msr_no, &l, &h);
	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, l);
	fipc_set_reg2(_request, h);
	return ret;
}

int rdmsr_safe_regs_on_cpu_callee(struct fipc_message *_request)
{
	unsigned int cpu = 0;
	u32 regs[8];
	int len = 8;
	int ret = 0;
	int func_ret = 0;
	int i;
	
	cpu = fipc_get_reg0(_request);

	for (i = 0; i < len; i ++)
		regs[i] = fipc_get_reg(i + 1, _request);

	func_ret = rdmsr_safe_regs_on_cpu(cpu, regs);

	fipc_set_reg0(_request, func_ret);

	for (i = 0; i < len; i ++)
		fipc_set_reg(i + 1, _request);

	return ret;

}

int wrmsr_safe_on_cpu_callee(struct fipc_message *_request)
{
	unsigned 	int cpu = 0;
	u32 msr_no = 0;
	u32 l = 0;
	u32 h = 0;
	int ret = 0;

	cpu = fipc_get_reg0(_request);
	msr_no = fipc_get_reg1(_request);
	l = fipc_get_reg2(_request);
	h = fipc_get_reg3(_request);

	wrmsr_safe_on_cpu(cpu, msr_no, l, h);
	
	return ret;

}

int wrmsr_safe_regs_on_cpu_callee(struct fipc_message *_request)
{
	unsigned int cpu = 0;
	u32 regs[8];
	int len = 8;
	int ret = 0;
	int func_ret = 0;
	int i; 
	
	cpu = fipc_get_reg0(_request);

	for (i = 0; i < len; i ++)
		regs[i] = fipc_get_reg(i + 1, _request);

	func_ret = wrmsr_safe_regs_on_cpu(cpu, regs);
	fipc_set_reg0(_request, func_ret);
	
	for (i = 0; i < len; i ++)
		fipc_set_reg(i + 1, _request);

	return ret;

}


int __register_chrdev_callee(struct fipc_message *_request)
{
	unsigned 	int major = 0;
	unsigned 	int baseminor = 0;
	unsigned 	int count = 0;
	char name = 0;
	struct file_operations_container *fops_container = NULL;
	struct file_operations *fops = NULL;
	int ret = 0;
	struct trampoline_hidden_args *fops_llseek_hidden_args;
	struct trampoline_hidden_args *fops_read_hidden_args;
	struct trampoline_hidden_args *fops_write_hidden_args;
	struct trampoline_hidden_args *fops_unlocked_ioctl_hidden_args;
	struct trampoline_hidden_args *fops_compat_ioctl_hidden_args;
	struct trampoline_hidden_args *fops_open_hidden_args;
	int func_ret = 0;
	fops_container = kzalloc(sizeof( struct file_operations_container   ),
		GFP_KERNEL);
	if (!fops_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	ret = glue_cap_insert_file_operations_type(c_cspace,
		fops_container,
		&fops_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	fops_container->other_ref.cptr = fipc_get_reg7(_request);
	fops_llseek_hidden_args = kzalloc(sizeof( *llseek_hidden_args ),
		GFP_KERNEL);
	if (!fops_llseek_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc1;
	}
	fops_llseek_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(llseek_trampoline);
	if (!fops_llseek_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	fops_llseek_hidden_args->t_handle->hidden_args = fops_llseek_hidden_args;
	fops_llseek_hidden_args->struct_container = fops_container;
	fops_llseek_hidden_args->cspace = c_cspace;
	fops_llseek_hidden_args->sync_ep = sync_ep;
	fops_llseek_hidden_args->async_chnl = _channel;
	fops_container->file_operations.llseek = LCD_HANDLE_TO_TRAMPOLINE(fops_llseek_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )fops_llseek_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(llseek_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	fops_read_hidden_args = kzalloc(sizeof( *read_hidden_args ),
		GFP_KERNEL);
	if (!fops_read_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc2;
	}
	fops_read_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(read_trampoline);
	if (!fops_read_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup2;
	}
	fops_read_hidden_args->t_handle->hidden_args = fops_read_hidden_args;
	fops_read_hidden_args->struct_container = fops_container;
	fops_read_hidden_args->cspace = c_cspace;
	fops_read_hidden_args->sync_ep = sync_ep;
	fops_read_hidden_args->async_chnl = _channel;
	fops_container->file_operations.read = LCD_HANDLE_TO_TRAMPOLINE(fops_read_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )fops_read_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(read_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	fops_write_hidden_args = kzalloc(sizeof( *write_hidden_args ),
		GFP_KERNEL);
	if (!fops_write_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc3;
	}
	fops_write_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(write_trampoline);
	if (!fops_write_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup3;
	}
	fops_write_hidden_args->t_handle->hidden_args = fops_write_hidden_args;
	fops_write_hidden_args->struct_container = fops_container;
	fops_write_hidden_args->cspace = c_cspace;
	fops_write_hidden_args->sync_ep = sync_ep;
	fops_write_hidden_args->async_chnl = _channel;
	fops_container->file_operations.write = LCD_HANDLE_TO_TRAMPOLINE(fops_write_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )fops_write_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(write_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	fops_unlocked_ioctl_hidden_args = kzalloc(sizeof( *unlocked_ioctl_hidden_args ),
		GFP_KERNEL);
	if (!fops_unlocked_ioctl_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc4;
	}
	fops_unlocked_ioctl_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(unlocked_ioctl_trampoline);
	if (!fops_unlocked_ioctl_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup4;
	}
	fops_unlocked_ioctl_hidden_args->t_handle->hidden_args = fops_unlocked_ioctl_hidden_args;
	fops_unlocked_ioctl_hidden_args->struct_container = fops_container;
	fops_unlocked_ioctl_hidden_args->cspace = c_cspace;
	fops_unlocked_ioctl_hidden_args->sync_ep = sync_ep;
	fops_unlocked_ioctl_hidden_args->async_chnl = _channel;
	fops_container->file_operations.unlocked_ioctl = LCD_HANDLE_TO_TRAMPOLINE(fops_unlocked_ioctl_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )fops_unlocked_ioctl_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(unlocked_ioctl_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	fops_compat_ioctl_hidden_args = kzalloc(sizeof( *compat_ioctl_hidden_args ),
		GFP_KERNEL);
	if (!fops_compat_ioctl_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc5;
	}
	fops_compat_ioctl_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(compat_ioctl_trampoline);
	if (!fops_compat_ioctl_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup5;
	}
	fops_compat_ioctl_hidden_args->t_handle->hidden_args = fops_compat_ioctl_hidden_args;
	fops_compat_ioctl_hidden_args->struct_container = fops_container;
	fops_compat_ioctl_hidden_args->cspace = c_cspace;
	fops_compat_ioctl_hidden_args->sync_ep = sync_ep;
	fops_compat_ioctl_hidden_args->async_chnl = _channel;
	fops_container->file_operations.compat_ioctl = LCD_HANDLE_TO_TRAMPOLINE(fops_compat_ioctl_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )fops_compat_ioctl_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(compat_ioctl_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	fops_open_hidden_args = kzalloc(sizeof( *open_hidden_args ),
		GFP_KERNEL);
	if (!fops_open_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc6;
	}
	fops_open_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(open_trampoline);
	if (!fops_open_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup6;
	}
	fops_open_hidden_args->t_handle->hidden_args = fops_open_hidden_args;
	fops_open_hidden_args->struct_container = fops_container;
	fops_open_hidden_args->cspace = c_cspace;
	fops_open_hidden_args->sync_ep = sync_ep;
	fops_open_hidden_args->async_chnl = _channel;
	fops_container->file_operations.open = LCD_HANDLE_TO_TRAMPOLINE(fops_open_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )fops_open_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(open_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	major = fipc_get_reg1(_request);
	baseminor = fipc_get_reg2(_request);
	count = fipc_get_reg3(_request);
	name = fipc_get_reg4(_request);
	func_ret = __register_chrdev(major,
		baseminor,
		count,
		name,
		( &fops_container->file_operations ));
	fipc_set_reg6(_response,
			fops_container->other_ref.cptr);
	fipc_set_reg2(_response,
			func_ret);
fail_alloc:
	return ret;

}

int __unregister_chrdev_callee(struct fipc_message *_request)
{
	unsigned 	int major = 0;
	unsigned 	int baseminor = 0;
	unsigned 	int count = 0;
	int ret = 0;

	major = fipc_get_reg0(_request);
	baseminor = fipc_get_reg1(_request);
	count = fipc_get_reg2(_request);

	__unregister_chrdev(major, baseminor, count, CHRDEV_NAME);

	return ret;
}

long llseek(struct file *file,
		long offset,
		int whence,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct file_container *file_container = NULL;
	int func_ret = 0;
	int ret = 0;

	ret = glue_lookup_file(__cptr((uint64_t)file), &file_container);
	ret = glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}


	async_msg_set_fn_type(_request, LLSEEK);
	
	fipc_set_reg0(_request, file_container->other_ref.cptr);
	fipc_set_reg1(_request, offset);
	fipc_set_reg2(_request, whence);
	
	ret = vmfunc_wrapper(_request);
	
	func_ret = fipc_get_reg0(_request);
	return func_ret;

}

LCD_TRAMPOLINE_DATA(llseek_trampoline);
long  LCD_TRAMPOLINE_LINKAGE(llseek_trampoline)
llseek_trampoline(struct file *file,
		long offset,
		int whence)
{
	long ( *volatile llseek_fp )(struct file *,
		long ,
		int ,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			llseek_trampoline);
	llseek_fp = llseek;
	return llseek_fp(file,
		offset,
		whence,
		hidden_args);

}

unsigned int read(struct file *file,
		char buf,
		unsigned int len,
		long ptr,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			READ);
	fipc_set_reg1(_request,
			buf);
	fipc_set_reg2(_request,
			len);
	fipc_set_reg3(_request,
			ptr);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

LCD_TRAMPOLINE_DATA(read_trampoline);
unsigned int  LCD_TRAMPOLINE_LINKAGE(read_trampoline)
read_trampoline(struct file *file,
		char buf,
		unsigned int len,
		long ptr)
{
	unsigned 	int ( *volatile read_fp )(struct file *,
		char ,
		unsigned int ,
		long ,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			read_trampoline);
	read_fp = read;
	return read_fp(file,
		buf,
		len,
		ptr,
		hidden_args);

}

unsigned int write(struct file *file,
		char buf,
		unsigned int len,
		long ptr,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			WRITE);
	fipc_set_reg1(_request,
			buf);
	fipc_set_reg2(_request,
			len);
	fipc_set_reg3(_request,
			ptr);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

LCD_TRAMPOLINE_DATA(write_trampoline);
unsigned int  LCD_TRAMPOLINE_LINKAGE(write_trampoline)
write_trampoline(struct file *file,
		char buf,
		unsigned int len,
		long ptr)
{
	unsigned 	int ( *volatile write_fp )(struct file *,
		char ,
		unsigned int ,
		long ,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			write_trampoline);
	write_fp = write;
	return write_fp(file,
		buf,
		len,
		ptr,
		hidden_args);

}

long unlocked_ioctl(struct file *file,
		unsigned int ioc,
		unsigned long arg,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			UNLOCKED_IOCTL);
	fipc_set_reg1(_request,
			ioc);
	fipc_set_reg2(_request,
			arg);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

LCD_TRAMPOLINE_DATA(unlocked_ioctl_trampoline);
long  LCD_TRAMPOLINE_LINKAGE(unlocked_ioctl_trampoline)
unlocked_ioctl_trampoline(struct file *file,
		unsigned int ioc,
		unsigned long arg)
{
	long ( *volatile unlocked_ioctl_fp )(struct file *,
		unsigned int ,
		unsigned long ,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			unlocked_ioctl_trampoline);
	unlocked_ioctl_fp = unlocked_ioctl;
	return unlocked_ioctl_fp(file,
		ioc,
		arg,
		hidden_args);

}

long compat_ioctl(struct file *file,
		unsigned int ioc,
		unsigned long arg,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			COMPAT_IOCTL);
	fipc_set_reg1(_request,
			ioc);
	fipc_set_reg2(_request,
			arg);
	ret = vmfunc_wrapper(_request);
	return func_ret;

}

LCD_TRAMPOLINE_DATA(compat_ioctl_trampoline);
long  LCD_TRAMPOLINE_LINKAGE(compat_ioctl_trampoline)
compat_ioctl_trampoline(struct file *file,
		unsigned int ioc,
		unsigned long arg)
{
	long ( *volatile compat_ioctl_fp )(struct file *,
		unsigned int ,
		unsigned long ,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			compat_ioctl_trampoline);
	compat_ioctl_fp = compat_ioctl;
	return compat_ioctl_fp(file,
		ioc,
		arg,
		hidden_args);

}


int open(struct inode *inode,
		struct file *file,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct file_container *file_container;
	int ret = 0; 
	int func_ret = 0;

	file_container = kzalloc(sizeof(*file_container), GFP_KERNEL);
	if (!file_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	file_container->file = file;

	glue_insert_file(file_container);

	async_msg_set_fn_type(_request,
			OPEN);

	fipc_set_reg0(_request, file_container->my_ref.cptr);
	ret = vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);
	file_container->other_ref.cptr = fipc_get_reg1(_request);

	return func_ret;

}

LCD_TRAMPOLINE_DATA(open_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(open_trampoline)
open_trampoline(struct inode *inode,
		struct file *file)
{
	int ( *volatile open_fp )(struct inode *,
		struct file *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			open_trampoline);
	open_fp = open;
	return open_fp(inode,
		file,
		hidden_args);

}

int __class_create_callee(struct fipc_message *_request)
{
	struct class_container *cls_container = NULL;
	struct lock_class_key key;
	int ret = 0;
	struct class *class;

	cls_container = kzalloc(sizeof( *cls_container ), GFP_KERNEL);

	if (!cls_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	class = __class_create(THIS_MODULE, DRVNAME, &key);

	cls_container->class_p = class;
	cls_container->other_ref.cptr = fipc_get_reg0(_request);

	ret = glue_cap_insert_class_type(c_cspace, cls_container,
				&cls_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	fipc_set_reg0(_request, cls_container->my_ref.cptr);
fail_alloc:
fail_insert:
	return ret;
}

int class_destroy_callee(struct fipc_message *_request)
{
	struct class_container *cls_container = NULL;
	int ret = 0;

	ret = glue_cap_lookup_class_type(c_cspace, __cptr(fipc_get_reg0(_request)),
				&cls_container);
	if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	class_destroy(cls_container->class_p);

fail_lookup:
	return ret;
}

