#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include "../../nvme_common.h"
#include "../nvme_caller.h"
#include <asm/lcd_domains/liblcd.h>

#include <lcd_config/post_hook.h>

static struct glue_cspace *c_cspace;

void *iocb_data_pool;
struct blk_mq_hw_ctx_container *ctx_container_g;
struct blk_mq_ops_container *ops_container_g;
struct blk_mq_tag_set *admin_tagset;
struct blk_mq_tag_set *io_tagset;

#ifdef IOMMU_ASSIGN
/* device for IOMMU assignment */
struct pcidev_info dev_assign = { 0x0000, 0x04, 0x00, 0x0 };
#endif

/* XXX: How to determine this? */
#define CPTR_HASH_BITS      5
static DEFINE_HASHTABLE(dev_table, CPTR_HASH_BITS);

extern struct blk_mq_ops_container nvme_mq_ops_container;
extern struct blk_mq_ops_container nvme_mq_admin_ops_container;

int glue_nvme_init(void)
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

	hash_init(dev_table);

	return 0;

fail2:
	glue_cap_exit();
fail1:
	return ret;
}

void glue_nvme_exit(void)
{
	glue_cap_destroy(c_cspace);
	glue_cap_exit();
}

int glue_insert_device(struct device_container *dev_c)
{
	BUG_ON(!dev_c->dev);

	dev_c->my_ref = __cptr((unsigned long)dev_c->dev);

	hash_add(dev_table, &dev_c->hentry, (unsigned long) dev_c->dev);

	return 0;
}

int glue_lookup_device(struct cptr c, struct
		device_container **dev_cout) {
	struct device_container *dev_c;

	hash_for_each_possible(dev_table, dev_c,
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

void complete(struct completion *x)
{
    // should never reach
    LIBLCD_ERR("Call to complete");
}

bool flush_work(struct work_struct *work)
{
    return false;
}

extern unsigned long wait_for_completion_io_timeout(struct completion *x,
						    unsigned long timeout)
{
    LIBLCD_ERR("Call to wait for completetion io timeout");
    return -1;
}

void blk_mq_stop_hw_queues(struct request_queue *rq)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *rq_container;

	rq_container = container_of(rq, struct request_queue_container,
				request_queue);

	async_msg_set_fn_type(_request, BLK_MQ_STOP_HW_QUEUES);
	fipc_set_reg0(_request, rq_container->other_ref.cptr);

	vmfunc_wrapper(_request);
}

int irq_set_affinity_hint(unsigned int irq, const struct cpumask *m)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;

	async_msg_set_fn_type(_request, IRQ_SET_AFFINITY_HINT);
	fipc_set_reg0(_request, irq);
	fipc_set_reg1(_request, (unsigned long) m);
	if (m)
		fipc_set_reg2(_request, m->bits[0]);

	vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

void blk_put_queue(struct request_queue *q)
{
	struct request_queue_container *q_container;
	struct fipc_message r;
	struct fipc_message *request = &r;

	q_container = container_of(q, struct request_queue_container,
					request_queue);

	async_msg_set_fn_type(request, BLK_PUT_QUEUE);
	fipc_set_reg0(request, q_container->other_ref.cptr);

	vmfunc_wrapper(request);

	return;
}

void blk_mq_update_nr_hw_queues(struct blk_mq_tag_set *set, int nr_hw_queues)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct blk_mq_tag_set_container *set_container;

	set_container = container_of(set, struct blk_mq_tag_set_container,
			tag_set);
	async_msg_set_fn_type(request, BLK_MQ_UPDATE_NR_HW_QUEUES);
	fipc_set_reg0(request, set_container->other_ref.cptr);
	fipc_set_reg1(request, nr_hw_queues);

	vmfunc_wrapper(request);
}

void blk_mq_tagset_busy_iter(struct blk_mq_tag_set *set,
		busy_tag_iter_fn *fn, void *priv)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct blk_mq_tag_set_container *set_container;

	set_container = container_of(set, struct blk_mq_tag_set_container,
			tag_set);

	async_msg_set_fn_type(request, BLK_MQ_TAGSET_BUSY_ITER);
	fipc_set_reg0(request, set_container->other_ref.cptr);

	vmfunc_wrapper(request);
}

void blk_mq_free_request(struct request *rq)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_container *req_container;
	struct request_queue_container *q_container;

	printk("%s, rq: %p q: %p" , __func__, rq, rq->q);

	req_container = container_of(rq, struct request_container,
				request);

	q_container = container_of(rq->q, struct request_queue_container,
					request_queue);

	async_msg_set_fn_type(_request, BLK_MQ_FREE_REQUEST);
	fipc_set_reg0(_request, req_container->other_ref.cptr);
	fipc_set_reg1(_request, q_container->other_ref.cptr);
	fipc_set_reg2(_request, rq->tag);

	rq->cmd_flags = 0;
#define REQ_ATOM_STARTED	1
	clear_bit(REQ_ATOM_STARTED, &rq->atomic_flags);

	vmfunc_wrapper(_request);
}

int create_rq_maps(struct blk_mq_tag_set *set);

int blk_mq_alloc_tag_set(struct blk_mq_tag_set *set)
{
	struct blk_mq_tag_set_container *set_container;
	struct blk_mq_ops_container *ops_container;
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;
	int func_ret = 0;

	set_container = container_of(set, struct blk_mq_tag_set_container, tag_set);
	ops_container = container_of(set->ops, struct blk_mq_ops_container, blk_mq_ops);

	ops_container->data = set->driver_data;

	/* Create local shadow rq_maps */
	create_rq_maps(set);

	/* save our tagset to admin tagset */
	if (ops_container == &nvme_mq_admin_ops_container)
		admin_tagset = set;
	else if (ops_container == &nvme_mq_ops_container)
		io_tagset = set;

	ret = glue_cap_insert_blk_mq_ops_type(c_cspace, ops_container,
			&ops_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert1;
	}

	ret = glue_cap_insert_blk_mq_tag_set_type(c_cspace, set_container,
			&set_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert2;
	}

	async_msg_set_fn_type(request, BLK_MQ_ALLOC_TAG_SET);

	fipc_set_reg0(request, set_container->my_ref.cptr);
	fipc_set_reg1(request, ops_container->my_ref.cptr);
	fipc_set_reg2(request, set->nr_hw_queues);
	fipc_set_reg3(request, set->queue_depth);
	fipc_set_reg4(request, (set->numa_node << 16) | (set->timeout));
	fipc_set_reg5(request, set->cmd_size);
	fipc_set_reg6(request, set->flags);

	vmfunc_wrapper(request);

	set_container->other_ref.cptr = fipc_get_reg0(request);
	ops_container->other_ref.cptr = fipc_get_reg1(request);
	func_ret = fipc_get_reg2(request);

	LIBLCD_MSG("%s received %d from", __func__, func_ret);

	return func_ret;

fail_insert1:
fail_insert2:
	return func_ret;
}

void blk_mq_complete_request(struct request *rq, int error)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue *q = rq->q;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_MQ_COMPLETE_REQUEST);
	fipc_set_reg0(_request, q_container->other_ref.cptr);
	fipc_set_reg1(_request, rq->tag);
	fipc_set_reg2(_request, error);

	vmfunc_wrapper(_request);

	return;
}

void blk_mq_start_stopped_hw_queues(struct request_queue *q, bool async)
{
	struct request_queue_container *q_container;
	struct fipc_message r;
	struct fipc_message *request = &r;

	q_container = container_of(q, struct request_queue_container, request_queue);

	async_msg_set_fn_type(request, BLK_MQ_START_STOPPED_HW_QUEUES);

	fipc_set_reg0(request, q_container->other_ref.cptr);
	fipc_set_reg1(request, async);

	vmfunc_wrapper(request);
}

void pci_restore_state(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);

	device_container = container_of(dev,
			struct pci_dev_container,
			pci_dev);

	async_msg_set_fn_type(request, PCI_RESTORE_STATE);
	fipc_set_reg0(request, device_container->other_ref.cptr);

	vmfunc_wrapper(request);
}

