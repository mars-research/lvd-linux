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
	struct hlist_node const* entry_ptr = &ptr->entry;
	unsigned long const* expires_ptr = &ptr->expires;

	{
		caller_marshal_kernel__add_timer__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

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
	struct hlist_node* entry_ptr = &ptr->entry;
	unsigned long* expires_ptr = &ptr->expires;

	{
		callee_unmarshal_kernel__add_timer__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

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
	struct hlist_node const* entry_ptr = &ptr->entry;
	unsigned long const* expires_ptr = &ptr->expires;

	{
		callee_marshal_kernel__add_timer__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

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
	struct hlist_node* entry_ptr = &ptr->entry;
	unsigned long* expires_ptr = &ptr->expires;

	{
		caller_unmarshal_kernel__add_timer__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

	{
		*expires_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void caller_marshal_kernel__add_timer__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct add_timer_call_ctx const* ctx,
	struct hlist_node const* ptr)
{

}

void callee_unmarshal_kernel__add_timer__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct add_timer_call_ctx const* ctx,
	struct hlist_node* ptr)
{

	{
	}
}

void callee_marshal_kernel__add_timer__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct add_timer_call_ctx const* ctx,
	struct hlist_node const* ptr)
{

}

void caller_unmarshal_kernel__add_timer__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct add_timer_call_ctx const* ctx,
	struct hlist_node* ptr)
{

	{
	}
}

void caller_marshal_kernel__complete__x__io(
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

void callee_unmarshal_kernel__complete__x__io(
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

void callee_marshal_kernel__complete__x__io(
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

void caller_unmarshal_kernel__complete__x__io(
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
	struct hlist_node const* entry_ptr = &ptr->entry;

	{
		caller_marshal_kernel__del_timer__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

}

void callee_unmarshal_kernel__del_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{
	struct hlist_node* entry_ptr = &ptr->entry;

	{
		callee_unmarshal_kernel__del_timer__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

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
	struct hlist_node const* entry_ptr = &ptr->entry;

	{
		callee_marshal_kernel__del_timer__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

}

void caller_unmarshal_kernel__del_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{
	struct hlist_node* entry_ptr = &ptr->entry;

	{
		caller_unmarshal_kernel__del_timer__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

	{
	}
}

void caller_marshal_kernel__del_timer__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_call_ctx const* ctx,
	struct hlist_node const* ptr)
{

}

void callee_unmarshal_kernel__del_timer__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_call_ctx const* ctx,
	struct hlist_node* ptr)
{

	{
	}
}

void callee_marshal_kernel__del_timer__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_call_ctx const* ctx,
	struct hlist_node const* ptr)
{

}

void caller_unmarshal_kernel__del_timer__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_call_ctx const* ctx,
	struct hlist_node* ptr)
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
	struct hlist_node const* entry_ptr = &ptr->entry;

	{
		caller_marshal_kernel__del_timer_sync__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

}

void callee_unmarshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* ctx,
	struct timer_list* ptr)
{
	struct hlist_node* entry_ptr = &ptr->entry;

	{
		callee_unmarshal_kernel__del_timer_sync__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

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
	struct hlist_node const* entry_ptr = &ptr->entry;

	{
		callee_marshal_kernel__del_timer_sync__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

}

void caller_unmarshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* ctx,
	struct timer_list* ptr)
{
	struct hlist_node* entry_ptr = &ptr->entry;

	{
		caller_unmarshal_kernel__del_timer_sync__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

	{
	}
}

void caller_marshal_kernel__del_timer_sync__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* ctx,
	struct hlist_node const* ptr)
{

}

void callee_unmarshal_kernel__del_timer_sync__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* ctx,
	struct hlist_node* ptr)
{

	{
	}
}

void callee_marshal_kernel__del_timer_sync__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* ctx,
	struct hlist_node const* ptr)
{

}

void caller_unmarshal_kernel__del_timer_sync__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* ctx,
	struct hlist_node* ptr)
{

	{
	}
}

void caller_marshal_kernel__init_timer_key__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* ctx,
	struct timer_list const* ptr)
{
	struct hlist_node const* entry_ptr = &ptr->entry;

	{
		caller_marshal_kernel__init_timer_key__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

}

void callee_unmarshal_kernel__init_timer_key__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* ctx,
	struct timer_list* ptr)
{
	struct hlist_node* entry_ptr = &ptr->entry;

	{
		callee_unmarshal_kernel__init_timer_key__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

	{
	}
}

void callee_marshal_kernel__init_timer_key__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* ctx,
	struct timer_list const* ptr)
{
	struct hlist_node const* entry_ptr = &ptr->entry;

	{
		callee_marshal_kernel__init_timer_key__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

}

void caller_unmarshal_kernel__init_timer_key__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* ctx,
	struct timer_list* ptr)
{
	struct hlist_node* entry_ptr = &ptr->entry;

	{
		caller_unmarshal_kernel__init_timer_key__hlist_node__in(__pos, __msg, __ext, ctx, entry_ptr);
	}

	{
	}
}

void caller_marshal_kernel__init_timer_key__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* ctx,
	struct hlist_node const* ptr)
{

}

void callee_unmarshal_kernel__init_timer_key__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* ctx,
	struct hlist_node* ptr)
{

	{
	}
}

void callee_marshal_kernel__init_timer_key__hlist_node__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* ctx,
	struct hlist_node const* ptr)
{

}

