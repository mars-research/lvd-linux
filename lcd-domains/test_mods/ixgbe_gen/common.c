#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel__probe__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	unsigned int const* devfn_ptr = &ptr->devfn;
	unsigned short const* vendor_ptr = &ptr->vendor;
	unsigned short const* device_ptr = &ptr->device;
	unsigned short const* subsystem_vendor_ptr = &ptr->subsystem_vendor;
	unsigned short const* subsystem_device_ptr = &ptr->subsystem_device;
	unsigned char const* revision_ptr = &ptr->revision;
	unsigned int __is_virtfn = ptr->is_virtfn;
	unsigned int const* __is_virtfn_ptr = &__is_virtfn;
	struct device const* dev_ptr = &ptr->dev;
	struct resource const* resource_ptr = ptr->resource;

	{
		__maybe_unused const void* __adjusted = *bus_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__probe__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *devfn_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *vendor_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *device_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *subsystem_vendor_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *subsystem_device_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *revision_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__is_virtfn_ptr);
	}

	{
		caller_marshal_kernel__probe__device__in(__pos, __msg, __ext, ctx, dev_ptr);
	}

	{
		size_t i, len = 11;
		struct resource const* array = resource_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			struct resource const* element = &array[i];
			caller_marshal_kernel__probe__resource__in(__pos, __msg, __ext, ctx, element);
		}

	}

}

void callee_unmarshal_kernel__probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	unsigned int* devfn_ptr = &ptr->devfn;
	unsigned short* vendor_ptr = &ptr->vendor;
	unsigned short* device_ptr = &ptr->device;
	unsigned short* subsystem_vendor_ptr = &ptr->subsystem_vendor;
	unsigned short* subsystem_device_ptr = &ptr->subsystem_device;
	unsigned char* revision_ptr = &ptr->revision;
	unsigned int __is_virtfn = ptr->is_virtfn;
	unsigned int* __is_virtfn_ptr = &__is_virtfn;
	struct device* dev_ptr = &ptr->dev;
	struct resource* resource_ptr = ptr->resource;

	{
		size_t __size = sizeof(struct pci_bus);
		*bus_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct pci_bus*, (__size), (DEFAULT_GFP_FLAGS));
		if (*bus_ptr) {
			callee_unmarshal_kernel__probe__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
		*devfn_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*vendor_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*device_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*subsystem_vendor_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*subsystem_device_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*revision_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
		*__is_virtfn_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		callee_unmarshal_kernel__probe__device__in(__pos, __msg, __ext, ctx, dev_ptr);
	}

	{
		int i;
		struct resource* array = resource_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			struct resource* element = &array[i];
			callee_unmarshal_kernel__probe__resource__in(__pos, __msg, __ext, ctx, element);
		}

	}

	{
		ptr->is_virtfn = *__is_virtfn_ptr;
	}
}

void callee_marshal_kernel__probe__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	struct device const* dev_ptr = &ptr->dev;

	{
		if (*bus_ptr) {
			callee_marshal_kernel__probe__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
		callee_marshal_kernel__probe__device__in(__pos, __msg, __ext, ctx, dev_ptr);
	}

}

void caller_unmarshal_kernel__probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	struct device* dev_ptr = &ptr->dev;

	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__probe__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
		caller_unmarshal_kernel__probe__device__in(__pos, __msg, __ext, ctx, dev_ptr);
	}

	{
	}
}

void caller_marshal_kernel__probe__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct pci_bus const* ptr)
{

}

void callee_unmarshal_kernel__probe__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct pci_bus* ptr)
{

	{
	}
}

void callee_marshal_kernel__probe__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct pci_bus const* ptr)
{

}

void caller_unmarshal_kernel__probe__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct pci_bus* ptr)
{

	{
	}
}

void caller_marshal_kernel__probe__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct device const* ptr)
{
	unsigned long long* const* dma_mask_ptr = &ptr->dma_mask;
	struct kobject const* kobj_ptr = &ptr->kobj;

	{
		__maybe_unused const void* __adjusted = *dma_mask_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dma_mask_ptr) {
			size_t i, len = 1;
			unsigned long long const* array = *dma_mask_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned long long const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		caller_marshal_kernel__probe__kobject__in(__pos, __msg, __ext, ctx, kobj_ptr);
	}

}

void callee_unmarshal_kernel__probe__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct device* ptr)
{
	unsigned long long** dma_mask_ptr = &ptr->dma_mask;
	struct kobject* kobj_ptr = &ptr->kobj;

	{
		size_t __size = sizeof(unsigned long long) * glue_peek(__pos, __msg, __ext);
		*dma_mask_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, unsigned long long*, (__size), (DEFAULT_GFP_FLAGS));
		if (*dma_mask_ptr) {
			int i;
			unsigned long long* array = *dma_mask_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned long long* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, unsigned long long);
			}

		}

	}

	{
		callee_unmarshal_kernel__probe__kobject__in(__pos, __msg, __ext, ctx, kobj_ptr);
	}

	{
	}
}

void callee_marshal_kernel__probe__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct device const* ptr)
{
	unsigned long long* const* dma_mask_ptr = &ptr->dma_mask;
	struct kobject const* kobj_ptr = &ptr->kobj;

	{
		(void)dma_mask_ptr;
	}

	{
		callee_marshal_kernel__probe__kobject__in(__pos, __msg, __ext, ctx, kobj_ptr);
	}

}

void caller_unmarshal_kernel__probe__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct device* ptr)
{
	unsigned long long** dma_mask_ptr = &ptr->dma_mask;
	struct kobject* kobj_ptr = &ptr->kobj;

	{
		(void)dma_mask_ptr;
	}

	{
		caller_unmarshal_kernel__probe__kobject__in(__pos, __msg, __ext, ctx, kobj_ptr);
	}

	{
	}
}

void caller_marshal_kernel__probe__kobject__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct kobject const* ptr)
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

void callee_unmarshal_kernel__probe__kobject__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct kobject* ptr)
{
	char const** name_ptr = &ptr->name;

	{
		*name_ptr = glue_unpack(__pos, __msg, __ext, char const*);
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

void callee_marshal_kernel__probe__kobject__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct kobject const* ptr)
{
	char const* const* name_ptr = &ptr->name;

	{
		(void)name_ptr;
	}

}

void caller_unmarshal_kernel__probe__kobject__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct kobject* ptr)
{
	char const** name_ptr = &ptr->name;

	{
		(void)name_ptr;
	}

	{
	}
}

void caller_marshal_kernel__probe__resource__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct resource const* ptr)
{

}

void callee_unmarshal_kernel__probe__resource__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct resource* ptr)
{

	{
	}
}

void callee_marshal_kernel__probe__resource__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct resource const* ptr)
{

}

void caller_unmarshal_kernel__probe__resource__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct resource* ptr)
{

	{
	}
}

void caller_marshal_kernel__probe__ent__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct pci_device_id const* ptr)
{
	unsigned long const* driver_data_ptr = &ptr->driver_data;

	{
		glue_pack(__pos, __msg, __ext, *driver_data_ptr);
	}

}

void callee_unmarshal_kernel__probe__ent__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct pci_device_id* ptr)
{
	unsigned long* driver_data_ptr = &ptr->driver_data;

	{
		*driver_data_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void callee_marshal_kernel__probe__ent__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct pci_device_id const* ptr)
{

}

void caller_unmarshal_kernel__probe__ent__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct probe_call_ctx const* ctx,
	struct pci_device_id* ptr)
{

	{
	}
}

void caller_marshal_kernel__remove__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct remove_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct remove_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__remove__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct remove_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct remove_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__shutdown__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct shutdown_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__shutdown__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct shutdown_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__shutdown__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct shutdown_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__shutdown__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct shutdown_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long long const* state_ptr = &ptr->state;
	unsigned long long const* features_ptr = &ptr->features;
	unsigned int const* flags_ptr = &ptr->flags;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

}

void callee_unmarshal_kernel__ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long long* state_ptr = &ptr->state;
	unsigned long long* features_ptr = &ptr->features;
	unsigned int* flags_ptr = &ptr->flags;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_rx_mode_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_stop__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_stop_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_stop__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_stop_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_stop__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_stop_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_stop__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_stop_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_open__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_open_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long long const* state_ptr = &ptr->state;
	struct netdev_queue* const* _tx_ptr = &ptr->_tx;
	unsigned char const* num_tc_ptr = &ptr->num_tc;
	unsigned char const* prio_tc_map_ptr = ptr->prio_tc_map;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *_tx_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*_tx_ptr) {
			caller_marshal_kernel__ndo_open__net_device__tx__in(__pos, __msg, __ext, ctx, *_tx_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *num_tc_ptr);
	}

	{
		size_t i, len = 16;
		unsigned char const* array = prio_tc_map_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

}

void callee_unmarshal_kernel__ndo_open__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_open_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long long* state_ptr = &ptr->state;
	struct netdev_queue** _tx_ptr = &ptr->_tx;
	unsigned char* num_tc_ptr = &ptr->num_tc;
	unsigned char* prio_tc_map_ptr = ptr->prio_tc_map;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*_tx_ptr = glue_unpack(__pos, __msg, __ext, struct netdev_queue*);
		if (*_tx_ptr) {
			callee_unmarshal_kernel__ndo_open__net_device__tx__in(__pos, __msg, __ext, ctx, *_tx_ptr);
		}

	}

	{
		*num_tc_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
		int i;
		unsigned char* array = prio_tc_map_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__ndo_open__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_open_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int const* num_rx_queues_ptr = &ptr->num_rx_queues;
	struct netdev_queue* const* _tx_ptr = &ptr->_tx;
	unsigned int const* num_tx_queues_ptr = &ptr->num_tx_queues;
	unsigned char const* num_tc_ptr = &ptr->num_tc;

	{
		glue_pack(__pos, __msg, __ext, *num_rx_queues_ptr);
	}

	{
		if (*_tx_ptr) {
			callee_marshal_kernel__ndo_open__net_device__tx__in(__pos, __msg, __ext, ctx, *_tx_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *num_tx_queues_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *num_tc_ptr);
	}

}

void caller_unmarshal_kernel__ndo_open__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_open_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int* num_rx_queues_ptr = &ptr->num_rx_queues;
	struct netdev_queue** _tx_ptr = &ptr->_tx;
	unsigned int* num_tx_queues_ptr = &ptr->num_tx_queues;
	unsigned char* num_tc_ptr = &ptr->num_tc;

	{
		*num_rx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		if (*_tx_ptr) {
			caller_unmarshal_kernel__ndo_open__net_device__tx__in(__pos, __msg, __ext, ctx, *_tx_ptr);
		}

	}

	{
		*num_tx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*num_tc_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void caller_marshal_kernel__ndo_open__net_device__tx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_open_call_ctx const* ctx,
	struct netdev_queue const* ptr)
{

}

void callee_unmarshal_kernel__ndo_open__net_device__tx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_open_call_ctx const* ctx,
	struct netdev_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_open__net_device__tx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_open_call_ctx const* ctx,
	struct netdev_queue const* ptr)
{

}

void caller_unmarshal_kernel__ndo_open__net_device__tx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_open_call_ctx const* ctx,
	struct netdev_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_start_xmit__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned int const* priority_ptr = &ptr->priority;
	unsigned short const* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* protocol_ptr = &ptr->protocol;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *priority_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *vlan_tci_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *protocol_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__ndo_start_xmit__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned int* priority_ptr = &ptr->priority;
	unsigned short* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* protocol_ptr = &ptr->protocol;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*priority_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*vlan_tci_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*protocol_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__ndo_start_xmit__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void caller_unmarshal_kernel__ndo_start_xmit__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void caller_marshal_kernel__ndo_start_xmit__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_start_xmit__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_start_xmit__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_start_xmit__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_start_xmit_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_features_check__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__ndo_features_check__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__ndo_features_check__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		(void)head_ptr;
	}

	{
		(void)data_ptr;
	}

}

void caller_unmarshal_kernel__ndo_features_check__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		(void)head_ptr;
	}

	{
		(void)data_ptr;
	}

	{
	}
}

void caller_marshal_kernel__ndo_features_check__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_features_check__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_features_check__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_features_check__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_features_check_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_set_mac_address__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_mac_address_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char const* addr_len_ptr = &ptr->addr_len;
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;

	{
		glue_pack(__pos, __msg, __ext, *addr_len_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *dev_addr_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		(void)dev_addr_ptr;
	}

}

void callee_unmarshal_kernel__ndo_set_mac_address__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_mac_address_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char* addr_len_ptr = &ptr->addr_len;
	unsigned char** dev_addr_ptr = &ptr->dev_addr;

	{
		*addr_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
		*dev_addr_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		(void)dev_addr_ptr;
	}

	{
	}
}

