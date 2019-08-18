#include "nvme_glue_helper.h"

int glue_cap_insert_bio_type(struct glue_cspace *cspace,
		struct bio_container *bio_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		bio_container,
		glue_libcap_type_ops[ GLUE_TYPE_BIO_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_bio_type(struct glue_cspace *cspace,
		struct cptr c,
		struct bio_container **bio_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_BIO_CONTAINER ].libcap_type,
		( void  ** )bio_container);

}

int glue_cap_insert_block_device_type(struct glue_cspace *cspace,
		struct block_device_container *block_device_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		block_device_container,
		glue_libcap_type_ops[ GLUE_TYPE_BLOCK_DEVICE_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_block_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct block_device_container **block_device_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_BLOCK_DEVICE_CONTAINER ].libcap_type,
		( void  ** )block_device_container);

}

int glue_cap_insert_class_type(struct glue_cspace *cspace,
		struct class_container *class_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		class_container,
		glue_libcap_type_ops[ GLUE_TYPE_CLASS_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_class_type(struct glue_cspace *cspace,
		struct cptr c,
		struct class_container **class_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_CLASS_CONTAINER ].libcap_type,
		( void  ** )class_container);

}

int glue_cap_insert_device_type(struct glue_cspace *cspace,
		struct device_container *device_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		device_container,
		glue_libcap_type_ops[ GLUE_TYPE_DEVICE_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct device_container **device_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_DEVICE_CONTAINER ].libcap_type,
		( void  ** )device_container);

}

int glue_cap_insert_pci_bus_type(struct glue_cspace *cspace,
		struct pci_bus_container *pci_bus_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		pci_bus_container,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_BUS_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_pci_bus_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_bus_container **pci_bus_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_BUS_CONTAINER ].libcap_type,
		( void  ** )pci_bus_container);

}

int glue_cap_insert_pci_dev_type(struct glue_cspace *cspace,
		struct pci_dev_container *pci_dev_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		pci_dev_container,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_DEV_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_pci_dev_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_dev_container **pci_dev_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_DEV_CONTAINER ].libcap_type,
		( void  ** )pci_dev_container);

}

int glue_cap_insert_pci_device_id_type(struct glue_cspace *cspace,
		struct pci_device_id_container *pci_device_id_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		pci_device_id_container,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_DEVICE_ID_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_pci_device_id_type(struct glue_cspace *cspace,
		struct cptr c,
		struct pci_device_id_container **pci_device_id_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_DEVICE_ID_CONTAINER ].libcap_type,
		( void  ** )pci_device_id_container);

}

int glue_cap_insert_pci_driver_type(struct glue_cspace *cspace,
		struct pci_driver_container *pci_driver_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		pci_driver_container,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_DRIVER_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_pci_driver_type(struct glue_cspace *cspace, 
		struct cptr c,
		struct pci_driver_container **pci_driver_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_PCI_DRIVER_CONTAINER ].libcap_type,
		( void  ** )pci_driver_container);

}


int glue_cap_lookup_irqhandler_type(struct glue_cspace *cspace,
		struct cptr c,
		struct irqhandler_t_container **irqhandler_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_IRQ_HANDLER_CONTAINER ].libcap_type,
		( void  ** )irqhandler_container);

}

int glue_cap_insert_blk_mq_ops_type(struct glue_cspace *cspace, 
			struct blk_mq_ops_container *blk_mq_ops_container,
		        cptr_t *c_out) {

        return glue_cspace_insert(cspace, blk_mq_ops_container,  
                                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_OPS_CONTAINER].libcap_type,
                                c_out);
}


int glue_cap_insert_blk_dev_ops_type(struct glue_cspace *cspace,
			struct block_device_operations_container *blo_container,
			cptr_t *c_out) { 

	 return glue_cspace_insert(cspace, blo_container,
			 	glue_libcap_type_ops[GLUE_TYPE_BLK_DEV_OPS_CONTAINER].libcap_type,
				c_out);

}

int glue_cap_insert_blk_mq_tag_set_type(struct glue_cspace *cspace,
			struct blk_mq_tag_set_container *set_container,
			 cptr_t *c_out) {
	
       return glue_cspace_insert(cspace, set_container,  
                                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_TAG_SET_CONTAINER].libcap_type,
                                c_out);
}

int glue_cap_insert_gendisk_type(struct glue_cspace *cspace,
			struct gendisk_container *disk_container,
			cptr_t *c_out) {

        return glue_cspace_insert(cspace, disk_container,  
                                glue_libcap_type_ops[GLUE_TYPE_GENDISK_CONTAINER].libcap_type,
                                c_out);

}

int glue_cap_insert_blk_mq_hw_ctx_type(struct glue_cspace *cspace,
			struct blk_mq_hw_ctx_container *ctx_container,
			cptr_t *c_out) {

        return glue_cspace_insert(cspace, ctx_container,  
                                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_HW_CTX_CONTAINER].libcap_type,
                                c_out);

}

