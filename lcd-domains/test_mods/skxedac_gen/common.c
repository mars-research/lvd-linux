#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel__notifier_call__nb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct notifier_call_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	fptr_notifier_call const* notifier_call_ptr = &ptr->notifier_call;
	
	{
		glue_pack(__pos, msg, ext, *notifier_call_ptr);
	}

}

void callee_unmarshal_kernel__notifier_call__nb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct notifier_call_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	fptr_notifier_call* notifier_call_ptr = &ptr->notifier_call;
	
	{
		*notifier_call_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, notifier_call);
	}

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

void caller_marshal_kernel__pci_dev_get__ret_pci_dev__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_dev_get__ret_pci_dev__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_dev_get__ret_pci_dev__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_dev_get__ret_pci_dev__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__pci_dev_get__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_dev_get__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_dev_get__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_dev_get__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_get_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_put_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_put_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_dev_put_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_dev_put__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_dev_put_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void caller_marshal_kernel__mce_unregister_decode_chain__notifier_block__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mce_unregister_decode_chain_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	
}

void callee_unmarshal_kernel__mce_unregister_decode_chain__notifier_block__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mce_unregister_decode_chain_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	
}

void callee_marshal_kernel__mce_unregister_decode_chain__notifier_block__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mce_unregister_decode_chain_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	
}

void caller_unmarshal_kernel__mce_unregister_decode_chain__notifier_block__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mce_unregister_decode_chain_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	
}

void caller_marshal_kernel__mce_register_decode_chain__notifier_block__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mce_register_decode_chain_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	
}

void callee_unmarshal_kernel__mce_register_decode_chain__notifier_block__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mce_register_decode_chain_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	
}

void callee_marshal_kernel__mce_register_decode_chain__notifier_block__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct mce_register_decode_chain_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	
}

void caller_unmarshal_kernel__mce_register_decode_chain__notifier_block__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct mce_register_decode_chain_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	
}

void caller_marshal_kernel__edac_mc_handle_error__mci__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* ctx,
	struct mem_ctl_info const* ptr)
{
	int const* mc_idx_ptr = &ptr->mc_idx;
	unsigned int const* n_layers_ptr = &ptr->n_layers;
	struct edac_mc_layer* const* layers_ptr = &ptr->layers;
	struct dimm_info* const* dimms_ptr = &ptr->dimms;
	
	{
		glue_pack(__pos, msg, ext, *mc_idx_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *n_layers_ptr);
	}

	{
		const void* __adjusted = *layers_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*layers_ptr) {
			caller_marshal_kernel__edac_mc_handle_error__layers__in(__pos, msg, ext, ctx, *layers_ptr);
		}

	}

	{
		const void* __adjusted = *dimms_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dimms_ptr) {
			caller_marshal_kernel__edac_mc_handle_error__dimms__in(__pos, msg, ext, ctx, *dimms_ptr);
		}

	}

}

void callee_unmarshal_kernel__edac_mc_handle_error__mci__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* ctx,
	struct mem_ctl_info* ptr)
{
	int* mc_idx_ptr = &ptr->mc_idx;
	unsigned int* n_layers_ptr = &ptr->n_layers;
	struct edac_mc_layer** layers_ptr = &ptr->layers;
	struct dimm_info** dimms_ptr = &ptr->dimms;
	
	{
		*mc_idx_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*n_layers_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*layers_ptr = glue_unpack(__pos, msg, ext, struct edac_mc_layer*);
		if (*layers_ptr) {
			callee_unmarshal_kernel__edac_mc_handle_error__layers__in(__pos, msg, ext, ctx, *layers_ptr);
		}

	}

	{
		*dimms_ptr = glue_unpack(__pos, msg, ext, struct dimm_info*);
		if (*dimms_ptr) {
			callee_unmarshal_kernel__edac_mc_handle_error__dimms__in(__pos, msg, ext, ctx, *dimms_ptr);
		}

	}

}

void callee_marshal_kernel__edac_mc_handle_error__mci__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* ctx,
	struct mem_ctl_info const* ptr)
{
	struct edac_mc_layer* const* layers_ptr = &ptr->layers;
	struct dimm_info* const* dimms_ptr = &ptr->dimms;
	
