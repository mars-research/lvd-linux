#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel__add_timer__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct add_timer_call_ctx const* ctx,
	struct timer_list const* ptr)
{
	unsigned long const* expires_ptr = &ptr->expires;

	{
		glue_pack(__pos, __msg, __ext, *expires_ptr);
	}

}

void callee_unmarshal_kernel__add_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct add_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{
	unsigned long* expires_ptr = &ptr->expires;

	{
		*expires_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void callee_marshal_kernel__add_timer__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct add_timer_call_ctx const* ctx,
	struct timer_list const* ptr)
{
	unsigned long const* expires_ptr = &ptr->expires;

	{
		glue_pack(__pos, __msg, __ext, *expires_ptr);
	}

}

void caller_unmarshal_kernel__add_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct add_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{
	unsigned long* expires_ptr = &ptr->expires;

	{
		*expires_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void caller_marshal_kernel__complete__x__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct complete_call_ctx const* ctx,
	struct completion const* ptr)
{
	unsigned int const* done_ptr = &ptr->done;

	{
		glue_pack(__pos, __msg, __ext, *done_ptr);
	}

}

void callee_unmarshal_kernel__complete__x__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct complete_call_ctx const* ctx,
	struct completion* ptr)
{
	unsigned int* done_ptr = &ptr->done;

	{
		*done_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__complete__x__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct complete_call_ctx const* ctx,
	struct completion const* ptr)
{
	unsigned int const* done_ptr = &ptr->done;

	{
		glue_pack(__pos, __msg, __ext, *done_ptr);
	}

}

void caller_unmarshal_kernel__complete__x__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct complete_call_ctx const* ctx,
	struct completion* ptr)
{
	unsigned int* done_ptr = &ptr->done;

	{
		*done_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__del_timer__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_call_ctx const* ctx,
	struct timer_list const* ptr)
{

}

void callee_unmarshal_kernel__del_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{

	{
	}
}

void callee_marshal_kernel__del_timer__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_call_ctx const* ctx,
	struct timer_list const* ptr)
{

}

void caller_unmarshal_kernel__del_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_call_ctx const* ctx,
	struct timer_list* ptr)
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
	unsigned long const* expires_ptr = &ptr->expires;

	{
		glue_pack(__pos, __msg, __ext, *expires_ptr);
	}

}

void caller_unmarshal_kernel__mod_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{
	unsigned long* expires_ptr = &ptr->expires;

	{
		*expires_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
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

}

void callee_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
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

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
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

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* ctx,
	struct pci_dev* ptr)
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

void caller_marshal_kernel__wait_for_completion__x__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct wait_for_completion_call_ctx const* ctx,
	struct completion const* ptr)
{
	unsigned int const* done_ptr = &ptr->done;

	{
		glue_pack(__pos, __msg, __ext, *done_ptr);
	}

}

void callee_unmarshal_kernel__wait_for_completion__x__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct wait_for_completion_call_ctx const* ctx,
	struct completion* ptr)
{
	unsigned int* done_ptr = &ptr->done;

	{
		*done_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__wait_for_completion__x__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct wait_for_completion_call_ctx const* ctx,
	struct completion const* ptr)
{
	unsigned int const* done_ptr = &ptr->done;

	{
		glue_pack(__pos, __msg, __ext, *done_ptr);
	}

}

void caller_unmarshal_kernel__wait_for_completion__x__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct wait_for_completion_call_ctx const* ctx,
	struct completion* ptr)
{
	unsigned int* done_ptr = &ptr->done;

	{
		*done_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__wait_for_completion_timeout__x__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct wait_for_completion_timeout_call_ctx const* ctx,
	struct completion const* ptr)
{
	unsigned int const* done_ptr = &ptr->done;

	{
		glue_pack(__pos, __msg, __ext, *done_ptr);
	}

}

void callee_unmarshal_kernel__wait_for_completion_timeout__x__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct wait_for_completion_timeout_call_ctx const* ctx,
	struct completion* ptr)
{
	unsigned int* done_ptr = &ptr->done;

	{
		*done_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__wait_for_completion_timeout__x__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct wait_for_completion_timeout_call_ctx const* ctx,
	struct completion const* ptr)
{
	unsigned int const* done_ptr = &ptr->done;

	{
		glue_pack(__pos, __msg, __ext, *done_ptr);
	}

}

void caller_unmarshal_kernel__wait_for_completion_timeout__x__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct wait_for_completion_timeout_call_ctx const* ctx,
	struct completion* ptr)
{
	unsigned int* done_ptr = &ptr->done;

	{
		*done_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__get_quirks__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_quirks_call_ctx const* ctx,
	struct device const* ptr)
{

}

void callee_unmarshal_kernel__get_quirks__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_quirks_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void callee_marshal_kernel__get_quirks__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_quirks_call_ctx const* ctx,
	struct device const* ptr)
{

}

void caller_unmarshal_kernel__get_quirks__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_quirks_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void caller_marshal_kernel__get_quirks__xhci_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_quirks_call_ctx const* ctx,
	struct xhci_hcd const* ptr)
{

}

void callee_unmarshal_kernel__get_quirks__xhci_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_quirks_call_ctx const* ctx,
	struct xhci_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__get_quirks__xhci_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_quirks_call_ctx const* ctx,
	struct xhci_hcd const* ptr)
{

}

void caller_unmarshal_kernel__get_quirks__xhci_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_quirks_call_ctx const* ctx,
	struct xhci_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__xhci_gen_setup_with_xhci__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_bus const* self_ptr = &ptr->self;
	struct hc_driver* const* driver_ptr = &ptr->driver;
	unsigned long const* rsrc_start_ptr = &ptr->rsrc_start;
	unsigned long const *rsrc_len_ptr = &ptr->rsrc_len;
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		caller_marshal_kernel__xhci_gen_setup_with_xhci__usb_bus__in(__pos, __msg, __ext, ctx, self_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *driver_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*driver_ptr) {
			caller_marshal_kernel__xhci_gen_setup_with_xhci__hc_driver__in(__pos, __msg, __ext, ctx, *driver_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *rsrc_start_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *rsrc_len_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__xhci_gen_setup_with_xhci__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__xhci_gen_setup_with_xhci__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_bus* self_ptr = &ptr->self;
	struct hc_driver** driver_ptr = &ptr->driver;
	unsigned long* rsrc_start_ptr = &ptr->rsrc_start;
	unsigned long* rsrc_len_ptr = &ptr->rsrc_len;
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		callee_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_bus__in(__pos, __msg, __ext, ctx, self_ptr);
	}

	{
		*driver_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct hc_driver*);
		if (*driver_ptr) {
			callee_unmarshal_kernel__xhci_gen_setup_with_xhci__hc_driver__in(__pos, __msg, __ext, ctx, *driver_ptr);
		}

	}

	{
		*rsrc_start_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*rsrc_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		size_t __size = sizeof(struct usb_hcd);
		*primary_hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__xhci_gen_setup_with_xhci__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_bus const* self_ptr = &ptr->self;
	int const* speed_ptr = &ptr->speed;
	struct hc_driver* const* driver_ptr = &ptr->driver;
	unsigned int __rh_registered = ptr->rh_registered;
	unsigned int const* __rh_registered_ptr = &__rh_registered;
	unsigned int __has_tt = ptr->has_tt;
	unsigned int const* __has_tt_ptr = &__has_tt;
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		callee_marshal_kernel__xhci_gen_setup_with_xhci__usb_bus__in(__pos, __msg, __ext, ctx, self_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *speed_ptr);
	}

	{
		if (*driver_ptr) {
			callee_marshal_kernel__xhci_gen_setup_with_xhci__hc_driver__in(__pos, __msg, __ext, ctx, *driver_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *__rh_registered_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__has_tt_ptr);
	}

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__xhci_gen_setup_with_xhci__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__xhci_gen_setup_with_xhci__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_bus* self_ptr = &ptr->self;
	int* speed_ptr = &ptr->speed;
	struct hc_driver** driver_ptr = &ptr->driver;
	unsigned int __rh_registered = ptr->rh_registered;
	unsigned int* __rh_registered_ptr = &__rh_registered;
	unsigned int __has_tt = ptr->has_tt;
	unsigned int* __has_tt_ptr = &__has_tt;
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		caller_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_bus__in(__pos, __msg, __ext, ctx, self_ptr);
	}

	{
		*speed_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		if (*driver_ptr) {
			caller_unmarshal_kernel__xhci_gen_setup_with_xhci__hc_driver__in(__pos, __msg, __ext, ctx, *driver_ptr);
		}

	}

	{
		*__rh_registered_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__has_tt_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
		ptr->rh_registered = *__rh_registered_ptr;
		ptr->has_tt = *__has_tt_ptr;
	}
}

