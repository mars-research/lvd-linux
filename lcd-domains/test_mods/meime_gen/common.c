#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel__probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__probe__ent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_device_id const* ptr)
{
	unsigned long const* driver_data_ptr = &ptr->driver_data;
	
	{
		glue_pack(__pos, msg, ext, *driver_data_ptr);
	}

}

void callee_unmarshal_kernel__probe__ent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_device_id* ptr)
{
	unsigned long* driver_data_ptr = &ptr->driver_data;
	
	{
		*driver_data_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

}

void callee_marshal_kernel__probe__ent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_device_id const* ptr)
{
	
}

void caller_unmarshal_kernel__probe__ent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_device_id* ptr)
{
	
}

void caller_marshal_kernel__remove__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct remove_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct remove_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__remove__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct remove_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct remove_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__shutdown__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct shutdown_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__shutdown__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct shutdown_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__shutdown__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct shutdown_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__shutdown__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct shutdown_call_ctx const* ctx,
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

void caller_marshal_kernel__host_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct host_is_ready_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__host_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct host_is_ready_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__host_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct host_is_ready_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__host_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct host_is_ready_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__hw_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct hw_is_ready_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__hw_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct hw_is_ready_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__hw_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct hw_is_ready_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__hw_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct hw_is_ready_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__hw_reset__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct hw_reset_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		glue_pack_shadow(__pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__hw_reset__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_unmarshal_kernel__hw_reset__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct hw_reset_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__hw_reset__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_marshal_kernel__hw_reset__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct hw_reset_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			callee_marshal_kernel__hw_reset__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_unmarshal_kernel__hw_reset__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct hw_reset_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__hw_reset__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_marshal_kernel__hw_start__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct hw_start_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		glue_pack_shadow(__pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__hw_start__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_unmarshal_kernel__hw_start__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct hw_start_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__hw_start__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_marshal_kernel__hw_start__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct hw_start_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			callee_marshal_kernel__hw_start__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_unmarshal_kernel__hw_start__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct hw_start_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__hw_start__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_marshal_kernel__hw_config__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct hw_config_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		glue_pack_shadow(__pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__hw_config__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_unmarshal_kernel__hw_config__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct hw_config_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__hw_config__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_marshal_kernel__hw_config__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct hw_config_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			callee_marshal_kernel__hw_config__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_unmarshal_kernel__hw_config__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct hw_config_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__hw_config__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_marshal_kernel__fw_status__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct fw_status_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		glue_pack_shadow(__pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__fw_status__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_unmarshal_kernel__fw_status__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct fw_status_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__fw_status__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_marshal_kernel__fw_status__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct fw_status_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			callee_marshal_kernel__fw_status__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_unmarshal_kernel__fw_status__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct fw_status_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__fw_status__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_marshal_kernel__fw_status__fw_status__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct fw_status_call_ctx const* ctx,
	struct mei_fw_status const* ptr)
{
	unsigned int const* status_ptr = ptr->status;
	
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

void callee_unmarshal_kernel__fw_status__fw_status__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct fw_status_call_ctx const* ctx,
	struct mei_fw_status* ptr)
{
	unsigned int* status_ptr = ptr->status;
	
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

void callee_marshal_kernel__fw_status__fw_status__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct fw_status_call_ctx const* ctx,
	struct mei_fw_status const* ptr)
{
	int const* count_ptr = &ptr->count;
	
	{
		glue_pack(__pos, msg, ext, *count_ptr);
	}

}

void caller_unmarshal_kernel__fw_status__fw_status__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct fw_status_call_ctx const* ctx,
	struct mei_fw_status* ptr)
{
	int* count_ptr = &ptr->count;
	
	{
		*count_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void caller_marshal_kernel__pg_state__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pg_state_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__pg_state__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pg_state_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__pg_state__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pg_state_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__pg_state__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pg_state_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__pg_in_transition__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pg_in_transition_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	unsigned int const* pg_event_ptr = &ptr->pg_event;
	
	{
		glue_pack(__pos, msg, ext, *pg_event_ptr);
	}

}

void callee_unmarshal_kernel__pg_in_transition__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pg_in_transition_call_ctx const* ctx,
	struct mei_device* ptr)
{
	unsigned int* pg_event_ptr = &ptr->pg_event;
	
	{
		*pg_event_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__pg_in_transition__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pg_in_transition_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__pg_in_transition__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pg_in_transition_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__pg_is_enabled__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pg_is_enabled_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		glue_pack_shadow(__pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__pg_is_enabled__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_unmarshal_kernel__pg_is_enabled__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pg_is_enabled_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pg_is_enabled__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_marshal_kernel__pg_is_enabled__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pg_is_enabled_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pg_is_enabled__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_unmarshal_kernel__pg_is_enabled__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pg_is_enabled_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pg_is_enabled__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_marshal_kernel__intr_clear__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct intr_clear_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__intr_clear__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct intr_clear_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__intr_clear__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct intr_clear_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__intr_clear__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct intr_clear_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__intr_enable__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct intr_enable_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__intr_enable__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct intr_enable_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__intr_enable__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct intr_enable_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__intr_enable__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct intr_enable_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__intr_disable__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct intr_disable_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__intr_disable__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct intr_disable_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__intr_disable__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct intr_disable_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__intr_disable__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct intr_disable_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__hbuf_free_slots__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct hbuf_free_slots_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__hbuf_free_slots__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct hbuf_free_slots_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__hbuf_free_slots__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct hbuf_free_slots_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__hbuf_free_slots__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct hbuf_free_slots_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__hbuf_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct hbuf_is_ready_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__hbuf_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct hbuf_is_ready_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__hbuf_is_ready__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct hbuf_is_ready_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__hbuf_is_ready__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct hbuf_is_ready_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__hbuf_max_len__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct hbuf_max_len_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__hbuf_max_len__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct hbuf_max_len_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__hbuf_max_len__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct hbuf_max_len_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__hbuf_max_len__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct hbuf_max_len_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__write__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct write_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		glue_pack_shadow(__pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__write__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_unmarshal_kernel__write__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct write_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__write__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_marshal_kernel__write__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct write_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			callee_marshal_kernel__write__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_unmarshal_kernel__write__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct write_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__write__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_marshal_kernel__write__header__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct write_call_ctx const* ctx,
	struct mei_msg_hdr const* ptr)
{
	
}

void callee_unmarshal_kernel__write__header__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct write_call_ctx const* ctx,
	struct mei_msg_hdr* ptr)
{
	
}

void callee_marshal_kernel__write__header__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct write_call_ctx const* ctx,
	struct mei_msg_hdr const* ptr)
{
	
}

void caller_unmarshal_kernel__write__header__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct write_call_ctx const* ctx,
	struct mei_msg_hdr* ptr)
{
	
}

void caller_marshal_kernel__rdbuf_full_slots__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rdbuf_full_slots_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		glue_pack_shadow(__pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__rdbuf_full_slots__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_unmarshal_kernel__rdbuf_full_slots__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rdbuf_full_slots_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__rdbuf_full_slots__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_marshal_kernel__rdbuf_full_slots__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct rdbuf_full_slots_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			callee_marshal_kernel__rdbuf_full_slots__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_unmarshal_kernel__rdbuf_full_slots__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct rdbuf_full_slots_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__rdbuf_full_slots__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_marshal_kernel__read_hdr__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct read_hdr_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__read_hdr__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct read_hdr_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__read_hdr__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct read_hdr_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__read_hdr__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct read_hdr_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__read__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct read_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__read__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct read_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__read__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct read_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__read__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct read_call_ctx const* ctx,
	struct mei_device* ptr)
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

void caller_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
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
	
}

void callee_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_dev* ptr)
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