void callee_marshal_kernel__ndo_set_mac_address__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_mac_address_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;

	{
		if (*dev_addr_ptr) {
			size_t i, len = (ctx->netdev->addr_len);
			unsigned char* array = *dev_addr_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

}

void caller_unmarshal_kernel__ndo_set_mac_address__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_mac_address_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char** dev_addr_ptr = &ptr->dev_addr;

	{
		if (*dev_addr_ptr) {
			int i;
			unsigned char* array = *dev_addr_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, unsigned char);
			}

		}

	}

	{
	}
}

void caller_marshal_kernel__ndo_do_ioctl__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_do_ioctl__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_do_ioctl__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_do_ioctl__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_do_ioctl__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* ctx,
	struct ifreq const* ptr)
{

}

void callee_unmarshal_kernel__ndo_do_ioctl__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* ctx,
	struct ifreq* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_do_ioctl__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* ctx,
	struct ifreq const* ptr)
{

}

void caller_unmarshal_kernel__ndo_do_ioctl__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_do_ioctl_call_ctx const* ctx,
	struct ifreq* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_change_mtu__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_change_mtu_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int const* mtu_ptr = &ptr->mtu;

	{
		glue_pack(__pos, __msg, __ext, *mtu_ptr);
	}

}

void callee_unmarshal_kernel__ndo_change_mtu__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_change_mtu_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int* mtu_ptr = &ptr->mtu;

	{
		*mtu_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__ndo_change_mtu__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_change_mtu_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int const* mtu_ptr = &ptr->mtu;

	{
		glue_pack(__pos, __msg, __ext, *mtu_ptr);
	}

}

void caller_unmarshal_kernel__ndo_change_mtu__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_change_mtu_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int* mtu_ptr = &ptr->mtu;

	{
		*mtu_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__ndo_tx_timeout__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_tx_timeout_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_tx_timeout__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_tx_timeout_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_tx_timeout__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_tx_timeout_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_tx_timeout__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_tx_timeout_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{
	unsigned long long const* rx_packets_ptr = &ptr->rx_packets;
	unsigned long long const* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long const* rx_bytes_ptr = &ptr->rx_bytes;
	unsigned long long const* tx_bytes_ptr = &ptr->tx_bytes;
	unsigned long long const* rx_errors_ptr = &ptr->rx_errors;
	unsigned long long const* multicast_ptr = &ptr->multicast;
	unsigned long long const* rx_length_errors_ptr = &ptr->rx_length_errors;
	unsigned long long const* rx_crc_errors_ptr = &ptr->rx_crc_errors;
	unsigned long long const* rx_missed_errors_ptr = &ptr->rx_missed_errors;

	{
		glue_pack(__pos, __msg, __ext, *rx_packets_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *tx_packets_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_bytes_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *tx_bytes_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_errors_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *multicast_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_length_errors_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_crc_errors_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_missed_errors_ptr);
	}

}

void callee_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{
	unsigned long long* rx_packets_ptr = &ptr->rx_packets;
	unsigned long long* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long* rx_bytes_ptr = &ptr->rx_bytes;
	unsigned long long* tx_bytes_ptr = &ptr->tx_bytes;
	unsigned long long* rx_errors_ptr = &ptr->rx_errors;
	unsigned long long* multicast_ptr = &ptr->multicast;
	unsigned long long* rx_length_errors_ptr = &ptr->rx_length_errors;
	unsigned long long* rx_crc_errors_ptr = &ptr->rx_crc_errors;
	unsigned long long* rx_missed_errors_ptr = &ptr->rx_missed_errors;

	{
		*rx_packets_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*tx_packets_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_bytes_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*tx_bytes_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*multicast_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_length_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_crc_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_missed_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void callee_marshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{
	unsigned long long const* rx_packets_ptr = &ptr->rx_packets;
	unsigned long long const* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long const* rx_bytes_ptr = &ptr->rx_bytes;
	unsigned long long const* tx_bytes_ptr = &ptr->tx_bytes;
	unsigned long long const* rx_errors_ptr = &ptr->rx_errors;
	unsigned long long const* multicast_ptr = &ptr->multicast;
	unsigned long long const* rx_length_errors_ptr = &ptr->rx_length_errors;
	unsigned long long const* rx_crc_errors_ptr = &ptr->rx_crc_errors;
	unsigned long long const* rx_missed_errors_ptr = &ptr->rx_missed_errors;

	{
		glue_pack(__pos, __msg, __ext, *rx_packets_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *tx_packets_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_bytes_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *tx_bytes_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_errors_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *multicast_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_length_errors_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_crc_errors_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_missed_errors_ptr);
	}

}

void caller_unmarshal_kernel__ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{
	unsigned long long* rx_packets_ptr = &ptr->rx_packets;
	unsigned long long* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long* rx_bytes_ptr = &ptr->rx_bytes;
	unsigned long long* tx_bytes_ptr = &ptr->tx_bytes;
	unsigned long long* rx_errors_ptr = &ptr->rx_errors;
	unsigned long long* multicast_ptr = &ptr->multicast;
	unsigned long long* rx_length_errors_ptr = &ptr->rx_length_errors;
	unsigned long long* rx_crc_errors_ptr = &ptr->rx_crc_errors;
	unsigned long long* rx_missed_errors_ptr = &ptr->rx_missed_errors;

	{
		*rx_packets_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*tx_packets_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_bytes_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*tx_bytes_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*multicast_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_length_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_crc_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_missed_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void caller_marshal_kernel__ndo_get_stats64__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_get_stats64__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_get_stats64__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_get_stats64__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_get_stats64__stats__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{
	unsigned long long const* rx_packets_ptr = &ptr->rx_packets;
	unsigned long long const* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long const* rx_bytes_ptr = &ptr->rx_bytes;
	unsigned long long const* tx_bytes_ptr = &ptr->tx_bytes;
	unsigned long long const* rx_errors_ptr = &ptr->rx_errors;
	unsigned long long const* multicast_ptr = &ptr->multicast;
	unsigned long long const* rx_length_errors_ptr = &ptr->rx_length_errors;
	unsigned long long const* rx_crc_errors_ptr = &ptr->rx_crc_errors;
	unsigned long long const* rx_missed_errors_ptr = &ptr->rx_missed_errors;

	{
		glue_pack(__pos, __msg, __ext, *rx_packets_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *tx_packets_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_bytes_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *tx_bytes_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_errors_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *multicast_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_length_errors_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_crc_errors_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_missed_errors_ptr);
	}

}

void callee_unmarshal_kernel__ndo_get_stats64__stats__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{
	unsigned long long* rx_packets_ptr = &ptr->rx_packets;
	unsigned long long* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long* rx_bytes_ptr = &ptr->rx_bytes;
	unsigned long long* tx_bytes_ptr = &ptr->tx_bytes;
	unsigned long long* rx_errors_ptr = &ptr->rx_errors;
	unsigned long long* multicast_ptr = &ptr->multicast;
	unsigned long long* rx_length_errors_ptr = &ptr->rx_length_errors;
	unsigned long long* rx_crc_errors_ptr = &ptr->rx_crc_errors;
	unsigned long long* rx_missed_errors_ptr = &ptr->rx_missed_errors;

	{
		*rx_packets_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*tx_packets_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_bytes_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*tx_bytes_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*multicast_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_length_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_crc_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_missed_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void callee_marshal_kernel__ndo_get_stats64__stats__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{
	unsigned long long const* rx_packets_ptr = &ptr->rx_packets;
	unsigned long long const* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long const* rx_bytes_ptr = &ptr->rx_bytes;
	unsigned long long const* tx_bytes_ptr = &ptr->tx_bytes;
	unsigned long long const* rx_errors_ptr = &ptr->rx_errors;
	unsigned long long const* multicast_ptr = &ptr->multicast;
	unsigned long long const* rx_length_errors_ptr = &ptr->rx_length_errors;
	unsigned long long const* rx_crc_errors_ptr = &ptr->rx_crc_errors;
	unsigned long long const* rx_missed_errors_ptr = &ptr->rx_missed_errors;

	{
		glue_pack(__pos, __msg, __ext, *rx_packets_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *tx_packets_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_bytes_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *tx_bytes_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_errors_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *multicast_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_length_errors_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_crc_errors_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rx_missed_errors_ptr);
	}

}

void caller_unmarshal_kernel__ndo_get_stats64__stats__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{
	unsigned long long* rx_packets_ptr = &ptr->rx_packets;
	unsigned long long* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long* rx_bytes_ptr = &ptr->rx_bytes;
	unsigned long long* tx_bytes_ptr = &ptr->tx_bytes;
	unsigned long long* rx_errors_ptr = &ptr->rx_errors;
	unsigned long long* multicast_ptr = &ptr->multicast;
	unsigned long long* rx_length_errors_ptr = &ptr->rx_length_errors;
	unsigned long long* rx_crc_errors_ptr = &ptr->rx_crc_errors;
	unsigned long long* rx_missed_errors_ptr = &ptr->rx_missed_errors;

	{
		*rx_packets_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*tx_packets_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_bytes_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*tx_bytes_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*multicast_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_length_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_crc_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*rx_missed_errors_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void caller_marshal_kernel__ndo_setup_tc__dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long long const* state_ptr = &ptr->state;
	unsigned int const* num_rx_queues_ptr = &ptr->num_rx_queues;
	unsigned int const* num_tx_queues_ptr = &ptr->num_tx_queues;
	unsigned char const* num_tc_ptr = &ptr->num_tc;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *num_rx_queues_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *num_tx_queues_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *num_tc_ptr);
	}

}

void callee_unmarshal_kernel__ndo_setup_tc__dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long long* state_ptr = &ptr->state;
	unsigned int* num_rx_queues_ptr = &ptr->num_rx_queues;
	unsigned int* num_tx_queues_ptr = &ptr->num_tx_queues;
	unsigned char* num_tc_ptr = &ptr->num_tc;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*num_rx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*num_tx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*num_tc_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void callee_marshal_kernel__ndo_setup_tc__dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long long const* state_ptr = &ptr->state;
	unsigned int const* num_rx_queues_ptr = &ptr->num_rx_queues;
	unsigned int const* num_tx_queues_ptr = &ptr->num_tx_queues;
	unsigned char const* num_tc_ptr = &ptr->num_tc;
	struct netdev_tc_txq const* tc_to_txq_ptr = ptr->tc_to_txq;
	unsigned char const* prio_tc_map_ptr = ptr->prio_tc_map;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *num_rx_queues_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *num_tx_queues_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *num_tc_ptr);
	}

	{
		size_t i, len = 16;
		struct netdev_tc_txq const* array = tc_to_txq_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			struct netdev_tc_txq const* element = &array[i];
			callee_marshal_kernel__ndo_setup_tc__netdev_tc_txq__out(__pos, __msg, __ext, ctx, element);
		}

	}

	{
		size_t i, len = 16;
		unsigned char const* array = prio_tc_map_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

}

void caller_unmarshal_kernel__ndo_setup_tc__dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long long* state_ptr = &ptr->state;
	unsigned int* num_rx_queues_ptr = &ptr->num_rx_queues;
	unsigned int* num_tx_queues_ptr = &ptr->num_tx_queues;
	unsigned char* num_tc_ptr = &ptr->num_tc;
	struct netdev_tc_txq* tc_to_txq_ptr = ptr->tc_to_txq;
	unsigned char* prio_tc_map_ptr = ptr->prio_tc_map;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*num_rx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*num_tx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*num_tc_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
		int i;
		struct netdev_tc_txq* array = tc_to_txq_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			struct netdev_tc_txq* element = &array[i];
			caller_unmarshal_kernel__ndo_setup_tc__netdev_tc_txq__out(__pos, __msg, __ext, ctx, element);
		}

	}

	{
		int i;
		unsigned char* array = prio_tc_map_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void caller_marshal_kernel__ndo_setup_tc__netdev_tc_txq__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* ctx,
	struct netdev_tc_txq const* ptr)
{

}

void callee_unmarshal_kernel__ndo_setup_tc__netdev_tc_txq__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* ctx,
	struct netdev_tc_txq* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_setup_tc__netdev_tc_txq__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* ctx,
	struct netdev_tc_txq const* ptr)
{

}

void caller_unmarshal_kernel__ndo_setup_tc__netdev_tc_txq__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* ctx,
	struct netdev_tc_txq* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_setup_tc__tc__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* ctx,
	struct tc_to_netdev const* ptr)
{

}

void callee_unmarshal_kernel__ndo_setup_tc__tc__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* ctx,
	struct tc_to_netdev* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_setup_tc__tc__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* ctx,
	struct tc_to_netdev const* ptr)
{

}

void caller_unmarshal_kernel__ndo_setup_tc__tc__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_setup_tc_call_ctx const* ctx,
	struct tc_to_netdev* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_fix_features__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_fix_features_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_fix_features__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_fix_features_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_fix_features__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_fix_features_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_fix_features__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_fix_features_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ndo_set_features__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_features_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long long const* features_ptr = &ptr->features;

	{
		glue_pack(__pos, __msg, __ext, *features_ptr);
	}

}

void callee_unmarshal_kernel__ndo_set_features__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_features_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long long* features_ptr = &ptr->features;

	{
		*features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void callee_marshal_kernel__ndo_set_features__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_features_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long long const* features_ptr = &ptr->features;

	{
		glue_pack(__pos, __msg, __ext, *features_ptr);
	}

}