int glue_cap_insert_blk_mq_queue_data_type(struct glue_cspace *cspace,
			struct blk_mq_queue_data_container *bd_container,
			cptr_t *c_out) {

        return glue_cspace_insert(cspace, bd_container,  
                                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_QUEUE_DATA_CONTAINER].libcap_type,
                                c_out);

}

int glue_cap_insert_request_type(struct glue_cspace *cspace,
		struct request_container *request_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		request_container,
		glue_libcap_type_ops[ GLUE_TYPE_REQUEST_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_request_type(struct glue_cspace *cspace,
		struct cptr c,
		struct request_container **request_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_REQUEST_CONTAINER ].libcap_type,
		( void  ** )request_container);

}

int glue_cap_insert_request_queue_type(struct glue_cspace *cspace,
			struct request_queue_container *req_queue_container,
			cptr_t *c_out) {

        return glue_cspace_insert(cspace, req_queue_container,  
                                glue_libcap_type_ops[GLUE_TYPE_REQUEST_QUEUE_CONTAINER].libcap_type,
                                c_out);

}

int glue_cap_lookup_blk_mq_ops_type(struct glue_cspace *cspace,
			cptr_t c,
			 struct blk_mq_ops_container **blk_mq_ops_container) {

        return glue_cspace_lookup(
                cspace, c,
                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_OPS_CONTAINER].libcap_type,
                (void **)blk_mq_ops_container);

}

int glue_cap_lookup_blk_mq_tag_set_type(struct glue_cspace *cspace,
			cptr_t c,
			struct blk_mq_tag_set_container **set_container) {

        return glue_cspace_lookup(
                cspace, c,
                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_TAG_SET_CONTAINER].libcap_type,
                (void **)set_container);

}

int glue_cap_lookup_blk_dev_ops_type(struct glue_cspace *cspace,
			cptr_t c,
			struct block_device_operations_container **blo_container) {

        return glue_cspace_lookup(
                cspace, c,
                glue_libcap_type_ops[GLUE_TYPE_BLK_DEV_OPS_CONTAINER].libcap_type,
                (void **)blo_container);

}

int glue_cap_lookup_gendisk_type(struct glue_cspace *cspace,
			cptr_t c,
			struct gendisk_container **disk_container) {

        return glue_cspace_lookup(
                cspace, c,
                glue_libcap_type_ops[GLUE_TYPE_GENDISK_CONTAINER].libcap_type,
                (void **)disk_container);

}

int glue_cap_lookup_blk_mq_hw_ctx_type(struct glue_cspace *cspace,
			cptr_t c,
			 struct blk_mq_hw_ctx_container **ctx_container) {

        return glue_cspace_lookup(
                cspace, c,
                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_HW_CTX_CONTAINER].libcap_type,
                (void **)ctx_container);

}

int glue_cap_lookup_blk_mq_queue_data_type(struct glue_cspace *cspace,
			cptr_t c,
			struct blk_mq_queue_data_container **bd_container) {

        return glue_cspace_lookup(
                cspace, c,
                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_QUEUE_DATA_CONTAINER].libcap_type,
                (void **)bd_container);

}


int glue_cap_lookup_request_queue_type(struct glue_cspace *cspace,
			cptr_t c,
			struct request_queue_container **req_queue_container) {

        return glue_cspace_lookup(
                cspace, c,
                glue_libcap_type_ops[GLUE_TYPE_REQUEST_QUEUE_CONTAINER].libcap_type,
                (void **)req_queue_container);

}

int glue_cap_insert_ida_type(struct glue_cspace *cspace,
		struct ida_container *ida_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		ida_container,
		glue_libcap_type_ops[ GLUE_TYPE_IDA_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_ida_type(struct glue_cspace *cspace,
		struct cptr c,
		struct ida_container **ida_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_IDA_CONTAINER ].libcap_type,
		( void  ** )ida_container);

}

int glue_cap_insert_file_type(struct glue_cspace *cspace,
		struct file_container *file_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		file_container,
		glue_libcap_type_ops[ GLUE_TYPE_FILE_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_file_type(struct glue_cspace *cspace,
		struct cptr c,
		struct file_container **file_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_FILE_CONTAINER ].libcap_type,
		( void  ** )file_container);

}

int glue_cap_insert_file_operations_type(struct glue_cspace *cspace,
		struct file_operations_container *file_operations_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		file_operations_container,
		glue_libcap_type_ops[ GLUE_TYPE_FILE_OPERATIONS_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_file_operations_type(struct glue_cspace *cspace,
		struct cptr c,
		struct file_operations_container **file_operations_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_FILE_OPERATIONS_CONTAINER ].libcap_type,
		( void  ** )file_operations_container);

}
