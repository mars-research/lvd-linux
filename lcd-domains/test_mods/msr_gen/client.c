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

	glue_call_server(msg, RPC_ID_read);

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

	glue_call_server(msg, RPC_ID_write);

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
	glue_call_server(msg, RPC_ID_compat_ioctl);

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

	glue_call_server(msg, RPC_ID_open);

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

int __register_chrdev(unsigned int major, unsigned int baseminor, unsigned int count, char const* name, struct file_operations const* fops)
{
	struct glue_message *msg = glue_init_msg();

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

	glue_pack(msg, *major_ptr);
	glue_pack(msg, *baseminor_ptr);
	glue_pack(msg, *count_ptr);
	glue_pack_shadow(msg, *name_ptr);
	if (*name_ptr) {
		glue_pack(msg, **name_ptr);
	}

	glue_pack_shadow(msg, *fops_ptr);
	if (*fops_ptr) {
		caller_marshal_kernel____register_chrdev__fops__in(msg, *fops_ptr);
	}

	glue_call_server(msg, RPC_ID___register_chrdev);

	(void)name_ptr;
	if (*fops_ptr) {
		struct file_operations* writable = (struct file_operations*)*fops_ptr;
		caller_unmarshal_kernel____register_chrdev__fops__in(msg, writable);
	}

	*ret_ptr = glue_unpack(msg, int);
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void __unregister_chrdev(unsigned int major, unsigned int baseminor, unsigned int count, char const* name)
{
	struct glue_message *msg = glue_init_msg();

	unsigned int* major_ptr = &major;
	unsigned int* baseminor_ptr = &baseminor;
	unsigned int* count_ptr = &count;
	char const** name_ptr = &name;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(msg, *major_ptr);
	glue_pack(msg, *baseminor_ptr);
	glue_pack(msg, *count_ptr);
	glue_pack_shadow(msg, *name_ptr);
	if (*name_ptr) {
		glue_pack(msg, **name_ptr);
	}

	glue_call_server(msg, RPC_ID___unregister_chrdev);

	(void)name_ptr;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void cpu_maps_update_begin(void)
{
	struct glue_message *msg = glue_init_msg();

	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(msg, RPC_ID_cpu_maps_update_begin);

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void cpu_maps_update_done(void)
{
	struct glue_message *msg = glue_init_msg();

	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(msg, RPC_ID_cpu_maps_update_done);

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int wrmsr_safe_regs_on_cpu(unsigned int cpu, unsigned int* regs)
{
	struct glue_message *msg = glue_init_msg();

	unsigned int* cpu_ptr = &cpu;
	unsigned int** regs_ptr = &regs;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(msg, *cpu_ptr);
	glue_pack_shadow(msg, *regs_ptr);
	if (*regs_ptr) {
		size_t i, len = 8;
		unsigned int* array = *regs_ptr;
		glue_pack(msg, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			glue_pack(msg, *element);
		}

	}

	glue_call_server(msg, RPC_ID_wrmsr_safe_regs_on_cpu);

	(void)regs_ptr;
	*ret_ptr = glue_unpack(msg, int);
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int rdmsr_safe_regs_on_cpu(unsigned int cpu, unsigned int* regs)
{
	struct glue_message *msg = glue_init_msg();

	unsigned int* cpu_ptr = &cpu;
	unsigned int** regs_ptr = &regs;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(msg, *cpu_ptr);
	glue_pack_shadow(msg, *regs_ptr);
	if (*regs_ptr) {
		size_t i, len = 8;
		unsigned int* array = *regs_ptr;
		glue_pack(msg, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			glue_pack(msg, *element);
		}

	}

	glue_call_server(msg, RPC_ID_rdmsr_safe_regs_on_cpu);

	(void)regs_ptr;
	*ret_ptr = glue_unpack(msg, int);
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int wrmsr_safe_on_cpu(unsigned int cpu, unsigned int msr_no, unsigned int l, unsigned int h)
{
	struct glue_message *msg = glue_init_msg();

	unsigned int* cpu_ptr = &cpu;
	unsigned int* msr_no_ptr = &msr_no;
	unsigned int* l_ptr = &l;
	unsigned int* h_ptr = &h;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(msg, *cpu_ptr);
	glue_pack(msg, *msr_no_ptr);
	glue_pack(msg, *l_ptr);
	glue_pack(msg, *h_ptr);
	glue_call_server(msg, RPC_ID_wrmsr_safe_on_cpu);

	*ret_ptr = glue_unpack(msg, int);
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int rdmsr_safe_on_cpu(unsigned int cpu, unsigned int msr_no, unsigned int* l, unsigned int* h)
{
	struct glue_message *msg = glue_init_msg();

	unsigned int* cpu_ptr = &cpu;
	unsigned int* msr_no_ptr = &msr_no;
	unsigned int** l_ptr = &l;
	unsigned int** h_ptr = &h;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(msg, *cpu_ptr);
	glue_pack(msg, *msr_no_ptr);
	glue_pack_shadow(msg, *l_ptr);
	if (*l_ptr) {
		glue_pack(msg, **l_ptr);
	}

	glue_pack_shadow(msg, *h_ptr);
	if (*h_ptr) {
		glue_pack(msg, **h_ptr);
	}

	glue_call_server(msg, RPC_ID_rdmsr_safe_on_cpu);

	(void)l_ptr;
	(void)h_ptr;
	*ret_ptr = glue_unpack(msg, int);
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

bool capable(int cap)
{
	struct glue_message *msg = glue_init_msg();

	int* cap_ptr = &cap;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(msg, *cap_ptr);
	glue_call_server(msg, RPC_ID_capable);

	*ret_ptr = glue_unpack(msg, bool);
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

long long no_seek_end_llseek(struct file* file, long long offset, int whence)
{
	struct glue_message *msg = glue_init_msg();

	struct file** file_ptr = &file;
	long long* offset_ptr = &offset;
	int* whence_ptr = &whence;
	long long ret = 0;
	long long* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack_shadow(msg, *file_ptr);
	if (*file_ptr) {
		caller_marshal_kernel__no_seek_end_llseek__file__in(msg, *file_ptr);
	}

	glue_pack(msg, *offset_ptr);
	glue_pack(msg, *whence_ptr);
	glue_call_server(msg, RPC_ID_no_seek_end_llseek);

	if (*file_ptr) {
		caller_unmarshal_kernel__no_seek_end_llseek__file__in(msg, *file_ptr);
	}

	*ret_ptr = glue_unpack(msg, long long);
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int try_dispatch(enum RPC_ID id, struct glue_message* msg)
{
	switch(id) {
	case MODULE_INIT:
		glue_user_trace("MODULE_INIT");
		__module_lcd_init();
		break;

	case MODULE_EXIT:
		glue_user_trace("MODULE_EXIT");
		__module_lcd_exit();
		break;

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

	default:
		return 0;
	}

	return 1;
}