void caller_marshal_kernel__finish_wait__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void callee_unmarshal_kernel__finish_wait__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void callee_marshal_kernel__finish_wait__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void caller_unmarshal_kernel__finish_wait__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void caller_marshal_kernel__finish_wait__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__finish_wait__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue* ptr)
{
	
}

void callee_marshal_kernel__finish_wait__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue const* ptr)
{
	
}

void caller_unmarshal_kernel__finish_wait__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue* ptr)
{
	
}

void caller_marshal_kernel__prepare_to_wait_event__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void callee_unmarshal_kernel__prepare_to_wait_event__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void callee_marshal_kernel__prepare_to_wait_event__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void caller_unmarshal_kernel__prepare_to_wait_event__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void caller_marshal_kernel__prepare_to_wait_event__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue const* ptr)
{
	unsigned int const* flags_ptr = &ptr->flags;
	
	{
		glue_pack(__pos, msg, ext, *flags_ptr);
	}

}

void callee_unmarshal_kernel__prepare_to_wait_event__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue* ptr)
{
	unsigned int* flags_ptr = &ptr->flags;
	
	{
		*flags_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__prepare_to_wait_event__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue const* ptr)
{
	
}

void caller_unmarshal_kernel__prepare_to_wait_event__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_event_call_ctx const* ctx,
	struct __wait_queue* ptr)
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
	
}

