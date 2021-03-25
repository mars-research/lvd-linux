#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

long trmp_impl_read(fptr_read target, struct file* file, char* buf, unsigned long count, long long* ppos)
{
	struct glue_message *msg = glue_init_msg();

	struct file** file_ptr = &file;
	char** buf_ptr = &buf;
	unsigned long* count_ptr = &count;
	long long** ppos_ptr = &ppos;
	long ret = 0;
	long* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(msg, target);
	glue_pack_shadow(msg, *file_ptr);
	if (*file_ptr) {
		caller_marshal_kernel__read__file__in(msg, *file_ptr);
	}

	glue_pack_shadow(msg, *buf_ptr);
	if (*buf_ptr) {
		glue_pack(msg, **buf_ptr);
	}

	glue_pack(msg, *count_ptr);
	glue_pack_shadow(msg, *ppos_ptr);
	if (*ppos_ptr) {
		glue_pack(msg, **ppos_ptr);
	}

	glue_call_client(msg, RPC_ID_read);

	if (*file_ptr) {
		caller_unmarshal_kernel__read__file__in(msg, *file_ptr);
	}

	(void)buf_ptr;
	(void)ppos_ptr;
	*ret_ptr = glue_unpack(msg, long);
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void read_callee(struct glue_message* msg)
{
	fptr_read function_ptr = glue_unpack(msg, fptr_read);
	struct file* file = 0;
	char* buf = 0;
	unsigned long count = 0;
	long long* ppos = 0;
	struct file** file_ptr = &file;
	char** buf_ptr = &buf;
	unsigned long* count_ptr = &count;
	long long** ppos_ptr = &ppos;
	long ret = 0;
	long* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	*file_ptr = glue_unpack(msg, struct file*);
	if (*file_ptr) {
		callee_unmarshal_kernel__read__file__in(msg, *file_ptr);
	}

	*buf_ptr = glue_unpack(msg, char*);
	if (*buf_ptr) {
		**buf_ptr = glue_unpack(msg, char);
	}

	*count_ptr = glue_unpack(msg, unsigned long);
	*ppos_ptr = glue_unpack(msg, long long*);
	if (*ppos_ptr) {
		**ppos_ptr = glue_unpack(msg, long long);
	}

	ret = function_ptr(file, buf, count, ppos);

	msg->position = 0;
	if (*file_ptr) {
		callee_marshal_kernel__read__file__in(msg, *file_ptr);
	}

	(void)buf_ptr;
	(void)ppos_ptr;
	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_read)
long LCD_TRAMPOLINE_LINKAGE(trmp_read) trmp_read(struct file* file, char* buf, unsigned long count, long long* ppos)
{
	volatile fptr_impl_read impl;
	fptr_read target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_read);
	impl = trmp_impl_read;
	return impl(target, file, buf, count, ppos);
}