int pci_enable_pcie_error_reporting(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	int func_ret;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);
	async_msg_set_fn_type(_request, PCI_ENABLE_PCIE_ERROR_REPORTING);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int pci_cleanup_aer_uncorrect_error_status(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);
	async_msg_set_fn_type(_request,
			PCI_CLEANUP_AER_UNCORRECT_ERROR_STATUS);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

void pci_disable_device(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_DISABLE_DEVICE);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	return;
}

void pci_disable_msi(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_DISABLE_MSI);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	vmfunc_wrapper(_request);

	return;
}

void pci_disable_msix(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_DISABLE_MSIX);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	vmfunc_wrapper(_request);

	dev->msix_enabled = fipc_get_reg0(_request);

	return;
}

int pci_disable_pcie_error_reporting(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);

	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_DISABLE_PCIE_ERROR_REPORTING);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int pci_enable_device_mem(struct pci_dev *dev)
{
	int func_ret = 0;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_ENABLE_DEVICE_MEM);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int pci_enable_msix(struct pci_dev *dev, struct msix_entry *entries, int nvec)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	int sync_ret;
	unsigned 	long p_mem_sz;
	unsigned 	long p_offset;
	cptr_t p_cptr;
	struct pci_dev_container *device_container;
	INIT_IPC_MSG(&r);

	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_ENABLE_MSIX);

	fipc_set_reg0(_request, nvec);

	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long )entries),
		&p_cptr,
		&p_mem_sz,
		&p_offset);

	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}

	fipc_set_reg1(_request, ilog2((p_mem_sz) >> (PAGE_SHIFT)));
	fipc_set_reg2(_request, p_offset);
	fipc_set_reg3(_request, cptr_val(p_cptr));
	fipc_set_reg4(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);
	dev->msix_enabled = fipc_get_reg1(_request);

	return func_ret;
}

int pci_enable_msi_range(struct pci_dev *dev, int minvec, int maxvec)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);

	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_ENABLE_MSI_RANGE);

	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, minvec);
	fipc_set_reg2(_request, maxvec);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int pci_enable_msix_range(struct pci_dev *dev, struct msix_entry *entries, int
		minvec, int maxvec)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	int sync_ret;
	unsigned 	long p_mem_sz;
	unsigned 	long p_offset;
	cptr_t p_cptr;
	struct pci_dev_container *device_container;
	INIT_IPC_MSG(&r);

	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_ENABLE_MSIX_RANGE);

	fipc_set_reg0(_request, minvec);
	fipc_set_reg1(_request, maxvec);

	sync_ret = lcd_virt_to_cptr(__gva(( unsigned  long )entries),
		&p_cptr,
		&p_mem_sz,
		&p_offset);

	if (sync_ret) {
		LIBLCD_ERR("virt to cptr failed");
		lcd_exit(-1);
	}

	fipc_set_reg2(_request, ilog2((p_mem_sz) >> (PAGE_SHIFT)));
	fipc_set_reg3(_request, p_offset);
	fipc_set_reg4(_request, cptr_val(p_cptr));
	fipc_set_reg5(_request, device_container->other_ref.cptr);

	LIBLCD_MSG("%s pdev: %p sending pdev_other.cptr %lx", __func__,
			dev,
			device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);
	dev->msix_enabled = fipc_get_reg1(_request);

	return func_ret;
}

int __must_check __pci_register_driver(struct pci_driver *drv,
		struct module *owner,
		const char* name)
{
	struct pci_driver_container *drv_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	INIT_IPC_MSG(&r);

	drv_container = container_of(drv, struct pci_driver_container,
			pci_driver);
	ret = glue_cap_insert_pci_driver_type(c_cspace, drv_container,
			&drv_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, __PCI_REGISTER_DRIVER);
	fipc_set_reg0(_request, drv_container->my_ref.cptr);

	vmfunc_wrapper(_request);

	drv_container->other_ref.cptr = fipc_get_reg0(_request);
	func_ret = fipc_get_reg2(_request);

	LIBLCD_MSG("%s returned %d", __func__, func_ret);

	return func_ret;

fail_insert:
	return ret;
}

void pci_set_master(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);
	async_msg_set_fn_type(_request, PCI_SET_MASTER);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	return;
}

int pci_save_state(struct pci_dev *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);
	async_msg_set_fn_type(_request, PCI_SAVE_STATE);
	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

void pci_release_selected_regions(struct pci_dev *dev, int r)
{
	struct fipc_message req;
	struct fipc_message *_request = &req;
	struct pci_dev_container *device_container;

	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_RELEASE_SELECTED_REGIONS);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, r);

	vmfunc_wrapper(_request);

	return;
}

int pci_request_selected_regions(struct pci_dev *dev,
		int type,
		const char *reg)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_REQUEST_SELECTED_REGIONS);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, type);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int pci_select_bars(struct pci_dev *dev, unsigned long flags)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret = 0;
	struct pci_dev_container *device_container;

	INIT_IPC_MSG(&r);
	device_container = container_of(dev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, PCI_SELECT_BARS);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, flags);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

struct request_queue *blk_mq_init_queue(struct blk_mq_tag_set *set)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct blk_mq_tag_set_container *set_container;
	struct request_queue_container *rq_container;

	/* XXX Scary! request_queue size can vary from inside and outside
	 * LCDs? This is a bit fragile! */
	rq_container = kzalloc((sizeof(*rq_container)), GFP_KERNEL);

	if (!rq_container) {
		LIBLCD_ERR("kzalloc failed");
		goto fail_alloc;
	}

	ret = glue_cap_insert_request_queue_type(c_cspace, rq_container,
			&rq_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	set_container = container_of(set, struct blk_mq_tag_set_container,
			tag_set);

	async_msg_set_fn_type(request, BLK_MQ_INIT_QUEUE);
	fipc_set_reg0(request, set_container->other_ref.cptr);
	fipc_set_reg1(request, rq_container->my_ref.cptr);

	vmfunc_wrapper(request);

	rq_container->other_ref.cptr = fipc_get_reg0(request);

	printk("%s returns local request queue struct!! \n", __func__);
	printk("%s, storing other_ref.cptr %lx", __func__, fipc_get_reg0(request));
	ctx_container_g->blk_mq_hw_ctx.queue = &rq_container->request_queue;
	return &rq_container->request_queue;

fail_insert:
	kfree(rq_container);
fail_alloc:
	return NULL;
}

bool blk_get_queue(struct request_queue *rq)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *rq_container;
	int ret;
	int func_ret;

	rq_container = container_of(rq, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_GET_QUEUE);
	fipc_set_reg0(_request, rq_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

void blk_cleanup_queue(struct request_queue *q)
{
	struct request_queue_container *rq_container;
	struct fipc_message r;
	struct fipc_message *request = &r;

	rq_container = container_of(q, struct request_queue_container,
						request_queue);
	async_msg_set_fn_type(request, BLK_CLEANUP_QUEUE);

	fipc_set_reg0(request, rq_container->other_ref.cptr);

	vmfunc_wrapper(request);

	glue_cap_remove(c_cspace, rq_container->my_ref);
	kfree(rq_container);
	return;
}

void blk_mq_end_request(struct request *rq, int error)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue *q = rq->q;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_MQ_END_REQUEST);
	fipc_set_reg0(_request, q_container->other_ref.cptr);
	fipc_set_reg1(_request, rq->tag);
	fipc_set_reg2(_request, error);

	vmfunc_wrapper(_request);
	printk("%s, update_request returns %d", __func__,
			blk_update_request(rq, error, blk_rq_bytes(rq)));
	return;
}

void blk_mq_free_tag_set(struct blk_mq_tag_set *set)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct blk_mq_tag_set_container *set_container;
	struct blk_mq_ops_container *ops_container;

	set_container = container_of(set, struct blk_mq_tag_set_container, tag_set);

	ops_container = container_of(set->ops, struct blk_mq_ops_container,
				blk_mq_ops);

	async_msg_set_fn_type(request, BLK_MQ_FREE_TAG_SET);

	fipc_set_reg0(request, set_container->other_ref.cptr);
	fipc_set_reg1(request, ops_container->other_ref.cptr);

	vmfunc_wrapper(request);

	glue_cap_remove(c_cspace, set_container->my_ref);
	glue_cap_remove(c_cspace, ops_container->my_ref);
	return;
}

