#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

struct glue_message shared_buffer = {0};

void caller_marshal_kernel__read__file__in(
	struct glue_message* msg,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	
	glue_pack_shadow(msg, *f_inode_ptr);
	if (*f_inode_ptr) {
		caller_marshal_kernel__read__inode__in(msg, *f_inode_ptr);
	}

}

void callee_unmarshal_kernel__read__file__in(
	struct glue_message* msg,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	
	*f_inode_ptr = glue_unpack(msg, struct inode*);
	if (*f_inode_ptr) {
		callee_unmarshal_kernel__read__inode__in(msg, *f_inode_ptr);
	}

}

void callee_marshal_kernel__read__file__in(
	struct glue_message* msg,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	
	if (*f_inode_ptr) {
		callee_marshal_kernel__read__inode__in(msg, *f_inode_ptr);
	}

}

void caller_unmarshal_kernel__read__file__in(
	struct glue_message* msg,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	
	if (*f_inode_ptr) {
		caller_unmarshal_kernel__read__inode__in(msg, *f_inode_ptr);
	}

}

void caller_marshal_kernel__read__inode__in(
	struct glue_message* msg,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;
	
	glue_pack(msg, *i_rdev_ptr);
}

void callee_unmarshal_kernel__read__inode__in(
	struct glue_message* msg,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;
	
	*i_rdev_ptr = glue_unpack(msg, unsigned int);
}

void callee_marshal_kernel__read__inode__in(
	struct glue_message* msg,
	struct inode const* ptr)
{
	
}

void caller_unmarshal_kernel__read__inode__in(
	struct glue_message* msg,
	struct inode* ptr)
{
	
}

void caller_marshal_kernel__write__file__in(
	struct glue_message* msg,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	
	glue_pack_shadow(msg, *f_inode_ptr);
	if (*f_inode_ptr) {
		caller_marshal_kernel__write__inode__in(msg, *f_inode_ptr);
	}

}

void callee_unmarshal_kernel__write__file__in(
	struct glue_message* msg,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	
	*f_inode_ptr = glue_unpack(msg, struct inode*);
	if (*f_inode_ptr) {
		callee_unmarshal_kernel__write__inode__in(msg, *f_inode_ptr);
	}

}

void callee_marshal_kernel__write__file__in(
	struct glue_message* msg,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	
	if (*f_inode_ptr) {
		callee_marshal_kernel__write__inode__in(msg, *f_inode_ptr);
	}

}

void caller_unmarshal_kernel__write__file__in(
	struct glue_message* msg,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	
	if (*f_inode_ptr) {
		caller_unmarshal_kernel__write__inode__in(msg, *f_inode_ptr);
	}

}

void caller_marshal_kernel__write__inode__in(
	struct glue_message* msg,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;
	
	glue_pack(msg, *i_rdev_ptr);
}

void callee_unmarshal_kernel__write__inode__in(
	struct glue_message* msg,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;
	
	*i_rdev_ptr = glue_unpack(msg, unsigned int);
}

void callee_marshal_kernel__write__inode__in(
	struct glue_message* msg,
	struct inode const* ptr)
{
	
}

void caller_unmarshal_kernel__write__inode__in(
	struct glue_message* msg,
	struct inode* ptr)
{
	
}

void caller_marshal_kernel__compat_ioctl__file__in(
	struct glue_message* msg,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	unsigned int const* f_mode_ptr = &ptr->f_mode;
	
	glue_pack_shadow(msg, *f_inode_ptr);
	if (*f_inode_ptr) {
		caller_marshal_kernel__compat_ioctl__inode__in(msg, *f_inode_ptr);
	}

	glue_pack(msg, *f_mode_ptr);
}

void callee_unmarshal_kernel__compat_ioctl__file__in(
	struct glue_message* msg,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	unsigned int* f_mode_ptr = &ptr->f_mode;
	
	*f_inode_ptr = glue_unpack(msg, struct inode*);
	if (*f_inode_ptr) {
		callee_unmarshal_kernel__compat_ioctl__inode__in(msg, *f_inode_ptr);
	}

	*f_mode_ptr = glue_unpack(msg, unsigned int);
}

void callee_marshal_kernel__compat_ioctl__file__in(
	struct glue_message* msg,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	
	if (*f_inode_ptr) {
		callee_marshal_kernel__compat_ioctl__inode__in(msg, *f_inode_ptr);
	}

}

void caller_unmarshal_kernel__compat_ioctl__file__in(
	struct glue_message* msg,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	
	if (*f_inode_ptr) {
		caller_unmarshal_kernel__compat_ioctl__inode__in(msg, *f_inode_ptr);
	}

}

void caller_marshal_kernel__compat_ioctl__inode__in(
	struct glue_message* msg,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;
	
	glue_pack(msg, *i_rdev_ptr);
}

void callee_unmarshal_kernel__compat_ioctl__inode__in(
	struct glue_message* msg,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;
	
	*i_rdev_ptr = glue_unpack(msg, unsigned int);
}

