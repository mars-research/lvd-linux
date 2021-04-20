#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel__sysfs_remove_group__kobj__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* ctx,
	struct kobject const* ptr)
{
	
}

void callee_unmarshal_kernel__sysfs_remove_group__kobj__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* ctx,
	struct kobject* ptr)
{
	
}

void callee_marshal_kernel__sysfs_remove_group__kobj__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* ctx,
	struct kobject const* ptr)
{
	
}

void caller_unmarshal_kernel__sysfs_remove_group__kobj__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* ctx,
	struct kobject* ptr)
{
	
}

void caller_marshal_kernel__sysfs_remove_group__grp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* ctx,
	struct attribute_group const* ptr)
{
	char* const* name_ptr = &ptr->name;
	
	{
		const void* __adjusted = *name_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*name_ptr) {
			glue_pack(__pos, msg, ext, **name_ptr);
		}

	}

}

void callee_unmarshal_kernel__sysfs_remove_group__grp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* ctx,
	struct attribute_group* ptr)
{
	char** name_ptr = &ptr->name;
	
	{
		*name_ptr = glue_unpack(__pos, msg, ext, char*);
		if (*name_ptr) {
			**name_ptr = glue_unpack(__pos, msg, ext, char);
		}

	}

}

void callee_marshal_kernel__sysfs_remove_group__grp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* ctx,
	struct attribute_group const* ptr)
{
	char* const* name_ptr = &ptr->name;
	
	{
		(void)name_ptr;
	}

}

void caller_unmarshal_kernel__sysfs_remove_group__grp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* ctx,
	struct attribute_group* ptr)
{
	char** name_ptr = &ptr->name;
	
	{
		(void)name_ptr;
	}

}

void caller_marshal_kernel__probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	int const* id_ptr = &ptr->id;
	struct device const* dev_ptr = &ptr->dev;
	
	{
		glue_pack(__pos, msg, ext, *id_ptr);
	}

	{
		caller_marshal_kernel__probe__device__in(__pos, msg, ext, ctx, dev_ptr);
	}

}

void callee_unmarshal_kernel__probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct platform_device* ptr)
{
	int* id_ptr = &ptr->id;
	struct device* dev_ptr = &ptr->dev;
	
	{
		*id_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		callee_unmarshal_kernel__probe__device__in(__pos, msg, ext, ctx, dev_ptr);
	}

}

void callee_marshal_kernel__probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	struct device const* dev_ptr = &ptr->dev;
	
	{
		callee_marshal_kernel__probe__device__in(__pos, msg, ext, ctx, dev_ptr);
	}

}

void caller_unmarshal_kernel__probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct platform_device* ptr)
{
	struct device* dev_ptr = &ptr->dev;
	
	{
		caller_unmarshal_kernel__probe__device__in(__pos, msg, ext, ctx, dev_ptr);
	}

}

void caller_marshal_kernel__probe__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void callee_unmarshal_kernel__probe__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void callee_marshal_kernel__probe__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void caller_unmarshal_kernel__probe__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void caller_marshal_kernel__remove__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct remove_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	
}

void callee_unmarshal_kernel__remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct remove_call_ctx const* ctx,
	struct platform_device* ptr)
{
	
}

void callee_marshal_kernel__remove__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct remove_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	
}

void caller_unmarshal_kernel__remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct remove_call_ctx const* ctx,
	struct platform_device* ptr)
{
	
}

void caller_marshal_kernel__sysfs_create_group__kobj__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct kobject const* ptr)
{
	
}

void callee_unmarshal_kernel__sysfs_create_group__kobj__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct kobject* ptr)
{
	
}

void callee_marshal_kernel__sysfs_create_group__kobj__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct kobject const* ptr)
{
	
}

void caller_unmarshal_kernel__sysfs_create_group__kobj__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct kobject* ptr)
{
	
}

