#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel__read__file__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	
	{
		glue_pack(pos, msg, ext, *f_inode_ptr);
		if (*f_inode_ptr) {
			caller_marshal_kernel__read__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

}

void callee_unmarshal_kernel__read__file__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	
	{
		*f_inode_ptr = glue_unpack_new_shadow(pos, msg, ext, struct inode*, sizeof(struct inode));
		if (*f_inode_ptr) {
			callee_unmarshal_kernel__read__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

}

void callee_marshal_kernel__read__file__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	
	{
		if (*f_inode_ptr) {
			callee_marshal_kernel__read__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

}

void caller_unmarshal_kernel__read__file__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	
	{
		if (*f_inode_ptr) {
			caller_unmarshal_kernel__read__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

}

void caller_marshal_kernel__read__inode__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;
	
	{
		glue_pack(pos, msg, ext, *i_rdev_ptr);
	}

}

void callee_unmarshal_kernel__read__inode__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;
	
	{
		*i_rdev_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__read__inode__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct inode const* ptr)
{
	
}

void caller_unmarshal_kernel__read__inode__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct inode* ptr)
{
	
}

void caller_marshal_kernel__write__file__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	
	{
		glue_pack(pos, msg, ext, *f_inode_ptr);
		if (*f_inode_ptr) {
			caller_marshal_kernel__write__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

}

void callee_unmarshal_kernel__write__file__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	
	{
		*f_inode_ptr = glue_unpack_new_shadow(pos, msg, ext, struct inode*, sizeof(struct inode));
		if (*f_inode_ptr) {
			callee_unmarshal_kernel__write__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

}

void callee_marshal_kernel__write__file__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	
	{
		if (*f_inode_ptr) {
			callee_marshal_kernel__write__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

}

void caller_unmarshal_kernel__write__file__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	
	{
		if (*f_inode_ptr) {
			caller_unmarshal_kernel__write__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

}

void caller_marshal_kernel__write__inode__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;
	
	{
		glue_pack(pos, msg, ext, *i_rdev_ptr);
	}

}

void callee_unmarshal_kernel__write__inode__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;
	
	{
		*i_rdev_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__write__inode__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct inode const* ptr)
{
	
}

void caller_unmarshal_kernel__write__inode__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct inode* ptr)
{
	
}

void caller_marshal_kernel__compat_ioctl__file__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	unsigned int const* f_mode_ptr = &ptr->f_mode;
	
	{
		glue_pack(pos, msg, ext, *f_inode_ptr);
		if (*f_inode_ptr) {
			caller_marshal_kernel__compat_ioctl__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *f_mode_ptr);
	}

}

void callee_unmarshal_kernel__compat_ioctl__file__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	unsigned int* f_mode_ptr = &ptr->f_mode;
	
	{
		*f_inode_ptr = glue_unpack_new_shadow(pos, msg, ext, struct inode*, sizeof(struct inode));
		if (*f_inode_ptr) {
			callee_unmarshal_kernel__compat_ioctl__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

	{
		*f_mode_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__compat_ioctl__file__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	
	{
		if (*f_inode_ptr) {
			callee_marshal_kernel__compat_ioctl__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

}

void caller_unmarshal_kernel__compat_ioctl__file__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	
	{
		if (*f_inode_ptr) {
			caller_unmarshal_kernel__compat_ioctl__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

}

void caller_marshal_kernel__compat_ioctl__inode__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;
	
	{
		glue_pack(pos, msg, ext, *i_rdev_ptr);
	}

}

void callee_unmarshal_kernel__compat_ioctl__inode__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;
	
	{
		*i_rdev_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__compat_ioctl__inode__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct inode const* ptr)
{
	
}

void caller_unmarshal_kernel__compat_ioctl__inode__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct inode* ptr)
{
	
}

void caller_marshal_kernel__open__inode__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;
	
	{
		glue_pack(pos, msg, ext, *i_rdev_ptr);
	}

}

void callee_unmarshal_kernel__open__inode__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;
	
	{
		*i_rdev_ptr = glue_unpack(pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__open__inode__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct inode const* ptr)
{
	
}

void caller_unmarshal_kernel__open__inode__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct inode* ptr)
{
	
}

void caller_marshal_kernel__open__file__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	
	{
		glue_pack(pos, msg, ext, *f_inode_ptr);
		if (*f_inode_ptr) {
			caller_marshal_kernel__open__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

}

void callee_unmarshal_kernel__open__file__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	
	{
		*f_inode_ptr = glue_unpack_new_shadow(pos, msg, ext, struct inode*, sizeof(struct inode));
		if (*f_inode_ptr) {
			callee_unmarshal_kernel__open__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

}

void callee_marshal_kernel__open__file__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	
	{
		if (*f_inode_ptr) {
			callee_marshal_kernel__open__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

}

void caller_unmarshal_kernel__open__file__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	
	{
		if (*f_inode_ptr) {
			caller_unmarshal_kernel__open__inode__in(pos, msg, ext, *f_inode_ptr);
		}

	}

}

void caller_marshal_kernel____register_chrdev__fops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct file_operations const* ptr)
{
	struct module* const* owner_ptr = &ptr->owner;
	fptr_read const* read_ptr = &ptr->read;
	fptr_write const* write_ptr = &ptr->write;
	fptr_compat_ioctl const* compat_ioctl_ptr = &ptr->compat_ioctl;
	fptr_open const* open_ptr = &ptr->open;
	
	{
		glue_pack(pos, msg, ext, *owner_ptr);
		if (*owner_ptr) {
			caller_marshal_kernel____register_chrdev__owner__in(pos, msg, ext, *owner_ptr);
		}

	}

	{
		glue_pack(pos, msg, ext, *read_ptr);
	}

	{
		glue_pack(pos, msg, ext, *write_ptr);
	}

	{
		glue_pack(pos, msg, ext, *compat_ioctl_ptr);
	}

	{
		glue_pack(pos, msg, ext, *open_ptr);
	}

}

void callee_unmarshal_kernel____register_chrdev__fops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct file_operations* ptr)
{
	struct module** owner_ptr = &ptr->owner;
	fptr_read* read_ptr = &ptr->read;
	fptr_write* write_ptr = &ptr->write;
	fptr_compat_ioctl* compat_ioctl_ptr = &ptr->compat_ioctl;
	fptr_open* open_ptr = &ptr->open;
	
	{
		*owner_ptr = glue_unpack_new_shadow(pos, msg, ext, struct module*, sizeof(struct module));
		if (*owner_ptr) {
			callee_unmarshal_kernel____register_chrdev__owner__in(pos, msg, ext, *owner_ptr);
		}

	}

	{
		*read_ptr = glue_unpack_rpc_ptr(pos, msg, ext, read);
	}

	{
		*write_ptr = glue_unpack_rpc_ptr(pos, msg, ext, write);
	}

	{
		*compat_ioctl_ptr = glue_unpack_rpc_ptr(pos, msg, ext, compat_ioctl);
	}

	{
		*open_ptr = glue_unpack_rpc_ptr(pos, msg, ext, open);
	}

}

void callee_marshal_kernel____register_chrdev__fops__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct file_operations const* ptr)
{
	struct module* const* owner_ptr = &ptr->owner;
	
	{
		if (*owner_ptr) {
			callee_marshal_kernel____register_chrdev__owner__in(pos, msg, ext, *owner_ptr);
		}

	}

}

void caller_unmarshal_kernel____register_chrdev__fops__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct file_operations* ptr)
{
	struct module** owner_ptr = &ptr->owner;
	
	{
		if (*owner_ptr) {
			caller_unmarshal_kernel____register_chrdev__owner__in(pos, msg, ext, *owner_ptr);
		}

	}

}

void caller_marshal_kernel____register_chrdev__owner__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct module const* ptr)
{
	
}

void callee_unmarshal_kernel____register_chrdev__owner__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct module* ptr)
{
	
}

void callee_marshal_kernel____register_chrdev__owner__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct module const* ptr)
{
	
}