	{
		if (*layers_ptr) {
			callee_marshal_kernel__edac_mc_handle_error__layers__in(__pos, msg, ext, ctx, *layers_ptr);
		}

	}

	{
		if (*dimms_ptr) {
			callee_marshal_kernel__edac_mc_handle_error__dimms__in(__pos, msg, ext, ctx, *dimms_ptr);
		}

	}

}

void caller_unmarshal_kernel__edac_mc_handle_error__mci__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* ctx,
	struct mem_ctl_info* ptr)
{
	struct edac_mc_layer** layers_ptr = &ptr->layers;
	struct dimm_info** dimms_ptr = &ptr->dimms;
	
	{
		if (*layers_ptr) {
			caller_unmarshal_kernel__edac_mc_handle_error__layers__in(__pos, msg, ext, ctx, *layers_ptr);
		}

	}

	{
		if (*dimms_ptr) {
			caller_unmarshal_kernel__edac_mc_handle_error__dimms__in(__pos, msg, ext, ctx, *dimms_ptr);
		}

	}

}

void caller_marshal_kernel__edac_mc_handle_error__layers__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* ctx,
	struct edac_mc_layer const* ptr)
{
	
}

void callee_unmarshal_kernel__edac_mc_handle_error__layers__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* ctx,
	struct edac_mc_layer* ptr)
{
	
}

void callee_marshal_kernel__edac_mc_handle_error__layers__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* ctx,
	struct edac_mc_layer const* ptr)
{
	
}

void caller_unmarshal_kernel__edac_mc_handle_error__layers__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* ctx,
	struct edac_mc_layer* ptr)
{
	
}

void caller_marshal_kernel__edac_mc_handle_error__dimms__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* ctx,
	struct dimm_info const* ptr)
{
	
}

void callee_unmarshal_kernel__edac_mc_handle_error__dimms__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* ctx,
	struct dimm_info* ptr)
{
	
}

void callee_marshal_kernel__edac_mc_handle_error__dimms__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* ctx,
	struct dimm_info const* ptr)
{
	
}

void caller_unmarshal_kernel__edac_mc_handle_error__dimms__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_handle_error_call_ctx const* ctx,
	struct dimm_info* ptr)
{
	
}

void caller_marshal_kernel__edac_mc_del_mc__ret_mem_ctl_info__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* ctx,
	struct mem_ctl_info const* ptr)
{
	
}

void callee_unmarshal_kernel__edac_mc_del_mc__ret_mem_ctl_info__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* ctx,
	struct mem_ctl_info* ptr)
{
	
}

void callee_marshal_kernel__edac_mc_del_mc__ret_mem_ctl_info__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* ctx,
	struct mem_ctl_info const* ptr)
{
	int const* mc_idx_ptr = &ptr->mc_idx;
	
	{
		glue_pack(__pos, msg, ext, *mc_idx_ptr);
	}

}

void caller_unmarshal_kernel__edac_mc_del_mc__ret_mem_ctl_info__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* ctx,
	struct mem_ctl_info* ptr)
{
	int* mc_idx_ptr = &ptr->mc_idx;
	
	{
		*mc_idx_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void caller_marshal_kernel__edac_mc_del_mc__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void callee_unmarshal_kernel__edac_mc_del_mc__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void callee_marshal_kernel__edac_mc_del_mc__dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void caller_unmarshal_kernel__edac_mc_del_mc__dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_del_mc_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void caller_marshal_kernel__edac_mc_add_mc_with_groups__mci__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct mem_ctl_info const* ptr)
{
	int const* mc_idx_ptr = &ptr->mc_idx;
	unsigned int const* tot_dimms_ptr = &ptr->tot_dimms;
	struct dimm_info** const* dimms_ptr = &ptr->dimms;
	char const* const* ctl_name_ptr = &ptr->ctl_name;
	unsigned long const* mtype_cap_ptr = &ptr->mtype_cap;
	unsigned long const* edac_ctl_cap_ptr = &ptr->edac_ctl_cap;
	unsigned long const* edac_cap_ptr = &ptr->edac_cap;
	char const* const* mod_name_ptr = &ptr->mod_name;
	char const* const* dev_name_ptr = &ptr->dev_name;
	char const* const* mod_ver_ptr = &ptr->mod_ver;
	struct device* const* pdev_ptr = &ptr->pdev;
	
	{
		glue_pack(__pos, msg, ext, *mc_idx_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tot_dimms_ptr);
	}

	{
		const void* __adjusted = *dimms_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dimms_ptr) {
			const void* __adjusted = **dimms_ptr;
			glue_pack_shadow(__pos, msg, ext, __adjusted);
			if (**dimms_ptr) {
				size_t i, len = (ctx->mci->tot_dimms);
				struct dimm_info* array = **dimms_ptr;
				glue_pack(__pos, msg, ext, len);
				// Warning: see David if this breaks
				glue_user_trace("Warning: see David if this breaks");
				for (i = 0; i < len; ++i) {
					struct dimm_info* element = &array[i];
					caller_marshal_kernel__edac_mc_add_mc_with_groups__dimms__in(__pos, msg, ext, ctx, element);
				}

			}

		}

	}

