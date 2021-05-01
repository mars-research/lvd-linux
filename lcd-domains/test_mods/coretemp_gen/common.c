#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

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
	int const* id_ptr = &ptr->id;
	
	{
		glue_pack(__pos, msg, ext, *id_ptr);
	}

}

void caller_unmarshal_kernel__platform_device_unregister__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_unregister_call_ctx const* ctx,
	struct platform_device* ptr)
{
	int* id_ptr = &ptr->id;
	
	{
		*id_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void caller_marshal_kernel__platform_driver_probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_driver_probe_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	int const* id_ptr = &ptr->id;
	
	{
		glue_pack(__pos, msg, ext, *id_ptr);
	}

}

void callee_unmarshal_kernel__platform_driver_probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_driver_probe_call_ctx const* ctx,
	struct platform_device* ptr)
{
	int* id_ptr = &ptr->id;
	
	{
		*id_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void callee_marshal_kernel__platform_driver_probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_driver_probe_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	
}

void caller_unmarshal_kernel__platform_driver_probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_driver_probe_call_ctx const* ctx,
	struct platform_device* ptr)
{
	
}

void caller_marshal_kernel__platform_driver_remove__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_driver_remove_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	
}

void callee_unmarshal_kernel__platform_driver_remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_driver_remove_call_ctx const* ctx,
	struct platform_device* ptr)
{
	
}

void callee_marshal_kernel__platform_driver_remove__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_driver_remove_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	
}

void caller_unmarshal_kernel__platform_driver_remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_driver_remove_call_ctx const* ctx,
	struct platform_device* ptr)
{
	
}

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
	struct attribute** const* attrs_ptr = &ptr->attrs;
	
	{
		__maybe_unused const void* __adjusted = *attrs_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*attrs_ptr) {
			size_t i, len = 5;
			struct attribute* const* array = *attrs_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct attribute* const* element = &array[i];
				__maybe_unused const void* __adjusted = *element;
				glue_pack(__pos, msg, ext, __adjusted);
				if (*element) {
					size_t i, len = 1;
					struct attribute const* array = *element;
					glue_pack(__pos, msg, ext, len);
					// Warning: see David if this breaks
					glue_user_trace("Warning: see David if this breaks");
					for (i = 0; i < len; ++i) {
						struct attribute const* element = &array[i];
						caller_marshal_kernel__sysfs_remove_group__attr__in(__pos, msg, ext, ctx, element);
					}

				}

			}

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
	struct attribute*** attrs_ptr = &ptr->attrs;
	
	{
		*attrs_ptr = glue_unpack(__pos, msg, ext, struct attribute**);
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
					int i;
					struct attribute* array = *element;
					size_t len = glue_unpack(__pos, msg, ext, size_t);
					// Warning: see David if this breaks
					glue_user_trace("Warning: see David if this breaks");
					for (i = 0; i < len; ++i) {
						struct attribute* element = &array[i];
						callee_unmarshal_kernel__sysfs_remove_group__attr__in(__pos, msg, ext, ctx, element);
					}

				}

			}

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
	struct attribute** const* attrs_ptr = &ptr->attrs;
	
	{
		(void)attrs_ptr;
		glue_remove_shadow(*attrs_ptr);
	}

}

void caller_unmarshal_kernel__sysfs_remove_group__grp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* ctx,
	struct attribute_group* ptr)
{
	struct attribute*** attrs_ptr = &ptr->attrs;
	
	{
		(void)attrs_ptr;
	}

}

void caller_marshal_kernel__sysfs_remove_group__attr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* ctx,
	struct attribute const* ptr)
{
	
}

void callee_unmarshal_kernel__sysfs_remove_group__attr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* ctx,
	struct attribute* ptr)
{
	
}

void callee_marshal_kernel__sysfs_remove_group__attr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* ctx,
	struct attribute const* ptr)
{
	
}

void caller_unmarshal_kernel__sysfs_remove_group__attr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_remove_group_call_ctx const* ctx,
	struct attribute* ptr)
{
	
}

void caller_marshal_kernel__dev_attr_show__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dev_attr_show_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void callee_unmarshal_kernel__dev_attr_show__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dev_attr_show_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void callee_marshal_kernel__dev_attr_show__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dev_attr_show_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void caller_unmarshal_kernel__dev_attr_show__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dev_attr_show_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void caller_marshal_kernel__dev_attr_show__dev_attr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dev_attr_show_call_ctx const* ctx,
	struct device_attribute const* ptr)
{
	
}

void callee_unmarshal_kernel__dev_attr_show__dev_attr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dev_attr_show_call_ctx const* ctx,
	struct device_attribute* ptr)
{
	
}