void caller_unmarshal_kernel____register_chrdev__owner__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct module* ptr)
{
	
}

void caller_marshal_kernel__no_seek_end_llseek__file__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct file const* ptr)
{
	
}

void callee_unmarshal_kernel__no_seek_end_llseek__file__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct file* ptr)
{
	
}

void callee_marshal_kernel__no_seek_end_llseek__file__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct file const* ptr)
{
	
}

void caller_unmarshal_kernel__no_seek_end_llseek__file__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct file* ptr)
{
	
}

void caller_marshal_kernel____class_create__ret_class__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct class const* ptr)
{
	
}

void callee_unmarshal_kernel____class_create__ret_class__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct class* ptr)
{
	
}

void callee_marshal_kernel____class_create__ret_class__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct class const* ptr)
{
	
}

void caller_unmarshal_kernel____class_create__ret_class__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct class* ptr)
{
	
}

void caller_marshal_kernel____class_create__owner__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct module const* ptr)
{
	
}

void callee_unmarshal_kernel____class_create__owner__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct module* ptr)
{
	
}

void callee_marshal_kernel____class_create__owner__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct module const* ptr)
{
	
}

void caller_unmarshal_kernel____class_create__owner__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct module* ptr)
{
	
}

void caller_marshal_kernel____device_create__ret_device__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device const* ptr)
{
	
}

void callee_unmarshal_kernel____device_create__ret_device__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device* ptr)
{
	
}

void callee_marshal_kernel____device_create__ret_device__out(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device const* ptr)
{
	
}

void caller_unmarshal_kernel____device_create__ret_device__out(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device* ptr)
{
	
}

void caller_marshal_kernel____device_create__class__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct class const* ptr)
{
	
}

void callee_unmarshal_kernel____device_create__class__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct class* ptr)
{
	
}

void callee_marshal_kernel____device_create__class__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct class const* ptr)
{
	
}

void caller_unmarshal_kernel____device_create__class__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct class* ptr)
{
	
}

void caller_marshal_kernel____device_create__parent__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device const* ptr)
{
	
}

void callee_unmarshal_kernel____device_create__parent__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device* ptr)
{
	
}

void callee_marshal_kernel____device_create__parent__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device const* ptr)
{
	
}

void caller_unmarshal_kernel____device_create__parent__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device* ptr)
{
	
}

void caller_marshal_kernel__device_destroy__class__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct class const* ptr)
{
	
}

void callee_unmarshal_kernel__device_destroy__class__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct class* ptr)
{
	
}

void callee_marshal_kernel__device_destroy__class__in(
	size_t* pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct class const* ptr)
{
	
}

void caller_unmarshal_kernel__device_destroy__class__in(
	size_t* pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct class* ptr)
{
	
}