	{
		const void* __adjusted = *ctl_name_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ctl_name_ptr) {
			size_t i, len;
			char const* array = *ctl_name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	{
		glue_pack(__pos, msg, ext, *mtype_cap_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *edac_ctl_cap_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *edac_cap_ptr);
	}

	{
		const void* __adjusted = *mod_name_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*mod_name_ptr) {
			size_t i, len;
			char const* array = *mod_name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	{
		const void* __adjusted = *dev_name_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_name_ptr) {
			size_t i, len;
			char const* array = *dev_name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	{
		const void* __adjusted = *mod_ver_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*mod_ver_ptr) {
			size_t i, len;
			char const* array = *mod_ver_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	{
		const void* __adjusted = *pdev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*pdev_ptr) {
			caller_marshal_kernel__edac_mc_add_mc_with_groups__device__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

}

void callee_unmarshal_kernel__edac_mc_add_mc_with_groups__mci__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct mem_ctl_info* ptr)
{
	int* mc_idx_ptr = &ptr->mc_idx;
	unsigned int* tot_dimms_ptr = &ptr->tot_dimms;
	struct dimm_info*** dimms_ptr = &ptr->dimms;
	char const** ctl_name_ptr = &ptr->ctl_name;
	unsigned long* mtype_cap_ptr = &ptr->mtype_cap;
	unsigned long* edac_ctl_cap_ptr = &ptr->edac_ctl_cap;
	unsigned long* edac_cap_ptr = &ptr->edac_cap;
	char const** mod_name_ptr = &ptr->mod_name;
	char const** dev_name_ptr = &ptr->dev_name;
	char const** mod_ver_ptr = &ptr->mod_ver;
	struct device** pdev_ptr = &ptr->pdev;
	
	{
		*mc_idx_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*tot_dimms_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*dimms_ptr = glue_unpack(__pos, msg, ext, struct dimm_info**);
		if (*dimms_ptr) {
			**dimms_ptr = glue_unpack(__pos, msg, ext, struct dimm_info*);
			if (**dimms_ptr) {
				int i;
				struct dimm_info* array = **dimms_ptr;
				size_t len = glue_unpack(__pos, msg, ext, size_t);
				// Warning: see David if this breaks
				glue_user_trace("Warning: see David if this breaks");
				for (i = 0; i < len; ++i) {
					struct dimm_info* element = &array[i];
					callee_unmarshal_kernel__edac_mc_add_mc_with_groups__dimms__in(__pos, msg, ext, ctx, element);
				}

			}

		}

	}

	{
		*ctl_name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, sizeof(char) * glue_peek(__pos, msg, ext));
		if (*ctl_name_ptr) {
			char* writable = (char*)*ctl_name_ptr;
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
		*mtype_cap_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		*edac_ctl_cap_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		*edac_cap_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		*mod_name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, sizeof(char) * glue_peek(__pos, msg, ext));
		if (*mod_name_ptr) {
			char* writable = (char*)*mod_name_ptr;
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
		*dev_name_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, sizeof(char) * glue_peek(__pos, msg, ext));
		if (*dev_name_ptr) {
			char* writable = (char*)*dev_name_ptr;
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
		*mod_ver_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, sizeof(char) * glue_peek(__pos, msg, ext));
		if (*mod_ver_ptr) {
			char* writable = (char*)*mod_ver_ptr;
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
		*pdev_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct device*, sizeof(struct device));
		if (*pdev_ptr) {
			callee_unmarshal_kernel__edac_mc_add_mc_with_groups__device__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

}

void callee_marshal_kernel__edac_mc_add_mc_with_groups__mci__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct mem_ctl_info const* ptr)
{
	struct dimm_info** const* dimms_ptr = &ptr->dimms;
	char const* const* ctl_name_ptr = &ptr->ctl_name;
	char const* const* mod_name_ptr = &ptr->mod_name;
	char const* const* dev_name_ptr = &ptr->dev_name;
	char const* const* mod_ver_ptr = &ptr->mod_ver;
	struct device* const* pdev_ptr = &ptr->pdev;
	
	{
		if (*dimms_ptr) {
			(void)*dimms_ptr;
		}

	}

	{
		(void)ctl_name_ptr;
	}

	{
		(void)mod_name_ptr;
	}

	{
		(void)dev_name_ptr;
	}

	{
		(void)mod_ver_ptr;
	}

	{
		if (*pdev_ptr) {
			callee_marshal_kernel__edac_mc_add_mc_with_groups__device__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

}

void caller_unmarshal_kernel__edac_mc_add_mc_with_groups__mci__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct mem_ctl_info* ptr)
{
	struct dimm_info*** dimms_ptr = &ptr->dimms;
	char const** ctl_name_ptr = &ptr->ctl_name;
	char const** mod_name_ptr = &ptr->mod_name;
	char const** dev_name_ptr = &ptr->dev_name;
	char const** mod_ver_ptr = &ptr->mod_ver;
	struct device** pdev_ptr = &ptr->pdev;
	
	{
		if (*dimms_ptr) {
			(void)*dimms_ptr;
		}

	}

	{
		(void)ctl_name_ptr;
	}

	{
		(void)mod_name_ptr;
	}

	{
		(void)dev_name_ptr;
	}

	{
		(void)mod_ver_ptr;
	}

	{
		if (*pdev_ptr) {
			caller_unmarshal_kernel__edac_mc_add_mc_with_groups__device__in(__pos, msg, ext, ctx, *pdev_ptr);
		}

	}

}

void caller_marshal_kernel__edac_mc_add_mc_with_groups__dimms__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct dimm_info const* ptr)
{
	unsigned int const* grain_ptr = &ptr->grain;
	unsigned int const* dtype_ptr = &ptr->dtype;
	unsigned int const* mtype_ptr = &ptr->mtype;
	unsigned int const* edac_mode_ptr = &ptr->edac_mode;
	unsigned int const* nr_pages_ptr = &ptr->nr_pages;
	char const* label_ptr = ptr->label;
	
	{
		glue_pack(__pos, msg, ext, *grain_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *dtype_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *mtype_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *edac_mode_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *nr_pages_ptr);
	}

	{
		size_t i, len = 32;
		char const* array = label_ptr;
		glue_pack(__pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(__pos, msg, ext, *element);
		}

	}

}

void callee_unmarshal_kernel__edac_mc_add_mc_with_groups__dimms__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct dimm_info* ptr)
{
	unsigned int* grain_ptr = &ptr->grain;
	unsigned int* dtype_ptr = &ptr->dtype;
	unsigned int* mtype_ptr = &ptr->mtype;
	unsigned int* edac_mode_ptr = &ptr->edac_mode;
	unsigned int* nr_pages_ptr = &ptr->nr_pages;
	char* label_ptr = ptr->label;
	
	{
		*grain_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*dtype_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*mtype_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*edac_mode_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*nr_pages_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		int i;
		char* array = label_ptr;
		size_t len = glue_unpack(__pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(__pos, msg, ext, char);
		}

	}

}

void callee_marshal_kernel__edac_mc_add_mc_with_groups__dimms__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct dimm_info const* ptr)
{
	
}

void caller_unmarshal_kernel__edac_mc_add_mc_with_groups__dimms__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct dimm_info* ptr)
{
	
}

void caller_marshal_kernel__edac_mc_add_mc_with_groups__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void callee_unmarshal_kernel__edac_mc_add_mc_with_groups__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void callee_marshal_kernel__edac_mc_add_mc_with_groups__device__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void caller_unmarshal_kernel__edac_mc_add_mc_with_groups__device__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void caller_marshal_kernel__edac_mc_add_mc_with_groups__groups__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct attribute_group const* ptr)
{
	
}

