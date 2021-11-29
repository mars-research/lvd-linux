#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel__read__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct read_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;

	{
		__maybe_unused const void* __adjusted = *f_inode_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*f_inode_ptr) {
			caller_marshal_kernel__read__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

}

void callee_unmarshal_kernel__read__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct read_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;

	{
		size_t __size = sizeof(struct inode);
		*f_inode_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct inode*, __size, DEFAULT_GFP_FLAGS);
		if (*f_inode_ptr) {
			callee_unmarshal_kernel__read__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__read__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct read_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			callee_marshal_kernel__read__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

}

void caller_unmarshal_kernel__read__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct read_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			caller_unmarshal_kernel__read__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__read__inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct read_call_ctx const* ctx,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;

	{
		glue_pack(__pos, __msg, __ext, *i_rdev_ptr);
	}

}

void callee_unmarshal_kernel__read__inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct read_call_ctx const* ctx,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;

	{
		*i_rdev_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__read__inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct read_call_ctx const* ctx,
	struct inode const* ptr)
{

}

void caller_unmarshal_kernel__read__inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct read_call_ctx const* ctx,
	struct inode* ptr)
{

	{
	}
}

void caller_marshal_kernel__write__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct write_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;

	{
		__maybe_unused const void* __adjusted = *f_inode_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*f_inode_ptr) {
			caller_marshal_kernel__write__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

}

void callee_unmarshal_kernel__write__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct write_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;

	{
		size_t __size = sizeof(struct inode);
		*f_inode_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct inode*, (__size), (DEFAULT_GFP_FLAGS));
		if (*f_inode_ptr) {
			callee_unmarshal_kernel__write__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__write__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct write_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			callee_marshal_kernel__write__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

}

void caller_unmarshal_kernel__write__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct write_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			caller_unmarshal_kernel__write__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__write__inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct write_call_ctx const* ctx,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;

	{
		glue_pack(__pos, __msg, __ext, *i_rdev_ptr);
	}

}

void callee_unmarshal_kernel__write__inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct write_call_ctx const* ctx,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;

	{
		*i_rdev_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__write__inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct write_call_ctx const* ctx,
	struct inode const* ptr)
{

}

void caller_unmarshal_kernel__write__inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct write_call_ctx const* ctx,
	struct inode* ptr)
{

	{
	}
}

void caller_marshal_kernel__compat_ioctl__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct compat_ioctl_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	unsigned int const* f_mode_ptr = &ptr->f_mode;

	{
		__maybe_unused const void* __adjusted = *f_inode_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*f_inode_ptr) {
			caller_marshal_kernel__compat_ioctl__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *f_mode_ptr);
	}

}

void callee_unmarshal_kernel__compat_ioctl__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct compat_ioctl_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	unsigned int* f_mode_ptr = &ptr->f_mode;

	{
		size_t __size = sizeof(struct inode);
		*f_inode_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct inode*, (__size), (DEFAULT_GFP_FLAGS));
		if (*f_inode_ptr) {
			callee_unmarshal_kernel__compat_ioctl__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
		*f_mode_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__compat_ioctl__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct compat_ioctl_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			callee_marshal_kernel__compat_ioctl__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

}

void caller_unmarshal_kernel__compat_ioctl__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct compat_ioctl_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			caller_unmarshal_kernel__compat_ioctl__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__compat_ioctl__inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct compat_ioctl_call_ctx const* ctx,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;

	{
		glue_pack(__pos, __msg, __ext, *i_rdev_ptr);
	}

}

void callee_unmarshal_kernel__compat_ioctl__inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct compat_ioctl_call_ctx const* ctx,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;

	{
		*i_rdev_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__compat_ioctl__inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct compat_ioctl_call_ctx const* ctx,
	struct inode const* ptr)
{

}

void caller_unmarshal_kernel__compat_ioctl__inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct compat_ioctl_call_ctx const* ctx,
	struct inode* ptr)
{

	{
	}
}

void caller_marshal_kernel__open__inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct open_call_ctx const* ctx,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;

	{
		glue_pack(__pos, __msg, __ext, *i_rdev_ptr);
	}

}

void callee_unmarshal_kernel__open__inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct open_call_ctx const* ctx,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;

	{
		*i_rdev_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__open__inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct open_call_ctx const* ctx,
	struct inode const* ptr)
{

}