void caller_unmarshal_kernel__init_timer_key__hlist_node__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* ctx,
	struct hlist_node* ptr)
{

	{
	}
}

void caller_marshal_kernel__init_timer_key__key__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* ctx,
	struct lock_class_key const* ptr)
{

}

void callee_unmarshal_kernel__init_timer_key__key__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* ctx,
	struct lock_class_key* ptr)
{

	{
	}
}

void callee_marshal_kernel__init_timer_key__key__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* ctx,
	struct lock_class_key const* ptr)
{

}

void caller_unmarshal_kernel__init_timer_key__key__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct init_timer_key_call_ctx const* ctx,
	struct lock_class_key* ptr)
{

	{
	}
}

void caller_marshal_kernel__mod_timer__timer__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct timer_list const* ptr)
{
	struct hlist_node const* entry_ptr = &ptr->entry;
	unsigned long const* expires_ptr = &ptr->expires;

	{
		caller_marshal_kernel__mod_timer__hlist_node__io(__pos, __msg, __ext, ctx, entry_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *expires_ptr);
	}

}

void callee_unmarshal_kernel__mod_timer__timer__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{
	struct hlist_node* entry_ptr = &ptr->entry;
	unsigned long* expires_ptr = &ptr->expires;

	{
		callee_unmarshal_kernel__mod_timer__hlist_node__io(__pos, __msg, __ext, ctx, entry_ptr);
	}

	{
		*expires_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void callee_marshal_kernel__mod_timer__timer__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct timer_list const* ptr)
{
	struct hlist_node const* entry_ptr = &ptr->entry;
	unsigned long const* expires_ptr = &ptr->expires;

	{
		callee_marshal_kernel__mod_timer__hlist_node__io(__pos, __msg, __ext, ctx, entry_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *expires_ptr);
	}

}

void caller_unmarshal_kernel__mod_timer__timer__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{
	struct hlist_node* entry_ptr = &ptr->entry;
	unsigned long* expires_ptr = &ptr->expires;

	{
		caller_unmarshal_kernel__mod_timer__hlist_node__io(__pos, __msg, __ext, ctx, entry_ptr);
	}

	{
		*expires_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void caller_marshal_kernel__mod_timer__hlist_node__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct hlist_node const* ptr)
{

}

void callee_unmarshal_kernel__mod_timer__hlist_node__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct hlist_node* ptr)
{

	{
	}
}

void callee_marshal_kernel__mod_timer__hlist_node__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct hlist_node const* ptr)
{

}

void caller_unmarshal_kernel__mod_timer__hlist_node__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct hlist_node* ptr)
{

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

}

void caller_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
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

}

void caller_unmarshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_enable_msix_range__entries__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct msix_entry const* ptr)
{

}

void callee_unmarshal_kernel__pci_enable_msix_range__entries__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct msix_entry* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_enable_msix_range__entries__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct msix_entry const* ptr)
{

}

void caller_unmarshal_kernel__pci_enable_msix_range__entries__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct msix_entry* ptr)
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

void caller_marshal_kernel__xhci_gen_setup__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__xhci_gen_setup__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__xhci_gen_setup__hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_gen_setup_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{
	int const* speed_ptr = &ptr->speed;

	{
		glue_pack(__pos, __msg, __ext, *speed_ptr);
	}

}

void caller_unmarshal_kernel__xhci_gen_setup__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_gen_setup_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	int* speed_ptr = &ptr->speed;

	{
		*speed_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void caller_marshal_kernel__xhci_init_driver__drv__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* ctx,
	struct hc_driver const* ptr)
{
	unsigned long const* hcd_priv_size_ptr = &ptr->hcd_priv_size;

	{
		glue_pack(__pos, __msg, __ext, *hcd_priv_size_ptr);
	}

}

void callee_unmarshal_kernel__xhci_init_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* ctx,
	struct hc_driver* ptr)
{
	unsigned long* hcd_priv_size_ptr = &ptr->hcd_priv_size;

	{
		*hcd_priv_size_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void callee_marshal_kernel__xhci_init_driver__drv__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* ctx,
	struct hc_driver const* ptr)
{
	unsigned long const* hcd_priv_size_ptr = &ptr->hcd_priv_size;

	{
		glue_pack(__pos, __msg, __ext, *hcd_priv_size_ptr);
	}

}

void caller_unmarshal_kernel__xhci_init_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* ctx,
	struct hc_driver* ptr)
{
	unsigned long* hcd_priv_size_ptr = &ptr->hcd_priv_size;

	{
		*hcd_priv_size_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
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

}

void callee_unmarshal_kernel__xhci_init_driver__over__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_init_driver_call_ctx const* ctx,
	struct xhci_driver_overrides* ptr)
{

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

}

void callee_unmarshal_kernel__xhci_run__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct xhci_run_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

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


#ifdef LCD_ISOLATE
__attribute__((weak)) void shared_mem_init(void) {
	LIBLCD_MSG("Weak shared_mem_init does nothing! Override if you want!");
}
#else
__attribute__((weak)) void shared_mem_init_callee(struct fipc_message *__msg, struct ext_registers* __ext) {
	LIBLCD_MSG("Weak shared_mem_init_callee does nothing! Override if you want!");
}
#endif	/* LCD_ISOLATE */

