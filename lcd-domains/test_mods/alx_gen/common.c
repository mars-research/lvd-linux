#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel__ethtool_ops_set_settings__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ethtool_ops_set_settings__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ethtool_ops_set_settings__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ethtool_ops_set_settings__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ethtool_ops_set_settings__ecmd__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* ctx,
	struct ethtool_cmd const* ptr)
{
	unsigned int const* advertising_ptr = &ptr->advertising;
	unsigned short const* speed_ptr = &ptr->speed;
	unsigned char const* duplex_ptr = &ptr->duplex;
	unsigned char const* autoneg_ptr = &ptr->autoneg;
	unsigned short const* speed_hi_ptr = &ptr->speed_hi;
	
	{
		glue_pack(__pos, msg, ext, *advertising_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *speed_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *duplex_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *autoneg_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *speed_hi_ptr);
	}

}

void callee_unmarshal_kernel__ethtool_ops_set_settings__ecmd__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* ctx,
	struct ethtool_cmd* ptr)
{
	unsigned int* advertising_ptr = &ptr->advertising;
	unsigned short* speed_ptr = &ptr->speed;
	unsigned char* duplex_ptr = &ptr->duplex;
	unsigned char* autoneg_ptr = &ptr->autoneg;
	unsigned short* speed_hi_ptr = &ptr->speed_hi;
	
	{
		*advertising_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*speed_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*duplex_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*autoneg_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*speed_hi_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

}

void callee_marshal_kernel__ethtool_ops_set_settings__ecmd__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* ctx,
	struct ethtool_cmd const* ptr)
{
	
}

void caller_unmarshal_kernel__ethtool_ops_set_settings__ecmd__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_settings_call_ctx const* ctx,
	struct ethtool_cmd* ptr)
{
	
}

void caller_marshal_kernel__ethtool_ops_get_settings__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ethtool_ops_get_settings__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ethtool_ops_get_settings__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ethtool_ops_get_settings__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ethtool_ops_get_settings__ecmd__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* ctx,
	struct ethtool_cmd const* ptr)
{
	
}

void callee_unmarshal_kernel__ethtool_ops_get_settings__ecmd__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* ctx,
	struct ethtool_cmd* ptr)
{
	
}

void callee_marshal_kernel__ethtool_ops_get_settings__ecmd__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* ctx,
	struct ethtool_cmd const* ptr)
{
	unsigned int const* supported_ptr = &ptr->supported;
	unsigned int const* advertising_ptr = &ptr->advertising;
	unsigned short const* speed_ptr = &ptr->speed;
	unsigned char const* duplex_ptr = &ptr->duplex;
	unsigned char const* port_ptr = &ptr->port;
	unsigned char const* phy_address_ptr = &ptr->phy_address;
	unsigned char const* transceiver_ptr = &ptr->transceiver;
	unsigned char const* autoneg_ptr = &ptr->autoneg;
	unsigned short const* speed_hi_ptr = &ptr->speed_hi;
	
	{
		glue_pack(__pos, msg, ext, *supported_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *advertising_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *speed_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *duplex_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *port_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *phy_address_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *transceiver_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *autoneg_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *speed_hi_ptr);
	}

}

void caller_unmarshal_kernel__ethtool_ops_get_settings__ecmd__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_settings_call_ctx const* ctx,
	struct ethtool_cmd* ptr)
{
	unsigned int* supported_ptr = &ptr->supported;
	unsigned int* advertising_ptr = &ptr->advertising;
	unsigned short* speed_ptr = &ptr->speed;
	unsigned char* duplex_ptr = &ptr->duplex;
	unsigned char* port_ptr = &ptr->port;
	unsigned char* phy_address_ptr = &ptr->phy_address;
	unsigned char* transceiver_ptr = &ptr->transceiver;
	unsigned char* autoneg_ptr = &ptr->autoneg;
	unsigned short* speed_hi_ptr = &ptr->speed_hi;
	
	{
		*supported_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*advertising_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*speed_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*duplex_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*port_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*phy_address_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*transceiver_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*autoneg_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*speed_hi_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

}

void caller_marshal_kernel__ethtool_ops_get_msglevel__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_msglevel_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ethtool_ops_get_msglevel__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_msglevel_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ethtool_ops_get_msglevel__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_msglevel_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ethtool_ops_get_msglevel__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_msglevel_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ethtool_ops_set_msglevel__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_msglevel_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ethtool_ops_set_msglevel__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_msglevel_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ethtool_ops_set_msglevel__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_msglevel_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ethtool_ops_set_msglevel__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_msglevel_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__cancel_work_sync__work__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct cancel_work_sync_call_ctx const* ctx,
	struct work_struct const* ptr)
{
	
}

void callee_unmarshal_kernel__cancel_work_sync__work__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct cancel_work_sync_call_ctx const* ctx,
	struct work_struct* ptr)
{
	
}

void callee_marshal_kernel__cancel_work_sync__work__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct cancel_work_sync_call_ctx const* ctx,
	struct work_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__cancel_work_sync__work__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct cancel_work_sync_call_ctx const* ctx,
	struct work_struct* ptr)
{
	
}

void caller_marshal_kernel__probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct device const* dev_ptr = &ptr->dev;
	struct pci_bus* const* bus_ptr = &ptr->bus;
	unsigned int const* devfn_ptr = &ptr->devfn;
	unsigned short const* vendor_ptr = &ptr->vendor;
	unsigned short const* device_ptr = &ptr->device;
	unsigned short const* subsystem_vendor_ptr = &ptr->subsystem_vendor;
	unsigned short const* subsystem_device_ptr = &ptr->subsystem_device;
	unsigned char const* revision_ptr = &ptr->revision;
	
	{
		caller_marshal_kernel__probe__device__in(__pos, msg, ext, ctx, dev_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *bus_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__probe__pci_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *devfn_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *vendor_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *device_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *subsystem_vendor_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *subsystem_device_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *revision_ptr);
	}

}

void callee_unmarshal_kernel__probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct device* dev_ptr = &ptr->dev;
	struct pci_bus** bus_ptr = &ptr->bus;
	unsigned int* devfn_ptr = &ptr->devfn;
	unsigned short* vendor_ptr = &ptr->vendor;
	unsigned short* device_ptr = &ptr->device;
	unsigned short* subsystem_vendor_ptr = &ptr->subsystem_vendor;
	unsigned short* subsystem_device_ptr = &ptr->subsystem_device;
	unsigned char* revision_ptr = &ptr->revision;
	
	{
		callee_unmarshal_kernel__probe__device__in(__pos, msg, ext, ctx, dev_ptr);
	}

	{
		*bus_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct pci_bus*, (sizeof(struct pci_bus)), (DEFAULT_GFP_FLAGS));
		if (*bus_ptr) {
			callee_unmarshal_kernel__probe__pci_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		*devfn_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*vendor_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*device_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*subsystem_vendor_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*subsystem_device_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*revision_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

}

void callee_marshal_kernel__probe__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct device const* dev_ptr = &ptr->dev;
	struct pci_bus* const* bus_ptr = &ptr->bus;
	
	{
		callee_marshal_kernel__probe__device__in(__pos, msg, ext, ctx, dev_ptr);
	}

	{
		if (*bus_ptr) {
			callee_marshal_kernel__probe__pci_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void caller_unmarshal_kernel__probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct device* dev_ptr = &ptr->dev;
	struct pci_bus** bus_ptr = &ptr->bus;
	
	{
		caller_unmarshal_kernel__probe__device__in(__pos, msg, ext, ctx, dev_ptr);
	}

	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__probe__pci_bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void caller_marshal_kernel__probe__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct device const* ptr)
{
	unsigned long long* const* dma_mask_ptr = &ptr->dma_mask;
	
	{
		__maybe_unused const void* __adjusted = *dma_mask_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dma_mask_ptr) {
			size_t i, len = 1;
			unsigned long long const* array = *dma_mask_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned long long const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

}

void callee_unmarshal_kernel__probe__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct device* ptr)
{
	unsigned long long** dma_mask_ptr = &ptr->dma_mask;
	
	{
		*dma_mask_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned long long*, (sizeof(unsigned long long) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
		if (*dma_mask_ptr) {
			int i;
			unsigned long long* array = *dma_mask_ptr;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned long long* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, unsigned long long);
			}

		}

	}

}

void callee_marshal_kernel__probe__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct device const* ptr)
{
	unsigned long long* const* dma_mask_ptr = &ptr->dma_mask;
	
	{
		(void)dma_mask_ptr;
	}

}

