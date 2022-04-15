#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel____class_create__ret_class__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct class const* ptr)
{
	char const* const* name_ptr = &ptr->name;

	{
		(void)name_ptr;
	}

}

void callee_unmarshal_kernel____class_create__ret_class__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct class* ptr)
{
	char const** name_ptr = &ptr->name;

	{
		(void)name_ptr;
	}

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
	char const* const* name_ptr = &ptr->name;

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

}

void caller_unmarshal_kernel____class_create__ret_class__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct class* ptr)
{
	char const** name_ptr = &ptr->name;

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*name_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*name_ptr) {
			char* writable = (char*)*name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(__pos, __msg, __ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	{
	}
}

void caller_marshal_kernel____class_create__module__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct module const* ptr)
{

}

void callee_unmarshal_kernel____class_create__module__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct module* ptr)
{

	{
	}
}

void callee_marshal_kernel____class_create__module__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct module const* ptr)
{

}

void caller_unmarshal_kernel____class_create__module__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct module* ptr)
{

	{
	}
}

void caller_marshal_kernel____class_create__lock_class_key__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct lock_class_key const* ptr)
{

}

void callee_unmarshal_kernel____class_create__lock_class_key__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct lock_class_key* ptr)
{

	{
	}
}

void callee_marshal_kernel____class_create__lock_class_key__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct lock_class_key const* ptr)
{

}

void caller_unmarshal_kernel____class_create__lock_class_key__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __class_create_call_ctx const* ctx,
	struct lock_class_key* ptr)
{

	{
	}
}

void caller_marshal_kernel___global_file_operations__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct file_operations const* ptr)
{
	fptr_msr_ioctl const* compat_ioctl_ptr = &ptr->compat_ioctl;
	fptr_msr_ioctl const* unlocked_ioctl_ptr = &ptr->unlocked_ioctl;
	fptr_msr_open const* open_ptr = &ptr->open;
	fptr_msr_read const* read_ptr = &ptr->read;
	fptr_msr_write const* write_ptr = &ptr->write;
	fptr_llseek const* llseek_ptr = &ptr->llseek;

	{
		glue_pack(__pos, __msg, __ext, *compat_ioctl_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *unlocked_ioctl_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *open_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *read_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *write_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *llseek_ptr);
	}

}

void callee_unmarshal_kernel___global_file_operations__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct file_operations* ptr)
{
	fptr_msr_ioctl* compat_ioctl_ptr = &ptr->compat_ioctl;
	fptr_msr_ioctl* unlocked_ioctl_ptr = &ptr->unlocked_ioctl;
	fptr_msr_open* open_ptr = &ptr->open;
	fptr_msr_read* read_ptr = &ptr->read;
	fptr_msr_write* write_ptr = &ptr->write;
	fptr_llseek* llseek_ptr = &ptr->llseek;

	{
		*compat_ioctl_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, msr_ioctl);
	}

	{
		*unlocked_ioctl_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, msr_ioctl);
	}

	{
		*open_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, msr_open);
	}

	{
		*read_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, msr_read);
	}

	{
		*write_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, msr_write);
	}

	{
		*llseek_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, llseek);
	}

	{
	}
}

void callee_marshal_kernel___global_file_operations__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct file_operations const* ptr)
{

}

void caller_unmarshal_kernel___global_file_operations__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct file_operations* ptr)
{

	{
	}
}

void caller_marshal_kernel__class_destroy__class__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct class_destroy_call_ctx const* ctx,
	struct class const* ptr)
{

}

void callee_unmarshal_kernel__class_destroy__class__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct class_destroy_call_ctx const* ctx,
	struct class* ptr)
{

	{
	}
}

void callee_marshal_kernel__class_destroy__class__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct class_destroy_call_ctx const* ctx,
	struct class const* ptr)
{

}

void caller_unmarshal_kernel__class_destroy__class__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct class_destroy_call_ctx const* ctx,
	struct class* ptr)
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

void caller_marshal_kernel__device_create__ret_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct device_create_call_ctx const* ctx,
	struct device const* ptr)
{

}

void callee_unmarshal_kernel__device_create__ret_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct device_create_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void callee_marshal_kernel__device_create__ret_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct device_create_call_ctx const* ctx,
	struct device const* ptr)
{

}

void caller_unmarshal_kernel__device_create__ret_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct device_create_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void caller_marshal_kernel__device_create__class__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct device_create_call_ctx const* ctx,
	struct class const* ptr)
{
	fptr_devnode const* devnode_ptr = &ptr->devnode;

	{
		glue_pack(__pos, __msg, __ext, *devnode_ptr);
	}

}

void callee_unmarshal_kernel__device_create__class__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct device_create_call_ctx const* ctx,
	struct class* ptr)
{
	fptr_devnode* devnode_ptr = &ptr->devnode;

	{
		*devnode_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, devnode);
	}

	{
	}
}

