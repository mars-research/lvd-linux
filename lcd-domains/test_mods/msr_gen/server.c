#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

long trmp_impl_read(fptr_read target, struct file* file, char* buf, unsigned long count, long long* ppos)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct file** file_ptr = &file;
	char** buf_ptr = &buf;
	unsigned long* count_ptr = &count;
	long long** ppos_ptr = &ppos;
	long ret = 0;
	long* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(pos, msg, ext, target);
	{
		glue_pack(pos, msg, ext, *file_ptr);
		if (*file_ptr) {
			caller_marshal_kernel__read__file__in(pos, msg, ext, *file_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *buf_ptr);
		if (*buf_ptr) {
			size_t i, len = *count_ptr;
			char* array = *buf_ptr;
			glue_pack(pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				glue_pack(pos, msg, ext, *element);
			}

		}

	}

	{
		glue_pack(pos, msg, ext, *count_ptr);
	}

	{
		glue_pack(pos, msg, ext, *ppos_ptr);
		if (*ppos_ptr) {
			glue_pack(pos, msg, ext, **ppos_ptr);
		}

	}

	glue_call_client(pos, msg, RPC_ID_read);

	*pos = 0;
	{
		if (*file_ptr) {
			caller_unmarshal_kernel__read__file__in(pos, msg, ext, *file_ptr);
		}

	}

	{
		if (*buf_ptr) {
			int i;
			char* array = *buf_ptr;
			size_t len = glue_unpack(pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(pos, msg, ext, char);
			}

		}

	}

	{
	}

	{
		(void)ppos_ptr;
	}

	{
		*ret_ptr = glue_unpack(pos, msg, ext, long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
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
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct file** file_ptr = &file;
	char const** buf_ptr = &buf;
	unsigned long* count_ptr = &count;
	long long** ppos_ptr = &ppos;
	long ret = 0;
	long* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(pos, msg, ext, target);
	{
		glue_pack(pos, msg, ext, *file_ptr);
		if (*file_ptr) {
			caller_marshal_kernel__write__file__in(pos, msg, ext, *file_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *buf_ptr);
		if (*buf_ptr) {
			size_t i, len = *count_ptr;
			char const* array = *buf_ptr;
			glue_pack(pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(pos, msg, ext, *element);
			}

		}

	}

	{
		glue_pack(pos, msg, ext, *count_ptr);
	}

	{
		glue_pack(pos, msg, ext, *ppos_ptr);
		if (*ppos_ptr) {
			glue_pack(pos, msg, ext, **ppos_ptr);
		}

	}

	glue_call_client(pos, msg, RPC_ID_write);

	*pos = 0;
	{
		if (*file_ptr) {
			caller_unmarshal_kernel__write__file__in(pos, msg, ext, *file_ptr);
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
		*ret_ptr = glue_unpack(pos, msg, ext, long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
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
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct file** file_ptr = &file;
	unsigned int* ioc_ptr = &ioc;
	unsigned long* arg_ptr = &arg;
	long ret = 0;
	long* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(pos, msg, ext, target);
	{
		glue_pack(pos, msg, ext, *file_ptr);
		if (*file_ptr) {
			caller_marshal_kernel__compat_ioctl__file__in(pos, msg, ext, *file_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *ioc_ptr);
	}

	{
		glue_pack(pos, msg, ext, *arg_ptr);
	}

	glue_call_client(pos, msg, RPC_ID_compat_ioctl);

	*pos = 0;
	{
		if (*file_ptr) {
			caller_unmarshal_kernel__compat_ioctl__file__in(pos, msg, ext, *file_ptr);
		}

	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(pos, msg, ext, long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
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
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct inode** inode_ptr = &inode;
	struct file** file_ptr = &file;
	int ret = 0;
	int* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(pos, msg, ext, target);
	{
		glue_pack(pos, msg, ext, *inode_ptr);
		if (*inode_ptr) {
			caller_marshal_kernel__open__inode__in(pos, msg, ext, *inode_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *file_ptr);
		if (*file_ptr) {
			caller_marshal_kernel__open__file__in(pos, msg, ext, *file_ptr);
		}

	}

	glue_call_client(pos, msg, RPC_ID_open);

	*pos = 0;
	{
		if (*inode_ptr) {
			caller_unmarshal_kernel__open__inode__in(pos, msg, ext, *inode_ptr);
		}

	}

	{
		if (*file_ptr) {
			caller_unmarshal_kernel__open__file__in(pos, msg, ext, *file_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
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

void __register_chrdev_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

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

	{
		*major_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*baseminor_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*count_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*name_ptr = glue_unpack_new_shadow(pos, msg, ext, char const*, sizeof(char) * glue_peek(pos, msg, ext));
		if (*name_ptr) {
			char* writable = (char*)*name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(pos, msg, ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(pos, msg, ext, char);
			}

		}

	}

	{
		*fops_ptr = glue_unpack_new_shadow(pos, msg, ext, struct file_operations const*, sizeof(struct file_operations));
		if (*fops_ptr) {
			struct file_operations* writable = (struct file_operations*)*fops_ptr;
			callee_unmarshal_kernel____register_chrdev__fops__in(pos, msg, ext, writable);
			writable->owner = THIS_MODULE;
		}

	}

	ret = __register_chrdev(major, baseminor, count, name, fops);

	*pos = 0;
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
			callee_marshal_kernel____register_chrdev__fops__in(pos, msg, ext, *fops_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __unregister_chrdev_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

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

	{
		*major_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*baseminor_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*count_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*name_ptr = glue_unpack_new_shadow(pos, msg, ext, char const*, sizeof(char) * glue_peek(pos, msg, ext));
		if (*name_ptr) {
			char* writable = (char*)*name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(pos, msg, ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(pos, msg, ext, char);
			}

		}

	}

	__unregister_chrdev(major, baseminor, count, name);

	*pos = 0;
	{
	}

	{
	}

	{
	}

	{
		(void)name_ptr;
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void cpu_maps_update_begin_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	cpu_maps_update_begin();

	*pos = 0;
	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void cpu_maps_update_done_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	cpu_maps_update_done();

	*pos = 0;
	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void wrmsr_safe_regs_on_cpu_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	unsigned int cpu = 0;
	unsigned int* regs = 0;
	unsigned int* cpu_ptr = &cpu;
	unsigned int** regs_ptr = &regs;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*cpu_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*regs_ptr = glue_unpack(pos, msg, ext, unsigned int*);
		if (*regs_ptr) {
			int i;
			unsigned int* array = *regs_ptr;
			size_t len = glue_unpack(pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned int* element = &array[i];
				*element = glue_unpack(pos, msg, ext, unsigned int);
			}

		}

	}

	ret = wrmsr_safe_regs_on_cpu(cpu, regs);

	*pos = 0;
	{
	}

	{
		(void)regs_ptr;
	}

	{
		glue_pack(pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rdmsr_safe_regs_on_cpu_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	unsigned int cpu = 0;
	unsigned int* regs = 0;
	unsigned int* cpu_ptr = &cpu;
	unsigned int** regs_ptr = &regs;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*cpu_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*regs_ptr = glue_unpack(pos, msg, ext, unsigned int*);
		if (*regs_ptr) {
			int i;
			unsigned int* array = *regs_ptr;
			size_t len = glue_unpack(pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned int* element = &array[i];
				*element = glue_unpack(pos, msg, ext, unsigned int);
			}

		}

	}

	ret = rdmsr_safe_regs_on_cpu(cpu, regs);

	*pos = 0;
	{
	}

	{
		(void)regs_ptr;
	}

	{
		glue_pack(pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void wrmsr_safe_on_cpu_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

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

	{
		*cpu_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*msr_no_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*l_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*h_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	ret = wrmsr_safe_on_cpu(cpu, msr_no, l, h);

	*pos = 0;
	{
	}

	{
	}

	{
	}

	{
	}

	{
		glue_pack(pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void rdmsr_safe_on_cpu_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

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

	{
		*cpu_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*msr_no_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*l_ptr = glue_unpack(pos, msg, ext, unsigned int*);
		if (*l_ptr) {
			**l_ptr = glue_unpack(pos, msg, ext, unsigned int);
		}

	}

	{
		*h_ptr = glue_unpack(pos, msg, ext, unsigned int*);
		if (*h_ptr) {
			**h_ptr = glue_unpack(pos, msg, ext, unsigned int);
		}

	}

	ret = rdmsr_safe_on_cpu(cpu, msr_no, l, h);

	*pos = 0;
	{
	}

	{
	}

	{
		(void)l_ptr;
	}

	{
		(void)h_ptr;
	}

	{
		glue_pack(pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void capable_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	int cap = 0;
	int* cap_ptr = &cap;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*cap_ptr = glue_unpack(pos, msg, ext, int);
	}

	ret = capable(cap);

	*pos = 0;
	{
	}

	{
		glue_pack(pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void no_seek_end_llseek_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

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

	{
		*file_ptr = glue_unpack(pos, msg, ext, struct file*);
		if (*file_ptr) {
			callee_unmarshal_kernel__no_seek_end_llseek__file__in(pos, msg, ext, *file_ptr);
		}

	}

	{
		*offset_ptr = glue_unpack(pos, msg, ext, long long);
	}

	{
		*whence_ptr = glue_unpack(pos, msg, ext, int);
	}

	ret = no_seek_end_llseek(file, offset, whence);

	*pos = 0;
	{
		if (*file_ptr) {
			callee_marshal_kernel__no_seek_end_llseek__file__in(pos, msg, ext, *file_ptr);
		}

	}

	{
	}

	{
	}

	{
		glue_pack(pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __class_create_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct module* owner = 0;
	char const* name = 0;
	struct module** owner_ptr = &owner;
	char const** name_ptr = &name;
	struct class* ret = 0;
	struct class** ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*owner_ptr = glue_unpack_shadow(pos, msg, ext, struct module*);
		if (*owner_ptr) {
			callee_unmarshal_kernel____class_create__owner__in(pos, msg, ext, *owner_ptr);
		}

	}

	{
		*name_ptr = glue_unpack_new_shadow(pos, msg, ext, char const*, sizeof(char) * glue_peek(pos, msg, ext));
		if (*name_ptr) {
			char* writable = (char*)*name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(pos, msg, ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(pos, msg, ext, char);
			}

		}

	}

	ret = class_create(THIS_MODULE, name);

	*pos = 0;
	{
		if (*owner_ptr) {
			callee_marshal_kernel____class_create__owner__in(pos, msg, ext, *owner_ptr);
		}

	}

	{
		(void)name_ptr;
	}

	{
		glue_pack(pos, msg, ext, *ret_ptr);
		if (*ret_ptr) {
			callee_marshal_kernel____class_create__ret_class__out(pos, msg, ext, *ret_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

char const* trmp_impl_devnode(fptr_devnode target, struct device* dev, unsigned short* mode)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct device** dev_ptr = &dev;
	unsigned short** mode_ptr = &mode;
	char const* ret = 0;
	char const** ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(pos, msg, ext, target);
	{
		glue_pack(pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__devnode__device__in(pos, msg, ext, *dev_ptr);
		}

	}

	if (0)
	{
		glue_pack_shadow(pos, msg, ext, *mode_ptr);
		if (*mode_ptr) {
			glue_pack(pos, msg, ext, **mode_ptr);
		}

	}

	glue_call_client(pos, msg, RPC_ID_devnode);

	*pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__devnode__device__in(pos, msg, ext, *dev_ptr);
		}

	}

	{
		(void)mode_ptr;
	}

	{
		*ret_ptr = glue_unpack_new_shadow(pos, msg, ext, char const*, sizeof(char) * glue_peek(pos, msg, ext));
		if (*ret_ptr) {
			char* writable = (char*)*ret_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(pos, msg, ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(pos, msg, ext, char);
			}

		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_devnode)
char const* LCD_TRAMPOLINE_LINKAGE(trmp_devnode) trmp_devnode(struct device* dev, unsigned short* mode)
{
	volatile fptr_impl_devnode impl;
	fptr_devnode target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_devnode);
	impl = trmp_impl_devnode;
	return impl(target, dev, mode);
}

void __device_create_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct class* class = 0;
	struct device* parent = 0;
	unsigned int devt = 0;
	void* drvdata = 0;
	char const* fmt = 0;
	unsigned int cpu = 0;
	struct class** class_ptr = &class;
	struct device** parent_ptr = &parent;
	unsigned int* devt_ptr = &devt;
	void** drvdata_ptr = &drvdata;
	char const** fmt_ptr = &fmt;
	unsigned int* cpu_ptr = &cpu;
	struct device* ret = 0;
	struct device** ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_dump_msg(msg);
	{
		*class_ptr = glue_unpack(pos, msg, ext, struct class*);
		if (*class_ptr) {
			callee_unmarshal_kernel____device_create__class__in(pos, msg, ext, *class_ptr);
		}

	}

	{
		*parent_ptr = glue_unpack_new_shadow(pos, msg, ext, struct device*, sizeof(struct device));
		if (*parent_ptr) {
			callee_unmarshal_kernel____device_create__parent__in(pos, msg, ext, *parent_ptr);
		}

	}

	{
		*devt_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	if (0)
	{
		*drvdata_ptr = glue_unpack(pos, msg, ext, void*);
		if (*drvdata_ptr) {
		}

	}

	{
		size_t cur_pos = *pos + 0;
		size_t sz = glue_peek(&cur_pos, msg, ext);
		printk("%s got sz %zu\n", __func__, sz);
		// 5
		*fmt_ptr = glue_unpack_new_shadow(pos, msg, ext, char const*, sizeof(char) * sz);
		if (*fmt_ptr) {
			char* writable = (char*)*fmt_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(pos, msg, ext, size_t) - 1;
			printk("%s, got len %zu\n", __func__, len);
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(pos, msg, ext, char);
			}

		}

	}

	{
		*cpu_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	ret = device_create(class, parent, devt, drvdata, fmt, cpu);

	*pos = 0;
	{
		if (*class_ptr) {
			callee_marshal_kernel____device_create__class__in(pos, msg, ext, *class_ptr);
		}

	}

	{
		if (*parent_ptr) {
			callee_marshal_kernel____device_create__parent__in(pos, msg, ext, *parent_ptr);
		}

	}

	{
	}

	{
		(void)drvdata_ptr;
	}

	{
		(void)fmt_ptr;
	}

	{
	}

	{
		glue_pack(pos, msg, ext, *ret_ptr);
		if (*ret_ptr) {
			callee_marshal_kernel____device_create__ret_device__out(pos, msg, ext, *ret_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void device_destroy_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct class* class = 0;
	unsigned int devt = 0;
	struct class** class_ptr = &class;
	unsigned int* devt_ptr = &devt;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*class_ptr = glue_unpack(pos, msg, ext, struct class*);
		if (*class_ptr) {
			callee_unmarshal_kernel__device_destroy__class__in(pos, msg, ext, *class_ptr);
		}

	}

	{
		*devt_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	device_destroy(class, devt);

	*pos = 0;
	{
		if (*class_ptr) {
			callee_marshal_kernel__device_destroy__class__in(pos, msg, ext, *class_ptr);
		}

	}

	{
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void class_destroy_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct class* cls = 0;
	struct class** cls_ptr = &cls;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*cls_ptr = glue_unpack(pos, msg, ext, struct class*);
		if (*cls_ptr) {
			callee_unmarshal_kernel__class_destroy__cls__in(pos, msg, ext, *cls_ptr);
		}

	}

	class_destroy(cls);

	*pos = 0;
	{
		if (*cls_ptr) {
			callee_marshal_kernel__class_destroy__cls__in(pos, msg, ext, *cls_ptr);
		}

	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext)
{
	switch(id) {
	case RPC_ID___register_chrdev:
		glue_user_trace("__register_chrdev\n");
		__register_chrdev_callee(msg, ext);
		break;

	case RPC_ID___unregister_chrdev:
		glue_user_trace("__unregister_chrdev\n");
		__unregister_chrdev_callee(msg, ext);
		break;

	case RPC_ID_cpu_maps_update_begin:
		glue_user_trace("cpu_maps_update_begin\n");
		cpu_maps_update_begin_callee(msg, ext);
		break;

	case RPC_ID_cpu_maps_update_done:
		glue_user_trace("cpu_maps_update_done\n");
		cpu_maps_update_done_callee(msg, ext);
		break;

	case RPC_ID_wrmsr_safe_regs_on_cpu:
		glue_user_trace("wrmsr_safe_regs_on_cpu\n");
		wrmsr_safe_regs_on_cpu_callee(msg, ext);
		break;

	case RPC_ID_rdmsr_safe_regs_on_cpu:
		glue_user_trace("rdmsr_safe_regs_on_cpu\n");
		rdmsr_safe_regs_on_cpu_callee(msg, ext);
		break;

	case RPC_ID_wrmsr_safe_on_cpu:
		glue_user_trace("wrmsr_safe_on_cpu\n");
		wrmsr_safe_on_cpu_callee(msg, ext);
		break;

	case RPC_ID_rdmsr_safe_on_cpu:
		glue_user_trace("rdmsr_safe_on_cpu\n");
		rdmsr_safe_on_cpu_callee(msg, ext);
		break;

	case RPC_ID_capable:
		glue_user_trace("capable\n");
		capable_callee(msg, ext);
		break;

	case RPC_ID_no_seek_end_llseek:
		glue_user_trace("no_seek_end_llseek\n");
		no_seek_end_llseek_callee(msg, ext);
		break;

	case RPC_ID___class_create:
		glue_user_trace("__class_create\n");
		__class_create_callee(msg, ext);
		break;

	case RPC_ID___device_create:
		glue_user_trace("__device_create\n");
		__device_create_callee(msg, ext);
		break;

	case RPC_ID_device_destroy:
		glue_user_trace("device_destroy\n");
		device_destroy_callee(msg, ext);
		break;

	case RPC_ID_class_destroy:
		glue_user_trace("class_destroy\n");
		class_destroy_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