long trmp_impl_write(fptr_write target, struct file* file, char const* buf, unsigned long count, long long* ppos)
{
	struct glue_message *msg = glue_init_msg();

	struct file** file_ptr = &file;
	char const** buf_ptr = &buf;
	unsigned long* count_ptr = &count;
	long long** ppos_ptr = &ppos;
	long ret = 0;
	long* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(msg, target);
	glue_pack_shadow(msg, *file_ptr);
	if (*file_ptr) {
		caller_marshal_kernel__write__file__in(msg, *file_ptr);
	}

	glue_pack_shadow(msg, *buf_ptr);
	if (*buf_ptr) {
		glue_pack(msg, **buf_ptr);
	}

	glue_pack(msg, *count_ptr);
	glue_pack_shadow(msg, *ppos_ptr);
	if (*ppos_ptr) {
		glue_pack(msg, **ppos_ptr);
	}

	glue_call_client(msg, RPC_ID_write);

	if (*file_ptr) {
		caller_unmarshal_kernel__write__file__in(msg, *file_ptr);
	}

	(void)buf_ptr;
	(void)ppos_ptr;
	*ret_ptr = glue_unpack(msg, long);
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void write_callee(struct glue_message* msg)
{
	fptr_write function_ptr = glue_unpack(msg, fptr_write);
	struct file* file = 0;
	char const* buf = 0;
	unsigned long count = 0;
	long long* ppos = 0;
	struct file** file_ptr = &file;
	char const** buf_ptr = &buf;
	unsigned long* count_ptr = &count;
	long long** ppos_ptr = &ppos;
	long ret = 0;
	long* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	*file_ptr = glue_unpack(msg, struct file*);
	if (*file_ptr) {
		callee_unmarshal_kernel__write__file__in(msg, *file_ptr);
	}

	*buf_ptr = glue_unpack(msg, char const*);
	if (*buf_ptr) {
		char* writable = (char*)*buf_ptr;
		*writable = glue_unpack(msg, char);
	}

	*count_ptr = glue_unpack(msg, unsigned long);
	*ppos_ptr = glue_unpack(msg, long long*);
	if (*ppos_ptr) {
		**ppos_ptr = glue_unpack(msg, long long);
	}

	ret = function_ptr(file, buf, count, ppos);

	msg->position = 0;
	if (*file_ptr) {
		callee_marshal_kernel__write__file__in(msg, *file_ptr);
	}

	(void)buf_ptr;
	(void)ppos_ptr;
	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_write)
long LCD_TRAMPOLINE_LINKAGE(trmp_write) trmp_write(struct file* file, char const* buf, unsigned long count, long long* ppos)
{
	volatile fptr_impl_write impl;
	fptr_write target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_write);
	impl = trmp_impl_write;
	return impl(target, file, buf, count, ppos);
}

long trmp_impl_compat_ioctl(fptr_compat_ioctl target, struct file* file, unsigned int ioc, unsigned long arg)
{
	struct glue_message *msg = glue_init_msg();

	struct file** file_ptr = &file;
	unsigned int* ioc_ptr = &ioc;
	unsigned long* arg_ptr = &arg;
	long ret = 0;
	long* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(msg, target);
	glue_pack_shadow(msg, *file_ptr);
	if (*file_ptr) {
		caller_marshal_kernel__compat_ioctl__file__in(msg, *file_ptr);
	}

	glue_pack(msg, *ioc_ptr);
	glue_pack(msg, *arg_ptr);
	glue_call_client(msg, RPC_ID_compat_ioctl);

	if (*file_ptr) {
		caller_unmarshal_kernel__compat_ioctl__file__in(msg, *file_ptr);
	}

	*ret_ptr = glue_unpack(msg, long);
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void compat_ioctl_callee(struct glue_message* msg)
{
	fptr_compat_ioctl function_ptr = glue_unpack(msg, fptr_compat_ioctl);
	struct file* file = 0;
	unsigned int ioc = 0;
	unsigned long arg = 0;
	struct file** file_ptr = &file;
	unsigned int* ioc_ptr = &ioc;
	unsigned long* arg_ptr = &arg;
	long ret = 0;
	long* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	*file_ptr = glue_unpack(msg, struct file*);
	if (*file_ptr) {
		callee_unmarshal_kernel__compat_ioctl__file__in(msg, *file_ptr);
	}

	*ioc_ptr = glue_unpack(msg, unsigned int);
	*arg_ptr = glue_unpack(msg, unsigned long);
	ret = function_ptr(file, ioc, arg);

	msg->position = 0;
	if (*file_ptr) {
		callee_marshal_kernel__compat_ioctl__file__in(msg, *file_ptr);
	}

	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_compat_ioctl)
long LCD_TRAMPOLINE_LINKAGE(trmp_compat_ioctl) trmp_compat_ioctl(struct file* file, unsigned int ioc, unsigned long arg)
{
	volatile fptr_impl_compat_ioctl impl;
	fptr_compat_ioctl target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_compat_ioctl);
	impl = trmp_impl_compat_ioctl;
	return impl(target, file, ioc, arg);
}