void pci_unregister_driver(struct pci_driver *drv)
{
	struct pci_driver_container *drv_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	INIT_IPC_MSG(&r);

	drv_container = container_of(drv,
		struct pci_driver_container,
		pci_driver);

	async_msg_set_fn_type(_request, PCI_UNREGISTER_DRIVER);
	fipc_set_reg0(_request, drv_container->other_ref.cptr);

	vmfunc_wrapper(_request);

	glue_cap_remove(c_cspace, drv_container->my_ref);

	return;
}

bool pci_device_is_present(struct pci_dev *pdev)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct pci_dev_container *device_container;
	bool func_ret;

	INIT_IPC_MSG(&r);

	device_container = container_of(pdev,
			struct pci_dev_container,
			pci_dev);

	async_msg_set_fn_type(request, PCI_DEVICE_IS_PRESENT);
	fipc_set_reg0(request, device_container->other_ref.cptr);

	vmfunc_wrapper(request);
	func_ret = fipc_get_reg0(request);

	return func_ret;
}

void blk_mq_start_request(struct request *rq)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue *q = rq->q;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_MQ_START_REQUEST);
	fipc_set_reg0(_request, q_container->other_ref.cptr);
	fipc_set_reg1(_request, rq->tag);

	vmfunc_wrapper(_request);

	return;
}

struct blk_mq_hw_ctx *blk_mq_map_queue(struct request_queue *rq, int ctx_index)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct blk_mq_hw_ctx *func_ret = NULL;

	async_msg_set_fn_type(request, BLK_MQ_MAP_QUEUE);
	fipc_set_reg0(request, ctx_index);

	vmfunc_wrapper(request);

	/* TODO: sync wrapper */

	return func_ret;
}

void blk_queue_logical_block_size(struct request_queue *q, unsigned short size)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
					request_queue);

	async_msg_set_fn_type(request, BLK_QUEUE_LOGICAL_BLOCK_SIZE);
	fipc_set_reg0(request, size);
	fipc_set_reg1(request, q_container->other_ref.cptr);

	vmfunc_wrapper(request);
	q->limits.logical_block_size = fipc_get_reg0(request);
	q->limits.physical_block_size = fipc_get_reg1(request);
	q->limits.io_min = fipc_get_reg2(request);

	return;
}

void blk_execute_rq_nowait(struct request_queue *q, struct gendisk *bd_disk,
			   struct request *rq, int at_head,
			   rq_end_io_fn *done)
{
    //TODO
    struct fipc_message r;
	struct fipc_message *request = &r;


    async_msg_set_fn_type(request, BLK_EXECUTE_RQ_NOWAIT);

    vmfunc_wrapper(request);

}

struct gendisk *alloc_disk_node(int minors, int node_id)
{
	struct gendisk_container *disk_container;
	struct fipc_message r;
	struct fipc_message *request = &r;
	int ret;

	disk_container = kzalloc(sizeof(struct gendisk_container), GFP_KERNEL);

	if (!disk_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_insert_gendisk_type(c_cspace, disk_container,
			&disk_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	async_msg_set_fn_type(request, ALLOC_DISK);
	fipc_set_reg0(request, minors);
	fipc_set_reg1(request, node_id);
	fipc_set_reg2(request, disk_container->my_ref.cptr);

	vmfunc_wrapper(request);

	disk_container->other_ref.cptr = fipc_get_reg0(request);

	return &disk_container->gendisk;

fail_insert:
	kfree(disk_container);
fail_alloc:
	return NULL;
}

void device_release_driver(struct device *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct pci_dev_container *device_container;
	struct pci_dev *pdev;

	INIT_IPC_MSG(&r);

	pdev = container_of(dev, struct pci_dev,
				dev);
	device_container = container_of(pdev,
				struct pci_dev_container,
				pci_dev);

	async_msg_set_fn_type(_request, DEVICE_RELEASE_DRIVER);

	fipc_set_reg0(_request, device_container->other_ref.cptr);

	vmfunc_wrapper(_request);
}

void device_add_disk(struct device *parent, struct gendisk *disk)
{
	struct gendisk_container *disk_container;
	struct block_device_operations_container *blo_container;
	struct request_queue_container *rq_container;
	struct device_container *device_container = NULL;
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;

	disk_container = container_of(disk, struct gendisk_container, gendisk);

	glue_lookup_device(__cptr((unsigned long)parent), &device_container);

	blo_container = container_of(disk->fops,
				struct block_device_operations_container,
				block_device_operations);

	rq_container = container_of(disk->queue, struct request_queue_container,
				request_queue);

	ret = glue_cap_insert_gendisk_type(c_cspace, disk_container,
			&disk_container->my_ref);
	if (ret) {
		 LIBLCD_ERR("lcd insert");
		 goto fail_insert2;
	}

	ret = glue_cap_insert_blk_dev_ops_type(c_cspace, blo_container,
			&blo_container->my_ref);
	if(ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert3;
	}

	async_msg_set_fn_type(request, DEVICE_ADD_DISK);

	fipc_set_reg0(request, device_container->other_ref.cptr);
	fipc_set_reg1(request, disk_container->other_ref.cptr);
	fipc_set_reg2(request, blo_container->my_ref.cptr);
	fipc_set_reg3(request, rq_container->other_ref.cptr);
	fipc_set_reg4(request, disk->flags);
	fipc_set_reg5(request, get_capacity(disk));
	memcpy((void*) request->regs[6], disk->disk_name, sizeof(unsigned long));
	//fipc_set_reg5(request, disk->major);
	//fipc_set_reg6(request, disk->first_minor);

	/*
	 * Ran out of registers to marshall the string, so hardcoding it
	 * in the klcd
	 */
	vmfunc_wrapper(request);

	blo_container->other_ref.cptr = fipc_get_reg0(request);
	return;

fail_insert3:
fail_insert2:
	return;
}

void put_disk(struct gendisk *disk)
{
	struct fipc_message r;
	struct fipc_message *request = &r;

	struct gendisk_container *disk_container;
	struct block_device_operations_container *blo_container;

	disk_container = container_of(disk, struct gendisk_container, gendisk);

	blo_container = container_of(disk->fops,
			struct block_device_operations_container, block_device_operations);

	async_msg_set_fn_type(request, PUT_DISK);

	fipc_set_reg0(request, disk_container->other_ref.cptr);
	fipc_set_reg1(request, blo_container->other_ref.cptr);

	vmfunc_wrapper(request);

	glue_cap_remove(c_cspace, disk_container->my_ref);
	glue_cap_remove(c_cspace, blo_container->my_ref);

	kfree(disk_container);
	return;
}

void del_gendisk(struct gendisk *gp)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct gendisk_container *disk_container;

	disk_container = container_of(gp, struct gendisk_container, gendisk);

	async_msg_set_fn_type(request, DEL_GENDISK);
	fipc_set_reg0(request, disk_container->other_ref.cptr);

	vmfunc_wrapper(request);

	return;
}

int register_blkdev(unsigned int devno, const char *name)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;

	async_msg_set_fn_type(request, REGISTER_BLKDEV);
	fipc_set_reg0(request, devno);

	vmfunc_wrapper(request);

        ret = fipc_get_reg0(request);

	return ret;
}

void unregister_blkdev(unsigned int devno, const char *name)
{
	struct fipc_message r;
	struct fipc_message *request = &r;

	LIBLCD_MSG("unreg blkdev caller glue");

	async_msg_set_fn_type(request, UNREGISTER_BLKDEV);

	fipc_set_reg0(request, devno);
	vmfunc_wrapper(request);

	return;
}

int msix_vector_handler_callee(struct fipc_message *_request)
{
	struct irqhandler_t_container *irqhandler_container;
	irqreturn_t irqret;
	int irq;
	int ret = 0;

	irq = fipc_get_reg0(_request);
	irqhandler_container = (struct irqhandler_t_container *)
		fipc_get_reg1(_request);

	/* call real irq handler */
	irqret = irqhandler_container->irqhandler(irq,
			irqhandler_container->data);

	//printk("%s, irq:%d irqret %d\n", __func__, irq, irqret);
	fipc_set_reg0(_request, irqret);
	return ret;
}