void caller_marshal_kernel__xhci_gen_setup_with_xhci__usb_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_bus const* ptr)
{
	struct device* const* controller_ptr = &ptr->controller;
	struct pci_dev* _pci_dev_ptr = to_pci_dev(ptr->controller);
	struct pci_dev* const* pci_dev_ptr = &_pci_dev_ptr;
	unsigned int __is_b_host = ptr->is_b_host;
	unsigned int const* __is_b_host_ptr = &__is_b_host;
	struct usb_device* const* root_hub_ptr = &ptr->root_hub;

	{
		__maybe_unused const void* __adjusted = *pci_dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*pci_dev_ptr) {
			caller_marshal_kernel__xhci_gen_setup_with_xhci__pci_device__in(__pos, __msg, __ext, ctx, *pci_dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *controller_ptr;
		const ptrdiff_t __offset = (void*)__adjusted - (void*) _pci_dev_ptr;
		if (__offset >= (sizeof(struct device)) || __offset < 0)
			glue_user_panic("Bounds check failed!");

		glue_pack(__pos, __msg, __ext, __offset);
		if (*controller_ptr) {
			caller_marshal_kernel__xhci_gen_setup_with_xhci__usb_bus_controller__in(__pos, __msg, __ext, ctx, *controller_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *__is_b_host_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *root_hub_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*root_hub_ptr) {
			caller_marshal_kernel__xhci_gen_setup_with_xhci__usb_bus_root_hub__in(__pos, __msg, __ext, ctx, *root_hub_ptr);
		}

	}

}

void callee_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_bus* ptr)
{
	struct pci_dev* pci_dev_ptr;
	struct device** controller_ptr = &ptr->controller;
	unsigned int __is_b_host = ptr->is_b_host;
	unsigned int* __is_b_host_ptr = &__is_b_host;
	struct usb_device** root_hub_ptr = &ptr->root_hub;

	{
		size_t __size = sizeof(struct pci_dev);
		pci_dev_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct pci_dev*, (__size), (DEFAULT_GFP_FLAGS));
		if (pci_dev_ptr) {
			callee_unmarshal_kernel__xhci_gen_setup_with_xhci__pci_device__in(__pos, __msg, __ext, ctx, pci_dev_ptr);
		}

	}

	{
		__maybe_unused size_t __offset = glue_unpack(__pos, __msg, __ext, size_t);
		*controller_ptr = &pci_dev_ptr->dev;
		if (*controller_ptr) {
			callee_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_bus_controller__in(__pos, __msg, __ext, ctx, *controller_ptr);
		}

	}

	{
		*__is_b_host_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		size_t __size = sizeof(struct usb_device);
		*root_hub_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct usb_device*, (__size), (DEFAULT_GFP_FLAGS));
		if (*root_hub_ptr) {
			callee_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_bus_root_hub__in(__pos, __msg, __ext, ctx, *root_hub_ptr);
		}

	}

	{
		ptr->is_b_host = *__is_b_host_ptr;
	}
}

void callee_marshal_kernel__xhci_gen_setup_with_xhci__usb_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_bus const* ptr)
{
	struct device* const* controller_ptr = &ptr->controller;
	struct pci_dev* _pci_dev_ptr = to_pci_dev(ptr->controller);
	struct pci_dev* const* pci_dev_ptr = &_pci_dev_ptr;
	unsigned int __is_b_host = ptr->is_b_host;
	unsigned int const* __is_b_host_ptr = &__is_b_host;
	struct usb_device* const* root_hub_ptr = &ptr->root_hub;

	{
		if (*pci_dev_ptr) {
			callee_marshal_kernel__xhci_gen_setup_with_xhci__pci_device__in(__pos, __msg, __ext, ctx, *pci_dev_ptr);
		}

	}

	{
		if (*controller_ptr) {
			callee_marshal_kernel__xhci_gen_setup_with_xhci__usb_bus_controller__in(__pos, __msg, __ext, ctx, *controller_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *__is_b_host_ptr);
	}

	{
		if (*root_hub_ptr) {
			callee_marshal_kernel__xhci_gen_setup_with_xhci__usb_bus_root_hub__in(__pos, __msg, __ext, ctx, *root_hub_ptr);
		}

	}

}

void caller_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_bus* ptr)
{
	struct device** controller_ptr = &ptr->controller;
	struct pci_dev* _pci_dev_ptr = to_pci_dev(ptr->controller);
	struct pci_dev** pci_dev_ptr = &_pci_dev_ptr;
	unsigned int __is_b_host = ptr->is_b_host;
	unsigned int* __is_b_host_ptr = &__is_b_host;
	struct usb_device** root_hub_ptr = &ptr->root_hub;

	{
		if (*pci_dev_ptr) {
			caller_unmarshal_kernel__xhci_gen_setup_with_xhci__pci_device__in(__pos, __msg, __ext, ctx, *pci_dev_ptr);
		}

	}

	{
		if (*controller_ptr) {
			caller_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_bus_controller__in(__pos, __msg, __ext, ctx, *controller_ptr);
		}

	}

	{
		*__is_b_host_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		if (*root_hub_ptr) {
			caller_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_bus_root_hub__in(__pos, __msg, __ext, ctx, *root_hub_ptr);
		}

	}

	{
		ptr->is_b_host = *__is_b_host_ptr;
	}
}

void caller_marshal_kernel__xhci_gen_setup_with_xhci__pci_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned short const* vendor_ptr = &ptr->vendor;
	unsigned short const* device_ptr = &ptr->device;
	unsigned int const* devfn_ptr = &ptr->devfn;
	struct pci_bus* const* bus_ptr = &ptr->bus;

	{
		glue_pack(__pos, __msg, __ext, *vendor_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *device_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *devfn_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *bus_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__xhci_gen_setup_with_xhci__pci_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

}

void callee_unmarshal_kernel__xhci_gen_setup_with_xhci__pci_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned short* vendor_ptr = &ptr->vendor;
	unsigned short* device_ptr = &ptr->device;
	unsigned int* devfn_ptr = &ptr->devfn;
	struct pci_bus** bus_ptr = &ptr->bus;

	{
		*vendor_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*device_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*devfn_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		size_t __size = sizeof(struct pci_bus);
		*bus_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct pci_bus*, (__size), (DEFAULT_GFP_FLAGS));
		if (*bus_ptr) {
			callee_unmarshal_kernel__xhci_gen_setup_with_xhci__pci_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__xhci_gen_setup_with_xhci__pci_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;

	{
		if (*bus_ptr) {
			callee_marshal_kernel__xhci_gen_setup_with_xhci__pci_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

}

void caller_unmarshal_kernel__xhci_gen_setup_with_xhci__pci_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;

	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__xhci_gen_setup_with_xhci__pci_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__xhci_gen_setup_with_xhci__pci_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	unsigned char const* number_ptr = &ptr->number;

	{
		glue_pack(__pos, __msg, __ext, *number_ptr);
	}

}

void callee_unmarshal_kernel__xhci_gen_setup_with_xhci__pci_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	unsigned char* number_ptr = &ptr->number;

	{
		*number_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void callee_marshal_kernel__xhci_gen_setup_with_xhci__pci_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct pci_bus const* ptr)
{

}

void caller_unmarshal_kernel__xhci_gen_setup_with_xhci__pci_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct pci_bus* ptr)
{

	{
	}
}

void caller_marshal_kernel__xhci_gen_setup_with_xhci__usb_bus_controller__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct device const* ptr)
{
	unsigned long long* const* dma_mask_ptr = &ptr->dma_mask;

	{
		__maybe_unused const void* __adjusted = *dma_mask_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dma_mask_ptr) {
			size_t i, len = 1;
			unsigned long long const* array = *dma_mask_ptr;
			glue_pack(__pos, __msg, __ext, len);
			for (i = 0; i < len; ++i) {
				unsigned long long const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

}

void callee_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_bus_controller__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct device* ptr)
{
	unsigned long long** dma_mask_ptr = &ptr->dma_mask;

	{
		size_t __size = sizeof(unsigned long long) * glue_peek(__pos, __msg, __ext);
		*dma_mask_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, unsigned long long*, (__size), (DEFAULT_GFP_FLAGS));
		if (*dma_mask_ptr) {
			int i;
			unsigned long long* array = *dma_mask_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			for (i = 0; i < len; ++i) {
				unsigned long long* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, unsigned long long);
			}

		}

	}

	{
	}
}

void callee_marshal_kernel__xhci_gen_setup_with_xhci__usb_bus_controller__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct device const* ptr)
{
	unsigned long long* const* dma_mask_ptr = &ptr->dma_mask;

	{
		(void)dma_mask_ptr;
	}

}

void caller_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_bus_controller__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct device* ptr)
{
	unsigned long long** dma_mask_ptr = &ptr->dma_mask;

	{
		(void)dma_mask_ptr;
	}

	{
	}
}

void caller_marshal_kernel__xhci_gen_setup_with_xhci__usb_bus_root_hub__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void callee_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_bus_root_hub__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__xhci_gen_setup_with_xhci__usb_bus_root_hub__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	unsigned int const* speed_ptr = &ptr->speed;

	{
		glue_pack(__pos, __msg, __ext, *speed_ptr);
	}

}

void caller_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_bus_root_hub__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_device* ptr)
{
	unsigned int* speed_ptr = &ptr->speed;

	{
		*speed_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__xhci_gen_setup_with_xhci__hc_driver__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct hc_driver const* ptr)
{

}

void callee_unmarshal_kernel__xhci_gen_setup_with_xhci__hc_driver__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct hc_driver* ptr)
{

	{
	}
}

void callee_marshal_kernel__xhci_gen_setup_with_xhci__hc_driver__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct hc_driver const* ptr)
{

}

void caller_unmarshal_kernel__xhci_gen_setup_with_xhci__hc_driver__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct hc_driver* ptr)
{

	{
	}
}

void caller_marshal_kernel__xhci_gen_setup_with_xhci__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__xhci_gen_setup_with_xhci__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__xhci_gen_setup_with_xhci__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__xhci_gen_setup_with_xhci__xhci_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct xhci_hcd const* ptr)
{
	char const* sbrn_ptr = &ptr->sbrn;
	unsigned short const* hci_version_ptr = &ptr->hci_version;
	unsigned int const* quirks_ptr = &ptr->quirks;
	unsigned int const* xhc_state_ptr = &ptr->xhc_state;

	{
		glue_pack(__pos, __msg, __ext, *sbrn_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *hci_version_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *quirks_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *xhc_state_ptr);
	}

}

void callee_unmarshal_kernel__xhci_gen_setup_with_xhci__xhci_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct xhci_hcd* ptr)
{
	char* sbrn_ptr = &ptr->sbrn;
	unsigned short* hci_version_ptr = &ptr->hci_version;
	unsigned int* quirks_ptr = &ptr->quirks;
	unsigned int* xhc_state_ptr = &ptr->xhc_state;

	{
		*sbrn_ptr = glue_unpack(__pos, __msg, __ext, char);
	}

	{
		*hci_version_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*quirks_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*xhc_state_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__xhci_gen_setup_with_xhci__xhci_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct xhci_hcd const* ptr)
{

}

void caller_unmarshal_kernel__xhci_gen_setup_with_xhci__xhci_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_with_xhci_call_ctx const* ctx,
	struct xhci_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__xhci_init_driver___global_hc_driver__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* ctx,
	struct hc_driver const* ptr)
{
	char const* const* description_ptr = &ptr->description;
	char const* const* product_desc_ptr = &ptr->product_desc;