int trmp_impl_open(fptr_open target, struct inode* inode, struct file* file)
{
	struct glue_message *msg = glue_init_msg();

	struct inode** inode_ptr = &inode;
	struct file** file_ptr = &file;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(msg, target);
	glue_pack_shadow(msg, *inode_ptr);
	if (*inode_ptr) {
		caller_marshal_kernel__open__inode__in(msg, *inode_ptr);
	}

	glue_pack_shadow(msg, *file_ptr);
	if (*file_ptr) {
		caller_marshal_kernel__open__file__in(msg, *file_ptr);
	}

	glue_call_client(msg, RPC_ID_open);

	if (*inode_ptr) {
		caller_unmarshal_kernel__open__inode__in(msg, *inode_ptr);
	}

	if (*file_ptr) {
		caller_unmarshal_kernel__open__file__in(msg, *file_ptr);
	}

	*ret_ptr = glue_unpack(msg, int);
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void open_callee(struct glue_message* msg)
{
	fptr_open function_ptr = glue_unpack(msg, fptr_open);
	struct inode* inode = 0;
	struct file* file = 0;
	struct inode** inode_ptr = &inode;
	struct file** file_ptr = &file;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	*inode_ptr = glue_unpack(msg, struct inode*);
	if (*inode_ptr) {
		callee_unmarshal_kernel__open__inode__in(msg, *inode_ptr);
	}

	*file_ptr = glue_unpack(msg, struct file*);
	if (*file_ptr) {
		callee_unmarshal_kernel__open__file__in(msg, *file_ptr);
	}

	ret = function_ptr(inode, file);

	msg->position = 0;
	if (*inode_ptr) {
		callee_marshal_kernel__open__inode__in(msg, *inode_ptr);
	}

	if (*file_ptr) {
		callee_marshal_kernel__open__file__in(msg, *file_ptr);
	}

	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_open)
int LCD_TRAMPOLINE_LINKAGE(trmp_open) trmp_open(struct inode* inode, struct file* file)
{
	volatile fptr_impl_open impl;
	fptr_open target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_open);
	impl = trmp_impl_open;
	return impl(target, inode, file);
}