void callee_marshal_kernel__device_create__class__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct device_create_call_ctx const* ctx,
	struct class const* ptr)
{

}

void caller_unmarshal_kernel__device_create__class__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct device_create_call_ctx const* ctx,
	struct class* ptr)
{

	{
	}
}

void caller_marshal_kernel__device_create__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct device_create_call_ctx const* ctx,
	struct device const* ptr)
{

}

void callee_unmarshal_kernel__device_create__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct device_create_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void callee_marshal_kernel__device_create__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct device_create_call_ctx const* ctx,
	struct device const* ptr)
{

}

void caller_unmarshal_kernel__device_create__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct device_create_call_ctx const* ctx,
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

void caller_marshal_kernel__no_seek_end_llseek__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct no_seek_end_llseek_call_ctx const* ctx,
	struct file const* ptr)
{
	unsigned int const* f_mode_ptr = &ptr->f_mode;
	unsigned long long const* f_pos_ptr = &ptr->f_pos;

	{
		glue_pack(__pos, __msg, __ext, *f_mode_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *f_pos_ptr);
	}

}

void callee_unmarshal_kernel__no_seek_end_llseek__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct no_seek_end_llseek_call_ctx const* ctx,
	struct file* ptr)
{
	unsigned int* f_mode_ptr = &ptr->f_mode;
	unsigned long long* f_pos_ptr = &ptr->f_pos;

	{
		*f_mode_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*f_pos_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

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
	unsigned long long const* f_pos_ptr = &ptr->f_pos;
	unsigned long long const* f_version_ptr = &ptr->f_version;

	{
		glue_pack(__pos, __msg, __ext, *f_pos_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *f_version_ptr);
	}

}

void caller_unmarshal_kernel__no_seek_end_llseek__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct no_seek_end_llseek_call_ctx const* ctx,
	struct file* ptr)
{
	unsigned long long* f_pos_ptr = &ptr->f_pos;
	unsigned long long* f_version_ptr = &ptr->f_version;

	{
		*f_pos_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*f_version_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void caller_marshal_kernel__llseek__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct llseek_call_ctx const* ctx,
	struct file const* ptr)
{
	unsigned int const* f_mode_ptr = &ptr->f_mode;
	unsigned long long const* f_pos_ptr = &ptr->f_pos;

	{
		glue_pack(__pos, __msg, __ext, *f_mode_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *f_pos_ptr);
	}

}

void callee_unmarshal_kernel__llseek__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct llseek_call_ctx const* ctx,
	struct file* ptr)
{
	unsigned int* f_mode_ptr = &ptr->f_mode;
	unsigned long long* f_pos_ptr = &ptr->f_pos;

	{
		*f_mode_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*f_pos_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void callee_marshal_kernel__llseek__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct llseek_call_ctx const* ctx,
	struct file const* ptr)
{
	unsigned long long const* f_pos_ptr = &ptr->f_pos;
	unsigned long long const* f_version_ptr = &ptr->f_version;

	{
		glue_pack(__pos, __msg, __ext, *f_pos_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *f_version_ptr);
	}

}

void caller_unmarshal_kernel__llseek__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct llseek_call_ctx const* ctx,
	struct file* ptr)
{
	unsigned long long* f_pos_ptr = &ptr->f_pos;
	unsigned long long* f_version_ptr = &ptr->f_version;

	{
		*f_pos_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*f_version_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void caller_marshal_kernel__msr_read__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_read_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;

	{
		__maybe_unused const void* __adjusted = *f_inode_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*f_inode_ptr) {
			caller_marshal_kernel__msr_read__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

}

void callee_unmarshal_kernel__msr_read__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_read_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;

	{
		size_t __size = sizeof(struct inode);
		*f_inode_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct inode*, (__size), (DEFAULT_GFP_FLAGS));
		if (*f_inode_ptr) {
			callee_unmarshal_kernel__msr_read__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__msr_read__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_read_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			callee_marshal_kernel__msr_read__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

}

void caller_unmarshal_kernel__msr_read__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_read_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			caller_unmarshal_kernel__msr_read__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__msr_read__file_inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_read_call_ctx const* ctx,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;

	{
		glue_pack(__pos, __msg, __ext, *i_rdev_ptr);
	}

}

void callee_unmarshal_kernel__msr_read__file_inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_read_call_ctx const* ctx,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;

	{
		*i_rdev_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__msr_read__file_inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_read_call_ctx const* ctx,
	struct inode const* ptr)
{

}

void caller_unmarshal_kernel__msr_read__file_inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_read_call_ctx const* ctx,
	struct inode* ptr)
{

	{
	}
}

void caller_marshal_kernel__msr_write__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_write_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;

	{
		__maybe_unused const void* __adjusted = *f_inode_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*f_inode_ptr) {
			caller_marshal_kernel__msr_write__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

}

