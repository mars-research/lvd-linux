#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

unsigned long loops_per_jiffy;
unsigned long volatile jiffies;

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
		glue_pack(__pos, __msg, __ext, __adjusted);
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
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*x_ptr) {
			caller_marshal_kernel__complete__x__in(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_complete);

	*__pos = 0;
	{
		if (*x_ptr) {
			caller_unmarshal_kernel__complete__x__in(__pos, __msg, __ext, ctx, *x_ptr);
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
		glue_pack(__pos, __msg, __ext, __adjusted);
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
		glue_pack(__pos, __msg, __ext, __adjusted);
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
		glue_pack(__pos, __msg, __ext, __adjusted);
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

void timer_func_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_timer_func function_ptr = glue_unpack(__pos, __msg, __ext, fptr_timer_func);
	unsigned long data = 0;
	unsigned long* data_ptr = &data;
	
	__maybe_unused struct timer_func_call_ctx call_ctx = {data};
	__maybe_unused struct timer_func_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	function_ptr(data);

	*__pos = 0;
	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_setup_timer(struct timer_list* timer, fptr_timer_func func, unsigned long data)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list** timer_ptr = &timer;
	fptr_timer_func* func_ptr = &func;
	unsigned long* data_ptr = &data;
	
	__maybe_unused const struct lvd_setup_timer_call_ctx call_ctx = {timer, func, data};
	__maybe_unused const struct lvd_setup_timer_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *timer_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*timer_ptr) {
			caller_marshal_kernel__lvd_setup_timer__timer__io(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *func_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *data_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_lvd_setup_timer);

	*__pos = 0;
	{
		*timer_ptr = glue_unpack(__pos, __msg, __ext, struct timer_list*);
		if (*timer_ptr) {
			caller_unmarshal_kernel__lvd_setup_timer__timer__io(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
	}

	{
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
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*timer_ptr) {
			caller_marshal_kernel__mod_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *expires_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_mod_timer);

	*__pos = 0;
	{
		if (*timer_ptr) {
			caller_unmarshal_kernel__mod_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
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
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*entries_ptr) {
			size_t i, len = (ctx->maxvec);
			struct msix_entry* array = *entries_ptr;
			glue_pack(__pos, __msg, __ext, len);
			for (i = 0; i < len; ++i) {
				struct msix_entry* element = &array[i];
				caller_marshal_kernel__pci_enable_msix_range__entries__io(__pos, __msg, __ext, ctx, element);
			}

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
		*entries_ptr = glue_unpack(__pos, __msg, __ext, struct msix_entry*);
		if (*entries_ptr) {
			int i;
			struct msix_entry* array = *entries_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			for (i = 0; i < len; ++i) {
				struct msix_entry* element = &array[i];
				caller_unmarshal_kernel__pci_enable_msix_range__entries__io(__pos, __msg, __ext, ctx, element);
			}

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
		(void)id_ptr;
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

void irq_handler_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_irq_handler function_ptr = glue_unpack(__pos, __msg, __ext, fptr_irq_handler);
	int irq = 0;
	void* id = 0;
	int* irq_ptr = &irq;
	void** id_ptr = &id;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct irq_handler_call_ctx call_ctx = {irq, id};
	__maybe_unused struct irq_handler_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		//printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*id_ptr = glue_unpack(__pos, __msg, __ext, void*);
		(void)id_ptr;
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

int request_threaded_irq(unsigned int irq, fptr_irq_handler handler, fptr_thread_fn thread_fn, unsigned long irqflags, char const* devname, void* dev_id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int* irq_ptr = &irq;
	fptr_irq_handler* handler_ptr = &handler;
	unsigned long* irqflags_ptr = &irqflags;
	char const** devname_ptr = &devname;
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
		glue_pack(__pos, __msg, __ext, *irqflags_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *devname_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*devname_ptr) {
			size_t i, len;
			char const* array = *devname_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		__maybe_unused const void* __adjusted = *dev_id_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
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
		glue_pack(__pos, __msg, __ext, __adjusted);
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
		glue_pack(__pos, __msg, __ext, __adjusted);
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

void* ioremap_nocache(unsigned long long phys_addr, unsigned long size)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long long* phys_addr_ptr = &phys_addr;
	unsigned long* size_ptr = &size;
	cptr_t ioremap_cptr;
	gpa_t ioremap_gpa;
	uint64_t ioremap_len;
	void* ret = 0;
	void** ret_ptr = &ret;
	
	__maybe_unused const struct ioremap_nocache_call_ctx call_ctx = {phys_addr, size};
	__maybe_unused const struct ioremap_nocache_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
		printk("got phys_addr 0x%llx, len 0x%lx", phys_addr, size);
	}

	{
		lcd_cptr_alloc(&ioremap_cptr);
		glue_pack(__pos, __msg, __ext, cptr_val(ioremap_cptr));
	}

	{
		glue_pack(__pos, __msg, __ext, *phys_addr_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *size_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_ioremap_nocache);

	*__pos = 0;
	{
		int __ret;
		ioremap_len = glue_unpack(__pos, __msg, __ext, uint64_t);
		__ret = lcd_ioremap_phys(ioremap_cptr, ioremap_len, &ioremap_gpa);
		if (__ret) {
			LIBLCD_ERR("failed to remap bar region");
		}
		*ret_ptr = lcd_ioremap(gpa_val(ioremap_gpa), ioremap_len);
		if (!*ret_ptr) {
			LIBLCD_ERR("failed to ioremap virt");
		}
	}

	{
	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void xhci_gen_setup_with_xhci_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd* hcd = 0;
	struct xhci_hcd* xhci = 0;
	fptr_get_quirks get_quirks = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct xhci_hcd** xhci_ptr = &xhci;
	fptr_get_quirks* get_quirks_ptr = &get_quirks;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct xhci_gen_setup_with_xhci_call_ctx call_ctx = {hcd, xhci, get_quirks};
	__maybe_unused struct xhci_gen_setup_with_xhci_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(*hcd) + sizeof(struct xhci_hcd);
		*hcd_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct usb_hcd*, (__size), (DEFAULT_GFP_FLAGS));
		if (*hcd_ptr) {
			callee_unmarshal_kernel__xhci_gen_setup_with_xhci__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	if (usb_hcd_is_primary_hcd(hcd))
	{
		size_t __offset = glue_unpack(__pos, __msg, __ext, size_t);

		printk("%s:%d unpacking offset for xhci(hcd_priv) %lu\n", __func__, __LINE__, __offset);
		*xhci_ptr = (struct xhci_hcd*)(__offset + (void*) hcd);

		printk("%s:%d hcd %p | hcd->hcd_priv %p\n", __func__, __LINE__,
					hcd, hcd->hcd_priv);
		printk("%s sizeof(struct usb_hcd) %zu | sizeof(struct xhci_hcd) %zu\n",
				__func__, sizeof(struct usb_hcd), sizeof(struct xhci_hcd));

			printk("%s, hcd %p primary_hcd %p\n",
					__func__, hcd, hcd->primary_hcd);

		//hcd->primary_hcd = NULL;
			printk("%s:%d hcd %p | hcd->hcd_priv %p | xhci %p\n",
				       	__func__, __LINE__,
					hcd, hcd->hcd_priv, xhci);
		if (*xhci_ptr) {
			callee_unmarshal_kernel__xhci_gen_setup_with_xhci__xhci_hcd__in(__pos, __msg, __ext, ctx, *xhci_ptr);
		}
	} else {
		*xhci_ptr = hcd_to_xhci(hcd);
	}

  	if (0)
	{
		*get_quirks_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, get_quirks);
	}

	ret = xhci_gen_setup_with_xhci(hcd, xhci, get_quirks);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__xhci_gen_setup_with_xhci__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*xhci_ptr) {
			callee_marshal_kernel__xhci_gen_setup_with_xhci__xhci_hcd__in(__pos, __msg, __ext, ctx, *xhci_ptr);
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
	struct xhci_driver_overrides const* over = 0;
	struct hc_driver** drv_ptr = &drv;
	struct xhci_driver_overrides const** over_ptr = &over;
	
	__maybe_unused struct xhci_init_driver_call_ctx call_ctx = {drv, over};
	__maybe_unused struct xhci_init_driver_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct hc_driver);
		*drv_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct hc_driver*, (__size), (DEFAULT_GFP_FLAGS));
		if (*drv_ptr) {
			callee_unmarshal_kernel__xhci_init_driver___global_hc_driver__out(__pos, __msg, __ext, ctx, *drv_ptr);
		}

	}

	{
		size_t __size = sizeof(struct xhci_driver_overrides);
		*over_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct xhci_driver_overrides const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*over_ptr) {
			struct xhci_driver_overrides* writable = (struct xhci_driver_overrides*)*over_ptr;
			callee_unmarshal_kernel__xhci_init_driver__over__in(__pos, __msg, __ext, ctx, writable);
		}

	}

	xhci_init_driver(drv, over);

	*__pos = 0;
	{
		if (*drv_ptr) {
			callee_marshal_kernel__xhci_init_driver___global_hc_driver__out(__pos, __msg, __ext, ctx, *drv_ptr);
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
		size_t __size = sizeof(struct usb_hcd);
		*hcd_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_hcd*, __size, DEFAULT_GFP_FLAGS);
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

char const* dmi_get_system_info(int field)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* field_ptr = &field;
	char const* ret = 0;
	char const** ret_ptr = &ret;
	
	__maybe_unused const struct dmi_get_system_info_call_ctx call_ctx = {field};
	__maybe_unused const struct dmi_get_system_info_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, __msg, __ext, *field_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_dmi_get_system_info);

	*__pos = 0;
	{
	}

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*ret_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*ret_ptr) {
			char* writable = (char*)*ret_ptr;
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

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

bool usb_acpi_power_manageable(struct usb_device* hdev, int index)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_device** hdev_ptr = &hdev;
	int* index_ptr = &index;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused const struct usb_acpi_power_manageable_call_ctx call_ctx = {hdev, index};
	__maybe_unused const struct usb_acpi_power_manageable_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *hdev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*hdev_ptr) {
			caller_marshal_kernel__usb_acpi_power_manageable__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *index_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_usb_acpi_power_manageable);

	*__pos = 0;
	{
		if (*hdev_ptr) {
			caller_unmarshal_kernel__usb_acpi_power_manageable__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, bool);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int usb_acpi_set_power_state(struct usb_device* hdev, int index, bool enable)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_device** hdev_ptr = &hdev;
	int* index_ptr = &index;
	bool* enable_ptr = &enable;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct usb_acpi_set_power_state_call_ctx call_ctx = {hdev, index, enable};
	__maybe_unused const struct usb_acpi_set_power_state_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *hdev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*hdev_ptr) {
			caller_marshal_kernel__usb_acpi_set_power_state__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *index_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *enable_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_usb_acpi_set_power_state);

	*__pos = 0;
	{
		if (*hdev_ptr) {
			caller_unmarshal_kernel__usb_acpi_set_power_state__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
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

int usb_disabled(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct usb_disabled_call_ctx call_ctx = {};
	__maybe_unused const struct usb_disabled_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, __msg, RPC_ID_usb_disabled);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void usb_hc_died(struct usb_hcd* hcd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	
	__maybe_unused const struct usb_hc_died_call_ctx call_ctx = {hcd};
	__maybe_unused const struct usb_hc_died_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__usb_hc_died__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_usb_hc_died);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__usb_hc_died__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int usb_hcd_check_unlink_urb(struct usb_hcd* hcd, struct urb* urb, int status)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct urb** urb_ptr = &urb;
	int* status_ptr = &status;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct usb_hcd_check_unlink_urb_call_ctx call_ctx = {hcd, urb, status};
	__maybe_unused const struct usb_hcd_check_unlink_urb_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *urb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*urb_ptr) {
			caller_marshal_kernel__usb_hcd_check_unlink_urb__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *status_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_usb_hcd_check_unlink_urb);

	*__pos = 0;
	{
		if (*urb_ptr) {
			caller_unmarshal_kernel__usb_hcd_check_unlink_urb__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
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

void usb_hcd_giveback_urb(struct usb_hcd* hcd, struct urb* urb, int status)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct urb** urb_ptr = &urb;
	int* status_ptr = &status;
	
	__maybe_unused const struct usb_hcd_giveback_urb_call_ctx call_ctx = {hcd, urb, status};
	__maybe_unused const struct usb_hcd_giveback_urb_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__usb_hcd_giveback_urb__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *urb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*urb_ptr) {
			caller_marshal_kernel__usb_hcd_giveback_urb__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *status_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_usb_hcd_giveback_urb);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__usb_hcd_giveback_urb__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*urb_ptr) {
			caller_unmarshal_kernel__usb_hcd_giveback_urb__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

unsigned int usb_hcd_irq(int irq, void* __hcd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	void** __hcd_ptr = &__hcd;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct usb_hcd_irq_call_ctx call_ctx = {irq, __hcd};
	__maybe_unused const struct usb_hcd_irq_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *__hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*__hcd_ptr) {
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_usb_hcd_irq);

	*__pos = 0;
	{
		(void)__hcd_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int usb_hcd_is_primary_hcd(struct usb_hcd* hcd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct usb_hcd_is_primary_hcd_call_ctx call_ctx = {hcd};
	__maybe_unused const struct usb_hcd_is_primary_hcd_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		//printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__usb_hcd_is_primary_hcd__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_usb_hcd_is_primary_hcd);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__usb_hcd_is_primary_hcd__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		//printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int usb_hcd_link_urb_to_ep(struct usb_hcd* hcd, struct urb* urb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct urb** urb_ptr = &urb;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct usb_hcd_link_urb_to_ep_call_ctx call_ctx = {hcd, urb};
	__maybe_unused const struct usb_hcd_link_urb_to_ep_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__usb_hcd_link_urb_to_ep__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *urb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*urb_ptr) {
			caller_marshal_kernel__usb_hcd_link_urb_to_ep__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_usb_hcd_link_urb_to_ep);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__usb_hcd_link_urb_to_ep__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*urb_ptr) {
			caller_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
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

void usb_hcd_poll_rh_status(struct usb_hcd* hcd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	
	__maybe_unused const struct usb_hcd_poll_rh_status_call_ctx call_ctx = {hcd};
	__maybe_unused const struct usb_hcd_poll_rh_status_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__usb_hcd_poll_rh_status__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_usb_hcd_poll_rh_status);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__usb_hcd_poll_rh_status__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void usb_hcd_unlink_urb_from_ep(struct usb_hcd* hcd, struct urb* urb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct urb** urb_ptr = &urb;
	
	__maybe_unused const struct usb_hcd_unlink_urb_from_ep_call_ctx call_ctx = {hcd, urb};
	__maybe_unused const struct usb_hcd_unlink_urb_from_ep_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *urb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*urb_ptr) {
			caller_marshal_kernel__usb_hcd_unlink_urb_from_ep__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	glue_call_server(__pos, __msg, RPC_ID_usb_hcd_unlink_urb_from_ep);

	*__pos = 0;
	{
		if (*urb_ptr) {
			caller_unmarshal_kernel__usb_hcd_unlink_urb_from_ep__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void usb_wakeup_notification(struct usb_device* hdev, unsigned int portnum)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_device** hdev_ptr = &hdev;
	unsigned int* portnum_ptr = &portnum;
	
	__maybe_unused const struct usb_wakeup_notification_call_ctx call_ctx = {hdev, portnum};
	__maybe_unused const struct usb_wakeup_notification_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *hdev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hdev_ptr) {
			caller_marshal_kernel__usb_wakeup_notification__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *portnum_ptr);
	}

	glue_call_server(__pos, __msg, RPC_ID_usb_wakeup_notification);

	*__pos = 0;
	{
		if (*hdev_ptr) {
			caller_unmarshal_kernel__usb_wakeup_notification__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void hc_driver_enable_device_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_enable_device function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_enable_device);
	struct usb_hcd* hcd = 0;
	struct usb_device* udev = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_enable_device_call_ctx call_ctx = {hcd, udev};
	__maybe_unused struct hc_driver_enable_device_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_enable_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_device);
		*udev_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_device*, __size, DEFAULT_GFP_FLAGS);
		if (*udev_ptr) {
			callee_unmarshal_kernel__hc_driver_enable_device__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	ret = function_ptr(hcd, udev);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_enable_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			callee_marshal_kernel__hc_driver_enable_device__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
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

void hc_driver_update_hub_device_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_update_hub_device function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_update_hub_device);
	struct usb_hcd* hcd = 0;
	struct usb_device* hdev = 0;
	struct usb_tt* tt = 0;
	unsigned int mem_flags = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** hdev_ptr = &hdev;
	struct usb_tt** tt_ptr = &tt;
	unsigned int* mem_flags_ptr = &mem_flags;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_update_hub_device_call_ctx call_ctx = {hcd, hdev, tt, mem_flags};
	__maybe_unused struct hc_driver_update_hub_device_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_update_hub_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_device);
		*hdev_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_device*, __size, DEFAULT_GFP_FLAGS);
		if (*hdev_ptr) {
			callee_unmarshal_kernel__hc_driver_update_hub_device__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_tt);
		*tt_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_tt*, __size, DEFAULT_GFP_FLAGS);
		if (*tt_ptr) {
			callee_unmarshal_kernel__hc_driver_update_hub_device__tt__in(__pos, __msg, __ext, ctx, *tt_ptr);
		}

	}

	{
		*mem_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	ret = function_ptr(hcd, hdev, tt, mem_flags);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_update_hub_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*hdev_ptr) {
			callee_marshal_kernel__hc_driver_update_hub_device__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
		}

	}

	{
		if (*tt_ptr) {
			callee_marshal_kernel__hc_driver_update_hub_device__tt__in(__pos, __msg, __ext, ctx, *tt_ptr);
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

void hc_driver_reset_device_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_reset_device function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_reset_device);
	struct usb_hcd* hcd = 0;
	struct usb_device* udev = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_reset_device_call_ctx call_ctx = {hcd, udev};
	__maybe_unused struct hc_driver_reset_device_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_reset_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_device);
		*udev_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_device*, __size, DEFAULT_GFP_FLAGS);
		if (*udev_ptr) {
			callee_unmarshal_kernel__hc_driver_reset_device__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	ret = function_ptr(hcd, udev);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_reset_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			callee_marshal_kernel__hc_driver_reset_device__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
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

void hc_driver_update_device_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_update_device function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_update_device);
	struct usb_hcd* hcd = 0;
	struct usb_device* udev = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_update_device_call_ctx call_ctx = {hcd, udev};
	__maybe_unused struct hc_driver_update_device_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_update_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	ret = function_ptr(hcd, udev);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_update_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

void hc_driver_set_usb2_hw_lpm_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_set_usb2_hw_lpm function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_set_usb2_hw_lpm);
	struct usb_hcd* hcd = 0;
	struct usb_device* udev = 0;
	int enable = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_set_usb2_hw_lpm_call_ctx call_ctx = {hcd, udev, enable};
	__maybe_unused struct hc_driver_set_usb2_hw_lpm_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_set_usb2_hw_lpm__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	ret = function_ptr(hcd, udev, enable);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_set_usb2_hw_lpm__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

void hc_driver_enable_usb3_lpm_timeout_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_enable_usb3_lpm_timeout function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_enable_usb3_lpm_timeout);
	struct usb_hcd* hcd = 0;
	struct usb_device* udev = 0;
	unsigned int state = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_enable_usb3_lpm_timeout_call_ctx call_ctx = {hcd, udev, state};
	__maybe_unused struct hc_driver_enable_usb3_lpm_timeout_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_enable_usb3_lpm_timeout__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	ret = function_ptr(hcd, udev, state);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_enable_usb3_lpm_timeout__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