void caller_unmarshal_kernel__ndo_set_features__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_features_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long long* features_ptr = &ptr->features;

	{
		*features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void caller_marshal_kernel__ndo_set_tx_maxrate__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_tx_maxrate_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ndo_set_tx_maxrate__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_tx_maxrate_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ndo_set_tx_maxrate__netdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_set_tx_maxrate_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ndo_set_tx_maxrate__netdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_set_tx_maxrate_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* ctx,
	struct timer_list const* ptr)
{

}

void callee_unmarshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* ctx,
	struct timer_list* ptr)
{

	{
	}
}

void callee_marshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* ctx,
	struct timer_list const* ptr)
{

}

void caller_unmarshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* ctx,
	struct timer_list* ptr)
{

	{
	}
}

void caller_marshal_kernel__lvd_setup_timer__timer__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_setup_timer_call_ctx const* ctx,
	struct timer_list const* ptr)
{

}

void callee_unmarshal_kernel__lvd_setup_timer__timer__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_setup_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{

	{
	}
}

void callee_marshal_kernel__lvd_setup_timer__timer__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_setup_timer_call_ctx const* ctx,
	struct timer_list const* ptr)
{
	unsigned long const* data_ptr = &ptr->data;

	{
		glue_pack(__pos, __msg, __ext, *data_ptr);
	}

}

void caller_unmarshal_kernel__lvd_setup_timer__timer__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_setup_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{
	unsigned long* data_ptr = &ptr->data;

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void caller_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int const* irq_ptr = &ptr->irq;
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int const* __msi_enabled_ptr = &__msi_enabled;

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__msi_enabled_ptr);
	}

}

void callee_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int* irq_ptr = &ptr->irq;
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int* __msi_enabled_ptr = &__msi_enabled;

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__msi_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->msi_enabled = *__msi_enabled_ptr;
	}
}

void callee_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int const* irq_ptr = &ptr->irq;
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int const* __msi_enabled_ptr = &__msi_enabled;

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__msi_enabled_ptr);
	}

}

void caller_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int* irq_ptr = &ptr->irq;
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int* __msi_enabled_ptr = &__msi_enabled;

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__msi_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->msi_enabled = *__msi_enabled_ptr;
	}
}

void caller_marshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int const* __msix_enabled_ptr = &__msix_enabled;

	{
		glue_pack(__pos, __msg, __ext, *__msix_enabled_ptr);
	}

}

void callee_unmarshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int* __msix_enabled_ptr = &__msix_enabled;

	{
		*__msix_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->msix_enabled = *__msix_enabled_ptr;
	}
}

void callee_marshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int const* __msix_enabled_ptr = &__msix_enabled;

	{
		glue_pack(__pos, __msg, __ext, *__msix_enabled_ptr);
	}

}

void caller_unmarshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int* __msix_enabled_ptr = &__msix_enabled;

	{
		*__msix_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->msix_enabled = *__msix_enabled_ptr;
	}
}

void caller_marshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_bus_read_config_word_call_ctx const* ctx,
	struct pci_bus const* ptr)
{

}

void callee_unmarshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_bus_read_config_word_call_ctx const* ctx,
	struct pci_bus* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_bus_read_config_word_call_ctx const* ctx,
	struct pci_bus const* ptr)
{

}

void caller_unmarshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_bus_read_config_word_call_ctx const* ctx,
	struct pci_bus* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_bus_write_config_word_call_ctx const* ctx,
	struct pci_bus const* ptr)
{

}

void callee_unmarshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_bus_write_config_word_call_ctx const* ctx,
	struct pci_bus* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_bus_write_config_word_call_ctx const* ctx,
	struct pci_bus const* ptr)
{

}

void caller_unmarshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_bus_write_config_word_call_ctx const* ctx,
	struct pci_bus* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_dev_put_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_dev_put_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_dev_put_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_dev_put_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* ctx,
	struct workqueue_struct const* ptr)
{

}

void callee_unmarshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* ctx,
	struct workqueue_struct* ptr)
{

	{
	}
}

void callee_marshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* ctx,
	struct workqueue_struct const* ptr)
{

}

void caller_unmarshal_kernel__queue_work_on__wq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* ctx,
	struct workqueue_struct* ptr)
{

	{
	}
}

void caller_marshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* ctx,
	struct work_struct const* ptr)
{
	struct list_head const* entry_ptr = &ptr->entry;

	{
		caller_marshal_kernel__queue_work_on__list_head__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

}

void callee_unmarshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* ctx,
	struct work_struct* ptr)
{
	struct list_head* entry_ptr = &ptr->entry;

	{
		callee_unmarshal_kernel__queue_work_on__list_head__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

	{
	}
}

void callee_marshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* ctx,
	struct work_struct const* ptr)
{
	struct list_head const* entry_ptr = &ptr->entry;

	{
		callee_marshal_kernel__queue_work_on__list_head__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

}

void caller_unmarshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* ctx,
	struct work_struct* ptr)
{
	struct list_head* entry_ptr = &ptr->entry;

	{
		caller_unmarshal_kernel__queue_work_on__list_head__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

	{
	}
}

void caller_marshal_kernel__queue_work_on__list_head__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* ctx,
	struct list_head const* ptr)
{

}

void callee_unmarshal_kernel__queue_work_on__list_head__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* ctx,
	struct list_head* ptr)
{

	{
	}
}

void callee_marshal_kernel__queue_work_on__list_head__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* ctx,
	struct list_head const* ptr)
{

}

void caller_unmarshal_kernel__queue_work_on__list_head__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct queue_work_on_call_ctx const* ctx,
	struct list_head* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pcie_get_minimum_link__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;

	{
		__maybe_unused const void* __adjusted = *bus_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__pcie_get_minimum_link__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

}

void callee_unmarshal_kernel__pcie_get_minimum_link__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;

	{
		*bus_ptr = glue_unpack(__pos, __msg, __ext, struct pci_bus*);
		if (*bus_ptr) {
			callee_unmarshal_kernel__pcie_get_minimum_link__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__pcie_get_minimum_link__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;

	{
		if (*bus_ptr) {
			callee_marshal_kernel__pcie_get_minimum_link__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

}

void caller_unmarshal_kernel__pcie_get_minimum_link__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;

	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__pcie_get_minimum_link__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__pcie_get_minimum_link__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* ctx,
	struct pci_bus const* ptr)
{

}

void callee_unmarshal_kernel__pcie_get_minimum_link__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* ctx,
	struct pci_bus* ptr)
{

	{
	}
}

void callee_marshal_kernel__pcie_get_minimum_link__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* ctx,
	struct pci_bus const* ptr)
{

}

void caller_unmarshal_kernel__pcie_get_minimum_link__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_get_minimum_link_call_ctx const* ctx,
	struct pci_bus* ptr)
{

	{
	}
}

void caller_marshal_kernel__pcie_capability_read_word__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	unsigned char const* pcie_cap_ptr = &ptr->pcie_cap;

	{
		__maybe_unused const void* __adjusted = *bus_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__pcie_capability_read_word__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *pcie_cap_ptr);
	}

}

void callee_unmarshal_kernel__pcie_capability_read_word__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	unsigned char* pcie_cap_ptr = &ptr->pcie_cap;

	{
		*bus_ptr = glue_unpack(__pos, __msg, __ext, struct pci_bus*);
		if (*bus_ptr) {
			callee_unmarshal_kernel__pcie_capability_read_word__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
		*pcie_cap_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void callee_marshal_kernel__pcie_capability_read_word__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;

	{
		if (*bus_ptr) {
			callee_marshal_kernel__pcie_capability_read_word__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

}

void caller_unmarshal_kernel__pcie_capability_read_word__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;

	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__pcie_capability_read_word__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__pcie_capability_read_word__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* ctx,
	struct pci_bus const* ptr)
{

}

void callee_unmarshal_kernel__pcie_capability_read_word__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* ctx,
	struct pci_bus* ptr)
{

	{
	}
}

void callee_marshal_kernel__pcie_capability_read_word__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* ctx,
	struct pci_bus const* ptr)
{

}

void caller_unmarshal_kernel__pcie_capability_read_word__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pcie_capability_read_word_call_ctx const* ctx,
	struct pci_bus* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int const* devfn_ptr = &ptr->devfn;
	unsigned char const* pcie_cap_ptr = &ptr->pcie_cap;

	{
		glue_pack(__pos, __msg, __ext, *devfn_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *pcie_cap_ptr);
	}

}

void callee_unmarshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int* devfn_ptr = &ptr->devfn;
	unsigned char* pcie_cap_ptr = &ptr->pcie_cap;

	{
		*devfn_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*pcie_cap_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void callee_marshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_set_power_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_set_power_state_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_set_power_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_set_power_state_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_set_power_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_set_power_state_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_set_power_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_set_power_state_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__mod_timer__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct timer_list const* ptr)
{

}

void callee_unmarshal_kernel__mod_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{

	{
	}
}

void callee_marshal_kernel__mod_timer__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct timer_list const* ptr)
{

}

void caller_unmarshal_kernel__mod_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_wake_from_d3__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_wake_from_d3_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_wake_from_d3__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_wake_from_d3_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_wake_from_d3__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_wake_from_d3_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_wake_from_d3__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_wake_from_d3_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__skb_copy_bits__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_copy_bits_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__skb_copy_bits__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_copy_bits_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__skb_copy_bits__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_copy_bits_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* len_ptr = &ptr->len;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		(void)head_ptr;
	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void caller_unmarshal_kernel__skb_copy_bits__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_copy_bits_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* len_ptr = &ptr->len;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		(void)head_ptr;
	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void caller_marshal_kernel_____pskb_trim__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ___pskb_trim_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel_____pskb_trim__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ___pskb_trim_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel_____pskb_trim__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ___pskb_trim_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void caller_unmarshal_kernel_____pskb_trim__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ___pskb_trim_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void caller_marshal_kernel__pskb_expand_head__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pskb_expand_head_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__pskb_expand_head__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pskb_expand_head_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__pskb_expand_head__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pskb_expand_head_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void caller_unmarshal_kernel__pskb_expand_head__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pskb_expand_head_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void caller_marshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct consume_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

}

void callee_unmarshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct consume_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct consume_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned char* const* head_ptr = &ptr->head;

	{
		(void)head_ptr;
	}

}

void caller_unmarshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct consume_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned char** head_ptr = &ptr->head;

	{
		(void)head_ptr;
	}

	{
	}
}

void caller_marshal_kernel____pskb_pull_tail__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pskb_pull_tail_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel____pskb_pull_tail__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pskb_pull_tail_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel____pskb_pull_tail__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pskb_pull_tail_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void caller_unmarshal_kernel____pskb_pull_tail__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pskb_pull_tail_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void caller_marshal_kernel__skb_put__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__skb_put__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__skb_put__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		(void)head_ptr;
	}

	{
		(void)data_ptr;
	}

}

void caller_unmarshal_kernel__skb_put__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* len_ptr = &ptr->len;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		(void)head_ptr;
	}

	{
		(void)data_ptr;
	}

	{
	}
}

void caller_marshal_kernel__skb_tstamp_tx__orig_skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__skb_tstamp_tx__orig_skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__skb_tstamp_tx__orig_skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void caller_unmarshal_kernel__skb_tstamp_tx__orig_skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void caller_marshal_kernel__skb_tstamp_tx__hwtstamps__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* ctx,
	struct skb_shared_hwtstamps const* ptr)
{

}

void callee_unmarshal_kernel__skb_tstamp_tx__hwtstamps__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* ctx,
	struct skb_shared_hwtstamps* ptr)
{

	{
	}
}

void callee_marshal_kernel__skb_tstamp_tx__hwtstamps__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* ctx,
	struct skb_shared_hwtstamps const* ptr)
{

}

void caller_unmarshal_kernel__skb_tstamp_tx__hwtstamps__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_tstamp_tx_call_ctx const* ctx,
	struct skb_shared_hwtstamps* ptr)
{

	{
	}
}

void caller_marshal_kernel__skb_pad__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_pad_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__skb_pad__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_pad_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__skb_pad__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_pad_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void caller_unmarshal_kernel__skb_pad__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_pad_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void caller_marshal_kernel__napi_consume_skb__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_consume_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

}

void callee_unmarshal_kernel__napi_consume_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_consume_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__napi_consume_skb__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_consume_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned char* const* head_ptr = &ptr->head;

	{
		(void)head_ptr;
	}

}

void caller_unmarshal_kernel__napi_consume_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_consume_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned char** head_ptr = &ptr->head;

	{
		(void)head_ptr;
	}

	{
	}
}

void caller_marshal_kernel____dev_kfree_skb_any__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __dev_kfree_skb_any_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

}

void callee_unmarshal_kernel____dev_kfree_skb_any__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __dev_kfree_skb_any_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel____dev_kfree_skb_any__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __dev_kfree_skb_any_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned char* const* head_ptr = &ptr->head;

	{
		(void)head_ptr;
	}

}

