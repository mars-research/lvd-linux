/*
 * cap.c
 *
 * cspace code for acpi_hwmon
 */

#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <linux/slab.h>
#include "acpi_hwmon_glue_helper.h"

#include <lcd_config/post_hook.h>

/* ------------------------------------------------------------ */

static struct cap_type_system *glue_libcap_type_system;

struct type_ops_id {
	struct cap_type_ops ops;
	cap_type_t libcap_type;
};


enum glue_type {
	GLUE_TYPE_ACPI_BUFFER_CONTAINER,
	GLUE_TYPE_ACPI_DEVICE_CONTAINER,
	GLUE_TYPE_ACPI_DEVICE_PTR_CONTAINER,
	GLUE_TYPE_ACPI_DRIVER_CONTAINER,
	GLUE_TYPE_ACPI_DEVICE_OPS_CONTAINER,
	GLUE_TYPE_ACPI_OBJECT_CONTAINER,
	GLUE_TYPE_ACPI_OBJECT_LIST_CONTAINER,
	GLUE_TYPE_DEVICE_ATTRIBUTE_CONTAINER,
	GLUE_TYPE_DEVICE_CONTAINER,
	GLUE_TYPE_DMI_SYSTEM_ID_CONTAINER,
	GLUE_TYPE_KOBJECT_CONTAINER,
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
			.name = "struct acpi_buffer",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct acpi_device",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct acpi_device_ptr",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct acpi_driver",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct acpi_device_ops",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct acpi_object",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct acpi_object_list",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct device_attribute",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct device",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct dmi_system_id",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct kobject",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	}

};

int glue_cap_insert_acpi_buffer_type(struct glue_cspace *cspace,
		struct acpi_buffer_container *acpi_buffer_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		acpi_buffer_container,
		glue_libcap_type_ops[ GLUE_TYPE_ACPI_BUFFER_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_acpi_buffer_type(struct glue_cspace *cspace,
		struct cptr c,
		struct acpi_buffer_container **acpi_buffer_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_ACPI_BUFFER_CONTAINER ].libcap_type,
		( void  ** )acpi_buffer_container);

}

int glue_cap_insert_acpi_device_type(struct glue_cspace *cspace,
		struct acpi_device_container *acpi_device_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		acpi_device_container,
		glue_libcap_type_ops[ GLUE_TYPE_ACPI_DEVICE_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_acpi_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct acpi_device_container **acpi_device_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_ACPI_DEVICE_CONTAINER ].libcap_type,
		( void  ** )acpi_device_container);

}

int glue_cap_insert_acpi_device_ptr_type(struct glue_cspace *cspace,
		struct acpi_device_ptr_container *acpi_device_ptr_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		acpi_device_ptr_container,
		glue_libcap_type_ops[ GLUE_TYPE_ACPI_DEVICE_PTR_CONTAINER ].libcap_type,
		c_out);
}

int glue_cap_lookup_acpi_device_ptr_type(struct glue_cspace *cspace,
		struct cptr c,
		struct acpi_device_ptr_container **acpi_device_ptr_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_ACPI_DEVICE_PTR_CONTAINER ].libcap_type,
		( void  ** )acpi_device_ptr_container);
}

int glue_cap_insert_acpi_driver_type(struct glue_cspace *cspace,
		struct acpi_driver_container *acpi_driver_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		acpi_driver_container,
		glue_libcap_type_ops[ GLUE_TYPE_ACPI_DRIVER_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_acpi_driver_type(struct glue_cspace *cspace,
		struct cptr c,
		struct acpi_driver_container **acpi_driver_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_ACPI_DRIVER_CONTAINER ].libcap_type,
		( void  ** )acpi_driver_container);

}

int glue_cap_insert_acpi_device_ops_type(struct glue_cspace *cspace,
		struct acpi_device_ops_container *acpi_device_ops_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		acpi_device_ops_container,
		glue_libcap_type_ops[ GLUE_TYPE_ACPI_DEVICE_OPS_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_acpi_device_ops_type(struct glue_cspace *cspace,
		struct cptr c,
		struct acpi_device_ops_container **acpi_device_ops_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_ACPI_DEVICE_OPS_CONTAINER ].libcap_type,
		( void  ** )acpi_device_ops_container);

}

int glue_cap_insert_acpi_object_type(struct glue_cspace *cspace,
		struct acpi_object_container *acpi_object_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		acpi_object_container,
		glue_libcap_type_ops[ GLUE_TYPE_ACPI_OBJECT_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_acpi_object_type(struct glue_cspace *cspace,
		struct cptr c,
		struct acpi_object_container **acpi_object_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_ACPI_OBJECT_CONTAINER ].libcap_type,
		( void  ** )acpi_object_container);

}

int glue_cap_insert_acpi_object_list_type(struct glue_cspace *cspace,
		struct acpi_object_list_container *acpi_object_list_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		acpi_object_list_container,
		glue_libcap_type_ops[ GLUE_TYPE_ACPI_OBJECT_LIST_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_acpi_object_list_type(struct glue_cspace *cspace,
		struct cptr c,
		struct acpi_object_list_container **acpi_object_list_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_ACPI_OBJECT_LIST_CONTAINER ].libcap_type,
		( void  ** )acpi_object_list_container);

}

int glue_cap_insert_device_attribute_type(struct glue_cspace *cspace,
		struct device_attribute_container *device_attribute_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		device_attribute_container,
		glue_libcap_type_ops[ GLUE_TYPE_DEVICE_ATTRIBUTE_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_device_attribute_type(struct glue_cspace *cspace,
		struct cptr c,
		struct device_attribute_container **device_attribute_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_DEVICE_ATTRIBUTE_CONTAINER ].libcap_type,
		( void  ** )device_attribute_container);

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

int glue_cap_insert_dmi_system_id_type(struct glue_cspace *cspace,
		struct dmi_system_id_container *dmi_system_id_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		dmi_system_id_container,
		glue_libcap_type_ops[ GLUE_TYPE_DMI_SYSTEM_ID_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_dmi_system_id_type(struct glue_cspace *cspace,
		struct cptr c,
		struct dmi_system_id_container **dmi_system_id_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_DMI_SYSTEM_ID_CONTAINER ].libcap_type,
		( void  ** )dmi_system_id_container);

}

int glue_cap_insert_kobject_type(struct glue_cspace *cspace,
		struct kobject_container *kobject_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		kobject_container,
		glue_libcap_type_ops[ GLUE_TYPE_KOBJECT_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_kobject_type(struct glue_cspace *cspace,
		struct cptr c,
		struct kobject_container **kobject_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_KOBJECT_CONTAINER ].libcap_type,
		( void  ** )kobject_container);

}

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

void glue_cap_remove(
	struct glue_cspace *cspace, 
	cptr_t c)
{
	glue_cspace_remove(cspace, c);
}