void caller_unmarshal_kernel__open__inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct open_call_ctx const* ctx,
	struct inode* ptr)
{

	{
	}
}

void caller_marshal_kernel__open__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct open_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;

	{
		__maybe_unused const void* __adjusted = *f_inode_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*f_inode_ptr) {
			caller_marshal_kernel__open__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

}

void callee_unmarshal_kernel__open__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct open_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;

	{
		size_t __size = sizeof(struct inode);
		*f_inode_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct inode*, (__size), (DEFAULT_GFP_FLAGS));
		if (*f_inode_ptr) {
			callee_unmarshal_kernel__open__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__open__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct open_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			callee_marshal_kernel__open__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

}

void caller_unmarshal_kernel__open__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct open_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			caller_unmarshal_kernel__open__inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel____register_chrdev__fops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __register_chrdev_call_ctx const* ctx,
	struct file_operations const* ptr)
{
	struct module* const* owner_ptr = &ptr->owner;
	fptr_read const* read_ptr = &ptr->read;
	fptr_write const* write_ptr = &ptr->write;
	fptr_compat_ioctl const* compat_ioctl_ptr = &ptr->compat_ioctl;
	fptr_open const* open_ptr = &ptr->open;

	{
		__maybe_unused const void* __adjusted = *owner_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*owner_ptr) {
			caller_marshal_kernel____register_chrdev__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *read_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *write_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *compat_ioctl_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *open_ptr);
	}

}

void callee_unmarshal_kernel____register_chrdev__fops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __register_chrdev_call_ctx const* ctx,
	struct file_operations* ptr)
{
	struct module** owner_ptr = &ptr->owner;
	fptr_read* read_ptr = &ptr->read;
	fptr_write* write_ptr = &ptr->write;
	fptr_compat_ioctl* compat_ioctl_ptr = &ptr->compat_ioctl;
	fptr_open* open_ptr = &ptr->open;

	{
		size_t __size = sizeof(struct module);
		*owner_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct module*, (__size), (DEFAULT_GFP_FLAGS));
		if (*owner_ptr) {
			callee_unmarshal_kernel____register_chrdev__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

	{
		*read_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, read);
	}

	{
		*write_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, write);
	}

	{
		*compat_ioctl_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, compat_ioctl);
	}

	{
		*open_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, open);
	}

	{
	}
}

void callee_marshal_kernel____register_chrdev__fops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __register_chrdev_call_ctx const* ctx,
	struct file_operations const* ptr)
{
	struct module* const* owner_ptr = &ptr->owner;

	{
		if (*owner_ptr) {
			callee_marshal_kernel____register_chrdev__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

}

void caller_unmarshal_kernel____register_chrdev__fops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __register_chrdev_call_ctx const* ctx,
	struct file_operations* ptr)
{
	struct module** owner_ptr = &ptr->owner;

	{
		if (*owner_ptr) {
			caller_unmarshal_kernel____register_chrdev__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel____register_chrdev__owner__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __register_chrdev_call_ctx const* ctx,
	struct module const* ptr)
{

}

void callee_unmarshal_kernel____register_chrdev__owner__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __register_chrdev_call_ctx const* ctx,
	struct module* ptr)
{

	{
	}
}

void callee_marshal_kernel____register_chrdev__owner__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __register_chrdev_call_ctx const* ctx,
	struct module const* ptr)
{

}

void caller_unmarshal_kernel____register_chrdev__owner__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __register_chrdev_call_ctx const* ctx,
	struct module* ptr)
{

	{
	}
}

void caller_marshal_kernel__no_seek_end_llseek__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct no_seek_end_llseek_call_ctx const* ctx,
	struct file const* ptr)
{

}

void callee_unmarshal_kernel__no_seek_end_llseek__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct no_seek_end_llseek_call_ctx const* ctx,
	struct file* ptr)
{

	{
	}
}

void callee_marshal_kernel__no_seek_end_llseek__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct no_seek_end_llseek_call_ctx const* ctx,
	struct file const* ptr)
{

}

void caller_unmarshal_kernel__no_seek_end_llseek__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct no_seek_end_llseek_call_ctx const* ctx,
	struct file* ptr)
{

	{
	}
}

void caller_marshal_kernel____class_create__ret_class__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct class const* ptr)
{

}

void callee_unmarshal_kernel____class_create__ret_class__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct class* ptr)
{

	{
	}
}

void callee_marshal_kernel____class_create__ret_class__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct class const* ptr)
{

}