void caller_unmarshal_kernel__probe__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct device* ptr)
{
	unsigned long long** dma_mask_ptr = &ptr->dma_mask;
	
	{
		(void)dma_mask_ptr;
	}

}

void caller_marshal_kernel__probe__pci_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void callee_unmarshal_kernel__probe__pci_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void callee_marshal_kernel__probe__pci_bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void caller_unmarshal_kernel__probe__pci_bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct probe_call_ctx const* ctx,
	struct pci_bus* ptr)
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
	fptr_probe const* probe_ptr = &ptr->probe;
	fptr_remove const* remove_ptr = &ptr->remove;
	struct pci_device_id* const* id_table_ptr = &ptr->id_table;
	
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
		glue_pack(__pos, msg, ext, *probe_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *remove_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *id_table_ptr;
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
	fptr_probe* probe_ptr = &ptr->probe;
	fptr_remove* remove_ptr = &ptr->remove;
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
		*probe_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, probe);
	}

	{
		*remove_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, remove);
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

void caller_marshal_kernel__ethtool_ops_get_link__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_link_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	
	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

}

void callee_unmarshal_kernel__ethtool_ops_get_link__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_link_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	
	{
		*state_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

}

void callee_marshal_kernel__ethtool_ops_get_link__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_link_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ethtool_ops_get_link__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_link_call_ctx const* ctx,
	struct net_device* ptr)
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

void caller_marshal_kernel__ethtool_ops_get_pauseparam__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ethtool_ops_get_pauseparam__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ethtool_ops_get_pauseparam__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ethtool_ops_get_pauseparam__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ethtool_ops_get_pauseparam__pause__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* ctx,
	struct ethtool_pauseparam const* ptr)
{
	
}

void callee_unmarshal_kernel__ethtool_ops_get_pauseparam__pause__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* ctx,
	struct ethtool_pauseparam* ptr)
{
	
}

void callee_marshal_kernel__ethtool_ops_get_pauseparam__pause__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* ctx,
	struct ethtool_pauseparam const* ptr)
{
	unsigned int const* autoneg_ptr = &ptr->autoneg;
	unsigned int const* rx_pause_ptr = &ptr->rx_pause;
	unsigned int const* tx_pause_ptr = &ptr->tx_pause;
	
	{
		glue_pack(__pos, msg, ext, *autoneg_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_pause_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tx_pause_ptr);
	}

}

void caller_unmarshal_kernel__ethtool_ops_get_pauseparam__pause__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_pauseparam_call_ctx const* ctx,
	struct ethtool_pauseparam* ptr)
{
	unsigned int* autoneg_ptr = &ptr->autoneg;
	unsigned int* rx_pause_ptr = &ptr->rx_pause;
	unsigned int* tx_pause_ptr = &ptr->tx_pause;
	
	{
		*autoneg_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*rx_pause_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*tx_pause_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void caller_marshal_kernel__ethtool_ops_set_pauseparam__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ethtool_ops_set_pauseparam__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ethtool_ops_set_pauseparam__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ethtool_ops_set_pauseparam__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ethtool_ops_set_pauseparam__pause__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* ctx,
	struct ethtool_pauseparam const* ptr)
{
	unsigned int const* autoneg_ptr = &ptr->autoneg;
	unsigned int const* rx_pause_ptr = &ptr->rx_pause;
	unsigned int const* tx_pause_ptr = &ptr->tx_pause;
	
	{
		glue_pack(__pos, msg, ext, *autoneg_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_pause_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tx_pause_ptr);
	}

}

void callee_unmarshal_kernel__ethtool_ops_set_pauseparam__pause__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* ctx,
	struct ethtool_pauseparam* ptr)
{
	unsigned int* autoneg_ptr = &ptr->autoneg;
	unsigned int* rx_pause_ptr = &ptr->rx_pause;
	unsigned int* tx_pause_ptr = &ptr->tx_pause;
	
	{
		*autoneg_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*rx_pause_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*tx_pause_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__ethtool_ops_set_pauseparam__pause__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* ctx,
	struct ethtool_pauseparam const* ptr)
{
	
}

void caller_unmarshal_kernel__ethtool_ops_set_pauseparam__pause__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_set_pauseparam_call_ctx const* ctx,
	struct ethtool_pauseparam* ptr)
{
	
}

void caller_marshal_kernel__ethtool_ops_get_strings__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_strings_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ethtool_ops_get_strings__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_strings_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ethtool_ops_get_strings__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_strings_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ethtool_ops_get_strings__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_strings_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ethtool_ops_get_ethtool_stats__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ethtool_ops_get_ethtool_stats__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ethtool_ops_get_ethtool_stats__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ethtool_ops_get_ethtool_stats__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ethtool_ops_get_ethtool_stats__estats__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* ctx,
	struct ethtool_stats const* ptr)
{
	
}

void callee_unmarshal_kernel__ethtool_ops_get_ethtool_stats__estats__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* ctx,
	struct ethtool_stats* ptr)
{
	
}

void callee_marshal_kernel__ethtool_ops_get_ethtool_stats__estats__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* ctx,
	struct ethtool_stats const* ptr)
{
	
}

void caller_unmarshal_kernel__ethtool_ops_get_ethtool_stats__estats__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_ethtool_stats_call_ctx const* ctx,
	struct ethtool_stats* ptr)
{
	
}

void caller_marshal_kernel__ethtool_ops_get_sset_count__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_sset_count_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ethtool_ops_get_sset_count__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_sset_count_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ethtool_ops_get_sset_count__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops_get_sset_count_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ethtool_ops_get_sset_count__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops_get_sset_count_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__net_device_ops_ndo_open__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_open_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__net_device_ops_ndo_open__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_open_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__net_device_ops_ndo_open__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_open_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__net_device_ops_ndo_open__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_open_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__net_device_ops_ndo_stop__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_stop_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__net_device_ops_ndo_stop__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_stop_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__net_device_ops_ndo_stop__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_stop_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__net_device_ops_ndo_stop__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_stop_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__net_device_ops_ndo_start_xmit__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* len_ptr = &ptr->len;
	unsigned char __ip_summed = ptr->ip_summed;
	unsigned char const* __ip_summed_ptr = &__ip_summed;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	
	{
		glue_pack(__pos, msg, ext, *len_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *__ip_summed_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*head_ptr) {
			size_t i, len = (ctx->skb->len);
			unsigned char* array = *head_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

}

void callee_unmarshal_kernel__net_device_ops_ndo_start_xmit__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* len_ptr = &ptr->len;
	unsigned char __ip_summed = ptr->ip_summed;
	unsigned char* __ip_summed_ptr = &__ip_summed;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	
	{
		*len_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*__ip_summed_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*end_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned char*, (sizeof(unsigned char) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
		if (*head_ptr) {
			int i;
			unsigned char* array = *head_ptr;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, unsigned char);
			}

		}

	}

	{
		ptr->ip_summed = *__ip_summed_ptr;
	}
}

void callee_marshal_kernel__net_device_ops_ndo_start_xmit__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned char* const* head_ptr = &ptr->head;
	
	{
		(void)head_ptr;
	}

}

void caller_unmarshal_kernel__net_device_ops_ndo_start_xmit__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned char** head_ptr = &ptr->head;
	
	{
		(void)head_ptr;
	}

}

void caller_marshal_kernel__net_device_ops_ndo_start_xmit__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__net_device_ops_ndo_start_xmit__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__net_device_ops_ndo_start_xmit__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__net_device_ops_ndo_start_xmit__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_start_xmit_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__net_device_ops_ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* ctx,
	struct net_device const* ptr)
{
	struct netdev_hw_addr_list const* mc_ptr = &ptr->mc;
	
	{
		caller_marshal_kernel__net_device_ops_ndo_set_rx_mode__netdev_hw_addr_list__in(__pos, msg, ext, ctx, mc_ptr);
	}

}

void callee_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* ctx,
	struct net_device* ptr)
{
	struct netdev_hw_addr_list* mc_ptr = &ptr->mc;
	
	{
		callee_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__netdev_hw_addr_list__in(__pos, msg, ext, ctx, mc_ptr);
	}

}

void callee_marshal_kernel__net_device_ops_ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* ctx,
	struct net_device const* ptr)
{
	struct netdev_hw_addr_list const* mc_ptr = &ptr->mc;
	
	{
		callee_marshal_kernel__net_device_ops_ndo_set_rx_mode__netdev_hw_addr_list__in(__pos, msg, ext, ctx, mc_ptr);
	}

}