void callee_unmarshal_kernel__queue_delayed_work_on__dwork__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* ctx,
	struct delayed_work* ptr)
{
	
}

void callee_marshal_kernel__queue_delayed_work_on__dwork__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* ctx,
	struct delayed_work const* ptr)
{
	
}

void caller_unmarshal_kernel__queue_delayed_work_on__dwork__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_delayed_work_on_call_ctx const* ctx,
	struct delayed_work* ptr)
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
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		glue_pack_shadow(__pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_hbm_pg__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_unmarshal_kernel__mei_hbm_pg__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hbm_pg_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_hbm_pg__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_marshal_kernel__mei_hbm_pg__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_hbm_pg_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_hbm_pg__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_unmarshal_kernel__mei_hbm_pg__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_hbm_pg_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_hbm_pg__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void caller_marshal_kernel__mei_device_init__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct delayed_work const* timer_work_ptr = &ptr->timer_work;
	
	{
		caller_marshal_kernel__mei_device_init__delayed_work__in(__pos, msg, ext, ctx, timer_work_ptr);
	}

}

void callee_unmarshal_kernel__mei_device_init__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct delayed_work* timer_work_ptr = &ptr->timer_work;
	
	{
		callee_unmarshal_kernel__mei_device_init__delayed_work__in(__pos, msg, ext, ctx, timer_work_ptr);
	}

}

void callee_marshal_kernel__mei_device_init__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct delayed_work const* timer_work_ptr = &ptr->timer_work;
	unsigned int const* dev_state_ptr = &ptr->dev_state;
	unsigned int const* pg_event_ptr = &ptr->pg_event;
	
	{
		callee_marshal_kernel__mei_device_init__delayed_work__in(__pos, msg, ext, ctx, timer_work_ptr);
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
	struct delayed_work* timer_work_ptr = &ptr->timer_work;
	unsigned int* dev_state_ptr = &ptr->dev_state;
	unsigned int* pg_event_ptr = &ptr->pg_event;
	
	{
		caller_unmarshal_kernel__mei_device_init__delayed_work__in(__pos, msg, ext, ctx, timer_work_ptr);
	}

	{
		*dev_state_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*pg_event_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void caller_marshal_kernel__mei_device_init__delayed_work__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct delayed_work const* ptr)
{
	struct work_struct const* work_ptr = &ptr->work;
	struct timer_list const* timer_ptr = &ptr->timer;
	
	{
		caller_marshal_kernel__mei_device_init__work_struct__in(__pos, msg, ext, ctx, work_ptr);
	}

	{
		caller_marshal_kernel__mei_device_init__timer_list__in(__pos, msg, ext, ctx, timer_ptr);
	}

}

void callee_unmarshal_kernel__mei_device_init__delayed_work__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct delayed_work* ptr)
{
	struct work_struct* work_ptr = &ptr->work;
	struct timer_list* timer_ptr = &ptr->timer;
	
	{
		callee_unmarshal_kernel__mei_device_init__work_struct__in(__pos, msg, ext, ctx, work_ptr);
	}

	{
		callee_unmarshal_kernel__mei_device_init__timer_list__in(__pos, msg, ext, ctx, timer_ptr);
	}

}

void callee_marshal_kernel__mei_device_init__delayed_work__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct delayed_work const* ptr)
{
	struct work_struct const* work_ptr = &ptr->work;
	struct timer_list const* timer_ptr = &ptr->timer;
	
	{
		callee_marshal_kernel__mei_device_init__work_struct__in(__pos, msg, ext, ctx, work_ptr);
	}

	{
		callee_marshal_kernel__mei_device_init__timer_list__in(__pos, msg, ext, ctx, timer_ptr);
	}

}

void caller_unmarshal_kernel__mei_device_init__delayed_work__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct delayed_work* ptr)
{
	struct work_struct* work_ptr = &ptr->work;
	struct timer_list* timer_ptr = &ptr->timer;
	
	{
		caller_unmarshal_kernel__mei_device_init__work_struct__in(__pos, msg, ext, ctx, work_ptr);
	}

	{
		caller_unmarshal_kernel__mei_device_init__timer_list__in(__pos, msg, ext, ctx, timer_ptr);
	}

}