int request_threaded_irq(unsigned int irq, irq_handler_t handler,
				irq_handler_t thread_fn,
				unsigned long flags, const char *name, void *dev)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct irqhandler_t_container *irqhandler_container;
	int func_ret;

	INIT_IPC_MSG(&r);
	printk("%s, irq # %d", __func__, irq);

	irqhandler_container = kzalloc(sizeof(struct irqhandler_t_container), GFP_KERNEL);

	if (!irqhandler_container) {
		LIBLCD_ERR("kzalloc");
		func_ret = -ENOMEM;
		goto fail_alloc;
	}

	irqhandler_container->irqhandler = handler;
	/*
	 * XXX: in irq context, we don't want to be doing a lookup.
	 * so save the handler_container pointer as myref
	 */
	irqhandler_container->my_ref.cptr = (unsigned long) irqhandler_container;
	irqhandler_container->data = dev;

	async_msg_set_fn_type(_request, REQUEST_THREADED_IRQ);
	fipc_set_reg0(_request, irq);
	fipc_set_reg1(_request, irqhandler_container->my_ref.cptr);
	fipc_set_reg2(_request, flags);
	/* reg3, reg4, reg5 */
	memcpy((void*)&_request->regs[3], name, sizeof(unsigned long) * 3);
	fipc_set_reg6(_request, (unsigned long)  dev);

	LIBLCD_MSG("%s, irq %d | data %p", __func__, irq, dev);

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	LIBLCD_MSG("%s returned %d", __func__, func_ret);
fail_alloc:
	return func_ret;
}

void free_irq(unsigned int irq, void *dev_id)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;

	INIT_IPC_MSG(&r);
	async_msg_set_fn_type(_request, FREE_IRQ);

	fipc_set_reg0(_request, irq);
	fipc_set_reg1(_request, (unsigned long) dev_id);

	LIBLCD_MSG("%s, freeing irq %d | data %p", __func__, irq, dev_id);
	vmfunc_wrapper(_request);

	return;
}

struct device *get_device(struct device *dev)
{
	struct device_container *device_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	glue_lookup_device(__cptr((unsigned long)dev), &device_container);

	async_msg_set_fn_type(_request, GET_DEVICE);
	fipc_set_reg0(_request, device_container->other_ref.cptr);
	fipc_set_reg1(_request, device_container->my_ref.cptr);

	ret = vmfunc_wrapper(_request);
	return dev;
}

