#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	unsigned int const* devfn_ptr = &ptr->devfn;
	unsigned int const* irq_ptr = &ptr->irq;
	
	{
		const void* __adjusted = *bus_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__pci_driver_probe__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *devfn_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *irq_ptr);
	}

}

void callee_unmarshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	unsigned int* devfn_ptr = &ptr->devfn;
	unsigned int* irq_ptr = &ptr->irq;
	
	{
		*bus_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct pci_bus*, (sizeof(struct pci_bus)), (DEFAULT_GFP_FLAGS));
		if (*bus_ptr) {
			callee_unmarshal_kernel__pci_driver_probe__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		*devfn_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*irq_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	
	{
		if (*bus_ptr) {
			callee_marshal_kernel__pci_driver_probe__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void caller_unmarshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	
	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__pci_driver_probe__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void caller_marshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void callee_marshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void caller_marshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_device_id const* ptr)
{
	unsigned long const* driver_data_ptr = &ptr->driver_data;
	
	{
		glue_pack(__pos, msg, ext, *driver_data_ptr);
	}

}

void callee_unmarshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_device_id* ptr)
{
	unsigned long* driver_data_ptr = &ptr->driver_data;
	
	{
		*driver_data_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

}

void callee_marshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_device_id const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_device_id* ptr)
{
	
}

void caller_marshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_remove_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int const* irq_ptr = &ptr->irq;
	
	{
		glue_pack(__pos, msg, ext, *irq_ptr);
	}

}

void callee_unmarshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_remove_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int* irq_ptr = &ptr->irq;
	
	{
		*irq_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_remove_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_remove_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_shutdown_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_shutdown_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_driver_shutdown_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_driver_shutdown_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	unsigned int const* devfn_ptr = &ptr->devfn;
	
	{
		const void* __adjusted = *bus_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__pci_set_master__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *devfn_ptr);
	}

}

void callee_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	unsigned int* devfn_ptr = &ptr->devfn;
	
	{
		*bus_ptr = glue_unpack(__pos, msg, ext, struct pci_bus*);
		if (*bus_ptr) {
			callee_unmarshal_kernel__pci_set_master__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		*devfn_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	
	{
		if (*bus_ptr) {
			callee_marshal_kernel__pci_set_master__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void caller_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	
	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__pci_set_master__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void caller_marshal_kernel__pci_set_master__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_set_master__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void callee_marshal_kernel__pci_set_master__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_set_master__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void caller_marshal_kernel__pci_request_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_request_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_request_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_request_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_request_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_request_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_request_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_request_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__pci_release_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_release_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_release_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_release_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_release_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_release_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_release_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_release_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__pci_dev_run_wake__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	
	{
		const void* __adjusted = *bus_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__pci_dev_run_wake__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void callee_unmarshal_kernel__pci_dev_run_wake__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	
	{
		*bus_ptr = glue_unpack(__pos, msg, ext, struct pci_bus*);
		if (*bus_ptr) {
			callee_unmarshal_kernel__pci_dev_run_wake__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void callee_marshal_kernel__pci_dev_run_wake__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	
	{
		if (*bus_ptr) {
			callee_marshal_kernel__pci_dev_run_wake__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void caller_unmarshal_kernel__pci_dev_run_wake__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	
	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__pci_dev_run_wake__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void caller_marshal_kernel__pci_dev_run_wake__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_dev_run_wake__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void callee_marshal_kernel__pci_dev_run_wake__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_dev_run_wake__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void caller_marshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	unsigned int const* devfn_ptr = &ptr->devfn;
	
	{
		const void* __adjusted = *bus_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__pci_enable_device__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *devfn_ptr);
	}

}

void callee_unmarshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	unsigned int* devfn_ptr = &ptr->devfn;
	
	{
		*bus_ptr = glue_unpack(__pos, msg, ext, struct pci_bus*);
		if (*bus_ptr) {
			callee_unmarshal_kernel__pci_enable_device__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		*devfn_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	
	{
		if (*bus_ptr) {
			callee_marshal_kernel__pci_enable_device__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void caller_unmarshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	
	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__pci_enable_device__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void caller_marshal_kernel__pci_enable_device__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_enable_device__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void callee_marshal_kernel__pci_enable_device__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_enable_device__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void caller_marshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_unregister_driver_call_ctx const* ctx,
	struct pci_driver const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_unregister_driver_call_ctx const* ctx,
	struct pci_driver* ptr)
{
	
}

void callee_marshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_unregister_driver_call_ctx const* ctx,
	struct pci_driver const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_unregister_driver_call_ctx const* ctx,
	struct pci_driver* ptr)
{
	
}

void caller_marshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct pci_driver const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	fptr_pci_driver_probe const* probe_ptr = &ptr->probe;
	fptr_pci_driver_remove const* remove_ptr = &ptr->remove;
	fptr_pci_driver_shutdown const* shutdown_ptr = &ptr->shutdown;
	struct pci_device_id* const* id_table_ptr = &ptr->id_table;
	
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
		glue_pack(__pos, msg, ext, *probe_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *remove_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *shutdown_ptr);
	}

	{
		const void* __adjusted = *id_table_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*id_table_ptr) {
			size_t i, len;
			struct pci_device_id sentinel = { 0 };
			struct pci_device_id const* array = *id_table_ptr;
			for (len = 0; memcmp(&array[len], &sentinel, sizeof(array[0])); ++len) ;
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				struct pci_device_id const* element = &array[i];
				caller_marshal_kernel____pci_register_driver__pci_device_id__in(__pos, msg, ext, ctx, element);
			}

		}

	}

}

void callee_unmarshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct pci_driver* ptr)
{
	char const** name_ptr = &ptr->name;
	fptr_pci_driver_probe* probe_ptr = &ptr->probe;
	fptr_pci_driver_remove* remove_ptr = &ptr->remove;
	fptr_pci_driver_shutdown* shutdown_ptr = &ptr->shutdown;
	struct pci_device_id** id_table_ptr = &ptr->id_table;
	
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
		*probe_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, pci_driver_probe);
	}

	{
		*remove_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, pci_driver_remove);
	}

	{
		*shutdown_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, pci_driver_shutdown);
	}

	{
		*id_table_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct pci_device_id*, (sizeof(struct pci_device_id) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
		if (*id_table_ptr) {
			size_t i, len;
			struct pci_device_id* array = *id_table_ptr;
			len = glue_unpack(__pos, msg, ext, size_t) - 1;
			memset(&array[len], 0x0, sizeof(array[len]));
			for (i = 0; i < len; ++i) {
				struct pci_device_id* element = &array[i];
				callee_unmarshal_kernel____pci_register_driver__pci_device_id__in(__pos, msg, ext, ctx, element);
			}

		}

	}

}

void callee_marshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct pci_driver const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	struct pci_device_id* const* id_table_ptr = &ptr->id_table;
	
	{
		(void)name_ptr;
	}

	{
		(void)id_table_ptr;
	}

}

