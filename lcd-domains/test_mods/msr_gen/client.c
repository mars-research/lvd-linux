#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void read_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	fptr_read function_ptr = glue_unpack(pos, msg, ext, fptr_read);
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

	{
		*file_ptr = glue_unpack_new_shadow(pos, msg, ext, struct file*, sizeof(struct file));
		if (*file_ptr) {
			callee_unmarshal_kernel__read__file__in(pos, msg, ext, *file_ptr);
		}

	}

	{
		*buf_ptr = glue_unpack(pos, msg, ext, char*);
		if (*buf_ptr) {
			**buf_ptr = glue_unpack(pos, msg, ext, char);
		}

	}

	{
		*count_ptr = glue_unpack(pos, msg, ext, unsigned long);
	}

	{
		*ppos_ptr = glue_unpack(pos, msg, ext, long long*);
		if (*ppos_ptr) {
			**ppos_ptr = glue_unpack(pos, msg, ext, long long);
		}

	}

	ret = function_ptr(file, buf, count, ppos);

	*pos = 0;
	{
		if (*file_ptr) {
			callee_marshal_kernel__read__file__in(pos, msg, ext, *file_ptr);
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
		glue_pack(pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void write_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	fptr_write function_ptr = glue_unpack(pos, msg, ext, fptr_write);
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

	{
		*file_ptr = glue_unpack_new_shadow(pos, msg, ext, struct file*, sizeof(struct file));
		if (*file_ptr) {
			callee_unmarshal_kernel__write__file__in(pos, msg, ext, *file_ptr);
		}

	}

	{
		*buf_ptr = glue_unpack(pos, msg, ext, char const*);
		if (*buf_ptr) {
			char* writable = (char*)*buf_ptr;
			*writable = glue_unpack(pos, msg, ext, char);
		}

	}

	{
		*count_ptr = glue_unpack(pos, msg, ext, unsigned long);
	}

	{
		*ppos_ptr = glue_unpack(pos, msg, ext, long long*);
		if (*ppos_ptr) {
			**ppos_ptr = glue_unpack(pos, msg, ext, long long);
		}

	}

	ret = function_ptr(file, buf, count, ppos);

	*pos = 0;
	{
		if (*file_ptr) {
			callee_marshal_kernel__write__file__in(pos, msg, ext, *file_ptr);
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
		glue_pack(pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void compat_ioctl_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	fptr_compat_ioctl function_ptr = glue_unpack(pos, msg, ext, fptr_compat_ioctl);
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

	{
		*file_ptr = glue_unpack_new_shadow(pos, msg, ext, struct file*, sizeof(struct file));
		if (*file_ptr) {
			callee_unmarshal_kernel__compat_ioctl__file__in(pos, msg, ext, *file_ptr);
		}

	}

	{
		*ioc_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

	{
		*arg_ptr = glue_unpack(pos, msg, ext, unsigned long);
	}

	ret = function_ptr(file, ioc, arg);

	*pos = 0;
	{
		if (*file_ptr) {
			callee_marshal_kernel__compat_ioctl__file__in(pos, msg, ext, *file_ptr);
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

void open_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	fptr_open function_ptr = glue_unpack(pos, msg, ext, fptr_open);
	struct inode* inode = 0;
	struct file* file = 0;
	struct inode** inode_ptr = &inode;
	struct file** file_ptr = &file;
	int ret = 0;
	int* ret_ptr = &ret;
	
	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*inode_ptr = glue_unpack_new_shadow(pos, msg, ext, struct inode*, sizeof(struct inode));
		if (*inode_ptr) {
			callee_unmarshal_kernel__open__inode__in(pos, msg, ext, *inode_ptr);
		}

	}

	{
		*file_ptr = glue_unpack_new_shadow(pos, msg, ext, struct file*, sizeof(struct file));
		if (*file_ptr) {
			callee_unmarshal_kernel__open__file__in(pos, msg, ext, *file_ptr);
		}

	}

	ret = function_ptr(inode, file);

	*pos = 0;
	{
		if (*inode_ptr) {
			callee_marshal_kernel__open__inode__in(pos, msg, ext, *inode_ptr);
		}

	}

	{
		if (*file_ptr) {
			callee_marshal_kernel__open__file__in(pos, msg, ext, *file_ptr);
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

int __register_chrdev(unsigned int major, unsigned int baseminor, unsigned int count, char const* name, struct file_operations const* fops)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	unsigned int* major_ptr = &major;
	unsigned int* baseminor_ptr = &baseminor;
	unsigned int* count_ptr = &count;
	char const** name_ptr = &name;
	struct file_operations const** fops_ptr = &fops;
	int ret = 0;
	int* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(pos, msg, ext, *major_ptr);
	}

	{
		glue_pack(pos, msg, ext, *baseminor_ptr);
	}

	{
		glue_pack(pos, msg, ext, *count_ptr);
	}

	{
		glue_pack(pos, msg, ext, *name_ptr);
		if (*name_ptr) {
			size_t i, len;
			char const* array = *name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(pos, msg, ext, *element);
			}

		}

	}

	{
		glue_pack(pos, msg, ext, *fops_ptr);
		if (*fops_ptr) {
			caller_marshal_kernel____register_chrdev__fops__in(pos, msg, ext, *fops_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID___register_chrdev);

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
			struct file_operations* writable = (struct file_operations*)*fops_ptr;
			caller_unmarshal_kernel____register_chrdev__fops__in(pos, msg, ext, writable);
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

void __unregister_chrdev(unsigned int major, unsigned int baseminor, unsigned int count, char const* name)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	unsigned int* major_ptr = &major;
	unsigned int* baseminor_ptr = &baseminor;
	unsigned int* count_ptr = &count;
	char const** name_ptr = &name;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(pos, msg, ext, *major_ptr);
	}

	{
		glue_pack(pos, msg, ext, *baseminor_ptr);
	}

	{
		glue_pack(pos, msg, ext, *count_ptr);
	}

	{
		glue_pack_shadow(pos, msg, ext, *name_ptr);
		if (*name_ptr) {
			glue_pack(pos, msg, ext, **name_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID___unregister_chrdev);

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

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void cpu_maps_update_begin(void)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(pos, msg, RPC_ID_cpu_maps_update_begin);

	*pos = 0;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void cpu_maps_update_done(void)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(pos, msg, RPC_ID_cpu_maps_update_done);

	*pos = 0;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int wrmsr_safe_regs_on_cpu(unsigned int cpu, unsigned int* regs)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	unsigned int* cpu_ptr = &cpu;
	unsigned int** regs_ptr = &regs;
	int ret = 0;
	int* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(pos, msg, ext, *cpu_ptr);
	}

	{
		glue_pack_shadow(pos, msg, ext, *regs_ptr);
		if (*regs_ptr) {
			size_t i, len = 8;
			unsigned int const* array = *regs_ptr;
			glue_pack(pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned int const* element = &array[i];
				glue_pack(pos, msg, ext, *element);
			}

		}

	}

	glue_call_server(pos, msg, RPC_ID_wrmsr_safe_regs_on_cpu);

	*pos = 0;
	{
	}

	{
		(void)regs_ptr;
	}

	{
		*ret_ptr = glue_unpack(pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int rdmsr_safe_regs_on_cpu(unsigned int cpu, unsigned int* regs)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	unsigned int* cpu_ptr = &cpu;
	unsigned int** regs_ptr = &regs;
	int ret = 0;
	int* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(pos, msg, ext, *cpu_ptr);
	}

	{
		glue_pack_shadow(pos, msg, ext, *regs_ptr);
		if (*regs_ptr) {
			size_t i, len = 8;
			unsigned int const* array = *regs_ptr;
			glue_pack(pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned int const* element = &array[i];
				glue_pack(pos, msg, ext, *element);
			}

		}

	}

	glue_call_server(pos, msg, RPC_ID_rdmsr_safe_regs_on_cpu);

	*pos = 0;
	{
	}

	{
		(void)regs_ptr;
	}

	{
		*ret_ptr = glue_unpack(pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int wrmsr_safe_on_cpu(unsigned int cpu, unsigned int msr_no, unsigned int l, unsigned int h)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	unsigned int* cpu_ptr = &cpu;
	unsigned int* msr_no_ptr = &msr_no;
	unsigned int* l_ptr = &l;
	unsigned int* h_ptr = &h;
	int ret = 0;
	int* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(pos, msg, ext, *cpu_ptr);
	}

	{
		glue_pack(pos, msg, ext, *msr_no_ptr);
	}

	{
		glue_pack(pos, msg, ext, *l_ptr);
	}

	{
		glue_pack(pos, msg, ext, *h_ptr);
	}

	glue_call_server(pos, msg, RPC_ID_wrmsr_safe_on_cpu);

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
		*ret_ptr = glue_unpack(pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int rdmsr_safe_on_cpu(unsigned int cpu, unsigned int msr_no, unsigned int* l, unsigned int* h)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	unsigned int* cpu_ptr = &cpu;
	unsigned int* msr_no_ptr = &msr_no;
	unsigned int** l_ptr = &l;
	unsigned int** h_ptr = &h;
	int ret = 0;
	int* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(pos, msg, ext, *cpu_ptr);
	}

	{
		glue_pack(pos, msg, ext, *msr_no_ptr);
	}

	{
		glue_pack_shadow(pos, msg, ext, *l_ptr);
		if (*l_ptr) {
			glue_pack(pos, msg, ext, **l_ptr);
		}

	}

	{
		glue_pack_shadow(pos, msg, ext, *h_ptr);
		if (*h_ptr) {
			glue_pack(pos, msg, ext, **h_ptr);
		}

	}

	glue_call_server(pos, msg, RPC_ID_rdmsr_safe_on_cpu);

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
		*ret_ptr = glue_unpack(pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

bool capable(int cap)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	int* cap_ptr = &cap;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(pos, msg, ext, *cap_ptr);
	}

	glue_call_server(pos, msg, RPC_ID_capable);

	*pos = 0;
	{
	}

	{
		*ret_ptr = glue_unpack(pos, msg, ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

long long no_seek_end_llseek(struct file* file, long long offset, int whence)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct file** file_ptr = &file;
	long long* offset_ptr = &offset;
	int* whence_ptr = &whence;
	long long ret = 0;
	long long* ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack_shadow(pos, msg, ext, *file_ptr);
		if (*file_ptr) {
			caller_marshal_kernel__no_seek_end_llseek__file__in(pos, msg, ext, *file_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *offset_ptr);
	}

	{
		glue_pack(pos, msg, ext, *whence_ptr);
	}

	glue_call_server(pos, msg, RPC_ID_no_seek_end_llseek);

	*pos = 0;
	{
		if (*file_ptr) {
			caller_unmarshal_kernel__no_seek_end_llseek__file__in(pos, msg, ext, *file_ptr);
		}

	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(pos, msg, ext, long long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct class* __class_create(struct module* owner, char const* name, struct lock_class_key *key)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct module** owner_ptr = &owner;
	char const** name_ptr = &name;
	struct class* ret = 0;
	struct class** ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(pos, msg, ext, *owner_ptr);
		if (*owner_ptr) {
			caller_marshal_kernel____class_create__owner__in(pos, msg, ext, *owner_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *name_ptr);
		if (*name_ptr) {
			size_t i, len;
			char const* array = *name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(pos, msg, ext, *element);
			}

		}

	}

	glue_call_server(pos, msg, RPC_ID___class_create);

	*pos = 0;
	{
		if (*owner_ptr) {
			caller_unmarshal_kernel____class_create__owner__in(pos, msg, ext, *owner_ptr);
		}

	}

	{
		(void)name_ptr;
	}

	{
		*ret_ptr = glue_unpack_shadow(pos, msg, ext, struct class*);
		if (*ret_ptr) {
			caller_unmarshal_kernel____class_create__ret_class__out(pos, msg, ext, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct device* __device_create(struct class* class, struct device* parent, unsigned int devt, void* drvdata, char const* fmt, unsigned int cpu)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct class** class_ptr = &class;
	struct device** parent_ptr = &parent;
	unsigned int* devt_ptr = &devt;
	void** drvdata_ptr = &drvdata;
	char const** fmt_ptr = &fmt;
	unsigned int* cpu_ptr = &cpu;
	struct device* ret = 0;
	struct device** ret_ptr = &ret;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack_shadow(pos, msg, ext, *class_ptr);
		if (*class_ptr) {
			caller_marshal_kernel____device_create__class__in(pos, msg, ext, *class_ptr);
		}

	}

	{
		glue_pack_shadow(pos, msg, ext, *parent_ptr);
		if (*parent_ptr) {
			caller_marshal_kernel____device_create__parent__in(pos, msg, ext, *parent_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *devt_ptr);
	}

	{
		glue_pack_shadow(pos, msg, ext, *drvdata_ptr);
		if (*drvdata_ptr) {
		}

	}

	{
		glue_pack(pos, msg, ext, *fmt_ptr);
		if (*fmt_ptr) {
			size_t i, len;
			char const* array = *fmt_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(pos, msg, ext, *element);
			}

		}

	}

	{
		glue_pack(pos, msg, ext, *cpu_ptr);
	}

	glue_call_server(pos, msg, RPC_ID___device_create);

	*pos = 0;
	{
		if (*class_ptr) {
			caller_unmarshal_kernel____device_create__class__in(pos, msg, ext, *class_ptr);
		}

	}

	{
		if (*parent_ptr) {
			caller_unmarshal_kernel____device_create__parent__in(pos, msg, ext, *parent_ptr);
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
		*ret_ptr = glue_unpack_new_shadow(pos, msg, ext, struct device*, sizeof(struct device));
		if (*ret_ptr) {
			caller_unmarshal_kernel____device_create__ret_device__out(pos, msg, ext, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void device_destroy(struct class* class, unsigned int devt)
{
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	struct class** class_ptr = &class;
	unsigned int* devt_ptr = &devt;
	
	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack_shadow(pos, msg, ext, *class_ptr);
		if (*class_ptr) {
			caller_marshal_kernel__device_destroy__class__in(pos, msg, ext, *class_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *devt_ptr);
	}

	glue_call_server(pos, msg, RPC_ID_device_destroy);

	*pos = 0;
	{
		if (*class_ptr) {
			caller_unmarshal_kernel__device_destroy__class__in(pos, msg, ext, *class_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext)
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
		read_callee(msg, ext);
		break;

	case RPC_ID_write:
		glue_user_trace("write\n");
		write_callee(msg, ext);
		break;

	case RPC_ID_compat_ioctl:
		glue_user_trace("compat_ioctl\n");
		compat_ioctl_callee(msg, ext);
		break;

	case RPC_ID_open:
		glue_user_trace("open\n");
		open_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

