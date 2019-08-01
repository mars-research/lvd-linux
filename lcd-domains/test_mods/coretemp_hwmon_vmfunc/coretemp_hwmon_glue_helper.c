/*
 * cap.c
 *
 * cspace code for acpi_hwmon
 */

#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <linux/slab.h>
#include "coretemp_hwmon_glue_helper.h"

#include <lcd_config/post_hook.h>

static struct cap_type_system *glue_libcap_type_system;

struct type_ops_id {
	struct cap_type_ops ops;
	cap_type_t libcap_type;
};

enum glue_type {
	GLUE_TYPE_ATTRIBUTE_GROUP_CONTAINER,
	GLUE_TYPE_DEVICE_CONTAINER,
	GLUE_TYPE_KOBJECT_CONTAINER,
	GLUE_TYPE_MODULE_CONTAINER,
	GLUE_TYPE_NOTIFIER_BLOCK_CONTAINER,
	GLUE_TYPE_PCI_DEV_CONTAINER,
	GLUE_TYPE_PLATFORM_DEVICE_CONTAINER,
	GLUE_TYPE_PLATFORM_DRIVER_CONTAINER,
	GLUE_TYPE_X86_CPU_ID_CONTAINER,
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
			.name = "struct attribute_group",
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
			.name = "struct kobject",
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
			.name = "struct notifier_block",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct pci_dev",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct platform_device",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct platform_driver",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct x86_cpu_id",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},

};

int glue_cap_insert_attribute_group_type(struct glue_cspace *cspace,
		struct attribute_group_container *attribute_group_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		attribute_group_container,
		glue_libcap_type_ops[ GLUE_TYPE_ATTRIBUTE_GROUP_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_attribute_group_type(struct glue_cspace *cspace,
		struct cptr c,
		struct attribute_group_container **attribute_group_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_ATTRIBUTE_GROUP_CONTAINER ].libcap_type,
		( void  ** )attribute_group_container);

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

int glue_cap_insert_notifier_block_type(struct glue_cspace *cspace,
		struct notifier_block_container *notifier_block_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		notifier_block_container,
		glue_libcap_type_ops[ GLUE_TYPE_NOTIFIER_BLOCK_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_notifier_block_type(struct glue_cspace *cspace,
		struct cptr c,
		struct notifier_block_container **notifier_block_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_NOTIFIER_BLOCK_CONTAINER ].libcap_type,
		( void  ** )notifier_block_container);

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

int glue_cap_insert_platform_device_type(struct glue_cspace *cspace,
		struct platform_device_container *platform_device_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		platform_device_container,
		glue_libcap_type_ops[ GLUE_TYPE_PLATFORM_DEVICE_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_platform_device_type(struct glue_cspace *cspace,
		struct cptr c,
		struct platform_device_container **platform_device_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_PLATFORM_DEVICE_CONTAINER ].libcap_type,
		( void  ** )platform_device_container);

}

int glue_cap_insert_platform_driver_type(struct glue_cspace *cspace,
		struct platform_driver_container *platform_driver_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		platform_driver_container,
		glue_libcap_type_ops[ GLUE_TYPE_PLATFORM_DRIVER_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_platform_driver_type(struct glue_cspace *cspace,
		struct cptr c,
		struct platform_driver_container **platform_driver_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_PLATFORM_DRIVER_CONTAINER ].libcap_type,
		( void  ** )platform_driver_container);

}

int glue_cap_insert_x86_cpu_id_type(struct glue_cspace *cspace,
		struct x86_cpu_id_container *x86_cpu_id_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		x86_cpu_id_container,
		glue_libcap_type_ops[ GLUE_TYPE_X86_CPU_ID_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_x86_cpu_id_type(struct glue_cspace *cspace,
		struct cptr c,
		struct x86_cpu_id_container **x86_cpu_id_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_X86_CPU_ID_CONTAINER ].libcap_type,
		( void  ** )x86_cpu_id_container);

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