void caller_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* ctx,
	struct net_device* ptr)
{
	struct netdev_hw_addr_list* mc_ptr = &ptr->mc;
	
	{
		caller_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__netdev_hw_addr_list__in(__pos, msg, ext, ctx, mc_ptr);
	}

}

void caller_marshal_kernel__net_device_ops_ndo_set_rx_mode__netdev_hw_addr_list__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* ctx,
	struct netdev_hw_addr_list const* ptr)
{
	struct list_head const* list_ptr = &ptr->list;
	
	{
		caller_marshal_kernel__net_device_ops_ndo_set_rx_mode__list_head__in(__pos, msg, ext, ctx, list_ptr);
	}

}

void callee_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__netdev_hw_addr_list__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* ctx,
	struct netdev_hw_addr_list* ptr)
{
	struct list_head* list_ptr = &ptr->list;
	
	{
		callee_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__list_head__in(__pos, msg, ext, ctx, list_ptr);
	}

}

void callee_marshal_kernel__net_device_ops_ndo_set_rx_mode__netdev_hw_addr_list__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* ctx,
	struct netdev_hw_addr_list const* ptr)
{
	struct list_head const* list_ptr = &ptr->list;
	
	{
		callee_marshal_kernel__net_device_ops_ndo_set_rx_mode__list_head__in(__pos, msg, ext, ctx, list_ptr);
	}

}

void caller_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__netdev_hw_addr_list__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* ctx,
	struct netdev_hw_addr_list* ptr)
{
	struct list_head* list_ptr = &ptr->list;
	
	{
		caller_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__list_head__in(__pos, msg, ext, ctx, list_ptr);
	}

}

void caller_marshal_kernel__net_device_ops_ndo_set_rx_mode__list_head__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* ctx,
	struct list_head const* ptr)
{
	
}

void callee_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__list_head__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* ctx,
	struct list_head* ptr)
{
	
}

void callee_marshal_kernel__net_device_ops_ndo_set_rx_mode__list_head__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* ctx,
	struct list_head const* ptr)
{
	
}

void caller_unmarshal_kernel__net_device_ops_ndo_set_rx_mode__list_head__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_rx_mode_call_ctx const* ctx,
	struct list_head* ptr)
{
	
}

void caller_marshal_kernel__net_device_ops_ndo_set_mac_address__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char const* addr_assign_type_ptr = &ptr->addr_assign_type;
	unsigned char const* addr_len_ptr = &ptr->addr_len;
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;
	
	{
		glue_pack(__pos, msg, ext, *addr_assign_type_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *addr_len_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *dev_addr_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		(void)dev_addr_ptr;
	}

}

void callee_unmarshal_kernel__net_device_ops_ndo_set_mac_address__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char* addr_assign_type_ptr = &ptr->addr_assign_type;
	unsigned char* addr_len_ptr = &ptr->addr_len;
	unsigned char** dev_addr_ptr = &ptr->dev_addr;
	
	{
		*addr_assign_type_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*addr_len_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*dev_addr_ptr = glue_unpack(__pos, msg, ext, unsigned char*);
		(void)dev_addr_ptr;
	}

}

void callee_marshal_kernel__net_device_ops_ndo_set_mac_address__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char const* addr_assign_type_ptr = &ptr->addr_assign_type;
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;
	
	{
		glue_pack(__pos, msg, ext, *addr_assign_type_ptr);
	}

	{
		if (*dev_addr_ptr) {
			size_t i, len = (MAX_ADDR_LEN);
			unsigned char* array = *dev_addr_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

}

void caller_unmarshal_kernel__net_device_ops_ndo_set_mac_address__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char* addr_assign_type_ptr = &ptr->addr_assign_type;
	unsigned char** dev_addr_ptr = &ptr->dev_addr;
	
	{
		*addr_assign_type_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		if (*dev_addr_ptr) {
			int i;
			unsigned char* array = *dev_addr_ptr;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, unsigned char);
			}

		}

	}

}

void caller_marshal_kernel__net_device_ops_ndo_set_mac_address__sockaddr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* ctx,
	struct sockaddr const* ptr)
{
	char const* sa_data_ptr = ptr->sa_data;
	
	{
		size_t i, len = 14;
		char const* array = sa_data_ptr;
		glue_pack(__pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(__pos, msg, ext, *element);
		}

	}

}

void callee_unmarshal_kernel__net_device_ops_ndo_set_mac_address__sockaddr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* ctx,
	struct sockaddr* ptr)
{
	char* sa_data_ptr = ptr->sa_data;
	
	{
		int i;
		char* array = sa_data_ptr;
		size_t len = glue_unpack(__pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(__pos, msg, ext, char);
		}

	}

}

void callee_marshal_kernel__net_device_ops_ndo_set_mac_address__sockaddr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* ctx,
	struct sockaddr const* ptr)
{
	
}

void caller_unmarshal_kernel__net_device_ops_ndo_set_mac_address__sockaddr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_set_mac_address_call_ctx const* ctx,
	struct sockaddr* ptr)
{
	
}

void caller_marshal_kernel__net_device_ops_ndo_do_ioctl__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	
	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

}

void callee_unmarshal_kernel__net_device_ops_ndo_do_ioctl__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	
	{
		*state_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

}

void callee_marshal_kernel__net_device_ops_ndo_do_ioctl__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__net_device_ops_ndo_do_ioctl__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__net_device_ops_ndo_do_ioctl__ifr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* ctx,
	struct ifreq const* ptr)
{
	
}

void callee_unmarshal_kernel__net_device_ops_ndo_do_ioctl__ifr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* ctx,
	struct ifreq* ptr)
{
	
}

void callee_marshal_kernel__net_device_ops_ndo_do_ioctl__ifr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* ctx,
	struct ifreq const* ptr)
{
	
}

void caller_unmarshal_kernel__net_device_ops_ndo_do_ioctl__ifr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_do_ioctl_call_ctx const* ctx,
	struct ifreq* ptr)
{
	
}

void caller_marshal_kernel__net_device_ops_ndo_change_mtu__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_change_mtu_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	unsigned int const* mtu_ptr = &ptr->mtu;
	
	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *mtu_ptr);
	}

}

void callee_unmarshal_kernel__net_device_ops_ndo_change_mtu__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_change_mtu_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	unsigned int* mtu_ptr = &ptr->mtu;
	
	{
		*state_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		*mtu_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__net_device_ops_ndo_change_mtu__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_change_mtu_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int const* mtu_ptr = &ptr->mtu;
	
	{
		glue_pack(__pos, msg, ext, *mtu_ptr);
	}

}

