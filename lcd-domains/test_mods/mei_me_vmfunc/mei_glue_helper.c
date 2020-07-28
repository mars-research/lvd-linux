#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <linux/slab.h>
#include "mei_glue_helper.h"

#include <lcd_config/post_hook.h>

/* ------------------------------------------------------------ */

static struct cap_type_system *glue_libcap_type_system;

struct type_ops_id {
	struct cap_type_ops ops;
	cap_type_t libcap_type;
};

static int dummy_func(struct cspace *cspace, struct cnode *cnode,
		void *object)
{
	return 0;
}

enum glue_type {
#if 0
	GLUE_TYPE_DELAYED_WORK_CONTAINER,
	GLUE_TYPE_IRQ_HANDLER_T_CONTAINER,
	GLUE_TYPE_MEI_CL_CB_CONTAINER,
	GLUE_TYPE_MEI_FW_STATUS_CONTAINER,
#endif
	GLUE_TYPE_MEI_HW_OPS_CONTAINER,
	//GLUE_TYPE_MEI_MSG_HDR_CONTAINER,
	//GLUE_TYPE_MODULE_CONTAINER,
	GLUE_TYPE_PCI_DRIVER_CONTAINER,
#if 0
	GLUE_TYPE_WORK_STRUCT_CONTAINER,
	GLUE_TYPE_WORKQUEUE_PROJECTION_CONTAINER,
	GLUE_TYPE___WAIT_QUEUE_CONTAINER,
	GLUE_TYPE___WAIT_QUEUE_HEAD_CONTAINER,
#endif
	GLUE_NR_TYPES,
};

static struct type_ops_id glue_libcap_type_ops[GLUE_NR_TYPES] = {
	{
		{
			.name = "struct mei_hw_ops",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct pci_driver",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
};

#if 0
int glue_cap_insert_delayed_work_type(struct glue_cspace *cspace,
		struct delayed_work_container *delayed_work_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		delayed_work_container,
		glue_libcap_type_ops[ GLUE_TYPE_DELAYED_WORK_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_delayed_work_type(struct glue_cspace *cspace,
		struct cptr c,
		struct delayed_work_container **delayed_work_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_DELAYED_WORK_CONTAINER ].libcap_type,
		( void  ** )delayed_work_container);

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

int glue_cap_insert_irq_handler_t_type(struct glue_cspace *cspace,
		struct irq_handler_t_container *irq_handler_t_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		irq_handler_t_container,
		glue_libcap_type_ops[ GLUE_TYPE_IRQ_HANDLER_T_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_irq_handler_t_type(struct glue_cspace *cspace,
		struct cptr c,
		struct irq_handler_t_container **irq_handler_t_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_IRQ_HANDLER_T_CONTAINER ].libcap_type,
		( void  ** )irq_handler_t_container);

}

int glue_cap_insert_mei_cl_cb_type(struct glue_cspace *cspace,
		struct mei_cl_cb_container *mei_cl_cb_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		mei_cl_cb_container,
		glue_libcap_type_ops[ GLUE_TYPE_MEI_CL_CB_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_mei_cl_cb_type(struct glue_cspace *cspace,
		struct cptr c,
		struct mei_cl_cb_container **mei_cl_cb_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_MEI_CL_CB_CONTAINER ].libcap_type,
		( void  ** )mei_cl_cb_container);

}

int glue_cap_insert_mei_fw_status_type(struct glue_cspace *cspace,
		struct mei_fw_status_container *mei_fw_status_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		mei_fw_status_container,
		glue_libcap_type_ops[ GLUE_TYPE_MEI_FW_STATUS_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_mei_fw_status_type(struct glue_cspace *cspace,
		struct cptr c,
		struct mei_fw_status_container **mei_fw_status_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_MEI_FW_STATUS_CONTAINER ].libcap_type,
		( void  ** )mei_fw_status_container);

}
#endif

int glue_cap_insert_mei_hw_ops_type(struct glue_cspace *cspace,
		struct mei_hw_ops_container *mei_hw_ops_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		mei_hw_ops_container,
		glue_libcap_type_ops[ GLUE_TYPE_MEI_HW_OPS_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_mei_hw_ops_type(struct glue_cspace *cspace,
		struct cptr c,
		struct mei_hw_ops_container **mei_hw_ops_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_MEI_HW_OPS_CONTAINER ].libcap_type,
		( void  ** )mei_hw_ops_container);

}

#if 0
int glue_cap_insert_mei_msg_hdr_type(struct glue_cspace *cspace,
		struct mei_msg_hdr_container *mei_msg_hdr_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		mei_msg_hdr_container,
		glue_libcap_type_ops[ GLUE_TYPE_MEI_MSG_HDR_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_mei_msg_hdr_type(struct glue_cspace *cspace,
		struct cptr c,
		struct mei_msg_hdr_container **mei_msg_hdr_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_MEI_MSG_HDR_CONTAINER ].libcap_type,
		( void  ** )mei_msg_hdr_container);

}

int glue_cap_insert_module_type(struct glue_cspace *cspace,
		struct module_container *module_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		module_container,
		glue_libcap_type_ops[ GLUE_TYPE_MODULE_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_module_type(struct glue_cspace *cspace,
		struct cptr c,
		struct module_container **module_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_MODULE_CONTAINER ].libcap_type,
		( void  ** )module_container);

}
#endif

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

#if 0
int glue_cap_insert_work_struct_type(struct glue_cspace *cspace,
		struct work_struct_container *work_struct_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		work_struct_container,
		glue_libcap_type_ops[ GLUE_TYPE_WORK_STRUCT_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_work_struct_type(struct glue_cspace *cspace,
		struct cptr c,
		struct work_struct_container **work_struct_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_WORK_STRUCT_CONTAINER ].libcap_type,
		( void  ** )work_struct_container);

}

int glue_cap_insert_workqueue_projection_type(struct glue_cspace *cspace,
		struct workqueue_projection_container *workqueue_projection_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		workqueue_projection_container,
		glue_libcap_type_ops[ GLUE_TYPE_WORKQUEUE_PROJECTION_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_workqueue_projection_type(struct glue_cspace *cspace,
		struct cptr c,
		struct workqueue_projection_container **workqueue_projection_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_WORKQUEUE_PROJECTION_CONTAINER ].libcap_type,
		( void  ** )workqueue_projection_container);

}

int glue_cap_insert___wait_queue_type(struct glue_cspace *cspace,
		struct __wait_queue_container *__wait_queue_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		__wait_queue_container,
		glue_libcap_type_ops[ GLUE_TYPE___WAIT_QUEUE_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup___wait_queue_type(struct glue_cspace *cspace,
		struct cptr c,
		struct __wait_queue_container **__wait_queue_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE___WAIT_QUEUE_CONTAINER ].libcap_type,
		( void  ** )__wait_queue_container);

}

int glue_cap_insert___wait_queue_head_type(struct glue_cspace *cspace,
		struct __wait_queue_head_container *__wait_queue_head_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		__wait_queue_head_container,
		glue_libcap_type_ops[ GLUE_TYPE___WAIT_QUEUE_HEAD_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup___wait_queue_head_type(struct glue_cspace *cspace,
		struct cptr c,
		struct __wait_queue_head_container **__wait_queue_head_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE___WAIT_QUEUE_HEAD_CONTAINER ].libcap_type,
		( void  ** )__wait_queue_head_container);

}
#endif

static struct cap_type_system *glue_libcap_type_system;

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