void callee_marshal_kernel__compat_ioctl__inode__in(
	struct glue_message* msg,
	struct inode const* ptr)
{
	
}

void caller_unmarshal_kernel__compat_ioctl__inode__in(
	struct glue_message* msg,
	struct inode* ptr)
{
	
}

void caller_marshal_kernel__open__inode__in(
	struct glue_message* msg,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;
	
	glue_pack(msg, *i_rdev_ptr);
}

void callee_unmarshal_kernel__open__inode__in(
	struct glue_message* msg,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;
	
	*i_rdev_ptr = glue_unpack(msg, unsigned int);
}

void callee_marshal_kernel__open__inode__in(
	struct glue_message* msg,
	struct inode const* ptr)
{
	
}

void caller_unmarshal_kernel__open__inode__in(
	struct glue_message* msg,
	struct inode* ptr)
{
	
}

void caller_marshal_kernel__open__file__in(
	struct glue_message* msg,
	struct file const* ptr)
{
	
}

void callee_unmarshal_kernel__open__file__in(
	struct glue_message* msg,
	struct file* ptr)
{
	
}

void callee_marshal_kernel__open__file__in(
	struct glue_message* msg,
	struct file const* ptr)
{
	
}

void caller_unmarshal_kernel__open__file__in(
	struct glue_message* msg,
	struct file* ptr)
{
	
}

void caller_marshal_kernel____register_chrdev__fops__in(
	struct glue_message* msg,
	struct file_operations const* ptr)
{
	struct module* const* owner_ptr = &ptr->owner;
	fptr_read const* read_ptr = &ptr->read;
	fptr_write const* write_ptr = &ptr->write;
	fptr_compat_ioctl const* compat_ioctl_ptr = &ptr->compat_ioctl;
	fptr_open const* open_ptr = &ptr->open;
	
	glue_pack_shadow(msg, *owner_ptr);
	if (*owner_ptr) {
		caller_marshal_kernel____register_chrdev__owner__in(msg, *owner_ptr);
	}

	glue_pack(msg, *read_ptr);
	glue_pack(msg, *write_ptr);
	glue_pack(msg, *compat_ioctl_ptr);
	glue_pack(msg, *open_ptr);
}

void callee_unmarshal_kernel____register_chrdev__fops__in(
	struct glue_message* msg,
	struct file_operations* ptr)
{
	struct module** owner_ptr = &ptr->owner;
	fptr_read* read_ptr = &ptr->read;
	fptr_write* write_ptr = &ptr->write;
	fptr_compat_ioctl* compat_ioctl_ptr = &ptr->compat_ioctl;
	fptr_open* open_ptr = &ptr->open;
	
	*owner_ptr = glue_unpack(msg, struct module*);
	if (*owner_ptr) {
		callee_unmarshal_kernel____register_chrdev__owner__in(msg, *owner_ptr);
	}

	*read_ptr = glue_unpack_rpc_ptr(msg, read);
	*write_ptr = glue_unpack_rpc_ptr(msg, write);
	*compat_ioctl_ptr = glue_unpack_rpc_ptr(msg, compat_ioctl);
	*open_ptr = glue_unpack_rpc_ptr(msg, open);
}

void callee_marshal_kernel____register_chrdev__fops__in(
	struct glue_message* msg,
	struct file_operations const* ptr)
{
	struct module* const* owner_ptr = &ptr->owner;
	
	if (*owner_ptr) {
		callee_marshal_kernel____register_chrdev__owner__in(msg, *owner_ptr);
	}

}

void caller_unmarshal_kernel____register_chrdev__fops__in(
	struct glue_message* msg,
	struct file_operations* ptr)
{
	struct module** owner_ptr = &ptr->owner;
	
	if (*owner_ptr) {
		caller_unmarshal_kernel____register_chrdev__owner__in(msg, *owner_ptr);
	}

}

void caller_marshal_kernel____register_chrdev__owner__in(
	struct glue_message* msg,
	struct module const* ptr)
{
	
}

void callee_unmarshal_kernel____register_chrdev__owner__in(
	struct glue_message* msg,
	struct module* ptr)
{
	
}

void callee_marshal_kernel____register_chrdev__owner__in(
	struct glue_message* msg,
	struct module const* ptr)
{
	
}

void caller_unmarshal_kernel____register_chrdev__owner__in(
	struct glue_message* msg,
	struct module* ptr)
{
	
}

void caller_marshal_kernel__no_seek_end_llseek__file__in(
	struct glue_message* msg,
	struct file const* ptr)
{
	
}

void callee_unmarshal_kernel__no_seek_end_llseek__file__in(
	struct glue_message* msg,
	struct file* ptr)
{
	
}

void callee_marshal_kernel__no_seek_end_llseek__file__in(
	struct glue_message* msg,
	struct file const* ptr)
{
	
}

void caller_unmarshal_kernel__no_seek_end_llseek__file__in(
	struct glue_message* msg,
	struct file* ptr)
{
	
}