void hc_driver_disable_usb3_lpm_timeout_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_disable_usb3_lpm_timeout function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_disable_usb3_lpm_timeout);
	struct usb_hcd* hcd = 0;
	struct usb_device* udev = 0;
	unsigned int state = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_disable_usb3_lpm_timeout_call_ctx call_ctx = {hcd, udev, state};
	__maybe_unused struct hc_driver_disable_usb3_lpm_timeout_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_disable_usb3_lpm_timeout__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	ret = function_ptr(hcd, udev, state);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_disable_usb3_lpm_timeout__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

void hc_driver_find_raw_port_number_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_find_raw_port_number function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_find_raw_port_number);
	struct usb_hcd* hcd = 0;
	int port1 = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	int* port1_ptr = &port1;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_find_raw_port_number_call_ctx call_ctx = {hcd, port1};
	__maybe_unused struct hc_driver_find_raw_port_number_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_find_raw_port_number__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		*port1_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = function_ptr(hcd, port1);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_find_raw_port_number__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

void hc_driver_stop_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_stop function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_stop);
	struct usb_hcd* hcd = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	
	__maybe_unused struct hc_driver_stop_call_ctx call_ctx = {hcd};
	__maybe_unused struct hc_driver_stop_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_stop__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	function_ptr(hcd);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_stop__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void hc_driver_shutdown_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_shutdown function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_shutdown);
	struct usb_hcd* hcd = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	
	__maybe_unused struct hc_driver_shutdown_call_ctx call_ctx = {hcd};
	__maybe_unused struct hc_driver_shutdown_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_shutdown__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	function_ptr(hcd);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_shutdown__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void hc_driver_get_frame_number_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_get_frame_number function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_get_frame_number);
	struct usb_hcd* hcd = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_get_frame_number_call_ctx call_ctx = {hcd};
	__maybe_unused struct hc_driver_get_frame_number_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_get_frame_number__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	ret = function_ptr(hcd);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_get_frame_number__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