void caller_unmarshal_kernel__net_device_ops_ndo_change_mtu__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_change_mtu_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int* mtu_ptr = &ptr->mtu;
	
	{
		*mtu_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void caller_marshal_kernel__net_device_ops_ndo_tx_timeout__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_tx_timeout_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__net_device_ops_ndo_tx_timeout__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_tx_timeout_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__net_device_ops_ndo_tx_timeout__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_tx_timeout_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__net_device_ops_ndo_tx_timeout__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_tx_timeout_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__net_device_ops_ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{
	
}

void callee_unmarshal_kernel__net_device_ops_ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{
	
}

void callee_marshal_kernel__net_device_ops_ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{
	unsigned long long const* rx_packets_ptr = &ptr->rx_packets;
	unsigned long long const* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long const* rx_bytes_ptr = &ptr->rx_bytes;
	unsigned long long const* tx_bytes_ptr = &ptr->tx_bytes;
	unsigned long long const* rx_errors_ptr = &ptr->rx_errors;
	unsigned long long const* tx_errors_ptr = &ptr->tx_errors;
	unsigned long long const* rx_dropped_ptr = &ptr->rx_dropped;
	unsigned long long const* multicast_ptr = &ptr->multicast;
	unsigned long long const* collisions_ptr = &ptr->collisions;
	unsigned long long const* rx_length_errors_ptr = &ptr->rx_length_errors;
	unsigned long long const* rx_crc_errors_ptr = &ptr->rx_crc_errors;
	unsigned long long const* rx_frame_errors_ptr = &ptr->rx_frame_errors;
	unsigned long long const* rx_fifo_errors_ptr = &ptr->rx_fifo_errors;
	unsigned long long const* tx_aborted_errors_ptr = &ptr->tx_aborted_errors;
	unsigned long long const* tx_fifo_errors_ptr = &ptr->tx_fifo_errors;
	unsigned long long const* tx_window_errors_ptr = &ptr->tx_window_errors;
	
	{
		glue_pack(__pos, msg, ext, *rx_packets_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tx_packets_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_bytes_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tx_bytes_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tx_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_dropped_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *multicast_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *collisions_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_length_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_crc_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_frame_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_fifo_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tx_aborted_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tx_fifo_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tx_window_errors_ptr);
	}

}

void caller_unmarshal_kernel__net_device_ops_ndo_get_stats64__ret_rtnl_link_stats64__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{
	unsigned long long* rx_packets_ptr = &ptr->rx_packets;
	unsigned long long* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long* rx_bytes_ptr = &ptr->rx_bytes;
	unsigned long long* tx_bytes_ptr = &ptr->tx_bytes;
	unsigned long long* rx_errors_ptr = &ptr->rx_errors;
	unsigned long long* tx_errors_ptr = &ptr->tx_errors;
	unsigned long long* rx_dropped_ptr = &ptr->rx_dropped;
	unsigned long long* multicast_ptr = &ptr->multicast;
	unsigned long long* collisions_ptr = &ptr->collisions;
	unsigned long long* rx_length_errors_ptr = &ptr->rx_length_errors;
	unsigned long long* rx_crc_errors_ptr = &ptr->rx_crc_errors;
	unsigned long long* rx_frame_errors_ptr = &ptr->rx_frame_errors;
	unsigned long long* rx_fifo_errors_ptr = &ptr->rx_fifo_errors;
	unsigned long long* tx_aborted_errors_ptr = &ptr->tx_aborted_errors;
	unsigned long long* tx_fifo_errors_ptr = &ptr->tx_fifo_errors;
	unsigned long long* tx_window_errors_ptr = &ptr->tx_window_errors;
	
	{
		*rx_packets_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*tx_packets_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*rx_bytes_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*tx_bytes_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*rx_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*tx_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*rx_dropped_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*multicast_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*collisions_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*rx_length_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*rx_crc_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*rx_frame_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*rx_fifo_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*tx_aborted_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*tx_fifo_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*tx_window_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

}

void caller_marshal_kernel__net_device_ops_ndo_get_stats64__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__net_device_ops_ndo_get_stats64__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__net_device_ops_ndo_get_stats64__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__net_device_ops_ndo_get_stats64__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__net_device_ops_ndo_get_stats64__net_stats__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{
	
}

void callee_unmarshal_kernel__net_device_ops_ndo_get_stats64__net_stats__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{
	
}

void callee_marshal_kernel__net_device_ops_ndo_get_stats64__net_stats__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64 const* ptr)
{
	unsigned long long const* rx_packets_ptr = &ptr->rx_packets;
	unsigned long long const* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long const* rx_bytes_ptr = &ptr->rx_bytes;
	unsigned long long const* tx_bytes_ptr = &ptr->tx_bytes;
	unsigned long long const* rx_errors_ptr = &ptr->rx_errors;
	unsigned long long const* tx_errors_ptr = &ptr->tx_errors;
	unsigned long long const* rx_dropped_ptr = &ptr->rx_dropped;
	unsigned long long const* multicast_ptr = &ptr->multicast;
	unsigned long long const* collisions_ptr = &ptr->collisions;
	unsigned long long const* rx_length_errors_ptr = &ptr->rx_length_errors;
	unsigned long long const* rx_crc_errors_ptr = &ptr->rx_crc_errors;
	unsigned long long const* rx_frame_errors_ptr = &ptr->rx_frame_errors;
	unsigned long long const* rx_fifo_errors_ptr = &ptr->rx_fifo_errors;
	unsigned long long const* tx_aborted_errors_ptr = &ptr->tx_aborted_errors;
	unsigned long long const* tx_fifo_errors_ptr = &ptr->tx_fifo_errors;
	unsigned long long const* tx_window_errors_ptr = &ptr->tx_window_errors;
	
	{
		glue_pack(__pos, msg, ext, *rx_packets_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tx_packets_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_bytes_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tx_bytes_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tx_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_dropped_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *multicast_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *collisions_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_length_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_crc_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_frame_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *rx_fifo_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tx_aborted_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tx_fifo_errors_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tx_window_errors_ptr);
	}

}

void caller_unmarshal_kernel__net_device_ops_ndo_get_stats64__net_stats__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_get_stats64_call_ctx const* ctx,
	struct rtnl_link_stats64* ptr)
{
	unsigned long long* rx_packets_ptr = &ptr->rx_packets;
	unsigned long long* tx_packets_ptr = &ptr->tx_packets;
	unsigned long long* rx_bytes_ptr = &ptr->rx_bytes;
	unsigned long long* tx_bytes_ptr = &ptr->tx_bytes;
	unsigned long long* rx_errors_ptr = &ptr->rx_errors;
	unsigned long long* tx_errors_ptr = &ptr->tx_errors;
	unsigned long long* rx_dropped_ptr = &ptr->rx_dropped;
	unsigned long long* multicast_ptr = &ptr->multicast;
	unsigned long long* collisions_ptr = &ptr->collisions;
	unsigned long long* rx_length_errors_ptr = &ptr->rx_length_errors;
	unsigned long long* rx_crc_errors_ptr = &ptr->rx_crc_errors;
	unsigned long long* rx_frame_errors_ptr = &ptr->rx_frame_errors;
	unsigned long long* rx_fifo_errors_ptr = &ptr->rx_fifo_errors;
	unsigned long long* tx_aborted_errors_ptr = &ptr->tx_aborted_errors;
	unsigned long long* tx_fifo_errors_ptr = &ptr->tx_fifo_errors;
	unsigned long long* tx_window_errors_ptr = &ptr->tx_window_errors;
	
	{
		*rx_packets_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*tx_packets_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*rx_bytes_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*tx_bytes_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*rx_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*tx_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*rx_dropped_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*multicast_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*collisions_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*rx_length_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*rx_crc_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*rx_frame_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*rx_fifo_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*tx_aborted_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*tx_fifo_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		*tx_window_errors_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

}

void caller_marshal_kernel__net_device_ops_ndo_fix_features__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_fix_features_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int const* mtu_ptr = &ptr->mtu;
	
	{
		glue_pack(__pos, msg, ext, *mtu_ptr);
	}

}

void callee_unmarshal_kernel__net_device_ops_ndo_fix_features__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_fix_features_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int* mtu_ptr = &ptr->mtu;
	
	{
		*mtu_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__net_device_ops_ndo_fix_features__netdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_fix_features_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__net_device_ops_ndo_fix_features__netdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_fix_features_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_bus_write_config_word_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_bus_write_config_word_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void callee_marshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_bus_write_config_word_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_bus_write_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_bus_write_config_word_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void caller_marshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_bus_read_config_word_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_bus_read_config_word_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void callee_marshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_bus_read_config_word_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_bus_read_config_word__bus__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_bus_read_config_word_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void caller_marshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_read_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_read_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_read_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mdio_read__net_device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_read_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_write_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_write_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_write_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__mdio_write__net_device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_write_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mdio_if_info const* ptr)
{
	int const* prtad_ptr = &ptr->prtad;
	unsigned int const* mmds_ptr = &ptr->mmds;
	unsigned int const* mode_support_ptr = &ptr->mode_support;
	fptr_mdio_read const* mdio_read_ptr = &ptr->mdio_read;
	fptr_mdio_write const* mdio_write_ptr = &ptr->mdio_write;
	
	{
		glue_pack(__pos, msg, ext, *prtad_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *mmds_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *mode_support_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *mdio_read_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *mdio_write_ptr);
	}

}

void callee_unmarshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mdio_if_info* ptr)
{
	int* prtad_ptr = &ptr->prtad;
	unsigned int* mmds_ptr = &ptr->mmds;
	unsigned int* mode_support_ptr = &ptr->mode_support;
	fptr_mdio_read* mdio_read_ptr = &ptr->mdio_read;
	fptr_mdio_write* mdio_write_ptr = &ptr->mdio_write;
	
	{
		*prtad_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*mmds_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*mode_support_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*mdio_read_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mdio_read);
	}

	{
		*mdio_write_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, mdio_write);
	}

}

void callee_marshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mdio_if_info const* ptr)
{
	
}

void caller_unmarshal_kernel__mdio_mii_ioctl__mdio__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mdio_if_info* ptr)
{
	
}

void caller_marshal_kernel__mdio_mii_ioctl__mii_data__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mii_ioctl_data const* ptr)
{
	
}

void callee_unmarshal_kernel__mdio_mii_ioctl__mii_data__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mii_ioctl_data* ptr)
{
	
}

void callee_marshal_kernel__mdio_mii_ioctl__mii_data__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mii_ioctl_data const* ptr)
{
	
}

