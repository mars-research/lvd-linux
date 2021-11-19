#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void add_timer_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list* timer = 0;
	struct timer_list** timer_ptr = &timer;
	
	__maybe_unused struct add_timer_call_ctx call_ctx = {timer};
	__maybe_unused struct add_timer_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*timer_ptr = glue_unpack(__pos, __msg, __ext, struct timer_list*);
		if (*timer_ptr) {
			callee_unmarshal_kernel__add_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	add_timer(timer);

	*__pos = 0;
	{
		if (*timer_ptr) {
			callee_marshal_kernel__add_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void complete_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct completion* x = 0;
	struct completion** x_ptr = &x;
	
	__maybe_unused struct complete_call_ctx call_ctx = {x};
	__maybe_unused struct complete_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*x_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct completion*);
		if (*x_ptr) {
			callee_unmarshal_kernel__complete__x__io(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	complete(x);

	*__pos = 0;
	{
		__maybe_unused const void* __adjusted = *x_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*x_ptr) {
			callee_marshal_kernel__complete__x__io(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void del_timer_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list* timer = 0;
	struct timer_list** timer_ptr = &timer;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct del_timer_call_ctx call_ctx = {timer};
	__maybe_unused struct del_timer_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*timer_ptr = glue_unpack(__pos, __msg, __ext, struct timer_list*);
		if (*timer_ptr) {
			callee_unmarshal_kernel__del_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	ret = del_timer(timer);

	*__pos = 0;
	{
		if (*timer_ptr) {
			callee_marshal_kernel__del_timer__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
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

void del_timer_sync_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list* timer = 0;
	struct timer_list** timer_ptr = &timer;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct del_timer_sync_call_ctx call_ctx = {timer};
	__maybe_unused struct del_timer_sync_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*timer_ptr = glue_unpack(__pos, __msg, __ext, struct timer_list*);
		if (*timer_ptr) {
			callee_unmarshal_kernel__del_timer_sync__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	ret = del_timer_sync(timer);

	*__pos = 0;
	{
		if (*timer_ptr) {
			callee_marshal_kernel__del_timer_sync__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
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

void free_irq_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int irq = 0;
	void* dev_id = 0;
	unsigned int* irq_ptr = &irq;
	void** dev_id_ptr = &dev_id;
	
	__maybe_unused struct free_irq_call_ctx call_ctx = {irq, dev_id};
	__maybe_unused struct free_irq_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*dev_id_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*dev_id_ptr) {
		}

	}

	free_irq(irq, dev_id);

	*__pos = 0;
	{
	}

	{
		(void)dev_id_ptr;
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void init_timer_key_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list* timer = 0;
	unsigned int flags = 0;
	char* name = 0;
	struct lock_class_key* key = 0;
	struct timer_list** timer_ptr = &timer;
	unsigned int* flags_ptr = &flags;
	char** name_ptr = &name;
	struct lock_class_key** key_ptr = &key;
	
	__maybe_unused struct init_timer_key_call_ctx call_ctx = {timer, flags, name, key};
	__maybe_unused struct init_timer_key_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*timer_ptr = glue_unpack(__pos, __msg, __ext, struct timer_list*);
		if (*timer_ptr) {
			callee_unmarshal_kernel__init_timer_key__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		*flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*name_ptr = glue_unpack(__pos, __msg, __ext, char*);
		if (*name_ptr) {
			**name_ptr = glue_unpack(__pos, __msg, __ext, char);
		}

	}

	{
		*key_ptr = glue_unpack(__pos, __msg, __ext, struct lock_class_key*);
		if (*key_ptr) {
			callee_unmarshal_kernel__init_timer_key__key__in(__pos, __msg, __ext, ctx, *key_ptr);
		}

	}

	init_timer_key(timer, flags, name, key);

	*__pos = 0;
	{
		if (*timer_ptr) {
			callee_marshal_kernel__init_timer_key__timer__in(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
	}

	{
		(void)name_ptr;
	}

	{
		if (*key_ptr) {
			callee_marshal_kernel__init_timer_key__key__in(__pos, __msg, __ext, ctx, *key_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void mod_timer_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct timer_list* timer = 0;
	unsigned long expires = 0;
	struct timer_list** timer_ptr = &timer;
	unsigned long* expires_ptr = &expires;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct mod_timer_call_ctx call_ctx = {timer, expires};
	__maybe_unused struct mod_timer_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*timer_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct timer_list*);
		if (*timer_ptr) {
			callee_unmarshal_kernel__mod_timer__timer__io(__pos, __msg, __ext, ctx, *timer_ptr);
		}

	}

	{
		*expires_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	ret = mod_timer(timer, expires);

	*__pos = 0;
	{
		__maybe_unused const void* __adjusted = *timer_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*timer_ptr) {
			callee_marshal_kernel__mod_timer__timer__io(__pos, __msg, __ext, ctx, *timer_ptr);
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

void pci_disable_msi_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused struct pci_disable_msi_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_disable_msi_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_disable_msi__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	pci_disable_msi(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_disable_msi__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_disable_msix_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct pci_dev** dev_ptr = &dev;
	
	__maybe_unused struct pci_disable_msix_call_ctx call_ctx = {dev};
	__maybe_unused struct pci_disable_msix_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_disable_msix__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	pci_disable_msix(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_disable_msix__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void pci_enable_msi_range_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	int minvec = 0;
	int maxvec = 0;
	struct pci_dev** dev_ptr = &dev;
	int* minvec_ptr = &minvec;
	int* maxvec_ptr = &maxvec;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_enable_msi_range_call_ctx call_ctx = {dev, minvec, maxvec};
	__maybe_unused struct pci_enable_msi_range_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_enable_msi_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*minvec_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*maxvec_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = pci_enable_msi_range(dev, minvec, maxvec);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_enable_msi_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

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

void pci_enable_msix_range_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	struct msix_entry* entries = 0;
	int minvec = 0;
	int maxvec = 0;
	struct pci_dev** dev_ptr = &dev;
	struct msix_entry** entries_ptr = &entries;
	int* minvec_ptr = &minvec;
	int* maxvec_ptr = &maxvec;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_enable_msix_range_call_ctx call_ctx = {dev, entries, minvec, maxvec};
	__maybe_unused struct pci_enable_msix_range_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_enable_msix_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*entries_ptr = glue_unpack(__pos, __msg, __ext, struct msix_entry*);
		if (*entries_ptr) {
			callee_unmarshal_kernel__pci_enable_msix_range__entries__in(__pos, __msg, __ext, ctx, *entries_ptr);
		}

	}

	{
		*minvec_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*maxvec_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = pci_enable_msix_range(dev, entries, minvec, maxvec);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_enable_msix_range__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*entries_ptr) {
			callee_marshal_kernel__pci_enable_msix_range__entries__in(__pos, __msg, __ext, ctx, *entries_ptr);
		}

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

void pci_set_power_state_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* dev = 0;
	int state = 0;
	struct pci_dev** dev_ptr = &dev;
	int* state_ptr = &state;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct pci_set_power_state_call_ctx call_ctx = {dev, state};
	__maybe_unused struct pci_set_power_state_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__pci_set_power_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = pci_set_power_state(dev, state);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__pci_set_power_state__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

unsigned int trmp_impl_thread_fn(fptr_thread_fn target, int irq, void* id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* irq_ptr = &irq;
	void** id_ptr = &id;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct thread_fn_call_ctx call_ctx = {irq, id};
	__maybe_unused const struct thread_fn_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *id_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*id_ptr) {
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_thread_fn);

	*__pos = 0;
	{
	}

	{
		(void)id_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_thread_fn)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_thread_fn) trmp_thread_fn(int irq, void* id)
{
	volatile fptr_impl_thread_fn impl;
	fptr_thread_fn target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_thread_fn);
	impl = trmp_impl_thread_fn;
	return impl(target, irq, id);
}

unsigned int trmp_impl_handler(fptr_handler target, int irq, void* id)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int* irq_ptr = &irq;
	void** id_ptr = &id;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct handler_call_ctx call_ctx = {irq, id};
	__maybe_unused const struct handler_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *id_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*id_ptr) {
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_handler);

	*__pos = 0;
	{
	}

	{
		(void)id_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_handler)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_handler) trmp_handler(int irq, void* id)
{
	volatile fptr_impl_handler impl;
	fptr_handler target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_handler);
	impl = trmp_impl_handler;
	return impl(target, irq, id);
}

void request_threaded_irq_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned int irq = 0;
	fptr_handler handler = 0;
	fptr_thread_fn thread_fn = 0;
	unsigned long irqflags = 0;
	char* devname = 0;
	void* dev_id = 0;
	unsigned int* irq_ptr = &irq;
	fptr_handler* handler_ptr = &handler;
	fptr_thread_fn* thread_fn_ptr = &thread_fn;
	unsigned long* irqflags_ptr = &irqflags;
	char** devname_ptr = &devname;
	void** dev_id_ptr = &dev_id;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct request_threaded_irq_call_ctx call_ctx = {irq, handler, thread_fn, irqflags, devname, dev_id};
	__maybe_unused struct request_threaded_irq_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*handler_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, handler);
	}

	{
		*thread_fn_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, thread_fn);
	}

	{
		*irqflags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*devname_ptr = glue_unpack(__pos, __msg, __ext, char*);
		if (*devname_ptr) {
			**devname_ptr = glue_unpack(__pos, __msg, __ext, char);
		}

	}

	{
		*dev_id_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*dev_id_ptr) {
		}

	}

	ret = request_threaded_irq(irq, handler, thread_fn, irqflags, devname, dev_id);

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
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void wait_for_completion_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct completion* x = 0;
	struct completion** x_ptr = &x;
	
	__maybe_unused struct wait_for_completion_call_ctx call_ctx = {x};
	__maybe_unused struct wait_for_completion_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*x_ptr = glue_unpack(__pos, __msg, __ext, struct completion*);
		if (*x_ptr) {
			callee_unmarshal_kernel__wait_for_completion__x__in(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	wait_for_completion(x);

	*__pos = 0;
	{
		if (*x_ptr) {
			callee_marshal_kernel__wait_for_completion__x__in(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void wait_for_completion_timeout_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct completion* x = 0;
	unsigned long timeout = 0;
	struct completion** x_ptr = &x;
	unsigned long* timeout_ptr = &timeout;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused struct wait_for_completion_timeout_call_ctx call_ctx = {x, timeout};
	__maybe_unused struct wait_for_completion_timeout_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*x_ptr = glue_unpack(__pos, __msg, __ext, struct completion*);
		if (*x_ptr) {
			callee_unmarshal_kernel__wait_for_completion_timeout__x__in(__pos, __msg, __ext, ctx, *x_ptr);
		}

	}

	{
		*timeout_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	ret = wait_for_completion_timeout(x, timeout);

	*__pos = 0;
	{
		if (*x_ptr) {
			callee_marshal_kernel__wait_for_completion_timeout__x__in(__pos, __msg, __ext, ctx, *x_ptr);
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

void trmp_impl_get_quirks(fptr_get_quirks target, struct device* dev, struct xhci_hcd* xhci_hcd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct device** dev_ptr = &dev;
	struct xhci_hcd** xhci_hcd_ptr = &xhci_hcd;
	
	__maybe_unused const struct get_quirks_call_ctx call_ctx = {dev, xhci_hcd};
	__maybe_unused const struct get_quirks_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__get_quirks__device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *xhci_hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*xhci_hcd_ptr) {
			caller_marshal_kernel__get_quirks__xhci_hcd__in(__pos, __msg, __ext, ctx, *xhci_hcd_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_get_quirks);

	*__pos = 0;
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__get_quirks__device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		if (*xhci_hcd_ptr) {
			caller_unmarshal_kernel__get_quirks__xhci_hcd__in(__pos, __msg, __ext, ctx, *xhci_hcd_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_get_quirks)
void LCD_TRAMPOLINE_LINKAGE(trmp_get_quirks) trmp_get_quirks(struct device* dev, struct xhci_hcd* xhci_hcd)
{
	volatile fptr_impl_get_quirks impl;
	fptr_get_quirks target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_get_quirks);
	impl = trmp_impl_get_quirks;
	return impl(target, dev, xhci_hcd);
}

int xhci_gen_setup(struct usb_hcd* hcd, fptr_get_quirks get_quirks)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	fptr_get_quirks* get_quirks_ptr = &get_quirks;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct xhci_gen_setup_call_ctx call_ctx = {hcd, get_quirks};
	__maybe_unused const struct xhci_gen_setup_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__xhci_gen_setup__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *get_quirks_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_xhci_gen_setup);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__xhci_gen_setup__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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
EXPORT_SYMBOL(xhci_gen_setup);

void xhci_init_driver(struct hc_driver* drv, struct xhci_driver_overrides* over)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct hc_driver** drv_ptr = &drv;
	struct xhci_driver_overrides** over_ptr = &over;
	
	__maybe_unused const struct xhci_init_driver_call_ctx call_ctx = {drv, over};
	__maybe_unused const struct xhci_init_driver_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *drv_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*drv_ptr) {
			caller_marshal_kernel___global_hc_driver__out(__pos, __msg, __ext, *drv_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *over_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*over_ptr) {
			caller_marshal_kernel__xhci_init_driver__over__in(__pos, __msg, __ext, ctx, *over_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_xhci_init_driver);

	*__pos = 0;
	{
		if (*drv_ptr) {
			caller_unmarshal_kernel___global_hc_driver__out(__pos, __msg, __ext, *drv_ptr);
		}

	}

	{
		if (*over_ptr) {
			caller_unmarshal_kernel__xhci_init_driver__over__in(__pos, __msg, __ext, ctx, *over_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}
EXPORT_SYMBOL(xhci_init_driver);

int xhci_run(struct usb_hcd* hcd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct xhci_run_call_ctx call_ctx = {hcd};
	__maybe_unused const struct xhci_run_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__xhci_run__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_xhci_run);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__xhci_run__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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
EXPORT_SYMBOL(xhci_run);

void usb_disable_xhci_ports_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct pci_dev* xhci_pdev = 0;
	struct pci_dev** xhci_pdev_ptr = &xhci_pdev;
	
	__maybe_unused struct usb_disable_xhci_ports_call_ctx call_ctx = {xhci_pdev};
	__maybe_unused struct usb_disable_xhci_ports_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*xhci_pdev_ptr = glue_unpack(__pos, __msg, __ext, struct pci_dev*);
		if (*xhci_pdev_ptr) {
			callee_unmarshal_kernel__usb_disable_xhci_ports__pci_dev__in(__pos, __msg, __ext, ctx, *xhci_pdev_ptr);
		}

	}

	usb_disable_xhci_ports(xhci_pdev);

	*__pos = 0;
	{
		if (*xhci_pdev_ptr) {
			callee_marshal_kernel__usb_disable_xhci_ports__pci_dev__in(__pos, __msg, __ext, ctx, *xhci_pdev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void dmi_get_system_info_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int field = 0;
	int* field_ptr = &field;
	char const* ret = 0;
	char const** ret_ptr = &ret;
	
	__maybe_unused struct dmi_get_system_info_call_ctx call_ctx = {field};
	__maybe_unused struct dmi_get_system_info_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*field_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = dmi_get_system_info(field);

	*__pos = 0;
	{
	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ret_ptr) {
			size_t i, len;
			char const* array = *ret_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void usb_acpi_power_manageable_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_device* hdev = 0;
	int index = 0;
	struct usb_device** hdev_ptr = &hdev;
	int* index_ptr = &index;
	bool ret = 0;
	bool* ret_ptr = &ret;
	
	__maybe_unused struct usb_acpi_power_manageable_call_ctx call_ctx = {hdev, index};
	__maybe_unused struct usb_acpi_power_manageable_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hdev_ptr = glue_unpack(__pos, __msg, __ext, struct usb_device*);
		if (*hdev_ptr) {
			callee_unmarshal_kernel__usb_acpi_power_manageable__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
		}

	}

	{
		*index_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = usb_acpi_power_manageable(hdev, index);

	*__pos = 0;
	{
		if (*hdev_ptr) {
			callee_marshal_kernel__usb_acpi_power_manageable__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
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

void usb_acpi_set_power_state_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_device* hdev = 0;
	int index = 0;
	bool enable = 0;
	struct usb_device** hdev_ptr = &hdev;
	int* index_ptr = &index;
	bool* enable_ptr = &enable;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct usb_acpi_set_power_state_call_ctx call_ctx = {hdev, index, enable};
	__maybe_unused struct usb_acpi_set_power_state_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hdev_ptr = glue_unpack(__pos, __msg, __ext, struct usb_device*);
		if (*hdev_ptr) {
			callee_unmarshal_kernel__usb_acpi_set_power_state__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
		}

	}

	{
		*index_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*enable_ptr = glue_unpack(__pos, __msg, __ext, bool);
	}

	ret = usb_acpi_set_power_state(hdev, index, enable);

	*__pos = 0;
	{
		if (*hdev_ptr) {
			callee_marshal_kernel__usb_acpi_set_power_state__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
		}

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

void usb_disabled_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct usb_disabled_call_ctx call_ctx = {};
	__maybe_unused struct usb_disabled_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = usb_disabled();

	*__pos = 0;
	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void usb_hc_died_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd* hcd = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	
	__maybe_unused struct usb_hc_died_call_ctx call_ctx = {hcd};
	__maybe_unused struct usb_hc_died_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__usb_hc_died__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	usb_hc_died(hcd);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__usb_hc_died__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void usb_hcd_check_unlink_urb_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd* hcd = 0;
	struct urb* urb = 0;
	int status = 0;
	struct urb** urb_ptr = &urb;
	int* status_ptr = &status;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct usb_hcd_check_unlink_urb_call_ctx call_ctx = {hcd, urb, status};
	__maybe_unused struct usb_hcd_check_unlink_urb_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*urb_ptr = glue_unpack(__pos, __msg, __ext, struct urb*);
		if (*urb_ptr) {
			callee_unmarshal_kernel__usb_hcd_check_unlink_urb__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	{
		*status_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = usb_hcd_check_unlink_urb(hcd, urb, status);

	*__pos = 0;
	{
		if (*urb_ptr) {
			callee_marshal_kernel__usb_hcd_check_unlink_urb__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
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

void usb_hcd_giveback_urb_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd* hcd = 0;
	struct urb* urb = 0;
	int status = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct urb** urb_ptr = &urb;
	int* status_ptr = &status;
	
	__maybe_unused struct usb_hcd_giveback_urb_call_ctx call_ctx = {hcd, urb, status};
	__maybe_unused struct usb_hcd_giveback_urb_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__usb_hcd_giveback_urb__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		*urb_ptr = glue_unpack(__pos, __msg, __ext, struct urb*);
		if (*urb_ptr) {
			callee_unmarshal_kernel__usb_hcd_giveback_urb__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	{
		*status_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	usb_hcd_giveback_urb(hcd, urb, status);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__usb_hcd_giveback_urb__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*urb_ptr) {
			callee_marshal_kernel__usb_hcd_giveback_urb__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void usb_hcd_irq_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int irq = 0;
	void* __hcd = 0;
	void** __hcd_ptr = &__hcd;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct usb_hcd_irq_call_ctx call_ctx = {irq, __hcd};
	__maybe_unused struct usb_hcd_irq_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*__hcd_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*__hcd_ptr) {
		}

	}

	ret = usb_hcd_irq(irq, __hcd);

	*__pos = 0;
	{
		(void)__hcd_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void usb_hcd_is_primary_hcd_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd* hcd = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct usb_hcd_is_primary_hcd_call_ctx call_ctx = {hcd};
	__maybe_unused struct usb_hcd_is_primary_hcd_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__usb_hcd_is_primary_hcd__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	ret = usb_hcd_is_primary_hcd(hcd);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__usb_hcd_is_primary_hcd__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

void usb_hcd_link_urb_to_ep_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd* hcd = 0;
	struct urb* urb = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	struct urb** urb_ptr = &urb;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct usb_hcd_link_urb_to_ep_call_ctx call_ctx = {hcd, urb};
	__maybe_unused struct usb_hcd_link_urb_to_ep_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__usb_hcd_link_urb_to_ep__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		*urb_ptr = glue_unpack(__pos, __msg, __ext, struct urb*);
		if (*urb_ptr) {
			callee_unmarshal_kernel__usb_hcd_link_urb_to_ep__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	ret = usb_hcd_link_urb_to_ep(hcd, urb);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__usb_hcd_link_urb_to_ep__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*urb_ptr) {
			callee_marshal_kernel__usb_hcd_link_urb_to_ep__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
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

void usb_hcd_poll_rh_status_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd* hcd = 0;
	struct usb_hcd** hcd_ptr = &hcd;
	
	__maybe_unused struct usb_hcd_poll_rh_status_call_ctx call_ctx = {hcd};
	__maybe_unused struct usb_hcd_poll_rh_status_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hcd_ptr = glue_unpack(__pos, __msg, __ext, struct usb_hcd*);
		if (*hcd_ptr) {
			callee_unmarshal_kernel__usb_hcd_poll_rh_status__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	usb_hcd_poll_rh_status(hcd);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			callee_marshal_kernel__usb_hcd_poll_rh_status__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void usb_hcd_unlink_urb_from_ep_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd* hcd = 0;
	struct urb* urb = 0;
	struct urb** urb_ptr = &urb;
	
	__maybe_unused struct usb_hcd_unlink_urb_from_ep_call_ctx call_ctx = {hcd, urb};
	__maybe_unused struct usb_hcd_unlink_urb_from_ep_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*urb_ptr = glue_unpack(__pos, __msg, __ext, struct urb*);
		if (*urb_ptr) {
			callee_unmarshal_kernel__usb_hcd_unlink_urb_from_ep__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	usb_hcd_unlink_urb_from_ep(hcd, urb);

	*__pos = 0;
	{
		if (*urb_ptr) {
			callee_marshal_kernel__usb_hcd_unlink_urb_from_ep__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void usb_wakeup_notification_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_device* hdev = 0;
	unsigned int portnum = 0;
	struct usb_device** hdev_ptr = &hdev;
	unsigned int* portnum_ptr = &portnum;
	
	__maybe_unused struct usb_wakeup_notification_call_ctx call_ctx = {hdev, portnum};
	__maybe_unused struct usb_wakeup_notification_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*hdev_ptr = glue_unpack(__pos, __msg, __ext, struct usb_device*);
		if (*hdev_ptr) {
			callee_unmarshal_kernel__usb_wakeup_notification__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
		}

	}

	{
		*portnum_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	usb_wakeup_notification(hdev, portnum);

	*__pos = 0;
	{
		if (*hdev_ptr) {
			callee_marshal_kernel__usb_wakeup_notification__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
		}

	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int trmp_impl_hc_driver_start(fptr_hc_driver_start target, struct usb_hcd* hcd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_start_call_ctx call_ctx = {hcd};
	__maybe_unused const struct hc_driver_start_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_start__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_start);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_start__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_start)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_start) trmp_hc_driver_start(struct usb_hcd* hcd)
{
	volatile fptr_impl_hc_driver_start impl;
	fptr_hc_driver_start target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_start);
	impl = trmp_impl_hc_driver_start;
	return impl(target, hcd);
}

int trmp_impl_hc_driver_enable_device(fptr_hc_driver_enable_device target, struct usb_hcd* hcd, struct usb_device* udev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_enable_device_call_ctx call_ctx = {hcd, udev};
	__maybe_unused const struct hc_driver_enable_device_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_enable_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *udev_ptr;
	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_enable_device);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_enable_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			caller_unmarshal_kernel__hc_driver_enable_device__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_enable_device)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_enable_device) trmp_hc_driver_enable_device(struct usb_hcd* hcd, struct usb_device* udev)
{
	volatile fptr_impl_hc_driver_enable_device impl;
	fptr_hc_driver_enable_device target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_enable_device);
	impl = trmp_impl_hc_driver_enable_device;
	return impl(target, hcd, udev);
}

int trmp_impl_hc_driver_update_hub_device(fptr_hc_driver_update_hub_device target, struct usb_hcd* hcd, struct usb_device* hdev, struct usb_tt* tt, unsigned int mem_flags)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** hdev_ptr = &hdev;
	struct usb_tt** tt_ptr = &tt;
	unsigned int* mem_flags_ptr = &mem_flags;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_update_hub_device_call_ctx call_ctx = {hcd, hdev, tt, mem_flags};
	__maybe_unused const struct hc_driver_update_hub_device_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_update_hub_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *hdev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hdev_ptr) {
			caller_marshal_kernel__hc_driver_update_hub_device__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *tt_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*tt_ptr) {
			caller_marshal_kernel__hc_driver_update_hub_device__tt__in(__pos, __msg, __ext, ctx, *tt_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *mem_flags_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_update_hub_device);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_update_hub_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*hdev_ptr) {
			caller_unmarshal_kernel__hc_driver_update_hub_device__hdev__in(__pos, __msg, __ext, ctx, *hdev_ptr);
		}

	}

	{
		if (*tt_ptr) {
			caller_unmarshal_kernel__hc_driver_update_hub_device__tt__in(__pos, __msg, __ext, ctx, *tt_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_update_hub_device)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_update_hub_device) trmp_hc_driver_update_hub_device(struct usb_hcd* hcd, struct usb_device* hdev, struct usb_tt* tt, unsigned int mem_flags)
{
	volatile fptr_impl_hc_driver_update_hub_device impl;
	fptr_hc_driver_update_hub_device target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_update_hub_device);
	impl = trmp_impl_hc_driver_update_hub_device;
	return impl(target, hcd, hdev, tt, mem_flags);
}

int trmp_impl_hc_driver_reset_device(fptr_hc_driver_reset_device target, struct usb_hcd* hcd, struct usb_device* udev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_reset_device_call_ctx call_ctx = {hcd, udev};
	__maybe_unused const struct hc_driver_reset_device_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_reset_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *udev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*udev_ptr) {
			caller_marshal_kernel__hc_driver_reset_device__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_reset_device);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_reset_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			caller_unmarshal_kernel__hc_driver_reset_device__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_reset_device)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_reset_device) trmp_hc_driver_reset_device(struct usb_hcd* hcd, struct usb_device* udev)
{
	volatile fptr_impl_hc_driver_reset_device impl;
	fptr_hc_driver_reset_device target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_reset_device);
	impl = trmp_impl_hc_driver_reset_device;
	return impl(target, hcd, udev);
}

int trmp_impl_hc_driver_update_device(fptr_hc_driver_update_device target, struct usb_hcd* hcd, struct usb_device* udev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_update_device_call_ctx call_ctx = {hcd, udev};
	__maybe_unused const struct hc_driver_update_device_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_update_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_update_device);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_update_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_update_device)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_update_device) trmp_hc_driver_update_device(struct usb_hcd* hcd, struct usb_device* udev)
{
	volatile fptr_impl_hc_driver_update_device impl;
	fptr_hc_driver_update_device target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_update_device);
	impl = trmp_impl_hc_driver_update_device;
	return impl(target, hcd, udev);
}

int trmp_impl_hc_driver_set_usb2_hw_lpm(fptr_hc_driver_set_usb2_hw_lpm target, struct usb_hcd* hcd, struct usb_device* udev, int enable)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_set_usb2_hw_lpm_call_ctx call_ctx = {hcd, udev, enable};
	__maybe_unused const struct hc_driver_set_usb2_hw_lpm_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_set_usb2_hw_lpm__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_set_usb2_hw_lpm);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_set_usb2_hw_lpm__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_set_usb2_hw_lpm)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_set_usb2_hw_lpm) trmp_hc_driver_set_usb2_hw_lpm(struct usb_hcd* hcd, struct usb_device* udev, int enable)
{
	volatile fptr_impl_hc_driver_set_usb2_hw_lpm impl;
	fptr_hc_driver_set_usb2_hw_lpm target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_set_usb2_hw_lpm);
	impl = trmp_impl_hc_driver_set_usb2_hw_lpm;
	return impl(target, hcd, udev, enable);
}

int trmp_impl_hc_driver_enable_usb3_lpm_timeout(fptr_hc_driver_enable_usb3_lpm_timeout target, struct usb_hcd* hcd, struct usb_device* udev, unsigned int state)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_enable_usb3_lpm_timeout_call_ctx call_ctx = {hcd, udev, state};
	__maybe_unused const struct hc_driver_enable_usb3_lpm_timeout_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_enable_usb3_lpm_timeout__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_enable_usb3_lpm_timeout);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_enable_usb3_lpm_timeout__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_enable_usb3_lpm_timeout)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_enable_usb3_lpm_timeout) trmp_hc_driver_enable_usb3_lpm_timeout(struct usb_hcd* hcd, struct usb_device* udev, unsigned int state)
{
	volatile fptr_impl_hc_driver_enable_usb3_lpm_timeout impl;
	fptr_hc_driver_enable_usb3_lpm_timeout target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_enable_usb3_lpm_timeout);
	impl = trmp_impl_hc_driver_enable_usb3_lpm_timeout;
	return impl(target, hcd, udev, state);
}

int trmp_impl_hc_driver_disable_usb3_lpm_timeout(fptr_hc_driver_disable_usb3_lpm_timeout target, struct usb_hcd* hcd, struct usb_device* udev, unsigned int state)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_disable_usb3_lpm_timeout_call_ctx call_ctx = {hcd, udev, state};
	__maybe_unused const struct hc_driver_disable_usb3_lpm_timeout_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_disable_usb3_lpm_timeout__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_disable_usb3_lpm_timeout);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_disable_usb3_lpm_timeout__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_disable_usb3_lpm_timeout)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_disable_usb3_lpm_timeout) trmp_hc_driver_disable_usb3_lpm_timeout(struct usb_hcd* hcd, struct usb_device* udev, unsigned int state)
{
	volatile fptr_impl_hc_driver_disable_usb3_lpm_timeout impl;
	fptr_hc_driver_disable_usb3_lpm_timeout target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_disable_usb3_lpm_timeout);
	impl = trmp_impl_hc_driver_disable_usb3_lpm_timeout;
	return impl(target, hcd, udev, state);
}

int trmp_impl_hc_driver_find_raw_port_number(fptr_hc_driver_find_raw_port_number target, struct usb_hcd* hcd, int port1)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	int* port1_ptr = &port1;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_find_raw_port_number_call_ctx call_ctx = {hcd, port1};
	__maybe_unused const struct hc_driver_find_raw_port_number_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_find_raw_port_number__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *port1_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_find_raw_port_number);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_find_raw_port_number__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_find_raw_port_number)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_find_raw_port_number) trmp_hc_driver_find_raw_port_number(struct usb_hcd* hcd, int port1)
{
	volatile fptr_impl_hc_driver_find_raw_port_number impl;
	fptr_hc_driver_find_raw_port_number target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_find_raw_port_number);
	impl = trmp_impl_hc_driver_find_raw_port_number;
	return impl(target, hcd, port1);
}

void trmp_impl_hc_driver_stop(fptr_hc_driver_stop target, struct usb_hcd* hcd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	
	__maybe_unused const struct hc_driver_stop_call_ctx call_ctx = {hcd};
	__maybe_unused const struct hc_driver_stop_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_stop__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_stop);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_stop__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_hc_driver_stop)
void LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_stop) trmp_hc_driver_stop(struct usb_hcd* hcd)
{
	volatile fptr_impl_hc_driver_stop impl;
	fptr_hc_driver_stop target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_stop);
	impl = trmp_impl_hc_driver_stop;
	return impl(target, hcd);
}

void trmp_impl_hc_driver_shutdown(fptr_hc_driver_shutdown target, struct usb_hcd* hcd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	
	__maybe_unused const struct hc_driver_shutdown_call_ctx call_ctx = {hcd};
	__maybe_unused const struct hc_driver_shutdown_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_shutdown__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_shutdown);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_shutdown__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_hc_driver_shutdown)
void LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_shutdown) trmp_hc_driver_shutdown(struct usb_hcd* hcd)
{
	volatile fptr_impl_hc_driver_shutdown impl;
	fptr_hc_driver_shutdown target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_shutdown);
	impl = trmp_impl_hc_driver_shutdown;
	return impl(target, hcd);
}

int trmp_impl_hc_driver_get_frame_number(fptr_hc_driver_get_frame_number target, struct usb_hcd* hcd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_get_frame_number_call_ctx call_ctx = {hcd};
	__maybe_unused const struct hc_driver_get_frame_number_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_get_frame_number__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_get_frame_number);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_get_frame_number__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_get_frame_number)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_get_frame_number) trmp_hc_driver_get_frame_number(struct usb_hcd* hcd)
{
	volatile fptr_impl_hc_driver_get_frame_number impl;
	fptr_hc_driver_get_frame_number target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_get_frame_number);
	impl = trmp_impl_hc_driver_get_frame_number;
	return impl(target, hcd);
}

int trmp_impl_hc_driver_urb_enqueue(fptr_hc_driver_urb_enqueue target, struct usb_hcd* hcd, struct urb* urb, unsigned int mem_flags)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct urb** urb_ptr = &urb;
	unsigned int* mem_flags_ptr = &mem_flags;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_urb_enqueue_call_ctx call_ctx = {hcd, urb, mem_flags};
	__maybe_unused const struct hc_driver_urb_enqueue_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_urb_enqueue__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *urb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*urb_ptr) {
			caller_marshal_kernel__hc_driver_urb_enqueue__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *mem_flags_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_urb_enqueue);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_urb_enqueue__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*urb_ptr) {
			caller_unmarshal_kernel__hc_driver_urb_enqueue__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_urb_enqueue)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_urb_enqueue) trmp_hc_driver_urb_enqueue(struct usb_hcd* hcd, struct urb* urb, unsigned int mem_flags)
{
	volatile fptr_impl_hc_driver_urb_enqueue impl;
	fptr_hc_driver_urb_enqueue target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_urb_enqueue);
	impl = trmp_impl_hc_driver_urb_enqueue;
	return impl(target, hcd, urb, mem_flags);
}