void callee_unmarshal_kernel__edac_mc_add_mc_with_groups__groups__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct attribute_group* ptr)
{
	
}

void callee_marshal_kernel__edac_mc_add_mc_with_groups__groups__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct attribute_group const* ptr)
{
	
}

void caller_unmarshal_kernel__edac_mc_add_mc_with_groups__groups__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_add_mc_with_groups_call_ctx const* ctx,
	struct attribute_group* ptr)
{
	
}

void caller_marshal_kernel__edac_mc_free__mci__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_free_call_ctx const* ctx,
	struct mem_ctl_info const* ptr)
{
	
}

void callee_unmarshal_kernel__edac_mc_free__mci__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_free_call_ctx const* ctx,
	struct mem_ctl_info* ptr)
{
	
}

void callee_marshal_kernel__edac_mc_free__mci__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_free_call_ctx const* ctx,
	struct mem_ctl_info const* ptr)
{
	
}

void caller_unmarshal_kernel__edac_mc_free__mci__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_free_call_ctx const* ctx,
	struct mem_ctl_info* ptr)
{
	
}

void caller_marshal_kernel__edac_mc_alloc__ret_mem_ctl_info__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct mem_ctl_info const* ptr)
{
	struct edac_mc_layer* const* layers_ptr = &ptr->layers;
	struct dimm_info** const* dimms_ptr = &ptr->dimms;
	void* const* pvt_info_ptr = &ptr->pvt_info;
	
	{
		if (*layers_ptr) {
			caller_marshal_kernel__edac_mc_alloc__ret_layers__out(__pos, msg, ext, ctx, *layers_ptr);
		}

	}

	{
		(void)dimms_ptr;
	}

	{
		(void)pvt_info_ptr;
	}

}