void caller_unmarshal_kernel__mdio_mii_ioctl__mii_data__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mdio_mii_ioctl_call_ctx const* ctx,
	struct mii_ioctl_data* ptr)
{
	
}

void caller_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int const* irq_ptr = &ptr->irq;
	unsigned short const* dev_flags_ptr = &ptr->dev_flags;
	
	{
		glue_pack(__pos, msg, ext, *irq_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *dev_flags_ptr);
	}

}

void callee_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int* irq_ptr = &ptr->irq;
	unsigned short* dev_flags_ptr = &ptr->dev_flags;
	
	{
		*irq_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*dev_flags_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

}

void callee_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int const* irq_ptr = &ptr->irq;
	
	{
		glue_pack(__pos, msg, ext, *irq_ptr);
	}

}

void caller_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int* irq_ptr = &ptr->irq;
	
	{
		*irq_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

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
	struct atomic64_t const* data_ptr = &ptr->data;
	
	{
		caller_marshal_kernel__queue_work_on__atomic64_t__io(__pos, msg, ext, ctx, data_ptr);
	}

}

void callee_unmarshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* ctx,
	struct work_struct* ptr)
{
	struct atomic64_t* data_ptr = &ptr->data;
	
	{
		callee_unmarshal_kernel__queue_work_on__atomic64_t__io(__pos, msg, ext, ctx, data_ptr);
	}

}

void callee_marshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* ctx,
	struct work_struct const* ptr)
{
	struct atomic64_t const* data_ptr = &ptr->data;
	
	{
		callee_marshal_kernel__queue_work_on__atomic64_t__io(__pos, msg, ext, ctx, data_ptr);
	}

}

void caller_unmarshal_kernel__queue_work_on__work__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* ctx,
	struct work_struct* ptr)
{
	struct atomic64_t* data_ptr = &ptr->data;
	
	{
		caller_unmarshal_kernel__queue_work_on__atomic64_t__io(__pos, msg, ext, ctx, data_ptr);
	}

}

void caller_marshal_kernel__queue_work_on__atomic64_t__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* ctx,
	struct atomic64_t const* ptr)
{
	long const* counter_ptr = &ptr->counter;
	
	{
		glue_pack(__pos, msg, ext, *counter_ptr);
	}

}

void callee_unmarshal_kernel__queue_work_on__atomic64_t__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* ctx,
	struct atomic64_t* ptr)
{
	long* counter_ptr = &ptr->counter;
	
	{
		*counter_ptr = glue_unpack(__pos, msg, ext, long);
	}

}

void callee_marshal_kernel__queue_work_on__atomic64_t__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_work_on_call_ctx const* ctx,
	struct atomic64_t const* ptr)
{
	long const* counter_ptr = &ptr->counter;
	
	{
		glue_pack(__pos, msg, ext, *counter_ptr);
	}

}

void caller_unmarshal_kernel__queue_work_on__atomic64_t__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_work_on_call_ctx const* ctx,
	struct atomic64_t* ptr)
{
	long* counter_ptr = &ptr->counter;
	
	{
		*counter_ptr = glue_unpack(__pos, msg, ext, long);
	}

}

void caller_marshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct consume_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	
	{
		glue_pack(__pos, msg, ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*head_ptr) {
			size_t i, len = (ctx->skb->len);
			unsigned char* array = *head_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

}

void callee_unmarshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct consume_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	
	{
		*end_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack(__pos, msg, ext, unsigned char*);
		if (*head_ptr) {
			int i;
			unsigned char* array = *head_ptr;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, unsigned char);
			}

		}

	}

}

void callee_marshal_kernel__consume_skb__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
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
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct consume_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned char** head_ptr = &ptr->head;
	
	{
		glue_remove_shadow(*head_ptr);
		(void)head_ptr;
	}

}

void caller_marshal_kernel__napi_complete_done__n__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_complete_done_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	
	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

}

void callee_unmarshal_kernel__napi_complete_done__n__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_complete_done_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	
	{
		*state_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

}

void callee_marshal_kernel__napi_complete_done__n__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_complete_done_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	
	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

}

void caller_unmarshal_kernel__napi_complete_done__n__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_complete_done_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	
	{
		*state_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

}

void caller_marshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;
	struct netdev_queue* const* _tx_ptr = &ptr->_tx;
	
	{
		(void)dev_addr_ptr;
	}

	{
		if (*_tx_ptr) {
			caller_marshal_kernel__alloc_etherdev_mqs__net_device__tx__out(__pos, msg, ext, ctx, *_tx_ptr);
		}

	}

}

void callee_unmarshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char** dev_addr_ptr = &ptr->dev_addr;
	struct netdev_queue** _tx_ptr = &ptr->_tx;
	
	{
		(void)dev_addr_ptr;
	}

	{
		if (*_tx_ptr) {
			callee_unmarshal_kernel__alloc_etherdev_mqs__net_device__tx__out(__pos, msg, ext, ctx, *_tx_ptr);
		}

	}

}

void callee_marshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;
	unsigned int const* num_tx_queues_ptr = &ptr->num_tx_queues;
	struct netdev_queue* const* _tx_ptr = &ptr->_tx;
	
	{
		__maybe_unused const void* __adjusted = *dev_addr_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_addr_ptr) {
			size_t i, len = (MAX_ADDR_LEN);
			unsigned char* array = *dev_addr_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	{
		glue_pack(__pos, msg, ext, *num_tx_queues_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *_tx_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*_tx_ptr) {
			callee_marshal_kernel__alloc_etherdev_mqs__net_device__tx__out(__pos, msg, ext, ctx, *_tx_ptr);
		}

	}

}

void caller_unmarshal_kernel__alloc_etherdev_mqs__ret_net_device__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char** dev_addr_ptr = &ptr->dev_addr;
	unsigned int* num_tx_queues_ptr = &ptr->num_tx_queues;
	struct netdev_queue** _tx_ptr = &ptr->_tx;
	
	{
		*dev_addr_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned char*, (sizeof(unsigned char) * glue_peek(__pos, msg, ext)), (DEFAULT_GFP_FLAGS));
		if (*dev_addr_ptr) {
			int i;
			unsigned char* array = *dev_addr_ptr;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, unsigned char);
			}

		}

	}

	{
		*num_tx_queues_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*_tx_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct netdev_queue*, (ptr->num_tx_queues), (DEFAULT_GFP_FLAGS));
		if (*_tx_ptr) {
			caller_unmarshal_kernel__alloc_etherdev_mqs__net_device__tx__out(__pos, msg, ext, ctx, *_tx_ptr);
		}

	}

}

void caller_marshal_kernel__alloc_etherdev_mqs__net_device__tx__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* ctx,
	struct netdev_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__alloc_etherdev_mqs__net_device__tx__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* ctx,
	struct netdev_queue* ptr)
{
	
}

void callee_marshal_kernel__alloc_etherdev_mqs__net_device__tx__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* ctx,
	struct netdev_queue const* ptr)
{
	
}

void caller_unmarshal_kernel__alloc_etherdev_mqs__net_device__tx__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_etherdev_mqs_call_ctx const* ctx,
	struct netdev_queue* ptr)
{
	
}

void caller_marshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct free_netdev_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct free_netdev_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct free_netdev_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__free_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct free_netdev_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_napi_del_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	
	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

}

void callee_unmarshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_napi_del_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	
	{
		*state_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

}

void callee_marshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_napi_del_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__netif_napi_del__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_napi_del_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	
}

void caller_marshal_kernel__net_device_ops_ndo_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_validate_addr_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;
	
	{
		__maybe_unused const void* __adjusted = *dev_addr_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_addr_ptr) {
			size_t i, len = 6;
			unsigned char const* array = *dev_addr_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

}

void callee_unmarshal_kernel__net_device_ops_ndo_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_validate_addr_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char** dev_addr_ptr = &ptr->dev_addr;
	
	{
		*dev_addr_ptr = glue_unpack_shadow(__pos, msg, ext, unsigned char*);
		if (*dev_addr_ptr) {
			int i;
			unsigned char* array = *dev_addr_ptr;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, unsigned char);
			}

		}

	}

}

void callee_marshal_kernel__net_device_ops_ndo_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops_ndo_validate_addr_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;
	
	{
		(void)dev_addr_ptr;
	}

}

void caller_unmarshal_kernel__net_device_ops_ndo_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops_ndo_validate_addr_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned char** dev_addr_ptr = &ptr->dev_addr;
	
	{
		(void)dev_addr_ptr;
	}

}

