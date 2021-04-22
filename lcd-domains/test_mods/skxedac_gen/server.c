#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

int trmp_impl_notifier_call(fptr_notifier_call target, struct notifier_block* nb, unsigned long val, void* data)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	void** data_ptr = &data;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct notifier_call_call_ctx call_ctx = {nb, val, data};
	__maybe_unused const struct notifier_call_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, msg, ext, target);
	{
		const void* __adjusted = *data_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*data_ptr) {
		}

	}

	glue_call_client(__pos, msg, RPC_ID_notifier_call);

	*__pos = 0;
	{
		(void)data_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_notifier_call)
int LCD_TRAMPOLINE_LINKAGE(trmp_notifier_call) trmp_notifier_call(struct notifier_block* nb, unsigned long val, void* data)
{
	volatile fptr_impl_notifier_call impl;
	fptr_notifier_call target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_notifier_call);
	impl = trmp_impl_notifier_call;
	return impl(target, nb, val, data);
}

void pci_enable_device_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_enable_device_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_enable_device_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_enable_device__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = pci_enable_device(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_enable_device__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void x86_match_cpu_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct x86_cpu_id const* match = 0;
	struct x86_cpu_id const** match_ptr = &match;
	struct x86_cpu_id const* ret = 0;
	struct x86_cpu_id const** ret_ptr = &ret;
	
	__maybe_unused struct x86_match_cpu_call_ctx call_ctx = {match};
	__maybe_unused struct x86_match_cpu_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*match_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct x86_cpu_id const*, sizeof(struct x86_cpu_id) * glue_peek(__pos, msg, ext));
		if (*match_ptr) {
			struct x86_cpu_id* writable = (struct x86_cpu_id*)*match_ptr;
			size_t i, len;
			struct x86_cpu_id* array = writable;
			len = glue_unpack(__pos, msg, ext, size_t) - 1;
			memset(&array[len], 0x0, sizeof(array[len]));
			for (i = 0; i < len; ++i) {
				struct x86_cpu_id* element = &array[i];
				callee_unmarshal_kernel__x86_match_cpu__match__in(__pos, msg, ext, ctx, element);
			}

		}

	}

	ret = x86_match_cpu(match);

	*__pos = 0;
	{
		(void)match_ptr;
	}

	{
		const void* __adjusted = *ret_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__x86_match_cpu__ret_x86_cpu_id__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_dev_get_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	struct pci_dev* ret = 0;
	struct pci_dev** ret_ptr = &ret;
	
	__maybe_unused struct pci_dev_get_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_dev_get_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_dev_get__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = pci_dev_get(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_dev_get__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		const void* __adjusted = *ret_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__pci_dev_get__ret_pci_dev__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_dev_put_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused struct pci_dev_put_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_dev_put_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_dev_put__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	pci_dev_put(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_dev_put__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mce_unregister_decode_chain_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct notifier_block* nb = 0;
	struct notifier_block** nb_ptr = &nb;
	
	__maybe_unused struct mce_unregister_decode_chain_call_ctx call_ctx = {nb};
	__maybe_unused struct mce_unregister_decode_chain_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*nb_ptr = glue_unpack_shadow(__pos, msg, ext, struct notifier_block*);
		if (*nb_ptr) {
			callee_unmarshal_kernel__mce_unregister_decode_chain__notifier_block__in(__pos, msg, ext, ctx, *nb_ptr);
		}

	}

	mce_unregister_decode_chain(nb);

	*__pos = 0;
	{
		if (*nb_ptr) {
			callee_marshal_kernel__mce_unregister_decode_chain__notifier_block__in(__pos, msg, ext, ctx, *nb_ptr);
		}

		glue_remove_shadow(*nb_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mce_register_decode_chain_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct notifier_block* nb = 0;
	struct notifier_block** nb_ptr = &nb;
	
	__maybe_unused struct mce_register_decode_chain_call_ctx call_ctx = {nb};
	__maybe_unused struct mce_register_decode_chain_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*nb_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct notifier_block*, sizeof(struct notifier_block));
		if (*nb_ptr) {
			callee_unmarshal_kernel__mce_register_decode_chain__notifier_block__in(__pos, msg, ext, ctx, *nb_ptr);
		}

	}

	mce_register_decode_chain(nb);

	*__pos = 0;
	{
		if (*nb_ptr) {
			callee_marshal_kernel__mce_register_decode_chain__notifier_block__in(__pos, msg, ext, ctx, *nb_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void edac_mc_handle_error_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int type = 0;
	struct mem_ctl_info* mci = 0;
	unsigned short error_count = 0;
	unsigned long page_frame_number = 0;
	unsigned long offset_in_page = 0;
	unsigned long syndrome = 0;
	int top_layer = 0;
	int mid_layer = 0;
	int low_layer = 0;
	char const* _msg = 0;
	char const* other_detail = 0;
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
	
	__maybe_unused struct edac_mc_handle_error_call_ctx call_ctx = {type, mci, error_count, page_frame_number, offset_in_page, syndrome, top_layer, mid_layer, low_layer, _msg, other_detail};
	__maybe_unused struct edac_mc_handle_error_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*type_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*mci_ptr = glue_unpack(__pos, msg, ext, struct mem_ctl_info*);
		if (*mci_ptr) {
			callee_unmarshal_kernel__edac_mc_handle_error__mci__in(__pos, msg, ext, ctx, *mci_ptr);
		}

	}

	{
		*error_count_ptr = glue_unpack(__pos, msg, ext, unsigned short);
	}

	{
		*page_frame_number_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		*offset_in_page_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		*syndrome_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		*top_layer_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*mid_layer_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*low_layer_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*_msg_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, sizeof(char) * glue_peek(__pos, msg, ext));
		if (*_msg_ptr) {
			char* writable = (char*)*_msg_ptr;
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
		*other_detail_ptr = glue_unpack_new_shadow(__pos, msg, ext, char const*, sizeof(char) * glue_peek(__pos, msg, ext));
		if (*other_detail_ptr) {
			char* writable = (char*)*other_detail_ptr;
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

	edac_mc_handle_error(type, mci, error_count, page_frame_number, offset_in_page, syndrome, top_layer, mid_layer, low_layer, _msg, other_detail);

	*__pos = 0;
	{
	}

	{
		if (*mci_ptr) {
			callee_marshal_kernel__edac_mc_handle_error__mci__in(__pos, msg, ext, ctx, *mci_ptr);
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

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void edac_mc_del_mc_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct device* dev = 0;
	struct device** dev_ptr = &dev;
	struct mem_ctl_info* ret = 0;
	struct mem_ctl_info** ret_ptr = &ret;
	
	__maybe_unused struct edac_mc_del_mc_call_ctx call_ctx = {dev};
	__maybe_unused struct edac_mc_del_mc_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__edac_mc_del_mc__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	ret = edac_mc_del_mc(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__edac_mc_del_mc__dev__in(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		const void* __adjusted = *ret_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__edac_mc_del_mc__ret_mem_ctl_info__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void edac_mc_add_mc_with_groups_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mem_ctl_info* mci = 0;
	struct attribute_group const** groups = 0;
	struct mem_ctl_info** mci_ptr = &mci;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct edac_mc_add_mc_with_groups_call_ctx call_ctx = {mci, groups};
	__maybe_unused struct edac_mc_add_mc_with_groups_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*mci_ptr = glue_unpack(__pos, msg, ext, struct mem_ctl_info*);
		if (*mci_ptr) {
			callee_unmarshal_kernel__edac_mc_add_mc_with_groups__mci__in(__pos, msg, ext, ctx, *mci_ptr);
		}

	}

	mci->mod_name = "skx_edac.c";

	printk("%s, mci->ctlname %s | mci->mtype_cap %ld "
			"mci->edac_cap %ld | mci->mod_name %s | mci->dev_name %s | mci->mod_ver %s\n",
			__func__, mci->ctl_name, mci->mtype_cap, mci->edac_cap, mci->mod_name, mci->dev_name, mci->mod_ver);

	ret = edac_mc_add_mc_with_groups(mci, groups);

	printk("%s returned %d\n", __func__, ret);
	*__pos = 0;
	{
		if (*mci_ptr) {
			callee_marshal_kernel__edac_mc_add_mc_with_groups__mci__in(__pos, msg, ext, ctx, *mci_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void edac_mc_free_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct mem_ctl_info* mci = 0;
	struct mem_ctl_info** mci_ptr = &mci;
	
	__maybe_unused struct edac_mc_free_call_ctx call_ctx = {mci};
	__maybe_unused struct edac_mc_free_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*mci_ptr = glue_unpack(__pos, msg, ext, struct mem_ctl_info*);
		if (*mci_ptr) {
			callee_unmarshal_kernel__edac_mc_free__mci__in(__pos, msg, ext, ctx, *mci_ptr);
		}

	}

	edac_mc_free(mci);

	*__pos = 0;
	{
		if (*mci_ptr) {
			callee_marshal_kernel__edac_mc_free__mci__in(__pos, msg, ext, ctx, *mci_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void edac_mc_alloc_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int mc_num = 0;
	unsigned int n_layers = 0;
	struct edac_mc_layer* layers = 0;
	unsigned int sz_pvt = 0;
	unsigned int* mc_num_ptr = &mc_num;
	unsigned int* n_layers_ptr = &n_layers;
	struct edac_mc_layer** layers_ptr = &layers;
	unsigned int* sz_pvt_ptr = &sz_pvt;
	struct mem_ctl_info* ret = 0;
	struct mem_ctl_info** ret_ptr = &ret;
	
	__maybe_unused struct edac_mc_alloc_call_ctx call_ctx = {mc_num, n_layers, layers, sz_pvt};
	__maybe_unused struct edac_mc_alloc_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*mc_num_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*n_layers_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*layers_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct edac_mc_layer*, sizeof(struct edac_mc_layer) * glue_peek(__pos, msg, ext));
		if (*layers_ptr) {
			int i;
			struct edac_mc_layer* array = *layers_ptr;
			size_t len = glue_unpack(__pos, msg, ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct edac_mc_layer* element = &array[i];
				callee_unmarshal_kernel__edac_mc_alloc__layers__io(__pos, msg, ext, ctx, element);
			}

		}

	}

	{
		*sz_pvt_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = edac_mc_alloc(mc_num, n_layers, layers, sz_pvt);

	*__pos = 0;
	{
	}

	{
	}

	{
		if (*layers_ptr) {
			size_t i, len = (ctx->n_layers);
			struct edac_mc_layer* array = *layers_ptr;
			glue_pack(__pos, msg, ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct edac_mc_layer* element = &array[i];
				callee_marshal_kernel__edac_mc_alloc__layers__io(__pos, msg, ext, ctx, element);
			}

		}

	}

	{
	}

	{
		const void* __adjusted = *ret_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__edac_mc_alloc__ret_mem_ctl_info__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_get_device_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int vendor = 0;
	unsigned int device = 0;
	struct pci_dev* from = 0;
	unsigned int* vendor_ptr = &vendor;
	unsigned int* device_ptr = &device;
	struct pci_dev** from_ptr = &from;
	struct pci_dev* ret = 0;
	struct pci_dev** ret_ptr = &ret;
	
	__maybe_unused struct pci_get_device_call_ctx call_ctx = {vendor, device, from};
	__maybe_unused struct pci_get_device_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*vendor_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*device_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*from_ptr = glue_unpack(__pos, msg, ext, struct pci_dev*);
		if (*from_ptr) {
			callee_unmarshal_kernel__pci_get_device__from__in(__pos, msg, ext, ctx, *from_ptr);
		}

	}

	ret = pci_get_device(vendor, device, from);

	*__pos = 0;
	{
	}

	{
	}

	{
		if (*from_ptr) {
			callee_marshal_kernel__pci_get_device__from__in(__pos, msg, ext, ctx, *from_ptr);
		}

	}

	{
		const void* __adjusted = *ret_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__pci_get_device__ret_pci_dev__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_bus_read_config_dword_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_bus* bus = 0;
	unsigned int devfn = 0;
	int pos = 0;
	unsigned int* value = 0;
	struct pci_bus** bus_ptr = &bus;
	unsigned int* devfn_ptr = &devfn;
	int* pos_ptr = &pos;
	unsigned int** value_ptr = &value;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_bus_read_config_dword_call_ctx call_ctx = {bus, devfn, pos, value};
	__maybe_unused struct pci_bus_read_config_dword_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*bus_ptr = glue_unpack(__pos, msg, ext, struct pci_bus*);
		if (*bus_ptr) {
			callee_unmarshal_kernel__pci_bus_read_config_dword__bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
		*devfn_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*pos_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*value_ptr = glue_unpack_new_shadow(__pos, msg, ext, unsigned int*, sizeof(unsigned int));
		(void)value_ptr;
	}

	ret = pci_bus_read_config_dword(bus, devfn, pos, value);

	*__pos = 0;
	{
		if (*bus_ptr) {
			callee_marshal_kernel__pci_bus_read_config_dword__bus__in(__pos, msg, ext, ctx, *bus_ptr);
		}

	}

	{
	}

	{
	}

	{
		if (*value_ptr) {
			glue_pack(__pos, msg, ext, **value_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext)
{
	switch(id) {
	case RPC_ID_shared_mem_init:
		glue_user_trace("shared_mem_init\n");
		shared_mem_init_callee(msg, ext);
		break;

	case RPC_ID_pci_enable_device:
		glue_user_trace("pci_enable_device\n");
		pci_enable_device_callee(msg, ext);
		break;

	case RPC_ID_x86_match_cpu:
		glue_user_trace("x86_match_cpu\n");
		x86_match_cpu_callee(msg, ext);
		break;

	case RPC_ID_pci_dev_get:
		glue_user_trace("pci_dev_get\n");
		pci_dev_get_callee(msg, ext);
		break;

	case RPC_ID_pci_dev_put:
		glue_user_trace("pci_dev_put\n");
		pci_dev_put_callee(msg, ext);
		break;

	case RPC_ID_mce_unregister_decode_chain:
		glue_user_trace("mce_unregister_decode_chain\n");
		mce_unregister_decode_chain_callee(msg, ext);
		break;

	case RPC_ID_mce_register_decode_chain:
		glue_user_trace("mce_register_decode_chain\n");
		mce_register_decode_chain_callee(msg, ext);
		break;

	case RPC_ID_edac_mc_handle_error:
		glue_user_trace("edac_mc_handle_error\n");
		edac_mc_handle_error_callee(msg, ext);
		break;

	case RPC_ID_edac_mc_del_mc:
		glue_user_trace("edac_mc_del_mc\n");
		edac_mc_del_mc_callee(msg, ext);
		break;

	case RPC_ID_edac_mc_add_mc_with_groups:
		glue_user_trace("edac_mc_add_mc_with_groups\n");
		edac_mc_add_mc_with_groups_callee(msg, ext);
		break;

	case RPC_ID_edac_mc_free:
		glue_user_trace("edac_mc_free\n");
		edac_mc_free_callee(msg, ext);
		break;

	case RPC_ID_edac_mc_alloc:
		glue_user_trace("edac_mc_alloc\n");
		edac_mc_alloc_callee(msg, ext);
		break;

	case RPC_ID_pci_get_device:
		glue_user_trace("pci_get_device\n");
		pci_get_device_callee(msg, ext);
		break;

	case RPC_ID_pci_bus_read_config_dword:
		glue_user_trace("pci_bus_read_config_dword\n");
		pci_bus_read_config_dword_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