void caller_marshal_kernel__mei_device_init__work_struct__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct work_struct const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_device_init__work_struct__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct work_struct* ptr)
{
	
}

void callee_marshal_kernel__mei_device_init__work_struct__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct work_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_device_init__work_struct__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct work_struct* ptr)
{
	
}

void caller_marshal_kernel__mei_device_init__timer_list__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct timer_list const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_device_init__timer_list__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct timer_list* ptr)
{
	
}

void callee_marshal_kernel__mei_device_init__timer_list__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct timer_list const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_device_init__timer_list__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_device_init_call_ctx const* ctx,
	struct timer_list* ptr)
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
	fptr_fw_status const* fw_status_ptr = &ptr->fw_status;
	fptr_hbuf_free_slots const* hbuf_free_slots_ptr = &ptr->hbuf_free_slots;
	fptr_hbuf_is_ready const* hbuf_is_ready_ptr = &ptr->hbuf_is_ready;
	fptr_hbuf_max_len const* hbuf_max_len_ptr = &ptr->hbuf_max_len;
	fptr_host_is_ready const* host_is_ready_ptr = &ptr->host_is_ready;
	fptr_hw_config const* hw_config_ptr = &ptr->hw_config;
	fptr_hw_is_ready const* hw_is_ready_ptr = &ptr->hw_is_ready;
	fptr_hw_reset const* hw_reset_ptr = &ptr->hw_reset;
	fptr_hw_start const* hw_start_ptr = &ptr->hw_start;
	fptr_intr_clear const* intr_clear_ptr = &ptr->intr_clear;
	fptr_intr_disable const* intr_disable_ptr = &ptr->intr_disable;
	fptr_intr_enable const* intr_enable_ptr = &ptr->intr_enable;
	fptr_pg_in_transition const* pg_in_transition_ptr = &ptr->pg_in_transition;
	fptr_pg_is_enabled const* pg_is_enabled_ptr = &ptr->pg_is_enabled;
	fptr_pg_state const* pg_state_ptr = &ptr->pg_state;
	fptr_rdbuf_full_slots const* rdbuf_full_slots_ptr = &ptr->rdbuf_full_slots;
	fptr_read const* read_ptr = &ptr->read;
	fptr_read_hdr const* read_hdr_ptr = &ptr->read_hdr;
	fptr_write const* write_ptr = &ptr->write;
	
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
	fptr_fw_status* fw_status_ptr = &ptr->fw_status;
	fptr_hbuf_free_slots* hbuf_free_slots_ptr = &ptr->hbuf_free_slots;
	fptr_hbuf_is_ready* hbuf_is_ready_ptr = &ptr->hbuf_is_ready;
	fptr_hbuf_max_len* hbuf_max_len_ptr = &ptr->hbuf_max_len;
	fptr_host_is_ready* host_is_ready_ptr = &ptr->host_is_ready;
	fptr_hw_config* hw_config_ptr = &ptr->hw_config;
	fptr_hw_is_ready* hw_is_ready_ptr = &ptr->hw_is_ready;
	fptr_hw_reset* hw_reset_ptr = &ptr->hw_reset;
	fptr_hw_start* hw_start_ptr = &ptr->hw_start;
	fptr_intr_clear* intr_clear_ptr = &ptr->intr_clear;
	fptr_intr_disable* intr_disable_ptr = &ptr->intr_disable;
	fptr_intr_enable* intr_enable_ptr = &ptr->intr_enable;
	fptr_pg_in_transition* pg_in_transition_ptr = &ptr->pg_in_transition;
	fptr_pg_is_enabled* pg_is_enabled_ptr = &ptr->pg_is_enabled;
	fptr_pg_state* pg_state_ptr = &ptr->pg_state;
	fptr_rdbuf_full_slots* rdbuf_full_slots_ptr = &ptr->rdbuf_full_slots;
	fptr_read* read_ptr = &ptr->read;
	fptr_read_hdr* read_hdr_ptr = &ptr->read_hdr;
	fptr_write* write_ptr = &ptr->write;
	
	{
		*fw_status_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, fw_status);
	}

	{
		*hbuf_free_slots_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, hbuf_free_slots);
	}

	{
		*hbuf_is_ready_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, hbuf_is_ready);
	}

	{
		*hbuf_max_len_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, hbuf_max_len);
	}

	{
		*host_is_ready_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, host_is_ready);
	}

	{
		*hw_config_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, hw_config);
	}

	{
		*hw_is_ready_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, hw_is_ready);
	}

	{
		*hw_reset_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, hw_reset);
	}

	{
		*hw_start_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, hw_start);
	}

	{
		*intr_clear_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, intr_clear);
	}

	{
		*intr_disable_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, intr_disable);
	}

	{
		*intr_enable_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, intr_enable);
	}

	{
		*pg_in_transition_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, pg_in_transition);
	}

	{
		*pg_is_enabled_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, pg_is_enabled);
	}

	{
		*pg_state_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, pg_state);
	}

	{
		*rdbuf_full_slots_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, rdbuf_full_slots);
	}

	{
		*read_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, read);
	}

	{
		*read_hdr_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, read_hdr);
	}

	{
		*write_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, write);
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
	struct mei_device* const* dev_ptr = &ptr->dev;
	
	{
		glue_pack_shadow(__pos, msg, ext, *dev_ptr);
		if (*dev_ptr) {
			caller_marshal_kernel__mei_stop__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_unmarshal_kernel__mei_stop__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_stop_call_ctx const* ctx,
	struct mei_device* ptr)
{
	struct mei_device** dev_ptr = &ptr->dev;
	
	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct mei_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mei_stop__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

}

void callee_marshal_kernel__mei_stop__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_stop_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	struct mei_device* const* dev_ptr = &ptr->dev;
	unsigned int const* dev_state_ptr = &ptr->dev_state;
	
	{
		if (*dev_ptr) {
			callee_marshal_kernel__mei_stop__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

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
	struct mei_device** dev_ptr = &ptr->dev;
	unsigned int* dev_state_ptr = &ptr->dev_state;
	
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mei_stop__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*dev_state_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

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

void caller_marshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_unregister_driver_call_ctx const* ctx,
	struct pci_driver const* ptr)
{
	fptr_probe const* probe_ptr = &ptr->probe;
	fptr_remove const* remove_ptr = &ptr->remove;
	fptr_shutdown const* shutdown_ptr = &ptr->shutdown;
	
	{
		glue_pack(__pos, msg, ext, *probe_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *remove_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *shutdown_ptr);
	}

}

void callee_unmarshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_unregister_driver_call_ctx const* ctx,
	struct pci_driver* ptr)
{
	fptr_probe* probe_ptr = &ptr->probe;
	fptr_remove* remove_ptr = &ptr->remove;
	fptr_shutdown* shutdown_ptr = &ptr->shutdown;
	
	{
		*probe_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, probe);
	}

	{
		*remove_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, remove);
	}

	{
		*shutdown_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, shutdown);
	}

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
	fptr_probe const* probe_ptr = &ptr->probe;
	fptr_remove const* remove_ptr = &ptr->remove;
	fptr_shutdown const* shutdown_ptr = &ptr->shutdown;
	
	{
		glue_pack(__pos, msg, ext, *name_ptr);
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

}