void caller_unmarshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct pci_driver* ptr)
{
	char const** name_ptr = &ptr->name;
	struct pci_device_id** id_table_ptr = &ptr->id_table;
	
	{
		(void)name_ptr;
	}

	{
		(void)id_table_ptr;
	}

}

void caller_marshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct pci_device_id const* ptr)
{
	unsigned int const* vendor_ptr = &ptr->vendor;
	unsigned int const* device_ptr = &ptr->device;
	unsigned int const* subvendor_ptr = &ptr->subvendor;
	unsigned int const* subdevice_ptr = &ptr->subdevice;
	unsigned int const* class_ptr = &ptr->class;
	unsigned int const* class_mask_ptr = &ptr->class_mask;
	unsigned long long const* driver_data_ptr = &ptr->driver_data;
	
	{
		glue_pack(__pos, msg, ext, *vendor_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *device_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *subvendor_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *subdevice_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *class_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *class_mask_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *driver_data_ptr);
	}

}

void callee_unmarshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct pci_device_id* ptr)
{
	unsigned int* vendor_ptr = &ptr->vendor;
	unsigned int* device_ptr = &ptr->device;
	unsigned int* subvendor_ptr = &ptr->subvendor;
	unsigned int* subdevice_ptr = &ptr->subdevice;
	unsigned int* class_ptr = &ptr->class;
	unsigned int* class_mask_ptr = &ptr->class_mask;
	unsigned long long* driver_data_ptr = &ptr->driver_data;
	
	{
		*vendor_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*device_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*subvendor_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*subdevice_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*class_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*class_mask_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*driver_data_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

}

void callee_marshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct pci_device_id const* ptr)
{
	
}

void caller_unmarshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct pci_device_id* ptr)
{
	
}

void caller_marshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct module const* ptr)
{
	
}

void callee_unmarshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct module* ptr)
{
	
}

void callee_marshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct module const* ptr)
{
	
}

void caller_unmarshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct module* ptr)
{
	
}

void caller_marshal_kernel__mei_hbm_pg_resume__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hbm_pg_resume_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hbm_pg_resume__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hbm_pg_resume_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hbm_pg_resume__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hbm_pg_resume_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hbm_pg_resume__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hbm_pg_resume_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hbm_pg__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hbm_pg_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	{
		glue_pack(__pos, msg, ext, ptr->hbm_f_pg_supported);
	}

}

void callee_unmarshal_kernel__mei_hbm_pg__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hbm_pg_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
	{
		ptr->hbm_f_pg_supported = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__mei_hbm_pg__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hbm_pg_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hbm_pg__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hbm_pg_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_device_init__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct __wait_queue_head const* wait_hw_ready_ptr = &ptr->wait_hw_ready;
	struct __wait_queue_head const* wait_pg_ptr = &ptr->wait_pg;
	unsigned int const* dev_state_ptr = &ptr->dev_state;
	unsigned int const* pg_event_ptr = &ptr->pg_event;
	
	{
		caller_marshal_kernel__mei_device_init____wait_queue_head__in(__pos, msg, ext, ctx, wait_hw_ready_ptr);
	}

	{
		caller_marshal_kernel__mei_device_init____wait_queue_head__in(__pos, msg, ext, ctx, wait_pg_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *dev_state_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *pg_event_ptr);
	}

}

void callee_unmarshal_kernel__mei_device_init__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct __wait_queue_head* wait_hw_ready_ptr = &ptr->wait_hw_ready;
	struct __wait_queue_head* wait_pg_ptr = &ptr->wait_pg;
	unsigned int* dev_state_ptr = &ptr->dev_state;
	unsigned int* pg_event_ptr = &ptr->pg_event;
	
	{
		callee_unmarshal_kernel__mei_device_init____wait_queue_head__in(__pos, msg, ext, ctx, wait_hw_ready_ptr);
	}

	{
		callee_unmarshal_kernel__mei_device_init____wait_queue_head__in(__pos, msg, ext, ctx, wait_pg_ptr);
	}

	{
		*dev_state_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*pg_event_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__mei_device_init__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct __wait_queue_head const* wait_hw_ready_ptr = &ptr->wait_hw_ready;
	struct __wait_queue_head const* wait_pg_ptr = &ptr->wait_pg;
	unsigned int const* dev_state_ptr = &ptr->dev_state;
	unsigned int const* pg_event_ptr = &ptr->pg_event;
	
	{
		callee_marshal_kernel__mei_device_init____wait_queue_head__in(__pos, msg, ext, ctx, wait_hw_ready_ptr);
	}

	{
		callee_marshal_kernel__mei_device_init____wait_queue_head__in(__pos, msg, ext, ctx, wait_pg_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *dev_state_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *pg_event_ptr);
	}

}

