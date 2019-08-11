#ifndef __NVME_GLUE_HELPER_H__
#define __NVME_GLUE_HELPER_H__


#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/rtnetlink.h>
#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include "nvme.h"
#include "iod.h"

struct device_container {
	struct device* device;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct module_container {
	struct module module;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct pci_bus_container {
	struct pci_bus pci_bus;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct pci_dev_container {
	struct pci_dev pci_dev;
	struct pci_dev *pdev;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct pci_device_id_container {
	struct pci_device_id pci_device_id;
	struct cptr other_ref;
	struct cptr my_ref;
};
struct pci_driver_container {
	struct pci_driver pci_driver;
	struct cptr other_ref;
	struct cptr my_ref;
};

struct nvme_ctrl_ops_container {
	struct nvme_ctrl_ops nvme_ctrl_ops;
	struct cptr other_ref;
	struct cptr my_ref;
};

struct nvme_ctrl_container{
	struct nvme_ctrl nvme_ctrl;
	struct cptr other_ref;
	struct cptr my_ref;
};

struct trampoline_hidden_args {
	void *struct_container;
	struct glue_cspace *cspace;
	struct lcd_trampoline_handle *t_handle;
	struct thc_channel *async_chnl;
	struct cptr sync_ep;
};

struct irqhandler_t_container {
	irqreturn_t (*irqhandler)(int, void *);
	void *data;
	cptr_t my_ref;
	cptr_t other_ref;
};

struct gendisk_container {
	struct gendisk gendisk;
	cptr_t other_ref;
	cptr_t my_ref;
};

struct blk_mq_hw_ctx_container {
	struct blk_mq_hw_ctx blk_mq_hw_ctx;
	cptr_t other_ref;
	cptr_t my_ref;
};
struct blk_mq_ops_container {
	struct blk_mq_ops blk_mq_ops;
	cptr_t other_ref;
	cptr_t my_ref;
	void *data;
};
struct blk_mq_queue_data_container {
	struct blk_mq_queue_data blk_mq_queue_data;
	cptr_t other_ref;
	cptr_t my_ref;
};
struct blk_mq_tag_set_container {
	struct blk_mq_tag_set tag_set;
	cptr_t other_ref;
	cptr_t my_ref;
};
struct block_device_container {
	struct block_device block_device;
	cptr_t other_ref;
	cptr_t my_ref;
};
struct block_device_operations_container {
	struct block_device_operations block_device_operations;
	cptr_t other_ref;
	cptr_t my_ref;
};

struct request_container {
	struct request request;
	cptr_t other_ref;
	cptr_t my_ref;
};
struct request_queue_container {
	struct request_queue request_queue;
	cptr_t other_ref;
	cptr_t my_ref;
};

struct nvme_ns_container {
	struct nvme_ns nvme_ns;
	cptr_t other_ref;
	cptr_t my_ref;
};

struct nvme_command_container {
	struct nvme_command nvme_cmd;
	cptr_t other_ref;
	cptr_t my_ref;
};

struct nvme_iod_container {
	struct nvme_iod nvme_iod;
	cptr_t other_ref;
	cptr_t my_ref;
};


int glue_cap_insert_device_type(struct glue_cspace *cspace,
		struct device_container *device_container,
		struct cptr *c_out);
int glue_cap_insert_module_type(struct glue_cspace *cspace,
		struct module_container *module_container,
		struct cptr *c_out);

int glue_cap_insert_pci_bus_type(struct glue_cspace *cspace,
		struct pci_bus_container *pci_bus_container,
		struct cptr *c_out);
int glue_cap_insert_pci_dev_type(struct glue_cspace *cspace,
		struct pci_dev_container *pci_dev_container,
		struct cptr *c_out);
int glue_cap_insert_pci_device_id_type(struct glue_cspace *cspace,
		struct pci_device_id_container *pci_device_id_container,
		struct cptr *c_out);
int glue_cap_insert_pci_driver_type(struct glue_cspace *cspace,
		struct pci_driver_container *pci_driver_container,
		struct cptr *c_out);

int glue_cap_init(void);

int glue_cap_create(struct glue_cspace **cspace);

void glue_cap_destroy(struct glue_cspace *cspace);

void glue_cap_exit(void);

void glue_cap_remove(
	struct glue_cspace *cspace,
	cptr_t c);

int glue_cap_insert_blk_mq_ops_type(struct glue_cspace *cspace, 
			struct blk_mq_ops_container *blk_mq_ops_container,
			cptr_t *c_out);

int glue_cap_insert_module_type(struct glue_cspace *cspace,
			struct module_container *module_container,
			cptr_t *c_out);

int glue_cap_insert_blk_mq_tag_set_type(struct glue_cspace *cspace,
			struct blk_mq_tag_set_container *set_container,
			 cptr_t *c_out);

int glue_cap_insert_gendisk_type(struct glue_cspace *cspace,
			struct gendisk_container *disk_container,
			cptr_t *c_out);

int glue_cap_insert_blk_mq_hw_ctx_type(struct glue_cspace *cspace,
			struct blk_mq_hw_ctx_container *ctx_container,
			cptr_t *c_out);

int glue_cap_insert_blk_dev_ops_type(struct glue_cspace *cspace,
			struct block_device_operations_container *blo_container,
			cptr_t *c_out);

int glue_cap_insert_blk_mq_queue_data_type(struct glue_cspace *cspace,
			struct blk_mq_queue_data_container *bd_container,
			cptr_t *c_out);

int glue_cap_insert_request_queue_type(struct glue_cspace *cspace,
			struct request_queue_container *req_queue_container,
			cptr_t *c_out);

int glue_cap_insert_nvme_ctrl_ops_type(struct glue_cspace *cspace,
			struct nvme_ctrl_ops_container *nvme_ctrl_ops_container,
			cptr_t *c_out);

int glue_cap_insert_nvme_ctrl_type(struct glue_cspace *cspace,
			struct nvme_ctrl_container *nvme_ctrl_container,
			cptr_t *c_out);

int glue_cap_lookup_blk_mq_ops_type(struct glue_cspace *cspace,
			cptr_t c,
			 struct blk_mq_ops_container **blk_mq_ops_container);

int glue_cap_lookup_blk_mq_tag_set_type(struct glue_cspace *cspace,
			cptr_t c,
			struct blk_mq_tag_set_container **set_container);

int glue_cap_lookup_gendisk_type(struct glue_cspace *cspace,
			cptr_t c,
			struct gendisk_container **disk_container);

int glue_cap_lookup_blk_mq_hw_ctx_type(struct glue_cspace *cspace,
			cptr_t c,
			 struct blk_mq_hw_ctx_container **ctx_container);

int glue_cap_lookup_blk_mq_queue_data_type(struct glue_cspace *cspace,
			cptr_t c,
			struct blk_mq_queue_data_container **bd_container);

int glue_cap_lookup_module_type(struct glue_cspace *cspace,
				cptr_t c,
				struct module_container **module_container);

int glue_cap_lookup_request_queue_type(struct glue_cspace *cspace,
			cptr_t c,
			struct request_queue_container **req_queue_container);

int glue_cap_lookup_blk_dev_ops_type(struct glue_cspace *cspace,
			cptr_t c,
			struct block_device_operations_container **ops_container);

int glue_cap_lookup_nvme_ctrl_ops_type(struct glue_cspace *cspace,
			cptr_t c,
			struct nvme_ctrl_ops_container **nvme_ctrl_ops_ctr);

int glue_cap_lookup_nvme_ctrl_type(struct glue_cspace *cspace,
			cptr_t c,
			struct nvme_ctrl_container **nvme_ctrl_ctr);

void glue_cap_remove(struct glue_cspace *cspace, cptr_t c);   

int glue_cap_lookup_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct device_container **device_container);
int glue_cap_lookup_module_type(struct glue_cspace *cspace,
		struct cptr c,
		struct module_container **module_container);
int glue_cap_lookup_pci_bus_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_bus_container **pci_bus_container);
int glue_cap_lookup_pci_dev_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_dev_container **pci_dev_container);
int glue_cap_lookup_pci_device_id_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_device_id_container **pci_device_id_container);
int glue_cap_lookup_pci_driver_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_driver_container **pci_driver_container);
int glue_cap_insert_irqhandler_type(struct glue_cspace *cspace,
		struct irqhandler_t_container *irqhandler_container,
		struct cptr *c_out);
int glue_cap_lookup_irqhandler_type(struct glue_cspace *cspace,
		struct cptr c,
		struct irqhandler_t_container **irqhandler_container);

#endif /* __NVME_GLUE_HELPER_H__ */