void caller_marshal_kernel__sysfs_create_group__grp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct attribute_group const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	struct attribute** const* attrs_ptr = &ptr->attrs;
	
	{
		const void* __adjusted = *name_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*name_ptr) {
			size_t i, len;
			char const* array = *name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	{
		const void* __adjusted = *attrs_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*attrs_ptr) {
			size_t i, len = 5;
			struct attribute* const* array = *attrs_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct attribute* const* element = &array[i];
				const void* __adjusted = *element;
				glue_pack_shadow(__pos, msg, ext, __adjusted);
				if (*element) {
					caller_marshal_kernel__sysfs_create_group__attr__in(__pos, msg, ext, ctx, *element);
				}

			}

		}

	}

}

void callee_unmarshal_kernel__sysfs_create_group__grp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct attribute_group* ptr)
{
	char const** name_ptr = &ptr->name;
	struct attribute*** attrs_ptr = &ptr->attrs;
	
	{
		*name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, sizeof(char) * glue_peek(__pos, msg, ext));
		if (*name_ptr) {
			char* writable = (char*)*name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(__pos, msg, ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, char);
			}

		}

	}

	{
		*attrs_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct attribute**, sizeof(struct attribute*) * glue_peek(__pos, msg, ext));
		if (*attrs_ptr) {
			int i;
			struct attribute** array = *attrs_ptr;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct attribute** element = &array[i];
				*element = glue_unpack(__pos, msg, ext, struct attribute*);
				if (*element) {
					callee_unmarshal_kernel__sysfs_create_group__attr__in(__pos, msg, ext, ctx, *element);
				}

			}

		}

	}

}

void callee_marshal_kernel__sysfs_create_group__grp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct attribute_group const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	struct attribute** const* attrs_ptr = &ptr->attrs;
	
	{
		(void)name_ptr;
	}

	{
		(void)attrs_ptr;
	}

}

void caller_unmarshal_kernel__sysfs_create_group__grp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct attribute_group* ptr)
{
	char const** name_ptr = &ptr->name;
	struct attribute*** attrs_ptr = &ptr->attrs;
	
	{
		(void)name_ptr;
	}

	{
		(void)attrs_ptr;
	}

}

void caller_marshal_kernel__sysfs_create_group__attr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct attribute const* ptr)
{
	unsigned short const* mode_ptr = &ptr->mode;
	
	{
		glue_pack(__pos, msg, ext, *mode_ptr);
	}

}

void callee_unmarshal_kernel__sysfs_create_group__attr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct attribute* ptr)
{
	unsigned short* mode_ptr = &ptr->mode;
	
	{
		*mode_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

}

void callee_marshal_kernel__sysfs_create_group__attr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct attribute const* ptr)
{
	
}

void caller_unmarshal_kernel__sysfs_create_group__attr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct attribute* ptr)
{
	
}

void caller_marshal_kernel__platform_driver_unregister__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_driver_unregister_call_ctx const* ctx,
	struct platform_driver const* ptr)
{
	
}

void callee_unmarshal_kernel__platform_driver_unregister__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_driver_unregister_call_ctx const* ctx,
	struct platform_driver* ptr)
{
	
}

void callee_marshal_kernel__platform_driver_unregister__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_driver_unregister_call_ctx const* ctx,
	struct platform_driver const* ptr)
{
	
}

void caller_unmarshal_kernel__platform_driver_unregister__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_driver_unregister_call_ctx const* ctx,
	struct platform_driver* ptr)
{
	
}

void caller_marshal_kernel____platform_driver_register__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* ctx,
	struct platform_driver const* ptr)
{
	fptr_probe const* probe_ptr = &ptr->probe;
	fptr_remove const* remove_ptr = &ptr->remove;
	
	{
		glue_pack(__pos, msg, ext, *probe_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *remove_ptr);
	}

}

void callee_unmarshal_kernel____platform_driver_register__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* ctx,
	struct platform_driver* ptr)
{
	fptr_probe* probe_ptr = &ptr->probe;
	fptr_remove* remove_ptr = &ptr->remove;
	
	{
		*probe_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, probe);
	}

	{
		*remove_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, remove);
	}

}

void callee_marshal_kernel____platform_driver_register__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* ctx,
	struct platform_driver const* ptr)
{
	
}