int trmp_impl_hc_driver_urb_dequeue(fptr_hc_driver_urb_dequeue target, struct usb_hcd* hcd, struct urb* urb, int status)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct urb** urb_ptr = &urb;
	int* status_ptr = &status;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_urb_dequeue_call_ctx call_ctx = {hcd, urb, status};
	__maybe_unused const struct hc_driver_urb_dequeue_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_urb_dequeue__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *urb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*urb_ptr) {
			caller_marshal_kernel__hc_driver_urb_dequeue__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *status_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_urb_dequeue);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_urb_dequeue__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*urb_ptr) {
			caller_unmarshal_kernel__hc_driver_urb_dequeue__urb__in(__pos, __msg, __ext, ctx, *urb_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_urb_dequeue)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_urb_dequeue) trmp_hc_driver_urb_dequeue(struct usb_hcd* hcd, struct urb* urb, int status)
{
	volatile fptr_impl_hc_driver_urb_dequeue impl;
	fptr_hc_driver_urb_dequeue target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_urb_dequeue);
	impl = trmp_impl_hc_driver_urb_dequeue;
	return impl(target, hcd, urb, status);
}

void trmp_impl_hc_driver_endpoint_reset(fptr_hc_driver_endpoint_reset target, struct usb_hcd* hcd, struct usb_host_endpoint* ep)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_host_endpoint** ep_ptr = &ep;
	
	__maybe_unused const struct hc_driver_endpoint_reset_call_ctx call_ctx = {hcd, ep};
	__maybe_unused const struct hc_driver_endpoint_reset_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_endpoint_reset__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ep_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ep_ptr) {
			caller_marshal_kernel__hc_driver_endpoint_reset__ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_endpoint_reset);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_endpoint_reset__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*ep_ptr) {
			caller_unmarshal_kernel__hc_driver_endpoint_reset__ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_hc_driver_endpoint_reset)
void LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_endpoint_reset) trmp_hc_driver_endpoint_reset(struct usb_hcd* hcd, struct usb_host_endpoint* ep)
{
	volatile fptr_impl_hc_driver_endpoint_reset impl;
	fptr_hc_driver_endpoint_reset target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_endpoint_reset);
	impl = trmp_impl_hc_driver_endpoint_reset;
	return impl(target, hcd, ep);
}

int trmp_impl_hc_driver_alloc_dev(fptr_hc_driver_alloc_dev target, struct usb_hcd* hcd, struct usb_device* udev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_alloc_dev_call_ctx call_ctx = {hcd, udev};
	__maybe_unused const struct hc_driver_alloc_dev_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_alloc_dev__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *udev_ptr;
	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_alloc_dev);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_alloc_dev__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			caller_unmarshal_kernel__hc_driver_alloc_dev__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_alloc_dev)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_alloc_dev) trmp_hc_driver_alloc_dev(struct usb_hcd* hcd, struct usb_device* udev)
{
	volatile fptr_impl_hc_driver_alloc_dev impl;
	fptr_hc_driver_alloc_dev target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_alloc_dev);
	impl = trmp_impl_hc_driver_alloc_dev;
	return impl(target, hcd, udev);
}

void trmp_impl_hc_driver_free_dev(fptr_hc_driver_free_dev target, struct usb_hcd* hcd, struct usb_device* udev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	
	__maybe_unused const struct hc_driver_free_dev_call_ctx call_ctx = {hcd, udev};
	__maybe_unused const struct hc_driver_free_dev_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_free_dev__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *udev_ptr;
	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_free_dev);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_free_dev__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			caller_unmarshal_kernel__hc_driver_free_dev__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_hc_driver_free_dev)
void LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_free_dev) trmp_hc_driver_free_dev(struct usb_hcd* hcd, struct usb_device* udev)
{
	volatile fptr_impl_hc_driver_free_dev impl;
	fptr_hc_driver_free_dev target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_free_dev);
	impl = trmp_impl_hc_driver_free_dev;
	return impl(target, hcd, udev);
}

int trmp_impl_hc_driver_alloc_streams(fptr_hc_driver_alloc_streams target, struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint** eps, unsigned int num_eps, unsigned int num_streams, unsigned int mem_flags)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	struct usb_host_endpoint*** eps_ptr = &eps;
	unsigned int* num_eps_ptr = &num_eps;
	unsigned int* num_streams_ptr = &num_streams;
	unsigned int* mem_flags_ptr = &mem_flags;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_alloc_streams_call_ctx call_ctx = {hcd, udev, eps, num_eps, num_streams, mem_flags};
	__maybe_unused const struct hc_driver_alloc_streams_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_alloc_streams__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *udev_ptr;
	}

	{
		__maybe_unused const void* __adjusted = *eps_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*eps_ptr) {
			__maybe_unused const void* __adjusted = **eps_ptr;
			glue_pack(__pos, __msg, __ext, __adjusted);
			if (**eps_ptr) {
				caller_marshal_kernel__hc_driver_alloc_streams__eps__in(__pos, __msg, __ext, ctx, **eps_ptr);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *num_eps_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *num_streams_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mem_flags_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_alloc_streams);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_alloc_streams__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			caller_unmarshal_kernel__hc_driver_alloc_streams__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	{
		if (*eps_ptr) {
			if (**eps_ptr) {
				caller_unmarshal_kernel__hc_driver_alloc_streams__eps__in(__pos, __msg, __ext, ctx, **eps_ptr);
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
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_hc_driver_alloc_streams)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_alloc_streams) trmp_hc_driver_alloc_streams(struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint** eps, unsigned int num_eps, unsigned int num_streams, unsigned int mem_flags)
{
	volatile fptr_impl_hc_driver_alloc_streams impl;
	fptr_hc_driver_alloc_streams target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_alloc_streams);
	impl = trmp_impl_hc_driver_alloc_streams;
	return impl(target, hcd, udev, eps, num_eps, num_streams, mem_flags);
}

int trmp_impl_hc_driver_free_streams(fptr_hc_driver_free_streams target, struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint** eps, unsigned int num_eps, unsigned int mem_flags)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	struct usb_host_endpoint*** eps_ptr = &eps;
	unsigned int* num_eps_ptr = &num_eps;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_free_streams_call_ctx call_ctx = {hcd, udev, eps, num_eps, mem_flags};
	__maybe_unused const struct hc_driver_free_streams_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_free_streams__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *udev_ptr;
	}

	{
		__maybe_unused const void* __adjusted = *eps_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*eps_ptr) {
			__maybe_unused const void* __adjusted = **eps_ptr;
			glue_pack_shadow(__pos, __msg, __ext, __adjusted);
			if (**eps_ptr) {
				caller_marshal_kernel__hc_driver_free_streams__eps__in(__pos, __msg, __ext, ctx, **eps_ptr);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *num_eps_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_free_streams);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_free_streams__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			caller_unmarshal_kernel__hc_driver_free_streams__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	{
		if (*eps_ptr) {
			if (**eps_ptr) {
				caller_unmarshal_kernel__hc_driver_free_streams__eps__in(__pos, __msg, __ext, ctx, **eps_ptr);
			}

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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_free_streams)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_free_streams) trmp_hc_driver_free_streams(struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint** eps, unsigned int num_eps, unsigned int mem_flags)
{
	volatile fptr_impl_hc_driver_free_streams impl;
	fptr_hc_driver_free_streams target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_free_streams);
	impl = trmp_impl_hc_driver_free_streams;
	return impl(target, hcd, udev, eps, num_eps, mem_flags);
}