void hc_driver_urb_enqueue_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_urb_enqueue function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_urb_enqueue);
	struct usb_hcd* hcd = 0;
	struct urb* urb = 0;
	unsigned int mem_flags = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct urb** urb_ptr = &urb;
	unsigned int* mem_flags_ptr = &mem_flags;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_urb_enqueue_call_ctx call_ctx = {hcd, urb, mem_flags};
	__maybe_unused struct hc_driver_urb_enqueue_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_urb_enqueue__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		size_t __size = sizeof(struct urb);
		*urb_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct urb*, __size, DEFAULT_GFP_FLAGS);
		if (*urb_ptr) {
			callee_unmarshal_kernel__hc_driver_urb_enqueue__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	{
		*mem_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	ret = function_ptr(hcd, urb, mem_flags);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_urb_enqueue__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*urb_ptr) {
			callee_marshal_kernel__hc_driver_urb_enqueue__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
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

void hc_driver_urb_dequeue_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_urb_dequeue function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_urb_dequeue);
	struct usb_hcd* hcd = 0;
	struct urb* urb = 0;
	int status = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct urb** urb_ptr = &urb;
	int* status_ptr = &status;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_urb_dequeue_call_ctx call_ctx = {hcd, urb, status};
	__maybe_unused struct hc_driver_urb_dequeue_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_urb_dequeue__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		*urb_ptr = glue_unpack(__pos, __msg, __ext, struct urb*);
		if (*urb_ptr) {
			callee_unmarshal_kernel__hc_driver_urb_dequeue__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	{
		*status_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = function_ptr(hcd, urb, status);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_urb_dequeue__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*urb_ptr) {
			callee_marshal_kernel__hc_driver_urb_dequeue__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
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

void hc_driver_endpoint_reset_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_endpoint_reset function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_endpoint_reset);
	struct usb_hcd* hcd = 0;
	struct usb_host_endpoint* ep = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_host_endpoint** ep_ptr = &ep;
	
	__maybe_unused struct hc_driver_endpoint_reset_call_ctx call_ctx = {hcd, ep};
	__maybe_unused struct hc_driver_endpoint_reset_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_endpoint_reset__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_host_endpoint);
		*ep_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_host_endpoint*, __size, DEFAULT_GFP_FLAGS);
		if (*ep_ptr) {
			callee_unmarshal_kernel__hc_driver_endpoint_reset__ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	function_ptr(hcd, ep);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_endpoint_reset__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*ep_ptr) {
			callee_marshal_kernel__hc_driver_endpoint_reset__ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void hc_driver_alloc_dev_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_alloc_dev function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_alloc_dev);
	struct usb_hcd* hcd = 0;
	struct usb_device* udev = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_alloc_dev_call_ctx call_ctx = {hcd, udev};
	__maybe_unused struct hc_driver_alloc_dev_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_alloc_dev__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_device);
		*udev_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_device*, __size, DEFAULT_GFP_FLAGS);
		if (*udev_ptr) {
			callee_unmarshal_kernel__hc_driver_alloc_dev__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	ret = function_ptr(hcd, udev);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_alloc_dev__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			callee_marshal_kernel__hc_driver_alloc_dev__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
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

void hc_driver_free_dev_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_free_dev function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_free_dev);
	struct usb_hcd* hcd = 0;
	struct usb_device* udev = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	
	__maybe_unused struct hc_driver_free_dev_call_ctx call_ctx = {hcd, udev};
	__maybe_unused struct hc_driver_free_dev_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_free_dev__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_device);
		*udev_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_device*, __size, DEFAULT_GFP_FLAGS);
		if (*udev_ptr) {
			callee_unmarshal_kernel__hc_driver_free_dev__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	function_ptr(hcd, udev);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_free_dev__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			callee_marshal_kernel__hc_driver_free_dev__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void hc_driver_alloc_streams_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_alloc_streams function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_alloc_streams);
	struct usb_hcd* hcd = 0;
	struct usb_device* udev = 0;
	struct usb_host_endpoint** eps = 0;
	unsigned int num_eps = 0;
	unsigned int num_streams = 0;
	unsigned int mem_flags = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	struct usb_host_endpoint*** eps_ptr = &eps;
	unsigned int* num_eps_ptr = &num_eps;
	unsigned int* num_streams_ptr = &num_streams;
	unsigned int* mem_flags_ptr = &mem_flags;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_alloc_streams_call_ctx call_ctx = {hcd, udev, eps, num_eps, num_streams, mem_flags};
	__maybe_unused struct hc_driver_alloc_streams_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_alloc_streams__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_device);
		*udev_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct usb_device*, (__size), (DEFAULT_GFP_FLAGS));
		if (*udev_ptr) {
			callee_unmarshal_kernel__hc_driver_alloc_streams__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_host_endpoint*);
		*eps_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct usb_host_endpoint**, (__size), (DEFAULT_GFP_FLAGS));
		if (*eps_ptr) {
			size_t __size = sizeof(struct usb_host_endpoint);
			**eps_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct usb_host_endpoint*, (__size), (DEFAULT_GFP_FLAGS));
			if (**eps_ptr) {
				callee_unmarshal_kernel__hc_driver_alloc_streams__eps__in(__pos, __msg, __ext, ctx, **eps_ptr);
			}

		}

	}

	{
		*num_eps_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*num_streams_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mem_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	ret = function_ptr(hcd, udev, eps, num_eps, num_streams, mem_flags);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_alloc_streams__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			callee_marshal_kernel__hc_driver_alloc_streams__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	{
		if (*eps_ptr) {
			if (**eps_ptr) {
				callee_marshal_kernel__hc_driver_alloc_streams__eps__in(__pos, __msg, __ext, ctx, **eps_ptr);
			}

		}

	}

	{
	}

	{
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

void hc_driver_free_streams_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_free_streams function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_free_streams);
	struct usb_hcd* hcd = 0;
	struct usb_device* udev = 0;
	struct usb_host_endpoint** eps = 0;
	unsigned int num_eps = 0;
	unsigned int mem_flags = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	struct usb_host_endpoint*** eps_ptr = &eps;
	unsigned int* num_eps_ptr = &num_eps;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_free_streams_call_ctx call_ctx = {hcd, udev, eps, num_eps, mem_flags};
	__maybe_unused struct hc_driver_free_streams_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_free_streams__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_device);
		*udev_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct usb_device*, (__size), (DEFAULT_GFP_FLAGS));
		if (*udev_ptr) {
			callee_unmarshal_kernel__hc_driver_free_streams__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	{
		*eps_ptr = glue_unpack(__pos, __msg, __ext, struct usb_host_endpoint**);
		if (*eps_ptr) {
			**eps_ptr = glue_unpack(__pos, __msg, __ext, struct usb_host_endpoint*);
			if (**eps_ptr) {
				callee_unmarshal_kernel__hc_driver_free_streams__eps__in(__pos, __msg, __ext, ctx, **eps_ptr);
			}

		}

	}

	{
		*num_eps_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	ret = function_ptr(hcd, udev, eps, num_eps, mem_flags);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_free_streams__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			callee_marshal_kernel__hc_driver_free_streams__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	{
		if (*eps_ptr) {
			if (**eps_ptr) {
				callee_marshal_kernel__hc_driver_free_streams__eps__in(__pos, __msg, __ext, ctx, **eps_ptr);
			}

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

void hc_driver_add_endpoint_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_add_endpoint function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_add_endpoint);
	struct usb_hcd* hcd = 0;
	struct usb_device* udev = 0;
	struct usb_host_endpoint* ep = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	struct usb_host_endpoint** ep_ptr = &ep;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_add_endpoint_call_ctx call_ctx = {hcd, udev, ep};
	__maybe_unused struct hc_driver_add_endpoint_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_add_endpoint__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_device);
		*udev_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_device*, __size, DEFAULT_GFP_FLAGS);
		if (*udev_ptr) {
			callee_unmarshal_kernel__hc_driver_add_endpoint__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_host_endpoint);
		*ep_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_host_endpoint*, __size, DEFAULT_GFP_FLAGS);
		if (*ep_ptr) {
			callee_unmarshal_kernel__hc_driver_add_endpoint__ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	ret = function_ptr(hcd, udev, ep);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_add_endpoint__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			callee_marshal_kernel__hc_driver_add_endpoint__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	{
		if (*ep_ptr) {
			callee_marshal_kernel__hc_driver_add_endpoint__ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
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

void hc_driver_drop_endpoint_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_drop_endpoint function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_drop_endpoint);
	struct usb_hcd* hcd = 0;
	struct usb_device* udev = 0;
	struct usb_host_endpoint* ep = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	struct usb_host_endpoint** ep_ptr = &ep;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_drop_endpoint_call_ctx call_ctx = {hcd, udev, ep};
	__maybe_unused struct hc_driver_drop_endpoint_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_drop_endpoint__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_device);
		*udev_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_device*, __size, DEFAULT_GFP_FLAGS);
		if (*udev_ptr) {
			callee_unmarshal_kernel__hc_driver_drop_endpoint__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_host_endpoint);
		*ep_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_host_endpoint*, __size, DEFAULT_GFP_FLAGS);
		if (*ep_ptr) {
			callee_unmarshal_kernel__hc_driver_drop_endpoint__ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	ret = function_ptr(hcd, udev, ep);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_drop_endpoint__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			callee_marshal_kernel__hc_driver_drop_endpoint__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	{
		if (*ep_ptr) {
			callee_marshal_kernel__hc_driver_drop_endpoint__ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
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

void hc_driver_check_bandwidth_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_check_bandwidth function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_check_bandwidth);
	struct usb_hcd* hcd = 0;
	struct usb_device* udev = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_check_bandwidth_call_ctx call_ctx = {hcd, udev};
	__maybe_unused struct hc_driver_check_bandwidth_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_check_bandwidth__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_device);
		*udev_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_device*, __size, DEFAULT_GFP_FLAGS);
		if (*udev_ptr) {
			callee_unmarshal_kernel__hc_driver_check_bandwidth__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	ret = function_ptr(hcd, udev);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_check_bandwidth__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			callee_marshal_kernel__hc_driver_check_bandwidth__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
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

void hc_driver_reset_bandwidth_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_reset_bandwidth function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_reset_bandwidth);
	struct usb_hcd* hcd = 0;
	struct usb_device* udev = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	
	__maybe_unused struct hc_driver_reset_bandwidth_call_ctx call_ctx = {hcd, udev};
	__maybe_unused struct hc_driver_reset_bandwidth_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_reset_bandwidth__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_device);
		*udev_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct usb_device*, (__size), (DEFAULT_GFP_FLAGS));
		if (*udev_ptr) {
			callee_unmarshal_kernel__hc_driver_reset_bandwidth__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	function_ptr(hcd, udev);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_reset_bandwidth__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			callee_marshal_kernel__hc_driver_reset_bandwidth__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void hc_driver_address_device_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_address_device function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_address_device);
	struct usb_hcd* hcd = 0;
	struct usb_device* udev = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_address_device_call_ctx call_ctx = {hcd, udev};
	__maybe_unused struct hc_driver_address_device_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_address_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		size_t __size = sizeof(struct usb_device);
		*udev_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct usb_device*, __size, DEFAULT_GFP_FLAGS);
		if (*udev_ptr) {
			callee_unmarshal_kernel__hc_driver_address_device__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	ret = function_ptr(hcd, udev);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_address_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			callee_marshal_kernel__hc_driver_address_device__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
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

void hc_driver_hub_status_data_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_hub_status_data function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_hub_status_data);
	struct usb_hcd* hcd = 0;
	char* buf = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	char** buf_ptr = &buf;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_hub_status_data_call_ctx call_ctx = {hcd, buf};
	__maybe_unused struct hc_driver_hub_status_data_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_hub_status_data__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*buf_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char*, (__size), (DEFAULT_GFP_FLAGS));
		if (*buf_ptr) {
			int i;
			char* array = *buf_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	ret = function_ptr(hcd, buf);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_hub_status_data__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *buf_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*buf_ptr) {
			size_t i, len = (32);
			char* array = *buf_ptr;
			glue_pack(__pos, __msg, __ext, len);
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

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

void hc_driver_irq_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_irq function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_irq);
	struct usb_hcd* hcd = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_irq_call_ctx call_ctx = {hcd};
	__maybe_unused struct hc_driver_irq_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_irq__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	ret = function_ptr(hcd);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_irq__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