void caller_unmarshal_kernel____platform_driver_register__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* ctx,
	struct platform_driver* ptr)
{
	
}

void caller_marshal_kernel____platform_driver_register__owner__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* ctx,
	struct module const* ptr)
{
	
}

void callee_unmarshal_kernel____platform_driver_register__owner__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* ctx,
	struct module* ptr)
{
	
}

void callee_marshal_kernel____platform_driver_register__owner__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* ctx,
	struct module const* ptr)
{
	
}

void caller_unmarshal_kernel____platform_driver_register__owner__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* ctx,
	struct module* ptr)
{
	
}

void caller_marshal_kernel__platform_device_alloc__ret_platform_device__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_alloc_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	
}

void callee_unmarshal_kernel__platform_device_alloc__ret_platform_device__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_alloc_call_ctx const* ctx,
	struct platform_device* ptr)
{
	
}

void callee_marshal_kernel__platform_device_alloc__ret_platform_device__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_alloc_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	
}

void caller_unmarshal_kernel__platform_device_alloc__ret_platform_device__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_alloc_call_ctx const* ctx,
	struct platform_device* ptr)
{
	
}

void caller_marshal_kernel__platform_device_add__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_add_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	int const* id_ptr = &ptr->id;
	
	{
		glue_pack(__pos, msg, ext, *id_ptr);
	}

}

void callee_unmarshal_kernel__platform_device_add__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_add_call_ctx const* ctx,
	struct platform_device* ptr)
{
	int* id_ptr = &ptr->id;
	
	{
		*id_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void callee_marshal_kernel__platform_device_add__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_add_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	
}

void caller_unmarshal_kernel__platform_device_add__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_add_call_ctx const* ctx,
	struct platform_device* ptr)
{
	
}

void caller_marshal_kernel__platform_device_put__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_put_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	
}

void callee_unmarshal_kernel__platform_device_put__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_put_call_ctx const* ctx,
	struct platform_device* ptr)
{
	
}

void callee_marshal_kernel__platform_device_put__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_put_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	
}

void caller_unmarshal_kernel__platform_device_put__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_put_call_ctx const* ctx,
	struct platform_device* ptr)
{
	
}

void caller_marshal_kernel__platform_device_unregister__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_unregister_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	
}

void callee_unmarshal_kernel__platform_device_unregister__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_unregister_call_ctx const* ctx,
	struct platform_device* ptr)
{
	
}

void callee_marshal_kernel__platform_device_unregister__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_unregister_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	
}

void caller_unmarshal_kernel__platform_device_unregister__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_unregister_call_ctx const* ctx,
	struct platform_device* ptr)
{
	
}

void caller_marshal_kernel__x86_match_cpu__ret_x86_cpu_id__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* ctx,
	struct x86_cpu_id const* ptr)
{
	
}

void callee_unmarshal_kernel__x86_match_cpu__ret_x86_cpu_id__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* ctx,
	struct x86_cpu_id* ptr)
{
	
}

void callee_marshal_kernel__x86_match_cpu__ret_x86_cpu_id__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* ctx,
	struct x86_cpu_id const* ptr)
{
	unsigned short const* vendor_ptr = &ptr->vendor;
	unsigned short const* family_ptr = &ptr->family;
	unsigned short const* model_ptr = &ptr->model;
	unsigned short const* feature_ptr = &ptr->feature;
	unsigned long long const* driver_data_ptr = &ptr->driver_data;
	
	{
		glue_pack(__pos, msg, ext, *vendor_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *family_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *model_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *feature_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *driver_data_ptr);
	}

}

void caller_unmarshal_kernel__x86_match_cpu__ret_x86_cpu_id__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* ctx,
	struct x86_cpu_id* ptr)
{
	unsigned short* vendor_ptr = &ptr->vendor;
	unsigned short* family_ptr = &ptr->family;
	unsigned short* model_ptr = &ptr->model;
	unsigned short* feature_ptr = &ptr->feature;
	unsigned long long* driver_data_ptr = &ptr->driver_data;
	