int trmp_impl_hc_driver_add_endpoint(fptr_hc_driver_add_endpoint target, struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint* ep)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	struct usb_host_endpoint** ep_ptr = &ep;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_add_endpoint_call_ctx call_ctx = {hcd, udev, ep};
	__maybe_unused const struct hc_driver_add_endpoint_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_add_endpoint__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *udev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*udev_ptr) {
			caller_marshal_kernel__hc_driver_add_endpoint__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ep_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ep_ptr) {
			caller_marshal_kernel__hc_driver_add_endpoint__ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_add_endpoint);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_add_endpoint__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			caller_unmarshal_kernel__hc_driver_add_endpoint__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	{
		if (*ep_ptr) {
			caller_unmarshal_kernel__hc_driver_add_endpoint__ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_add_endpoint)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_add_endpoint) trmp_hc_driver_add_endpoint(struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint* ep)
{
	volatile fptr_impl_hc_driver_add_endpoint impl;
	fptr_hc_driver_add_endpoint target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_add_endpoint);
	impl = trmp_impl_hc_driver_add_endpoint;
	return impl(target, hcd, udev, ep);
}

int trmp_impl_hc_driver_drop_endpoint(fptr_hc_driver_drop_endpoint target, struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint* ep)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	struct usb_host_endpoint** ep_ptr = &ep;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_drop_endpoint_call_ctx call_ctx = {hcd, udev, ep};
	__maybe_unused const struct hc_driver_drop_endpoint_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_drop_endpoint__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *udev_ptr;
	}

	{
		__maybe_unused const void* __adjusted = *ep_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ep_ptr) {
			caller_marshal_kernel__hc_driver_drop_endpoint__ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_drop_endpoint);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_drop_endpoint__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			caller_unmarshal_kernel__hc_driver_drop_endpoint__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	{
		if (*ep_ptr) {
			caller_unmarshal_kernel__hc_driver_drop_endpoint__ep__in(__pos, __msg, __ext, ctx, *ep_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_drop_endpoint)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_drop_endpoint) trmp_hc_driver_drop_endpoint(struct usb_hcd* hcd, struct usb_device* udev, struct usb_host_endpoint* ep)
{
	volatile fptr_impl_hc_driver_drop_endpoint impl;
	fptr_hc_driver_drop_endpoint target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_drop_endpoint);
	impl = trmp_impl_hc_driver_drop_endpoint;
	return impl(target, hcd, udev, ep);
}