void callee_unmarshal_kernel__edac_mc_alloc__ret_mem_ctl_info__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct mem_ctl_info* ptr)
{
	struct edac_mc_layer** layers_ptr = &ptr->layers;
	struct dimm_info*** dimms_ptr = &ptr->dimms;
	void** pvt_info_ptr = &ptr->pvt_info;
	
	{
		if (*layers_ptr) {
			callee_unmarshal_kernel__edac_mc_alloc__ret_layers__out(__pos, msg, ext, ctx, *layers_ptr);
		}

	}

	{
		(void)dimms_ptr;
	}

	{
		(void)pvt_info_ptr;
	}

}

void callee_marshal_kernel__edac_mc_alloc__ret_mem_ctl_info__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct mem_ctl_info const* ptr)
{
	int const* mc_idx_ptr = &ptr->mc_idx;
	unsigned int const* n_layers_ptr = &ptr->n_layers;
	unsigned int const* tot_dimms_ptr = &ptr->tot_dimms;
	struct edac_mc_layer* const* layers_ptr = &ptr->layers;
	struct dimm_info** const* dimms_ptr = &ptr->dimms;
	void* const* pvt_info_ptr = &ptr->pvt_info;
	
	{
		glue_pack(__pos, msg, ext, *mc_idx_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *n_layers_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *tot_dimms_ptr);
	}

	{
		const void* __adjusted = *layers_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*layers_ptr) {
			callee_marshal_kernel__edac_mc_alloc__ret_layers__out(__pos, msg, ext, ctx, *layers_ptr);
		}

	}

	{
		const void* __adjusted = *dimms_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dimms_ptr) {
			size_t i, len = (ptr->tot_dimms);
			struct dimm_info** array = *dimms_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct dimm_info** element = &array[i];
				const void* __adjusted = *element;
				glue_pack(__pos, msg, ext, __adjusted);
				if (*element) {
					size_t i, len = 1;
					struct dimm_info const* array = *element;
					glue_pack(__pos, msg, ext, len);
					// Warning: see David if this breaks
					glue_user_trace("Warning: see David if this breaks");
					for (i = 0; i < len; ++i) {
						struct dimm_info const* element = &array[i];
						callee_marshal_kernel__edac_mc_alloc__ret_dimms__out(__pos, msg, ext, ctx, element);
					}

				}

			}

		}

	}

	{
		const void* __adjusted = *pvt_info_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*pvt_info_ptr) {
		}

	}

}