void put_device(struct device *dev)
{
	struct device_container *device_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	glue_lookup_device(__cptr((unsigned long)dev), &device_container);

	async_msg_set_fn_type(_request, PUT_DEVICE);

	fipc_set_reg0(_request, device_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	return;
}

u64 dma_mask = 0;
void *data_pool;
#define PCI_CLASS_STORAGE_EXPRESS	0x010802

struct pci_dev *g_pdev;

int probe_callee(struct fipc_message *_request)
{
	struct pci_dev_container *dev_container;
	struct pci_driver_container *pdrv_container;
	struct pci_device_id *id;
	struct device_container *device_container;

	int func_ret;
	int ret = 0;
	unsigned int devfn;

	cptr_t res0_cptr;
	gpa_t gpa_addr;
	unsigned int res0_len;
	volatile void *dev_resource_0;
	cptr_t pool_cptr;
	gva_t pool_addr;
	unsigned int pool_ord;

	LIBLCD_MSG("%s called", __func__);

	ret = glue_cap_lookup_pci_driver_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &pdrv_container);

	if (ret) {
		LIBLCD_ERR("fail lookup");
		goto fail_lookup;
	}

	dma_mask = fipc_get_reg2(_request);

	dev_container = kzalloc(sizeof( struct pci_dev_container   ),
		GFP_KERNEL);
	if (!dev_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_insert_pci_dev_type(c_cspace,
		dev_container,
		&dev_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	device_container = kzalloc(sizeof( struct device_container   ),
		GFP_KERNEL);
	if (!device_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	device_container->dev = &dev_container->pci_dev.dev;

	glue_insert_device(device_container);

	device_container->other_ref.cptr = fipc_get_reg2(_request);

	id = kzalloc(sizeof(*id), GFP_KERNEL);

	if (!id) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	id->device = LOWER_HALF(fipc_get_reg3(_request));
	id->vendor = UPPER_HALF(fipc_get_reg3(_request));

	id->subdevice = LOWER_HALF(fipc_get_reg4(_request));
	id->subvendor = UPPER_HALF(fipc_get_reg4(_request));

	id->class_mask = LOWER_HALF(fipc_get_reg5(_request));
	id->class = UPPER_HALF(fipc_get_reg5(_request));

	id->driver_data = UPPER_HALF(fipc_get_reg6(_request));

	dev_container->pci_dev.dev.kobj.name = "nvme_lcd";
	dev_container->pci_dev.vendor = PCI_ANY_ID;
	dev_container->pci_dev.device = PCI_ANY_ID;
	dev_container->other_ref.cptr = fipc_get_reg1(_request);
	dev_container->pci_dev.dev.dma_mask = &dma_mask;
	atomic_set(&dev_container->pci_dev.enable_cnt,
			LOWER_HALF(fipc_get_reg6(_request)));

	g_pdev = &dev_container->pci_dev;

	/* remap pci_resource 0 */
	ret = lcd_cptr_alloc(&res0_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_cptr;
	}

	ret = lcd_cptr_alloc(&pool_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_cptr;
	}

	LIBLCD_MSG("%s pdev: %p sending pdev_other.cptr %lx", __func__,
			g_pdev,
			dev_container->other_ref.cptr);

	fipc_set_reg0(_request, cptr_val(res0_cptr));
	fipc_set_reg1(_request, cptr_val(pool_cptr));
	fipc_set_reg2(_request, dev_container->other_ref.cptr);

	vmfunc_sync_call(_request, SYNC_PROBE);

	res0_len = fipc_get_reg0(_request);
	pool_ord = fipc_get_reg1(_request);

	ret = lcd_ioremap_phys(res0_cptr, res0_len, &gpa_addr);

	if (ret) {
		LIBLCD_ERR("failed to ioremap phys");
		goto fail_ioremap;
	}

	dev_resource_0 = lcd_ioremap(gpa_val(gpa_addr), res0_len);

	if (!dev_resource_0) {
		LIBLCD_ERR("failed to ioremap virt");
		goto fail_ioremap2;
	}
	dev_container->pci_dev.resource[0].start = (resource_size_t) dev_resource_0;
	dev_container->pci_dev.resource[0].end =
		(resource_size_t)((char*)dev_resource_0 + res0_len - 1);

	LIBLCD_MSG("%s: NVME_REG_VS: 0x%X", __func__,
			*(unsigned int *)((char*)dev_resource_0 + NVME_REG_VS));

	ret = lcd_map_virt(pool_cptr, pool_ord, &pool_addr);

	if (ret) {
		LIBLCD_ERR("failed to map pool");
		goto fail_pool;
	}

	LIBLCD_MSG("%s, mapping private pool %p | ord %d", __func__,
			gva_val(pool_addr), pool_ord);

	data_pool = (void*)gva_val(pool_addr);

	devfn = PCI_DEVFN(dev_assign.slot, dev_assign.fn);

	/*
	 * assign the device after we map the memory so that
	 * all the rammapped pages are automatically keyed
	 * into the iommu hardware
	 */
	ret = lcd_syscall_assign_device(dev_assign.domain,
				dev_assign.bus,
				devfn);
	if (ret)
		LIBLCD_ERR("Could not assign pci device to LCD: ret %d",
				ret);

	func_ret = pdrv_container->pci_driver.probe(&dev_container->pci_dev, id);

	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, dev_container->my_ref.cptr);

fail_ioremap:
fail_ioremap2:
fail_alloc:
fail_insert:
fail_cptr:
fail_pool:
fail_lookup:
	return ret;
}

int remove_callee(struct fipc_message *_request)
{
	struct pci_driver_container *pdrv_container;
	struct pci_dev_container *pdev_container;
	int ret;

	ret = glue_cap_lookup_pci_driver_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &pdrv_container);

	if (ret) {
		LIBLCD_ERR("fail lookup");
		goto fail_lookup;
	}

	ret = glue_cap_lookup_pci_dev_type(c_cspace,
			__cptr(fipc_get_reg1(_request)), &pdev_container);

	if (ret) {
		LIBLCD_ERR("fail lookup");
		goto fail_lookup;
	}

	pdrv_container->pci_driver.remove(&pdev_container->pci_dev);

fail_lookup:
	return ret;
}

extern int nvme_poll(struct blk_mq_hw_ctx *hctx, unsigned int tag);

int poll_callee(struct fipc_message *_request)
{
	int tag;
	struct blk_mq_hw_ctx_container *hctx_c = NULL;
	struct blk_mq_hw_ctx *hctx;
	struct blk_mq_ops_container *ops_container;
	int ret;

	ret = glue_cap_lookup_blk_mq_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container);
	if(ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	ret = glue_cap_lookup_blk_mq_hw_ctx_type(c_cspace,
			__cptr(fipc_get_reg1(_request)), &hctx_c);

	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	hctx = &hctx_c->blk_mq_hw_ctx;

	tag = fipc_get_reg2(_request);

	ret = nvme_poll(hctx, tag);

	fipc_set_reg0(_request, ret);

fail_lookup:
	return 0;
}

int nvme_queue_rq(struct blk_mq_hw_ctx *hctx,
			 const struct blk_mq_queue_data *bd);

struct request *get_rq_from_tagset(struct blk_mq_ops_container *ops_container,
			int tag, int qnum)
{
	struct request *rq = NULL;

	if (ops_container == &nvme_mq_ops_container) {
		rq = blk_mq_tag_to_rq(io_tagset->tags[qnum], tag);
	} else if (ops_container == &nvme_mq_admin_ops_container) {
		assert(qnum == 0);
		rq = blk_mq_tag_to_rq(admin_tagset->tags[qnum], tag);
	}
	return rq;
}

int queue_rq_fn_callee(struct fipc_message *request)
{
	struct blk_mq_hw_ctx_container *ctx_container;
	struct blk_mq_ops_container *ops_container;
	struct blk_mq_queue_data bd;
	struct request *rq;
	int tag;
	int qnum;
	int ret = 0;
	int func_ret = 0;

	ret = glue_cap_lookup_blk_mq_hw_ctx_type(c_cspace,
			__cptr(fipc_get_reg1(request)), &ctx_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	qnum = ctx_container->blk_mq_hw_ctx.queue_num = fipc_get_reg0(request);

	ret = glue_cap_lookup_blk_mq_ops_type(c_cspace,
			__cptr(fipc_get_reg2(request)), &ops_container);
	if (ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	tag = fipc_get_reg3(request);

	rq = get_rq_from_tagset(ops_container, tag, qnum);

	rq->tag = tag;
	bd.rq = rq;

	printk("%s, hctx: %p tag %d", __func__, &ctx_container->blk_mq_hw_ctx,
						tag);
	func_ret =
		ops_container->blk_mq_ops.queue_rq(&ctx_container->blk_mq_hw_ctx,
				&bd);

	LIBLCD_MSG("%s, returned ", __func__, func_ret);
	fipc_set_reg0(request, func_ret);

fail_lookup:
	return ret;
}

int map_queue_fn_callee(struct fipc_message *request)
{
	struct request_queue_container *req_queue_container;
	int cpu;
	int err;

	struct blk_mq_hw_ctx *func_ret;

	err = glue_cap_lookup_request_queue_type(c_cspace,
			__cptr(fipc_get_reg4(request)), &req_queue_container);
	if (err) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}
	cpu = fipc_get_reg1(request);

	func_ret = kzalloc(sizeof( *func_ret ), GFP_KERNEL);

	if (!func_ret) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	/* TODO sync wrapper */
#if 0
	func_ret = map_queue_fn(( &req_queue_container->request_queue ), cpu);
#endif
fail_lookup:
fail_alloc:
	return err;
}

int init_hctx_fn_callee(struct fipc_message *request)
{
	struct blk_mq_hw_ctx_container *ctx_container;
	struct blk_mq_ops_container *ops_container;
	unsigned int index;
	int ret = 0;
	void *driver_data;

	ret = glue_cap_lookup_blk_mq_ops_type(c_cspace,
			__cptr(fipc_get_reg0(request)), &ops_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	ctx_container = kzalloc(sizeof(*ctx_container), GFP_KERNEL);
	if (!ctx_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ctx_container_g = ctx_container;
	ctx_container->other_ref.cptr = fipc_get_reg1(request);

	ret = glue_cap_insert_blk_mq_hw_ctx_type(c_cspace, ctx_container,
			&ctx_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	index = fipc_get_reg2(request);

	driver_data = ops_container->data;

	ret = ops_container->blk_mq_ops.init_hctx(&ctx_container->blk_mq_hw_ctx,
				driver_data, index);
	if(ret) {
	        LIBLCD_ERR("call to init_hctx failed");
                goto fail_hctx;
	}

	fipc_set_reg0(request, ctx_container->my_ref.cptr);
	fipc_set_reg1(request, ret);
	return ret;

fail_hctx:
	glue_cap_remove(c_cspace, ctx_container->my_ref);
fail_insert:
	kfree(ctx_container);
fail_alloc:
fail_lookup:
	return ret;
}

int complete_fn_callee(struct fipc_message *_request)
{
	struct request *rq;
	struct blk_mq_ops_container *ops_container;
	int tag;
	int ret;
	int qnum;

	ret = glue_cap_lookup_blk_mq_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	tag = fipc_get_reg1(_request);
	qnum = fipc_get_reg2(_request);

	rq = get_rq_from_tagset(ops_container, tag, qnum);

	ops_container->blk_mq_ops.complete(rq);

fail_lookup:
	return ret;
}

int exit_hctx_fn_callee(struct fipc_message *_request)
{
	struct blk_mq_hw_ctx_container *ctx_container;
	struct blk_mq_ops_container *ops_container;
	unsigned int index;
	int ret = 0;

	ret = glue_cap_lookup_blk_mq_ops_type(c_cspace,
			__cptr(fipc_get_reg0(_request)), &ops_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	ret = glue_cap_lookup_blk_mq_hw_ctx_type(c_cspace,
			__cptr(fipc_get_reg1(_request)),
			&ctx_container);
	if (ret) {
		LIBLCD_ERR("lcd lookup");
		goto fail_lookup;
	}

	index = fipc_get_reg2(_request);

	if (ops_container->blk_mq_ops.exit_hctx)
		ops_container->blk_mq_ops.exit_hctx(&ctx_container->blk_mq_hw_ctx,
				index);
fail_lookup:
	return ret;
}

int init_request_fn_callee(struct fipc_message *_request)
{
	struct request *rq;
	int tag;
	unsigned int hctx_idx;
	unsigned int rq_idx;
	unsigned int numa_node;
	struct blk_mq_ops_container *ops_container;
	int func_ret;
	int ret;

	ret = glue_cap_lookup_blk_mq_ops_type(c_cspace,
			__cptr(fipc_get_reg4(_request)), &ops_container);
	if(ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	tag = fipc_get_reg0(_request);
	hctx_idx = fipc_get_reg1(_request);
	rq_idx = fipc_get_reg2(_request);
	numa_node = fipc_get_reg3(_request);

	rq = get_rq_from_tagset(ops_container, tag, hctx_idx);

	func_ret = ops_container->blk_mq_ops.init_request(ops_container->data,
			rq, hctx_idx, rq_idx, numa_node);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return 0;
}

int poll_fn_callee(struct fipc_message *_request)
{
    /*
	struct request *rq;
	struct request_container *rq_c;

	null_softirq_done_fn(rq);
    */

	printk("%s, called", __func__);
	return 0;
}

int timeout_fn_callee(struct fipc_message *_request)
{
	printk("%s, called", __func__);
	return 0;
}

void __unregister_chrdev(unsigned int major, unsigned int baseminor, unsigned
		int count, const char *name)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request, __UNREGISTER_CHRDEV);
	fipc_set_reg0(_request, major);
	fipc_set_reg1(_request, baseminor);
	fipc_set_reg2(_request, count);
	ret = vmfunc_wrapper(_request);
	return;
}

int __register_chrdev(unsigned int major,
		unsigned int baseminor,
		unsigned int count,
		const char *name,
		const struct file_operations *fops)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct file_operations_container *fops_container;
	int func_ret;
	int ret;

	fops_container = container_of(fops, struct file_operations_container, fops);

	ret = glue_cap_insert_file_operations_type(c_cspace, fops_container,
				&fops_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, __REGISTER_CHRDEV);
	fipc_set_reg0(_request, fops_container->my_ref.cptr);
	fipc_set_reg1(_request, major);
	fipc_set_reg2(_request, baseminor);
	fipc_set_reg3(_request, count);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	fops_container->other_ref.cptr = fipc_get_reg0(_request);
	return func_ret;
fail_insert:
	return ret;
}

void ida_destroy(struct ida *ida)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct ida_container *ida_container = NULL;
	int ret;

	ida_container = container_of(ida, struct ida_container, ida);

	async_msg_set_fn_type(_request, IDA_DESTROY);
	fipc_set_reg0(_request, ida_container->other_ref.cptr);
	ret = vmfunc_wrapper(_request);
	return;
}

int ida_get_new_above(struct ida *ida,
		int starting_id,
		int *p_id)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct ida_container *ida_container = NULL;
	int func_ret;
	int ret;

	ida_container = container_of(ida, struct ida_container, ida);
	async_msg_set_fn_type(_request, IDA_GET_NEW_ABOVE);
	fipc_set_reg0(_request, ida_container->other_ref.cptr);
	fipc_set_reg1(_request, starting_id);

	ret = vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);
	*p_id = fipc_get_reg1(_request);

	return func_ret;
}

void ida_init(struct ida *ida)
{
	struct ida_container *ida_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;

	ida_container = container_of(ida, struct ida_container, ida);

	ret = glue_cap_insert_ida_type(c_cspace, ida_container,
			&ida_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, IDA_INIT);
	fipc_set_reg0(_request, ida_container->my_ref.cptr);
	ret = vmfunc_wrapper(_request);
	ida_container->other_ref.cptr = fipc_get_reg0(_request);
fail_insert:
	return;
}

int ida_pre_get(struct ida *ida,
		gfp_t gfp_mask)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct ida_container *ida_container = NULL;
	int ret;
	int func_ret;

	ida_container = container_of(ida, struct ida_container, ida);

	ret = glue_cap_insert_ida_type(c_cspace, ida_container,
			&ida_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, IDA_PRE_GET);
	fipc_set_reg0(_request, ida_container->my_ref.cptr);
	fipc_set_reg1(_request, gfp_mask);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	ida_container->other_ref.cptr = fipc_get_reg1(_request);
	return func_ret;
fail_insert:
	return ret;
}

