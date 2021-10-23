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

void xhci_init_driver(struct hc_driver* drv, const struct xhci_driver_overrides* over)
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
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*drv_ptr) {
			caller_marshal_kernel__xhci_init_driver__drv__in(__pos, __msg, __ext, ctx, *drv_ptr);
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
			caller_unmarshal_kernel__xhci_init_driver__drv__in(__pos, __msg, __ext, ctx, *drv_ptr);
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

	default:
		return 0;
	}

	return 1;
}

