/*
 * cap.c
 *
 * cspace code for pmfs/vfs
 */

#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <linux/slab.h>
#include "glue_helper.h"

#include <lcd_config/post_hook.h>

/* ------------------------------------------------------------ */

static struct cap_type_system *glue_libcap_type_system;

struct type_ops_id {
        struct cap_type_ops ops;
        cap_type_t libcap_type;
};

enum glue_type {
        GLUE_TYPE_BLK_MQ_OPS,
	GLUE_TYPE_MODULE,
        GLUE_TYPE_BLK_MQ_TAG_SET,
	GLUE_TYPE_BLK_DEV_OPS,
	GLUE_TYPE_GENDISK,
        GLUE_TYPE_BLK_MQ_HW_CTX,
        GLUE_TYPE_BLK_MQ_QUEUE_DATA,
        GLUE_TYPE_REQUEST_QUEUE,
	GLUE_NR_TYPES,
};

static int dummy_func(struct cspace *cspace, struct cnode *cnode,
                void *object)
{
        return 0;
}

static struct type_ops_id glue_libcap_type_ops[GLUE_NR_TYPES] = { 
        {
                {
                        .name = "struct blk_mq_ops",
                        .delete = dummy_func,
                        .revoke = dummy_func,
                }
        },
        {
                {
                        .name = "struct module",
                        .delete = dummy_func,
                        .revoke = dummy_func,
                }
        },

	{
                {
                        .name = "struct blk_mq_tag_set",
                        .delete = dummy_func,
                        .revoke = dummy_func,
                }
        },
	{
                {
                        .name = "struct gendisk",
                        .delete = dummy_func,
                        .revoke = dummy_func,
                }
        },
	{
                {
                        .name = "struct blk_mq_hw_ctx",
                        .delete = dummy_func,
                        .revoke = dummy_func,
                }
        },
	{
                {
                        .name = "struct blk_mq_queue_data",
                        .delete = dummy_func,
                        .revoke = dummy_func,
                }
        },
	{
                {
                        .name = "struct request_queue",
                        .delete = dummy_func,
                        .revoke = dummy_func,
                }
        },


};

int glue_cap_init(void)
{
        int ret;
        int i;
        cap_type_t libcap_type;
        /*
         * Alloc and init microkernel type system
         */
        ret = cap_type_system_alloc(&glue_libcap_type_system);
        if (ret) {
                LIBLCD_ERR("alloc glue type system failed");
                goto fail1;
        }
        ret = cap_type_system_init(glue_libcap_type_system);
        if (ret) {
                LIBLCD_ERR("init glue type system failed");
                goto fail2;
        }
        /*
         * Add types
         */
        for (i = 0; i < GLUE_NR_TYPES; i++) {

                libcap_type = cap_register_private_type(
                        glue_libcap_type_system,
                        0,
                        &glue_libcap_type_ops[i].ops);
                if (libcap_type == CAP_TYPE_ERR) {
                        LIBLCD_ERR("failed to register glue cap type %s",
                                glue_libcap_type_ops[i].ops.name);
                        ret = -EIO;
                        goto fail3;
                }
                glue_libcap_type_ops[i].libcap_type = libcap_type;
        }

        return 0;

fail3:
        cap_type_system_destroy(glue_libcap_type_system);
fail2:
        cap_type_system_free(glue_libcap_type_system);
        glue_libcap_type_system = NULL;
fail1:
        return ret;
}

int glue_cap_create(struct glue_cspace **cspace_out)
{
        return glue_cspace_alloc_init(glue_libcap_type_system, cspace_out);
}

void glue_cap_destroy(struct glue_cspace *cspace)
{
        glue_cspace_destroy_free(cspace);
}

void glue_cap_exit(void)
{
        /*
         * Destroy and free type system if necessary
         */
        if (glue_libcap_type_system) {
                cap_type_system_destroy(glue_libcap_type_system);
                cap_type_system_free(glue_libcap_type_system);
                glue_libcap_type_system = NULL;
        }
}


int glue_cap_insert_blk_mq_ops_type(struct glue_cspace *cspace, 
			struct blk_mq_ops_container *blk_mq_ops_container,
		        cptr_t *c_out) {

        return glue_cspace_insert(cspace, blk_mq_ops_container,  
                                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_OPS].libcap_type,
                                c_out);
}

int glue_cap_insert_module_type(struct glue_cspace *cspace,
        		struct module_container *module_container,
        		cptr_t *c_out) {


        return glue_cspace_insert(cspace, module_container,
                                glue_libcap_type_ops[GLUE_TYPE_MODULE].libcap_type,
                                c_out);
}

