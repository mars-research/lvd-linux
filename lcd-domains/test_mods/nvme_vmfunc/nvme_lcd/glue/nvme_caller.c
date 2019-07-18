#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <liblcd/glue_cspace.h>
#include "../../nvme_common.h"
#include "../nvme_caller.h"
#include <asm/lcd_domains/liblcd.h>
#include <lcd_config/post_hook.h>

//#include "../../benchmark.h"
static struct glue_cspace *c_cspace;

void *iocb_data_pool;
struct blk_mq_hw_ctx_container *ctx_container_g; 
struct blk_mq_ops_container *ops_container_g;

extern struct pci_driver_container nvme_driver_container;

struct lcd_request_container {
	struct request rq;
};

#ifdef IOMMU_ASSIGN
/* device for IOMMU assignment */
// TODO: replace with real values
struct pcidev_info dev_assign = { 0x0000, 0x00, 0x00, 0x0 };
#endif

void null_softirq_done_fn(struct request *rq);

//int null_open(struct block_device *bdev, fmode_t mode);
//void null_release(struct gendisk *disk, fmode_t mode);

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



struct request * TODO_nvme_alloc_request(struct request_queue *q,
		struct nvme_command *cmd, unsigned int flags)
{
    struct request_queue_container *rq_container;
    struct nvme_command_container *cmd_container;
    struct request *ret = NULL;
    struct fipc_message r;
	struct fipc_message *request = &r;
    
    //glue_cap_insert_blk_dev_ops_type
    
    rq_container = container_of(q, struct request_queue_container,
						request_queue);
    cmd_container = container_of(cmd, struct nvme_command_container,
						cmd);
                        
    async_msg_set_fn_type(request, NVME_ALLOC_REQUEST);
    
    return ret;
}

int TODO_nvme_setup_cmd(struct nvme_ns *ns, struct request *req,
		struct nvme_command *cmd)
{
    struct nvme_ns_container *ns_container;
    struct lcd_request_container *req_container;
    struct nvme_command_container *cmd_container;
	struct fipc_message r;
	struct fipc_message *request = &r;
    
    ns_container = container_of(ns, struct nvme_ns_container, nvme_ns);
    req_container = container_of(req, struct lcd_request_container, rq);
    cmd_container = container_of(cmd, struct nvme_command_container, cmd);
    
    fipc_set_reg0(request, req->cmd_type);
    fipc_set_reg1(request, req->cmd_flags);
    
    async_msg_set_fn_type(request, NVME_SETUP_CMD);
    
    vmfunc_wrapper(request);
    
    return 0;
    
}

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
	ops_container_g = ops_container;

	ret = glue_cap_insert_blk_mq_ops_type(c_cspace, ops_container, &ops_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert1;
	}
	
	ret = glue_cap_insert_blk_mq_tag_set_type(c_cspace, set_container, &set_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert2;
	}
	
	async_msg_set_fn_type(request, BLK_MQ_ALLOC_TAG_SET);

	fipc_set_reg0(request, set_container->my_ref.cptr);
	fipc_set_reg1(request, set->nr_hw_queues);
	fipc_set_reg2(request, set->queue_depth);
	fipc_set_reg3(request, set->reserved_tags);
	fipc_set_reg4(request, set->cmd_size);
	fipc_set_reg5(request, set->flags);
	fipc_set_reg6(request, ops_container->my_ref.cptr);
	
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
    struct lcd_request_container *rq_container;
    struct fipc_message r;
	struct fipc_message *request = &r;
    
    rq_container = container_of(rq, struct lcd_request_container, rq);
    
    async_msg_set_fn_type(request, BLK_MQ_COMPLETE_REQUEST);
    fipc_set_reg0(request, rq->tag);
    fipc_set_reg1(request, error);
    
    vmfunc_wrapper(request);
    
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
    fipc_set_reg1(request, (int)async);
    
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

int pci_enable_msix_range(struct pci_dev *dev, struct msix_entry *entries, int minvec, int maxvec)
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

	vmfunc_wrapper(_request);

	func_ret = fipc_get_reg0(_request);

	return func_ret;
}