void ida_remove(struct ida *ida, int id)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct ida_container *ida_container = NULL;
	int ret;

	ida_container = container_of(ida, struct ida_container, ida);
	async_msg_set_fn_type(_request, IDA_REMOVE);

	fipc_set_reg0(_request, ida_container->other_ref.cptr);
	fipc_set_reg1(_request, id);
	ret = vmfunc_wrapper(_request);
	return;
}

int ida_simple_get(struct ida *ida,
		unsigned int start,
		unsigned int end,
		gfp_t gfp_mask)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct ida_container *ida_container = NULL;
	int ret;
	int func_ret;

	async_msg_set_fn_type(_request, IDA_SIMPLE_GET);
	ida_container = container_of(ida, struct ida_container, ida);

	fipc_set_reg0(_request, ida_container->other_ref.cptr);
	fipc_set_reg1(_request, start);
	fipc_set_reg2(_request, end);
	fipc_set_reg3(_request, gfp_mask);

	ret = vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

void ida_simple_remove(struct ida *ida, unsigned int id)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct ida_container *ida_container = NULL;

	async_msg_set_fn_type(_request, IDA_SIMPLE_REMOVE);

	ida_container = container_of(ida, struct ida_container, ida);

	fipc_set_reg0(_request, ida_container->other_ref.cptr);
	fipc_set_reg1(_request, id);
	ret = vmfunc_wrapper(_request);

	return;
}

void blk_set_queue_dying(struct request_queue *q)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_SET_QUEUE_DYING);
	fipc_set_reg0(_request, q_container->other_ref.cptr);
	ret = vmfunc_wrapper(_request);
	return;

}

int blk_rq_unmap_user(struct bio *bio)
{
	int ret;
	int func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			BLK_RQ_UNMAP_USER);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;

}

void blk_queue_write_cache(struct request_queue *q,
		bool wc,
		bool fua)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_QUEUE_WRITE_CACHE);
	fipc_set_reg0(_request, q_container->other_ref.cptr);
	fipc_set_reg1(_request, wc);
	fipc_set_reg2(_request, fua);
	ret = vmfunc_wrapper(_request);
	return;
}

void blk_queue_virt_boundary(struct request_queue *q,
		unsigned long mask)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_QUEUE_VIRT_BOUNDARY);
	fipc_set_reg0(_request, q_container->other_ref.cptr);
	fipc_set_reg1(_request, mask);
	ret = vmfunc_wrapper(_request);

	q->limits.virt_boundary_mask = mask;
	return;
}

void blk_queue_max_segments(struct request_queue *q,
		unsigned short max_segments)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_QUEUE_MAX_SEGMENTS);
	fipc_set_reg0(_request, q_container->other_ref.cptr);
	fipc_set_reg1(_request, max_segments);

	ret = vmfunc_wrapper(_request);

	q->limits.max_segments = max_segments;
	return;

}

void blk_queue_max_hw_sectors(struct request_queue *q,
		unsigned int max_hw_sectors)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_QUEUE_MAX_HW_SECTORS);
	fipc_set_reg0(_request, q_container->other_ref.cptr);
	fipc_set_reg1(_request, max_hw_sectors);
	ret = vmfunc_wrapper(_request);
	q->limits.max_hw_sectors = max_hw_sectors;
	return;
}

void blk_queue_max_discard_sectors(struct request_queue *q,
		unsigned int max_discard_sectors)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_QUEUE_MAX_DISCARD_SECTORS);
	fipc_set_reg0(_request, q_container->other_ref.cptr);
	fipc_set_reg1(_request, max_discard_sectors);
	ret = vmfunc_wrapper(_request);
	return;

}

void blk_queue_chunk_sectors(struct request_queue *q,
		unsigned int chunk_sectors)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_QUEUE_CHUNK_SECTORS);
	fipc_set_reg0(_request, q_container->other_ref.cptr);
	fipc_set_reg1(_request, chunk_sectors);
	ret = vmfunc_wrapper(_request);

	q->limits.chunk_sectors = chunk_sectors;

	return;
}

void blk_mq_unfreeze_queue(struct request_queue *q)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_MQ_UNFREEZE_QUEUE);
	fipc_set_reg0(_request, q_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	return;
}

void blk_mq_requeue_request(struct request *rq)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			BLK_MQ_REQUEUE_REQUEST);
	ret = vmfunc_wrapper(_request);
	return;

}

int blk_mq_request_started(struct request *rq)
{
	int func_ret;
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	async_msg_set_fn_type(_request,
			BLK_MQ_REQUEST_STARTED);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;

}

void blk_mq_kick_requeue_list(struct request_queue *q)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_MQ_KICK_REQUEUE_LIST);
	fipc_set_reg0(_request, q_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	return;
}

void blk_mq_freeze_queue(struct request_queue *q)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_MQ_FREEZE_QUEUE);
	fipc_set_reg0(_request, q_container->other_ref.cptr);
	ret = vmfunc_wrapper(_request);
	return;
}

void blk_mq_cancel_requeue_work(struct request_queue *q)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_MQ_CANCEL_REQUEUE_WORK);
	fipc_set_reg0(_request, q_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	return;
}