void caller_marshal_kernel__napi_disable__n__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_disable_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	
	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

}

void callee_unmarshal_kernel__napi_disable__n__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_disable_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	
	{
		*state_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

}

void callee_marshal_kernel__napi_disable__n__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_disable_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__napi_disable__n__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_disable_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	
}

void caller_marshal_kernel__poll__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct poll_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	
}

void callee_unmarshal_kernel__poll__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct poll_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	
}

void callee_marshal_kernel__poll__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct poll_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__poll__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct poll_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	
}

void caller_marshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__netif_napi_add__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	
	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

}

void callee_unmarshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	
	{
		*state_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

}

void callee_marshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__netif_napi_add__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_napi_add_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	
}

void caller_marshal_kernel____napi_schedule__n__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __napi_schedule_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	
}

void callee_unmarshal_kernel____napi_schedule__n__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __napi_schedule_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	
}

void callee_marshal_kernel____napi_schedule__n__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __napi_schedule_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	
}

void caller_unmarshal_kernel____napi_schedule__n__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __napi_schedule_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	
}

void caller_marshal_kernel____dev_kfree_skb_any__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __dev_kfree_skb_any_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* end_ptr = &ptr->end;
	unsigned int const* len_ptr = &ptr->len;
	unsigned char* const* head_ptr = &ptr->head;
	
	{
		glue_pack(__pos, msg, ext, *end_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *len_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*head_ptr) {
			size_t i, len = (ptr->len);
			unsigned char* array = *head_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

}

void callee_unmarshal_kernel____dev_kfree_skb_any__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __dev_kfree_skb_any_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* end_ptr = &ptr->end;
	unsigned int* len_ptr = &ptr->len;
	unsigned char** head_ptr = &ptr->head;
	
	{
		*end_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*len_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack(__pos, msg, ext, unsigned char*);
		if (*head_ptr) {
			int i;
			unsigned char* array = *head_ptr;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, unsigned char);
			}

		}

	}

}

void callee_marshal_kernel____dev_kfree_skb_any__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
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
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __dev_kfree_skb_any_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned char** head_ptr = &ptr->head;
	
	{
		glue_remove_shadow(*head_ptr);
		(void)head_ptr;
	}

}

void caller_marshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_device_attach_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int const* num_tx_queues_ptr = &ptr->num_tx_queues;
	int const* watchdog_timeo_ptr = &ptr->watchdog_timeo;
	
	{
		glue_pack(__pos, msg, ext, *num_tx_queues_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *watchdog_timeo_ptr);
	}

}

void callee_unmarshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_device_attach_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int* num_tx_queues_ptr = &ptr->num_tx_queues;
	int* watchdog_timeo_ptr = &ptr->watchdog_timeo;
	
	{
		*num_tx_queues_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*watchdog_timeo_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void callee_marshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_device_attach_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	int const* watchdog_timeo_ptr = &ptr->watchdog_timeo;
	
	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *watchdog_timeo_ptr);
	}

}

void caller_unmarshal_kernel__netif_device_attach__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_device_attach_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	int* watchdog_timeo_ptr = &ptr->watchdog_timeo;
	
	{
		*state_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		*watchdog_timeo_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void caller_marshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_tx_wake_queue_call_ctx const* ctx,
	struct netdev_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_tx_wake_queue_call_ctx const* ctx,
	struct netdev_queue* ptr)
{
	
}

void callee_marshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_tx_wake_queue_call_ctx const* ctx,
	struct netdev_queue const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	
	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

}

void caller_unmarshal_kernel__netif_tx_wake_queue__dev_queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_tx_wake_queue_call_ctx const* ctx,
	struct netdev_queue* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	
	{
		*state_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

}

void caller_marshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_device_detach_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned int const* num_tx_queues_ptr = &ptr->num_tx_queues;
	
	{
		glue_pack(__pos, msg, ext, *num_tx_queues_ptr);
	}

}

void callee_unmarshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_device_detach_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned int* num_tx_queues_ptr = &ptr->num_tx_queues;
	
	{
		*num_tx_queues_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void callee_marshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_device_detach_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	
	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

}

void caller_unmarshal_kernel__netif_device_detach__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_device_detach_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	
	{
		*state_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

}

void caller_marshal_kernel__netif_schedule_queue__txq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_schedule_queue_call_ctx const* ctx,
	struct netdev_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__netif_schedule_queue__txq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_schedule_queue_call_ctx const* ctx,
	struct netdev_queue* ptr)
{
	
}

void callee_marshal_kernel__netif_schedule_queue__txq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_schedule_queue_call_ctx const* ctx,
	struct netdev_queue const* ptr)
{
	
}

void caller_unmarshal_kernel__netif_schedule_queue__txq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_schedule_queue_call_ctx const* ctx,
	struct netdev_queue* ptr)
{
	
}

void caller_marshal_kernel__unregister_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct unregister_netdev_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__unregister_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct unregister_netdev_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__unregister_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct unregister_netdev_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__unregister_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct unregister_netdev_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct register_netdev_call_ctx const* ctx,
	struct net_device const* ptr)
{
	struct net_device_ops* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned char const* perm_addr_ptr = ptr->perm_addr;
	unsigned char const* addr_assign_type_ptr = &ptr->addr_assign_type;
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;
	
	{
		__maybe_unused const void* __adjusted = *netdev_ops_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*netdev_ops_ptr) {
			caller_marshal_kernel____global_netdev_ops__in(__pos, msg, ext, *netdev_ops_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ethtool_ops_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ethtool_ops_ptr) {
			caller_marshal_kernel____global_ethtool_ops__in(__pos, msg, ext, *ethtool_ops_ptr);
		}

	}

	{
		size_t i, len = 32;
		unsigned char const* array = perm_addr_ptr;
		glue_pack(__pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char const* element = &array[i];
			glue_pack(__pos, msg, ext, *element);
		}

	}

	{
		glue_pack(__pos, msg, ext, *addr_assign_type_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *dev_addr_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_addr_ptr) {
			size_t i, len = (MAX_ADDR_LEN);
			unsigned char* array = *dev_addr_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

}

void callee_unmarshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct register_netdev_call_ctx const* ctx,
	struct net_device* ptr)
{
	struct net_device_ops** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops** ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned char* perm_addr_ptr = ptr->perm_addr;
	unsigned char* addr_assign_type_ptr = &ptr->addr_assign_type;
	unsigned char** dev_addr_ptr = &ptr->dev_addr;
	
	{
		*netdev_ops_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct net_device_ops*, (sizeof(struct net_device_ops)), (DEFAULT_GFP_FLAGS));
		if (*netdev_ops_ptr) {
			callee_unmarshal_kernel____global_netdev_ops__in(__pos, msg, ext, *netdev_ops_ptr);
		}

	}

	{
		*ethtool_ops_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct ethtool_ops*, (sizeof(struct ethtool_ops)), (DEFAULT_GFP_FLAGS));
		if (*ethtool_ops_ptr) {
			callee_unmarshal_kernel____global_ethtool_ops__in(__pos, msg, ext, *ethtool_ops_ptr);
		}

	}

	{
		int i;
		unsigned char* array = perm_addr_ptr;
		size_t len = glue_unpack(__pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned char* element = &array[i];
			*element = glue_unpack(__pos, msg, ext, unsigned char);
		}

	}

	{
		*addr_assign_type_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*dev_addr_ptr = glue_unpack(__pos, msg, ext, unsigned char*);
		if (*dev_addr_ptr) {
			int i;
			unsigned char* array = *dev_addr_ptr;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				*element = glue_unpack(__pos, msg, ext, unsigned char);
			}

		}

	}

}

void callee_marshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct register_netdev_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	unsigned long long const* hw_features_ptr = &ptr->hw_features;
	struct net_device_ops* const* netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops* const* ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int const* mtu_ptr = &ptr->mtu;
	unsigned char const* addr_len_ptr = &ptr->addr_len;
	unsigned char* const* dev_addr_ptr = &ptr->dev_addr;
	unsigned int const* num_tx_queues_ptr = &ptr->num_tx_queues;
	
	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *hw_features_ptr);
	}

	{
		if (*netdev_ops_ptr) {
			callee_marshal_kernel____global_netdev_ops__in(__pos, msg, ext, *netdev_ops_ptr);
		}

	}

	{
		if (*ethtool_ops_ptr) {
			callee_marshal_kernel____global_ethtool_ops__in(__pos, msg, ext, *ethtool_ops_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *mtu_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *addr_len_ptr);
	}

	{
		(void)dev_addr_ptr;
	}

	{
		glue_pack(__pos, msg, ext, *num_tx_queues_ptr);
	}

}