void caller_unmarshal_kernel____dev_kfree_skb_any__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __dev_kfree_skb_any_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned char** head_ptr = &ptr->head;

	{
		(void)head_ptr;
	}

	{
	}
}

void caller_marshal_kernel____napi_alloc_skb__ret_sk_buff__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

}

void callee_unmarshal_kernel____napi_alloc_skb__ret_sk_buff__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel____napi_alloc_skb__ret_sk_buff__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

}

void caller_unmarshal_kernel____napi_alloc_skb__ret_sk_buff__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void caller_marshal_kernel____napi_alloc_skb__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	struct sk_buff* const* gro_list_ptr = &ptr->gro_list;

	{
		__maybe_unused const void* __adjusted = *gro_list_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*gro_list_ptr) {
			caller_marshal_kernel____napi_alloc_skb__napi_struct_gro_list__in(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

}

void callee_unmarshal_kernel____napi_alloc_skb__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	struct sk_buff** gro_list_ptr = &ptr->gro_list;

	{
		*gro_list_ptr = glue_unpack(__pos, __msg, __ext, struct sk_buff*);
		if (*gro_list_ptr) {
			callee_unmarshal_kernel____napi_alloc_skb__napi_struct_gro_list__in(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel____napi_alloc_skb__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	struct sk_buff* const* gro_list_ptr = &ptr->gro_list;

	{
		if (*gro_list_ptr) {
			callee_marshal_kernel____napi_alloc_skb__napi_struct_gro_list__in(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

}

void caller_unmarshal_kernel____napi_alloc_skb__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	struct sk_buff** gro_list_ptr = &ptr->gro_list;

	{
		if (*gro_list_ptr) {
			caller_unmarshal_kernel____napi_alloc_skb__napi_struct_gro_list__in(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel____napi_alloc_skb__napi_struct_gro_list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{

}

void callee_unmarshal_kernel____napi_alloc_skb__napi_struct_gro_list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{

	{
	}
}

void callee_marshal_kernel____napi_alloc_skb__napi_struct_gro_list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{

}

void caller_unmarshal_kernel____napi_alloc_skb__napi_struct_gro_list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_alloc_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{

	{
	}
}

void caller_marshal_kernel__napi_gro_flush__napi__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	struct sk_buff* const* gro_list_ptr = &ptr->gro_list;

	{
		__maybe_unused const void* __adjusted = *gro_list_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*gro_list_ptr) {
			caller_marshal_kernel__napi_gro_flush__napi_struct_gro_list__io(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

}

void callee_unmarshal_kernel__napi_gro_flush__napi__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	struct sk_buff** gro_list_ptr = &ptr->gro_list;

	{
		*gro_list_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sk_buff*);
		if (*gro_list_ptr) {
			callee_unmarshal_kernel__napi_gro_flush__napi_struct_gro_list__io(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__napi_gro_flush__napi__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	struct sk_buff* const* gro_list_ptr = &ptr->gro_list;

	{
		__maybe_unused const void* __adjusted = *gro_list_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*gro_list_ptr) {
			callee_marshal_kernel__napi_gro_flush__napi_struct_gro_list__io(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

}

void caller_unmarshal_kernel__napi_gro_flush__napi__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	struct sk_buff** gro_list_ptr = &ptr->gro_list;

	{
		*gro_list_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sk_buff*);
		if (*gro_list_ptr) {
			caller_unmarshal_kernel__napi_gro_flush__napi_struct_gro_list__io(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__napi_gro_flush__napi_struct_gro_list__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned int const* hash_ptr = &ptr->hash;
	unsigned short const* vlan_proto_ptr = &ptr->vlan_proto;
	unsigned short const* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* protocol_ptr = &ptr->protocol;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *hash_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *vlan_proto_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *vlan_tci_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *protocol_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__napi_gro_flush__napi_struct_gro_list__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned int* hash_ptr = &ptr->hash;
	unsigned short* vlan_proto_ptr = &ptr->vlan_proto;
	unsigned short* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* protocol_ptr = &ptr->protocol;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*hash_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*vlan_proto_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*vlan_tci_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*protocol_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__napi_gro_flush__napi_struct_gro_list__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short const* vlan_proto_ptr = &ptr->vlan_proto;
	unsigned short const* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* protocol_ptr = &ptr->protocol;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *vlan_proto_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *vlan_tci_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *protocol_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void caller_unmarshal_kernel__napi_gro_flush__napi_struct_gro_list__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_flush_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short* vlan_proto_ptr = &ptr->vlan_proto;
	unsigned short* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* protocol_ptr = &ptr->protocol;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*vlan_proto_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*vlan_tci_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*protocol_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void caller_marshal_kernel__napi_complete_done__n__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	unsigned long long const* state_ptr = &ptr->state;
	struct sk_buff* const* gro_list_ptr = &ptr->gro_list;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *gro_list_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*gro_list_ptr) {
			caller_marshal_kernel__napi_complete_done__napi_struct_gro_list__io(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

}

void callee_unmarshal_kernel__napi_complete_done__n__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	unsigned long long* state_ptr = &ptr->state;
	struct sk_buff** gro_list_ptr = &ptr->gro_list;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*gro_list_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sk_buff*);
		if (*gro_list_ptr) {
			callee_unmarshal_kernel__napi_complete_done__napi_struct_gro_list__io(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__napi_complete_done__n__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	unsigned long long const* state_ptr = &ptr->state;
	struct sk_buff* const* gro_list_ptr = &ptr->gro_list;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *gro_list_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*gro_list_ptr) {
			callee_marshal_kernel__napi_complete_done__napi_struct_gro_list__io(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

}

void caller_unmarshal_kernel__napi_complete_done__n__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	unsigned long long* state_ptr = &ptr->state;
	struct sk_buff** gro_list_ptr = &ptr->gro_list;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*gro_list_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sk_buff*);
		if (*gro_list_ptr) {
			caller_unmarshal_kernel__napi_complete_done__napi_struct_gro_list__io(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__napi_complete_done__napi_struct_gro_list__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned int const* hash_ptr = &ptr->hash;
	unsigned short const* vlan_proto_ptr = &ptr->vlan_proto;
	unsigned short const* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* protocol_ptr = &ptr->protocol;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *hash_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *vlan_proto_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *vlan_tci_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *protocol_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__napi_complete_done__napi_struct_gro_list__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned int* hash_ptr = &ptr->hash;
	unsigned short* vlan_proto_ptr = &ptr->vlan_proto;
	unsigned short* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* protocol_ptr = &ptr->protocol;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*hash_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*vlan_proto_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*vlan_tci_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*protocol_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__napi_complete_done__napi_struct_gro_list__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short const* vlan_proto_ptr = &ptr->vlan_proto;
	unsigned short const* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* protocol_ptr = &ptr->protocol;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *vlan_proto_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *vlan_tci_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *protocol_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void caller_unmarshal_kernel__napi_complete_done__napi_struct_gro_list__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_complete_done_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short* vlan_proto_ptr = &ptr->vlan_proto;
	unsigned short* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* protocol_ptr = &ptr->protocol;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*vlan_proto_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*vlan_tci_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*protocol_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void caller_marshal_kernel__call_netdevice_notifiers__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct call_netdevice_notifiers_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__call_netdevice_notifiers__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct call_netdevice_notifiers_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__call_netdevice_notifiers__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct call_netdevice_notifiers_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__call_netdevice_notifiers__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct call_netdevice_notifiers_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__dev_get_stats__ret_rtnl_link_stats64__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{

}

void callee_unmarshal_kernel__dev_get_stats__ret_rtnl_link_stats64__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{

	{
	}
}

void callee_marshal_kernel__dev_get_stats__ret_rtnl_link_stats64__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{

}

void caller_unmarshal_kernel__dev_get_stats__ret_rtnl_link_stats64__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{

	{
	}
}

void caller_marshal_kernel__dev_get_stats__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__dev_get_stats__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__dev_get_stats__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__dev_get_stats__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__dev_get_stats__storage__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{

}

void callee_unmarshal_kernel__dev_get_stats__storage__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{

	{
	}
}

void callee_marshal_kernel__dev_get_stats__storage__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{

}

void caller_unmarshal_kernel__dev_get_stats__storage__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_stats_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{

	{
	}
}

void caller_marshal_kernel__unregister_netdev__dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct unregister_netdev_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int __reg_state = ptr->reg_state;
	unsigned int const* __reg_state_ptr = &__reg_state;

	{
		glue_pack(__pos, __msg, __ext, *__reg_state_ptr);
	}

}

void callee_unmarshal_kernel__unregister_netdev__dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct unregister_netdev_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int __reg_state = ptr->reg_state;
	unsigned int* __reg_state_ptr = &__reg_state;

	{
		*__reg_state_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->reg_state = *__reg_state_ptr;
	}
}

void callee_marshal_kernel__unregister_netdev__dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct unregister_netdev_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int __reg_state = ptr->reg_state;
	unsigned int const* __reg_state_ptr = &__reg_state;

	{
		glue_pack(__pos, __msg, __ext, *__reg_state_ptr);
	}

}

void caller_unmarshal_kernel__unregister_netdev__dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct unregister_netdev_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int __reg_state = ptr->reg_state;
	unsigned int* __reg_state_ptr = &__reg_state;

	{
		*__reg_state_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->reg_state = *__reg_state_ptr;
	}
}

void caller_marshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct free_netdev_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;

	{
		__maybe_unused const void* __adjusted = *dev_addr_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_addr_ptr) {
			glue_pack(__pos, __msg, __ext, **dev_addr_ptr);
		}

	}

}

void callee_unmarshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct free_netdev_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char** dev_addr_ptr = &ptr->dev_addr;

	{
		*dev_addr_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*dev_addr_ptr) {
			**dev_addr_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct free_netdev_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;

	{
		(void)dev_addr_ptr;
	}

}

void caller_unmarshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct free_netdev_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char** dev_addr_ptr = &ptr->dev_addr;

	{
		(void)dev_addr_ptr;
	}

	{
	}
}

void caller_marshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	unsigned long long const* state_ptr = &ptr->state;
	struct sk_buff* const* gro_list_ptr = &ptr->gro_list;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *gro_list_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*gro_list_ptr) {
			caller_marshal_kernel__netif_napi_del__napi_struct_gro_list__in(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

}

void callee_unmarshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	unsigned long long* state_ptr = &ptr->state;
	struct sk_buff** gro_list_ptr = &ptr->gro_list;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*gro_list_ptr = glue_unpack(__pos, __msg, __ext, struct sk_buff*);
		if (*gro_list_ptr) {
			callee_unmarshal_kernel__netif_napi_del__napi_struct_gro_list__in(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	struct sk_buff* const* gro_list_ptr = &ptr->gro_list;

	{
		if (*gro_list_ptr) {
			callee_marshal_kernel__netif_napi_del__napi_struct_gro_list__in(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

}

void caller_unmarshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	struct sk_buff** gro_list_ptr = &ptr->gro_list;

	{
		if (*gro_list_ptr) {
			caller_unmarshal_kernel__netif_napi_del__napi_struct_gro_list__in(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__netif_napi_del__napi_struct_gro_list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* ctx,
	struct sk_buff const* ptr)
{

}

void callee_unmarshal_kernel__netif_napi_del__napi_struct_gro_list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* ctx,
	struct sk_buff* ptr)
{

	{
	}
}

void callee_marshal_kernel__netif_napi_del__napi_struct_gro_list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* ctx,
	struct sk_buff const* ptr)
{

}

void caller_unmarshal_kernel__netif_napi_del__napi_struct_gro_list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_del_call_ctx const* ctx,
	struct sk_buff* ptr)
{

	{
	}
}

void caller_marshal_kernel__napi_hash_del__napi__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	unsigned long long const* state_ptr = &ptr->state;
	struct sk_buff* const* gro_list_ptr = &ptr->gro_list;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *gro_list_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*gro_list_ptr) {
			caller_marshal_kernel__napi_hash_del__napi_struct_gro_list__io(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

}

void callee_unmarshal_kernel__napi_hash_del__napi__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	unsigned long long* state_ptr = &ptr->state;
	struct sk_buff** gro_list_ptr = &ptr->gro_list;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*gro_list_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sk_buff*);
		if (*gro_list_ptr) {
			callee_unmarshal_kernel__napi_hash_del__napi_struct_gro_list__io(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__napi_hash_del__napi__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	struct sk_buff* const* gro_list_ptr = &ptr->gro_list;

	{
		__maybe_unused const void* __adjusted = *gro_list_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*gro_list_ptr) {
			callee_marshal_kernel__napi_hash_del__napi_struct_gro_list__io(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

}

void caller_unmarshal_kernel__napi_hash_del__napi__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	struct sk_buff** gro_list_ptr = &ptr->gro_list;

	{
		*gro_list_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sk_buff*);
		if (*gro_list_ptr) {
			caller_unmarshal_kernel__napi_hash_del__napi_struct_gro_list__io(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__napi_hash_del__napi_struct_gro_list__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* ctx,
	struct sk_buff const* ptr)
{

}

void callee_unmarshal_kernel__napi_hash_del__napi_struct_gro_list__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* ctx,
	struct sk_buff* ptr)
{

	{
	}
}

void callee_marshal_kernel__napi_hash_del__napi_struct_gro_list__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* ctx,
	struct sk_buff const* ptr)
{

}

void caller_unmarshal_kernel__napi_hash_del__napi_struct_gro_list__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_hash_del_call_ctx const* ctx,
	struct sk_buff* ptr)
{

	{
	}
}

void caller_marshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct register_netdev_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long long const* features_ptr = &ptr->features;
	unsigned long long const* hw_features_ptr = &ptr->hw_features;
	unsigned long long __vlan_features = ptr->vlan_features;
	unsigned long long const* __vlan_features_ptr = &__vlan_features;
	unsigned long long const* hw_enc_features_ptr = &ptr->hw_enc_features;
	unsigned long long const* mpls_features_ptr = &ptr->mpls_features;
	unsigned long long const* gso_partial_features_ptr = &ptr->gso_partial_features;
	struct net_device_ops* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int const* flags_ptr = &ptr->flags;
	unsigned char const* addr_len_ptr = &ptr->addr_len;
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;
	unsigned int const* priv_flags_ptr = &ptr->priv_flags;
	int const* watchdog_timeo_ptr = &ptr->watchdog_timeo;

	{
		glue_pack(__pos, __msg, __ext, *features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *hw_features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__vlan_features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *hw_enc_features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mpls_features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *gso_partial_features_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *netdev_ops_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*netdev_ops_ptr) {
			caller_marshal_kernel__net_device__global_netdev_ops__in(__pos, __msg, __ext, *netdev_ops_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ethtool_ops_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ethtool_ops_ptr) {
			caller_marshal_kernel__net_device__global_ethtool_ops__in(__pos, __msg, __ext, *ethtool_ops_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *addr_len_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *dev_addr_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_addr_ptr) {
			size_t i, len = (ctx->dev->addr_len);
			unsigned char* array = *dev_addr_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *priv_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *watchdog_timeo_ptr);
	}

}

void callee_unmarshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct register_netdev_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long long* features_ptr = &ptr->features;
	unsigned long long* hw_features_ptr = &ptr->hw_features;
	unsigned long long __vlan_features = ptr->vlan_features;
	unsigned long long* __vlan_features_ptr = &__vlan_features;
	unsigned long long* hw_enc_features_ptr = &ptr->hw_enc_features;
	unsigned long long* mpls_features_ptr = &ptr->mpls_features;
	unsigned long long* gso_partial_features_ptr = &ptr->gso_partial_features;
	struct net_device_ops** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops** ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int* flags_ptr = &ptr->flags;
	unsigned char* addr_len_ptr = &ptr->addr_len;
	unsigned char** dev_addr_ptr = &ptr->dev_addr;
	unsigned int* priv_flags_ptr = &ptr->priv_flags;
	int* watchdog_timeo_ptr = &ptr->watchdog_timeo;

	{
		*features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*hw_features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*__vlan_features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*hw_enc_features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*mpls_features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*gso_partial_features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		size_t __size = sizeof(struct net_device_ops);
		*netdev_ops_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct net_device_ops*, (__size), (DEFAULT_GFP_FLAGS));
		if (*netdev_ops_ptr) {
			callee_unmarshal_kernel__net_device__global_netdev_ops__in(__pos, __msg, __ext, *netdev_ops_ptr);
		}

	}

	{
		size_t __size = sizeof(struct ethtool_ops);
		*ethtool_ops_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct ethtool_ops*, (__size), (DEFAULT_GFP_FLAGS));
		if (*ethtool_ops_ptr) {
			callee_unmarshal_kernel__net_device__global_ethtool_ops__in(__pos, __msg, __ext, *ethtool_ops_ptr);
		}

	}

	{
		*flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*addr_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
		*dev_addr_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*dev_addr_ptr) {
			int i;
			unsigned char* array = *dev_addr_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, unsigned char);
			}

		}

	}

	{
		*priv_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*watchdog_timeo_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct register_netdev_call_ctx const* ctx,
	struct net_device const* ptr)
{
	char const* name_ptr = ptr->name;
	unsigned long const* state_ptr = &ptr->state;
	unsigned int __reg_state = ptr->reg_state;
	unsigned int const* __reg_state_ptr = &__reg_state;
	unsigned long long const* features_ptr = &ptr->features;
	unsigned long long const* hw_features_ptr = &ptr->hw_features;
	unsigned long long const* vlan_features_ptr = &ptr->vlan_features;
	unsigned long long const* hw_enc_features_ptr = &ptr->hw_enc_features;
	unsigned long long const* mpls_features_ptr = &ptr->mpls_features;
	unsigned long long const* gso_partial_features_ptr = &ptr->gso_partial_features;
	struct net_device_ops* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int const* mtu_ptr = &ptr->mtu;
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;
	unsigned int const* num_tx_queues_ptr = &ptr->num_tx_queues;

	{
		size_t i, len = 16;
		char const* array = name_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__reg_state_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *hw_features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *vlan_features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *hw_enc_features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mpls_features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *gso_partial_features_ptr);
	}

	{
		if (*netdev_ops_ptr) {
			callee_marshal_kernel__net_device__global_netdev_ops__in(__pos, __msg, __ext, *netdev_ops_ptr);
		}

	}

	{
		if (*ethtool_ops_ptr) {
			callee_marshal_kernel__net_device__global_ethtool_ops__in(__pos, __msg, __ext, *ethtool_ops_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *mtu_ptr);
	}

	{
		(void)dev_addr_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *num_tx_queues_ptr);
	}

}

void caller_unmarshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct register_netdev_call_ctx const* ctx,
	struct net_device* ptr)
{
	char* name_ptr = ptr->name;
	unsigned long* state_ptr = &ptr->state;
	unsigned int __reg_state = ptr->reg_state;
	unsigned int* __reg_state_ptr = &__reg_state;
	unsigned long long* features_ptr = &ptr->features;
	unsigned long long* hw_features_ptr = &ptr->hw_features;
	unsigned long long* vlan_features_ptr = &ptr->vlan_features;
	unsigned long long* hw_enc_features_ptr = &ptr->hw_enc_features;
	unsigned long long* mpls_features_ptr = &ptr->mpls_features;
	unsigned long long* gso_partial_features_ptr = &ptr->gso_partial_features;
	struct net_device_ops** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops** ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int* mtu_ptr = &ptr->mtu;
	unsigned char** dev_addr_ptr = &ptr->dev_addr;
	unsigned int* num_tx_queues_ptr = &ptr->num_tx_queues;

	{
		int i;
		char* array = name_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, char);
		}

	}

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*__reg_state_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*hw_features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*vlan_features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*hw_enc_features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*mpls_features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*gso_partial_features_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		if (*netdev_ops_ptr) {
			caller_unmarshal_kernel__net_device__global_netdev_ops__in(__pos, __msg, __ext, *netdev_ops_ptr);
		}

	}

	{
		if (*ethtool_ops_ptr) {
			caller_unmarshal_kernel__net_device__global_ethtool_ops__in(__pos, __msg, __ext, *ethtool_ops_ptr);
		}

	}

	{
		*mtu_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		(void)dev_addr_ptr;
	}

	{
		*num_tx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->reg_state = *__reg_state_ptr;
	}
}

void caller_marshal_kernel__net_device__global_netdev_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct net_device_ops const* ptr)
{
	fptr_ndo_change_mtu const* ndo_change_mtu_ptr = &ptr->ndo_change_mtu;
	fptr_ndo_do_ioctl const* ndo_do_ioctl_ptr = &ptr->ndo_do_ioctl;
	fptr_ndo_features_check const* ndo_features_check_ptr = &ptr->ndo_features_check;
	fptr_ndo_fix_features const* ndo_fix_features_ptr = &ptr->ndo_fix_features;
	fptr_ndo_get_stats64 const* ndo_get_stats64_ptr = &ptr->ndo_get_stats64;
	fptr_ndo_open const* ndo_open_ptr = &ptr->ndo_open;
	fptr_ndo_set_features const* ndo_set_features_ptr = &ptr->ndo_set_features;
	fptr_ndo_set_mac_address const* ndo_set_mac_address_ptr = &ptr->ndo_set_mac_address;
	fptr_ndo_set_rx_mode const* ndo_set_rx_mode_ptr = &ptr->ndo_set_rx_mode;
	fptr_ndo_set_tx_maxrate const* ndo_set_tx_maxrate_ptr = &ptr->ndo_set_tx_maxrate;
	fptr_ndo_setup_tc const* ndo_setup_tc_ptr = &ptr->ndo_setup_tc;
	fptr_ndo_start_xmit const* ndo_start_xmit_ptr = &ptr->ndo_start_xmit;
	fptr_ndo_stop const* ndo_stop_ptr = &ptr->ndo_stop;
	fptr_ndo_tx_timeout const* ndo_tx_timeout_ptr = &ptr->ndo_tx_timeout;

	{
		glue_pack(__pos, __msg, __ext, *ndo_change_mtu_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_do_ioctl_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_features_check_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_fix_features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_get_stats64_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_open_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_set_features_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_set_mac_address_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_set_rx_mode_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_set_tx_maxrate_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_setup_tc_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_start_xmit_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_stop_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ndo_tx_timeout_ptr);
	}

}

void callee_unmarshal_kernel__net_device__global_netdev_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct net_device_ops* ptr)
{
	fptr_ndo_change_mtu* ndo_change_mtu_ptr = &ptr->ndo_change_mtu;
	fptr_ndo_do_ioctl* ndo_do_ioctl_ptr = &ptr->ndo_do_ioctl;
	fptr_ndo_features_check* ndo_features_check_ptr = &ptr->ndo_features_check;
	fptr_ndo_fix_features* ndo_fix_features_ptr = &ptr->ndo_fix_features;
	fptr_ndo_get_stats64* ndo_get_stats64_ptr = &ptr->ndo_get_stats64;
	fptr_ndo_open* ndo_open_ptr = &ptr->ndo_open;
	fptr_ndo_set_features* ndo_set_features_ptr = &ptr->ndo_set_features;
	fptr_ndo_set_mac_address* ndo_set_mac_address_ptr = &ptr->ndo_set_mac_address;
	fptr_ndo_set_rx_mode* ndo_set_rx_mode_ptr = &ptr->ndo_set_rx_mode;
	fptr_ndo_set_tx_maxrate* ndo_set_tx_maxrate_ptr = &ptr->ndo_set_tx_maxrate;
	fptr_ndo_setup_tc* ndo_setup_tc_ptr = &ptr->ndo_setup_tc;
	fptr_ndo_start_xmit* ndo_start_xmit_ptr = &ptr->ndo_start_xmit;
	fptr_ndo_stop* ndo_stop_ptr = &ptr->ndo_stop;
	fptr_ndo_tx_timeout* ndo_tx_timeout_ptr = &ptr->ndo_tx_timeout;

	{
		*ndo_change_mtu_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_change_mtu);
	}

	{
		*ndo_do_ioctl_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_do_ioctl);
	}

	{
		*ndo_features_check_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_features_check);
	}

	{
		*ndo_fix_features_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_fix_features);
	}

	{
		*ndo_get_stats64_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_get_stats64);
	}

	{
		*ndo_open_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_open);
	}

	{
		*ndo_set_features_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_set_features);
	}

	{
		*ndo_set_mac_address_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_set_mac_address);
	}

	{
		*ndo_set_rx_mode_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_set_rx_mode);
	}

	{
		*ndo_set_tx_maxrate_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_set_tx_maxrate);
	}

	{
		*ndo_setup_tc_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_setup_tc);
	}

	{
		*ndo_start_xmit_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_start_xmit);
	}

	{
		*ndo_stop_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_stop);
	}

	{
		*ndo_tx_timeout_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, ndo_tx_timeout);
	}

	{
	}
}

