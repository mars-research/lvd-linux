#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <linux/slab.h>
#include "msr_glue_helper.h"

#include <lcd_config/post_hook.h>


enum glue_type {
	GLUE_TYPE_CLASS_CONTAINER,
	GLUE_TYPE_DEVICE_CONTAINER,
	GLUE_TYPE_FILE_CONTAINER,
	GLUE_TYPE_FILE_OPERATIONS_CONTAINER,
	GLUE_TYPE_LOCK_CLASS_KEY_CONTAINER,
	GLUE_TYPE_MODULE_CONTAINER,
	GLUE_NR_TYPES,

};
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

int glue_cap_insert_lock_class_key_type(struct glue_cspace *cspace,
		struct lock_class_key_container *lock_class_key_container,
		struct cptr *c_out)
{
	return glue_cspace_insert(cspace,
		lock_class_key_container,
		glue_libcap_type_ops[ GLUE_TYPE_LOCK_CLASS_KEY_CONTAINER ].libcap_type,
		c_out);

}

int glue_cap_lookup_lock_class_key_type(struct glue_cspace *cspace,
		struct cptr c,
		struct lock_class_key_container **lock_class_key_container)
{
	return glue_cspace_lookup(cspace,
		c,
		glue_libcap_type_ops[ GLUE_TYPE_LOCK_CLASS_KEY_CONTAINER ].libcap_type,
		( void  ** )lock_class_key_container);

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