void caller_unmarshal_kernel__mei_device_init__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct __wait_queue_head* wait_hw_ready_ptr = &ptr->wait_hw_ready;
	struct __wait_queue_head* wait_pg_ptr = &ptr->wait_pg;
	unsigned int* dev_state_ptr = &ptr->dev_state;
	unsigned int* pg_event_ptr = &ptr->pg_event;
	
	{
		caller_unmarshal_kernel__mei_device_init____wait_queue_head__in(__pos, msg, ext, ctx, wait_hw_ready_ptr);
	}

	{
		caller_unmarshal_kernel__mei_device_init____wait_queue_head__in(__pos, msg, ext, ctx, wait_pg_ptr);
	}

	{
		*dev_state_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*pg_event_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void caller_marshal_kernel__mei_device_init____wait_queue_head__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_device_init____wait_queue_head__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void callee_marshal_kernel__mei_device_init____wait_queue_head__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_device_init____wait_queue_head__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void caller_marshal_kernel__mei_device_init__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_device_init__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void callee_marshal_kernel__mei_device_init__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_device_init__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void caller_marshal_kernel___global_mei_hw_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops const* ptr)
{
	fptr_mei_hw_ops_fw_status const* fw_status_ptr = &ptr->fw_status;
	fptr_mei_hw_ops_hbuf_free_slots const* hbuf_free_slots_ptr = &ptr->hbuf_free_slots;
	fptr_mei_hw_ops_hbuf_is_ready const* hbuf_is_ready_ptr = &ptr->hbuf_is_ready;
	fptr_mei_hw_ops_hbuf_max_len const* hbuf_max_len_ptr = &ptr->hbuf_max_len;
	fptr_mei_hw_ops_host_is_ready const* host_is_ready_ptr = &ptr->host_is_ready;
	fptr_mei_hw_ops_hw_config const* hw_config_ptr = &ptr->hw_config;
	fptr_mei_hw_ops_hw_is_ready const* hw_is_ready_ptr = &ptr->hw_is_ready;
	fptr_mei_hw_ops_hw_reset const* hw_reset_ptr = &ptr->hw_reset;
	fptr_mei_hw_ops_hw_start const* hw_start_ptr = &ptr->hw_start;
	fptr_mei_hw_ops_intr_clear const* intr_clear_ptr = &ptr->intr_clear;
	fptr_mei_hw_ops_intr_disable const* intr_disable_ptr = &ptr->intr_disable;
	fptr_mei_hw_ops_intr_enable const* intr_enable_ptr = &ptr->intr_enable;
	fptr_mei_hw_ops_pg_in_transition const* pg_in_transition_ptr = &ptr->pg_in_transition;
	fptr_mei_hw_ops_pg_is_enabled const* pg_is_enabled_ptr = &ptr->pg_is_enabled;
	fptr_mei_hw_ops_pg_state const* pg_state_ptr = &ptr->pg_state;
	fptr_mei_hw_ops_rdbuf_full_slots const* rdbuf_full_slots_ptr = &ptr->rdbuf_full_slots;
	fptr_mei_hw_ops_read const* read_ptr = &ptr->read;
	fptr_mei_hw_ops_read_hdr const* read_hdr_ptr = &ptr->read_hdr;
	fptr_mei_hw_ops_write const* write_ptr = &ptr->write;
	
	{
		glue_pack(__pos, msg, ext, *fw_status_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *hbuf_free_slots_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *hbuf_is_ready_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *hbuf_max_len_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *host_is_ready_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *hw_config_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *hw_is_ready_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *hw_reset_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *hw_start_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *intr_clear_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *intr_disable_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *intr_enable_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *pg_in_transition_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *pg_is_enabled_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *pg_state_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rdbuf_full_slots_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *read_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *read_hdr_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *write_ptr);
	}

}

void callee_unmarshal_kernel___global_mei_hw_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops* ptr)
{
	fptr_mei_hw_ops_fw_status* fw_status_ptr = &ptr->fw_status;
	fptr_mei_hw_ops_hbuf_free_slots* hbuf_free_slots_ptr = &ptr->hbuf_free_slots;
	fptr_mei_hw_ops_hbuf_is_ready* hbuf_is_ready_ptr = &ptr->hbuf_is_ready;
	fptr_mei_hw_ops_hbuf_max_len* hbuf_max_len_ptr = &ptr->hbuf_max_len;
	fptr_mei_hw_ops_host_is_ready* host_is_ready_ptr = &ptr->host_is_ready;
	fptr_mei_hw_ops_hw_config* hw_config_ptr = &ptr->hw_config;
	fptr_mei_hw_ops_hw_is_ready* hw_is_ready_ptr = &ptr->hw_is_ready;
	fptr_mei_hw_ops_hw_reset* hw_reset_ptr = &ptr->hw_reset;
	fptr_mei_hw_ops_hw_start* hw_start_ptr = &ptr->hw_start;
	fptr_mei_hw_ops_intr_clear* intr_clear_ptr = &ptr->intr_clear;
	fptr_mei_hw_ops_intr_disable* intr_disable_ptr = &ptr->intr_disable;
	fptr_mei_hw_ops_intr_enable* intr_enable_ptr = &ptr->intr_enable;
	fptr_mei_hw_ops_pg_in_transition* pg_in_transition_ptr = &ptr->pg_in_transition;
	fptr_mei_hw_ops_pg_is_enabled* pg_is_enabled_ptr = &ptr->pg_is_enabled;
	fptr_mei_hw_ops_pg_state* pg_state_ptr = &ptr->pg_state;
	fptr_mei_hw_ops_rdbuf_full_slots* rdbuf_full_slots_ptr = &ptr->rdbuf_full_slots;
	fptr_mei_hw_ops_read* read_ptr = &ptr->read;
	fptr_mei_hw_ops_read_hdr* read_hdr_ptr = &ptr->read_hdr;
	fptr_mei_hw_ops_write* write_ptr = &ptr->write;
	
	{
		*fw_status_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_fw_status);
	}

	{
		*hbuf_free_slots_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_hbuf_free_slots);
	}

	{
		*hbuf_is_ready_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_hbuf_is_ready);
	}

	{
		*hbuf_max_len_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_hbuf_max_len);
	}

	{
		*host_is_ready_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_host_is_ready);
	}

	{
		*hw_config_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_hw_config);
	}

	{
		*hw_is_ready_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_hw_is_ready);
	}

	{
		*hw_reset_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_hw_reset);
	}

	{
		*hw_start_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_hw_start);
	}

	{
		*intr_clear_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_intr_clear);
	}

	{
		*intr_disable_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_intr_disable);
	}

	{
		*intr_enable_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_intr_enable);
	}

	{
		*pg_in_transition_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_pg_in_transition);
	}

	{
		*pg_is_enabled_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_pg_is_enabled);
	}

	{
		*pg_state_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_pg_state);
	}

	{
		*rdbuf_full_slots_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_rdbuf_full_slots);
	}

	{
		*read_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_read);
	}

	{
		*read_hdr_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_read_hdr);
	}

	{
		*write_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mei_hw_ops_write);
	}

}