void callee_marshal_kernel__net_device__global_netdev_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct net_device_ops const* ptr)
{

}

void caller_unmarshal_kernel__net_device__global_netdev_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct net_device_ops* ptr)
{

	{
	}
}

void caller_marshal_kernel__net_device__global_ethtool_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_ops const* ptr)
{

}

void callee_unmarshal_kernel__net_device__global_ethtool_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_ops* ptr)
{

	{
	}
}

void callee_marshal_kernel__net_device__global_ethtool_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_ops const* ptr)
{

}

void caller_unmarshal_kernel__net_device__global_ethtool_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_ops* ptr)
{

	{
	}
}

void caller_marshal_kernel__netif_tx_stop_all_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_tx_stop_all_queues_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int const* num_tx_queues_ptr = &ptr->num_tx_queues;

	{
		glue_pack(__pos, __msg, __ext, *num_tx_queues_ptr);
	}

}

void callee_unmarshal_kernel__netif_tx_stop_all_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_tx_stop_all_queues_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int* num_tx_queues_ptr = &ptr->num_tx_queues;

	{
		*num_tx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__netif_tx_stop_all_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_tx_stop_all_queues_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__netif_tx_stop_all_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_tx_stop_all_queues_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__netif_carrier_on__dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_carrier_on_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	int const* watchdog_timeo_ptr = &ptr->watchdog_timeo;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *watchdog_timeo_ptr);
	}

}