void caller_unmarshal_kernel__edac_mc_alloc__ret_mem_ctl_info__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct mem_ctl_info* ptr)
{
	int* mc_idx_ptr = &ptr->mc_idx;
	unsigned int* n_layers_ptr = &ptr->n_layers;
	unsigned int* tot_dimms_ptr = &ptr->tot_dimms;
	struct edac_mc_layer** layers_ptr = &ptr->layers;
	struct dimm_info*** dimms_ptr = &ptr->dimms;
	void** pvt_info_ptr = &ptr->pvt_info;
	
	{
		*mc_idx_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*n_layers_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*tot_dimms_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*layers_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct edac_mc_layer*, (ptr->n_layers));
		if (*layers_ptr) {
			caller_unmarshal_kernel__edac_mc_alloc__ret_layers__out(__pos, msg, ext, ctx, *layers_ptr);
		}

	}

	{
		*dimms_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct dimm_info**, sizeof(struct dimm_info*) * glue_peek(__pos, msg, ext));
		if (*dimms_ptr) {
			int i;
			struct dimm_info** array = *dimms_ptr;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct dimm_info** element = &array[i];
				*element = glue_unpack_new_shadow(__pos, msg, ext, struct dimm_info*, sizeof(struct dimm_info) * glue_peek(__pos, msg, ext));
				if (*element) {
					int i;
					struct dimm_info* array = *element;
					size_t len = glue_unpack(__pos, msg, ext, size_t);
					// Warning: see David if this breaks
					glue_user_trace("Warning: see David if this breaks");
					for (i = 0; i < len; ++i) {
						struct dimm_info* element = &array[i];
						caller_unmarshal_kernel__edac_mc_alloc__ret_dimms__out(__pos, msg, ext, ctx, element);
					}

				}

			}

		}

	}

	{
		*pvt_info_ptr = glue_unpack_new_shadow(__pos, msg, ext, void*, (sizeof(void*)));
		if (*pvt_info_ptr) {
		}

	}

}

void caller_marshal_kernel__edac_mc_alloc__ret_layers__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct edac_mc_layer const* ptr)
{
	
}

void callee_unmarshal_kernel__edac_mc_alloc__ret_layers__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct edac_mc_layer* ptr)
{
	
}

void callee_marshal_kernel__edac_mc_alloc__ret_layers__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct edac_mc_layer const* ptr)
{
	int const* type_ptr = &ptr->type;
	unsigned int const* size_ptr = &ptr->size;
	bool const* is_virt_csrow_ptr = &ptr->is_virt_csrow;
	
	{
		glue_pack(__pos, msg, ext, *type_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *size_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *is_virt_csrow_ptr);
	}

}

void caller_unmarshal_kernel__edac_mc_alloc__ret_layers__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct edac_mc_layer* ptr)
{
	int* type_ptr = &ptr->type;
	unsigned int* size_ptr = &ptr->size;
	bool* is_virt_csrow_ptr = &ptr->is_virt_csrow;
	
	{
		*type_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*size_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*is_virt_csrow_ptr = glue_unpack(__pos, msg, ext, bool);
	}

}

void caller_marshal_kernel__edac_mc_alloc__ret_dimms__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct dimm_info const* ptr)
{
	
}

void callee_unmarshal_kernel__edac_mc_alloc__ret_dimms__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct dimm_info* ptr)
{
	
}

void callee_marshal_kernel__edac_mc_alloc__ret_dimms__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct dimm_info const* ptr)
{
	
}

void caller_unmarshal_kernel__edac_mc_alloc__ret_dimms__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct dimm_info* ptr)
{
	
}