void callee_marshal_kernel__dev_attr_show__dev_attr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dev_attr_show_call_ctx const* ctx,
	struct device_attribute const* ptr)
{
	
}

void caller_unmarshal_kernel__dev_attr_show__dev_attr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dev_attr_show_call_ctx const* ctx,
	struct device_attribute* ptr)
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
	struct attribute** const* attrs_ptr = &ptr->attrs;
	
	{
		struct device_attribute** __casted = (struct device_attribute**)*attrs_ptr;
		struct device_attribute** const* __casted_ptr = &__casted;
		{
			__maybe_unused const void* __adjusted = *__casted_ptr;
			glue_pack(__pos, msg, ext, __adjusted);
			if (*__casted_ptr) {
				size_t i, len = 5;
				struct device_attribute* const* array = *__casted_ptr;
				glue_pack(__pos, msg, ext, len);
				// Warning: see David if this breaks
				glue_user_trace("Warning: see David if this breaks");
				for (i = 0; i < len; ++i) {
					struct device_attribute* const* element = &array[i];
					__maybe_unused const void* __adjusted = *element;
					glue_pack(__pos, msg, ext, __adjusted);
					if (*element) {
						size_t i, len = 1;
						struct device_attribute const* array = *element;
						glue_pack(__pos, msg, ext, len);
						// Warning: see David if this breaks
						glue_user_trace("Warning: see David if this breaks");
						for (i = 0; i < len; ++i) {
							struct device_attribute const* element = &array[i];
							caller_marshal_kernel__sysfs_create_group__dev_attr__in(__pos, msg, ext, ctx, element);
						}

					}

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
	struct attribute*** attrs_ptr = &ptr->attrs;
	
	{
		struct device_attribute** __casted = (struct device_attribute**)*attrs_ptr;
		struct device_attribute*** __casted_ptr = &__casted;
		{
			*__casted_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct device_attribute**, (sizeof(struct device_attribute*) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
			if (*__casted_ptr) {
				int i;
				struct device_attribute** array = *__casted_ptr;
				size_t len = glue_unpack(__pos, msg, ext, size_t);
				// Warning: see David if this breaks
				glue_user_trace("Warning: see David if this breaks");
				for (i = 0; i < len; ++i) {
					struct device_attribute** element = &array[i];
					*element = glue_unpack_new_shadow(__pos, msg, ext, struct device_attribute*, (sizeof(struct device_attribute) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
					if (*element) {
						int i;
						struct device_attribute* array = *element;
						size_t len = glue_unpack(__pos, msg, ext, size_t);
						// Warning: see David if this breaks
						glue_user_trace("Warning: see David if this breaks");
						for (i = 0; i < len; ++i) {
							struct device_attribute* element = &array[i];
							callee_unmarshal_kernel__sysfs_create_group__dev_attr__in(__pos, msg, ext, ctx, element);
						}

					}

				}

			}

		}

		*attrs_ptr = (struct attribute**)__casted;
	}

}

void callee_marshal_kernel__sysfs_create_group__grp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct attribute_group const* ptr)
{
	
}

void caller_unmarshal_kernel__sysfs_create_group__grp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct attribute_group* ptr)
{
	
}

void caller_marshal_kernel__sysfs_create_group__dev_attr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct device_attribute const* ptr)
{
	struct attribute const* attr_ptr = &ptr->attr;
	fptr_dev_attr_show const* show_ptr = &ptr->show;
	
	{
		caller_marshal_kernel__sysfs_create_group__attr__in(__pos, msg, ext, ctx, attr_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *show_ptr);
	}

}

void callee_unmarshal_kernel__sysfs_create_group__dev_attr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct device_attribute* ptr)
{
	struct attribute* attr_ptr = &ptr->attr;
	fptr_dev_attr_show* show_ptr = &ptr->show;
	
	{
		callee_unmarshal_kernel__sysfs_create_group__attr__in(__pos, msg, ext, ctx, attr_ptr);
	}

	{
		*show_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, dev_attr_show);
	}

}

void callee_marshal_kernel__sysfs_create_group__dev_attr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct device_attribute const* ptr)
{
	struct attribute const* attr_ptr = &ptr->attr;
	
	{
		callee_marshal_kernel__sysfs_create_group__attr__in(__pos, msg, ext, ctx, attr_ptr);
	}

}

void caller_unmarshal_kernel__sysfs_create_group__dev_attr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct device_attribute* ptr)
{
	struct attribute* attr_ptr = &ptr->attr;
	
	{
		caller_unmarshal_kernel__sysfs_create_group__attr__in(__pos, msg, ext, ctx, attr_ptr);
	}

}