void callee_marshal_kernel___global_mei_hw_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops const* ptr)
{
	
}

void caller_unmarshal_kernel___global_mei_hw_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops* ptr)
{
	
}

void caller_marshal_kernel__mei_stop__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_stop_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_stop__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_stop_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_stop__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_stop_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	unsigned int const* dev_state_ptr = &ptr->dev_state;
	
	{
		glue_pack(__pos, msg, ext, *dev_state_ptr);
	}

}

void caller_unmarshal_kernel__mei_stop__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_stop_call_ctx const* ctx,
	struct mei_device* ptr)
{
	unsigned int* dev_state_ptr = &ptr->dev_state;
	
	{
		*dev_state_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void caller_marshal_kernel__mei_cancel_work__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_cancel_work_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_cancel_work__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_cancel_work_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_cancel_work__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_cancel_work_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_cancel_work__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_cancel_work_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_start__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_start_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_start__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_start_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_start__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_start_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	unsigned int const* dev_state_ptr = &ptr->dev_state;
	
	{
		glue_pack(__pos, msg, ext, *dev_state_ptr);
	}

}

void caller_unmarshal_kernel__mei_start__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_start_call_ctx const* ctx,
	struct mei_device* ptr)
{
	unsigned int* dev_state_ptr = &ptr->dev_state;
	
	{
		*dev_state_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void caller_marshal_kernel__pci_iomap__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_iomap_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_iomap__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_iomap_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_iomap__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_iomap_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_iomap__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_iomap_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__pci_bus_read_config_dword__bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_bus_read_config_dword_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_bus_read_config_dword__bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_bus_read_config_dword_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void callee_marshal_kernel__pci_bus_read_config_dword__bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_bus_read_config_dword_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_bus_read_config_dword__bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_bus_read_config_dword_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_host_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_host_is_ready_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_host_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_host_is_ready_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_host_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_host_is_ready_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_host_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_host_is_ready_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_hw_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_is_ready_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_hw_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_is_ready_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_hw_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_is_ready_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_hw_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_is_ready_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_hw_reset__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct __wait_queue_head const* wait_pg_ptr = &ptr->wait_pg;
	unsigned int const* pg_event_ptr = &ptr->pg_event;
	
	{
		caller_marshal_kernel__mei_hw_ops_hw_reset____wait_queue_head__in(__pos, msg, ext, ctx, wait_pg_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *pg_event_ptr);
	}

}

void callee_unmarshal_kernel__mei_hw_ops_hw_reset__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct __wait_queue_head* wait_pg_ptr = &ptr->wait_pg;
	unsigned int* pg_event_ptr = &ptr->pg_event;
	
	{
		callee_unmarshal_kernel__mei_hw_ops_hw_reset____wait_queue_head__in(__pos, msg, ext, ctx, wait_pg_ptr);
	}

	{
		*pg_event_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__mei_hw_ops_hw_reset__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct __wait_queue_head const* wait_pg_ptr = &ptr->wait_pg;
	unsigned int const* pg_event_ptr = &ptr->pg_event;
	
	{
		callee_marshal_kernel__mei_hw_ops_hw_reset____wait_queue_head__in(__pos, msg, ext, ctx, wait_pg_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *pg_event_ptr);
	}

}

void caller_unmarshal_kernel__mei_hw_ops_hw_reset__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct __wait_queue_head* wait_pg_ptr = &ptr->wait_pg;
	unsigned int* pg_event_ptr = &ptr->pg_event;
	
	{
		caller_unmarshal_kernel__mei_hw_ops_hw_reset____wait_queue_head__in(__pos, msg, ext, ctx, wait_pg_ptr);
	}

	{
		*pg_event_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void caller_marshal_kernel__mei_hw_ops_hw_reset____wait_queue_head__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_hw_reset____wait_queue_head__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_hw_reset____wait_queue_head__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_hw_reset____wait_queue_head__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_reset_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_hw_start__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct __wait_queue_head const* wait_hw_ready_ptr = &ptr->wait_hw_ready;
	
	{
		caller_marshal_kernel__mei_hw_ops_hw_start____wait_queue_head__io(__pos, msg, ext, ctx, wait_hw_ready_ptr);
	}

}

void callee_unmarshal_kernel__mei_hw_ops_hw_start__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct __wait_queue_head* wait_hw_ready_ptr = &ptr->wait_hw_ready;
	
	{
		callee_unmarshal_kernel__mei_hw_ops_hw_start____wait_queue_head__io(__pos, msg, ext, ctx, wait_hw_ready_ptr);
	}

}

void callee_marshal_kernel__mei_hw_ops_hw_start__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct __wait_queue_head const* wait_hw_ready_ptr = &ptr->wait_hw_ready;
	
	{
		callee_marshal_kernel__mei_hw_ops_hw_start____wait_queue_head__io(__pos, msg, ext, ctx, wait_hw_ready_ptr);
	}

}

void caller_unmarshal_kernel__mei_hw_ops_hw_start__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct __wait_queue_head* wait_hw_ready_ptr = &ptr->wait_hw_ready;
	
	{
		caller_unmarshal_kernel__mei_hw_ops_hw_start____wait_queue_head__io(__pos, msg, ext, ctx, wait_hw_ready_ptr);
	}

}