int __must_check __pci_register_driver(struct pci_driver *drv,
		struct module *owner,
		const char* name)
{
	struct pci_driver_container *drv_container;
	struct module_container *owner_container;
	struct fipc_message r;
	struct fipc_message *_request = &r;
	int ret;
	int func_ret;
	INIT_IPC_MSG(&r);

	drv_container = container_of(drv,
		struct pci_driver_container,
		pci_driver);
	ret = glue_cap_insert_pci_driver_type(c_cspace,
		drv_container,
		&drv_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	owner_container = container_of(owner,
		struct module_container,
		module);
	ret = glue_cap_insert_module_type(c_cspace,
		owner_container,
		&owner_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}

	async_msg_set_fn_type(_request,
			__PCI_REGISTER_DRIVER);
	fipc_set_reg0(_request, drv_container->my_ref.cptr);
	fipc_set_reg1(_request, owner_container->my_ref.cptr);

	vmfunc_wrapper(_request);

	drv_container->other_ref.cptr = fipc_get_reg1(_request);
	owner_container->other_ref.cptr = fipc_get_reg2(_request);
	drv_container->pci_driver.driver.owner = &owner_container->module;
	func_ret = fipc_get_reg3(_request);

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

	ret = glue_cap_insert_request_queue_type(c_cspace, rq_container, &rq_container->my_ref);

    if (ret) {
    LIBLCD_ERR("lcd insert");
            goto fail_insert;
    }
	
	set_container = container_of(set, struct blk_mq_tag_set_container, tag_set);

	async_msg_set_fn_type(request, BLK_MQ_INIT_QUEUE);
	fipc_set_reg0(request, set_container->other_ref.cptr);
	fipc_set_reg1(request, rq_container->my_ref.cptr);
	
	vmfunc_wrapper(request);
	
	rq_container->other_ref.cptr = fipc_get_reg0(request);

	printk("%s returns local request queue struct!! \n", __func__);
	return &rq_container->request_queue;

fail_insert:
	kfree(rq_container);
fail_alloc:
	return NULL;
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
	struct fipc_message *request = &r;
	struct lcd_request_container *rq_c;

	rq_c = container_of(rq, struct lcd_request_container, rq);

	async_msg_set_fn_type(request, BLK_MQ_END_REQUEST);

	fipc_set_reg0(request, rq->tag);	
	fipc_set_reg1(request, error);

	vmfunc_wrapper(request);

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

void blk_mq_start_request(struct request *rq)
{
	struct fipc_message r;
	struct fipc_message *request = &r;

	struct lcd_request_container *rq_c;

	rq_c = container_of(rq, struct lcd_request_container, rq);

	async_msg_set_fn_type(request, BLK_MQ_START_REQUEST);

	fipc_set_reg0(request, rq->tag);

	vmfunc_wrapper(request);

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

void blk_queue_logical_block_size(struct request_queue *rq, unsigned short size)
{
	struct fipc_message r;
	struct fipc_message *request = &r;
	struct request_queue_container *rq_container;

	rq_container = container_of(rq, struct request_queue_container,
					request_queue);
	
	async_msg_set_fn_type(request, BLK_QUEUE_LOGICAL_BLOCK_SIZE);
	fipc_set_reg0(request, size);
	fipc_set_reg1(request, rq_container->other_ref.cptr);

	vmfunc_wrapper(request);

	return;
}

// we don't need the irq or cpu mask to be stored on this side at all
struct cpumask *blk_mq_tags_cpumask(struct blk_mq_tags *tags)
{
    struct fipc_message r;
	struct fipc_message *request = &r;
    struct blk_mq_tags_container *tags_c = container_of(tags, struct blk_mq_tags_container, blk_mq_tags);
    
    fipc_set_reg0(request, tags_c->other_ref.cptr);
    
    async_msg_set_fn_type(request, BLK_MQ_TAGS_CPUMASK);
    
    vmfunc_wrapper(request);
}

void _TODO_blk_execute_rq_nowait(struct request_queue *q, struct gendisk *bd_disk,
			   struct request *rq, int at_head,
			   rq_end_io_fn *done)
{
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

	ret = glue_cap_insert_gendisk_type(c_cspace, disk_container, &disk_container->my_ref);
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

void device_add_disk(struct device *parent, struct gendisk *disk)
{
	struct gendisk_container *disk_container;
	struct block_device_operations_container *blo_container;
	struct module_container *module_container;
	struct request_queue_container *rq_container;
	int ret;
	struct fipc_message r;
	struct fipc_message *request = &r;

	disk_container = container_of(disk, struct gendisk_container, gendisk);

	blo_container = container_of(disk->fops, 
				struct block_device_operations_container,
				block_device_operations);

	module_container = container_of(disk->fops->owner, struct module_container,
				module);

	rq_container = container_of(disk->queue, struct request_queue_container,
				request_queue);

	ret = glue_cap_insert_module_type(c_cspace, module_container, &module_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert1;
	}
	
	ret = glue_cap_insert_gendisk_type(c_cspace, disk_container, &disk_container->my_ref);
	if (ret) {
		 LIBLCD_ERR("lcd insert");
		 goto fail_insert2;
	}

	ret = glue_cap_insert_blk_dev_ops_type(c_cspace, blo_container, &blo_container->my_ref);
	if(ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert3;
	}
	
	async_msg_set_fn_type(request, DEVICE_ADD_DISK);
	fipc_set_reg0(request, disk_container->other_ref.cptr);
	fipc_set_reg1(request, blo_container->my_ref.cptr);
	fipc_set_reg2(request, module_container->my_ref.cptr);
	fipc_set_reg3(request, rq_container->other_ref.cptr);
	fipc_set_reg4(request, disk->flags);
	fipc_set_reg5(request, disk->major);
	fipc_set_reg6(request, disk->first_minor);

	/*
	 * Ran out of registers to marshall the string, so hardcoding it
	 * in the klcd
	 */
	vmfunc_wrapper(request);
	
	blo_container->other_ref.cptr = fipc_get_reg0(request);
	module_container->other_ref.cptr = fipc_get_reg1(request);

	return;

fail_insert3:
fail_insert2:
fail_insert1:
	return;
}

void put_disk(struct gendisk *disk)
{
	struct fipc_message r;
	struct fipc_message *request = &r;

	struct gendisk_container *disk_container;
	struct module_container *module_container;
	struct block_device_operations_container *blo_container;

	disk_container = container_of(disk, struct gendisk_container, gendisk);

	blo_container = container_of(disk->fops,
			struct block_device_operations_container, block_device_operations);

	module_container = container_of(disk->fops->owner, struct
			module_container, module);

	async_msg_set_fn_type(request, PUT_DISK);

	fipc_set_reg0(request, disk_container->other_ref.cptr);
	fipc_set_reg1(request, blo_container->other_ref.cptr);
	fipc_set_reg2(request, module_container->other_ref.cptr);
	
	vmfunc_wrapper(request);

	glue_cap_remove(c_cspace, disk_container->my_ref);
	glue_cap_remove(c_cspace, blo_container->my_ref);
	glue_cap_remove(c_cspace, module_container->my_ref);

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
	irqhandler_container = (struct irqhandler_t_container *) fipc_get_reg1(_request); 

	/* call real irq handler */
	irqret = irqhandler_container->irqhandler(irq, irqhandler_container->data); 

	/* printk("%s, irq:%d irqret %d\n", __func__, irq, irqret); */
	fipc_set_reg0(_request, irqret);
	return ret;
}

void put_device(struct device *dev)
{
    struct device_container* device_container;
    
    struct fipc_message r;
	struct fipc_message *request = &r;
    
    device_container = container_of(dev, struct device_container, device);
    
    fipc_set_reg0(request, device_container->other_ref.cptr);
    async_msg_set_fn_type(request, PUT_DEVICE);
	vmfunc_wrapper(request);
}

u64 dma_mask = 0;
void *data_pool;

struct pci_dev *g_pdev;
int probe_callee(struct fipc_message *_request)
{
	struct pci_dev_container *dev_container;

	int func_ret;
	int ret = 0;
	cptr_t other_ref;
	unsigned int devfn;

	cptr_t res0_cptr;
	gpa_t gpa_addr;
	unsigned int res0_len;
	volatile void *dev_resource_0;
	cptr_t pool_cptr;
	gva_t pool_addr;
	unsigned int pool_ord;

	LIBLCD_MSG("%s called", __func__);
	other_ref.cptr = fipc_get_reg1(_request);
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
	dev_container->pci_dev.dev.kobj.name = "ixgbe_lcd";
	dev_container->pci_dev.vendor = 0x8086;
	dev_container->pci_dev.device = 0x10fd;
	dev_container->other_ref = other_ref;
	dev_container->pci_dev.dev.dma_mask = &dma_mask;

	g_pdev = &dev_container->pci_dev;

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
	dev_container->pci_dev.resource[0].end = (resource_size_t)((char*)dev_resource_0 + res0_len - 1);
	LIBLCD_MSG("%s: status reg 0x%X", __func__, *(unsigned int *)((char*)dev_resource_0 + 0x8));

	ret = lcd_map_virt(pool_cptr, pool_ord, &pool_addr);
	if (ret) {
		LIBLCD_ERR("failed to map pool");
		goto fail_pool;
	}

	LIBLCD_MSG("%s, mapping private pool %p | ord %d", __func__,
			gva_val(pool_addr), pool_ord);

	data_pool = (void*)gva_val(pool_addr);

	devfn = PCI_DEVFN(dev_assign.slot, dev_assign.fn);

	/* assign the device after we map the memory so that
	 * all the rammapped pages are automatically keyed
	 * into the iommu hardware
	 */
	ret = lcd_syscall_assign_device(dev_assign.domain,
				dev_assign.bus,
				devfn);
	if (ret)
		LIBLCD_ERR("Could not assign pci device to LCD: ret %d",
				ret);

	/* XXX: Pass null for now. struct pci_dev is passed from the
	 * kernel and needs analysis for wrapping it around a container
	 */
	func_ret = nvme_driver_container.pci_driver.probe(&dev_container->pci_dev, NULL);

	fipc_set_reg0(_request, func_ret);

fail_ioremap:
fail_ioremap2:
fail_alloc:
fail_insert:
fail_cptr:
fail_pool:
	return ret;
}

extern int nvme_poll(struct blk_mq_hw_ctx *hctx, unsigned int tag);

int poll_callee(struct fipc_message *_request)
{
	int tag;
	struct blk_mq_hw_ctx_container *hctx_c = NULL;

    //not sure if this is supposed to be a pointer
	struct blk_mq_hw_ctx hctx;
	//static int count = 0;
	int ret;

	tag = fipc_get_reg0(_request);


	glue_cap_lookup_blk_mq_hw_ctx_type(c_cspace, __cptr(fipc_get_reg1(_request)), &hctx_c);
	//BUG_ON(!hctx_c->blk_mq_hw_ctx);
	hctx = hctx_c->blk_mq_hw_ctx;

/*
	if (!count) {
		//printk("%s, napi_c %p | napi_c->napi %p", __func__, napi_c,
		//		napi_c ? napi_c->napi : NULL);
		//printk("%s, napi %p", __func__, napi);
		printk("%s, napi->state 0x%lx budget:%d", __func__,
						napi->state, budget);
		count = 1;
	}
*/
	//napi->state = fipc_get_reg2(_request);

	ret = nvme_poll(&hctx, tag);

	fipc_set_reg0(_request, ret);
	//fipc_set_reg1(_request, napi->state);

	return 0;
}

int queue_rq_fn_callee(struct fipc_message *request)
{	struct blk_mq_hw_ctx_container *ctx_container = ctx_container_g;
	struct blk_mq_ops_container *ops_container = ops_container_g;
	struct blk_mq_queue_data bd;
	struct lcd_request_container rq_c;
	struct request *rq = &rq_c.rq;
	int ret = 0;
	int func_ret;

//	ret = glue_cap_lookup_blk_mq_hw_ctx_type(c_cspace, __cptr(fipc_get_reg1(request)),
//						&ctx_container);
//	if(ret) {
//		LIBLCD_ERR("lookup");
//		goto fail_lookup;
//	}
	
	ctx_container->blk_mq_hw_ctx.queue_num = fipc_get_reg0(request);

//	ret = glue_cap_lookup_blk_mq_ops_type(c_cspace, __cptr(fipc_get_reg2(request)),
//				&ops_container);
//	if(ret) {
//		LIBLCD_ERR("lookup");
//		goto fail_lookup;
//	}

	rq->tag = fipc_get_reg3(request);
	bd.rq = rq;

	func_ret = ops_container->blk_mq_ops.queue_rq(&ctx_container->blk_mq_hw_ctx,
				&bd);
	
	fipc_set_reg0(request, func_ret);

//fail_lookup:
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
	cptr_t pool_cptr;
	gva_t pool_addr;
	unsigned int pool_ord;
	int ret = 0;

	ctx_container = kzalloc(sizeof(*ctx_container), GFP_KERNEL);
	if (!ctx_container) {
		LIBLCD_ERR("kzalloc");
		goto fail_alloc;
	}

	ctx_container_g = ctx_container;	

	ret = glue_cap_insert_blk_mq_hw_ctx_type(c_cspace, ctx_container, &ctx_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd insert");
		goto fail_insert;
	}
	
	ctx_container->other_ref.cptr = fipc_get_reg0(request); 
	index = fipc_get_reg1(request);

	ret = glue_cap_lookup_blk_mq_ops_type(c_cspace,
			__cptr(fipc_get_reg2(request)), &ops_container);
        if (ret) {
                LIBLCD_ERR("lookup");
                goto fail_lookup;
        }

	/* receive shared data pool */
	ret = lcd_cptr_alloc(&pool_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_cptr;
	}

	fipc_set_reg0(request, cptr_val(pool_cptr));
	vmfunc_sync_call(request, INIT_HCTX_SYNC);
	pool_ord = fipc_get_reg0(request);

	ret = lcd_map_virt(pool_cptr, pool_ord, &pool_addr);
	if (ret) {
		LIBLCD_ERR("failed to map pool");
		goto fail_pool;
	}

	LIBLCD_MSG("%s, mapping private pool %p | ord %d", __func__,
			gva_val(pool_addr), pool_ord);

	iocb_data_pool = (void*)gva_val(pool_addr);

	/* Passing NULL to data arg, hack to get address within the driver */
    //copied from nullb, not sure if this needs to be changed
	ret = ops_container->blk_mq_ops.init_hctx(&ctx_container->blk_mq_hw_ctx, NULL, index);
	if(ret) {
	        LIBLCD_ERR("call to init_hctx failed");
                goto fail_hctx;
	}

	fipc_set_reg0(request, ctx_container->my_ref.cptr);
	fipc_set_reg1(request, ret);
	return ret;

fail_pool:
fail_cptr:
fail_hctx:
fail_lookup:
	glue_cap_remove(c_cspace, ctx_container->my_ref);
fail_insert:
	kfree(ctx_container);
fail_alloc:
	return ret;
}

int softirq_done_fn_callee(struct fipc_message *_request)
{
    /*
	struct request *rq;
	struct request_container *rq_c;

	null_softirq_done_fn(rq);
    */

	printk("%s, called", __func__);
	return 0;
}

/*
int open_callee(struct fipc_message *_request)
{
	int ret;

	fmode_t mode = fipc_get_reg0(_request);

	ret = null_open(NULL, mode);

	fipc_set_reg0(_request, ret);

	return ret;
}
*/

/*
int release_callee(struct fipc_message *_request)
{
	struct gendisk_container *disk_container;
	int ret;
	fmode_t mode;

	ret = glue_cap_lookup_gendisk_type(c_cspace, __cptr(fipc_get_reg0(_request)),
					&disk_container);
	mode = fipc_get_reg1(_request);

	null_release(&disk_container->gendisk, mode);

	return 0;
}
*/