void callee_unmarshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct pci_driver* ptr)
{
	char const** name_ptr = &ptr->name;
	fptr_probe* probe_ptr = &ptr->probe;
	fptr_remove* remove_ptr = &ptr->remove;
	fptr_shutdown* shutdown_ptr = &ptr->shutdown;
	
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
		*probe_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, probe);
	}

	{
		*remove_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, remove);
	}

	{
		*shutdown_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, shutdown);
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
	
	{
		(void)name_ptr;
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
	
	{
		(void)name_ptr;
	}

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
		glue_pack_shadow(__pos, msg, ext, *bus_ptr);
		if (*bus_ptr) {
			caller_marshal_kernel__pci_dev_run_wake__bus__in(__pos, msg, ext, ctx, *bus_ptr);
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
			callee_unmarshal_kernel__pci_dev_run_wake__bus__in(__pos, msg, ext, ctx, *bus_ptr);
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
			callee_marshal_kernel__pci_dev_run_wake__bus__in(__pos, msg, ext, ctx, *bus_ptr);
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
			caller_unmarshal_kernel__pci_dev_run_wake__bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void caller_marshal_kernel__pci_dev_run_wake__bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_dev_run_wake__bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void callee_marshal_kernel__pci_dev_run_wake__bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_run_wake_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_dev_run_wake__bus__in(
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
	
}

void callee_unmarshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
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

void caller_marshal_kernel__mei_irq_write_handler__cmpl_list__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct mei_cl_cb const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_irq_write_handler__cmpl_list__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct mei_cl_cb* ptr)
{
	
}

void callee_marshal_kernel__mei_irq_write_handler__cmpl_list__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct mei_cl_cb const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_irq_write_handler__cmpl_list__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_write_handler_call_ctx const* ctx,
	struct mei_cl_cb* ptr)
{
	
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

void caller_marshal_kernel__mei_irq_read_handler__cmpl_list__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct mei_cl_cb const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_irq_read_handler__cmpl_list__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct mei_cl_cb* ptr)
{
	
}