void caller_marshal_kernel__mei_hw_ops_hw_start____wait_queue_head__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_hw_start____wait_queue_head__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_hw_start____wait_queue_head__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_hw_start____wait_queue_head__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_start_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_hw_config__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_config_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_hw_config__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_config_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_hw_config__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hw_config_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_hw_config__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hw_config_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_fw_status__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_fw_status__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_fw_status__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_fw_status__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_fw_status__fw_status__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* ctx,
	struct mei_fw_status const* ptr)
{
	int const* count_ptr = &ptr->count;
	unsigned int const* status_ptr = ptr->status;
	
	{
		glue_pack(__pos, msg, ext, *count_ptr);
	}

	{
		size_t i, len = 6;
		unsigned int const* array = status_ptr;
		glue_pack(__pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, msg, ext, *element);
		}

	}

}

void callee_unmarshal_kernel__mei_hw_ops_fw_status__fw_status__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* ctx,
	struct mei_fw_status* ptr)
{
	int* count_ptr = &ptr->count;
	unsigned int* status_ptr = ptr->status;
	
	{
		*count_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		int i;
		unsigned int* array = status_ptr;
		size_t len = glue_unpack(__pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

}

void callee_marshal_kernel__mei_hw_ops_fw_status__fw_status__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* ctx,
	struct mei_fw_status const* ptr)
{
	int const* count_ptr = &ptr->count;
	unsigned int const* status_ptr = ptr->status;
	
	{
		glue_pack(__pos, msg, ext, *count_ptr);
	}

	{
		size_t i, len = 6;
		unsigned int const* array = status_ptr;
		glue_pack(__pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, msg, ext, *element);
		}

	}

}

void caller_unmarshal_kernel__mei_hw_ops_fw_status__fw_status__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_fw_status_call_ctx const* ctx,
	struct mei_fw_status* ptr)
{
	int* count_ptr = &ptr->count;
	unsigned int* status_ptr = ptr->status;
	
	{
		*count_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		int i;
		unsigned int* array = status_ptr;
		size_t len = glue_unpack(__pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

}

void caller_marshal_kernel__mei_hw_ops_pg_state__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_pg_state_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_pg_state__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_pg_state_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_pg_state__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_pg_state_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_pg_state__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_pg_state_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_pg_in_transition__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_pg_in_transition_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	unsigned int const* pg_event_ptr = &ptr->pg_event;
	
	{
		glue_pack(__pos, msg, ext, *pg_event_ptr);
	}

}

void callee_unmarshal_kernel__mei_hw_ops_pg_in_transition__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_pg_in_transition_call_ctx const* ctx,
	struct mei_device* ptr)
{
	unsigned int* pg_event_ptr = &ptr->pg_event;
	
	{
		*pg_event_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__mei_hw_ops_pg_in_transition__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_pg_in_transition_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_pg_in_transition__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_pg_in_transition_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_pg_is_enabled__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_pg_is_enabled_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
	{
		glue_pack(__pos, msg, ext, ptr->hbm_f_pg_supported);
	}

}

void callee_unmarshal_kernel__mei_hw_ops_pg_is_enabled__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_pg_is_enabled_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
	{
		ptr->hbm_f_pg_supported = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__mei_hw_ops_pg_is_enabled__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_pg_is_enabled_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_pg_is_enabled__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_pg_is_enabled_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_intr_clear__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_intr_clear_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_intr_clear__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_intr_clear_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_intr_clear__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_intr_clear_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_intr_clear__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_intr_clear_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_intr_enable__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_intr_enable_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_intr_enable__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_intr_enable_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_intr_enable__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_intr_enable_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_intr_enable__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_intr_enable_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_intr_disable__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_intr_disable_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_intr_disable__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_intr_disable_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_intr_disable__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_intr_disable_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_intr_disable__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_intr_disable_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_hbuf_free_slots__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hbuf_free_slots_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_hbuf_free_slots__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hbuf_free_slots_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_hbuf_free_slots__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hbuf_free_slots_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_hbuf_free_slots__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hbuf_free_slots_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_hbuf_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hbuf_is_ready_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_hbuf_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hbuf_is_ready_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_hbuf_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hbuf_is_ready_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_hbuf_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hbuf_is_ready_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_hbuf_max_len__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hbuf_max_len_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_hbuf_max_len__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hbuf_max_len_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_hbuf_max_len__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_hbuf_max_len_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_hbuf_max_len__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_hbuf_max_len_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_write__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_write__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_write__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_write__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_write__header__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* ctx,
	struct mei_msg_hdr const* ptr)
{
	{
		glue_pack(__pos, msg, ext, ptr->me_addr);
	}

	{
		glue_pack(__pos, msg, ext, ptr->host_addr);
	}

	{
		glue_pack(__pos, msg, ext, ptr->length);
	}

	{
		glue_pack(__pos, msg, ext, ptr->internal);
	}

	{
		glue_pack(__pos, msg, ext, ptr->msg_complete);
	}

}

void callee_unmarshal_kernel__mei_hw_ops_write__header__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* ctx,
	struct mei_msg_hdr* ptr)
{

	{
		ptr->me_addr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		ptr->host_addr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		ptr->length = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		ptr->internal = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		ptr->msg_complete = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__mei_hw_ops_write__header__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* ctx,
	struct mei_msg_hdr const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_write__header__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_write_call_ctx const* ctx,
	struct mei_msg_hdr* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_rdbuf_full_slots__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_rdbuf_full_slots_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_rdbuf_full_slots__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_rdbuf_full_slots_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_rdbuf_full_slots__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_rdbuf_full_slots_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_rdbuf_full_slots__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_rdbuf_full_slots_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_read_hdr__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_read_hdr_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_read_hdr__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_read_hdr_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_read_hdr__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_read_hdr_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_read_hdr__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_read_hdr_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_hw_ops_read__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_read_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_hw_ops_read__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_read_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_hw_ops_read__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hw_ops_read_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_hw_ops_read__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hw_ops_read_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__finish_wait__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void callee_unmarshal_kernel__finish_wait__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void callee_marshal_kernel__finish_wait__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void caller_unmarshal_kernel__finish_wait__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void caller_marshal_kernel__finish_wait____wait_queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__finish_wait____wait_queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue* ptr)
{
	
}

void callee_marshal_kernel__finish_wait____wait_queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue const* ptr)
{
	
}

