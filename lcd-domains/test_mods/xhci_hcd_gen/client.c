#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void add_timer(struct timer_list* timer)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list** timer_ptr = &timer;
	
	__maybe_unused const struct add_timer_call_ctx call_ctx = {timer};
	__maybe_unused const struct add_timer_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *timer_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*timer_ptr) {
			caller_marshal_kernel__add_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_add_timer);

	*__pos = 0;
	{
		if (*timer_ptr) {
			caller_unmarshal_kernel__add_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void complete(struct completion* x)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct completion** x_ptr = &x;
	
	__maybe_unused const struct complete_call_ctx call_ctx = {x};
	__maybe_unused const struct complete_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *x_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*x_ptr) {
			caller_marshal_kernel__complete__x__io(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_complete);

	*__pos = 0;
	{
		*x_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct completion*);
		if (*x_ptr) {
			caller_unmarshal_kernel__complete__x__io(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int del_timer(struct timer_list* timer)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list** timer_ptr = &timer;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct del_timer_call_ctx call_ctx = {timer};
	__maybe_unused const struct del_timer_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *timer_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*timer_ptr) {
			caller_marshal_kernel__del_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_del_timer);

	*__pos = 0;
	{
		if (*timer_ptr) {
			caller_unmarshal_kernel__del_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int del_timer_sync(struct timer_list* timer)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list** timer_ptr = &timer;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct del_timer_sync_call_ctx call_ctx = {timer};
	__maybe_unused const struct del_timer_sync_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *timer_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*timer_ptr) {
			caller_marshal_kernel__del_timer_sync__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_del_timer_sync);

	*__pos = 0;
	{
		if (*timer_ptr) {
			caller_unmarshal_kernel__del_timer_sync__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void free_irq(unsigned int irq, void* dev_id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* irq_ptr = &irq;
	void** dev_id_ptr = &dev_id;
	
	__maybe_unused const struct free_irq_call_ctx call_ctx = {irq, dev_id};
	__maybe_unused const struct free_irq_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *dev_id_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_id_ptr) {
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_free_irq);

	*__pos = 0;
	{
	}

	{
		(void)dev_id_ptr;
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void init_timer_key(struct timer_list* timer, unsigned int flags, const char* name, struct lock_class_key* key)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list** timer_ptr = &timer;
	unsigned int* flags_ptr = &flags;
	char** name_ptr = &name;
	struct lock_class_key** key_ptr = &key;
	
	__maybe_unused const struct init_timer_key_call_ctx call_ctx = {timer, flags, name, key};
	__maybe_unused const struct init_timer_key_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *timer_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*timer_ptr) {
			caller_marshal_kernel__init_timer_key__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *name_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*name_ptr) {
			glue_pack(__pos, __msg, __ext, **name_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *key_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*key_ptr) {
			caller_marshal_kernel__init_timer_key__key__in(__pos, __msg, __ext, ctx, *key_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_init_timer_key);

	*__pos = 0;
	{
		if (*timer_ptr) {
			caller_unmarshal_kernel__init_timer_key__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
	}

	{
		(void)name_ptr;
	}

	{
		if (*key_ptr) {
			caller_unmarshal_kernel__init_timer_key__key__in(__pos, __msg, __ext, ctx, *key_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int mod_timer(struct timer_list* timer, unsigned long expires)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list** timer_ptr = &timer;
	unsigned long* expires_ptr = &expires;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct mod_timer_call_ctx call_ctx = {timer, expires};
	__maybe_unused const struct mod_timer_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *timer_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*timer_ptr) {
			caller_marshal_kernel__mod_timer__timer__io(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *expires_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_mod_timer);

	*__pos = 0;
	{
		*timer_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct timer_list*);
		if (*timer_ptr) {
			caller_unmarshal_kernel__mod_timer__timer__io(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void pci_disable_msi(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_disable_msi_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_disable_msi_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_disable_msi__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_disable_msi);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_disable_msi__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_disable_msix(struct pci_dev* dev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused const struct pci_disable_msix_call_ctx call_ctx = {dev};
	__maybe_unused const struct pci_disable_msix_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_disable_msix__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_pci_disable_msix);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_disable_msix__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int pci_enable_msi_range(struct pci_dev* dev, int minvec, int maxvec)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int* minvec_ptr = &minvec;
	int* maxvec_ptr = &maxvec;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_msi_range_call_ctx call_ctx = {dev, minvec, maxvec};
	__maybe_unused const struct pci_enable_msi_range_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_msi_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *minvec_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *maxvec_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_enable_msi_range);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_msi_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pci_enable_msix_range(struct pci_dev* dev, struct msix_entry* entries, int minvec, int maxvec)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	struct msix_entry** entries_ptr = &entries;
	int* minvec_ptr = &minvec;
	int* maxvec_ptr = &maxvec;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_enable_msix_range_call_ctx call_ctx = {dev, entries, minvec, maxvec};
	__maybe_unused const struct pci_enable_msix_range_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_enable_msix_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *entries_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*entries_ptr) {
			caller_marshal_kernel__pci_enable_msix_range__entries__in(__pos, __msg, __ext, ctx, *entries_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *minvec_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *maxvec_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_enable_msix_range);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_enable_msix_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*entries_ptr) {
			caller_unmarshal_kernel__pci_enable_msix_range__entries__in(__pos, __msg, __ext, ctx, *entries_ptr);
		}

	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int pci_set_power_state(struct pci_dev* dev, int state)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** dev_ptr = &dev;
	int* state_ptr = &state;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct pci_set_power_state_call_ctx call_ctx = {dev, state};
	__maybe_unused const struct pci_set_power_state_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__pci_set_power_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_pci_set_power_state);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__pci_set_power_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void thread_fn_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_thread_fn function_ptr = glue_unpack(__pos, __msg, __ext, fptr_thread_fn);
	int irq = 0;
	void* id = 0;
	int* irq_ptr = &irq;
	void** id_ptr = &id;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct thread_fn_call_ctx call_ctx = {irq, id};
	__maybe_unused struct thread_fn_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*id_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*id_ptr) {
		}

	}

	ret = function_ptr(irq, id);

	*__pos = 0;
	{
	}

	{
		(void)id_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void handler_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_handler function_ptr = glue_unpack(__pos, __msg, __ext, fptr_handler);
	int irq = 0;
	void* id = 0;
	int* irq_ptr = &irq;
	void** id_ptr = &id;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct handler_call_ctx call_ctx = {irq, id};
	__maybe_unused struct handler_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*id_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*id_ptr) {
		}

	}

	ret = function_ptr(irq, id);

	*__pos = 0;
	{
	}

	{
		(void)id_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int request_threaded_irq(unsigned int irq, fptr_handler handler, fptr_thread_fn thread_fn, unsigned long irqflags, const char* devname, void* dev_id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* irq_ptr = &irq;
	fptr_handler* handler_ptr = &handler;
	fptr_thread_fn* thread_fn_ptr = &thread_fn;
	unsigned long* irqflags_ptr = &irqflags;
	char** devname_ptr = &devname;
	void** dev_id_ptr = &dev_id;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct request_threaded_irq_call_ctx call_ctx = {irq, handler, thread_fn, irqflags, devname, dev_id};
	__maybe_unused const struct request_threaded_irq_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *handler_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *thread_fn_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *irqflags_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *devname_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*devname_ptr) {
			glue_pack(__pos, __msg, __ext, **devname_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *dev_id_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_id_ptr) {
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_request_threaded_irq);

	*__pos = 0;
	{
	}

	{
	}

	{
	}

	{
	}

	{
		(void)devname_ptr;
	}

	{
		(void)dev_id_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void wait_for_completion(struct completion* x)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct completion** x_ptr = &x;
	
	__maybe_unused const struct wait_for_completion_call_ctx call_ctx = {x};
	__maybe_unused const struct wait_for_completion_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *x_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*x_ptr) {
			caller_marshal_kernel__wait_for_completion__x__in(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_wait_for_completion);

	*__pos = 0;
	{
		if (*x_ptr) {
			caller_unmarshal_kernel__wait_for_completion__x__in(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

unsigned long wait_for_completion_timeout(struct completion* x, unsigned long timeout)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct completion** x_ptr = &x;
	unsigned long* timeout_ptr = &timeout;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused const struct wait_for_completion_timeout_call_ctx call_ctx = {x, timeout};
	__maybe_unused const struct wait_for_completion_timeout_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *x_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*x_ptr) {
			caller_marshal_kernel__wait_for_completion_timeout__x__in(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *timeout_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_wait_for_completion_timeout);

	*__pos = 0;
	{
		if (*x_ptr) {
			caller_unmarshal_kernel__wait_for_completion_timeout__x__in(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void get_quirks_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_get_quirks function_ptr = glue_unpack(__pos, __msg, __ext, fptr_get_quirks);
	struct device* dev = 0;
	struct xhci_hcd* xhci_hcd = 0;
	struct device** dev_ptr = &dev;
	struct xhci_hcd** xhci_hcd_ptr = &xhci_hcd;
	
	__maybe_unused struct get_quirks_call_ctx call_ctx = {dev, xhci_hcd};
	__maybe_unused struct get_quirks_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__get_quirks__device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*xhci_hcd_ptr = glue_unpack(__pos, __msg, __ext, struct xhci_hcd*);
		if (*xhci_hcd_ptr) {
			callee_unmarshal_kernel__get_quirks__xhci_hcd__in(__pos, __msg, __ext, ctx, *xhci_hcd_ptr);
		}

	}

	function_ptr(dev, xhci_hcd);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__get_quirks__device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*xhci_hcd_ptr) {
			callee_marshal_kernel__get_quirks__xhci_hcd__in(__pos, __msg, __ext, ctx, *xhci_hcd_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void xhci_gen_setup_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd* hcd = 0;
	fptr_get_quirks get_quirks = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	fptr_get_quirks* get_quirks_ptr = &get_quirks;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct xhci_gen_setup_call_ctx call_ctx = {hcd, get_quirks};
	__maybe_unused struct xhci_gen_setup_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__xhci_gen_setup__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		*get_quirks_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, get_quirks);
	}

	ret = xhci_gen_setup(hcd, get_quirks);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__xhci_gen_setup__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void xhci_init_driver_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct hc_driver* drv = 0;
	struct xhci_driver_overrides* over = 0;
	struct hc_driver** drv_ptr = &drv;
	struct xhci_driver_overrides** over_ptr = &over;
	
	__maybe_unused struct xhci_init_driver_call_ctx call_ctx = {drv, over};
	__maybe_unused struct xhci_init_driver_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*drv_ptr = glue_unpack(__pos, __msg, __ext, struct hc_driver*);
		if (*drv_ptr) {
			callee_unmarshal_kernel__xhci_init_driver__drv__in(__pos, __msg, __ext, ctx, *drv_ptr);
		}

	}

	{
		*over_ptr = glue_unpack(__pos, __msg, __ext, struct xhci_driver_overrides*);
		if (*over_ptr) {
			callee_unmarshal_kernel__xhci_init_driver__over__in(__pos, __msg, __ext, ctx, *over_ptr);
		}

	}

	xhci_init_driver(drv, over);

	*__pos = 0;
	{
		if (*drv_ptr) {
			callee_marshal_kernel__xhci_init_driver__drv__in(__pos, __msg, __ext, ctx, *drv_ptr);
		}

	}

	{
		if (*over_ptr) {
			callee_marshal_kernel__xhci_init_driver__over__in(__pos, __msg, __ext, ctx, *over_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void xhci_run_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd* hcd = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct xhci_run_call_ctx call_ctx = {hcd};
	__maybe_unused struct xhci_run_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__xhci_run__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	ret = xhci_run(hcd);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__xhci_run__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void usb_disable_xhci_ports(struct pci_dev* xhci_pdev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev** xhci_pdev_ptr = &xhci_pdev;
	
	__maybe_unused const struct usb_disable_xhci_ports_call_ctx call_ctx = {xhci_pdev};
	__maybe_unused const struct usb_disable_xhci_ports_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *xhci_pdev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*xhci_pdev_ptr) {
			caller_marshal_kernel__usb_disable_xhci_ports__pci_dev__in(__pos, __msg, __ext, ctx, *xhci_pdev_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_usb_disable_xhci_ports);

	*__pos = 0;
	{
		if (*xhci_pdev_ptr) {
			caller_unmarshal_kernel__usb_disable_xhci_ports__pci_dev__in(__pos, __msg, __ext, ctx, *xhci_pdev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int try_dispatch(enum RPC_ID id, struct fipc_message* __msg, struct ext_registers* __ext)
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

	case RPC_ID_thread_fn:
		glue_user_trace("thread_fn\n");
		thread_fn_callee(__msg, __ext);
		break;

	case RPC_ID_handler:
		glue_user_trace("handler\n");
		handler_callee(__msg, __ext);
		break;

	case RPC_ID_get_quirks:
		glue_user_trace("get_quirks\n");
		get_quirks_callee(__msg, __ext);
		break;

	case RPC_ID_xhci_gen_setup:
		glue_user_trace("xhci_gen_setup\n");
		xhci_gen_setup_callee(__msg, __ext);
		break;

	case RPC_ID_xhci_init_driver:
		glue_user_trace("xhci_init_driver\n");
		xhci_init_driver_callee(__msg, __ext);
		break;

	case RPC_ID_xhci_run:
		glue_user_trace("xhci_run\n");
		xhci_run_callee(__msg, __ext);
		break;

	default:
		return 0;
	}

	return 1;
}