void callee_marshal_kernel__mei_irq_read_handler__cmpl_list__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct mei_cl_cb const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_irq_read_handler__cmpl_list__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_read_handler_call_ctx const* ctx,
	struct mei_cl_cb* ptr)
{
	
}

void caller_marshal_kernel__mei_irq_compl_handler__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_irq_compl_handler__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void callee_marshal_kernel__mei_irq_compl_handler__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_irq_compl_handler__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_device* ptr)
{
	
}

void caller_marshal_kernel__mei_irq_compl_handler__compl_list__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_cl_cb const* ptr)
{
	struct list_head const* list_ptr = &ptr->list;
	
	{
		caller_marshal_kernel__mei_irq_compl_handler__list_head__in(__pos, msg, ext, ctx, list_ptr);
	}

}

void callee_unmarshal_kernel__mei_irq_compl_handler__compl_list__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_cl_cb* ptr)
{
	struct list_head* list_ptr = &ptr->list;
	
	{
		callee_unmarshal_kernel__mei_irq_compl_handler__list_head__in(__pos, msg, ext, ctx, list_ptr);
	}

}

void callee_marshal_kernel__mei_irq_compl_handler__compl_list__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_cl_cb const* ptr)
{
	struct list_head const* list_ptr = &ptr->list;
	
	{
		callee_marshal_kernel__mei_irq_compl_handler__list_head__in(__pos, msg, ext, ctx, list_ptr);
	}

}

void caller_unmarshal_kernel__mei_irq_compl_handler__compl_list__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct mei_cl_cb* ptr)
{
	struct list_head* list_ptr = &ptr->list;
	
	{
		caller_unmarshal_kernel__mei_irq_compl_handler__list_head__in(__pos, msg, ext, ctx, list_ptr);
	}

}

void caller_marshal_kernel__mei_irq_compl_handler__list_head__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct list_head const* ptr)
{
	struct list_head* const* next_ptr = &ptr->next;
	
	{
		glue_pack_shadow(__pos, msg, ext, *next_ptr);
		if (*next_ptr) {
			caller_marshal_kernel__mei_irq_compl_handler__next__in(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void callee_unmarshal_kernel__mei_irq_compl_handler__list_head__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct list_head* ptr)
{
	struct list_head** next_ptr = &ptr->next;
	
	{
		*next_ptr = glue_unpack(__pos, msg, ext, struct list_head*);
		if (*next_ptr) {
			callee_unmarshal_kernel__mei_irq_compl_handler__next__in(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void callee_marshal_kernel__mei_irq_compl_handler__list_head__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct list_head const* ptr)
{
	struct list_head* const* next_ptr = &ptr->next;
	
	{
		if (*next_ptr) {
			callee_marshal_kernel__mei_irq_compl_handler__next__in(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void caller_unmarshal_kernel__mei_irq_compl_handler__list_head__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct list_head* ptr)
{
	struct list_head** next_ptr = &ptr->next;
	
	{
		if (*next_ptr) {
			caller_unmarshal_kernel__mei_irq_compl_handler__next__in(__pos, msg, ext, ctx, *next_ptr);
		}

	}

}

void caller_marshal_kernel__mei_irq_compl_handler__next__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct list_head const* ptr)
{
	
}

void callee_unmarshal_kernel__mei_irq_compl_handler__next__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct list_head* ptr)
{
	
}

void callee_marshal_kernel__mei_irq_compl_handler__next__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct list_head const* ptr)
{
	
}

void caller_unmarshal_kernel__mei_irq_compl_handler__next__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mei_irq_compl_handler_call_ctx const* ctx,
	struct list_head* ptr)
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


#ifdef LCD_ISOLATE
__attribute__((weak)) void shared_mem_init(void) {
	LIBLCD_MSG("Weak shared_mem_init does nothing! Override if you want!");
}
#else
__attribute__((weak)) void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext) {
	LIBLCD_MSG("Weak shared_mem_init_callee does nothing! Override if you want!");
}
#endif	/* LCD_ISOLATE */