void callee_unmarshal_kernel__netif_carrier_on__dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_carrier_on_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	int* watchdog_timeo_ptr = &ptr->watchdog_timeo;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*watchdog_timeo_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__netif_carrier_on__dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_carrier_on_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	int const* watchdog_timeo_ptr = &ptr->watchdog_timeo;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *watchdog_timeo_ptr);
	}

}

void caller_unmarshal_kernel__netif_carrier_on__dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_carrier_on_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	int* watchdog_timeo_ptr = &ptr->watchdog_timeo;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*watchdog_timeo_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void caller_marshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_carrier_off_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

}

void callee_unmarshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_carrier_off_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long* state_ptr = &ptr->state;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void callee_marshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_carrier_off_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

}

void caller_unmarshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_carrier_off_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long* state_ptr = &ptr->state;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void caller_marshal_kernel__netdev_all_upper_get_next_dev_rcu__ret_net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__ret_net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__netdev_all_upper_get_next_dev_rcu__ret_net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__ret_net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__netdev_all_upper_get_next_dev_rcu__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__netdev_all_upper_get_next_dev_rcu__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__netdev_all_upper_get_next_dev_rcu__iter__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* ctx,
	struct list_head const* ptr)
{

}

void callee_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__iter__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* ctx,
	struct list_head* ptr)
{

	{
	}
}

void callee_marshal_kernel__netdev_all_upper_get_next_dev_rcu__iter__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* ctx,
	struct list_head const* ptr)
{

}

void caller_unmarshal_kernel__netdev_all_upper_get_next_dev_rcu__iter__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netdev_all_upper_get_next_dev_rcu_call_ctx const* ctx,
	struct list_head* ptr)
{

	{
	}
}

void caller_marshal_kernel__napi_disable__n__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_disable_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

}

void callee_unmarshal_kernel__napi_disable__n__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_disable_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	unsigned long* state_ptr = &ptr->state;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void callee_marshal_kernel__napi_disable__n__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_disable_call_ctx const* ctx,
	struct napi_struct const* ptr)
{

}

void caller_unmarshal_kernel__napi_disable__n__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_disable_call_ctx const* ctx,
	struct napi_struct* ptr)
{

	{
	}
}

void caller_marshal_kernel__poll__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct poll_call_ctx const* ctx,
	struct napi_struct const* ptr)
{

}

void callee_unmarshal_kernel__poll__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct poll_call_ctx const* ctx,
	struct napi_struct* ptr)
{

	{
	}
}

void callee_marshal_kernel__poll__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct poll_call_ctx const* ctx,
	struct napi_struct const* ptr)
{

}

void caller_unmarshal_kernel__poll__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct poll_call_ctx const* ctx,
	struct napi_struct* ptr)
{

	{
	}
}

void caller_marshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	unsigned int const* napi_id_ptr = &ptr->napi_id;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *napi_id_ptr);
	}

}

void callee_unmarshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	unsigned int* napi_id_ptr = &ptr->napi_id;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*napi_id_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	unsigned int const* napi_id_ptr = &ptr->napi_id;

	{
		glue_pack(__pos, __msg, __ext, *napi_id_ptr);
	}

}

void caller_unmarshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	unsigned int* napi_id_ptr = &ptr->napi_id;

	{
		*napi_id_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel____napi_schedule_irqoff__n__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_schedule_irqoff_call_ctx const* ctx,
	struct napi_struct const* ptr)
{

}

void callee_unmarshal_kernel____napi_schedule_irqoff__n__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_schedule_irqoff_call_ctx const* ctx,
	struct napi_struct* ptr)
{

	{
	}
}

void callee_marshal_kernel____napi_schedule_irqoff__n__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __napi_schedule_irqoff_call_ctx const* ctx,
	struct napi_struct const* ptr)
{

}

void caller_unmarshal_kernel____napi_schedule_irqoff__n__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __napi_schedule_irqoff_call_ctx const* ctx,
	struct napi_struct* ptr)
{

	{
	}
}

void caller_marshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	struct sk_buff* const* gro_list_ptr = &ptr->gro_list;
	unsigned int const* napi_id_ptr = &ptr->napi_id;

	{
		__maybe_unused const void* __adjusted = *gro_list_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*gro_list_ptr) {
			caller_marshal_kernel__napi_gro_receive__napi_struct_gro_list__in(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *napi_id_ptr);
	}

}

void callee_unmarshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	struct sk_buff** gro_list_ptr = &ptr->gro_list;
	unsigned int* napi_id_ptr = &ptr->napi_id;

	{
		*gro_list_ptr = glue_unpack(__pos, __msg, __ext, struct sk_buff*);
		if (*gro_list_ptr) {
			callee_unmarshal_kernel__napi_gro_receive__napi_struct_gro_list__in(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

	{
		*napi_id_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	struct sk_buff* const* gro_list_ptr = &ptr->gro_list;

	{
		if (*gro_list_ptr) {
			callee_marshal_kernel__napi_gro_receive__napi_struct_gro_list__in(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

}

void caller_unmarshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	struct sk_buff** gro_list_ptr = &ptr->gro_list;

	{
		if (*gro_list_ptr) {
			caller_unmarshal_kernel__napi_gro_receive__napi_struct_gro_list__in(__pos, __msg, __ext, ctx, *gro_list_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__napi_gro_receive__napi_struct_gro_list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct sk_buff const* ptr)
{

}

void callee_unmarshal_kernel__napi_gro_receive__napi_struct_gro_list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct sk_buff* ptr)
{

	{
	}
}

void callee_marshal_kernel__napi_gro_receive__napi_struct_gro_list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct sk_buff const* ptr)
{

}

void caller_unmarshal_kernel__napi_gro_receive__napi_struct_gro_list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct sk_buff* ptr)
{

	{
	}
}

void caller_marshal_kernel__napi_gro_receive__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned int const* hash_ptr = &ptr->hash;
	unsigned short const* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short const* protocol_ptr = &ptr->protocol;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *hash_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *vlan_tci_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *protocol_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__napi_gro_receive__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned int* hash_ptr = &ptr->hash;
	unsigned short* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short* protocol_ptr = &ptr->protocol;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*hash_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*vlan_tci_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*protocol_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__napi_gro_receive__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		(void)head_ptr;
	}

	{
		(void)data_ptr;
	}

}

void caller_unmarshal_kernel__napi_gro_receive__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		(void)head_ptr;
	}

	{
		(void)data_ptr;
	}

	{
	}
}

void caller_marshal_kernel__netif_receive_skb__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_receive_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned int const* hash_ptr = &ptr->hash;
	unsigned short const* vlan_proto_ptr = &ptr->vlan_proto;
	unsigned short const* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* protocol_ptr = &ptr->protocol;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *hash_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *vlan_proto_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *vlan_tci_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *protocol_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__netif_receive_skb__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_receive_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned int* hash_ptr = &ptr->hash;
	unsigned short* vlan_proto_ptr = &ptr->vlan_proto;
	unsigned short* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* protocol_ptr = &ptr->protocol;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*hash_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*vlan_proto_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*vlan_tci_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*protocol_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__netif_receive_skb__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_receive_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short const* vlan_proto_ptr = &ptr->vlan_proto;
	unsigned short const* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* protocol_ptr = &ptr->protocol;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *vlan_proto_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *vlan_tci_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *protocol_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void caller_unmarshal_kernel__netif_receive_skb__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_receive_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short* vlan_proto_ptr = &ptr->vlan_proto;
	unsigned short* vlan_tci_ptr = &ptr->vlan_tci;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* protocol_ptr = &ptr->protocol;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*vlan_proto_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*vlan_tci_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*protocol_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void caller_marshal_kernel__skb_checksum_help__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_checksum_help_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__skb_checksum_help__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_checksum_help_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__skb_checksum_help__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_checksum_help_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	signed char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned short const* mac_len_ptr = &ptr->mac_len;
	unsigned short const* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int const* headers_start_ptr = ptr->headers_start;
	unsigned char const* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short const* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short const* transport_header_ptr = &ptr->transport_header;
	unsigned short const* network_header_ptr = &ptr->network_header;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;

	{
		size_t i, len = 48;
		signed char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mac_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_mapping_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_start_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		size_t i, len = 0;
		unsigned char const* array = __pkt_type_offset_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *inner_network_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transport_header_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *network_header_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void caller_unmarshal_kernel__skb_checksum_help__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_checksum_help_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	signed char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned short* mac_len_ptr = &ptr->mac_len;
	unsigned short* queue_mapping_ptr = &ptr->queue_mapping;
	unsigned int* headers_start_ptr = ptr->headers_start;
	unsigned char* __pkt_type_offset_ptr = ptr->__pkt_type_offset;
	unsigned short* inner_network_header_ptr = &ptr->inner_network_header;
	unsigned short* transport_header_ptr = &ptr->transport_header;
	unsigned short* network_header_ptr = &ptr->network_header;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;

	{
		int i;
		signed char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			signed char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, signed char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mac_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*queue_mapping_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_start_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		int i;
		unsigned char* array = __pkt_type_offset_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*inner_network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*transport_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*network_header_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void caller_marshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_device_attach_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long long const* state_ptr = &ptr->state;
	unsigned int const* num_tx_queues_ptr = &ptr->num_tx_queues;
	int const* watchdog_timeo_ptr = &ptr->watchdog_timeo;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *num_tx_queues_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *watchdog_timeo_ptr);
	}

}

void callee_unmarshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_device_attach_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long long* state_ptr = &ptr->state;
	unsigned int* num_tx_queues_ptr = &ptr->num_tx_queues;
	int* watchdog_timeo_ptr = &ptr->watchdog_timeo;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*num_tx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*watchdog_timeo_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_device_attach_call_ctx const* ctx,
	struct net_device const* ptr)
{
	int const* watchdog_timeo_ptr = &ptr->watchdog_timeo;

	{
		glue_pack(__pos, __msg, __ext, *watchdog_timeo_ptr);
	}

}

void caller_unmarshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_device_attach_call_ctx const* ctx,
	struct net_device* ptr)
{
	int* watchdog_timeo_ptr = &ptr->watchdog_timeo;

	{
		*watchdog_timeo_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void caller_marshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_tx_wake_queue_call_ctx const* ctx,
	struct netdev_queue const* ptr)
{
	unsigned long long const* state_ptr = &ptr->state;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

}

void callee_unmarshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_tx_wake_queue_call_ctx const* ctx,
	struct netdev_queue* ptr)
{
	unsigned long long* state_ptr = &ptr->state;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void callee_marshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_tx_wake_queue_call_ctx const* ctx,
	struct netdev_queue const* ptr)
{

}

void caller_unmarshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_tx_wake_queue_call_ctx const* ctx,
	struct netdev_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_device_detach_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long long const* state_ptr = &ptr->state;
	unsigned int const* num_tx_queues_ptr = &ptr->num_tx_queues;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *num_tx_queues_ptr);
	}

}

void callee_unmarshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_device_detach_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long long* state_ptr = &ptr->state;
	unsigned int* num_tx_queues_ptr = &ptr->num_tx_queues;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*num_tx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_device_detach_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_device_detach_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__netif_wake_subqueue__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_wake_subqueue_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__netif_wake_subqueue__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_wake_subqueue_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__netif_wake_subqueue__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_wake_subqueue_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__netif_wake_subqueue__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_wake_subqueue_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__netif_set_real_num_rx_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_real_num_rx_queues_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int const* num_rx_queues_ptr = &ptr->num_rx_queues;

	{
		glue_pack(__pos, __msg, __ext, *num_rx_queues_ptr);
	}

}

void callee_unmarshal_kernel__netif_set_real_num_rx_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_real_num_rx_queues_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int* num_rx_queues_ptr = &ptr->num_rx_queues;

	{
		*num_rx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__netif_set_real_num_rx_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_real_num_rx_queues_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int const* num_rx_queues_ptr = &ptr->num_rx_queues;

	{
		glue_pack(__pos, __msg, __ext, *num_rx_queues_ptr);
	}

}

void caller_unmarshal_kernel__netif_set_real_num_rx_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_real_num_rx_queues_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int* num_rx_queues_ptr = &ptr->num_rx_queues;

	{
		*num_rx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__netif_set_real_num_tx_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_real_num_tx_queues_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int const* num_tx_queues_ptr = &ptr->num_tx_queues;
	unsigned char const* num_tc_ptr = &ptr->num_tc;

	{
		glue_pack(__pos, __msg, __ext, *num_tx_queues_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *num_tc_ptr);
	}

}