	{
		*vendor_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*family_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*model_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*feature_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*driver_data_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

}

void caller_marshal_kernel__x86_match_cpu__match__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* ctx,
	struct x86_cpu_id const* ptr)
{
	unsigned short const* vendor_ptr = &ptr->vendor;
	unsigned short const* family_ptr = &ptr->family;
	unsigned short const* model_ptr = &ptr->model;
	unsigned short const* feature_ptr = &ptr->feature;
	unsigned long long const* driver_data_ptr = &ptr->driver_data;
	
	{
		glue_pack(__pos, msg, ext, *vendor_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *family_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *model_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *feature_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *driver_data_ptr);
	}

}

void callee_unmarshal_kernel__x86_match_cpu__match__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* ctx,
	struct x86_cpu_id* ptr)
{
	unsigned short* vendor_ptr = &ptr->vendor;
	unsigned short* family_ptr = &ptr->family;
	unsigned short* model_ptr = &ptr->model;
	unsigned short* feature_ptr = &ptr->feature;
	unsigned long long* driver_data_ptr = &ptr->driver_data;
	
	{
		*vendor_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*family_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*model_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*feature_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*driver_data_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

}

void callee_marshal_kernel__x86_match_cpu__match__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* ctx,
	struct x86_cpu_id const* ptr)
{
	
}

void caller_unmarshal_kernel__x86_match_cpu__match__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct x86_match_cpu_call_ctx const* ctx,
	struct x86_cpu_id* ptr)
{
	
}

void caller_marshal_kernel__pci_get_domain_bus_and_slot__ret_pci_dev__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_domain_bus_and_slot_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_get_domain_bus_and_slot__ret_pci_dev__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_domain_bus_and_slot_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_get_domain_bus_and_slot__ret_pci_dev__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_domain_bus_and_slot_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned short const* vendor_ptr = &ptr->vendor;
	unsigned short const* device_ptr = &ptr->device;
	
	{
		glue_pack(__pos, msg, ext, *vendor_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *device_ptr);
	}

}

void caller_unmarshal_kernel__pci_get_domain_bus_and_slot__ret_pci_dev__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_domain_bus_and_slot_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned short* vendor_ptr = &ptr->vendor;
	unsigned short* device_ptr = &ptr->device;
	
	{
		*vendor_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*device_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

}

void caller_marshal_kernel__devm_hwmon_device_register_with_groups__ret_device__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void callee_unmarshal_kernel__devm_hwmon_device_register_with_groups__ret_device__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void callee_marshal_kernel__devm_hwmon_device_register_with_groups__ret_device__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void caller_unmarshal_kernel__devm_hwmon_device_register_with_groups__ret_device__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void caller_marshal_kernel__devm_hwmon_device_register_with_groups__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void callee_unmarshal_kernel__devm_hwmon_device_register_with_groups__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void callee_marshal_kernel__devm_hwmon_device_register_with_groups__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void caller_unmarshal_kernel__devm_hwmon_device_register_with_groups__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void caller_marshal_kernel__devm_hwmon_device_register_with_groups__groups__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* ctx,
	struct attribute_group const* ptr)
{
	
}

void callee_unmarshal_kernel__devm_hwmon_device_register_with_groups__groups__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* ctx,
	struct attribute_group* ptr)
{
	
}

void callee_marshal_kernel__devm_hwmon_device_register_with_groups__groups__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* ctx,
	struct attribute_group const* ptr)
{
	
}

void caller_unmarshal_kernel__devm_hwmon_device_register_with_groups__groups__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct devm_hwmon_device_register_with_groups_call_ctx const* ctx,
	struct attribute_group* ptr)
{
	
}


#ifdef LCD_ISOLATE
__attribute__((weak)) void shared_mem_init(void) {
	LIBLCD_MSG("Weak shared_mem_init does nothing! Override if you want!");
}
#else
__attribute__((weak)) void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext) {
	LIBLCD_MSG("Weak shared_mem_init_callee does nothing! Override if you want!");
}
#endif	/* LCD_ISOLATE */