void caller_unmarshal_kernel__register_netdev__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct register_netdev_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	unsigned long long* hw_features_ptr = &ptr->hw_features;
	struct net_device_ops** netdev_ops_ptr = &ptr->netdev_ops;
	struct ethtool_ops** ethtool_ops_ptr = &ptr->ethtool_ops;
	unsigned int* mtu_ptr = &ptr->mtu;
	unsigned char* addr_len_ptr = &ptr->addr_len;
	unsigned char** dev_addr_ptr = &ptr->dev_addr;
	unsigned int* num_tx_queues_ptr = &ptr->num_tx_queues;
	
	{
		*state_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		*hw_features_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

	{
		if (*netdev_ops_ptr) {
			caller_unmarshal_kernel____global_netdev_ops__in(__pos, msg, ext, *netdev_ops_ptr);
		}

	}

	{
		if (*ethtool_ops_ptr) {
			caller_unmarshal_kernel____global_ethtool_ops__in(__pos, msg, ext, *ethtool_ops_ptr);
		}

	}

	{
		*mtu_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*addr_len_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		(void)dev_addr_ptr;
	}

	{
		*num_tx_queues_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void caller_marshal_kernel____global_netdev_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops const* ptr)
{
	fptr_net_device_ops_ndo_change_mtu const* ndo_change_mtu_ptr = &ptr->ndo_change_mtu;
	fptr_net_device_ops_ndo_do_ioctl const* ndo_do_ioctl_ptr = &ptr->ndo_do_ioctl;
	fptr_net_device_ops_ndo_fix_features const* ndo_fix_features_ptr = &ptr->ndo_fix_features;
	fptr_net_device_ops_ndo_get_stats64 const* ndo_get_stats64_ptr = &ptr->ndo_get_stats64;
	fptr_net_device_ops_ndo_open const* ndo_open_ptr = &ptr->ndo_open;
	fptr_net_device_ops_ndo_set_mac_address const* ndo_set_mac_address_ptr = &ptr->ndo_set_mac_address;
	fptr_net_device_ops_ndo_set_rx_mode const* ndo_set_rx_mode_ptr = &ptr->ndo_set_rx_mode;
	fptr_net_device_ops_ndo_start_xmit const* ndo_start_xmit_ptr = &ptr->ndo_start_xmit;
	fptr_net_device_ops_ndo_stop const* ndo_stop_ptr = &ptr->ndo_stop;
	fptr_net_device_ops_ndo_tx_timeout const* ndo_tx_timeout_ptr = &ptr->ndo_tx_timeout;
	fptr_net_device_ops_ndo_validate_addr const* ndo_validate_addr_ptr = &ptr->ndo_validate_addr;
	
	{
		glue_pack(__pos, msg, ext, *ndo_change_mtu_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *ndo_do_ioctl_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *ndo_fix_features_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *ndo_get_stats64_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *ndo_open_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *ndo_set_mac_address_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *ndo_set_rx_mode_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *ndo_start_xmit_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *ndo_stop_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *ndo_tx_timeout_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *ndo_validate_addr_ptr);
	}

}

void callee_unmarshal_kernel____global_netdev_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops* ptr)
{
	fptr_net_device_ops_ndo_change_mtu* ndo_change_mtu_ptr = &ptr->ndo_change_mtu;
	fptr_net_device_ops_ndo_do_ioctl* ndo_do_ioctl_ptr = &ptr->ndo_do_ioctl;
	fptr_net_device_ops_ndo_fix_features* ndo_fix_features_ptr = &ptr->ndo_fix_features;
	fptr_net_device_ops_ndo_get_stats64* ndo_get_stats64_ptr = &ptr->ndo_get_stats64;
	fptr_net_device_ops_ndo_open* ndo_open_ptr = &ptr->ndo_open;
	fptr_net_device_ops_ndo_set_mac_address* ndo_set_mac_address_ptr = &ptr->ndo_set_mac_address;
	fptr_net_device_ops_ndo_set_rx_mode* ndo_set_rx_mode_ptr = &ptr->ndo_set_rx_mode;
	fptr_net_device_ops_ndo_start_xmit* ndo_start_xmit_ptr = &ptr->ndo_start_xmit;
	fptr_net_device_ops_ndo_stop* ndo_stop_ptr = &ptr->ndo_stop;
	fptr_net_device_ops_ndo_tx_timeout* ndo_tx_timeout_ptr = &ptr->ndo_tx_timeout;
	fptr_net_device_ops_ndo_validate_addr* ndo_validate_addr_ptr = &ptr->ndo_validate_addr;
	
	{
		*ndo_change_mtu_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, net_device_ops_ndo_change_mtu);
	}

	{
		*ndo_do_ioctl_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, net_device_ops_ndo_do_ioctl);
	}

	{
		*ndo_fix_features_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, net_device_ops_ndo_fix_features);
	}

	{
		*ndo_get_stats64_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, net_device_ops_ndo_get_stats64);
	}

	{
		*ndo_open_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, net_device_ops_ndo_open);
	}

	{
		*ndo_set_mac_address_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, net_device_ops_ndo_set_mac_address);
	}

	{
		*ndo_set_rx_mode_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, net_device_ops_ndo_set_rx_mode);
	}

	{
		*ndo_start_xmit_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, net_device_ops_ndo_start_xmit);
	}

	{
		*ndo_stop_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, net_device_ops_ndo_stop);
	}

	{
		*ndo_tx_timeout_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, net_device_ops_ndo_tx_timeout);
	}

	{
		*ndo_validate_addr_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, net_device_ops_ndo_validate_addr);
	}

}

void callee_marshal_kernel____global_netdev_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct net_device_ops const* ptr)
{
	
}

void caller_unmarshal_kernel____global_netdev_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct net_device_ops* ptr)
{
	
}

void caller_marshal_kernel____global_ethtool_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops const* ptr)
{
	fptr_ethtool_ops_get_ethtool_stats const* get_ethtool_stats_ptr = &ptr->get_ethtool_stats;
	fptr_ethtool_ops_get_link const* get_link_ptr = &ptr->get_link;
	fptr_ethtool_ops_get_msglevel const* get_msglevel_ptr = &ptr->get_msglevel;
	fptr_ethtool_ops_get_pauseparam const* get_pauseparam_ptr = &ptr->get_pauseparam;
	fptr_ethtool_ops_get_settings const* get_settings_ptr = &ptr->get_settings;
	fptr_ethtool_ops_get_sset_count const* get_sset_count_ptr = &ptr->get_sset_count;
	fptr_ethtool_ops_get_strings const* get_strings_ptr = &ptr->get_strings;
	fptr_ethtool_ops_set_msglevel const* set_msglevel_ptr = &ptr->set_msglevel;
	fptr_ethtool_ops_set_pauseparam const* set_pauseparam_ptr = &ptr->set_pauseparam;
	fptr_ethtool_ops_set_settings const* set_settings_ptr = &ptr->set_settings;
	
	{
		glue_pack(__pos, msg, ext, *get_ethtool_stats_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *get_link_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *get_msglevel_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *get_pauseparam_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *get_settings_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *get_sset_count_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *get_strings_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *set_msglevel_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *set_pauseparam_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *set_settings_ptr);
	}

}

void callee_unmarshal_kernel____global_ethtool_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops* ptr)
{
	fptr_ethtool_ops_get_ethtool_stats* get_ethtool_stats_ptr = &ptr->get_ethtool_stats;
	fptr_ethtool_ops_get_link* get_link_ptr = &ptr->get_link;
	fptr_ethtool_ops_get_msglevel* get_msglevel_ptr = &ptr->get_msglevel;
	fptr_ethtool_ops_get_pauseparam* get_pauseparam_ptr = &ptr->get_pauseparam;
	fptr_ethtool_ops_get_settings* get_settings_ptr = &ptr->get_settings;
	fptr_ethtool_ops_get_sset_count* get_sset_count_ptr = &ptr->get_sset_count;
	fptr_ethtool_ops_get_strings* get_strings_ptr = &ptr->get_strings;
	fptr_ethtool_ops_set_msglevel* set_msglevel_ptr = &ptr->set_msglevel;
	fptr_ethtool_ops_set_pauseparam* set_pauseparam_ptr = &ptr->set_pauseparam;
	fptr_ethtool_ops_set_settings* set_settings_ptr = &ptr->set_settings;
	
	{
		*get_ethtool_stats_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, ethtool_ops_get_ethtool_stats);
	}

	{
		*get_link_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, ethtool_ops_get_link);
	}

	{
		*get_msglevel_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, ethtool_ops_get_msglevel);
	}

	{
		*get_pauseparam_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, ethtool_ops_get_pauseparam);
	}

	{
		*get_settings_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, ethtool_ops_get_settings);
	}

	{
		*get_sset_count_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, ethtool_ops_get_sset_count);
	}

	{
		*get_strings_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, ethtool_ops_get_strings);
	}

	{
		*set_msglevel_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, ethtool_ops_set_msglevel);
	}

	{
		*set_pauseparam_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, ethtool_ops_set_pauseparam);
	}

	{
		*set_settings_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, ethtool_ops_set_settings);
	}

}