void hc_driver_hub_control_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_hub_control function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_hub_control);
	struct usb_hcd* hcd = 0;
	unsigned short typeReq = 0;
	unsigned short wValue = 0;
	unsigned short wIndex = 0;
	char* buf = 0;
	unsigned short wLength = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	unsigned short* typeReq_ptr = &typeReq;
	unsigned short* wValue_ptr = &wValue;
	unsigned short* wIndex_ptr = &wIndex;
	char** buf_ptr = &buf;
	unsigned short* wLength_ptr = &wLength;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_hub_control_call_ctx call_ctx = {hcd, typeReq, wValue, wIndex, buf, wLength};
	__maybe_unused struct hc_driver_hub_control_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_hub_control__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		*typeReq_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*wValue_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*wIndex_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*buf_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char*, (__size), (DEFAULT_GFP_FLAGS));
		if (*buf_ptr) {
			int i;
			char* array = *buf_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	{
		*wLength_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	ret = function_ptr(hcd, typeReq, wValue, wIndex, buf, wLength);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_hub_control__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
	}

	{
	}

	{
	}

	{
		__maybe_unused const void* __adjusted = *buf_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*buf_ptr) {
			size_t i, len = (wLength);
			char* array = *buf_ptr;
			glue_pack(__pos, __msg, __ext, len);
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

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

void hc_driver_reset_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_reset function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_reset);
	struct usb_hcd* hcd = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_reset_call_ctx call_ctx = {hcd};
	__maybe_unused struct hc_driver_reset_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_reset__usb_hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	ret = function_ptr(hcd);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_reset__usb_hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

void hc_driver_start_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_hc_driver_start function_ptr = glue_unpack(__pos, __msg, __ext, fptr_hc_driver_start);
	struct usb_hcd* hcd = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct hc_driver_start_call_ctx call_ctx = {hcd};
	__maybe_unused struct hc_driver_start_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__hc_driver_start__usb_hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	ret = function_ptr(hcd);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__hc_driver_start__usb_hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

unsigned long get_loops_per_jiffy(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused const struct get_loops_per_jiffy_call_ctx call_ctx = {};
	__maybe_unused const struct get_loops_per_jiffy_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, msg, RPC_ID_get_loops_per_jiffy);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

unsigned long get_jiffies(void)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused const struct get_jiffies_call_ctx call_ctx = {};
	__maybe_unused const struct get_jiffies_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_call_server(__pos, msg, RPC_ID_get_jiffies);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void __init_globals(void) {

	loops_per_jiffy = get_loops_per_jiffy();
	jiffies = get_jiffies();
}
int try_dispatch(enum RPC_ID id, struct fipc_message* __msg, struct ext_registers* __ext)
{
	switch(id) {
	case MODULE_INIT:
		glue_user_trace("MODULE_INIT");
		__init_globals();
		__module_lcd_init();
		shared_mem_init();
		break;

	case MODULE_EXIT:
		glue_user_trace("MODULE_EXIT");
		__module_lcd_exit();
		break;

	case RPC_ID_timer_func:
		glue_user_trace("timer_func\n");
		timer_func_callee(__msg, __ext);
		break;

	case RPC_ID_thread_fn:
		glue_user_trace("thread_fn\n");
		thread_fn_callee(__msg, __ext);
		break;

	case RPC_ID_irq_handler:
		//glue_user_trace("irq_handler\n");
		irq_handler_callee(__msg, __ext);
		break;

	case RPC_ID_get_quirks:
		glue_user_trace("get_quirks\n");
		get_quirks_callee(__msg, __ext);
		break;

	case RPC_ID_xhci_gen_setup_with_xhci:
		glue_user_trace("xhci_gen_setup_with_xhci\n");
		xhci_gen_setup_with_xhci_callee(__msg, __ext);
		break;

	case RPC_ID_xhci_init_driver:
		glue_user_trace("xhci_init_driver\n");
		xhci_init_driver_callee(__msg, __ext);
		break;

	case RPC_ID_xhci_run:
		glue_user_trace("xhci_run\n");
		xhci_run_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_enable_device:
		glue_user_trace("hc_driver_enable_device\n");
		hc_driver_enable_device_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_update_hub_device:
		glue_user_trace("hc_driver_update_hub_device\n");
		hc_driver_update_hub_device_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_reset_device:
		glue_user_trace("hc_driver_reset_device\n");
		hc_driver_reset_device_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_update_device:
		glue_user_trace("hc_driver_update_device\n");
		hc_driver_update_device_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_set_usb2_hw_lpm:
		glue_user_trace("hc_driver_set_usb2_hw_lpm\n");
		hc_driver_set_usb2_hw_lpm_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_enable_usb3_lpm_timeout:
		glue_user_trace("hc_driver_enable_usb3_lpm_timeout\n");
		hc_driver_enable_usb3_lpm_timeout_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_disable_usb3_lpm_timeout:
		glue_user_trace("hc_driver_disable_usb3_lpm_timeout\n");
		hc_driver_disable_usb3_lpm_timeout_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_find_raw_port_number:
		glue_user_trace("hc_driver_find_raw_port_number\n");
		hc_driver_find_raw_port_number_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_stop:
		glue_user_trace("hc_driver_stop\n");
		hc_driver_stop_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_shutdown:
		glue_user_trace("hc_driver_shutdown\n");
		hc_driver_shutdown_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_get_frame_number:
		glue_user_trace("hc_driver_get_frame_number\n");
		hc_driver_get_frame_number_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_urb_enqueue:
		glue_user_trace("hc_driver_urb_enqueue\n");
		hc_driver_urb_enqueue_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_urb_dequeue:
		glue_user_trace("hc_driver_urb_dequeue\n");
		hc_driver_urb_dequeue_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_endpoint_reset:
		glue_user_trace("hc_driver_endpoint_reset\n");
		hc_driver_endpoint_reset_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_alloc_dev:
		glue_user_trace("hc_driver_alloc_dev\n");
		hc_driver_alloc_dev_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_free_dev:
		glue_user_trace("hc_driver_free_dev\n");
		hc_driver_free_dev_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_alloc_streams:
		glue_user_trace("hc_driver_alloc_streams\n");
		hc_driver_alloc_streams_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_free_streams:
		glue_user_trace("hc_driver_free_streams\n");
		hc_driver_free_streams_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_add_endpoint:
		glue_user_trace("hc_driver_add_endpoint\n");
		hc_driver_add_endpoint_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_drop_endpoint:
		glue_user_trace("hc_driver_drop_endpoint\n");
		hc_driver_drop_endpoint_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_check_bandwidth:
		glue_user_trace("hc_driver_check_bandwidth\n");
		hc_driver_check_bandwidth_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_reset_bandwidth:
		glue_user_trace("hc_driver_reset_bandwidth\n");
		hc_driver_reset_bandwidth_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_address_device:
		glue_user_trace("hc_driver_address_device\n");
		hc_driver_address_device_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_hub_status_data:
		glue_user_trace("hc_driver_hub_status_data\n");
		hc_driver_hub_status_data_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_irq:
		glue_user_trace("hc_driver_irq\n");
		hc_driver_irq_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_hub_control:
		glue_user_trace("hc_driver_hub_control\n");
		hc_driver_hub_control_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_reset:
		glue_user_trace("hc_driver_reset\n");
		hc_driver_reset_callee(__msg, __ext);
		break;

	case RPC_ID_hc_driver_start:
		glue_user_trace("hc_driver_start\n");
		hc_driver_start_callee(__msg, __ext);
		break;

	default:
		return 0;
	}

	return 1;
}