void __register_chrdev_callee(struct glue_message* msg)
{
	unsigned int major = 0;
	unsigned int baseminor = 0;
	unsigned int count = 0;
	char const* name = 0;
	struct file_operations const* fops = 0;
	unsigned int* major_ptr = &major;
	unsigned int* baseminor_ptr = &baseminor;
	unsigned int* count_ptr = &count;
	char const** name_ptr = &name;
	struct file_operations const** fops_ptr = &fops;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	*major_ptr = glue_unpack(msg, unsigned int);
	*baseminor_ptr = glue_unpack(msg, unsigned int);
	*count_ptr = glue_unpack(msg, unsigned int);
	*name_ptr = glue_unpack(msg, char const*);
	if (*name_ptr) {
		char* writable = (char*)*name_ptr;
		*writable = glue_unpack(msg, char);
	}

	*fops_ptr = glue_unpack(msg, struct file_operations const*);
	if (*fops_ptr) {
		struct file_operations* writable = (struct file_operations*)*fops_ptr;
		callee_unmarshal_kernel____register_chrdev__fops__in(msg, writable);
	}

	ret = __register_chrdev(major, baseminor, count, name, fops);

	msg->position = 0;
	(void)name_ptr;
	if (*fops_ptr) {
		callee_marshal_kernel____register_chrdev__fops__in(msg, *fops_ptr);
	}

	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __unregister_chrdev_callee(struct glue_message* msg)
{
	unsigned int major = 0;
	unsigned int baseminor = 0;
	unsigned int count = 0;
	char const* name = 0;
	unsigned int* major_ptr = &major;
	unsigned int* baseminor_ptr = &baseminor;
	unsigned int* count_ptr = &count;
	char const** name_ptr = &name;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	*major_ptr = glue_unpack(msg, unsigned int);
	*baseminor_ptr = glue_unpack(msg, unsigned int);
	*count_ptr = glue_unpack(msg, unsigned int);
	*name_ptr = glue_unpack(msg, char const*);
	if (*name_ptr) {
		char* writable = (char*)*name_ptr;
		*writable = glue_unpack(msg, char);
	}

	__unregister_chrdev(major, baseminor, count, name);

	msg->position = 0;
	(void)name_ptr;
	msg->slots[0] = msg->position;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void cpu_maps_update_begin_callee(struct glue_message* msg)
{
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	cpu_maps_update_begin();

	msg->position = 0;
	msg->slots[0] = msg->position;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void cpu_maps_update_done_callee(struct glue_message* msg)
{
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	cpu_maps_update_done();

	msg->position = 0;
	msg->slots[0] = msg->position;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void wrmsr_safe_regs_on_cpu_callee(struct glue_message* msg)
{
	unsigned int cpu = 0;
	unsigned int* regs = 0;
	unsigned int* cpu_ptr = &cpu;
	unsigned int** regs_ptr = &regs;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	*cpu_ptr = glue_unpack(msg, unsigned int);
	*regs_ptr = glue_unpack(msg, unsigned int*);
	if (*regs_ptr) {
		int i;
		unsigned int* array = *regs_ptr;
		size_t len = glue_unpack(msg, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(msg, unsigned int);
		}

	}

	ret = wrmsr_safe_regs_on_cpu(cpu, regs);

	msg->position = 0;
	(void)regs_ptr;
	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rdmsr_safe_regs_on_cpu_callee(struct glue_message* msg)
{
	unsigned int cpu = 0;
	unsigned int* regs = 0;
	unsigned int* cpu_ptr = &cpu;
	unsigned int** regs_ptr = &regs;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	*cpu_ptr = glue_unpack(msg, unsigned int);
	*regs_ptr = glue_unpack(msg, unsigned int*);
	if (*regs_ptr) {
		int i;
		unsigned int* array = *regs_ptr;
		size_t len = glue_unpack(msg, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(msg, unsigned int);
		}

	}

	ret = rdmsr_safe_regs_on_cpu(cpu, regs);

	msg->position = 0;
	(void)regs_ptr;
	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void wrmsr_safe_on_cpu_callee(struct glue_message* msg)
{
	unsigned int cpu = 0;
	unsigned int msr_no = 0;
	unsigned int l = 0;
	unsigned int h = 0;
	unsigned int* cpu_ptr = &cpu;
	unsigned int* msr_no_ptr = &msr_no;
	unsigned int* l_ptr = &l;
	unsigned int* h_ptr = &h;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	*cpu_ptr = glue_unpack(msg, unsigned int);
	*msr_no_ptr = glue_unpack(msg, unsigned int);
	*l_ptr = glue_unpack(msg, unsigned int);
	*h_ptr = glue_unpack(msg, unsigned int);
	ret = wrmsr_safe_on_cpu(cpu, msr_no, l, h);

	msg->position = 0;
	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rdmsr_safe_on_cpu_callee(struct glue_message* msg)
{
	unsigned int cpu = 0;
	unsigned int msr_no = 0;
	unsigned int* l = 0;
	unsigned int* h = 0;
	unsigned int* cpu_ptr = &cpu;
	unsigned int* msr_no_ptr = &msr_no;
	unsigned int** l_ptr = &l;
	unsigned int** h_ptr = &h;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	*cpu_ptr = glue_unpack(msg, unsigned int);
	*msr_no_ptr = glue_unpack(msg, unsigned int);
	*l_ptr = glue_unpack(msg, unsigned int*);
	if (*l_ptr) {
		**l_ptr = glue_unpack(msg, unsigned int);
	}

	*h_ptr = glue_unpack(msg, unsigned int*);
	if (*h_ptr) {
		**h_ptr = glue_unpack(msg, unsigned int);
	}

	ret = rdmsr_safe_on_cpu(cpu, msr_no, l, h);

	msg->position = 0;
	(void)l_ptr;
	(void)h_ptr;
	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void capable_callee(struct glue_message* msg)
{
	int cap = 0;
	int* cap_ptr = &cap;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	*cap_ptr = glue_unpack(msg, int);
	ret = capable(cap);

	msg->position = 0;
	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void no_seek_end_llseek_callee(struct glue_message* msg)
{
	struct file* file = 0;
	long long offset = 0;
	int whence = 0;
	struct file** file_ptr = &file;
	long long* offset_ptr = &offset;
	int* whence_ptr = &whence;
	long long ret = 0;
	long long* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	*file_ptr = glue_unpack(msg, struct file*);
	if (*file_ptr) {
		callee_unmarshal_kernel__no_seek_end_llseek__file__in(msg, *file_ptr);
	}

	*offset_ptr = glue_unpack(msg, long long);
	*whence_ptr = glue_unpack(msg, int);
	ret = no_seek_end_llseek(file, offset, whence);

	msg->position = 0;
	if (*file_ptr) {
		callee_marshal_kernel__no_seek_end_llseek__file__in(msg, *file_ptr);
	}

	glue_pack(msg, *ret_ptr);
	msg->slots[0] = msg->position;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int try_dispatch(enum RPC_ID id, struct glue_message* msg)
{
	switch(id) {
	case RPC_ID_read:
		glue_user_trace("read\n");
		read_callee(msg);
		break;

	case RPC_ID_write:
		glue_user_trace("write\n");
		write_callee(msg);
		break;

	case RPC_ID_compat_ioctl:
		glue_user_trace("compat_ioctl\n");
		compat_ioctl_callee(msg);
		break;

	case RPC_ID_open:
		glue_user_trace("open\n");
		open_callee(msg);
		break;

	case RPC_ID___register_chrdev:
		glue_user_trace("__register_chrdev\n");
		__register_chrdev_callee(msg);
		break;

	case RPC_ID___unregister_chrdev:
		glue_user_trace("__unregister_chrdev\n");
		__unregister_chrdev_callee(msg);
		break;

	case RPC_ID_cpu_maps_update_begin:
		glue_user_trace("cpu_maps_update_begin\n");
		cpu_maps_update_begin_callee(msg);
		break;

	case RPC_ID_cpu_maps_update_done:
		glue_user_trace("cpu_maps_update_done\n");
		cpu_maps_update_done_callee(msg);
		break;

	case RPC_ID_wrmsr_safe_regs_on_cpu:
		glue_user_trace("wrmsr_safe_regs_on_cpu\n");
		wrmsr_safe_regs_on_cpu_callee(msg);
		break;

	case RPC_ID_rdmsr_safe_regs_on_cpu:
		glue_user_trace("rdmsr_safe_regs_on_cpu\n");
		rdmsr_safe_regs_on_cpu_callee(msg);
		break;

	case RPC_ID_wrmsr_safe_on_cpu:
		glue_user_trace("wrmsr_safe_on_cpu\n");
		wrmsr_safe_on_cpu_callee(msg);
		break;

	case RPC_ID_rdmsr_safe_on_cpu:
		glue_user_trace("rdmsr_safe_on_cpu\n");
		rdmsr_safe_on_cpu_callee(msg);
		break;

	case RPC_ID_capable:
		glue_user_trace("capable\n");
		capable_callee(msg);
		break;

	case RPC_ID_no_seek_end_llseek:
		glue_user_trace("no_seek_end_llseek\n");
		no_seek_end_llseek_callee(msg);
		break;

	default:
		return 0;
	}

	return 1;
}