void caller_unmarshal_kernel__finish_wait____wait_queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue* ptr)
{
	
}

void caller_marshal_kernel__mei_deregister__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_deregister_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_deregister__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_deregister_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_deregister__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_deregister_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_deregister__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_deregister_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_register__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_register_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_register__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_register_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_register__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_register_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_register__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_register_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_register__parent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_register_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_register__parent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_register_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void callee_marshal_kernel__mei_register__parent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_register_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_register__parent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_register_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void caller_marshal_kernel__prepare_to_wait_event__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void callee_unmarshal_kernel__prepare_to_wait_event__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void callee_marshal_kernel__prepare_to_wait_event__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void caller_unmarshal_kernel__prepare_to_wait_event__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void caller_marshal_kernel__prepare_to_wait_event____wait_queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__prepare_to_wait_event____wait_queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue* ptr)
{
	
}

void callee_marshal_kernel__prepare_to_wait_event____wait_queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue const* ptr)
{
	
}

void caller_unmarshal_kernel__prepare_to_wait_event____wait_queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue* ptr)
{
	
}

void caller_marshal_kernel__queue_delayed_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* ctx,
	struct workqueue_struct const* ptr)
{
	
}

void callee_unmarshal_kernel__queue_delayed_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* ctx,
	struct workqueue_struct* ptr)
{
	
}

void callee_marshal_kernel__queue_delayed_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* ctx,
	struct workqueue_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__queue_delayed_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* ctx,
	struct workqueue_struct* ptr)
{
	
}

void caller_marshal_kernel__queue_delayed_work_on__dwork__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* ctx,
	struct delayed_work const* ptr)
{
	int const* cpu_ptr = &ptr->cpu;
	
	{
		glue_pack(__pos, msg, ext, *cpu_ptr);
	}

}

void callee_unmarshal_kernel__queue_delayed_work_on__dwork__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* ctx,
	struct delayed_work* ptr)
{
	int* cpu_ptr = &ptr->cpu;
	
	{
		*cpu_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void callee_marshal_kernel__queue_delayed_work_on__dwork__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* ctx,
	struct delayed_work const* ptr)
{
	int const* cpu_ptr = &ptr->cpu;
	
	{
		glue_pack(__pos, msg, ext, *cpu_ptr);
	}

}

void caller_unmarshal_kernel__queue_delayed_work_on__dwork__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* ctx,
	struct delayed_work* ptr)
{
	int* cpu_ptr = &ptr->cpu;
	
	{
		*cpu_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void caller_marshal_kernel__mei_irq_write_handler__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_irq_write_handler__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_irq_write_handler__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_irq_write_handler__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_irq_write_handler__cmpl_list__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct mei_cl_cb const* ptr)
{
	struct list_head const* list_ptr = &ptr->list;
	
	{
		caller_marshal_kernel__mei_irq_write_handler__list_head__out(__pos, msg, ext, ctx, list_ptr);
	}

}

void callee_unmarshal_kernel__mei_irq_write_handler__cmpl_list__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct mei_cl_cb* ptr)
{
	struct list_head* list_ptr = &ptr->list;
	
	{
		callee_unmarshal_kernel__mei_irq_write_handler__list_head__out(__pos, msg, ext, ctx, list_ptr);
	}

}

void callee_marshal_kernel__mei_irq_write_handler__cmpl_list__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct mei_cl_cb const* ptr)
{
	struct list_head const* list_ptr = &ptr->list;
	
	{
		callee_marshal_kernel__mei_irq_write_handler__list_head__out(__pos, msg, ext, ctx, list_ptr);
	}

}

void caller_unmarshal_kernel__mei_irq_write_handler__cmpl_list__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct mei_cl_cb* ptr)
{
	struct list_head* list_ptr = &ptr->list;
	
	{
		caller_unmarshal_kernel__mei_irq_write_handler__list_head__out(__pos, msg, ext, ctx, list_ptr);
	}

}