int glue_cap_insert_blk_dev_ops_type(struct glue_cspace *cspace,
			struct block_device_operations_container *blo_container,
			cptr_t *c_out) { 

	 return glue_cspace_insert(cspace, blo_container,
			 	glue_libcap_type_ops[GLUE_TYPE_BLK_DEV_OPS].libcap_type,
				c_out);

}

int glue_cap_insert_blk_mq_tag_set_type(struct glue_cspace *cspace,
			struct blk_mq_tag_set_container *set_container,
			 cptr_t *c_out) {
	
       return glue_cspace_insert(cspace, set_container,  
                                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_TAG_SET].libcap_type,
                                c_out);
}

int glue_cap_insert_gendisk_type(struct glue_cspace *cspace,
			struct gendisk_container *disk_container,
			cptr_t *c_out) {

        return glue_cspace_insert(cspace, disk_container,  
                                glue_libcap_type_ops[GLUE_TYPE_GENDISK].libcap_type,
                                c_out);

}

int glue_cap_insert_blk_mq_hw_ctx_type(struct glue_cspace *cspace,
			struct blk_mq_hw_ctx_container *ctx_container,
			cptr_t *c_out) {

        return glue_cspace_insert(cspace, ctx_container,  
                                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_HW_CTX].libcap_type,
                                c_out);

}

int glue_cap_insert_blk_mq_queue_data_type(struct glue_cspace *cspace,
			struct blk_mq_queue_data_container *bd_container,
			cptr_t *c_out) {

        return glue_cspace_insert(cspace, bd_container,  
                                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_QUEUE_DATA].libcap_type,
                                c_out);

}

int glue_cap_insert_request_queue_type(struct glue_cspace *cspace,
			struct request_queue_container *req_queue_container,
			cptr_t *c_out) {

        return glue_cspace_insert(cspace, req_queue_container,  
                                glue_libcap_type_ops[GLUE_TYPE_REQUEST_QUEUE].libcap_type,
                                c_out);

}

int glue_cap_lookup_blk_mq_ops_type(struct glue_cspace *cspace,
			cptr_t c,
			 struct blk_mq_ops_container **blk_mq_ops_container) {

        return glue_cspace_lookup(
                cspace, c,
                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_OPS].libcap_type,
                (void **)blk_mq_ops_container);

}

int glue_cap_lookup_blk_mq_tag_set_type(struct glue_cspace *cspace,
			cptr_t c,
			struct blk_mq_tag_set_container **set_container) {

        return glue_cspace_lookup(
                cspace, c,
                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_TAG_SET].libcap_type,
                (void **)set_container);

}

int glue_cap_lookup_blk_dev_ops_type(struct glue_cspace *cspace,
			cptr_t c,
			struct block_device_operations_container **blo_container) {

        return glue_cspace_lookup(
                cspace, c,
                glue_libcap_type_ops[GLUE_TYPE_BLK_DEV_OPS].libcap_type,
                (void **)blo_container);

}

int glue_cap_lookup_gendisk_type(struct glue_cspace *cspace,
			cptr_t c,
			struct gendisk_container **disk_container) {

        return glue_cspace_lookup(
                cspace, c,
                glue_libcap_type_ops[GLUE_TYPE_GENDISK].libcap_type,
                (void **)disk_container);

}

int glue_cap_lookup_blk_mq_hw_ctx_type(struct glue_cspace *cspace,
			cptr_t c,
			 struct blk_mq_hw_ctx_container **ctx_container) {

        return glue_cspace_lookup(
                cspace, c,
                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_HW_CTX].libcap_type,
                (void **)ctx_container);

}

int glue_cap_lookup_blk_mq_queue_data_type(struct glue_cspace *cspace,
			cptr_t c,
			struct blk_mq_queue_data_container **bd_container) {

        return glue_cspace_lookup(
                cspace, c,
                glue_libcap_type_ops[GLUE_TYPE_BLK_MQ_QUEUE_DATA].libcap_type,
                (void **)bd_container);

}

int glue_cap_lookup_module_type(struct glue_cspace *cspace,
        			cptr_t c,
        			struct module_container **module_container) {

        return glue_cspace_lookup(
                cspace, c,
                glue_libcap_type_ops[GLUE_TYPE_MODULE].libcap_type,
                (void **)module_container);
}

int glue_cap_lookup_request_queue_type(struct glue_cspace *cspace,
			cptr_t c,
			struct request_queue_container **req_queue_container) {

        return glue_cspace_lookup(
                cspace, c,
                glue_libcap_type_ops[GLUE_TYPE_REQUEST_QUEUE].libcap_type,
                (void **)req_queue_container);

}
void glue_cap_remove(
        struct glue_cspace *cspace, 
        cptr_t c)
{
        glue_cspace_remove(cspace, c); 
}

