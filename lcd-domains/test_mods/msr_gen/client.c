#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

unsigned long volatile jiffies;

void read_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_read function_ptr = glue_unpack(__pos, __msg, __ext, fptr_read);
	struct file* file = 0;
	char* buf = 0;
	unsigned long count = 0;
	struct file** file_ptr = &file;
	char** buf_ptr = &buf;
	unsigned long* count_ptr = &count;
	long long __ppos;
	long long* ppos = &__ppos;
	long long** ppos_ptr = &ppos;
	long ret = 0;
	long* ret_ptr = &ret;
	
	__maybe_unused struct read_call_ctx call_ctx = {file, buf, count, ppos};
	__maybe_unused struct read_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct file);
		*file_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct file*, __size, DEFAULT_GFP_FLAGS);
		if (*file_ptr) {
			callee_unmarshal_kernel__read__file__in(__pos, __msg, __ext, ctx, *file_ptr);
		}

	}

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*buf_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char*, (__size), (DEFAULT_GFP_FLAGS));
		if (*buf_ptr) {
			int i;
			char* array = *buf_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	{
		*count_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		if (*ppos_ptr) {
			**ppos_ptr = glue_unpack(__pos, __msg, __ext, long long);
		}

	}

	ret = function_ptr(file, buf, count, ppos);

	*__pos = 0;
	{
		if (*file_ptr) {
			callee_marshal_kernel__read__file__in(__pos, __msg, __ext, ctx, *file_ptr);
		}

	}

	{
		if (*buf_ptr) {
			size_t i, len = (count);
			char* array = *buf_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
	}

	{
		(void)ppos_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void write_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_write function_ptr = glue_unpack(__pos, __msg, __ext, fptr_write);
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
	
	__maybe_unused struct write_call_ctx call_ctx = {file, buf, count, ppos};
	__maybe_unused struct write_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct file);
		*file_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct file*, (__size), (DEFAULT_GFP_FLAGS));
		if (*file_ptr) {
			callee_unmarshal_kernel__write__file__in(__pos, __msg, __ext, ctx, *file_ptr);
		}

	}

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*buf_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*buf_ptr) {
			char* writable = (char*)*buf_ptr;
			int i;
			char* array = writable;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	{
		*count_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		size_t __size = sizeof(long long);
		*ppos_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, long long*, (__size), (DEFAULT_GFP_FLAGS));
		if (*ppos_ptr) {
			**ppos_ptr = glue_unpack(__pos, __msg, __ext, long long);
		}

	}

	ret = function_ptr(file, buf, count, ppos);

	*__pos = 0;
	{
		if (*file_ptr) {
			callee_marshal_kernel__write__file__in(__pos, __msg, __ext, ctx, *file_ptr);
		}

	}

	{
		(void)buf_ptr;
	}

	{
	}

	{
		(void)ppos_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void compat_ioctl_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_compat_ioctl function_ptr = glue_unpack(__pos, __msg, __ext, fptr_compat_ioctl);
	struct file* file = 0;
	unsigned int ioc = 0;
	unsigned long arg = 0;
	struct file** file_ptr = &file;
	unsigned int* ioc_ptr = &ioc;
	unsigned long* arg_ptr = &arg;
	long ret = 0;
	long* ret_ptr = &ret;
	
	__maybe_unused struct compat_ioctl_call_ctx call_ctx = {file, ioc, arg};
	__maybe_unused struct compat_ioctl_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct file);
		*file_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct file*, (__size), (DEFAULT_GFP_FLAGS));
		if (*file_ptr) {
			callee_unmarshal_kernel__compat_ioctl__file__in(__pos, __msg, __ext, ctx, *file_ptr);
		}

	}

	{
		*ioc_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*arg_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	ret = function_ptr(file, ioc, arg);

	*__pos = 0;
	{
		if (*file_ptr) {
			callee_marshal_kernel__compat_ioctl__file__in(__pos, __msg, __ext, ctx, *file_ptr);
		}

	}

	{
	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void open_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_open function_ptr = glue_unpack(__pos, __msg, __ext, fptr_open);
	struct inode* inode = 0;
	struct file* file = 0;
	struct inode** inode_ptr = &inode;
	struct file** file_ptr = &file;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct open_call_ctx call_ctx = {inode, file};
	__maybe_unused struct open_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct inode);
		*inode_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct inode*, (__size), (DEFAULT_GFP_FLAGS));
		if (*inode_ptr) {
			callee_unmarshal_kernel__open__inode__in(__pos, __msg, __ext, ctx, *inode_ptr);
		}

	}

	{
		size_t __size = sizeof(struct file);
		*file_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct file*, (__size), (DEFAULT_GFP_FLAGS));
		if (*file_ptr) {
			callee_unmarshal_kernel__open__file__in(__pos, __msg, __ext, ctx, *file_ptr);
		}

	}

	ret = function_ptr(inode, file);

	*__pos = 0;
	{
		if (*inode_ptr) {
			callee_marshal_kernel__open__inode__in(__pos, __msg, __ext, ctx, *inode_ptr);
		}

	}

	{
		if (*file_ptr) {
			callee_marshal_kernel__open__file__in(__pos, __msg, __ext, ctx, *file_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int __register_chrdev(unsigned int major, unsigned int baseminor, unsigned int count, char const* name, struct file_operations const* fops)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* major_ptr = &major;
	unsigned int* baseminor_ptr = &baseminor;
	unsigned int* count_ptr = &count;
	char const** name_ptr = &name;
	struct file_operations const** fops_ptr = &fops;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct __register_chrdev_call_ctx call_ctx = {major, baseminor, count, name, fops};
	__maybe_unused const struct __register_chrdev_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *major_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *baseminor_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *count_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *name_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*name_ptr) {
			size_t i, len;
			char const* array = *name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		__maybe_unused const void* __adjusted = *fops_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*fops_ptr) {
			caller_marshal_kernel____register_chrdev__fops__in(__pos, __msg, __ext, ctx, *fops_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID___register_chrdev);

	*__pos = 0;
	{
	}

	{
	}

	{
	}

	{
		(void)name_ptr;
	}

	{
		if (*fops_ptr) {
			struct file_operations* writable = (struct file_operations*)*fops_ptr;
			caller_unmarshal_kernel____register_chrdev__fops__in(__pos, __msg, __ext, ctx, writable);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void __unregister_chrdev(unsigned int major, unsigned int baseminor, unsigned int count, char const* name)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* major_ptr = &major;
	unsigned int* baseminor_ptr = &baseminor;
	unsigned int* count_ptr = &count;
	char const** name_ptr = &name;
	
	__maybe_unused const struct __unregister_chrdev_call_ctx call_ctx = {major, baseminor, count, name};
	__maybe_unused const struct __unregister_chrdev_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *major_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *baseminor_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *count_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *name_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*name_ptr) {
			size_t i, len;
			char const* array = *name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	glue_call_server(__pos, __msg, RPC_ID___unregister_chrdev);

	*__pos = 0;
	{
	}

	{
	}

	{
	}

	{
		(void)name_ptr;
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void cpu_maps_update_begin(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	
	__maybe_unused const struct cpu_maps_update_begin_call_ctx call_ctx = {};
	__maybe_unused const struct cpu_maps_update_begin_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, __msg, RPC_ID_cpu_maps_update_begin);

	*__pos = 0;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void cpu_maps_update_done(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	
	__maybe_unused const struct cpu_maps_update_done_call_ctx call_ctx = {};
	__maybe_unused const struct cpu_maps_update_done_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, __msg, RPC_ID_cpu_maps_update_done);

	*__pos = 0;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int wrmsr_safe_regs_on_cpu(unsigned int cpu, unsigned int* regs)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* cpu_ptr = &cpu;
	unsigned int** regs_ptr = &regs;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct wrmsr_safe_regs_on_cpu_call_ctx call_ctx = {cpu, regs};
	__maybe_unused const struct wrmsr_safe_regs_on_cpu_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *cpu_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *regs_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*regs_ptr) {
			size_t i, len = 8;
			unsigned int const* array = *regs_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned int const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	glue_call_server(__pos, __msg, RPC_ID_wrmsr_safe_regs_on_cpu);

	*__pos = 0;
	{
	}

	{
		if (*regs_ptr) {
			int i;
			unsigned int* array = *regs_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned int* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, unsigned int);
			}

		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int rdmsr_safe_regs_on_cpu(unsigned int cpu, unsigned int* regs)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* cpu_ptr = &cpu;
	unsigned int** regs_ptr = &regs;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct rdmsr_safe_regs_on_cpu_call_ctx call_ctx = {cpu, regs};
	__maybe_unused const struct rdmsr_safe_regs_on_cpu_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *cpu_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *regs_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*regs_ptr) {
			size_t i, len = 8;
			unsigned int const* array = *regs_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned int const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	glue_call_server(__pos, __msg, RPC_ID_rdmsr_safe_regs_on_cpu);

	*__pos = 0;
	{
	}

	{
		if (*regs_ptr) {
			int i;
			unsigned int* array = *regs_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned int* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, unsigned int);
			}

		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int wrmsr_safe_on_cpu(unsigned int cpu, unsigned int msr_no, unsigned int l, unsigned int h)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* cpu_ptr = &cpu;
	unsigned int* msr_no_ptr = &msr_no;
	unsigned int* l_ptr = &l;
	unsigned int* h_ptr = &h;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct wrmsr_safe_on_cpu_call_ctx call_ctx = {cpu, msr_no, l, h};
	__maybe_unused const struct wrmsr_safe_on_cpu_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *cpu_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *msr_no_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *l_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *h_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_wrmsr_safe_on_cpu);

	*__pos = 0;
	{
	}

	{
	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int rdmsr_safe_on_cpu(unsigned int cpu, unsigned int msr_no, unsigned int* l, unsigned int* h)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* cpu_ptr = &cpu;
	unsigned int* msr_no_ptr = &msr_no;
	unsigned int** l_ptr = &l;
	unsigned int** h_ptr = &h;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct rdmsr_safe_on_cpu_call_ctx call_ctx = {cpu, msr_no, l, h};
	__maybe_unused const struct rdmsr_safe_on_cpu_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *cpu_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *msr_no_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *l_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		(void)l_ptr;
	}

	{
		__maybe_unused const void* __adjusted = *h_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		(void)h_ptr;
	}

	glue_call_server(__pos, __msg, RPC_ID_rdmsr_safe_on_cpu);

	*__pos = 0;
	{
	}

	{
	}

	{
		if (*l_ptr) {
			**l_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		if (*h_ptr) {
			**h_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

bool capable(int cap)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* cap_ptr = &cap;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct capable_call_ctx call_ctx = {cap};
	__maybe_unused const struct capable_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *cap_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_capable);

	*__pos = 0;
	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

long long no_seek_end_llseek(struct file* file, long long offset, int whence)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct file** file_ptr = &file;
	long long* offset_ptr = &offset;
	int* whence_ptr = &whence;
	long long ret = 0;
	long long* ret_ptr = &ret;
	
	__maybe_unused const struct no_seek_end_llseek_call_ctx call_ctx = {file, offset, whence};
	__maybe_unused const struct no_seek_end_llseek_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *file_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*file_ptr) {
			caller_marshal_kernel__no_seek_end_llseek__file__in(__pos, __msg, __ext, ctx, *file_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *offset_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *whence_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_no_seek_end_llseek);

	*__pos = 0;
	{
		if (*file_ptr) {
			caller_unmarshal_kernel__no_seek_end_llseek__file__in(__pos, __msg, __ext, ctx, *file_ptr);
		}

	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, long long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct class* __class_create(struct module* owner, char const* name, struct lock_class_key *key)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct module** owner_ptr = &owner;
	char const** name_ptr = &name;
	struct class* ret = 0;
	struct class** ret_ptr = &ret;
	
	__maybe_unused const struct __class_create_call_ctx call_ctx = {owner, name};
	__maybe_unused const struct __class_create_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *owner_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*owner_ptr) {
			caller_marshal_kernel____class_create__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *name_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*name_ptr) {
			size_t i, len;
			char const* array = *name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	glue_call_server(__pos, __msg, RPC_ID___class_create);

	*__pos = 0;
	{
		if (*owner_ptr) {
			caller_unmarshal_kernel____class_create__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

	{
		(void)name_ptr;
	}

	{
		size_t __size = sizeof(struct class);
		*ret_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct class*, (__size), (DEFAULT_GFP_FLAGS));
		if (*ret_ptr) {
			caller_unmarshal_kernel____class_create__ret_class__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void devnode_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_devnode function_ptr = glue_unpack(__pos, __msg, __ext, fptr_devnode);
	struct device* dev = 0;
	unsigned short* mode = 0;
	struct device** dev_ptr = &dev;
	char* ret = 0;
	char** ret_ptr = &ret;
	
	__maybe_unused struct devnode_call_ctx call_ctx = {dev, mode};
	__maybe_unused struct devnode_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct device);
		*dev_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct device*, (__size), (DEFAULT_GFP_FLAGS));
		if (*dev_ptr) {
			callee_unmarshal_kernel__devnode__device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	ret = function_ptr(dev, mode);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__devnode__device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ret_ptr) {
			size_t i, len;
			char const* array = *ret_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct device* __device_create(struct class* class, struct device* parent, unsigned int devt, void* drvdata, char const* fmt, unsigned int cpu)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct class** class_ptr = &class;
	struct device** parent_ptr = &parent;
	unsigned int* devt_ptr = &devt;
	char const** fmt_ptr = &fmt;
	unsigned int* cpu_ptr = &cpu;
	struct device* ret = 0;
	struct device** ret_ptr = &ret;
	
	__maybe_unused const struct __device_create_call_ctx call_ctx = {class, parent, devt, drvdata, fmt, cpu};
	__maybe_unused const struct __device_create_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *class_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*class_ptr) {
			caller_marshal_kernel____device_create__class__in(__pos, __msg, __ext, ctx, *class_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *parent_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*parent_ptr) {
			caller_marshal_kernel____device_create__parent__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *devt_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *fmt_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*fmt_ptr) {
			size_t i, len;
			char const* array = *fmt_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cpu_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID___device_create);

	*__pos = 0;
	{
		if (*class_ptr) {
			caller_unmarshal_kernel____device_create__class__in(__pos, __msg, __ext, ctx, *class_ptr);
		}

	}

	{
		if (*parent_ptr) {
			caller_unmarshal_kernel____device_create__parent__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

	{
	}

	{
		(void)fmt_ptr;
	}

	{
	}

	{
		*ret_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct device*);
		if (*ret_ptr) {
			caller_unmarshal_kernel____device_create__ret_device__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void device_destroy(struct class* class, unsigned int devt)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct class** class_ptr = &class;
	unsigned int* devt_ptr = &devt;
	
	__maybe_unused const struct device_destroy_call_ctx call_ctx = {class, devt};
	__maybe_unused const struct device_destroy_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *class_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*class_ptr) {
			caller_marshal_kernel__device_destroy__class__in(__pos, __msg, __ext, ctx, *class_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *devt_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_device_destroy);

	*__pos = 0;
	{
		if (*class_ptr) {
			caller_unmarshal_kernel__device_destroy__class__in(__pos, __msg, __ext, ctx, *class_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void class_destroy(struct class* cls)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct class** cls_ptr = &cls;
	
	__maybe_unused const struct class_destroy_call_ctx call_ctx = {cls};
	__maybe_unused const struct class_destroy_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *cls_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*cls_ptr) {
			caller_marshal_kernel__class_destroy__cls__in(__pos, __msg, __ext, ctx, *cls_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_class_destroy);

	*__pos = 0;
	{
		if (*cls_ptr) {
			caller_unmarshal_kernel__class_destroy__cls__in(__pos, __msg, __ext, ctx, *cls_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

unsigned long get_jiffies(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused const struct get_jiffies_call_ctx call_ctx = {};
	__maybe_unused const struct get_jiffies_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, __msg, RPC_ID_get_jiffies);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void __init_globals(void) {

	jiffies = get_jiffies();
}

int try_dispatch(enum RPC_ID id, struct fipc_message* __msg, struct ext_registers* __ext)
{
	switch(id) {
	case MODULE_INIT:
		glue_user_trace("MODULE_INIT");
		__init_globals();
		__module_lcd_init();
		shared_mem_init();
		break;

	case MODULE_EXIT:
		glue_user_trace("MODULE_EXIT");
		__module_lcd_exit();
		break;

	case RPC_ID_read:
		glue_user_trace("read\n");
		read_callee(__msg, __ext);
		break;

	case RPC_ID_write:
		glue_user_trace("write\n");
		write_callee(__msg, __ext);
		break;

	case RPC_ID_compat_ioctl:
		glue_user_trace("compat_ioctl\n");
		compat_ioctl_callee(__msg, __ext);
		break;

	case RPC_ID_open:
		glue_user_trace("open\n");
		open_callee(__msg, __ext);
		break;

	case RPC_ID_devnode:
		glue_user_trace("devnode\n");
		devnode_callee(__msg, __ext);
		break;

	default:
		return 0;
	}

	return 1;
}