void callee_unmarshal_kernel__msr_write__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_write_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;

	{
		size_t __size = sizeof(struct inode);
		*f_inode_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct inode*, (__size), (DEFAULT_GFP_FLAGS));
		if (*f_inode_ptr) {
			callee_unmarshal_kernel__msr_write__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__msr_write__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_write_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			callee_marshal_kernel__msr_write__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

}

void caller_unmarshal_kernel__msr_write__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_write_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			caller_unmarshal_kernel__msr_write__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__msr_write__file_inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_write_call_ctx const* ctx,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;

	{
		glue_pack(__pos, __msg, __ext, *i_rdev_ptr);
	}

}

void callee_unmarshal_kernel__msr_write__file_inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_write_call_ctx const* ctx,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;

	{
		*i_rdev_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__msr_write__file_inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_write_call_ctx const* ctx,
	struct inode const* ptr)
{

}

void caller_unmarshal_kernel__msr_write__file_inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_write_call_ctx const* ctx,
	struct inode* ptr)
{

	{
	}
}

void caller_marshal_kernel__msr_ioctl__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_ioctl_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;
	unsigned int const* f_mode_ptr = &ptr->f_mode;

	{
		__maybe_unused const void* __adjusted = *f_inode_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*f_inode_ptr) {
			caller_marshal_kernel__msr_ioctl__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *f_mode_ptr);
	}

}

void callee_unmarshal_kernel__msr_ioctl__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_ioctl_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;
	unsigned int* f_mode_ptr = &ptr->f_mode;

	{
		size_t __size = sizeof(struct inode);
		*f_inode_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct inode*, (__size), (DEFAULT_GFP_FLAGS));
		if (*f_inode_ptr) {
			callee_unmarshal_kernel__msr_ioctl__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
		*f_mode_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__msr_ioctl__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_ioctl_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			callee_marshal_kernel__msr_ioctl__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

}

void caller_unmarshal_kernel__msr_ioctl__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_ioctl_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			caller_unmarshal_kernel__msr_ioctl__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__msr_ioctl__file_inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_ioctl_call_ctx const* ctx,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;

	{
		glue_pack(__pos, __msg, __ext, *i_rdev_ptr);
	}

}

void callee_unmarshal_kernel__msr_ioctl__file_inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_ioctl_call_ctx const* ctx,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;

	{
		*i_rdev_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__msr_ioctl__file_inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_ioctl_call_ctx const* ctx,
	struct inode const* ptr)
{

}

void caller_unmarshal_kernel__msr_ioctl__file_inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_ioctl_call_ctx const* ctx,
	struct inode* ptr)
{

	{
	}
}

void caller_marshal_kernel__msr_open__inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_open_call_ctx const* ctx,
	struct inode const* ptr)
{

}

void callee_unmarshal_kernel__msr_open__inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_open_call_ctx const* ctx,
	struct inode* ptr)
{

	{
	}
}

void callee_marshal_kernel__msr_open__inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_open_call_ctx const* ctx,
	struct inode const* ptr)
{

}

void caller_unmarshal_kernel__msr_open__inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_open_call_ctx const* ctx,
	struct inode* ptr)
{

	{
	}
}

void caller_marshal_kernel__msr_open__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_open_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;

	{
		__maybe_unused const void* __adjusted = *f_inode_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*f_inode_ptr) {
			caller_marshal_kernel__msr_open__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

}

void callee_unmarshal_kernel__msr_open__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_open_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;

	{
		size_t __size = sizeof(struct inode);
		*f_inode_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct inode*, (__size), (DEFAULT_GFP_FLAGS));
		if (*f_inode_ptr) {
			callee_unmarshal_kernel__msr_open__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__msr_open__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_open_call_ctx const* ctx,
	struct file const* ptr)
{
	struct inode* const* f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			callee_marshal_kernel__msr_open__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

}

void caller_unmarshal_kernel__msr_open__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_open_call_ctx const* ctx,
	struct file* ptr)
{
	struct inode** f_inode_ptr = &ptr->f_inode;

	{
		if (*f_inode_ptr) {
			caller_unmarshal_kernel__msr_open__file_inode__in(__pos, __msg, __ext, ctx, *f_inode_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__msr_open__file_inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_open_call_ctx const* ctx,
	struct inode const* ptr)
{
	unsigned int const* i_rdev_ptr = &ptr->i_rdev;

	{
		glue_pack(__pos, __msg, __ext, *i_rdev_ptr);
	}

}

void callee_unmarshal_kernel__msr_open__file_inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_open_call_ctx const* ctx,
	struct inode* ptr)
{
	unsigned int* i_rdev_ptr = &ptr->i_rdev;

	{
		*i_rdev_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__msr_open__file_inode__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct msr_open_call_ctx const* ctx,
	struct inode const* ptr)
{

}

void caller_unmarshal_kernel__msr_open__file_inode__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct msr_open_call_ctx const* ctx,
	struct inode* ptr)
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

