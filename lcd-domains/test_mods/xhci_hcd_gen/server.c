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

void xhci_dbg_trace(struct xhci_hcd *xhci, void (*trace)(struct va_format *),
            const char *fmt, ...)
{
    struct va_format vaf;
    va_list args;

    va_start(args, fmt);
    vaf.fmt = fmt;
    vaf.va = &args;
    printk("%pV\n", &vaf);
    trace(&vaf);
    va_end(args);
}
EXPORT_SYMBOL(xhci_dbg_trace);

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

void get_loops_per_jiffy_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused struct get_loops_per_jiffy_call_ctx call_ctx = {};
	__maybe_unused struct get_loops_per_jiffy_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = loops_per_jiffy;

	*__pos = 0;
	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void get_jiffies_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused struct get_jiffies_call_ctx call_ctx = {};
	__maybe_unused struct get_jiffies_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = jiffies;

	*__pos = 0;
	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
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

	case RPC_ID_get_loops_per_jiffy:
		glue_user_trace("get_loops_per_jiffy\n");
		get_loops_per_jiffy_callee(__msg, __ext);
		break;

	case RPC_ID_get_jiffies:
		glue_user_trace("get_jiffies\n");
		get_jiffies_callee(__msg, __ext);
		break;


	default:
		return 0;
	}

	return 1;
}