int trmp_impl_hc_driver_check_bandwidth(fptr_hc_driver_check_bandwidth target, struct usb_hcd* hcd, struct usb_device* udev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_check_bandwidth_call_ctx call_ctx = {hcd, udev};
	__maybe_unused const struct hc_driver_check_bandwidth_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_check_bandwidth__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *udev_ptr;
	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_check_bandwidth);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_check_bandwidth__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			caller_unmarshal_kernel__hc_driver_check_bandwidth__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_check_bandwidth)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_check_bandwidth) trmp_hc_driver_check_bandwidth(struct usb_hcd* hcd, struct usb_device* udev)
{
	volatile fptr_impl_hc_driver_check_bandwidth impl;
	fptr_hc_driver_check_bandwidth target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_check_bandwidth);
	impl = trmp_impl_hc_driver_check_bandwidth;
	return impl(target, hcd, udev);
}

void trmp_impl_hc_driver_reset_bandwidth(fptr_hc_driver_reset_bandwidth target, struct usb_hcd* hcd, struct usb_device* udev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	
	__maybe_unused const struct hc_driver_reset_bandwidth_call_ctx call_ctx = {hcd, udev};
	__maybe_unused const struct hc_driver_reset_bandwidth_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_reset_bandwidth__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *udev_ptr;
	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_reset_bandwidth);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_reset_bandwidth__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			caller_unmarshal_kernel__hc_driver_reset_bandwidth__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_hc_driver_reset_bandwidth)
void LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_reset_bandwidth) trmp_hc_driver_reset_bandwidth(struct usb_hcd* hcd, struct usb_device* udev)
{
	volatile fptr_impl_hc_driver_reset_bandwidth impl;
	fptr_hc_driver_reset_bandwidth target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_reset_bandwidth);
	impl = trmp_impl_hc_driver_reset_bandwidth;
	return impl(target, hcd, udev);
}

int trmp_impl_hc_driver_address_device(fptr_hc_driver_address_device target, struct usb_hcd* hcd, struct usb_device* udev)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	struct usb_device** udev_ptr = &udev;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_address_device_call_ctx call_ctx = {hcd, udev};
	__maybe_unused const struct hc_driver_address_device_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_address_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *udev_ptr;
	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_address_device);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_address_device__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		if (*udev_ptr) {
			caller_unmarshal_kernel__hc_driver_address_device__udev__in(__pos, __msg, __ext, ctx, *udev_ptr);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_address_device)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_address_device) trmp_hc_driver_address_device(struct usb_hcd* hcd, struct usb_device* udev)
{
	volatile fptr_impl_hc_driver_address_device impl;
	fptr_hc_driver_address_device target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_address_device);
	impl = trmp_impl_hc_driver_address_device;
	return impl(target, hcd, udev);
}

int trmp_impl_hc_driver_hub_status_data(fptr_hc_driver_hub_status_data target, struct usb_hcd* hcd, char* buf)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	char** buf_ptr = &buf;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_hub_status_data_call_ctx call_ctx = {hcd, buf};
	__maybe_unused const struct hc_driver_hub_status_data_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_hub_status_data__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *buf_ptr;
	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_hub_status_data);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_hub_status_data__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		(void)buf_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_hc_driver_hub_status_data)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_hub_status_data) trmp_hc_driver_hub_status_data(struct usb_hcd* hcd, char* buf)
{
	volatile fptr_impl_hc_driver_hub_status_data impl;
	fptr_hc_driver_hub_status_data target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_hub_status_data);
	impl = trmp_impl_hc_driver_hub_status_data;
	return impl(target, hcd, buf);
}

unsigned int trmp_impl_hc_driver_irq(fptr_hc_driver_irq target, struct usb_hcd* hcd)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_irq_call_ctx call_ctx = {hcd};
	__maybe_unused const struct hc_driver_irq_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_irq__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_irq);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_irq__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_hc_driver_irq)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_irq) trmp_hc_driver_irq(struct usb_hcd* hcd)
{
	volatile fptr_impl_hc_driver_irq impl;
	fptr_hc_driver_irq target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_irq);
	impl = trmp_impl_hc_driver_irq;
	return impl(target, hcd);
}