	{
		(void)description_ptr;
	}

	{
		(void)product_desc_ptr;
	}

}

void callee_unmarshal_kernel__xhci_init_driver___global_hc_driver__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* ctx,
	struct hc_driver* ptr)
{
	char const** description_ptr = &ptr->description;
	char const** product_desc_ptr = &ptr->product_desc;

	{
		(void)description_ptr;
	}

	{
		(void)product_desc_ptr;
	}

	{
	}
}

void callee_marshal_kernel__xhci_init_driver___global_hc_driver__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* ctx,
	struct hc_driver const* ptr)
{
	char const* const* description_ptr = &ptr->description;
	char const* const* product_desc_ptr = &ptr->product_desc;
	int const* flags_ptr = &ptr->flags;
	unsigned long const* hcd_priv_size_ptr = &ptr->hcd_priv_size;
	fptr_hc_driver_add_endpoint const* add_endpoint_ptr = &ptr->add_endpoint;
	fptr_hc_driver_address_device const* address_device_ptr = &ptr->address_device;
	fptr_hc_driver_alloc_dev const* alloc_dev_ptr = &ptr->alloc_dev;
	fptr_hc_driver_alloc_streams const* alloc_streams_ptr = &ptr->alloc_streams;
	fptr_hc_driver_check_bandwidth const* check_bandwidth_ptr = &ptr->check_bandwidth;
	fptr_hc_driver_disable_usb3_lpm_timeout const* disable_usb3_lpm_timeout_ptr = &ptr->disable_usb3_lpm_timeout;
	fptr_hc_driver_drop_endpoint const* drop_endpoint_ptr = &ptr->drop_endpoint;
	fptr_hc_driver_enable_device const* enable_device_ptr = &ptr->enable_device;
	fptr_hc_driver_enable_usb3_lpm_timeout const* enable_usb3_lpm_timeout_ptr = &ptr->enable_usb3_lpm_timeout;
	fptr_hc_driver_endpoint_reset const* endpoint_reset_ptr = &ptr->endpoint_reset;
	fptr_hc_driver_find_raw_port_number const* find_raw_port_number_ptr = &ptr->find_raw_port_number;
	fptr_hc_driver_free_dev const* free_dev_ptr = &ptr->free_dev;
	fptr_hc_driver_free_streams const* free_streams_ptr = &ptr->free_streams;
	fptr_hc_driver_get_frame_number const* get_frame_number_ptr = &ptr->get_frame_number;
	fptr_hc_driver_hub_control const* hub_control_ptr = &ptr->hub_control;
	fptr_hc_driver_hub_status_data const* hub_status_data_ptr = &ptr->hub_status_data;
	fptr_hc_driver_irq const* irq_ptr = &ptr->irq;
	fptr_hc_driver_reset_bandwidth const* reset_bandwidth_ptr = &ptr->reset_bandwidth;
	fptr_hc_driver_reset_device const* reset_device_ptr = &ptr->reset_device;
	fptr_hc_driver_set_usb2_hw_lpm const* set_usb2_hw_lpm_ptr = &ptr->set_usb2_hw_lpm;
	fptr_hc_driver_shutdown const* shutdown_ptr = &ptr->shutdown;
	fptr_hc_driver_start const* start_ptr = &ptr->start;
	fptr_hc_driver_stop const* stop_ptr = &ptr->stop;
	fptr_hc_driver_update_device const* update_device_ptr = &ptr->update_device;
	fptr_hc_driver_update_hub_device const* update_hub_device_ptr = &ptr->update_hub_device;
	fptr_hc_driver_urb_dequeue const* urb_dequeue_ptr = &ptr->urb_dequeue;
	fptr_hc_driver_urb_enqueue const* urb_enqueue_ptr = &ptr->urb_enqueue;

	{
		__maybe_unused const void* __adjusted = *description_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*description_ptr) {
			size_t i, len;
			char const* array = *description_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		__maybe_unused const void* __adjusted = *product_desc_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*product_desc_ptr) {
			size_t i, len;
			char const* array = *product_desc_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *hcd_priv_size_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *add_endpoint_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *address_device_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *alloc_dev_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *alloc_streams_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *check_bandwidth_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *disable_usb3_lpm_timeout_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *drop_endpoint_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *enable_device_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *enable_usb3_lpm_timeout_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *endpoint_reset_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *find_raw_port_number_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *free_dev_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *free_streams_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *get_frame_number_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *hub_control_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *hub_status_data_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *reset_bandwidth_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *reset_device_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *set_usb2_hw_lpm_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *shutdown_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *start_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *stop_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *update_device_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *update_hub_device_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *urb_dequeue_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *urb_enqueue_ptr);
	}

}

void caller_unmarshal_kernel__xhci_init_driver___global_hc_driver__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* ctx,
	struct hc_driver* ptr)
{
	char const** description_ptr = &ptr->description;
	char const** product_desc_ptr = &ptr->product_desc;
	int* flags_ptr = &ptr->flags;
	unsigned long* hcd_priv_size_ptr = &ptr->hcd_priv_size;
	fptr_hc_driver_add_endpoint* add_endpoint_ptr = &ptr->add_endpoint;
	fptr_hc_driver_address_device* address_device_ptr = &ptr->address_device;
	fptr_hc_driver_alloc_dev* alloc_dev_ptr = &ptr->alloc_dev;
	fptr_hc_driver_alloc_streams* alloc_streams_ptr = &ptr->alloc_streams;
	fptr_hc_driver_check_bandwidth* check_bandwidth_ptr = &ptr->check_bandwidth;
	fptr_hc_driver_disable_usb3_lpm_timeout* disable_usb3_lpm_timeout_ptr = &ptr->disable_usb3_lpm_timeout;
	fptr_hc_driver_drop_endpoint* drop_endpoint_ptr = &ptr->drop_endpoint;
	fptr_hc_driver_enable_device* enable_device_ptr = &ptr->enable_device;
	fptr_hc_driver_enable_usb3_lpm_timeout* enable_usb3_lpm_timeout_ptr = &ptr->enable_usb3_lpm_timeout;
	fptr_hc_driver_endpoint_reset* endpoint_reset_ptr = &ptr->endpoint_reset;
	fptr_hc_driver_find_raw_port_number* find_raw_port_number_ptr = &ptr->find_raw_port_number;
	fptr_hc_driver_free_dev* free_dev_ptr = &ptr->free_dev;
	fptr_hc_driver_free_streams* free_streams_ptr = &ptr->free_streams;
	fptr_hc_driver_get_frame_number* get_frame_number_ptr = &ptr->get_frame_number;
	fptr_hc_driver_hub_control* hub_control_ptr = &ptr->hub_control;
	fptr_hc_driver_hub_status_data* hub_status_data_ptr = &ptr->hub_status_data;
	fptr_hc_driver_irq* irq_ptr = &ptr->irq;
	fptr_hc_driver_reset_bandwidth* reset_bandwidth_ptr = &ptr->reset_bandwidth;
	fptr_hc_driver_reset_device* reset_device_ptr = &ptr->reset_device;
	fptr_hc_driver_set_usb2_hw_lpm* set_usb2_hw_lpm_ptr = &ptr->set_usb2_hw_lpm;
	fptr_hc_driver_shutdown* shutdown_ptr = &ptr->shutdown;
	fptr_hc_driver_start* start_ptr = &ptr->start;
	fptr_hc_driver_stop* stop_ptr = &ptr->stop;
	fptr_hc_driver_update_device* update_device_ptr = &ptr->update_device;
	fptr_hc_driver_update_hub_device* update_hub_device_ptr = &ptr->update_hub_device;
	fptr_hc_driver_urb_dequeue* urb_dequeue_ptr = &ptr->urb_dequeue;
	fptr_hc_driver_urb_enqueue* urb_enqueue_ptr = &ptr->urb_enqueue;

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*description_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*description_ptr) {
			char* writable = (char*)*description_ptr;
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
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*product_desc_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*product_desc_ptr) {
			char* writable = (char*)*product_desc_ptr;
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
		*flags_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*hcd_priv_size_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*add_endpoint_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_add_endpoint);
	}

	{
		*address_device_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_address_device);
	}

	{
		*alloc_dev_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_alloc_dev);
	}

	{
		*alloc_streams_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_alloc_streams);
	}

	{
		*check_bandwidth_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_check_bandwidth);
	}

	{
		*disable_usb3_lpm_timeout_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_disable_usb3_lpm_timeout);
	}

	{
		*drop_endpoint_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_drop_endpoint);
	}

	{
		*enable_device_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_enable_device);
	}

	{
		*enable_usb3_lpm_timeout_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_enable_usb3_lpm_timeout);
	}

	{
		*endpoint_reset_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_endpoint_reset);
	}

	{
		*find_raw_port_number_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_find_raw_port_number);
	}

	{
		*free_dev_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_free_dev);
	}

	{
		*free_streams_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_free_streams);
	}

	{
		*get_frame_number_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_get_frame_number);
	}

	{
		*hub_control_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_hub_control);
	}

	{
		*hub_status_data_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_hub_status_data);
	}

	{
		*irq_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_irq);
	}

	{
		*reset_bandwidth_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_reset_bandwidth);
	}

	{
		*reset_device_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_reset_device);
	}

	{
		*set_usb2_hw_lpm_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_set_usb2_hw_lpm);
	}

	{
		*shutdown_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_shutdown);
	}

	{
		*start_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_start);
	}

	{
		*stop_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_stop);
	}

	{
		*update_device_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_update_device);
	}

	{
		*update_hub_device_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_update_hub_device);
	}

	{
		*urb_dequeue_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_urb_dequeue);
	}

	{
		*urb_enqueue_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, hc_driver_urb_enqueue);
	}

	{
	}
}