void callee_marshal_kernel____global_ethtool_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_ops const* ptr)
{
	
}

void caller_unmarshal_kernel____global_ethtool_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_ops* ptr)
{
	
}

void caller_marshal_kernel__netdev_update_features__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netdev_update_features_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__netdev_update_features__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netdev_update_features_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__netdev_update_features__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netdev_update_features_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__netdev_update_features__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netdev_update_features_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_carrier_on_call_ctx const* ctx,
	struct net_device const* ptr)
{
	int const* watchdog_timeo_ptr = &ptr->watchdog_timeo;
	
	{
		glue_pack(__pos, msg, ext, *watchdog_timeo_ptr);
	}

}

void callee_unmarshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_carrier_on_call_ctx const* ctx,
	struct net_device* ptr)
{
	int* watchdog_timeo_ptr = &ptr->watchdog_timeo;
	
	{
		*watchdog_timeo_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void callee_marshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_carrier_on_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	int const* watchdog_timeo_ptr = &ptr->watchdog_timeo;
	
	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *watchdog_timeo_ptr);
	}

}

void caller_unmarshal_kernel__netif_carrier_on__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_carrier_on_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	int* watchdog_timeo_ptr = &ptr->watchdog_timeo;
	
	{
		*state_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		*watchdog_timeo_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void caller_marshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_carrier_off_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_carrier_off_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct netif_carrier_off_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned long const* state_ptr = &ptr->state;
	
	{
		glue_pack(__pos, msg, ext, *state_ptr);
	}

}

void caller_unmarshal_kernel__netif_carrier_off__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct netif_carrier_off_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned long* state_ptr = &ptr->state;
	
	{
		*state_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

}

void caller_marshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	
}

void callee_unmarshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	
}

void callee_marshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct napi_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__napi_gro_receive__napi__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct napi_struct* ptr)
{
	
}

void caller_marshal_kernel__napi_gro_receive__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned char __ip_summed = ptr->ip_summed;
	unsigned char const* __ip_summed_ptr = &__ip_summed;
	unsigned short const* protocol_ptr = &ptr->protocol;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;
	
	{
		glue_pack(__pos, msg, ext, *len_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *__ip_summed_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *protocol_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, msg, ext, *element);
		}

	}

	{
		glue_pack(__pos, msg, ext, *tail_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, msg, ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, msg, ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__napi_gro_receive__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned char __ip_summed = ptr->ip_summed;
	unsigned char* __ip_summed_ptr = &__ip_summed;
	unsigned short* protocol_ptr = &ptr->protocol;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;
	
	{
		*len_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*__ip_summed_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*protocol_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, msg, ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, msg, ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, msg, ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, msg, ext, unsigned char);
		}

	}

	{
		ptr->ip_summed = *__ip_summed_ptr;
	}
}

void callee_marshal_kernel__napi_gro_receive__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* data_len_ptr = &ptr->data_len;
	unsigned char __ip_summed = ptr->ip_summed;
	unsigned char const* __ip_summed_ptr = &__ip_summed;
	unsigned short const* protocol_ptr = &ptr->protocol;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;
	
	{
		glue_pack(__pos, msg, ext, *len_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *data_len_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *__ip_summed_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *protocol_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, msg, ext, *element);
		}

	}

	{
		glue_pack(__pos, msg, ext, *tail_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, msg, ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, msg, ext, **data_ptr);
		}

	}

}

void caller_unmarshal_kernel__napi_gro_receive__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct napi_gro_receive_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* len_ptr = &ptr->len;
	unsigned int* data_len_ptr = &ptr->data_len;
	unsigned char __ip_summed = ptr->ip_summed;
	unsigned char* __ip_summed_ptr = &__ip_summed;
	unsigned short* protocol_ptr = &ptr->protocol;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;
	
	{
		*len_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*__ip_summed_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*protocol_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, msg, ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, msg, ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, msg, ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, msg, ext, unsigned char);
		}

	}

	{
		ptr->ip_summed = *__ip_summed_ptr;
	}
}

void caller_marshal_kernel__skb_put__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
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
		glue_pack(__pos, msg, ext, *len_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *data_len_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, msg, ext, *element);
		}

	}

	{
		glue_pack(__pos, msg, ext, *tail_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, msg, ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, msg, ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__skb_put__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
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
		*len_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*data_len_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack(__pos, msg, ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, msg, ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack(__pos, msg, ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, msg, ext, unsigned char);
		}

	}

}

void callee_marshal_kernel__skb_put__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_put_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;
	
	{
		glue_pack(__pos, msg, ext, *len_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, msg, ext, *element);
		}

	}

	{
		glue_pack(__pos, msg, ext, *tail_ptr);
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
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_put_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* len_ptr = &ptr->len;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;
	
	{
		*len_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		(void)head_ptr;
	}

	{
		(void)data_ptr;
	}

}

void caller_marshal_kernel____netdev_alloc_skb__ret_sk_buff__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void callee_unmarshal_kernel____netdev_alloc_skb__ret_sk_buff__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void callee_marshal_kernel____netdev_alloc_skb__ret_sk_buff__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void caller_unmarshal_kernel____netdev_alloc_skb__ret_sk_buff__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void caller_marshal_kernel____netdev_alloc_skb__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel____netdev_alloc_skb__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel____netdev_alloc_skb__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel____netdev_alloc_skb__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __netdev_alloc_skb_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_select_bars_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_select_bars_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_select_bars_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_select_bars_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__pcie_set_readrq__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pcie_set_readrq_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pcie_set_readrq__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pcie_set_readrq_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pcie_set_readrq__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pcie_set_readrq_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pcie_set_readrq__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pcie_set_readrq_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__pcie_get_readrq__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pcie_get_readrq_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pcie_get_readrq__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pcie_get_readrq_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pcie_get_readrq__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pcie_get_readrq_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pcie_get_readrq__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pcie_get_readrq_call_ctx const* ctx,
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

void caller_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_request_selected_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_request_selected_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_request_selected_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_request_selected_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_release_selected_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_release_selected_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_release_selected_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_release_selected_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
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
	unsigned char const* pm_cap_ptr = &ptr->pm_cap;
	unsigned short const* dev_flags_ptr = &ptr->dev_flags;
	
	{
		glue_pack(__pos, msg, ext, *pm_cap_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *dev_flags_ptr);
	}

}

void caller_unmarshal_kernel__pci_enable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned char* pm_cap_ptr = &ptr->pm_cap;
	unsigned short* dev_flags_ptr = &ptr->dev_flags;
	
	{
		*pm_cap_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*dev_flags_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

}

void caller_marshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned char const* pm_cap_ptr = &ptr->pm_cap;
	unsigned short const* dev_flags_ptr = &ptr->dev_flags;
	
	{
		glue_pack(__pos, msg, ext, *pm_cap_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *dev_flags_ptr);
	}

}

void caller_unmarshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned char* pm_cap_ptr = &ptr->pm_cap;
	unsigned short* dev_flags_ptr = &ptr->dev_flags;
	
	{
		*pm_cap_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

	{
		*dev_flags_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

}

void caller_marshal_kernel__pci_ioremap_bar__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_ioremap_bar_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_ioremap_bar__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_ioremap_bar_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_ioremap_bar__pdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_ioremap_bar_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_ioremap_bar__pdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_ioremap_bar_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct eth_validate_addr_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct eth_validate_addr_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct eth_validate_addr_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__eth_validate_addr__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct eth_validate_addr_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_op_get_link_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_op_get_link_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct ethtool_op_get_link_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__ethtool_op_get_link__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct ethtool_op_get_link_call_ctx const* ctx,
	struct net_device* ptr)
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