int trmp_impl_hc_driver_hub_control(fptr_hc_driver_hub_control target, struct usb_hcd* hcd, unsigned short typeReq, unsigned short wValue, unsigned short wIndex, char* buf, unsigned short wLength)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct usb_hcd** hcd_ptr = &hcd;
	unsigned short* typeReq_ptr = &typeReq;
	unsigned short* wValue_ptr = &wValue;
	unsigned short* wIndex_ptr = &wIndex;
	char** buf_ptr = &buf;
	unsigned short* wLength_ptr = &wLength;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct hc_driver_hub_control_call_ctx call_ctx = {hcd, typeReq, wValue, wIndex, buf, wLength};
	__maybe_unused const struct hc_driver_hub_control_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *hcd_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*hcd_ptr) {
			caller_marshal_kernel__hc_driver_hub_control__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *typeReq_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *wValue_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *wIndex_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *buf_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *wLength_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_hc_driver_hub_control);

	*__pos = 0;
	{
		if (*hcd_ptr) {
			caller_unmarshal_kernel__hc_driver_hub_control__hcd__in(__pos, __msg, __ext, ctx, *hcd_ptr);
		}

	}

	{
	}

	{
	}

	{
	}

	{
		if (*buf_ptr) {
			**buf_ptr = glue_unpack(__pos, __msg, __ext, char);
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

LCD_TRAMPOLINE_DATA(trmp_hc_driver_hub_control)
int LCD_TRAMPOLINE_LINKAGE(trmp_hc_driver_hub_control) trmp_hc_driver_hub_control(struct usb_hcd* hcd, unsigned short typeReq, unsigned short wValue, unsigned short wIndex, char* buf, unsigned short wLength)
{
	volatile fptr_impl_hc_driver_hub_control impl;
	fptr_hc_driver_hub_control target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_hc_driver_hub_control);
	impl = trmp_impl_hc_driver_hub_control;
	return impl(target, hcd, typeReq, wValue, wIndex, buf, wLength);
}

int try_dispatch(enum RPC_ID id, struct fipc_message* __msg, struct ext_registers* __ext)
{
	switch(id) {
	case RPC_ID_shared_mem_init:
		glue_user_trace("shared_mem_init\n");
		shared_mem_init_callee(__msg, __ext);
		break;

	case RPC_ID_add_timer:
		glue_user_trace("add_timer\n");
		add_timer_callee(__msg, __ext);
		break;

	case RPC_ID_complete:
		glue_user_trace("complete\n");
		complete_callee(__msg, __ext);
		break;

	case RPC_ID_del_timer:
		glue_user_trace("del_timer\n");
		del_timer_callee(__msg, __ext);
		break;

	case RPC_ID_del_timer_sync:
		glue_user_trace("del_timer_sync\n");
		del_timer_sync_callee(__msg, __ext);
		break;

	case RPC_ID_free_irq:
		glue_user_trace("free_irq\n");
		free_irq_callee(__msg, __ext);
		break;

	case RPC_ID_init_timer_key:
		glue_user_trace("init_timer_key\n");
		init_timer_key_callee(__msg, __ext);
		break;

	case RPC_ID_mod_timer:
		glue_user_trace("mod_timer\n");
		mod_timer_callee(__msg, __ext);
		break;

	case RPC_ID_pci_disable_msi:
		glue_user_trace("pci_disable_msi\n");
		pci_disable_msi_callee(__msg, __ext);
		break;

	case RPC_ID_pci_disable_msix:
		glue_user_trace("pci_disable_msix\n");
		pci_disable_msix_callee(__msg, __ext);
		break;

	case RPC_ID_pci_enable_msi_range:
		glue_user_trace("pci_enable_msi_range\n");
		pci_enable_msi_range_callee(__msg, __ext);
		break;

	case RPC_ID_pci_enable_msix_range:
		glue_user_trace("pci_enable_msix_range\n");
		pci_enable_msix_range_callee(__msg, __ext);
		break;

	case RPC_ID_pci_set_power_state:
		glue_user_trace("pci_set_power_state\n");
		pci_set_power_state_callee(__msg, __ext);
		break;

	case RPC_ID_request_threaded_irq:
		glue_user_trace("request_threaded_irq\n");
		request_threaded_irq_callee(__msg, __ext);
		break;

	case RPC_ID_wait_for_completion:
		glue_user_trace("wait_for_completion\n");
		wait_for_completion_callee(__msg, __ext);
		break;

	case RPC_ID_wait_for_completion_timeout:
		glue_user_trace("wait_for_completion_timeout\n");
		wait_for_completion_timeout_callee(__msg, __ext);
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

	case RPC_ID_usb_disable_xhci_ports:
		glue_user_trace("usb_disable_xhci_ports\n");
		usb_disable_xhci_ports_callee(__msg, __ext);
		break;

	case RPC_ID_dmi_get_system_info:
		glue_user_trace("dmi_get_system_info\n");
		dmi_get_system_info_callee(__msg, __ext);
		break;

	case RPC_ID_usb_acpi_power_manageable:
		glue_user_trace("usb_acpi_power_manageable\n");
		usb_acpi_power_manageable_callee(__msg, __ext);
		break;

	case RPC_ID_usb_acpi_set_power_state:
		glue_user_trace("usb_acpi_set_power_state\n");
		usb_acpi_set_power_state_callee(__msg, __ext);
		break;

	case RPC_ID_usb_disabled:
		glue_user_trace("usb_disabled\n");
		usb_disabled_callee(__msg, __ext);
		break;

	case RPC_ID_usb_hc_died:
		glue_user_trace("usb_hc_died\n");
		usb_hc_died_callee(__msg, __ext);
		break;

	case RPC_ID_usb_hcd_check_unlink_urb:
		glue_user_trace("usb_hcd_check_unlink_urb\n");
		usb_hcd_check_unlink_urb_callee(__msg, __ext);
		break;

	case RPC_ID_usb_hcd_giveback_urb:
		glue_user_trace("usb_hcd_giveback_urb\n");
		usb_hcd_giveback_urb_callee(__msg, __ext);
		break;

	case RPC_ID_usb_hcd_irq:
		glue_user_trace("usb_hcd_irq\n");
		usb_hcd_irq_callee(__msg, __ext);
		break;

	case RPC_ID_usb_hcd_is_primary_hcd:
		glue_user_trace("usb_hcd_is_primary_hcd\n");
		usb_hcd_is_primary_hcd_callee(__msg, __ext);
		break;

	case RPC_ID_usb_hcd_link_urb_to_ep:
		glue_user_trace("usb_hcd_link_urb_to_ep\n");
		usb_hcd_link_urb_to_ep_callee(__msg, __ext);
		break;

	case RPC_ID_usb_hcd_poll_rh_status:
		glue_user_trace("usb_hcd_poll_rh_status\n");
		usb_hcd_poll_rh_status_callee(__msg, __ext);
		break;

	case RPC_ID_usb_hcd_unlink_urb_from_ep:
		glue_user_trace("usb_hcd_unlink_urb_from_ep\n");
		usb_hcd_unlink_urb_from_ep_callee(__msg, __ext);
		break;

	case RPC_ID_usb_wakeup_notification:
		glue_user_trace("usb_wakeup_notification\n");
		usb_wakeup_notification_callee(__msg, __ext);
		break;

	default:
		return 0;
	}

	return 1;
}

