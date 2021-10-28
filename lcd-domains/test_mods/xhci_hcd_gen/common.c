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
	struct hc_driver* const* driver_ptr = &ptr->driver;

	{
		__maybe_unused const void* __adjusted = *driver_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*driver_ptr) {
			caller_marshal_kernel__usb_hcd_giveback_urb__usb_hcd_driver__in(__pos, __msg, __ext, ctx, *driver_ptr);
		}

	}

}

void callee_unmarshal_kernel__usb_hcd_giveback_urb__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct hc_driver** driver_ptr = &ptr->driver;

	{
		*driver_ptr = glue_unpack(__pos, __msg, __ext, struct hc_driver*);
		if (*driver_ptr) {
			callee_unmarshal_kernel__usb_hcd_giveback_urb__usb_hcd_driver__in(__pos, __msg, __ext, ctx, *driver_ptr);
		}

	}

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
	struct hc_driver* const* driver_ptr = &ptr->driver;

	{
		if (*driver_ptr) {
			callee_marshal_kernel__usb_hcd_giveback_urb__usb_hcd_driver__in(__pos, __msg, __ext, ctx, *driver_ptr);
		}

	}

}

void caller_unmarshal_kernel__usb_hcd_giveback_urb__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct hc_driver** driver_ptr = &ptr->driver;

	{
		if (*driver_ptr) {
			caller_unmarshal_kernel__usb_hcd_giveback_urb__usb_hcd_driver__in(__pos, __msg, __ext, ctx, *driver_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__usb_hcd_giveback_urb__usb_hcd_driver__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct hc_driver const* ptr)
{

}

void callee_unmarshal_kernel__usb_hcd_giveback_urb__usb_hcd_driver__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct hc_driver* ptr)
{

	{
	}
}

void callee_marshal_kernel__usb_hcd_giveback_urb__usb_hcd_driver__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct hc_driver const* ptr)
{

}

void caller_unmarshal_kernel__usb_hcd_giveback_urb__usb_hcd_driver__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_giveback_urb_call_ctx const* ctx,
	struct hc_driver* ptr)
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
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		__maybe_unused const void* __adjusted = *primary_hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*primary_hcd_ptr) {
			caller_marshal_kernel__usb_hcd_is_primary_hcd__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void callee_unmarshal_kernel__usb_hcd_is_primary_hcd__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		*primary_hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*primary_hcd_ptr) {
			callee_unmarshal_kernel__usb_hcd_is_primary_hcd__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

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
	struct usb_hcd* const* primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			callee_marshal_kernel__usb_hcd_is_primary_hcd__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

}

void caller_unmarshal_kernel__usb_hcd_is_primary_hcd__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct usb_hcd** primary_hcd_ptr = &ptr->primary_hcd;

	{
		if (*primary_hcd_ptr) {
			caller_unmarshal_kernel__usb_hcd_is_primary_hcd__usb_hcd_primary_hcd__in(__pos, __msg, __ext, ctx, *primary_hcd_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__usb_hcd_is_primary_hcd__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void callee_unmarshal_kernel__usb_hcd_is_primary_hcd__usb_hcd_primary_hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* ctx,
	struct usb_hcd* ptr)
{

	{
	}
}

void callee_marshal_kernel__usb_hcd_is_primary_hcd__usb_hcd_primary_hcd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_is_primary_hcd_call_ctx const* ctx,
	struct usb_hcd const* ptr)
{

}

void caller_unmarshal_kernel__usb_hcd_is_primary_hcd__usb_hcd_primary_hcd__in(
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
	struct hc_driver* const* driver_ptr = &ptr->driver;
	unsigned long const* flags_ptr = &ptr->flags;

	{
		__maybe_unused const void* __adjusted = *driver_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*driver_ptr) {
			caller_marshal_kernel__usb_hcd_poll_rh_status__usb_hcd_driver__in(__pos, __msg, __ext, ctx, *driver_ptr);
		}

	}

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
	struct hc_driver** driver_ptr = &ptr->driver;
	unsigned long* flags_ptr = &ptr->flags;

	{
		*driver_ptr = glue_unpack(__pos, __msg, __ext, struct hc_driver*);
		if (*driver_ptr) {
			callee_unmarshal_kernel__usb_hcd_poll_rh_status__usb_hcd_driver__in(__pos, __msg, __ext, ctx, *driver_ptr);
		}

	}

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
	struct hc_driver* const* driver_ptr = &ptr->driver;

	{
		if (*driver_ptr) {
			callee_marshal_kernel__usb_hcd_poll_rh_status__usb_hcd_driver__in(__pos, __msg, __ext, ctx, *driver_ptr);
		}

	}

}

void caller_unmarshal_kernel__usb_hcd_poll_rh_status__hcd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* ctx,
	struct usb_hcd* ptr)
{
	struct hc_driver** driver_ptr = &ptr->driver;

	{
		if (*driver_ptr) {
			caller_unmarshal_kernel__usb_hcd_poll_rh_status__usb_hcd_driver__in(__pos, __msg, __ext, ctx, *driver_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__usb_hcd_poll_rh_status__usb_hcd_driver__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* ctx,
	struct hc_driver const* ptr)
{

}

void callee_unmarshal_kernel__usb_hcd_poll_rh_status__usb_hcd_driver__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* ctx,
	struct hc_driver* ptr)
{

	{
	}
}

void callee_marshal_kernel__usb_hcd_poll_rh_status__usb_hcd_driver__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* ctx,
	struct hc_driver const* ptr)
{

}

void caller_unmarshal_kernel__usb_hcd_poll_rh_status__usb_hcd_driver__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct usb_hcd_poll_rh_status_call_ctx const* ctx,
	struct hc_driver* ptr)
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


#ifdef LCD_ISOLATE
__attribute__((weak)) void shared_mem_init(void) {
	LIBLCD_MSG("Weak shared_mem_init does nothing! Override if you want!");
}
#else
__attribute__((weak)) void shared_mem_init_callee(struct fipc_message *__msg, struct ext_registers* __ext) {
	LIBLCD_MSG("Weak shared_mem_init_callee does nothing! Override if you want!");
}
#endif	/* LCD_ISOLATE */

