#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void notifier_call_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_notifier_call function_ptr = glue_unpack(__pos, msg, ext, fptr_notifier_call);
	struct notifier_block* nb = 0;
	unsigned long val = 0;
	void* data = 0;
	void** data_ptr = &data;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct notifier_call_call_ctx call_ctx = {nb, val, data};
	__maybe_unused struct notifier_call_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*data_ptr = glue_unpack_new_shadow(__pos, msg, ext, void*, (sizeof(struct mce)), (DEFAULT_GFP_FLAGS));
		if (*data_ptr) {
		}

	}

	ret = function_ptr(nb, val, data);

	*__pos = 0;
	{
		(void)data_ptr;
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mce_unregister_decode_chain(struct notifier_block* nb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct notifier_block** nb_ptr = &nb;
	
	__maybe_unused const struct mce_unregister_decode_chain_call_ctx call_ctx = {nb};
	__maybe_unused const struct mce_unregister_decode_chain_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *nb_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*nb_ptr) {
			caller_marshal_kernel__mce_unregister_decode_chain__notifier_block__in(__pos, msg, ext, ctx, *nb_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_mce_unregister_decode_chain);

	*__pos = 0;
	{
		if (*nb_ptr) {
			caller_unmarshal_kernel__mce_unregister_decode_chain__notifier_block__in(__pos, msg, ext, ctx, *nb_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mce_register_decode_chain(struct notifier_block* nb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct notifier_block** nb_ptr = &nb;
	
	__maybe_unused const struct mce_register_decode_chain_call_ctx call_ctx = {nb};
	__maybe_unused const struct mce_register_decode_chain_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *nb_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*nb_ptr) {
			caller_marshal_kernel__mce_register_decode_chain__notifier_block__in(__pos, msg, ext, ctx, *nb_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_mce_register_decode_chain);

	*__pos = 0;
	{
		if (*nb_ptr) {
			caller_unmarshal_kernel__mce_register_decode_chain__notifier_block__in(__pos, msg, ext, ctx, *nb_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_dev_put(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_dev_put_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_dev_put_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_dev_put__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_pci_dev_put);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_dev_put__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct pci_dev* pci_dev_get(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	struct pci_dev* ret = 0;
	struct pci_dev** ret_ptr = &ret;
	
	__maybe_unused const struct pci_dev_get_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_dev_get_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_dev_get__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_pci_dev_get);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_dev_get__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack_shadow(__pos, msg, ext, struct pci_dev*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__pci_dev_get__ret_pci_dev__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct x86_cpu_id const* x86_match_cpu(struct x86_cpu_id const* match)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct x86_cpu_id const** match_ptr = &match;
	struct x86_cpu_id const* ret = 0;
	struct x86_cpu_id const** ret_ptr = &ret;
	
	__maybe_unused const struct x86_match_cpu_call_ctx call_ctx = {match};
	__maybe_unused const struct x86_match_cpu_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *match_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*match_ptr) {
			size_t i, len;
			struct x86_cpu_id sentinel = { 0 };
			struct x86_cpu_id const* array = *match_ptr;
			for (len = 0; memcmp(&array[len], &sentinel, sizeof(array[0])); ++len) ;
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				struct x86_cpu_id const* element = &array[i];
				caller_marshal_kernel__x86_match_cpu__match__in(__pos, msg, ext, ctx, element);
			}

		}

	}

	glue_call_server(__pos, msg, RPC_ID_x86_match_cpu);

	*__pos = 0;
	{
		(void)match_ptr;
	}

	{
		*ret_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct x86_cpu_id const*, (sizeof(struct x86_cpu_id)), (DEFAULT_GFP_FLAGS));
		if (*ret_ptr) {
			struct x86_cpu_id* writable = (struct x86_cpu_id*)*ret_ptr;
			caller_unmarshal_kernel__x86_match_cpu__ret_x86_cpu_id__out(__pos, msg, ext, ctx, writable);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void edac_mc_handle_error(unsigned int type, struct mem_ctl_info* mci, unsigned short error_count, unsigned long page_frame_number, unsigned long offset_in_page, unsigned long syndrome, int top_layer, int mid_layer, int low_layer, char const* _msg, char const* other_detail)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* type_ptr = &type;
	struct mem_ctl_info** mci_ptr = &mci;
	unsigned short* error_count_ptr = &error_count;
	unsigned long* page_frame_number_ptr = &page_frame_number;
	unsigned long* offset_in_page_ptr = &offset_in_page;
	unsigned long* syndrome_ptr = &syndrome;
	int* top_layer_ptr = &top_layer;
	int* mid_layer_ptr = &mid_layer;
	int* low_layer_ptr = &low_layer;
	char const** _msg_ptr = &_msg;
	char const** other_detail_ptr = &other_detail;
	
	__maybe_unused const struct edac_mc_handle_error_call_ctx call_ctx = {type, mci, error_count, page_frame_number, offset_in_page, syndrome, top_layer, mid_layer, low_layer, _msg, other_detail};
	__maybe_unused const struct edac_mc_handle_error_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *type_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *mci_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*mci_ptr) {
			caller_marshal_kernel__edac_mc_handle_error__mci__in(__pos, msg, ext, ctx, *mci_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *error_count_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *page_frame_number_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *offset_in_page_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *syndrome_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *top_layer_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *mid_layer_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *low_layer_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *_msg_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*_msg_ptr) {
			size_t i, len;
			char const* array = *_msg_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	{
		__maybe_unused const void* __adjusted = *other_detail_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*other_detail_ptr) {
			size_t i, len;
			char const* array = *other_detail_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, msg, ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, msg, ext, *element);
			}

		}

	}

	glue_call_server(__pos, msg, RPC_ID_edac_mc_handle_error);

	*__pos = 0;
	{
	}

	{
		if (*mci_ptr) {
			caller_unmarshal_kernel__edac_mc_handle_error__mci__in(__pos, msg, ext, ctx, *mci_ptr);
		}

	}

	{
	}

	{
	}

	{
	}

	{
	}

	{
	}

	{
	}

	{
	}

	{
		(void)_msg_ptr;
	}

	{
		(void)other_detail_ptr;
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int pci_enable_device(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_device_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_enable_device_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_device__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_pci_enable_device);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_device__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct mem_ctl_info* edac_mc_del_mc(struct device* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct device** dev_ptr = &dev;
	struct mem_ctl_info* ret = 0;
	struct mem_ctl_info** ret_ptr = &ret;
	
	__maybe_unused const struct edac_mc_del_mc_call_ctx call_ctx = {dev};
	__maybe_unused const struct edac_mc_del_mc_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__edac_mc_del_mc__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_edac_mc_del_mc);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__edac_mc_del_mc__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, struct mem_ctl_info*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__edac_mc_del_mc__ret_mem_ctl_info__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int edac_mc_add_mc_with_groups(struct mem_ctl_info* mci, struct attribute_group const** groups)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mem_ctl_info** mci_ptr = &mci;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct edac_mc_add_mc_with_groups_call_ctx call_ctx = {mci, groups};
	__maybe_unused const struct edac_mc_add_mc_with_groups_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *mci_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*mci_ptr) {
			caller_marshal_kernel__edac_mc_add_mc_with_groups__mci__in(__pos, msg, ext, ctx, *mci_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_edac_mc_add_mc_with_groups);

	*__pos = 0;
	{
		if (*mci_ptr) {
			caller_unmarshal_kernel__edac_mc_add_mc_with_groups__mci__in(__pos, msg, ext, ctx, *mci_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void edac_mc_free(struct mem_ctl_info* mci)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mem_ctl_info** mci_ptr = &mci;
	
	__maybe_unused const struct edac_mc_free_call_ctx call_ctx = {mci};
	__maybe_unused const struct edac_mc_free_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *mci_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*mci_ptr) {
			caller_marshal_kernel__edac_mc_free__mci__in(__pos, msg, ext, ctx, *mci_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_edac_mc_free);

	*__pos = 0;
	{
		//glue_remove_shadow(*mci_ptr);
		if (*mci_ptr) {
			caller_unmarshal_kernel__edac_mc_free__mci__in(__pos, msg, ext, ctx, *mci_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct mem_ctl_info* edac_mc_alloc(unsigned int mc_num, unsigned int n_layers, struct edac_mc_layer* layers, unsigned int sz_pvt)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* mc_num_ptr = &mc_num;
	unsigned int* n_layers_ptr = &n_layers;
	struct edac_mc_layer** layers_ptr = &layers;
	unsigned int* sz_pvt_ptr = &sz_pvt;
	struct mem_ctl_info* ret = 0;
	struct mem_ctl_info** ret_ptr = &ret;
	
	__maybe_unused const struct edac_mc_alloc_call_ctx call_ctx = {mc_num, n_layers, layers, sz_pvt};
	__maybe_unused const struct edac_mc_alloc_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *mc_num_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *n_layers_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *layers_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*layers_ptr) {
			size_t i, len = (ctx->n_layers);
			struct edac_mc_layer* array = *layers_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct edac_mc_layer* element = &array[i];
				caller_marshal_kernel__edac_mc_alloc__layers__in(__pos, msg, ext, ctx, element);
			}

		}

	}

	{
		glue_pack(__pos, msg, ext, *sz_pvt_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_edac_mc_alloc);

	*__pos = 0;
	{
	}

	{
	}

	{
		(void)layers_ptr;
	}

	{
	}

	{
		*ret_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct mem_ctl_info*, (sizeof(struct mem_ctl_info) + ctx->sz_pvt), (DEFAULT_GFP_FLAGS));
		if (*ret_ptr) {
			caller_unmarshal_kernel__edac_mc_alloc__ret_mem_ctl_info__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

struct pci_dev* pci_get_device(unsigned int vendor, unsigned int device, struct pci_dev* from)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* vendor_ptr = &vendor;
	unsigned int* device_ptr = &device;
	struct pci_dev** from_ptr = &from;
	struct pci_dev* ret = 0;
	struct pci_dev** ret_ptr = &ret;
	
	__maybe_unused const struct pci_get_device_call_ctx call_ctx = {vendor, device, from};
	__maybe_unused const struct pci_get_device_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *vendor_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *device_ptr);
	}

	if (from)
	{
		__maybe_unused const void* __adjusted = *from_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*from_ptr) {
			caller_marshal_kernel__pci_get_device__from__in(__pos, msg, ext, ctx, *from_ptr);
		}

	} else {
		glue_pack(__pos, msg, ext, 0x0);
	}

	glue_call_server(__pos, msg, RPC_ID_pci_get_device);

	*__pos = 0;
	{
	}

	{
	}

	{
		if (*from_ptr) {
			caller_unmarshal_kernel__pci_get_device__from__in(__pos, msg, ext, ctx, *from_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack_bind_or_new_shadow(__pos, msg, ext, struct pci_dev*, sizeof(struct pci_dev));
		if (*ret_ptr) {
			caller_unmarshal_kernel__pci_get_device__ret_pci_dev__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pci_bus_read_config_dword(struct pci_bus* bus, unsigned int devfn, int pos, unsigned int* value)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_bus** bus_ptr = &bus;
	unsigned int* devfn_ptr = &devfn;
	int* pos_ptr = &pos;
	unsigned int** value_ptr = &value;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_bus_read_config_dword_call_ctx call_ctx = {bus, devfn, pos, value};
	__maybe_unused const struct pci_bus_read_config_dword_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *bus_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__pci_bus_read_config_dword__bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *devfn_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *pos_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *value_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		(void)value_ptr;
	}

	glue_call_server(__pos, msg, RPC_ID_pci_bus_read_config_dword);

	*__pos = 0;
	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__pci_bus_read_config_dword__bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
	}

	{
	}

	{
		if (*value_ptr) {
			**value_ptr = glue_unpack(__pos, msg, ext, unsigned int);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext)
{
	switch(id) {
	case MODULE_INIT:
		glue_user_trace("MODULE_INIT");
		__module_lcd_init();
		shared_mem_init();
		break;

	case MODULE_EXIT:
		glue_user_trace("MODULE_EXIT");
		__module_lcd_exit();
		break;

	case RPC_ID_notifier_call:
		glue_user_trace("notifier_call\n");
		notifier_call_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