struct request *blk_mq_alloc_request_hctx(struct request_queue *rq,
		int rw,
		unsigned int flags,
		unsigned int hctx_idx)
{
	int ret;
	struct request *func_ret;
	struct request_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *rq_container;

	rq_container = container_of(rq, struct request_queue_container,
			request_queue);

	func_ret_container = kzalloc(sizeof( struct request_container   ),
			GFP_KERNEL);

	if (!func_ret_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	func_ret = &func_ret_container->request;

	func_ret->q = rq;

	ret = glue_cap_insert_request_type(c_cspace, func_ret_container,
		&func_ret_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	async_msg_set_fn_type(_request, BLK_MQ_ALLOC_REQUEST_HCTX);
	fipc_set_reg0(_request, rq_container->other_ref.cptr);
	fipc_set_reg1(_request, func_ret_container->my_ref.cptr);
	fipc_set_reg2(_request, rw);
	fipc_set_reg3(_request, flags);
	fipc_set_reg4(_request, hctx_idx);

	ret = vmfunc_wrapper(_request);
	func_ret_container->other_ref.cptr = fipc_get_reg0(_request);

	return func_ret;
fail_alloc:
fail_insert:
	return NULL;
}

struct request *blk_mq_alloc_request(struct request_queue *q,
		int rw,
		unsigned int flags)
{
	int ret;
	struct request *rq;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *q_container;
	struct blk_mq_ops_container *ops_container;
	int tag;
	int qnum;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_MQ_ALLOC_REQUEST);
	fipc_set_reg0(_request, q_container->other_ref.cptr);
	fipc_set_reg2(_request, rw);
	fipc_set_reg3(_request, flags);

	ret = vmfunc_wrapper(_request);

	ret = glue_cap_lookup_blk_mq_ops_type(c_cspace,
			__cptr(fipc_get_reg4(_request)), &ops_container);
	if(ret) {
		LIBLCD_ERR("lookup");
		goto fail_lookup;
	}

	qnum = fipc_get_reg0(_request);
	tag = fipc_get_reg1(_request);
	q->limits.max_hw_sectors = fipc_get_reg2(_request);
	q->limits.max_segments = fipc_get_reg3(_request);

	rq = get_rq_from_tagset(ops_container, tag, qnum);

	printk("%s, rq: %p tag %d", __func__, rq, tag);

	rq->q = q;
	return rq;
fail_lookup:
	return NULL;
}

void blk_mq_abort_requeue_list(struct request_queue *q)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);

	async_msg_set_fn_type(_request, BLK_MQ_ABORT_REQUEUE_LIST);
	fipc_set_reg0(_request, q_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	return;
}

int blk_execute_rq(struct request_queue *q,
		struct gendisk *bd_disk,
		struct request *req,
		int at_head)
{
	int ret;
	int func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct request_queue_container *rq_container;
	struct request_container *req_container = NULL;
	struct gendisk_container *gdisk_container;

	rq_container = container_of(q, struct request_queue_container,
			request_queue);
	req_container = container_of(req, struct request_container,
				request);

	if (bd_disk) {
		gdisk_container = container_of(bd_disk, struct
				gendisk_container, gendisk);
	}

	async_msg_set_fn_type(_request, BLK_EXECUTE_RQ);
	fipc_set_reg0(_request, rq_container->other_ref.cptr);
	fipc_set_reg1(_request, req_container->other_ref.cptr);
	fipc_set_reg2(_request, req->tag);

	if (bd_disk)
		fipc_set_reg2(_request, gdisk_container->other_ref.cptr);

	fipc_set_reg3(_request, at_head);
	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);
	return func_ret;
}

void bdput(struct block_device *bdev)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct block_device_container *bdev_container;

	bdev_container = container_of(bdev, struct block_device_container,
				block_device);
	async_msg_set_fn_type(_request, BDPUT);
	fipc_set_reg0(_request, bdev_container->other_ref.cptr);
	ret = vmfunc_wrapper(_request);
	return;
}

struct block_device *bdget_disk(struct gendisk *disk, int partno)
{
	int ret;
	struct block_device_container *func_ret_container = NULL;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct gendisk_container *gdisk_container;
	struct block_device *func_ret;

	gdisk_container = container_of(disk, struct gendisk_container, gendisk);

	func_ret_container = kzalloc(sizeof(struct block_device_container),
			GFP_KERNEL);

	if (!func_ret_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_insert_block_device_type(c_cspace, func_ret_container,
			&func_ret_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, BDGET_DISK);
	fipc_set_reg0(_request, gdisk_container->other_ref.cptr);
	fipc_set_reg1(_request, partno);
	fipc_set_reg2(_request, func_ret_container->my_ref.cptr);

	ret = vmfunc_wrapper(_request);
	func_ret_container->other_ref.cptr = fipc_get_reg0(_request);

	func_ret = &func_ret_container->block_device;

	return func_ret;
fail_insert:
fail_alloc:
	return NULL;
}

struct class *__class_create(struct module *owner, const char *name,
		struct lock_class_key *key)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct class_container *func_ret_container;
	struct class *func_ret;

	func_ret_container = kzalloc(sizeof(struct class_container),
			GFP_KERNEL);

	if (!func_ret_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ret = glue_cap_insert_class_type(c_cspace, func_ret_container,
			&func_ret_container->my_ref);

	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	async_msg_set_fn_type(_request, __CLASS_CREATE);
	fipc_set_reg0(_request, func_ret_container->my_ref.cptr);

	ret = vmfunc_wrapper(_request);

	func_ret_container->other_ref.cptr = fipc_get_reg0(_request);
	func_ret = &func_ret_container->class;

	return func_ret;
fail_alloc:
fail_insert:
	return NULL;
}

void class_destroy(struct class *cls)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct class_container *class_container;

	class_container = container_of(cls, struct class_container, class);

	async_msg_set_fn_type(_request, CLASS_DESTROY);
	fipc_set_reg0(_request, class_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	return;
}

struct device *device_create(struct class *class,
		struct device *parent,
		dev_t devt,
		void *drvdata,
		const char *fmt,
		...)
{
	int ret;
	struct device_container *func_ret_container = NULL;
	struct device *func_ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct class_container *class_container;
	va_list valist;
	int instance;
	struct device_container *device_container = NULL;

	class_container = container_of(class, struct class_container, class);

	func_ret_container = kzalloc(sizeof( struct device_container   ),
		GFP_KERNEL);
	if (!func_ret_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	glue_lookup_device(__cptr((unsigned long) parent), &device_container);

	func_ret = &func_ret_container->device;
	func_ret_container->dev = &func_ret_container->device;

	glue_insert_device(func_ret_container);

	va_start(valist, fmt);
	instance = va_arg(valist, int);
	va_end(valist);

	async_msg_set_fn_type(_request, DEVICE_CREATE);
	fipc_set_reg0(_request, class_container->other_ref.cptr);
	fipc_set_reg1(_request, device_container->other_ref.cptr);
	fipc_set_reg2(_request, devt);
	fipc_set_reg3(_request, func_ret_container->my_ref.cptr);
	fipc_set_reg4(_request, instance);

	ret = vmfunc_wrapper(_request);

	func_ret_container->other_ref.cptr = fipc_get_reg0(_request);

	return func_ret;
fail_alloc:
	return NULL;
}

void device_destroy(struct class *class,
		dev_t devt)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	struct class_container *class_container;

	class_container = container_of(class, struct class_container, class);
	async_msg_set_fn_type(_request, DEVICE_DESTROY);
	fipc_set_reg0(_request, class_container->other_ref.cptr);
	fipc_set_reg1(_request, devt);
	ret = vmfunc_wrapper(_request);
	return;
}

int fops_open_callee(struct fipc_message *_request)
{
        struct inode *inode = NULL;
        struct file_container *file_container = NULL;
	struct file_operations_container *fops_container;
        struct file *file = NULL;
        int ret = 0;
        int func_ret = 0;

	inode = kzalloc(sizeof( *inode ), GFP_KERNEL);

	if (!inode) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	inode->i_rdev = fipc_get_reg2(_request);

        file_container = kzalloc(sizeof( struct file_container   ),
                GFP_KERNEL);
        if (!file_container) {
                LIBLCD_ERR("kzalloc");
                goto fail_alloc;
        }
        file = &file_container->file;
        ret = glue_cap_insert_file_type(c_cspace,
                file_container,
                &file_container->my_ref);
        if (ret) {
                LIBLCD_ERR("lcd insert");
                goto fail_insert;
        }

        file_container->other_ref.cptr = fipc_get_reg1(_request);

	ret = glue_cap_lookup_file_operations_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&fops_container);