void callee_unmarshal_kernel__netif_set_real_num_tx_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_real_num_tx_queues_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int* num_tx_queues_ptr = &ptr->num_tx_queues;
	unsigned char* num_tc_ptr = &ptr->num_tc;

	{
		*num_tx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*num_tc_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void callee_marshal_kernel__netif_set_real_num_tx_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_real_num_tx_queues_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int const* num_tx_queues_ptr = &ptr->num_tx_queues;
	unsigned char const* num_tc_ptr = &ptr->num_tc;

	{
		glue_pack(__pos, __msg, __ext, *num_tx_queues_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *num_tc_ptr);
	}

}

void caller_unmarshal_kernel__netif_set_real_num_tx_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_real_num_tx_queues_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int* num_tx_queues_ptr = &ptr->num_tx_queues;
	unsigned char* num_tc_ptr = &ptr->num_tc;

	{
		*num_tx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*num_tc_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void caller_marshal_kernel__netif_set_xps_queue__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__netif_set_xps_queue__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__netif_set_xps_queue__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__netif_set_xps_queue__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__netif_set_xps_queue__mask__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* ctx,
	struct cpumask const* ptr)
{
	unsigned long long const* bits_ptr = ptr->bits;

	{
		size_t i, len = 1;
		unsigned long long const* array = bits_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned long long const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

}

void callee_unmarshal_kernel__netif_set_xps_queue__mask__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* ctx,
	struct cpumask* ptr)
{
	unsigned long long* bits_ptr = ptr->bits;

	{
		int i;
		unsigned long long* array = bits_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned long long* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned long long);
		}

	}

	{
	}
}

void callee_marshal_kernel__netif_set_xps_queue__mask__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* ctx,
	struct cpumask const* ptr)
{

}

void caller_unmarshal_kernel__netif_set_xps_queue__mask__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct netif_set_xps_queue_call_ctx const* ctx,
	struct cpumask* ptr)
{

	{
	}
}

void caller_marshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* ctx,
	struct cpumask const* ptr)
{
	unsigned long long const* bits_ptr = ptr->bits;

	{
		size_t i, len = 1;
		unsigned long long const* array = bits_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned long long const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

}

void callee_unmarshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* ctx,
	struct cpumask* ptr)
{
	unsigned long long* bits_ptr = ptr->bits;

	{
		int i;
		unsigned long long* array = bits_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned long long* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned long long);
		}

	}

	{
	}
}

void callee_marshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* ctx,
	struct cpumask const* ptr)
{

}

void caller_unmarshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* ctx,
	struct cpumask* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int const* __msi_enabled_ptr = &__msi_enabled;
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int const* __msix_enabled_ptr = &__msix_enabled;

	{
		glue_pack(__pos, __msg, __ext, *__msi_enabled_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__msix_enabled_ptr);
	}

}

void caller_unmarshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int* __msi_enabled_ptr = &__msi_enabled;
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int* __msix_enabled_ptr = &__msix_enabled;

	{
		*__msi_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__msix_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->msi_enabled = *__msi_enabled_ptr;
		ptr->msix_enabled = *__msix_enabled_ptr;
	}
}

void caller_marshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct msix_entry const* ptr)
{
	unsigned short const* entry_ptr = &ptr->entry;

	{
		glue_pack(__pos, __msg, __ext, *entry_ptr);
	}

}

void callee_unmarshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct msix_entry* ptr)
{
	unsigned short* entry_ptr = &ptr->entry;

	{
		*entry_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
	}
}

void callee_marshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct msix_entry const* ptr)
{
	unsigned int const* vector_ptr = &ptr->vector;
	unsigned short const* entry_ptr = &ptr->entry;

	{
		glue_pack(__pos, __msg, __ext, *vector_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *entry_ptr);
	}

}

void caller_unmarshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct msix_entry* ptr)
{
	unsigned int* vector_ptr = &ptr->vector;
	unsigned short* entry_ptr = &ptr->entry;

	{
		*vector_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*entry_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
	}
}

void caller_marshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int const* __msi_enabled_ptr = &__msi_enabled;
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int const* __msix_enabled_ptr = &__msix_enabled;
	unsigned int const* irq_ptr = &ptr->irq;

	{
		glue_pack(__pos, __msg, __ext, *__msi_enabled_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__msix_enabled_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

}

void caller_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int* __msi_enabled_ptr = &__msi_enabled;
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int* __msix_enabled_ptr = &__msix_enabled;
	unsigned int* irq_ptr = &ptr->irq;

	{
		*__msi_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__msix_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->msi_enabled = *__msi_enabled_ptr;
		ptr->msix_enabled = *__msix_enabled_ptr;
	}
}

void caller_marshal_kernel__dev_addr_del__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_addr_del_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char const* addr_len_ptr = &ptr->addr_len;

	{
		glue_pack(__pos, __msg, __ext, *addr_len_ptr);
	}

}

void callee_unmarshal_kernel__dev_addr_del__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_addr_del_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char* addr_len_ptr = &ptr->addr_len;

	{
		*addr_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void callee_marshal_kernel__dev_addr_del__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_addr_del_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__dev_addr_del__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_addr_del_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__dev_addr_add__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_addr_add_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char const* addr_len_ptr = &ptr->addr_len;

	{
		glue_pack(__pos, __msg, __ext, *addr_len_ptr);
	}

}

void callee_unmarshal_kernel__dev_addr_add__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_addr_add_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char* addr_len_ptr = &ptr->addr_len;

	{
		*addr_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void callee_marshal_kernel__dev_addr_add__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_addr_add_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__dev_addr_add__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_addr_add_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__sync__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sync_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__sync__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sync_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__sync__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sync_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__sync__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sync_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__unsync__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct unsync_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__unsync__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct unsync_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__unsync__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct unsync_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__unsync__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct unsync_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel____hw_addr_unsync_dev__list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct netdev_hw_addr_list const* ptr)
{
	struct list_head const* list_ptr = &ptr->list;
	int const* count_ptr = &ptr->count;

	{
		caller_marshal_kernel____hw_addr_unsync_dev__list_head__in(__pos, __msg, __ext, ctx, list_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *count_ptr);
	}

}

void callee_unmarshal_kernel____hw_addr_unsync_dev__list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct netdev_hw_addr_list* ptr)
{
	struct list_head* list_ptr = &ptr->list;
	int* count_ptr = &ptr->count;

	{
		callee_unmarshal_kernel____hw_addr_unsync_dev__list_head__in(__pos, __msg, __ext, ctx, list_ptr);
	}

	{
		*count_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel____hw_addr_unsync_dev__list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct netdev_hw_addr_list const* ptr)
{
	struct list_head const* list_ptr = &ptr->list;
	int const* count_ptr = &ptr->count;

	{
		callee_marshal_kernel____hw_addr_unsync_dev__list_head__in(__pos, __msg, __ext, ctx, list_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *count_ptr);
	}

}

void caller_unmarshal_kernel____hw_addr_unsync_dev__list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct netdev_hw_addr_list* ptr)
{
	struct list_head* list_ptr = &ptr->list;
	int* count_ptr = &ptr->count;

	{
		caller_unmarshal_kernel____hw_addr_unsync_dev__list_head__in(__pos, __msg, __ext, ctx, list_ptr);
	}

	{
		*count_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void caller_marshal_kernel____hw_addr_unsync_dev__list_head__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct list_head const* ptr)
{
	struct list_head* const* next_ptr = &ptr->next;

	{
		__maybe_unused const void* __adjusted = *next_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*next_ptr) {
			caller_marshal_kernel____hw_addr_unsync_dev__list_head_next__in(__pos, __msg, __ext, ctx, *next_ptr);
		}

	}

}

void callee_unmarshal_kernel____hw_addr_unsync_dev__list_head__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct list_head* ptr)
{
	struct list_head** next_ptr = &ptr->next;

	{
		*next_ptr = glue_unpack(__pos, __msg, __ext, struct list_head*);
		if (*next_ptr) {
			callee_unmarshal_kernel____hw_addr_unsync_dev__list_head_next__in(__pos, __msg, __ext, ctx, *next_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel____hw_addr_unsync_dev__list_head__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct list_head const* ptr)
{
	struct list_head* const* next_ptr = &ptr->next;

	{
		if (*next_ptr) {
			callee_marshal_kernel____hw_addr_unsync_dev__list_head_next__in(__pos, __msg, __ext, ctx, *next_ptr);
		}

	}

}

void caller_unmarshal_kernel____hw_addr_unsync_dev__list_head__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct list_head* ptr)
{
	struct list_head** next_ptr = &ptr->next;

	{
		if (*next_ptr) {
			caller_unmarshal_kernel____hw_addr_unsync_dev__list_head_next__in(__pos, __msg, __ext, ctx, *next_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel____hw_addr_unsync_dev__list_head_next__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct list_head const* ptr)
{

}

void callee_unmarshal_kernel____hw_addr_unsync_dev__list_head_next__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct list_head* ptr)
{

	{
	}
}

void callee_marshal_kernel____hw_addr_unsync_dev__list_head_next__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct list_head const* ptr)
{

}

void caller_unmarshal_kernel____hw_addr_unsync_dev__list_head_next__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct list_head* ptr)
{

	{
	}
}

void caller_marshal_kernel____hw_addr_unsync_dev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel____hw_addr_unsync_dev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel____hw_addr_unsync_dev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel____hw_addr_unsync_dev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_unsync_dev_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel____hw_addr_sync_dev__list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* ctx,
	struct netdev_hw_addr_list const* ptr)
{

}

void callee_unmarshal_kernel____hw_addr_sync_dev__list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* ctx,
	struct netdev_hw_addr_list* ptr)
{

	{
	}
}

void callee_marshal_kernel____hw_addr_sync_dev__list__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* ctx,
	struct netdev_hw_addr_list const* ptr)
{

}

void caller_unmarshal_kernel____hw_addr_sync_dev__list__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* ctx,
	struct netdev_hw_addr_list* ptr)
{

	{
	}
}

void caller_marshal_kernel____hw_addr_sync_dev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel____hw_addr_sync_dev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel____hw_addr_sync_dev__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel____hw_addr_sync_dev__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __hw_addr_sync_dev_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__eth_platform_get_mac_address__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct eth_platform_get_mac_address_call_ctx const* ctx,
	struct device const* ptr)
{

}

void callee_unmarshal_kernel__eth_platform_get_mac_address__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct eth_platform_get_mac_address_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void callee_marshal_kernel__eth_platform_get_mac_address__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct eth_platform_get_mac_address_call_ctx const* ctx,
	struct device const* ptr)
{

}

void caller_unmarshal_kernel__eth_platform_get_mac_address__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct eth_platform_get_mac_address_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void caller_marshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct alloc_etherdev_mqs_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;

	{
		(void)dev_addr_ptr;
	}

}

void callee_unmarshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct alloc_etherdev_mqs_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char** dev_addr_ptr = &ptr->dev_addr;

	{
		(void)dev_addr_ptr;
	}

	{
	}
}

void callee_marshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct alloc_etherdev_mqs_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;
	unsigned int const* num_tx_queues_ptr = &ptr->num_tx_queues;
	unsigned char const* num_tc_ptr = &ptr->num_tc;

	{
		__maybe_unused const void* __adjusted = *dev_addr_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_addr_ptr) {
			size_t i, len = (MAX_ADDR_LEN);
			unsigned char* array = *dev_addr_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *num_tx_queues_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *num_tc_ptr);
	}

}

void caller_unmarshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct alloc_etherdev_mqs_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char** dev_addr_ptr = &ptr->dev_addr;
	unsigned int* num_tx_queues_ptr = &ptr->num_tx_queues;
	unsigned char* num_tc_ptr = &ptr->num_tc;

	{
		size_t __size = sizeof(unsigned char) * glue_peek(__pos, __msg, __ext);
		*dev_addr_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, unsigned char*, (__size), (DEFAULT_GFP_FLAGS));
		if (*dev_addr_ptr) {
			int i;
			unsigned char* array = *dev_addr_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, unsigned char);
			}

		}

	}

	{
		*num_tx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*num_tc_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void caller_marshal_kernel__ndo_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_validate_addr_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;

	{
		__maybe_unused const void* __adjusted = *dev_addr_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_addr_ptr) {
			glue_pack(__pos, __msg, __ext, **dev_addr_ptr);
		}

	}

}

void callee_unmarshal_kernel__ndo_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_validate_addr_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char** dev_addr_ptr = &ptr->dev_addr;

	{
		*dev_addr_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*dev_addr_ptr) {
			**dev_addr_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
	}
}

void callee_marshal_kernel__ndo_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ndo_validate_addr_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;

	{
		(void)dev_addr_ptr;
	}

}

void caller_unmarshal_kernel__ndo_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ndo_validate_addr_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char** dev_addr_ptr = &ptr->dev_addr;

	{
		(void)dev_addr_ptr;
	}

	{
	}
}

void caller_marshal_kernel__dev_trans_start__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_trans_start_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int const* priv_flags_ptr = &ptr->priv_flags;
	unsigned int const* num_tx_queues_ptr = &ptr->num_tx_queues;

	{
		glue_pack(__pos, __msg, __ext, *priv_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *num_tx_queues_ptr);
	}

}