void caller_marshal_kernel__edac_mc_alloc__layers__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct edac_mc_layer const* ptr)
{
	int const* type_ptr = &ptr->type;
	unsigned int const* size_ptr = &ptr->size;
	bool const* is_virt_csrow_ptr = &ptr->is_virt_csrow;
	
	{
		glue_pack(__pos, msg, ext, *type_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *size_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *is_virt_csrow_ptr);
	}

}

void callee_unmarshal_kernel__edac_mc_alloc__layers__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct edac_mc_layer* ptr)
{
	int* type_ptr = &ptr->type;
	unsigned int* size_ptr = &ptr->size;
	bool* is_virt_csrow_ptr = &ptr->is_virt_csrow;
	
	{
		*type_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*size_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*is_virt_csrow_ptr = glue_unpack(__pos, msg, ext, bool);
	}

}

void callee_marshal_kernel__edac_mc_alloc__layers__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct edac_mc_layer const* ptr)
{
	int const* type_ptr = &ptr->type;
	unsigned int const* size_ptr = &ptr->size;
	bool const* is_virt_csrow_ptr = &ptr->is_virt_csrow;
	
	{
		glue_pack(__pos, msg, ext, *type_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *size_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *is_virt_csrow_ptr);
	}

}

void caller_unmarshal_kernel__edac_mc_alloc__layers__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct edac_mc_alloc_call_ctx const* ctx,
	struct edac_mc_layer* ptr)
{
	int* type_ptr = &ptr->type;
	unsigned int* size_ptr = &ptr->size;
	bool* is_virt_csrow_ptr = &ptr->is_virt_csrow;
	
	{
		*type_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*size_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*is_virt_csrow_ptr = glue_unpack(__pos, msg, ext, bool);
	}

}

void caller_marshal_kernel__pci_get_device__ret_pci_dev__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_device_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	
	{
		if (*bus_ptr) {
			caller_marshal_kernel__pci_get_device__bus__out(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void callee_unmarshal_kernel__pci_get_device__ret_pci_dev__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_device_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	
	{
		if (*bus_ptr) {
			callee_unmarshal_kernel__pci_get_device__bus__out(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

}

void callee_marshal_kernel__pci_get_device__ret_pci_dev__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_device_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	unsigned int const* devfn_ptr = &ptr->devfn;
	
	{
		const void* __adjusted = *bus_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*bus_ptr) {
			callee_marshal_kernel__pci_get_device__bus__out(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *devfn_ptr);
	}

}

void caller_unmarshal_kernel__pci_get_device__ret_pci_dev__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_device_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	unsigned int* devfn_ptr = &ptr->devfn;
	
	{
		*bus_ptr = glue_unpack_bind_or_new_shadow(__pos, msg, ext, struct pci_bus*, sizeof(struct pci_bus));
		if (*bus_ptr) {
			caller_unmarshal_kernel__pci_get_device__bus__out(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		*devfn_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void caller_marshal_kernel__pci_get_device__bus__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_device_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_get_device__bus__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_device_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	
}

void callee_marshal_kernel__pci_get_device__bus__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_device_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	unsigned char const* number_ptr = &ptr->number;
	
	{
		glue_pack(__pos, msg, ext, *number_ptr);
	}

}

void caller_unmarshal_kernel__pci_get_device__bus__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_device_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	unsigned char* number_ptr = &ptr->number;
	
	{
		*number_ptr = glue_unpack(__pos, msg, ext, unsigned char);
	}

}

void caller_marshal_kernel__pci_get_device__from__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_device_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void callee_unmarshal_kernel__pci_get_device__from__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_device_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	
}

void callee_marshal_kernel__pci_get_device__from__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct pci_get_device_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	
}

void caller_unmarshal_kernel__pci_get_device__from__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct pci_get_device_call_ctx const* ctx,
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


#ifdef LCD_ISOLATE
__attribute__((weak)) void shared_mem_init(void) {
	LIBLCD_MSG("Weak shared_mem_init does nothing! Override if you want!");
}
#else
__attribute__((weak)) void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext) {
	LIBLCD_MSG("Weak shared_mem_init_callee does nothing! Override if you want!");
}
#endif	/* LCD_ISOLATE */