	if (ret) {
		LIBLCD_ERR("fail lookup");
		goto fail_lookup;
	}

	func_ret = fops_container->fops.open(inode, file);

	fipc_set_reg0(_request, func_ret);
	fipc_set_reg1(_request, file_container->my_ref.cptr);

fail_alloc:
fail_lookup:
fail_insert:
	return ret;

}

int fops_release_callee(struct fipc_message *_request)
{
	struct inode *inode = NULL;
	struct file *file = NULL;
	int ret = 0;
	int func_ret = 0;
        struct file_container *file_container = NULL;
	struct file_operations_container *fops_container;

	ret = glue_cap_lookup_file_type(c_cspace,
			__cptr(fipc_get_reg1(_request)), &file_container);

        if (ret) {
                LIBLCD_ERR("lcd lookup");
                goto fail_lookup;
        }

        file = &file_container->file;

	ret = glue_cap_lookup_file_operations_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&fops_container);

	if (ret) {
		LIBLCD_ERR("fail lookup");
		goto fail_lookup;
	}

	func_ret = fops_container->fops.release(inode, file);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int fops_unlocked_ioctl_callee(struct fipc_message *_request)
{
	struct file *file = NULL;
	int ret = 0;
	int func_ret = 0;
        struct file_container *file_container = NULL;
	struct file_operations_container *fops_container;
	unsigned 	int cmd = 0;
	unsigned 	long arg = 0;

	cmd = fipc_get_reg2(_request);
	arg = fipc_get_reg3(_request);

	ret = glue_cap_lookup_file_type(c_cspace,
			__cptr(fipc_get_reg1(_request)), &file_container);

        if (ret) {
                LIBLCD_ERR("lcd lookup");
                goto fail_lookup;
        }

        file = &file_container->file;

	ret = glue_cap_lookup_file_operations_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&fops_container);

	if (ret) {
		LIBLCD_ERR("fail lookup");
		goto fail_lookup;
	}

	func_ret = fops_container->fops.unlocked_ioctl(file, cmd, arg);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int bd_open_callee(struct fipc_message *_request)
{
        int ret = 0;
        int func_ret = 0;
	fmode_t mode;
	struct block_device *bdev = NULL;
	struct block_device_operations_container *bdops_container;

	ret = glue_cap_lookup_blk_dev_ops_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&bdops_container);

	if (ret) {
		LIBLCD_ERR("fail lookup");
		goto fail_lookup;
	}

	mode = fipc_get_reg1(_request);

	func_ret = bdops_container->block_device_operations.open(bdev, mode);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

int bd_release_callee(struct fipc_message *_request)
{
	int ret = 0;
	struct block_device_operations_container *bdops_container;
	struct gendisk_container *disk_container;
	struct gendisk *disk;
	fmode_t mode;

	ret = glue_cap_lookup_blk_dev_ops_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&bdops_container);

	if (ret) {
		LIBLCD_ERR("fail lookup");
		goto fail_lookup;
	}

	ret = glue_cap_lookup_gendisk_type(c_cspace,
			__cptr(fipc_get_reg1(_request)), &disk_container);

        if (ret) {
                LIBLCD_ERR("lcd lookup");
                goto fail_lookup;
        }

	disk = &disk_container->gendisk;

	mode = fipc_get_reg1(_request);

	bdops_container->block_device_operations.release(disk, mode);


fail_lookup:
	return ret;
}

int bd_ioctl_callee(struct fipc_message *_request)
{
	int ret;
	int func_ret;
	unsigned int cmd;
	unsigned long arg;
	fmode_t mode;
	struct block_device *bdev = NULL;
	struct block_device_operations_container *bdops_container;

	ret = glue_cap_lookup_blk_dev_ops_type(c_cspace,
				__cptr(fipc_get_reg0(_request)),
				&bdops_container);

	if (ret) {
		LIBLCD_ERR("fail lookup");
		goto fail_lookup;
	}

	mode = fipc_get_reg1(_request);
	cmd = fipc_get_reg2(_request);
	arg = fipc_get_reg3(_request);

	func_ret = bdops_container->block_device_operations.ioctl(bdev, mode, cmd, arg);

	fipc_set_reg0(_request, func_ret);

fail_lookup:
	return ret;
}

bool capable(int cap)
{
	int ret;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	bool yes;

	async_msg_set_fn_type(_request, CAPABLE);
	fipc_set_reg0(_request, cap);

	ret = vmfunc_wrapper(_request);
	yes = fipc_get_reg0(_request);
	return yes;
}

int revalidate_disk(struct gendisk *disk)
{
	int ret;
	int func_ret;
	struct gendisk_container *gdisk_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;

	gdisk_container = container_of(disk, struct gendisk_container, gendisk);

	async_msg_set_fn_type(_request, REVALIDATE_DISK);
	fipc_set_reg0(_request, gdisk_container->other_ref.cptr);

	ret = vmfunc_wrapper(_request);
	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

void blk_add_request_payload(struct request *rq, struct page *page,
		int offset, unsigned int len)
{
	struct bio *bio = rq->bio;

	bio->bi_io_vec->bv_page = page;
	bio->bi_io_vec->bv_offset = offset;
	bio->bi_io_vec->bv_len = len;

	bio->bi_iter.bi_size = len;
	bio->bi_vcnt = 1;
	bio->bi_phys_segments = 1;

	rq->__data_len = rq->resid_len = len;
	rq->nr_phys_segments = 1;
}

int blk_rq_map_user(struct request_queue *q, struct request *rq,
		    struct rq_map_data *map_data, void __user *ubuf,
		    unsigned long len, gfp_t gfp_mask)
{
	LIBLCD_MSG("%s, called", __func__);
	return 0;
}

#if 0
int blk_rq_map_kern(struct request_queue *q, struct request *rq, void *kbuf,
		    unsigned int len, gfp_t gfp_mask)
{
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int func_ret;
	int ret;
	unsigned 	long p_mem_sz;
	unsigned 	long p_offset;
	cptr_t p_cptr;
	struct request_container *rq_container = NULL;
	struct request_queue_container *q_container;

	q_container = container_of(q, struct request_queue_container,
			request_queue);
	rq_container = container_of(rq, struct request_container, request);

	ret = lcd_virt_to_cptr(__gva(( unsigned  long )kbuf), &p_cptr,
			&p_mem_sz, &p_offset);

	if (ret) {
		LIBLCD_ERR("virt to cptr failed");
		goto fail_virt;
	}

	LIBLCD_MSG("%s, called", __func__);

	printk("%s, passing q->other_ref.cptr %lx", __func__,
				q_container->other_ref.cptr);
	async_msg_set_fn_type(_request, BLK_RQ_MAP_KERN);
	fipc_set_reg0(_request, q_container->other_ref.cptr);
	fipc_set_reg1(_request, rq_container->other_ref.cptr);
	fipc_set_reg2(_request, ilog2((p_mem_sz) >> (PAGE_SHIFT)));
	fipc_set_reg3(_request, p_offset);
	fipc_set_reg4(_request, cptr_val(p_cptr));
	fipc_set_reg5(_request, len);
	fipc_set_reg6(_request, gfp_mask);

	ret = vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
fail_virt:
	return ret;
}
#endif

unsigned long _lcd_copy_from_user(void *to, const void *from, unsigned long n)
{
	LIBLCD_MSG("%s, called", __func__);
	return 0;
}

unsigned long _lcd_copy_to_user(void  *to, const void *from, unsigned long n)
{
	LIBLCD_MSG("%s, called", __func__);
	return 0;
}