void caller_unmarshal_kernel____class_create__ret_class__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct class* ptr)
{

	{
	}
}

void caller_marshal_kernel____class_create__owner__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct module const* ptr)
{

}

void callee_unmarshal_kernel____class_create__owner__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct module* ptr)
{

	{
	}
}

void callee_marshal_kernel____class_create__owner__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct module const* ptr)
{

}

void caller_unmarshal_kernel____class_create__owner__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct module* ptr)
{

	{
	}
}

void caller_marshal_kernel__devnode__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct devnode_call_ctx const* ctx,
	struct device const* ptr)
{
	unsigned int const* devt_ptr = &ptr->devt;

	{
		glue_pack(__pos, __msg, __ext, *devt_ptr);
	}

}

void callee_unmarshal_kernel__devnode__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct devnode_call_ctx const* ctx,
	struct device* ptr)
{
	unsigned int* devt_ptr = &ptr->devt;

	{
		*devt_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__devnode__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct devnode_call_ctx const* ctx,
	struct device const* ptr)
{

}

void caller_unmarshal_kernel__devnode__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct devnode_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void caller_marshal_kernel____device_create__ret_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __device_create_call_ctx const* ctx,
	struct device const* ptr)
{

}

void callee_unmarshal_kernel____device_create__ret_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __device_create_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void callee_marshal_kernel____device_create__ret_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __device_create_call_ctx const* ctx,
	struct device const* ptr)
{

}

void caller_unmarshal_kernel____device_create__ret_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __device_create_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void caller_marshal_kernel____device_create__class__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __device_create_call_ctx const* ctx,
	struct class const* ptr)
{
	fptr_devnode const* devnode_ptr = &ptr->devnode;

	{
		glue_pack(__pos, __msg, __ext, *devnode_ptr);
	}

}

void callee_unmarshal_kernel____device_create__class__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __device_create_call_ctx const* ctx,
	struct class* ptr)
{
	fptr_devnode* devnode_ptr = &ptr->devnode;

	{
		*devnode_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, devnode);
	}

	{
	}
}

void callee_marshal_kernel____device_create__class__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __device_create_call_ctx const* ctx,
	struct class const* ptr)
{

}

void caller_unmarshal_kernel____device_create__class__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __device_create_call_ctx const* ctx,
	struct class* ptr)
{

	{
	}
}

void caller_marshal_kernel____device_create__parent__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __device_create_call_ctx const* ctx,
	struct device const* ptr)
{

}

void callee_unmarshal_kernel____device_create__parent__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __device_create_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void callee_marshal_kernel____device_create__parent__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __device_create_call_ctx const* ctx,
	struct device const* ptr)
{

}

void caller_unmarshal_kernel____device_create__parent__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __device_create_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void caller_marshal_kernel__device_destroy__class__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct device_destroy_call_ctx const* ctx,
	struct class const* ptr)
{

}

void callee_unmarshal_kernel__device_destroy__class__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct device_destroy_call_ctx const* ctx,
	struct class* ptr)
{

	{
	}
}

void callee_marshal_kernel__device_destroy__class__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct device_destroy_call_ctx const* ctx,
	struct class const* ptr)
{

}

void caller_unmarshal_kernel__device_destroy__class__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct device_destroy_call_ctx const* ctx,
	struct class* ptr)
{

	{
	}
}

void caller_marshal_kernel__class_destroy__cls__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct class_destroy_call_ctx const* ctx,
	struct class const* ptr)
{

}

void callee_unmarshal_kernel__class_destroy__cls__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct class_destroy_call_ctx const* ctx,
	struct class* ptr)
{

	{
	}
}

void callee_marshal_kernel__class_destroy__cls__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct class_destroy_call_ctx const* ctx,
	struct class const* ptr)
{

}

void caller_unmarshal_kernel__class_destroy__cls__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct class_destroy_call_ctx const* ctx,
	struct class* ptr)
{

	{
	}
}


#ifdef LCD_ISOLATE
__attribute__((weak)) void shared_mem_init(void) {
	LIBLCD_MSG("Weak shared_mem_init does nothing! Override if you want!");
}
#else
__attribute__((weak)) void shared_mem_init_callee(struct fipc_message *__msg, struct ext_registers* __ext) {
	LIBLCD_MSG("Weak shared_mem_init_callee does nothing! Override if you want!");
}
#endif	/* LCD_ISOLATE */

