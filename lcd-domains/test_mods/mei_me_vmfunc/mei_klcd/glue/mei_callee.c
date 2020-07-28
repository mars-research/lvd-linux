#include <lcd_config/pre_hook.h>


#include <lcd_domains/microkernel.h>
#include <liblcd/sync_ipc_poll.h>
#include <asm/cacheflush.h>
#include <linux/atomic.h>
#include <linux/interrupt.h>
#include <liblcd/trampoline.h>

#include "../mei_callee.h"

#include <lcd_config/post_hook.h>

#define CPTR_HASH_BITS      5

static struct glue_cspace *c_cspace;
static DEFINE_HASHTABLE(pdev_hash, CPTR_HASH_BITS);
static DEFINE_HASHTABLE(mei_hash, CPTR_HASH_BITS);
static DEFINE_HASHTABLE(dev_hash, CPTR_HASH_BITS);

struct irq_handler_data_map {
	int irq;
	void *data;
	struct irqhandler_t_container *irqhandler_data;
}irq_map[32];

static int reg_irqs = 0;


int glue_mei_init(void)
{
	int ret;
	ret = glue_cap_init();
	if (ret) {
		LIBLCD_ERR("cap init");
		goto fail1;
	}
	ret = glue_cap_create(&c_cspace);
	if (ret) {
		LIBLCD_ERR("cap create");
		goto fail2;
	}
	return 0;
fail2:
	glue_cap_exit();
fail1:
	return ret;
}

void glue_mei_exit(void)
{
	glue_cap_destroy(c_cspace);
	glue_cap_exit();
}

int glue_insert_mei_device(struct mei_device_container *dev_c)
{
	BUG_ON(!dev_c->mei);

	dev_c->my_ref = __cptr((unsigned long)dev_c->mei);

	hash_add(mei_hash, &dev_c->hentry, (unsigned long) dev_c->mei);

	return 0;
}

int glue_lookup_mei_device(struct cptr c, struct
		mei_device_container **dev_cout) {
	struct mei_device_container *dev_c;
 
	hash_for_each_possible(mei_hash, dev_c,
			hentry, (unsigned long) cptr_val(c)) {
		if (dev_c->mei == (struct mei_device*) c.cptr)
			*dev_cout = dev_c;
	}
	return 0;
}

void glue_remove_mei_device(struct mei_device_container *dev_c)
{
	hash_del(&dev_c->hentry);
}


int inline glue_insert_pdev_hash(struct pci_dev_container *dev_container)
{
	BUG_ON(!dev_container->pdev);

	dev_container->my_ref = __cptr((unsigned long)dev_container->pdev);

	hash_add(pdev_hash, &dev_container->hentry,
				(unsigned long) dev_container->pdev);

	return 0;
}

int inline glue_lookup_pdev_hash(struct cptr c, struct pci_dev_container
		**dev_container_out)
{
        struct pci_dev_container *dev_container;

        hash_for_each_possible(pdev_hash, dev_container,
				hentry, (unsigned long) cptr_val(c)) {
		if (!dev_container) {
			WARN_ON(!dev_container);
			continue;
		}
		if (dev_container->pdev == (struct pci_dev*) c.cptr) {
	                *dev_container_out = dev_container;
			break;
		}
        }
        return 0;
}

void inline glue_remove_pdev_hash(struct pci_dev_container *dev_container)
{
	hash_del(&dev_container->hentry);
}

int glue_insert_device(struct device_container *dev_c)
{
	BUG_ON(!dev_c->dev);

	dev_c->my_ref = __cptr((unsigned long)dev_c->dev);

	hash_add(dev_hash, &dev_c->hentry, (unsigned long) dev_c->dev);

	return 0;
}

int glue_lookup_device(struct cptr c, struct
		device_container **dev_cout) {
	struct device_container *dev_c;

	hash_for_each_possible(dev_hash, dev_c,
			hentry, (unsigned long) cptr_val(c)) {
		if (dev_c->dev == (struct device*) c.cptr)
			*dev_cout = dev_c;
	}
	return 0;
}

void glue_remove_device(struct device_container *dev_c)
{
	hash_del(&dev_c->hentry);
}


int _cond_resched_callee(struct fipc_message *_request)
{
	_cond_resched();
	return 0;
}

int mei_cancel_work_callee(struct fipc_message *_request)
{
	struct mei_device *dev = NULL;
	int ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	mei_cancel_work(dev);
fail_alloc:
	return ret;

}

int mei_deregister_callee(struct fipc_message *_request)
{
	struct mei_device *dev = NULL;
	int ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	mei_deregister(dev);
fail_alloc:
	return ret;

}


bool host_is_ready(struct mei_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	bool func_ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);

	async_msg_set_fn_type(_request,
			HOST_IS_READY);

	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

LCD_TRAMPOLINE_DATA(host_is_ready_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(host_is_ready_trampoline)
host_is_ready_trampoline(struct mei_device *dev)
{
	bool ( *volatile host_is_ready_fp )(struct mei_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			host_is_ready_trampoline);
	host_is_ready_fp = host_is_ready;
	return host_is_ready_fp(dev,
		hidden_args);

}

bool hw_is_ready(struct mei_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	bool func_ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);

	async_msg_set_fn_type(_request, HW_IS_READY);

        fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

LCD_TRAMPOLINE_DATA(hw_is_ready_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(hw_is_ready_trampoline)
hw_is_ready_trampoline(struct mei_device *dev)
{
	bool ( *volatile hw_is_ready_fp )(struct mei_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			hw_is_ready_trampoline);
	hw_is_ready_fp = hw_is_ready;
	return hw_is_ready_fp(dev,
		hidden_args);

}

int hw_reset(struct mei_device *dev,
		int enable,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			HW_RESET);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	fipc_set_reg2(_request,
			enable);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}


LCD_TRAMPOLINE_DATA(hw_reset_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(hw_reset_trampoline)
hw_reset_trampoline(struct mei_device *dev,
		int enable)
{
	int ( *volatile hw_reset_fp )(struct mei_device *,
		int ,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			hw_reset_trampoline);
	hw_reset_fp = hw_reset;
	return hw_reset_fp(dev,
		enable,
		hidden_args);

}

int hw_start(struct mei_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			HW_START);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;

}

LCD_TRAMPOLINE_DATA(hw_start_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(hw_start_trampoline)
hw_start_trampoline(struct mei_device *dev)
{
	int ( *volatile hw_start_fp )(struct mei_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			hw_start_trampoline);
	hw_start_fp = hw_start;
	return hw_start_fp(dev,
		hidden_args);

}

void hw_config(struct mei_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);

	async_msg_set_fn_type(_request,
			HW_CONFIG);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	return;

}

LCD_TRAMPOLINE_DATA(hw_config_trampoline);
void  LCD_TRAMPOLINE_LINKAGE(hw_config_trampoline)
hw_config_trampoline(struct mei_device *dev)
{
	void ( *volatile hw_config_fp )(struct mei_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			hw_config_trampoline);
	hw_config_fp = hw_config;
	return hw_config_fp(dev,
		hidden_args);

}

int fw_status(struct mei_device *dev,
		struct mei_fw_status *fw_sts,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			FW_STATUS);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;

}