void caller_marshal_kernel__sysfs_create_group__attr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct attribute const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	unsigned short const* mode_ptr = &ptr->mode;
	
	{
		__maybe_unused const void* __adjusted = *name_ptr;
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
	char const** name_ptr = &ptr->name;
	unsigned short* mode_ptr = &ptr->mode;
	
	{
		*name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, (sizeof(char) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
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
	char const* const* name_ptr = &ptr->name;
	
	{
		(void)name_ptr;
	}

}

void caller_unmarshal_kernel__sysfs_create_group__attr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sysfs_create_group_call_ctx const* ctx,
	struct attribute* ptr)
{
	char const** name_ptr = &ptr->name;
	
	{
		(void)name_ptr;
	}

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
	struct device_driver const* driver_ptr = &ptr->driver;
	fptr_platform_driver_probe const* probe_ptr = &ptr->probe;
	fptr_platform_driver_remove const* remove_ptr = &ptr->remove;
	
	{
		caller_marshal_kernel____platform_driver_register__device_driver__in(__pos, msg, ext, ctx, driver_ptr);
	}

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
	struct device_driver* driver_ptr = &ptr->driver;
	fptr_platform_driver_probe* probe_ptr = &ptr->probe;
	fptr_platform_driver_remove* remove_ptr = &ptr->remove;
	
	{
		callee_unmarshal_kernel____platform_driver_register__device_driver__in(__pos, msg, ext, ctx, driver_ptr);
	}

	{
		*probe_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, platform_driver_probe);
	}

	{
		*remove_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, platform_driver_remove);
	}

}

void callee_marshal_kernel____platform_driver_register__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* ctx,
	struct platform_driver const* ptr)
{
	struct device_driver const* driver_ptr = &ptr->driver;
	
	{
		callee_marshal_kernel____platform_driver_register__device_driver__in(__pos, msg, ext, ctx, driver_ptr);
	}

}

void caller_unmarshal_kernel____platform_driver_register__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* ctx,
	struct platform_driver* ptr)
{
	struct device_driver* driver_ptr = &ptr->driver;
	
	{
		caller_unmarshal_kernel____platform_driver_register__device_driver__in(__pos, msg, ext, ctx, driver_ptr);
	}

}

void caller_marshal_kernel____platform_driver_register__device_driver__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* ctx,
	struct device_driver const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	
	{
		__maybe_unused const void* __adjusted = *name_ptr;
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

}

void callee_unmarshal_kernel____platform_driver_register__device_driver__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* ctx,
	struct device_driver* ptr)
{
	char const** name_ptr = &ptr->name;
	
	{
		*name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, (sizeof(char) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
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

}

void callee_marshal_kernel____platform_driver_register__device_driver__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* ctx,
	struct device_driver const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	
	{
		(void)name_ptr;
	}

}

void caller_unmarshal_kernel____platform_driver_register__device_driver__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __platform_driver_register_call_ctx const* ctx,
	struct device_driver* ptr)
{
	char const** name_ptr = &ptr->name;
	
	{
		(void)name_ptr;
	}

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
	struct device const* dev_ptr = &ptr->dev;
	
	{
		caller_marshal_kernel__platform_device_add__device__in(__pos, msg, ext, ctx, dev_ptr);
	}

}

void callee_unmarshal_kernel__platform_device_add__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_add_call_ctx const* ctx,
	struct platform_device* ptr)
{
	struct device* dev_ptr = &ptr->dev;
	
	{
		callee_unmarshal_kernel__platform_device_add__device__in(__pos, msg, ext, ctx, dev_ptr);
	}

}

void callee_marshal_kernel__platform_device_add__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_add_call_ctx const* ctx,
	struct platform_device const* ptr)
{
	int const* id_ptr = &ptr->id;
	struct device const* dev_ptr = &ptr->dev;
	
	{
		glue_pack(__pos, msg, ext, *id_ptr);
	}

	{
		callee_marshal_kernel__platform_device_add__device__in(__pos, msg, ext, ctx, dev_ptr);
	}

}

void caller_unmarshal_kernel__platform_device_add__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_add_call_ctx const* ctx,
	struct platform_device* ptr)
{
	int* id_ptr = &ptr->id;
	struct device* dev_ptr = &ptr->dev;
	
	{
		*id_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		caller_unmarshal_kernel__platform_device_add__device__in(__pos, msg, ext, ctx, dev_ptr);
	}

}

void caller_marshal_kernel__platform_device_add__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_add_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void callee_unmarshal_kernel__platform_device_add__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_add_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void callee_marshal_kernel__platform_device_add__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct platform_device_add_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void caller_unmarshal_kernel__platform_device_add__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct platform_device_add_call_ctx const* ctx,
	struct device* ptr)
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