void caller_marshal_kernel__mei_irq_write_handler__list_head__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct list_head const* ptr)
{
	struct list_head* const* prev_ptr = &ptr->prev;
	struct list_head* const* next_ptr = &ptr->next;
	
	{
		if (*prev_ptr) {
			caller_marshal_kernel__mei_irq_write_handler__list_head__out(__pos, msg, ext, ctx, *prev_ptr);
		}

	}

	{
		if (*next_ptr) {
			caller_marshal_kernel__mei_irq_write_handler__list_head__out(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void callee_unmarshal_kernel__mei_irq_write_handler__list_head__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct list_head* ptr)
{
	struct list_head** prev_ptr = &ptr->prev;
	struct list_head** next_ptr = &ptr->next;
	
	{
		if (*prev_ptr) {
			callee_unmarshal_kernel__mei_irq_write_handler__list_head__out(__pos, msg, ext, ctx, *prev_ptr);
		}

	}

	{
		if (*next_ptr) {
			callee_unmarshal_kernel__mei_irq_write_handler__list_head__out(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void callee_marshal_kernel__mei_irq_write_handler__list_head__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct list_head const* ptr)
{
	struct list_head* const* prev_ptr = &ptr->prev;
	struct list_head* const* next_ptr = &ptr->next;
	
	{
		const void* __adjusted = *prev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*prev_ptr) {
			callee_marshal_kernel__mei_irq_write_handler__list_head__out(__pos, msg, ext, ctx, *prev_ptr);
		}

	}

	{
		const void* __adjusted = *next_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*next_ptr) {
			callee_marshal_kernel__mei_irq_write_handler__list_head__out(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void caller_unmarshal_kernel__mei_irq_write_handler__list_head__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct list_head* ptr)
{
	struct list_head** prev_ptr = &ptr->prev;
	struct list_head** next_ptr = &ptr->next;
	
	{
		*prev_ptr = glue_unpack(__pos, msg, ext, struct list_head*);
		if (*prev_ptr) {
			caller_unmarshal_kernel__mei_irq_write_handler__list_head__out(__pos, msg, ext, ctx, *prev_ptr);
		}

	}

	{
		*next_ptr = glue_unpack(__pos, msg, ext, struct list_head*);
		if (*next_ptr) {
			caller_unmarshal_kernel__mei_irq_write_handler__list_head__out(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void caller_marshal_kernel__mei_irq_read_handler__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_irq_read_handler__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_irq_read_handler__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_irq_read_handler__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_irq_read_handler__cmpl_list__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct mei_cl_cb const* ptr)
{
	struct list_head const* list_ptr = &ptr->list;
	
	{
		caller_marshal_kernel__mei_irq_read_handler__list_head__out(__pos, msg, ext, ctx, list_ptr);
	}

}

void callee_unmarshal_kernel__mei_irq_read_handler__cmpl_list__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct mei_cl_cb* ptr)
{
	struct list_head* list_ptr = &ptr->list;
	
	{
		callee_unmarshal_kernel__mei_irq_read_handler__list_head__out(__pos, msg, ext, ctx, list_ptr);
	}

}

void callee_marshal_kernel__mei_irq_read_handler__cmpl_list__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct mei_cl_cb const* ptr)
{
	struct list_head const* list_ptr = &ptr->list;
	
	{
		callee_marshal_kernel__mei_irq_read_handler__list_head__out(__pos, msg, ext, ctx, list_ptr);
	}

}

void caller_unmarshal_kernel__mei_irq_read_handler__cmpl_list__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct mei_cl_cb* ptr)
{
	struct list_head* list_ptr = &ptr->list;
	
	{
		caller_unmarshal_kernel__mei_irq_read_handler__list_head__out(__pos, msg, ext, ctx, list_ptr);
	}

}

void caller_marshal_kernel__mei_irq_read_handler__list_head__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct list_head const* ptr)
{
	struct list_head* const* prev_ptr = &ptr->prev;
	struct list_head* const* next_ptr = &ptr->next;
	
	{
		if (*prev_ptr) {
			caller_marshal_kernel__mei_irq_read_handler__list_head__out(__pos, msg, ext, ctx, *prev_ptr);
		}

	}

	{
		if (*next_ptr) {
			caller_marshal_kernel__mei_irq_read_handler__list_head__out(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void callee_unmarshal_kernel__mei_irq_read_handler__list_head__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct list_head* ptr)
{
	struct list_head** prev_ptr = &ptr->prev;
	struct list_head** next_ptr = &ptr->next;
	
	{
		if (*prev_ptr) {
			callee_unmarshal_kernel__mei_irq_read_handler__list_head__out(__pos, msg, ext, ctx, *prev_ptr);
		}

	}

	{
		if (*next_ptr) {
			callee_unmarshal_kernel__mei_irq_read_handler__list_head__out(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void callee_marshal_kernel__mei_irq_read_handler__list_head__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct list_head const* ptr)
{
	struct list_head* const* prev_ptr = &ptr->prev;
	struct list_head* const* next_ptr = &ptr->next;
	
	{
		const void* __adjusted = *prev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*prev_ptr) {
			callee_marshal_kernel__mei_irq_read_handler__list_head__out(__pos, msg, ext, ctx, *prev_ptr);
		}

	}

	{
		const void* __adjusted = *next_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*next_ptr) {
			callee_marshal_kernel__mei_irq_read_handler__list_head__out(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void caller_unmarshal_kernel__mei_irq_read_handler__list_head__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct list_head* ptr)
{
	struct list_head** prev_ptr = &ptr->prev;
	struct list_head** next_ptr = &ptr->next;
	
	{
		*prev_ptr = glue_unpack(__pos, msg, ext, struct list_head*);
		if (*prev_ptr) {
			caller_unmarshal_kernel__mei_irq_read_handler__list_head__out(__pos, msg, ext, ctx, *prev_ptr);
		}

	}

	{
		*next_ptr = glue_unpack(__pos, msg, ext, struct list_head*);
		if (*next_ptr) {
			caller_unmarshal_kernel__mei_irq_read_handler__list_head__out(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void caller_marshal_kernel__mei_irq_compl_handler__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct device* const* dev_ptr = &ptr->dev;
	
	{
		const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_irq_compl_handler__mei_device_dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_unmarshal_kernel__mei_irq_compl_handler__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct device** dev_ptr = &ptr->dev;
	
	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_irq_compl_handler__mei_device_dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_marshal_kernel__mei_irq_compl_handler__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct device* const* dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_irq_compl_handler__mei_device_dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_unmarshal_kernel__mei_irq_compl_handler__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct device** dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_irq_compl_handler__mei_device_dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_marshal_kernel__mei_irq_compl_handler__mei_device_dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_irq_compl_handler__mei_device_dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void callee_marshal_kernel__mei_irq_compl_handler__mei_device_dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_irq_compl_handler__mei_device_dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void caller_marshal_kernel__mei_irq_compl_handler__compl_list__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_cl_cb const* ptr)
{
	struct list_head const* list_ptr = &ptr->list;
	
	{
		caller_marshal_kernel__mei_irq_compl_handler__list_head__out(__pos, msg, ext, ctx, list_ptr);
	}

}

void callee_unmarshal_kernel__mei_irq_compl_handler__compl_list__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_cl_cb* ptr)
{
	struct list_head* list_ptr = &ptr->list;
	
	{
		callee_unmarshal_kernel__mei_irq_compl_handler__list_head__out(__pos, msg, ext, ctx, list_ptr);
	}

}

void callee_marshal_kernel__mei_irq_compl_handler__compl_list__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_cl_cb const* ptr)
{
	struct list_head const* list_ptr = &ptr->list;
	
	{
		callee_marshal_kernel__mei_irq_compl_handler__list_head__out(__pos, msg, ext, ctx, list_ptr);
	}

}

void caller_unmarshal_kernel__mei_irq_compl_handler__compl_list__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_cl_cb* ptr)
{
	struct list_head* list_ptr = &ptr->list;
	
	{
		caller_unmarshal_kernel__mei_irq_compl_handler__list_head__out(__pos, msg, ext, ctx, list_ptr);
	}

}

void caller_marshal_kernel__mei_irq_compl_handler__list_head__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct list_head const* ptr)
{
	struct list_head* const* prev_ptr = &ptr->prev;
	struct list_head* const* next_ptr = &ptr->next;
	
	{
		if (*prev_ptr) {
			caller_marshal_kernel__mei_irq_compl_handler__list_head__out(__pos, msg, ext, ctx, *prev_ptr);
		}

	}

	{
		if (*next_ptr) {
			caller_marshal_kernel__mei_irq_compl_handler__list_head__out(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void callee_unmarshal_kernel__mei_irq_compl_handler__list_head__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct list_head* ptr)
{
	struct list_head** prev_ptr = &ptr->prev;
	struct list_head** next_ptr = &ptr->next;
	
	{
		if (*prev_ptr) {
			callee_unmarshal_kernel__mei_irq_compl_handler__list_head__out(__pos, msg, ext, ctx, *prev_ptr);
		}

	}

	{
		if (*next_ptr) {
			callee_unmarshal_kernel__mei_irq_compl_handler__list_head__out(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void callee_marshal_kernel__mei_irq_compl_handler__list_head__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct list_head const* ptr)
{
	struct list_head* const* prev_ptr = &ptr->prev;
	struct list_head* const* next_ptr = &ptr->next;
	
	{
		const void* __adjusted = *prev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*prev_ptr) {
			callee_marshal_kernel__mei_irq_compl_handler__list_head__out(__pos, msg, ext, ctx, *prev_ptr);
		}

	}

	{
		const void* __adjusted = *next_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*next_ptr) {
			callee_marshal_kernel__mei_irq_compl_handler__list_head__out(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void caller_unmarshal_kernel__mei_irq_compl_handler__list_head__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct list_head* ptr)
{
	struct list_head** prev_ptr = &ptr->prev;
	struct list_head** next_ptr = &ptr->next;
	
	{
		*prev_ptr = glue_unpack(__pos, msg, ext, struct list_head*);
		if (*prev_ptr) {
			caller_unmarshal_kernel__mei_irq_compl_handler__list_head__out(__pos, msg, ext, ctx, *prev_ptr);
		}

	}

	{
		*next_ptr = glue_unpack(__pos, msg, ext, struct list_head*);
		if (*next_ptr) {
			caller_unmarshal_kernel__mei_irq_compl_handler__list_head__out(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void caller_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	unsigned int const* devfn_ptr = &ptr->devfn;
	
	{
		const void* __adjusted = *bus_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__pci_disable_msi__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *devfn_ptr);
	}

}

void callee_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	unsigned int* devfn_ptr = &ptr->devfn;
	
	{
		*bus_ptr = glue_unpack(__pos, msg, ext, struct pci_bus*);
		if (*bus_ptr) {
			callee_unmarshal_kernel__pci_disable_msi__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		*devfn_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	unsigned int const* irq_ptr = &ptr->irq;
	
	{
		if (*bus_ptr) {
			callee_marshal_kernel__pci_disable_msi__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *irq_ptr);
	}

	{
		glue_pack(__pos, msg, ext, ptr->msi_enabled);
	}

}

void caller_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	unsigned int* irq_ptr = &ptr->irq;
	
	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__pci_disable_msi__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		*irq_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		ptr->msi_enabled = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void caller_marshal_kernel__pci_disable_msi__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_disable_msi__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void callee_marshal_kernel__pci_disable_msi__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_disable_msi__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void caller_marshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* ctx,
	struct workqueue_struct const* ptr)
{
	
}

void callee_unmarshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* ctx,
	struct workqueue_struct* ptr)
{
	
}

void callee_marshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* ctx,
	struct workqueue_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* ctx,
	struct workqueue_struct* ptr)
{
	
}

void caller_marshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* ctx,
	struct work_struct const* ptr)
{
	
}

void callee_unmarshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* ctx,
	struct work_struct* ptr)
{
	
}

void callee_marshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* ctx,
	struct work_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* ctx,
	struct work_struct* ptr)
{
	
}

void caller_marshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	
	{
		const void* __adjusted = *bus_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__pci_enable_msi_range__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void callee_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	
	{
		*bus_ptr = glue_unpack(__pos, msg, ext, struct pci_bus*);
		if (*bus_ptr) {
			callee_unmarshal_kernel__pci_enable_msi_range__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void callee_marshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	unsigned int const* irq_ptr = &ptr->irq;
	
	{
		if (*bus_ptr) {
			callee_marshal_kernel__pci_enable_msi_range__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, ptr->msi_enabled);
	}

	{
		glue_pack(__pos, msg, ext, ptr->msix_enabled);
	}

	{
		glue_pack(__pos, msg, ext, *irq_ptr);
	}

}

void caller_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	unsigned int* irq_ptr = &ptr->irq;
	
	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__pci_enable_msi_range__pci_dev_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		ptr->msi_enabled = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		ptr->msix_enabled = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*irq_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void caller_marshal_kernel__pci_enable_msi_range__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_enable_msi_range__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void callee_marshal_kernel__pci_enable_msi_range__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_enable_msi_range__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void caller_marshal_kernel__pci_iounmap__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_iounmap_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_iounmap__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_iounmap_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_iounmap__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_iounmap_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_iounmap__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_iounmap_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel____wake_up__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __wake_up_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void callee_unmarshal_kernel____wake_up__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __wake_up_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void callee_marshal_kernel____wake_up__wq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __wake_up_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void caller_unmarshal_kernel____wake_up__wq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __wake_up_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
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