LCD_TRAMPOLINE_DATA(fw_status_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(fw_status_trampoline)
fw_status_trampoline(struct mei_device *dev,
		struct mei_fw_status *fw_sts)
{
	int ( *volatile fw_status_fp )(struct mei_device *,
		struct mei_fw_status *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			fw_status_trampoline);
	fw_status_fp = fw_status;
	return fw_status_fp(dev,
		fw_sts,
		hidden_args);

}

enum mei_pg_state pg_state(struct mei_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	enum mei_pg_state func_ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			PG_STATE);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;

}

LCD_TRAMPOLINE_DATA(pg_state_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(pg_state_trampoline)
pg_state_trampoline(struct mei_device *dev)
{
	enum mei_pg_state ( *volatile pg_state_fp )(struct mei_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			pg_state_trampoline);
	pg_state_fp = pg_state;
	return pg_state_fp(dev,
		hidden_args);

}

bool pg_in_transition(struct mei_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	bool func_ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			PG_IN_TRANSITION);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

LCD_TRAMPOLINE_DATA(pg_in_transition_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(pg_in_transition_trampoline)
pg_in_transition_trampoline(struct mei_device *dev)
{
	bool ( *volatile pg_in_transition_fp )(struct mei_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			pg_in_transition_trampoline);
	pg_in_transition_fp = pg_in_transition;
	return pg_in_transition_fp(dev,
		hidden_args);

}

bool pg_is_enabled(struct mei_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	bool func_ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			PG_IS_ENABLED);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;

}

LCD_TRAMPOLINE_DATA(pg_is_enabled_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(pg_is_enabled_trampoline)
pg_is_enabled_trampoline(struct mei_device *dev)
{
	bool ( *volatile pg_is_enabled_fp )(struct mei_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			pg_is_enabled_trampoline);
	pg_is_enabled_fp = pg_is_enabled;
	return pg_is_enabled_fp(dev,
		hidden_args);

}

void intr_clear(struct mei_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			INTR_CLEAR);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	return;

}

LCD_TRAMPOLINE_DATA(intr_clear_trampoline);
void  LCD_TRAMPOLINE_LINKAGE(intr_clear_trampoline)
intr_clear_trampoline(struct mei_device *dev)
{
	void ( *volatile intr_clear_fp )(struct mei_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			intr_clear_trampoline);
	intr_clear_fp = intr_clear;
	return intr_clear_fp(dev,
		hidden_args);

}

void intr_enable(struct mei_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			INTR_ENABLE);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	return;

}

LCD_TRAMPOLINE_DATA(intr_enable_trampoline);
void  LCD_TRAMPOLINE_LINKAGE(intr_enable_trampoline)
intr_enable_trampoline(struct mei_device *dev)
{
	void ( *volatile intr_enable_fp )(struct mei_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			intr_enable_trampoline);
	intr_enable_fp = intr_enable;
	return intr_enable_fp(dev,
		hidden_args);

}

void intr_disable(struct mei_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			INTR_DISABLE);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	return;

}

LCD_TRAMPOLINE_DATA(intr_disable_trampoline);
void  LCD_TRAMPOLINE_LINKAGE(intr_disable_trampoline)
intr_disable_trampoline(struct mei_device *dev)
{
	void ( *volatile intr_disable_fp )(struct mei_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			intr_disable_trampoline);
	intr_disable_fp = intr_disable;
	return intr_disable_fp(dev,
		hidden_args);

}

int hbuf_free_slots(struct mei_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			HBUF_FREE_SLOTS);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;

}