void caller_marshal_kernel__get_pcpu_cpu_data__cpuinfo_x86__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct get_pcpu_cpu_data_call_ctx const* ctx,
	struct cpuinfo_x86 const* ptr)
{
	
}

void callee_unmarshal_kernel__get_pcpu_cpu_data__cpuinfo_x86__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct get_pcpu_cpu_data_call_ctx const* ctx,
	struct cpuinfo_x86* ptr)
{
	
}

void callee_marshal_kernel__get_pcpu_cpu_data__cpuinfo_x86__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct get_pcpu_cpu_data_call_ctx const* ctx,
	struct cpuinfo_x86 const* ptr)
{
	unsigned int const* x86_capability_ptr = &ptr->x86_capability;
	unsigned short const* phys_proc_id_ptr = &ptr->phys_proc_id;
	unsigned short const* cpu_core_id_ptr = &ptr->cpu_core_id;
	unsigned char const* x86_vendor_ptr = &ptr->x86_vendor;
	unsigned char const* x86_model_ptr = &ptr->x86_model;
	unsigned char const* x86_mask_ptr = &ptr->x86_mask;
	unsigned int const* microcode_ptr = &ptr->microcode;
	char const* x86_model_id_ptr = ptr->x86_model_id;
	
	{
		size_t i, len = (NCAPINTS + NBUGINTS);
		unsigned int const* array = x86_capability_ptr;
		glue_pack(__pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, msg, ext, *element);
		}

	}

	{
		glue_pack(__pos, msg, ext, *phys_proc_id_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *cpu_core_id_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *x86_vendor_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *x86_model_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *x86_mask_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *microcode_ptr);
	}

	{
		size_t i, len = 64;
		char const* array = x86_model_id_ptr;
		glue_pack(__pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(__pos, msg, ext, *element);
		}

	}

}

void caller_unmarshal_kernel__get_pcpu_cpu_data__cpuinfo_x86__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct get_pcpu_cpu_data_call_ctx const* ctx,
	struct cpuinfo_x86* ptr)
{
	unsigned int* x86_capability_ptr = &ptr->x86_capability;
	unsigned short* phys_proc_id_ptr = &ptr->phys_proc_id;
	unsigned short* cpu_core_id_ptr = &ptr->cpu_core_id;
	unsigned char* x86_vendor_ptr = &ptr->x86_vendor;
	unsigned char* x86_model_ptr = &ptr->x86_model;
	unsigned char* x86_mask_ptr = &ptr->x86_mask;
	unsigned int* microcode_ptr = &ptr->microcode;
	char* x86_model_id_ptr = ptr->x86_model_id;
	
	{
		int i;
		unsigned int* array = x86_capability_ptr;
		size_t len = glue_unpack(__pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

	{
		*phys_proc_id_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*cpu_core_id_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*x86_vendor_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*x86_model_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*x86_mask_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*microcode_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		int i;
		char* array = x86_model_id_ptr;
		size_t len = glue_unpack(__pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(__pos, msg, ext, char);
		}

	}

}

void caller_marshal_kernel__notifier_call__nb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct notifier_call_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	
}

void callee_unmarshal_kernel__notifier_call__nb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct notifier_call_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	
}

void callee_marshal_kernel__notifier_call__nb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct notifier_call_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	
}

void caller_unmarshal_kernel__notifier_call__nb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct notifier_call_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	
}

void caller_marshal_kernel____register_cpu_notifier__notifier_block__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __register_cpu_notifier_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	fptr_notifier_call const* notifier_call_ptr = &ptr->notifier_call;
	
	{
		glue_pack(__pos, msg, ext, *notifier_call_ptr);
	}

}

void callee_unmarshal_kernel____register_cpu_notifier__notifier_block__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __register_cpu_notifier_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	fptr_notifier_call* notifier_call_ptr = &ptr->notifier_call;
	
	{
		*notifier_call_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, notifier_call);
	}

}

void callee_marshal_kernel____register_cpu_notifier__notifier_block__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __register_cpu_notifier_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	
}

void caller_unmarshal_kernel____register_cpu_notifier__notifier_block__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __register_cpu_notifier_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	
}

void caller_marshal_kernel____unregister_cpu_notifier__notifier_block__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __unregister_cpu_notifier_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	
}

void callee_unmarshal_kernel____unregister_cpu_notifier__notifier_block__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __unregister_cpu_notifier_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	
}

void callee_marshal_kernel____unregister_cpu_notifier__notifier_block__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __unregister_cpu_notifier_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	
}

void caller_unmarshal_kernel____unregister_cpu_notifier__notifier_block__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __unregister_cpu_notifier_call_ctx const* ctx,
	struct notifier_block* ptr)
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