void callee_unmarshal_kernel__dev_trans_start__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_trans_start_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int* priv_flags_ptr = &ptr->priv_flags;
	unsigned int* num_tx_queues_ptr = &ptr->num_tx_queues;

	{
		*priv_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*num_tx_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__dev_trans_start__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_trans_start_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__dev_trans_start__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_trans_start_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_read_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_read_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_read_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_read_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_write_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_write_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_write_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_write_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mdio_if_info const* ptr)
{
	int const* prtad_ptr = &ptr->prtad;
	unsigned int const* mmds_ptr = &ptr->mmds;
	unsigned int const* mode_support_ptr = &ptr->mode_support;
	struct net_device* const* dev_ptr = &ptr->dev;

	{
		glue_pack(__pos, __msg, __ext, *prtad_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mmds_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mode_support_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mdio_mii_ioctl__mdio_if_info_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

}

void callee_unmarshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mdio_if_info* ptr)
{
	int* prtad_ptr = &ptr->prtad;
	unsigned int* mmds_ptr = &ptr->mmds;
	unsigned int* mode_support_ptr = &ptr->mode_support;
	struct net_device** dev_ptr = &ptr->dev;

	{
		*prtad_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*mmds_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mode_support_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mdio_mii_ioctl__mdio_if_info_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mdio_if_info const* ptr)
{
	struct net_device* const* dev_ptr = &ptr->dev;

	{
		if (*dev_ptr) {
			callee_marshal_kernel__mdio_mii_ioctl__mdio_if_info_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

}

void caller_unmarshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mdio_if_info* ptr)
{
	struct net_device** dev_ptr = &ptr->dev;

	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mdio_mii_ioctl__mdio_if_info_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__mdio_mii_ioctl__mdio_if_info_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__mdio_mii_ioctl__mdio_if_info_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__mdio_mii_ioctl__mdio_if_info_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__mdio_mii_ioctl__mdio_if_info_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__mdio_mii_ioctl__mii_data__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mii_ioctl_data const* ptr)
{

}

void callee_unmarshal_kernel__mdio_mii_ioctl__mii_data__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mii_ioctl_data* ptr)
{

	{
	}
}

void callee_marshal_kernel__mdio_mii_ioctl__mii_data__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mii_ioctl_data const* ptr)
{

}

void caller_unmarshal_kernel__mdio_mii_ioctl__mii_data__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mii_ioctl_data* ptr)
{

	{
	}
}

void caller_marshal_kernel__mdio45_probe__mdio__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* ctx,
	struct mdio_if_info const* ptr)
{
	int const* prtad_ptr = &ptr->prtad;
	unsigned int const* mmds_ptr = &ptr->mmds;
	unsigned int const* mode_support_ptr = &ptr->mode_support;
	struct net_device* const* dev_ptr = &ptr->dev;
	fptr_mdio_read const* mdio_read_ptr = &ptr->mdio_read;
	fptr_mdio_write const* mdio_write_ptr = &ptr->mdio_write;

	{
		glue_pack(__pos, __msg, __ext, *prtad_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mmds_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mode_support_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__mdio45_probe__mdio_if_info_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *mdio_read_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mdio_write_ptr);
	}

}

void callee_unmarshal_kernel__mdio45_probe__mdio__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* ctx,
	struct mdio_if_info* ptr)
{
	int* prtad_ptr = &ptr->prtad;
	unsigned int* mmds_ptr = &ptr->mmds;
	unsigned int* mode_support_ptr = &ptr->mode_support;
	struct net_device** dev_ptr = &ptr->dev;
	fptr_mdio_read* mdio_read_ptr = &ptr->mdio_read;
	fptr_mdio_write* mdio_write_ptr = &ptr->mdio_write;

	{
		*prtad_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*mmds_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mode_support_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__mdio45_probe__mdio_if_info_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*mdio_read_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, mdio_read);
	}

	{
		*mdio_write_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, mdio_write);
	}

	{
	}
}

void callee_marshal_kernel__mdio45_probe__mdio__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* ctx,
	struct mdio_if_info const* ptr)
{
	int const* prtad_ptr = &ptr->prtad;
	unsigned int const* mmds_ptr = &ptr->mmds;
	struct net_device* const* dev_ptr = &ptr->dev;

	{
		glue_pack(__pos, __msg, __ext, *prtad_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mmds_ptr);
	}

	{
		if (*dev_ptr) {
			callee_marshal_kernel__mdio45_probe__mdio_if_info_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

}

void caller_unmarshal_kernel__mdio45_probe__mdio__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* ctx,
	struct mdio_if_info* ptr)
{
	int* prtad_ptr = &ptr->prtad;
	unsigned int* mmds_ptr = &ptr->mmds;
	struct net_device** dev_ptr = &ptr->dev;

	{
		*prtad_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*mmds_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__mdio45_probe__mdio_if_info_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__mdio45_probe__mdio_if_info_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__mdio45_probe__mdio_if_info_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__mdio45_probe__mdio_if_info_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__mdio45_probe__mdio_if_info_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mdio45_probe_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* ctx,
	struct pci_driver const* ptr)
{

}

void callee_unmarshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* ctx,
	struct pci_driver* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* ctx,
	struct pci_driver const* ptr)
{

}

void caller_unmarshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* ctx,
	struct pci_driver* ptr)
{

	{
	}
}

void caller_marshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct pci_driver const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	struct pci_device_id* const* id_table_ptr = &ptr->id_table;
	fptr_probe const* probe_ptr = &ptr->probe;
	fptr_remove const* remove_ptr = &ptr->remove;
	fptr_shutdown const* shutdown_ptr = &ptr->shutdown;

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

	{
		__maybe_unused const void* __adjusted = *id_table_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*id_table_ptr) {
			size_t i, len;
			struct pci_device_id sentinel = { 0 };
			struct pci_device_id const* array = *id_table_ptr;
			for (len = 0; memcmp(&array[len], &sentinel, sizeof(array[0])); ++len) ;
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				struct pci_device_id const* element = &array[i];
				caller_marshal_kernel____pci_register_driver__pci_device_id__in(__pos, __msg, __ext, ctx, element);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *probe_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *remove_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *shutdown_ptr);
	}

}

void callee_unmarshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct pci_driver* ptr)
{
	char const** name_ptr = &ptr->name;
	struct pci_device_id** id_table_ptr = &ptr->id_table;
	fptr_probe* probe_ptr = &ptr->probe;
	fptr_remove* remove_ptr = &ptr->remove;
	fptr_shutdown* shutdown_ptr = &ptr->shutdown;

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
		size_t __size = sizeof(struct pci_device_id) * glue_peek(__pos, __msg, __ext);
		*id_table_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct pci_device_id*, (__size), (DEFAULT_GFP_FLAGS));
		if (*id_table_ptr) {
			size_t i, len;
			struct pci_device_id* array = *id_table_ptr;
			len = glue_unpack(__pos, __msg, __ext, size_t) - 1;
			memset(&array[len], 0x0, sizeof(array[len]));
			for (i = 0; i < len; ++i) {
				struct pci_device_id* element = &array[i];
				callee_unmarshal_kernel____pci_register_driver__pci_device_id__in(__pos, __msg, __ext, ctx, element);
			}

		}

	}

	{
		*probe_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, probe);
	}

	{
		*remove_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, remove);
	}

	{
		*shutdown_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, shutdown);
	}

	{
	}
}

void callee_marshal_kernel____pci_register_driver__drv__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
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
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
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

	{
	}
}

void caller_marshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
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
		glue_pack(__pos, __msg, __ext, *vendor_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *device_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *subvendor_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *subdevice_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *class_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *class_mask_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *driver_data_ptr);
	}

}

void callee_unmarshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
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
		*vendor_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*device_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*subvendor_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*subdevice_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*class_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*class_mask_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*driver_data_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void callee_marshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct pci_device_id const* ptr)
{

}

void caller_unmarshal_kernel____pci_register_driver__pci_device_id__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct pci_device_id* ptr)
{

	{
	}
}

void caller_marshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct module const* ptr)
{

}

void callee_unmarshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct module* ptr)
{

	{
	}
}

void callee_marshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct module const* ptr)
{

}

void caller_unmarshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct module* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__lvd_napi_enable__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_napi_enable_call_ctx const* ctx,
	struct napi_struct const* ptr)
{

}

void callee_unmarshal_kernel__lvd_napi_enable__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_napi_enable_call_ctx const* ctx,
	struct napi_struct* ptr)
{

	{
	}
}

void callee_marshal_kernel__lvd_napi_enable__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_napi_enable_call_ctx const* ctx,
	struct napi_struct const* ptr)
{

}

void caller_unmarshal_kernel__lvd_napi_enable__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_napi_enable_call_ctx const* ctx,
	struct napi_struct* ptr)
{

	{
	}
}

void caller_marshal_kernel__lvd_netif_trans_update__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_netif_trans_update_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__lvd_netif_trans_update__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_netif_trans_update_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__lvd_netif_trans_update__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_netif_trans_update_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__lvd_netif_trans_update__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_netif_trans_update_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__lvd_netif_tx_disable__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_netif_tx_disable_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__lvd_netif_tx_disable__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_netif_tx_disable_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__lvd_netif_tx_disable__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_netif_tx_disable_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__lvd_netif_tx_disable__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_netif_tx_disable_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__lvd_napi_schedule_irqoff__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_napi_schedule_irqoff_call_ctx const* ctx,
	struct napi_struct const* ptr)
{

}

void callee_unmarshal_kernel__lvd_napi_schedule_irqoff__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_napi_schedule_irqoff_call_ctx const* ctx,
	struct napi_struct* ptr)
{

	{
	}
}

void callee_marshal_kernel__lvd_napi_schedule_irqoff__napi__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_napi_schedule_irqoff_call_ctx const* ctx,
	struct napi_struct const* ptr)
{

}

void caller_unmarshal_kernel__lvd_napi_schedule_irqoff__napi__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_napi_schedule_irqoff_call_ctx const* ctx,
	struct napi_struct* ptr)
{

	{
	}
}

void caller_marshal_kernel__lvd_netif_tx_wake_all_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_netif_tx_wake_all_queues_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__lvd_netif_tx_wake_all_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_netif_tx_wake_all_queues_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__lvd_netif_tx_wake_all_queues__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_netif_tx_wake_all_queues_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__lvd_netif_tx_wake_all_queues__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_netif_tx_wake_all_queues_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct eth_validate_addr_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct eth_validate_addr_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct eth_validate_addr_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct eth_validate_addr_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_op_get_link_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_op_get_link_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_op_get_link_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_op_get_link_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ethtool_op_get_ts_info__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void callee_unmarshal_kernel__ethtool_op_get_ts_info__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__ethtool_op_get_ts_info__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* ctx,
	struct net_device const* ptr)
{

}

void caller_unmarshal_kernel__ethtool_op_get_ts_info__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* ctx,
	struct net_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__ethtool_op_get_ts_info__info__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* ctx,
	struct ethtool_ts_info const* ptr)
{
	unsigned int const* so_timestamping_ptr = &ptr->so_timestamping;
	int const* phc_index_ptr = &ptr->phc_index;

	{
		glue_pack(__pos, __msg, __ext, *so_timestamping_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *phc_index_ptr);
	}

}

void callee_unmarshal_kernel__ethtool_op_get_ts_info__info__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* ctx,
	struct ethtool_ts_info* ptr)
{
	unsigned int* so_timestamping_ptr = &ptr->so_timestamping;
	int* phc_index_ptr = &ptr->phc_index;

	{
		*so_timestamping_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*phc_index_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__ethtool_op_get_ts_info__info__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* ctx,
	struct ethtool_ts_info const* ptr)
{
	unsigned int const* so_timestamping_ptr = &ptr->so_timestamping;
	int const* phc_index_ptr = &ptr->phc_index;

	{
		glue_pack(__pos, __msg, __ext, *so_timestamping_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *phc_index_ptr);
	}

}

void caller_unmarshal_kernel__ethtool_op_get_ts_info__info__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ethtool_op_get_ts_info_call_ctx const* ctx,
	struct ethtool_ts_info* ptr)
{
	unsigned int* so_timestamping_ptr = &ptr->so_timestamping;
	int* phc_index_ptr = &ptr->phc_index;

	{
		*so_timestamping_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*phc_index_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void caller_marshal_kernel__ipv6_find_hdr__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ipv6_find_hdr_call_ctx const* ctx,
	struct sk_buff const* ptr)
{

}

void callee_unmarshal_kernel__ipv6_find_hdr__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ipv6_find_hdr_call_ctx const* ctx,
	struct sk_buff* ptr)
{

	{
	}
}

void callee_marshal_kernel__ipv6_find_hdr__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct ipv6_find_hdr_call_ctx const* ctx,
	struct sk_buff const* ptr)
{

}

void caller_unmarshal_kernel__ipv6_find_hdr__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct ipv6_find_hdr_call_ctx const* ctx,
	struct sk_buff* ptr)
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