LCD_TRAMPOLINE_DATA(hbuf_free_slots_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(hbuf_free_slots_trampoline)
hbuf_free_slots_trampoline(struct mei_device *dev)
{
	int ( *volatile hbuf_free_slots_fp )(struct mei_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			hbuf_free_slots_trampoline);
	hbuf_free_slots_fp = hbuf_free_slots;
	return hbuf_free_slots_fp(dev,
		hidden_args);

}

bool hbuf_is_ready(struct mei_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	bool func_ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			HBUF_IS_READY);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

LCD_TRAMPOLINE_DATA(hbuf_is_ready_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(hbuf_is_ready_trampoline)
hbuf_is_ready_trampoline(struct mei_device *dev)
{
	bool ( *volatile hbuf_is_ready_fp )(struct mei_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			hbuf_is_ready_trampoline);
	hbuf_is_ready_fp = hbuf_is_ready;
	return hbuf_is_ready_fp(dev,
		hidden_args);

}

size_t hbuf_max_len(struct mei_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	size_t func_ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			HBUF_MAX_LEN);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

LCD_TRAMPOLINE_DATA(hbuf_max_len_trampoline);
unsigned int  LCD_TRAMPOLINE_LINKAGE(hbuf_max_len_trampoline)
hbuf_max_len_trampoline(struct mei_device *dev)
{
	size_t ( *volatile hbuf_max_len_fp )(struct mei_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			hbuf_max_len_trampoline);
	hbuf_max_len_fp = hbuf_max_len;
	return hbuf_max_len_fp(dev,
		hidden_args);

}

int write(struct mei_device *dev,
		struct mei_msg_hdr *hdr,
		unsigned char *buf,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			WRITE);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	/* TODO: handle write buffer */
	//fipc_set_reg1(_request,
	//		buf);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

LCD_TRAMPOLINE_DATA(write_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(write_trampoline)
write_trampoline(struct mei_device *dev,
		struct mei_msg_hdr *hdr,
		unsigned char *buf)
{
	int ( *volatile write_fp )(struct mei_device *,
		struct mei_msg_hdr *,
		unsigned char *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			write_trampoline);
	write_fp = write;
	return write_fp(dev,
		hdr,
		buf,
		hidden_args);

}

int rdbuf_full_slots(struct mei_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			RDBUF_FULL_SLOTS);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;

}

LCD_TRAMPOLINE_DATA(rdbuf_full_slots_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(rdbuf_full_slots_trampoline)
rdbuf_full_slots_trampoline(struct mei_device *dev)
{
	int ( *volatile rdbuf_full_slots_fp )(struct mei_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			rdbuf_full_slots_trampoline);
	rdbuf_full_slots_fp = rdbuf_full_slots;
	return rdbuf_full_slots_fp(dev,
		hidden_args);

}

u32 read_hdr(struct mei_device *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	u32 func_ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			READ_HDR);
	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

LCD_TRAMPOLINE_DATA(read_hdr_trampoline);
unsigned int  LCD_TRAMPOLINE_LINKAGE(read_hdr_trampoline)
read_hdr_trampoline(struct mei_device *dev)
{
	u32 ( *volatile read_hdr_fp )(struct mei_device *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			read_hdr_trampoline);
	read_hdr_fp = read_hdr;
	return read_hdr_fp(dev,
		hidden_args);

}

int read(struct mei_device *dev,
		unsigned char *buf,
		unsigned long len,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	struct mei_hw_ops_container *ops_container =
		hidden_args->struct_container;
	struct mei_device_container *meidev_container = NULL;

	glue_lookup_mei_device(__cptr((uint64_t) dev), &meidev_container);


	async_msg_set_fn_type(_request,
			READ);

	fipc_set_reg0(_request, ops_container->other_ref.cptr);
	fipc_set_reg1(_request, meidev_container->other_ref.cptr);

	/* TODO: handle read buffer */
	//fipc_set_reg1(_request,
	//		buf);
	fipc_set_reg2(_request,
			len);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

LCD_TRAMPOLINE_DATA(read_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(read_trampoline)
read_trampoline(struct mei_device *dev,
		unsigned char *buf,
		unsigned long len)
{
	int ( *volatile read_fp )(struct mei_device *,
		unsigned char *,
		unsigned long ,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			read_trampoline);
	read_fp = read;
	return read_fp(dev,
		buf,
		len,
		hidden_args);

}

irqreturn_t mei_irq_handler(int irq, void *data)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct irqhandler_t_container *irqhandler_container;
	irqreturn_t irqret;

	INIT_IPC_MSG(&r);

	WARN_ONCE(!irqs_disabled(),"irqs enabled in %s\n", __func__);

	irqhandler_container = (struct irqhandler_t_container*) data;

	async_msg_set_fn_type(_request, MEI_IRQ_HANDLER);

	fipc_set_reg0(_request, irq);

	/* pass this irqhandler's other ref cptr */
	fipc_set_reg1(_request, irqhandler_container->other_ref.cptr);

#ifdef CONFIG_LCD_TRACE_BUFFER
	//add_trace_entry(EVENT_MSIX_HANDLER, async_msg_get_fn_type(_request));
#endif
	vmfunc_klcd_wrapper(_request, 1);

	irqret = fipc_get_reg0(_request);

	return irqret;
}

int setup_meidev_ops(struct mei_hw_ops_container *hw_ops_container)
{
	int ret;
	struct trampoline_hidden_args *host_is_ready_hidden_args;
	struct trampoline_hidden_args *hw_is_ready_hidden_args;
	struct trampoline_hidden_args *hw_reset_hidden_args;
	struct trampoline_hidden_args *hw_start_hidden_args;
	struct trampoline_hidden_args *hw_config_hidden_args;
	struct trampoline_hidden_args *fw_status_hidden_args;
	struct trampoline_hidden_args *pg_state_hidden_args;
	struct trampoline_hidden_args *pg_in_transition_hidden_args;
	struct trampoline_hidden_args *pg_is_enabled_hidden_args;
	struct trampoline_hidden_args *intr_clear_hidden_args;
	struct trampoline_hidden_args *intr_enable_hidden_args;
	struct trampoline_hidden_args *intr_disable_hidden_args;
	struct trampoline_hidden_args *hbuf_free_slots_hidden_args;
	struct trampoline_hidden_args *hbuf_is_ready_hidden_args;
	struct trampoline_hidden_args *hbuf_max_len_hidden_args;
	struct trampoline_hidden_args *write_hidden_args;
	struct trampoline_hidden_args *rdbuf_full_slots_hidden_args;
	struct trampoline_hidden_args *read_hdr_hidden_args;
	struct trampoline_hidden_args *read_hidden_args;
		host_is_ready_hidden_args = kzalloc(sizeof( *host_is_ready_hidden_args ),
		GFP_KERNEL);
	if (!host_is_ready_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc1;
	}
	host_is_ready_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(host_is_ready_trampoline);
	if (!host_is_ready_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	host_is_ready_hidden_args->t_handle->hidden_args = host_is_ready_hidden_args;
	host_is_ready_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.host_is_ready = LCD_HANDLE_TO_TRAMPOLINE(host_is_ready_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )host_is_ready_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(host_is_ready_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	hw_is_ready_hidden_args = kzalloc(sizeof( *hw_is_ready_hidden_args ),
		GFP_KERNEL);
	if (!hw_is_ready_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc2;
	}
	hw_is_ready_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(hw_is_ready_trampoline);
	if (!hw_is_ready_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup2;
	}
	hw_is_ready_hidden_args->t_handle->hidden_args = hw_is_ready_hidden_args;
	hw_is_ready_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.hw_is_ready = LCD_HANDLE_TO_TRAMPOLINE(hw_is_ready_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )hw_is_ready_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(hw_is_ready_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	hw_reset_hidden_args = kzalloc(sizeof( *hw_reset_hidden_args ),
		GFP_KERNEL);
	if (!hw_reset_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc3;
	}
	hw_reset_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(hw_reset_trampoline);
	if (!hw_reset_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup3;
	}
	hw_reset_hidden_args->t_handle->hidden_args = hw_reset_hidden_args;
	hw_reset_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.hw_reset = LCD_HANDLE_TO_TRAMPOLINE(hw_reset_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )hw_reset_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(hw_reset_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	hw_start_hidden_args = kzalloc(sizeof( *hw_start_hidden_args ),
		GFP_KERNEL);
	if (!hw_start_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc4;
	}
	hw_start_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(hw_start_trampoline);
	if (!hw_start_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup4;
	}
	hw_start_hidden_args->t_handle->hidden_args = hw_start_hidden_args;
	hw_start_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.hw_start = LCD_HANDLE_TO_TRAMPOLINE(hw_start_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )hw_start_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(hw_start_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	hw_config_hidden_args = kzalloc(sizeof( *hw_config_hidden_args ),
		GFP_KERNEL);
	if (!hw_config_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc5;
	}
	hw_config_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(hw_config_trampoline);
	if (!hw_config_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup5;
	}
	hw_config_hidden_args->t_handle->hidden_args = hw_config_hidden_args;
	hw_config_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.hw_config = LCD_HANDLE_TO_TRAMPOLINE(hw_config_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )hw_config_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(hw_config_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	fw_status_hidden_args = kzalloc(sizeof( *fw_status_hidden_args ),
		GFP_KERNEL);
	if (!fw_status_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc6;
	}
	fw_status_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(fw_status_trampoline);
	if (!fw_status_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup6;
	}
	fw_status_hidden_args->t_handle->hidden_args = fw_status_hidden_args;
	fw_status_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.fw_status = LCD_HANDLE_TO_TRAMPOLINE(fw_status_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )fw_status_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(fw_status_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	pg_state_hidden_args = kzalloc(sizeof( *pg_state_hidden_args ),
		GFP_KERNEL);
	if (!pg_state_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc7;
	}
	pg_state_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(pg_state_trampoline);
	if (!pg_state_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup7;
	}
	pg_state_hidden_args->t_handle->hidden_args = pg_state_hidden_args;
	pg_state_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.pg_state = LCD_HANDLE_TO_TRAMPOLINE(pg_state_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )pg_state_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(pg_state_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	pg_in_transition_hidden_args = kzalloc(sizeof( *pg_in_transition_hidden_args ),
		GFP_KERNEL);
	if (!pg_in_transition_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc8;
	}
	pg_in_transition_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(pg_in_transition_trampoline);
	if (!pg_in_transition_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup8;
	}
	pg_in_transition_hidden_args->t_handle->hidden_args = pg_in_transition_hidden_args;
	pg_in_transition_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.pg_in_transition = LCD_HANDLE_TO_TRAMPOLINE(pg_in_transition_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )pg_in_transition_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(pg_in_transition_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	pg_is_enabled_hidden_args = kzalloc(sizeof( *pg_is_enabled_hidden_args ),
		GFP_KERNEL);
	if (!pg_is_enabled_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc9;
	}
	pg_is_enabled_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(pg_is_enabled_trampoline);
	if (!pg_is_enabled_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup9;
	}
	pg_is_enabled_hidden_args->t_handle->hidden_args = pg_is_enabled_hidden_args;
	pg_is_enabled_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.pg_is_enabled = LCD_HANDLE_TO_TRAMPOLINE(pg_is_enabled_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )pg_is_enabled_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(pg_is_enabled_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	intr_clear_hidden_args = kzalloc(sizeof( *intr_clear_hidden_args ),
		GFP_KERNEL);
	if (!intr_clear_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc10;
	}
	intr_clear_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(intr_clear_trampoline);
	if (!intr_clear_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup10;
	}
	intr_clear_hidden_args->t_handle->hidden_args = intr_clear_hidden_args;
	intr_clear_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.intr_clear = LCD_HANDLE_TO_TRAMPOLINE(intr_clear_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )intr_clear_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(intr_clear_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	intr_enable_hidden_args = kzalloc(sizeof( *intr_enable_hidden_args ),
		GFP_KERNEL);
	if (!intr_enable_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc11;
	}
	intr_enable_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(intr_enable_trampoline);
	if (!intr_enable_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup11;
	}
	intr_enable_hidden_args->t_handle->hidden_args = intr_enable_hidden_args;
	intr_enable_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.intr_enable = LCD_HANDLE_TO_TRAMPOLINE(intr_enable_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )intr_enable_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(intr_enable_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	intr_disable_hidden_args = kzalloc(sizeof( *intr_disable_hidden_args ),
		GFP_KERNEL);
	if (!intr_disable_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc12;
	}
	intr_disable_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(intr_disable_trampoline);
	if (!intr_disable_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup12;
	}
	intr_disable_hidden_args->t_handle->hidden_args = intr_disable_hidden_args;
	intr_disable_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.intr_disable = LCD_HANDLE_TO_TRAMPOLINE(intr_disable_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )intr_disable_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(intr_disable_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	hbuf_free_slots_hidden_args = kzalloc(sizeof( *hbuf_free_slots_hidden_args ),
		GFP_KERNEL);
	if (!hbuf_free_slots_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc13;
	}
	hbuf_free_slots_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(hbuf_free_slots_trampoline);
	if (!hbuf_free_slots_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup13;
	}
	hbuf_free_slots_hidden_args->t_handle->hidden_args = hbuf_free_slots_hidden_args;
	hbuf_free_slots_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.hbuf_free_slots = LCD_HANDLE_TO_TRAMPOLINE(hbuf_free_slots_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )hbuf_free_slots_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(hbuf_free_slots_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	hbuf_is_ready_hidden_args = kzalloc(sizeof( *hbuf_is_ready_hidden_args ),
		GFP_KERNEL);
	if (!hbuf_is_ready_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc14;
	}
	hbuf_is_ready_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(hbuf_is_ready_trampoline);
	if (!hbuf_is_ready_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup14;
	}
	hbuf_is_ready_hidden_args->t_handle->hidden_args = hbuf_is_ready_hidden_args;
	hbuf_is_ready_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.hbuf_is_ready = LCD_HANDLE_TO_TRAMPOLINE(hbuf_is_ready_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )hbuf_is_ready_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(hbuf_is_ready_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	hbuf_max_len_hidden_args = kzalloc(sizeof( *hbuf_max_len_hidden_args ),
		GFP_KERNEL);
	if (!hbuf_max_len_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc15;
	}
	hbuf_max_len_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(hbuf_max_len_trampoline);
	if (!hbuf_max_len_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup15;
	}
	hbuf_max_len_hidden_args->t_handle->hidden_args = hbuf_max_len_hidden_args;
	hbuf_max_len_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.hbuf_max_len = LCD_HANDLE_TO_TRAMPOLINE(hbuf_max_len_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )hbuf_max_len_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(hbuf_max_len_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	write_hidden_args = kzalloc(sizeof( *write_hidden_args ),
		GFP_KERNEL);
	if (!write_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc16;
	}
	write_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(write_trampoline);
	if (!write_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup16;
	}
	write_hidden_args->t_handle->hidden_args = write_hidden_args;
	write_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.write = LCD_HANDLE_TO_TRAMPOLINE(write_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )write_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(write_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	rdbuf_full_slots_hidden_args = kzalloc(sizeof( *rdbuf_full_slots_hidden_args ),
		GFP_KERNEL);
	if (!rdbuf_full_slots_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc17;
	}
	rdbuf_full_slots_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(rdbuf_full_slots_trampoline);
	if (!rdbuf_full_slots_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup17;
	}
	rdbuf_full_slots_hidden_args->t_handle->hidden_args = rdbuf_full_slots_hidden_args;
	rdbuf_full_slots_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.rdbuf_full_slots = LCD_HANDLE_TO_TRAMPOLINE(rdbuf_full_slots_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )rdbuf_full_slots_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(rdbuf_full_slots_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	read_hdr_hidden_args = kzalloc(sizeof( *read_hdr_hidden_args ),
		GFP_KERNEL);
	if (!read_hdr_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc18;
	}
	read_hdr_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(read_hdr_trampoline);
	if (!read_hdr_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup18;
	}
	read_hdr_hidden_args->t_handle->hidden_args = read_hdr_hidden_args;
	read_hdr_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.read_hdr = LCD_HANDLE_TO_TRAMPOLINE(read_hdr_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )read_hdr_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(read_hdr_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
	read_hidden_args = kzalloc(sizeof( *read_hidden_args ),
		GFP_KERNEL);
	if (!read_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc19;
	}
	read_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(read_trampoline);
	if (!read_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup19;
	}
	read_hidden_args->t_handle->hidden_args = read_hidden_args;
	read_hidden_args->struct_container = hw_ops_container;
	hw_ops_container->mei_hw_ops.read = LCD_HANDLE_TO_TRAMPOLINE(read_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )read_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(read_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));
fail_alloc1:
fail_dup1:
fail_alloc2:
fail_dup2:
fail_alloc3:
fail_dup3:
fail_alloc4:
fail_dup4:
fail_alloc5:
fail_dup5:
fail_alloc6:
fail_dup6:
fail_alloc7:
fail_dup7:
fail_alloc8:
fail_dup8:
fail_alloc9:
fail_dup9:
fail_alloc10:
fail_dup10:
fail_alloc11:
fail_dup11:
fail_alloc12:
fail_dup12:
fail_alloc13:
fail_dup13:
fail_alloc14:
fail_dup14:
fail_alloc15:
fail_dup15:
fail_alloc16:
fail_dup16:
fail_alloc17:
fail_dup17:
fail_alloc18:
fail_dup18:
fail_alloc19:
fail_dup19:
	return 0;
}

int mei_device_init_callee(struct fipc_message *_request)
{
	struct mei_device_container *meidev_container = NULL;
	struct mei_device *meidev = NULL;
	struct mei_hw_ops_container *hw_ops_container = NULL;
	struct device_container *device_container = NULL;
	struct device *device;
	int ret = 0;

	glue_lookup_device(__cptr(fipc_get_reg1(_request)), &device_container);

	device = device_container->dev;

	meidev_container = kzalloc(sizeof(*meidev_container), GFP_KERNEL);
	if (!meidev_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	meidev = kzalloc(sizeof(*meidev) + sizeof(struct mei_me_hw), GFP_KERNEL);
	if (!meidev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	meidev_container->mei = meidev;
	meidev_container->other_ref.cptr = fipc_get_reg0(_request);

	glue_insert_mei_device(meidev_container);

	hw_ops_container = kzalloc(sizeof(struct mei_hw_ops_container), GFP_KERNEL);

	if (!hw_ops_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_insert_mei_hw_ops_type(c_cspace,
		hw_ops_container,
		&hw_ops_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	hw_ops_container->other_ref.cptr = fipc_get_reg2(_request);

	mei_device_init(meidev, device,
			( &hw_ops_container->mei_hw_ops ));

	fipc_set_reg0(_request, hw_ops_container->other_ref.cptr);
fail_alloc:
fail_insert:
	return ret;

}

int mei_hbm_pg_callee(struct fipc_message *_request)
{
	struct mei_device *dev = NULL;
	char pg_cmd = 0;
	int ret = 0;
	int func_ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	pg_cmd = fipc_get_reg1(_request);
	func_ret = mei_hbm_pg(dev,
		pg_cmd);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int mei_hbm_pg_resume_callee(struct fipc_message *_request)
{
	struct mei_device *dev = NULL;
	int ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	mei_hbm_pg_resume(dev);
fail_alloc:
	return ret;

}

int mei_irq_compl_handler_callee(struct fipc_message *_request)
{
	struct mei_device *dev = NULL;
	struct mei_cl_cb *compl_list = NULL;
	int ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	compl_list = kzalloc(sizeof( *compl_list ),
		GFP_KERNEL);
	if (!compl_list) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	mei_irq_compl_handler(dev,
			compl_list);
fail_alloc:
	return ret;

}

int mei_irq_read_handler_callee(struct fipc_message *_request)
{
	struct mei_device *dev = NULL;
	struct mei_cl_cb *cmpl_list = NULL;
	int *slots = 0;
	int ret = 0;
	int func_ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	cmpl_list = kzalloc(sizeof( *cmpl_list ),
		GFP_KERNEL);
	if (!cmpl_list) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	slots = kzalloc(sizeof( int   ),
		GFP_KERNEL);
	if (!slots) {
		LIBLCD_ERR("kzalloc");
		lcd_exit(-1);
	}
	/* TODO: handle slots */
	//slots = fipc_get_reg1(_request);
	func_ret = mei_irq_read_handler(dev,
		cmpl_list,
		slots);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int mei_irq_write_handler_callee(struct fipc_message *_request)
{
	struct mei_device *dev = NULL;
	struct mei_cl_cb *cmpl_list = NULL;
	int ret = 0;
	int func_ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	cmpl_list = kzalloc(sizeof( *cmpl_list ),
		GFP_KERNEL);
	if (!cmpl_list) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = mei_irq_write_handler(dev,
		cmpl_list);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int mei_register_callee(struct fipc_message *_request)
{
	struct mei_device *dev = NULL;
	struct device *parent = NULL;
	int ret = 0;
	int func_ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	parent = kzalloc(sizeof( *parent ),
		GFP_KERNEL);
	if (!parent) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = mei_register(dev,
		parent);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int mei_restart_callee(struct fipc_message *_request)
{
	struct mei_device *dev = NULL;
	int ret = 0;
	int func_ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = mei_restart(dev);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int mei_start_callee(struct fipc_message *_request)
{
	struct mei_device *dev = NULL;
	int ret = 0;
	int func_ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = mei_start(dev);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int mei_stop_callee(struct fipc_message *_request)
{
	struct mei_device *dev = NULL;
	int ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	mei_stop(dev);
fail_alloc:
	return ret;

}

int mei_write_is_idle_callee(struct fipc_message *_request)
{
	struct mei_device *dev = NULL;
	int ret = 0;
	int func_ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	func_ret = mei_write_is_idle(dev);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int probe(struct pci_dev *dev,
		const struct pci_device_id *id,
		struct trampoline_hidden_args *hidden_args)
{
	struct pci_dev_container *dev_container = NULL;
	struct device_container *device_container;
	int ret = 0;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_driver_container *pdrv_container =
		hidden_args->struct_container;
	struct ext_registers *_regs = get_register_page(smp_processor_id());
	uint64_t *regs = &_regs->regs[0];
	u32 i = 0;

	LIBLCD_MSG("%s, irq # %d | msix_enabled %d", __func__, dev->irq,
				dev->msix_enabled);

	dev_container = kzalloc(sizeof( struct pci_dev_container   ),
		GFP_KERNEL);
	if (!dev_container) {
		LIBLCD_ERR("kzalloc dev_container");
		goto fail_alloc;
	}

	dev_container->pdev = dev;

	device_container = kzalloc(sizeof( struct device_container   ),
		GFP_KERNEL);
	if (!device_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	device_container->dev = &dev_container->pdev->dev;

	glue_insert_device(device_container);
	/*
	 * Kernel does not give us pci_dev with a container wrapped in.
	 * So, let's have a pointer!
	 */
	glue_insert_pdev_hash(dev_container);

	async_msg_set_fn_type(_request, PROBE);
	
	regs[i++] = pdrv_container->other_ref.cptr;
	regs[i++] = dev_container->my_ref.cptr;
	regs[i++] = device_container->my_ref.cptr;
	regs[i++] = *dev->dev.dma_mask;
	/* needed for pci_is_enabled() check */
	regs[i++] = atomic_read(&dev->enable_cnt);

	// marshal array
	{
		u32 num_elements = 1;
		u32 e;
		regs[i++] = num_elements;
#define ID(x)	id->x
		for (e = 0; e < num_elements; e++) {
			regs[i++] = ID(vendor);
			regs[i++] = ID(device);

			regs[i++] = ID(subvendor);
			regs[i++] = ID(subdevice);

			regs[i++] = ID(class);
			regs[i++] = ID(class_mask);
			regs[i++] = ID(driver_data);
		}
#undef ID
	}

	vmfunc_klcd_wrapper(_request, 1);

	printk("%s, send request done\n", __func__);

	func_ret = fipc_get_reg0(_request);
	dev_container->other_ref.cptr = fipc_get_reg1(_request);

	return func_ret;

fail_alloc:
	return ret;
}

LCD_TRAMPOLINE_DATA(probe_trampoline);
int  LCD_TRAMPOLINE_LINKAGE(probe_trampoline)
probe_trampoline(struct pci_dev *dev,
		const struct pci_device_id *id)
{
	int ( *volatile probe_fp )(struct pci_dev *,
		const struct pci_device_id *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			probe_trampoline);
	probe_fp = probe;
	return probe_fp(dev, id, hidden_args);
}

void remove(struct pci_dev *dev,
		struct trampoline_hidden_args *hidden_args)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *dev_container = NULL;
	struct pci_driver_container *pdrv_container =
		hidden_args->struct_container;

	INIT_IPC_MSG(&r);

	glue_lookup_pdev_hash(__cptr((unsigned long) dev), &dev_container);

	async_msg_set_fn_type(_request, REMOVE);
	fipc_set_reg0(_request, pdrv_container->other_ref.cptr);
	fipc_set_reg1(_request, dev_container->other_ref.cptr);

	vmfunc_klcd_wrapper(_request, 1);

	return;
}

LCD_TRAMPOLINE_DATA(remove_trampoline);
void  LCD_TRAMPOLINE_LINKAGE(remove_trampoline)
remove_trampoline(struct pci_dev *dev)
{
	void ( *volatile remove_fp )(struct pci_dev *,
		struct trampoline_hidden_args *);
	struct trampoline_hidden_args *hidden_args;
	LCD_TRAMPOLINE_PROLOGUE(hidden_args,
			remove_trampoline);
	remove_fp = remove;
	return remove_fp(dev, hidden_args);
}


int pci_disable_device_callee(struct fipc_message *_request)
{
	int ret = 0;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	pci_disable_device(pdev);

	return ret;
}

int pci_disable_msi_callee(struct fipc_message *_request)
{
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;
	int ret = 0;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	pci_disable_msi(pdev);

	LIBLCD_MSG("%s returned");
	return ret;
}


int pci_enable_device_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	func_ret = pci_enable_device(pdev);

	fipc_set_reg0(_request, func_ret);

	return ret;
}


int pci_enable_msi_range_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret;
	int minvec, maxvec;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	pdev = dev_container->pdev;

	minvec = fipc_get_reg1(_request);
	maxvec = fipc_get_reg2(_request);

	func_ret = pci_enable_msi_range(pdev, minvec, maxvec);

	LIBLCD_MSG("%s, returned %d", __func__, func_ret);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int pci_iomap_callee(struct fipc_message *_request)
{
	struct pci_dev *dev = NULL;
	int bar = 0;
	unsigned 	long maxlen = 0;
	int ret = 0;
	void *iomem;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;
	cptr_t res0_cptr;
	unsigned int res0_len;
	cptr_t lcd_res0_cptr;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg2(_request)),
			&dev_container);

	pdev = dev_container->pdev;
	/* get LCD's pool cptr */
	lcd_res0_cptr = __cptr(fipc_get_reg0(_request));

	bar = fipc_get_reg3(_request);
	maxlen = fipc_get_reg4(_request);

	iomem = pci_iomap(dev, bar, maxlen);

	res0_len = pci_resource_len(pdev, 0);

	LIBLCD_MSG("res0 start: %lx, len: %d",
			pci_resource_start(pdev, 0),
			get_order(res0_len));

	ret = lcd_volunteer_dev_mem(__gpa(pci_resource_start(pdev, 0)),
			get_order(res0_len),
			&res0_cptr);

	if (ret) {
		LIBLCD_ERR("volunteer devmem");
		goto fail_vol;
	}

	copy_msg_cap_vmfunc(current->lcd, current->vmfunc_lcd, res0_cptr, lcd_res0_cptr);

	fipc_set_reg0(_request, res0_len);

fail_vol:
	return ret;
}

int pci_iounmap_callee(struct fipc_message *_request)
{
	struct pci_dev *dev = NULL;
	int *addr = 0;
	int ret = 0;
	dev = kzalloc(sizeof( *dev ),
		GFP_KERNEL);
	if (!dev) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	/* TODO: handle addr */
	//addr = fipc_get_reg1(_request);
	pci_iounmap(dev,
			addr);
fail_alloc:
	return ret;

}

int __pci_register_driver_callee(struct fipc_message *_request)
{
	struct pci_driver_container *pci_driver_container;
	struct ext_registers *_regs = get_register_page(smp_processor_id());
	uint64_t *regs = &_regs->regs[0];
	struct pci_driver *drv = NULL;
	//struct module *owner = NULL;
	struct trampoline_hidden_args *drv_probe_hidden_args;
	struct trampoline_hidden_args *drv_remove_hidden_args;
	unsigned i = 0;

	char *mod_name = 0;
	int ret = 0;
	int func_ret = 0;

	pci_driver_container = kzalloc(sizeof(*pci_driver_container), GFP_KERNEL);

	if (!pci_driver_container) {
		LIBLCD_ERR("alloc failed");
		goto fail_alloc;
	}

	drv = &pci_driver_container->pci_driver;

	ret = glue_cap_insert_pci_driver_type(c_cspace, pci_driver_container, &pci_driver_container->my_ref);	

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert1;
	}

	drv_probe_hidden_args = kzalloc(sizeof( *drv_probe_hidden_args ),
		GFP_KERNEL);
	if (!drv_probe_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc1;
	}
	drv_probe_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(probe_trampoline);
	if (!drv_probe_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup1;
	}
	drv_probe_hidden_args->t_handle->hidden_args = drv_probe_hidden_args;
	drv_probe_hidden_args->struct_container = pci_driver_container;
	pci_driver_container->pci_driver.probe = LCD_HANDLE_TO_TRAMPOLINE(drv_probe_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_probe_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(probe_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));

	drv_remove_hidden_args = kzalloc(sizeof( *drv_remove_hidden_args ),
		GFP_KERNEL);
	if (!drv_remove_hidden_args) {
		LIBLCD_ERR("kzalloc hidden args");
		goto fail_alloc2;
	}
	drv_remove_hidden_args->t_handle = LCD_DUP_TRAMPOLINE(remove_trampoline);
	if (!drv_remove_hidden_args->t_handle) {
		LIBLCD_ERR("duplicate trampoline");
		goto fail_dup2;
	}
	drv_remove_hidden_args->t_handle->hidden_args = drv_remove_hidden_args;
	drv_remove_hidden_args->struct_container = pci_driver_container;
	pci_driver_container->pci_driver.remove = LCD_HANDLE_TO_TRAMPOLINE(drv_remove_hidden_args->t_handle);
	ret = set_memory_x(( ( unsigned  long   )drv_remove_hidden_args->t_handle ) & ( PAGE_MASK ),
		( ALIGN(LCD_TRAMPOLINE_SIZE(remove_trampoline),
		PAGE_SIZE) ) >> ( PAGE_SHIFT ));


	pci_driver_container->other_ref.cptr = regs[i++];
	// unmarshall string
	{
		size_t slen = regs[i++];
		u8 num_regs = DIV_ROUND_UP(slen, sizeof(uint64_t));
		mod_name = kzalloc(slen, GFP_KERNEL);
		if (!mod_name) {
			LIBLCD_ERR("alloc failed");
			goto fail_alloc;
		}
		memcpy(mod_name, &regs[i], slen);
		i += num_regs;
		drv->name = mod_name;
	}

	// unmarshal sentinel array
	{
		struct pci_device_id *id_table;
		u32 num_elements = regs[i++];
		u32 e;
		id_table = kzalloc(sizeof(struct pci_device_id) * num_elements, GFP_KERNEL);
#define ID(idx, x)	id_table[idx].x
		for (e = 0; e < num_elements; e++) {
			ID(e, vendor) = regs[i++];
			ID(e, device) = regs[i++];
			ID(e, subvendor) = regs[i++];
			ID(e, subdevice) = regs[i++];
			ID(e, class) = regs[i++];
			ID(e, class_mask) = regs[i++];
			ID(e, driver_data) = regs[i++];
		}
		drv->id_table = id_table;
#undef ID
	}

	func_ret = __pci_register_driver(drv, THIS_MODULE, mod_name);

	fipc_set_reg0(_request, func_ret);

fail_alloc:
fail_alloc1:
fail_insert1:
fail_alloc2:
fail_dup1:
fail_dup2:
	return ret;
}


int pci_release_regions_callee(struct fipc_message *_request)
{
	int ret = 0;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	pci_release_regions(pdev);

	return ret;
}

int pci_request_regions_callee(struct fipc_message *_request)
{
	int type;
	int ret = 0;
	int func_ret;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	type = fipc_get_reg1(_request);

	func_ret = pci_request_regions(pdev,
				KBUILD_MODNAME);

	fipc_set_reg0(_request, func_ret);

	return ret;
}

int pci_set_master_callee(struct fipc_message *_request)
{
	int ret = 0;
	struct pci_dev_container *dev_container = NULL;
	struct pci_dev *pdev;

	glue_lookup_pdev_hash(__cptr(fipc_get_reg0(_request)),
			&dev_container);

	pdev = dev_container->pdev;

	pci_set_master(pdev);

	return ret;
}

#if 0
int finish_wait_callee(struct fipc_message *_request)
{
	struct __wait_queue_head *q = NULL;
	struct __wait_queue *wait = NULL;
	int ret = 0;
	q = kzalloc(sizeof( *q ),
		GFP_KERNEL);
	if (!q) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	wait = kzalloc(sizeof( *wait ),
		GFP_KERNEL);
	if (!wait) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	finish_wait(q,
			wait);
fail_alloc:
	return ret;

}

int prepare_to_wait_event_callee(struct fipc_message *_request)
{
	struct __wait_queue_head *q = NULL;
	struct __wait_queue *wait = NULL;
	int state = 0;
	int ret = 0;
	long func_ret = 0;
	q = kzalloc(sizeof( *q ),
		GFP_KERNEL);
	if (!q) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	wait = kzalloc(sizeof( *wait ),
		GFP_KERNEL);
	if (!wait) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	state = fipc_get_reg1(_request);
	func_ret = prepare_to_wait_event(q,
		wait,
		state);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int queue_delayed_work_on_callee(struct fipc_message *_request)
{
	int cpu = 0;
	struct workqueue_projection *wq = NULL;
	struct delayed_work *work = NULL;
	unsigned 	long delay = 0;
	int ret = 0;
	int func_ret = 0;
	wq = kzalloc(sizeof( *wq ),
		GFP_KERNEL);
	if (!wq) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	work = kzalloc(sizeof( *work ),
		GFP_KERNEL);
	if (!work) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	cpu = fipc_get_reg1(_request);
	delay = fipc_get_reg2(_request);
	func_ret = queue_delayed_work_on(cpu,
		wq,
		work,
		delay);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}

int queue_work_on_callee(struct fipc_message *_request)
{
	int cpu = 0;
	struct workqueue_projection *wq = NULL;
	struct work_struct *work = NULL;
	int ret = 0;
	int func_ret = 0;
	wq = kzalloc(sizeof( *wq ),
		GFP_KERNEL);
	if (!wq) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	work = kzalloc(sizeof( *work ),
		GFP_KERNEL);
	if (!work) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	cpu = fipc_get_reg1(_request);
	func_ret = queue_work_on(cpu,
		wq,
		work);
	fipc_set_reg1(_request,
			func_ret);
fail_alloc:
	return ret;

}
#endif

int request_threaded_irq_callee(struct fipc_message *_request)
{
	int ret = 0;
	int func_ret = 0;
	int irq;
	unsigned long flags;
	struct irqhandler_t_container *irqhandler_container;
	unsigned char *vector_name;
	void *data;

	irqhandler_container = kzalloc(sizeof(struct irqhandler_t_container),
					GFP_KERNEL);
	if (!irqhandler_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	vector_name = kzalloc(sizeof(unsigned long) * 3, GFP_KERNEL);

	if (!vector_name) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	irq = fipc_get_reg0(_request);
	irqhandler_container->other_ref.cptr = fipc_get_reg1(_request);
	flags = fipc_get_reg2(_request);
	data = (void*) fipc_get_reg6(_request);

	memcpy(vector_name, (void *)&_request->regs[3], sizeof(unsigned long) * 3);

	LIBLCD_MSG("%s, request_threaded_irq for %d | name: %s | data %p",
			__func__, irq, vector_name, data);

	func_ret = request_threaded_irq(irq, mei_irq_handler,
				NULL, flags,
				vector_name, (void*) irqhandler_container);

	printk("%s, storing in irq_map at idx: %d\n", __func__, reg_irqs);

	irq_map[reg_irqs].irq = irq;
	irq_map[reg_irqs].data = data;
	irq_map[reg_irqs].irqhandler_data = irqhandler_container;

	reg_irqs++;
	fipc_set_reg0(_request, func_ret);

fail_alloc:
	return ret;
}

int free_irq_callee(struct fipc_message *_request)
{
	unsigned int irq;
	struct irqhandler_t_container *irqhandler_container = NULL;
	int ret = 0;
	void *data;
	int i;

	irq = fipc_get_reg0(_request);
	data = (void*) fipc_get_reg1(_request);

	LIBLCD_MSG("%s, freeing irq %d | data %p", __func__, irq, data);

	for (i = 0; i < 32; i++) {
		if ((irq_map[i].irq == irq) && (irq_map[i].data == data)) {
			irqhandler_container = irq_map[i].irqhandler_data;
			break;
		}
	}

	if (!irqhandler_container) {
		printk("%s unable to retrieve container data for irq %d",
				__func__, irq);
		goto exit;
	}
	free_irq(irq, irqhandler_container);
	reg_irqs--;
exit:
	return ret;
}

int schedule_timeout_callee(struct fipc_message *_request)
{
	long timeout = 0;
	int ret = 0;
	long func_ret = 0;
	timeout = fipc_get_reg1(_request);
	func_ret = schedule_timeout(timeout);
	fipc_set_reg1(_request,
			func_ret);
	return ret;
}

#if 0
int __wake_up_callee(struct fipc_message *_request)
{
	struct __wait_queue_head *q = NULL;
	unsigned 	int mode = 0;
	int nr = 0;
	int key = 0;
	int ret = 0;
	q = kzalloc(sizeof( *q ),
		GFP_KERNEL);
	if (!q) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}
	mode = fipc_get_reg1(_request);
	nr = fipc_get_reg2(_request);
	key = fipc_get_reg3(_request);
	__wake_up(q,
			mode,
			nr,
			key);
fail_alloc:
	return ret;

}
#endif