void caller_marshal_kernel__xhci_init_driver__over__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* ctx,
	struct xhci_driver_overrides const* ptr)
{
	unsigned int const* extra_priv_size_ptr = &ptr->extra_priv_size;

	{
		glue_pack(__pos, __msg, __ext, *extra_priv_size_ptr);
	}

}

void callee_unmarshal_kernel__xhci_init_driver__over__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* ctx,
	struct xhci_driver_overrides* ptr)
{
	unsigned int* extra_priv_size_ptr = &ptr->extra_priv_size;

	{
		*extra_priv_size_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__xhci_init_driver__over__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* ctx,
	struct xhci_driver_overrides const* ptr)
{

}

void caller_unmarshal_kernel__xhci_init_driver__over__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* ctx,
	struct xhci_driver_overrides* ptr)
{

	{
	}
}

void caller_marshal_kernel__xhci_run__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_run_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	unsigned long* const* hcd_priv_ptr = &ptr->hcd_priv;

	{
		struct xhci_hcd* __casted = (struct xhci_hcd*)*hcd_priv_ptr;
		struct xhci_hcd* const* __casted_ptr = &__casted;
		{
			__maybe_unused const void* __adjusted = *__casted_ptr;
			const ptrdiff_t __offset = (void*)__adjusted - (void*) ptr;
			if (__offset >= (sizeof(struct xhci_hcd)) || __offset < 0)
				glue_user_panic("Bounds check failed!");

			glue_pack(__pos, __msg, __ext, __offset);
			if (*__casted_ptr) {
				caller_marshal_kernel__xhci_run__xhci_hcd__in(__pos, __msg, __ext, ctx, *__casted_ptr);
			}

		}

	}

}

void callee_unmarshal_kernel__xhci_run__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_run_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	unsigned long** hcd_priv_ptr = &ptr->hcd_priv;

	{
		struct xhci_hcd* __casted = (struct xhci_hcd*)*hcd_priv_ptr;
		struct xhci_hcd** __casted_ptr = &__casted;
		{
			size_t __offset = glue_unpack(__pos, __msg, __ext, size_t);
			*__casted_ptr = (struct xhci_hcd*)(__offset + (void*) ptr);
			if (*__casted_ptr) {
				callee_unmarshal_kernel__xhci_run__xhci_hcd__in(__pos, __msg, __ext, ctx, *__casted_ptr);
			}

		}

		*hcd_priv_ptr = (unsigned long*)__casted;
	}

	{
	}
}

void callee_marshal_kernel__xhci_run__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_run_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__xhci_run__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_run_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__xhci_run__xhci_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_run_call_ctx const* ctx,
	struct xhci_hcd const* ptr)
{
	struct usb_hcd* const* shared_hcd_ptr = &ptr->shared_hcd;

	{
		__maybe_unused const void* __adjusted = *shared_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*shared_hcd_ptr) {
			caller_marshal_kernel__xhci_run__usb_hcd_shared_hcd__in(__pos, __msg, __ext, ctx, *shared_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__xhci_run__xhci_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_run_call_ctx const* ctx,
	struct xhci_hcd* ptr)
{
	struct usb_hcd** shared_hcd_ptr = &ptr->shared_hcd;

	{
		size_t __size = sizeof(struct usb_hcd);
		*shared_hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
		if (*shared_hcd_ptr) {
			callee_unmarshal_kernel__xhci_run__usb_hcd_shared_hcd__in(__pos, __msg, __ext, ctx, *shared_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__xhci_run__xhci_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_run_call_ctx const* ctx,
	struct xhci_hcd const* ptr)
{
	struct usb_hcd* const* shared_hcd_ptr = &ptr->shared_hcd;

	{
		if (*shared_hcd_ptr) {
			callee_marshal_kernel__xhci_run__usb_hcd_shared_hcd__in(__pos, __msg, __ext, ctx, *shared_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__xhci_run__xhci_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_run_call_ctx const* ctx,
	struct xhci_hcd* ptr)
{
	struct usb_hcd** shared_hcd_ptr = &ptr->shared_hcd;

	{
		if (*shared_hcd_ptr) {
			caller_unmarshal_kernel__xhci_run__usb_hcd_shared_hcd__in(__pos, __msg, __ext, ctx, *shared_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__xhci_run__usb_hcd_shared_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_run_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__xhci_run__usb_hcd_shared_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_run_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__xhci_run__usb_hcd_shared_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_run_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__xhci_run__usb_hcd_shared_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_run_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_disable_xhci_ports__pci_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_disable_xhci_ports_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__usb_disable_xhci_ports__pci_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_disable_xhci_ports_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__usb_disable_xhci_ports__pci_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_disable_xhci_ports_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__usb_disable_xhci_ports__pci_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_disable_xhci_ports_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_acpi_power_manageable__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_acpi_power_manageable_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* maxchild_ptr = &ptr->maxchild;

	{
		glue_pack(__pos, __msg, __ext, *maxchild_ptr);
	}

}

void callee_unmarshal_kernel__usb_acpi_power_manageable__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_acpi_power_manageable_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* maxchild_ptr = &ptr->maxchild;

	{
		*maxchild_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__usb_acpi_power_manageable__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_acpi_power_manageable_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__usb_acpi_power_manageable__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_acpi_power_manageable_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_acpi_set_power_state__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_acpi_set_power_state_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* maxchild_ptr = &ptr->maxchild;

	{
		glue_pack(__pos, __msg, __ext, *maxchild_ptr);
	}

}

void callee_unmarshal_kernel__usb_acpi_set_power_state__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_acpi_set_power_state_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* maxchild_ptr = &ptr->maxchild;

	{
		*maxchild_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__usb_acpi_set_power_state__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_acpi_set_power_state_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__usb_acpi_set_power_state__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_acpi_set_power_state_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_hc_died__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_bus const* self_ptr = &ptr->self;
	unsigned long const* flags_ptr = &ptr->flags;
	unsigned int __rh_registered = ptr->rh_registered;
	unsigned int const* __rh_registered_ptr = &__rh_registered;

	{
		caller_marshal_kernel__usb_hc_died__usb_bus__in(__pos, __msg, __ext, ctx, self_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__rh_registered_ptr);
	}

}

void callee_unmarshal_kernel__usb_hc_died__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_bus* self_ptr = &ptr->self;
	unsigned long* flags_ptr = &ptr->flags;
	unsigned int __rh_registered = ptr->rh_registered;
	unsigned int* __rh_registered_ptr = &__rh_registered;

	{
		callee_unmarshal_kernel__usb_hc_died__usb_bus__in(__pos, __msg, __ext, ctx, self_ptr);
	}

	{
		*flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*__rh_registered_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->rh_registered = *__rh_registered_ptr;
	}
}

void callee_marshal_kernel__usb_hc_died__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_bus const* self_ptr = &ptr->self;

	{
		callee_marshal_kernel__usb_hc_died__usb_bus__in(__pos, __msg, __ext, ctx, self_ptr);
	}

}

void caller_unmarshal_kernel__usb_hc_died__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_bus* self_ptr = &ptr->self;

	{
		caller_unmarshal_kernel__usb_hc_died__usb_bus__in(__pos, __msg, __ext, ctx, self_ptr);
	}

	{
	}
}

void caller_marshal_kernel__usb_hc_died__usb_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* ctx,
	struct usb_bus const* ptr)
{
	struct usb_device* const* root_hub_ptr = &ptr->root_hub;

	{
		__maybe_unused const void* __adjusted = *root_hub_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*root_hub_ptr) {
			caller_marshal_kernel__usb_hc_died__usb_bus_root_hub__in(__pos, __msg, __ext, ctx, *root_hub_ptr);
		}

	}

}

void callee_unmarshal_kernel__usb_hc_died__usb_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* ctx,
	struct usb_bus* ptr)
{
	struct usb_device** root_hub_ptr = &ptr->root_hub;

	{
		*root_hub_ptr = glue_unpack(__pos, __msg, __ext, struct usb_device*);
		if (*root_hub_ptr) {
			callee_unmarshal_kernel__usb_hc_died__usb_bus_root_hub__in(__pos, __msg, __ext, ctx, *root_hub_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__usb_hc_died__usb_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* ctx,
	struct usb_bus const* ptr)
{
	struct usb_device* const* root_hub_ptr = &ptr->root_hub;

	{
		if (*root_hub_ptr) {
			callee_marshal_kernel__usb_hc_died__usb_bus_root_hub__in(__pos, __msg, __ext, ctx, *root_hub_ptr);
		}

	}

}

void caller_unmarshal_kernel__usb_hc_died__usb_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* ctx,
	struct usb_bus* ptr)
{
	struct usb_device** root_hub_ptr = &ptr->root_hub;

	{
		if (*root_hub_ptr) {
			caller_unmarshal_kernel__usb_hc_died__usb_bus_root_hub__in(__pos, __msg, __ext, ctx, *root_hub_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__usb_hc_died__usb_bus_root_hub__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void callee_unmarshal_kernel__usb_hc_died__usb_bus_root_hub__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__usb_hc_died__usb_bus_root_hub__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__usb_hc_died__usb_bus_root_hub__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hc_died_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_hcd_check_unlink_urb__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__usb_hcd_check_unlink_urb__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__usb_hcd_check_unlink_urb__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__usb_hcd_check_unlink_urb__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_hcd_check_unlink_urb__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* ctx,
	struct urb const* ptr)
{
	struct usb_host_endpoint* const* ep_ptr = &ptr->ep;

	{
		__maybe_unused const void* __adjusted = *ep_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ep_ptr) {
			caller_marshal_kernel__usb_hcd_check_unlink_urb__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

}

void callee_unmarshal_kernel__usb_hcd_check_unlink_urb__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* ctx,
	struct urb* ptr)
{
	struct usb_host_endpoint** ep_ptr = &ptr->ep;

	{
		*ep_ptr = glue_unpack(__pos, __msg, __ext, struct usb_host_endpoint*);
		if (*ep_ptr) {
			callee_unmarshal_kernel__usb_hcd_check_unlink_urb__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__usb_hcd_check_unlink_urb__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* ctx,
	struct urb const* ptr)
{
	struct usb_host_endpoint* const* ep_ptr = &ptr->ep;

	{
		if (*ep_ptr) {
			callee_marshal_kernel__usb_hcd_check_unlink_urb__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

}

void caller_unmarshal_kernel__usb_hcd_check_unlink_urb__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* ctx,
	struct urb* ptr)
{
	struct usb_host_endpoint** ep_ptr = &ptr->ep;

	{
		if (*ep_ptr) {
			caller_unmarshal_kernel__usb_hcd_check_unlink_urb__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__usb_hcd_check_unlink_urb__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{

}

void callee_unmarshal_kernel__usb_hcd_check_unlink_urb__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{

	{
	}
}

void callee_marshal_kernel__usb_hcd_check_unlink_urb__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{

}

void caller_unmarshal_kernel__usb_hcd_check_unlink_urb__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_check_unlink_urb_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_hcd_giveback_urb__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__usb_hcd_giveback_urb__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__usb_hcd_giveback_urb__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__usb_hcd_giveback_urb__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_hcd_giveback_urb__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct urb const* ptr)
{
	void* const* hcpriv_ptr = &ptr->hcpriv;
	struct usb_device* const* dev_ptr = &ptr->dev;
	unsigned int const* transfer_flags_ptr = &ptr->transfer_flags;
	unsigned int const* transfer_buffer_length_ptr = &ptr->transfer_buffer_length;
	unsigned int const* actual_length_ptr = &ptr->actual_length;

	{
		(void)hcpriv_ptr;
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__usb_hcd_giveback_urb__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *transfer_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *transfer_buffer_length_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *actual_length_ptr);
	}

}

void callee_unmarshal_kernel__usb_hcd_giveback_urb__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct urb* ptr)
{
	void** hcpriv_ptr = &ptr->hcpriv;
	struct usb_device** dev_ptr = &ptr->dev;
	unsigned int* transfer_flags_ptr = &ptr->transfer_flags;
	unsigned int* transfer_buffer_length_ptr = &ptr->transfer_buffer_length;
	unsigned int* actual_length_ptr = &ptr->actual_length;

	{
		(void)hcpriv_ptr;
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct usb_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__usb_hcd_giveback_urb__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*transfer_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*transfer_buffer_length_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*actual_length_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__usb_hcd_giveback_urb__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct urb const* ptr)
{
	void* const* hcpriv_ptr = &ptr->hcpriv;
	struct usb_device* const* dev_ptr = &ptr->dev;

	{
		__maybe_unused const void* __adjusted = *hcpriv_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcpriv_ptr) {
		}

	}

	{
		if (*dev_ptr) {
			callee_marshal_kernel__usb_hcd_giveback_urb__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

}

void caller_unmarshal_kernel__usb_hcd_giveback_urb__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct urb* ptr)
{
	void** hcpriv_ptr = &ptr->hcpriv;
	struct usb_device** dev_ptr = &ptr->dev;

	{
		*hcpriv_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*hcpriv_ptr) {
		}

	}

	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__usb_hcd_giveback_urb__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__usb_hcd_giveback_urb__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	struct usb_device* const* parent_ptr = &ptr->parent;

	{
		__maybe_unused const void* __adjusted = *parent_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*parent_ptr) {
			caller_marshal_kernel__usb_hcd_giveback_urb__usb_device_parent__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

}

void callee_unmarshal_kernel__usb_hcd_giveback_urb__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct usb_device* ptr)
{
	struct usb_device** parent_ptr = &ptr->parent;

	{
		*parent_ptr = glue_unpack(__pos, __msg, __ext, struct usb_device*);
		if (*parent_ptr) {
			callee_unmarshal_kernel__usb_hcd_giveback_urb__usb_device_parent__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__usb_hcd_giveback_urb__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	struct usb_device* const* parent_ptr = &ptr->parent;

	{
		if (*parent_ptr) {
			callee_marshal_kernel__usb_hcd_giveback_urb__usb_device_parent__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

}

void caller_unmarshal_kernel__usb_hcd_giveback_urb__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct usb_device* ptr)
{
	struct usb_device** parent_ptr = &ptr->parent;

	{
		if (*parent_ptr) {
			caller_unmarshal_kernel__usb_hcd_giveback_urb__usb_device_parent__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__usb_hcd_giveback_urb__usb_device_parent__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void callee_unmarshal_kernel__usb_hcd_giveback_urb__usb_device_parent__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__usb_hcd_giveback_urb__usb_device_parent__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__usb_hcd_giveback_urb__usb_device_parent__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_hcd_is_primary_hcd__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__usb_hcd_is_primary_hcd__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__usb_hcd_is_primary_hcd__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__usb_hcd_is_primary_hcd__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_hcd_link_urb_to_ep__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	unsigned long const* flags_ptr = &ptr->flags;

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

}

void callee_unmarshal_kernel__usb_hcd_link_urb_to_ep__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	unsigned long* flags_ptr = &ptr->flags;

	{
		*flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void callee_marshal_kernel__usb_hcd_link_urb_to_ep__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__usb_hcd_link_urb_to_ep__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_hcd_link_urb_to_ep__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct urb const* ptr)
{
	struct usb_device* const* dev_ptr = &ptr->dev;
	struct usb_host_endpoint* const* ep_ptr = &ptr->ep;

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__usb_hcd_link_urb_to_ep__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ep_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ep_ptr) {
			caller_marshal_kernel__usb_hcd_link_urb_to_ep__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

}

void callee_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct urb* ptr)
{
	struct usb_device** dev_ptr = &ptr->dev;
	struct usb_host_endpoint** ep_ptr = &ptr->ep;

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct usb_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*ep_ptr = glue_unpack(__pos, __msg, __ext, struct usb_host_endpoint*);
		if (*ep_ptr) {
			callee_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__usb_hcd_link_urb_to_ep__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct urb const* ptr)
{
	struct usb_device* const* dev_ptr = &ptr->dev;
	struct usb_host_endpoint* const* ep_ptr = &ptr->ep;

	{
		if (*dev_ptr) {
			callee_marshal_kernel__usb_hcd_link_urb_to_ep__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*ep_ptr) {
			callee_marshal_kernel__usb_hcd_link_urb_to_ep__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

}

void caller_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct urb* ptr)
{
	struct usb_device** dev_ptr = &ptr->dev;
	struct usb_host_endpoint** ep_ptr = &ptr->ep;

	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*ep_ptr) {
			caller_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__usb_hcd_link_urb_to_ep__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void callee_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__usb_hcd_link_urb_to_ep__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_hcd_link_urb_to_ep__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{

}

void callee_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{

	{
	}
}

void callee_marshal_kernel__usb_hcd_link_urb_to_ep__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{

}

void caller_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_link_urb_to_ep_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_hcd_poll_rh_status__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	unsigned long const* flags_ptr = &ptr->flags;

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

}

void callee_unmarshal_kernel__usb_hcd_poll_rh_status__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	unsigned long* flags_ptr = &ptr->flags;

	{
		*flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void callee_marshal_kernel__usb_hcd_poll_rh_status__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__usb_hcd_poll_rh_status__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_hcd_unlink_urb_from_ep__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__usb_hcd_unlink_urb_from_ep__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__usb_hcd_unlink_urb_from_ep__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__usb_hcd_unlink_urb_from_ep__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_hcd_unlink_urb_from_ep__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* ctx,
	struct urb const* ptr)
{

}

void callee_unmarshal_kernel__usb_hcd_unlink_urb_from_ep__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* ctx,
	struct urb* ptr)
{

	{
	}
}

void callee_marshal_kernel__usb_hcd_unlink_urb_from_ep__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* ctx,
	struct urb const* ptr)
{

}

void caller_unmarshal_kernel__usb_hcd_unlink_urb_from_ep__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_unlink_urb_from_ep_call_ctx const* ctx,
	struct urb* ptr)
{

	{
	}
}

void caller_marshal_kernel__usb_wakeup_notification__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_wakeup_notification_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* maxchild_ptr = &ptr->maxchild;

	{
		glue_pack(__pos, __msg, __ext, *maxchild_ptr);
	}

}

void callee_unmarshal_kernel__usb_wakeup_notification__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_wakeup_notification_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* maxchild_ptr = &ptr->maxchild;

	{
		*maxchild_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__usb_wakeup_notification__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_wakeup_notification_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__usb_wakeup_notification__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_wakeup_notification_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_enable_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_enable_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_enable_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		size_t __size = sizeof(struct usb_hcd);
		*primary_hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_enable_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_enable_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_enable_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_enable_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_enable_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_enable_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_enable_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_enable_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_enable_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_enable_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* slot_id_ptr = &ptr->slot_id;

	{
		glue_pack(__pos, __msg, __ext, *slot_id_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_enable_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* slot_id_ptr = &ptr->slot_id;

	{
		*slot_id_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_enable_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_enable_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_device_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_update_hub_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_update_hub_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_update_hub_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		size_t __size = sizeof(struct usb_hcd);
		*primary_hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_update_hub_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_update_hub_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_update_hub_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_update_hub_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_update_hub_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_update_hub_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_update_hub_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_update_hub_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_update_hub_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_update_hub_device__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* slot_id_ptr = &ptr->slot_id;
	int const* maxchild_ptr = &ptr->maxchild;
	struct usb_device* const* parent_ptr = &ptr->parent;

	{
		glue_pack(__pos, __msg, __ext, *slot_id_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *maxchild_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *parent_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*parent_ptr) {
			caller_marshal_kernel__hc_driver_update_hub_device__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_update_hub_device__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* slot_id_ptr = &ptr->slot_id;
	int* maxchild_ptr = &ptr->maxchild;
	struct usb_device** parent_ptr = &ptr->parent;

	{
		*slot_id_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*maxchild_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		size_t __size = sizeof(struct usb_device);
		*parent_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_device*, __size, DEFAULT_GFP_FLAGS);
		if (*parent_ptr) {
			callee_unmarshal_kernel__hc_driver_update_hub_device__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_update_hub_device__hdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	struct usb_device* const* parent_ptr = &ptr->parent;

	{
		if (*parent_ptr) {
			callee_marshal_kernel__hc_driver_update_hub_device__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_update_hub_device__hdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_device* ptr)
{
	struct usb_device** parent_ptr = &ptr->parent;

	{
		if (*parent_ptr) {
			caller_unmarshal_kernel__hc_driver_update_hub_device__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_update_hub_device__usb_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_update_hub_device__usb_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_update_hub_device__usb_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_update_hub_device__usb_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_update_hub_device__tt__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_tt const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_update_hub_device__tt__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_tt* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_update_hub_device__tt__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_tt const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_update_hub_device__tt__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_hub_device_call_ctx const* ctx,
	struct usb_tt* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_reset_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_reset_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_reset_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*primary_hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_reset_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_reset_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_reset_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_reset_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_reset_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_reset_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_reset_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_reset_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_reset_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_reset_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* slot_id_ptr = &ptr->slot_id;
	struct usb_device* const* parent_ptr = &ptr->parent;

	{
		glue_pack(__pos, __msg, __ext, *slot_id_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *parent_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*parent_ptr) {
			caller_marshal_kernel__hc_driver_reset_device__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_reset_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* slot_id_ptr = &ptr->slot_id;
	struct usb_device** parent_ptr = &ptr->parent;

	{
		*slot_id_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		size_t __size = sizeof(struct usb_device);
		*parent_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_device*, __size, DEFAULT_GFP_FLAGS);
		if (*parent_ptr) {
			callee_unmarshal_kernel__hc_driver_reset_device__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_reset_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	struct usb_device* const* parent_ptr = &ptr->parent;

	{
		if (*parent_ptr) {
			callee_marshal_kernel__hc_driver_reset_device__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_reset_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_device* ptr)
{
	struct usb_device** parent_ptr = &ptr->parent;

	{
		if (*parent_ptr) {
			caller_unmarshal_kernel__hc_driver_reset_device__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_reset_device__usb_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_reset_device__usb_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_reset_device__usb_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_reset_device__usb_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_device_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_update_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_update_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_update_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_update_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_update_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_update_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_update_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_update_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_update_device_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_set_usb2_hw_lpm__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_set_usb2_hw_lpm__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_set_usb2_hw_lpm__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_set_usb2_hw_lpm__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_set_usb2_hw_lpm__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_set_usb2_hw_lpm__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_set_usb2_hw_lpm__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_set_usb2_hw_lpm__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_set_usb2_hw_lpm_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_enable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_enable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_enable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_enable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_enable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_enable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_enable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_enable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_enable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_disable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_disable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_disable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_disable_usb3_lpm_timeout__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_disable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_disable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_disable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_disable_usb3_lpm_timeout__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_disable_usb3_lpm_timeout_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_find_raw_port_number__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	int const* speed_ptr = &ptr->speed;
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		glue_pack(__pos, __msg, __ext, *speed_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_find_raw_port_number__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_find_raw_port_number__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	int* speed_ptr = &ptr->speed;
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*speed_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*primary_hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_find_raw_port_number__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_find_raw_port_number__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_find_raw_port_number__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_find_raw_port_number__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_find_raw_port_number__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_find_raw_port_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_find_raw_port_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_find_raw_port_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_find_raw_port_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_find_raw_port_number_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_stop__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_stop__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_stop__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*primary_hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_stop__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_stop__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_stop__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_stop__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_stop__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_stop__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_stop__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_stop__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_stop__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_stop_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_shutdown__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_shutdown__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_shutdown__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*primary_hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_shutdown__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_shutdown__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_shutdown__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_shutdown__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_shutdown__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_shutdown__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_shutdown__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_shutdown__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_shutdown__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_shutdown_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_get_frame_number__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_get_frame_number__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_get_frame_number__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*primary_hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_get_frame_number__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_get_frame_number__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_get_frame_number__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_get_frame_number__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_get_frame_number__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_get_frame_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_get_frame_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_get_frame_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_get_frame_number__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_get_frame_number_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_urb_enqueue__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_urb_enqueue__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_urb_enqueue__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*primary_hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_urb_enqueue__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_urb_enqueue__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_urb_enqueue__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_urb_enqueue__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_urb_enqueue__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_urb_enqueue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_urb_enqueue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_urb_enqueue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_urb_enqueue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_urb_enqueue__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct urb const* ptr)
{
	struct usb_device* const* dev_ptr = &ptr->dev;
	struct usb_host_endpoint* const* ep_ptr = &ptr->ep;

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__hc_driver_urb_enqueue__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ep_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ep_ptr) {
			caller_marshal_kernel__hc_driver_urb_enqueue__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_urb_enqueue__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct urb* ptr)
{
	struct usb_device** dev_ptr = &ptr->dev;
	struct usb_host_endpoint** ep_ptr = &ptr->ep;

	{
		size_t __size = sizeof(struct usb_device);
		*dev_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_device*, __size, DEFAULT_GFP_FLAGS);
		if (*dev_ptr) {
			callee_unmarshal_kernel__hc_driver_urb_enqueue__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_host_endpoint);
		*ep_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_host_endpoint*, __size, DEFAULT_GFP_FLAGS);
		if (*ep_ptr) {
			callee_unmarshal_kernel__hc_driver_urb_enqueue__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_urb_enqueue__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct urb const* ptr)
{
	struct usb_device* const* dev_ptr = &ptr->dev;
	struct usb_host_endpoint* const* ep_ptr = &ptr->ep;

	{
		if (*dev_ptr) {
			callee_marshal_kernel__hc_driver_urb_enqueue__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*ep_ptr) {
			callee_marshal_kernel__hc_driver_urb_enqueue__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_urb_enqueue__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct urb* ptr)
{
	struct usb_device** dev_ptr = &ptr->dev;
	struct usb_host_endpoint** ep_ptr = &ptr->ep;

	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__hc_driver_urb_enqueue__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*ep_ptr) {
			caller_unmarshal_kernel__hc_driver_urb_enqueue__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_urb_enqueue__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* slot_id_ptr = &ptr->slot_id;

	{
		glue_pack(__pos, __msg, __ext, *slot_id_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_urb_enqueue__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* slot_id_ptr = &ptr->slot_id;

	{
		*slot_id_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_urb_enqueue__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_urb_enqueue__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_urb_enqueue__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{
	struct usb_endpoint_descriptor const* desc_ptr = &ptr->desc;

	{
		caller_marshal_kernel__hc_driver_urb_enqueue__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_urb_enqueue__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{
	struct usb_endpoint_descriptor* desc_ptr = &ptr->desc;

	{
		callee_unmarshal_kernel__hc_driver_urb_enqueue__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_urb_enqueue__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{
	struct usb_endpoint_descriptor const* desc_ptr = &ptr->desc;

	{
		callee_marshal_kernel__hc_driver_urb_enqueue__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

}

void caller_unmarshal_kernel__hc_driver_urb_enqueue__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{
	struct usb_endpoint_descriptor* desc_ptr = &ptr->desc;

	{
		caller_unmarshal_kernel__hc_driver_urb_enqueue__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

	{
	}
}

void caller_marshal_kernel__hc_driver_urb_enqueue__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_endpoint_descriptor const* ptr)
{
	unsigned char const* bEndpointAddress_ptr = &ptr->bEndpointAddress;

	{
		glue_pack(__pos, __msg, __ext, *bEndpointAddress_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_urb_enqueue__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_endpoint_descriptor* ptr)
{
	unsigned char* bEndpointAddress_ptr = &ptr->bEndpointAddress;

	{
		*bEndpointAddress_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_urb_enqueue__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_endpoint_descriptor const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_urb_enqueue__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_enqueue_call_ctx const* ctx,
	struct usb_endpoint_descriptor* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_urb_dequeue__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_urb_dequeue__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*primary_hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_urb_dequeue__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_urb_dequeue__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*primary_hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_urb_dequeue__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_urb_dequeue__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct urb const* ptr)
{
	struct usb_device* const* dev_ptr = &ptr->dev;
	struct usb_host_endpoint* const* ep_ptr = &ptr->ep;

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__hc_driver_urb_dequeue__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ep_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ep_ptr) {
			caller_marshal_kernel__hc_driver_urb_dequeue__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_urb_dequeue__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct urb* ptr)
{
	struct usb_device** dev_ptr = &ptr->dev;
	struct usb_host_endpoint** ep_ptr = &ptr->ep;

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct usb_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__hc_driver_urb_dequeue__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*ep_ptr = glue_unpack(__pos, __msg, __ext, struct usb_host_endpoint*);
		if (*ep_ptr) {
			callee_unmarshal_kernel__hc_driver_urb_dequeue__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_urb_dequeue__urb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct urb const* ptr)
{
	struct usb_device* const* dev_ptr = &ptr->dev;
	struct usb_host_endpoint* const* ep_ptr = &ptr->ep;

	{
		if (*dev_ptr) {
			callee_marshal_kernel__hc_driver_urb_dequeue__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*ep_ptr) {
			callee_marshal_kernel__hc_driver_urb_dequeue__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_urb_dequeue__urb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct urb* ptr)
{
	struct usb_device** dev_ptr = &ptr->dev;
	struct usb_host_endpoint** ep_ptr = &ptr->ep;

	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__hc_driver_urb_dequeue__urb_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*ep_ptr) {
			caller_unmarshal_kernel__hc_driver_urb_dequeue__urb_ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_urb_dequeue__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* slot_id_ptr = &ptr->slot_id;

	{
		glue_pack(__pos, __msg, __ext, *slot_id_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_urb_dequeue__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* slot_id_ptr = &ptr->slot_id;

	{
		*slot_id_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_urb_dequeue__urb_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_urb_dequeue__urb_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_urb_dequeue__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{
	struct usb_endpoint_descriptor const* desc_ptr = &ptr->desc;

	{
		caller_marshal_kernel__hc_driver_urb_dequeue__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_urb_dequeue__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{
	struct usb_endpoint_descriptor* desc_ptr = &ptr->desc;

	{
		callee_unmarshal_kernel__hc_driver_urb_dequeue__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_urb_dequeue__urb_ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{
	struct usb_endpoint_descriptor const* desc_ptr = &ptr->desc;

	{
		callee_marshal_kernel__hc_driver_urb_dequeue__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

}

void caller_unmarshal_kernel__hc_driver_urb_dequeue__urb_ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{
	struct usb_endpoint_descriptor* desc_ptr = &ptr->desc;

	{
		caller_unmarshal_kernel__hc_driver_urb_dequeue__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

	{
	}
}

void caller_marshal_kernel__hc_driver_urb_dequeue__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_endpoint_descriptor const* ptr)
{
	unsigned char const* bEndpointAddress_ptr = &ptr->bEndpointAddress;

	{
		glue_pack(__pos, __msg, __ext, *bEndpointAddress_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_urb_dequeue__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_endpoint_descriptor* ptr)
{
	unsigned char* bEndpointAddress_ptr = &ptr->bEndpointAddress;

	{
		*bEndpointAddress_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_urb_dequeue__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_endpoint_descriptor const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_urb_dequeue__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_urb_dequeue_call_ctx const* ctx,
	struct usb_endpoint_descriptor* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_endpoint_reset__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_endpoint_reset__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_endpoint_reset__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		size_t __size = sizeof(struct usb_hcd);
		*primary_hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_endpoint_reset__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_endpoint_reset__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_endpoint_reset__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_endpoint_reset__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_endpoint_reset__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_endpoint_reset__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_endpoint_reset__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_endpoint_reset__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_endpoint_reset__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_endpoint_reset__ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{
	struct usb_endpoint_descriptor const* desc_ptr = &ptr->desc;

	{
		caller_marshal_kernel__hc_driver_endpoint_reset__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_endpoint_reset__ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{
	struct usb_endpoint_descriptor* desc_ptr = &ptr->desc;

	{
		callee_unmarshal_kernel__hc_driver_endpoint_reset__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_endpoint_reset__ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{
	struct usb_endpoint_descriptor const* desc_ptr = &ptr->desc;

	{
		callee_marshal_kernel__hc_driver_endpoint_reset__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

}

void caller_unmarshal_kernel__hc_driver_endpoint_reset__ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{
	struct usb_endpoint_descriptor* desc_ptr = &ptr->desc;

	{
		caller_unmarshal_kernel__hc_driver_endpoint_reset__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

	{
	}
}

void caller_marshal_kernel__hc_driver_endpoint_reset__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_endpoint_descriptor const* ptr)
{
	unsigned char const* bEndpointAddress_ptr = &ptr->bEndpointAddress;

	{
		glue_pack(__pos, __msg, __ext, *bEndpointAddress_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_endpoint_reset__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_endpoint_descriptor* ptr)
{
	unsigned char* bEndpointAddress_ptr = &ptr->bEndpointAddress;

	{
		*bEndpointAddress_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_endpoint_reset__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_endpoint_descriptor const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_endpoint_reset__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_endpoint_reset_call_ctx const* ctx,
	struct usb_endpoint_descriptor* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_alloc_dev__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_alloc_dev__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_alloc_dev__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		size_t __size = sizeof(struct usb_hcd);
		*primary_hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_alloc_dev__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_alloc_dev__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_alloc_dev__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_alloc_dev__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_alloc_dev__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_alloc_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_alloc_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_alloc_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_alloc_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_alloc_dev__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* slot_id_ptr = &ptr->slot_id;

	{
		glue_pack(__pos, __msg, __ext, *slot_id_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_alloc_dev__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* slot_id_ptr = &ptr->slot_id;

	{
		*slot_id_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_alloc_dev__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* slot_id_ptr = &ptr->slot_id;

	{
		glue_pack(__pos, __msg, __ext, *slot_id_ptr);
	}

}

void caller_unmarshal_kernel__hc_driver_alloc_dev__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_dev_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* slot_id_ptr = &ptr->slot_id;

	{
		*slot_id_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void caller_marshal_kernel__hc_driver_free_dev__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_free_dev__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_free_dev__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		size_t __size = sizeof(struct usb_hcd);
		*primary_hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_free_dev__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_free_dev__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_free_dev__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_free_dev__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_free_dev__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_free_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_free_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_free_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_free_dev__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_free_dev__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* slot_id_ptr = &ptr->slot_id;

	{
		glue_pack(__pos, __msg, __ext, *slot_id_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_free_dev__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* slot_id_ptr = &ptr->slot_id;

	{
		*slot_id_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_free_dev__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_free_dev__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_dev_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_alloc_streams__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_alloc_streams__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*primary_hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_alloc_streams__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_alloc_streams__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*primary_hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_alloc_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_alloc_streams__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* slot_id_ptr = &ptr->slot_id;

	{
		glue_pack(__pos, __msg, __ext, *slot_id_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_alloc_streams__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* slot_id_ptr = &ptr->slot_id;

	{
		*slot_id_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_alloc_streams__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_alloc_streams__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_alloc_streams__eps__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_alloc_streams__eps__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_alloc_streams__eps__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_alloc_streams__eps__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_alloc_streams_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_free_streams__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_free_streams__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*primary_hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_free_streams__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_free_streams__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*primary_hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_free_streams__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_free_streams__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* slot_id_ptr = &ptr->slot_id;

	{
		glue_pack(__pos, __msg, __ext, *slot_id_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_free_streams__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* slot_id_ptr = &ptr->slot_id;

	{
		*slot_id_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_free_streams__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_free_streams__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_free_streams__eps__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_free_streams__eps__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_free_streams__eps__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_free_streams__eps__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_free_streams_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_add_endpoint__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_add_endpoint__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_add_endpoint__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		size_t __size = sizeof(struct usb_hcd);
		*primary_hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_add_endpoint__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_add_endpoint__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_add_endpoint__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_add_endpoint__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_add_endpoint__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_add_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_add_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_add_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_add_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_add_endpoint__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* slot_id_ptr = &ptr->slot_id;
	struct usb_device* const* parent_ptr = &ptr->parent;

	{
		glue_pack(__pos, __msg, __ext, *slot_id_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *parent_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*parent_ptr) {
			caller_marshal_kernel__hc_driver_add_endpoint__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_add_endpoint__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* slot_id_ptr = &ptr->slot_id;
	struct usb_device** parent_ptr = &ptr->parent;

	{
		*slot_id_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		size_t __size = sizeof(struct usb_device);
		*parent_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct usb_device*, (__size), (DEFAULT_GFP_FLAGS));
		if (*parent_ptr) {
			callee_unmarshal_kernel__hc_driver_add_endpoint__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_add_endpoint__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	struct usb_device* const* parent_ptr = &ptr->parent;

	{
		if (*parent_ptr) {
			callee_marshal_kernel__hc_driver_add_endpoint__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_add_endpoint__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_device* ptr)
{
	struct usb_device** parent_ptr = &ptr->parent;

	{
		if (*parent_ptr) {
			caller_unmarshal_kernel__hc_driver_add_endpoint__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_add_endpoint__usb_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_add_endpoint__usb_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_add_endpoint__usb_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_add_endpoint__usb_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_add_endpoint__ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{
	struct usb_endpoint_descriptor const* desc_ptr = &ptr->desc;

	{
		caller_marshal_kernel__hc_driver_add_endpoint__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_add_endpoint__ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{
	struct usb_endpoint_descriptor* desc_ptr = &ptr->desc;

	{
		callee_unmarshal_kernel__hc_driver_add_endpoint__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_add_endpoint__ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{
	struct usb_endpoint_descriptor const* desc_ptr = &ptr->desc;

	{
		callee_marshal_kernel__hc_driver_add_endpoint__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

}

void caller_unmarshal_kernel__hc_driver_add_endpoint__ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{
	struct usb_endpoint_descriptor* desc_ptr = &ptr->desc;

	{
		caller_unmarshal_kernel__hc_driver_add_endpoint__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

	{
	}
}

void caller_marshal_kernel__hc_driver_add_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_endpoint_descriptor const* ptr)
{
	unsigned char const* bEndpointAddress_ptr = &ptr->bEndpointAddress;

	{
		glue_pack(__pos, __msg, __ext, *bEndpointAddress_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_add_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_endpoint_descriptor* ptr)
{
	unsigned char* bEndpointAddress_ptr = &ptr->bEndpointAddress;

	{
		*bEndpointAddress_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_add_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_endpoint_descriptor const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_add_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_add_endpoint_call_ctx const* ctx,
	struct usb_endpoint_descriptor* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_drop_endpoint__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_drop_endpoint__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_drop_endpoint__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		size_t __size = sizeof(struct usb_hcd);
		*primary_hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_drop_endpoint__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_drop_endpoint__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_drop_endpoint__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_drop_endpoint__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_drop_endpoint__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_drop_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_drop_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_drop_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_drop_endpoint__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_drop_endpoint__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* slot_id_ptr = &ptr->slot_id;

	{
		glue_pack(__pos, __msg, __ext, *slot_id_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_drop_endpoint__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* slot_id_ptr = &ptr->slot_id;

	{
		*slot_id_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_drop_endpoint__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_drop_endpoint__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_drop_endpoint__ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{
	struct usb_endpoint_descriptor const* desc_ptr = &ptr->desc;

	{
		caller_marshal_kernel__hc_driver_drop_endpoint__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_drop_endpoint__ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{
	struct usb_endpoint_descriptor* desc_ptr = &ptr->desc;

	{
		callee_unmarshal_kernel__hc_driver_drop_endpoint__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_drop_endpoint__ep__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_host_endpoint const* ptr)
{
	struct usb_endpoint_descriptor const* desc_ptr = &ptr->desc;

	{
		callee_marshal_kernel__hc_driver_drop_endpoint__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

}

void caller_unmarshal_kernel__hc_driver_drop_endpoint__ep__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_host_endpoint* ptr)
{
	struct usb_endpoint_descriptor* desc_ptr = &ptr->desc;

	{
		caller_unmarshal_kernel__hc_driver_drop_endpoint__usb_endpoint_descriptor__in(__pos, __msg, __ext, ctx, desc_ptr);
	}

	{
	}
}

void caller_marshal_kernel__hc_driver_drop_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_endpoint_descriptor const* ptr)
{
	unsigned char const* bEndpointAddress_ptr = &ptr->bEndpointAddress;

	{
		glue_pack(__pos, __msg, __ext, *bEndpointAddress_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_drop_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_endpoint_descriptor* ptr)
{
	unsigned char* bEndpointAddress_ptr = &ptr->bEndpointAddress;

	{
		*bEndpointAddress_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_drop_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_endpoint_descriptor const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_drop_endpoint__usb_endpoint_descriptor__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_drop_endpoint_call_ctx const* ctx,
	struct usb_endpoint_descriptor* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_check_bandwidth__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_check_bandwidth__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_check_bandwidth__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		size_t __size = sizeof(struct usb_hcd);
		*primary_hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_check_bandwidth__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_check_bandwidth__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_check_bandwidth__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_check_bandwidth__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_check_bandwidth__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_check_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_check_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_check_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_check_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_check_bandwidth__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	struct usb_device* const* parent_ptr = &ptr->parent;
	int const* slot_id_ptr = &ptr->slot_id;

	{
		__maybe_unused const void* __adjusted = *parent_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*parent_ptr) {
			caller_marshal_kernel__hc_driver_check_bandwidth__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *slot_id_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_check_bandwidth__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_device* ptr)
{
	struct usb_device** parent_ptr = &ptr->parent;
	int* slot_id_ptr = &ptr->slot_id;

	{
		size_t __size = sizeof(struct usb_device);
		*parent_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct usb_device*, (__size), (DEFAULT_GFP_FLAGS));
		if (*parent_ptr) {
			callee_unmarshal_kernel__hc_driver_check_bandwidth__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

	{
		*slot_id_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_check_bandwidth__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	struct usb_device* const* parent_ptr = &ptr->parent;

	{
		if (*parent_ptr) {
			callee_marshal_kernel__hc_driver_check_bandwidth__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_check_bandwidth__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_device* ptr)
{
	struct usb_device** parent_ptr = &ptr->parent;

	{
		if (*parent_ptr) {
			caller_unmarshal_kernel__hc_driver_check_bandwidth__usb_device__in(__pos, __msg, __ext, ctx, *parent_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_check_bandwidth__usb_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_check_bandwidth__usb_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_check_bandwidth__usb_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_check_bandwidth__usb_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_check_bandwidth_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_reset_bandwidth__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_reset_bandwidth__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*primary_hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_reset_bandwidth__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_reset_bandwidth__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*primary_hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_reset_bandwidth__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_reset_bandwidth__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	int const* slot_id_ptr = &ptr->slot_id;

	{
		glue_pack(__pos, __msg, __ext, *slot_id_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_reset_bandwidth__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* ctx,
	struct usb_device* ptr)
{
	int* slot_id_ptr = &ptr->slot_id;

	{
		*slot_id_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_reset_bandwidth__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_reset_bandwidth__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_bandwidth_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_address_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_address_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_address_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		size_t __size = sizeof(struct usb_hcd);
		*primary_hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_address_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_address_device__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_address_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_address_device__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_address_device__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_address_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_address_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_address_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_address_device__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_address_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_address_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_address_device__udev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_address_device__udev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_address_device_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_hub_status_data__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_hub_status_data_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	int const* speed_ptr = &ptr->speed;

	{
		glue_pack(__pos, __msg, __ext, *speed_ptr);
	}

}

void callee_unmarshal_kernel__hc_driver_hub_status_data__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_hub_status_data_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	int* speed_ptr = &ptr->speed;

	{
		*speed_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__hc_driver_hub_status_data__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_hub_status_data_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_hub_status_data__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_hub_status_data_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_irq__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_irq_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_irq__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_irq__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_irq_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		size_t __size = sizeof(struct usb_hcd);
		*primary_hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_irq__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_irq__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_irq_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_irq__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_irq__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_irq_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_irq__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_irq__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_irq_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_irq__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_irq_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_irq__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_irq_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_irq__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_irq_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_hub_control__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_bus const* self_ptr = &ptr->self;
	int const* speed_ptr = &ptr->speed;
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		caller_marshal_kernel__hc_driver_hub_control__usb_bus__in(__pos, __msg, __ext, ctx, self_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *speed_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_hub_control__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_hub_control__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_bus* self_ptr = &ptr->self;
	int* speed_ptr = &ptr->speed;
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		callee_unmarshal_kernel__hc_driver_hub_control__usb_bus__in(__pos, __msg, __ext, ctx, self_ptr);
	}

	{
		*speed_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		size_t __size = sizeof(struct usb_hcd);
		*primary_hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_hub_control__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_hub_control__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_bus const* self_ptr = &ptr->self;
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		callee_marshal_kernel__hc_driver_hub_control__usb_bus__in(__pos, __msg, __ext, ctx, self_ptr);
	}

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_hub_control__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_hub_control__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_bus* self_ptr = &ptr->self;
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		caller_unmarshal_kernel__hc_driver_hub_control__usb_bus__in(__pos, __msg, __ext, ctx, self_ptr);
	}

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_hub_control__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_hub_control__usb_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_bus const* ptr)
{
	struct usb_device* const* root_hub_ptr = &ptr->root_hub;

	{
		__maybe_unused const void* __adjusted = *root_hub_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*root_hub_ptr) {
			caller_marshal_kernel__hc_driver_hub_control__usb_bus_root_hub__in(__pos, __msg, __ext, ctx, *root_hub_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_hub_control__usb_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_bus* ptr)
{
	struct usb_device** root_hub_ptr = &ptr->root_hub;

	{
		size_t __size = sizeof(struct usb_device);
		*root_hub_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_device*, __size, DEFAULT_GFP_FLAGS);
		if (*root_hub_ptr) {
			callee_unmarshal_kernel__hc_driver_hub_control__usb_bus_root_hub__in(__pos, __msg, __ext, ctx, *root_hub_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_hub_control__usb_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_bus const* ptr)
{
	struct usb_device* const* root_hub_ptr = &ptr->root_hub;

	{
		if (*root_hub_ptr) {
			callee_marshal_kernel__hc_driver_hub_control__usb_bus_root_hub__in(__pos, __msg, __ext, ctx, *root_hub_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_hub_control__usb_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_bus* ptr)
{
	struct usb_device** root_hub_ptr = &ptr->root_hub;

	{
		if (*root_hub_ptr) {
			caller_unmarshal_kernel__hc_driver_hub_control__usb_bus_root_hub__in(__pos, __msg, __ext, ctx, *root_hub_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_hub_control__usb_bus_root_hub__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_device const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_hub_control__usb_bus_root_hub__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_device* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_hub_control__usb_bus_root_hub__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_device const* ptr)
{
	unsigned int const* speed_ptr = &ptr->speed;

	{
		glue_pack(__pos, __msg, __ext, *speed_ptr);
	}

}

void caller_unmarshal_kernel__hc_driver_hub_control__usb_bus_root_hub__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_device* ptr)
{
	unsigned int* speed_ptr = &ptr->speed;

	{
		*speed_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__hc_driver_hub_control__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_hub_control__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_hub_control__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_hub_control__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_hub_control_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_reset__usb_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_reset__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_reset__usb_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		size_t __size = sizeof(struct usb_hcd);
		*primary_hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_reset__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_reset__usb_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_reset__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_reset__usb_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_reset__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__hc_driver_reset__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_reset__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_reset__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_reset_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_reset__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_reset_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void caller_marshal_kernel__hc_driver_start__usb_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	struct usb_hcd* __primary_hcd = ptr->primary_hcd;
	struct usb_hcd* const* __primary_hcd_ptr = &__primary_hcd;

	{
		__maybe_unused const void* __adjusted = *__primary_hcd_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*__primary_hcd_ptr) {
			caller_marshal_kernel__hc_driver_start__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *__primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__hc_driver_start__usb_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd* __primary_hcd = ptr->primary_hcd;
	struct usb_hcd** __primary_hcd_ptr = &__primary_hcd;

	{
		size_t __size = sizeof(struct usb_hcd);
		*__primary_hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
		if (*__primary_hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_start__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *__primary_hcd_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__hc_driver_start__usb_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	unsigned int __uses_new_polling = ptr->uses_new_polling;
	unsigned int const* __uses_new_polling_ptr = &__uses_new_polling;
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		glue_pack(__pos, __msg, __ext, *__uses_new_polling_ptr);
	}

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__hc_driver_start__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__hc_driver_start__usb_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	unsigned int __uses_new_polling = ptr->uses_new_polling;
	unsigned int* __uses_new_polling_ptr = &__uses_new_polling;
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*__uses_new_polling_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_start__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
		ptr->uses_new_polling = *__uses_new_polling_ptr;
	}
}

void caller_marshal_kernel__hc_driver_start__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__hc_driver_start__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__hc_driver_start__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__hc_driver_start__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct hc_driver_start_call_ctx const* ctx,
	struct usb_hcd* ptr)
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

