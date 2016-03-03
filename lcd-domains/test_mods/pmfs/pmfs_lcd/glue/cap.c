/*
 * cap.c
 *
 * cspace code for pmfs
 */

#include <lcd_config/pre_hook.h>

#include <libcap.h>
#include <liblcd/liblcd.h>
#include <linux/slab.h>
#include "../internal.h"

#include <lcd_config/post_hook.h>

/* ------------------------------------------------------------ */

static struct cap_type_system *glue_libcap_type_system;

struct type_ops_id {
	struct cap_type_ops ops;
	cap_type_t libcap_type;
};

enum glue_type {
	GLUE_TYPE_FILE_SYSTEM_TYPE,
	GLUE_TYPE_BACKING_DEV_INFO,
	GLUE_TYPE_MODULE,
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
			.name = "struct file_system_type",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct backing_dev_info",
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

int glue_cap_insert_file_system_type_type(
	struct glue_cspace *cspace, 
	struct file_system_type_container *file_system_type_container,
	cptr_t *c_out)
{
	return glue_cspace_insert(cspace, file_system_type_container,  
				glue_libcap_type_ops[GLUE_TYPE_FILE_SYSTEM_TYPE].libcap_type,
				c_out);
}

int glue_cap_insert_backing_dev_info_type(
	struct glue_cspace *cspace, 
	struct backing_dev_info_container *backing_dev_info_container,
	cptr_t *c_out)
{
	return glue_cspace_insert(cspace, backing_dev_info_container,  
				glue_libcap_type_ops[GLUE_TYPE_BACKING_DEV_INFO].libcap_type,
				c_out);
}

int glue_cap_insert_module_type(
	struct glue_cspace *cspace, 
	struct module_container *module_container,
	cptr_t *c_out)
{
	return glue_cspace_insert(cspace, module_container,  
				glue_libcap_type_ops[GLUE_TYPE_MODULE].libcap_type,
				c_out);
}

int glue_cap_lookup_file_system_type_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct file_system_type_container **file_system_type_container)
{
	return glue_cspace_lookup(cspace, c, 
				glue_libcap_type_ops[GLUE_TYPE_FILE_SYSTEM_TYPE].libcap_type,
				(void **)file_system_type_container);
}

int glue_cap_lookup_backing_dev_info_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct backing_dev_info_container **backing_dev_info_container)
{
	return glue_cspace_lookup(
		cspace, c, 
		glue_libcap_type_ops[GLUE_TYPE_BACKING_DEV_INFO].libcap_type,
		(void **)backing_dev_info_container);
}

int glue_cap_lookup_module_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct module_container **module_container)
{
	return glue_cspace_lookup(
		cspace, c, 
		glue_libcap_type_ops[GLUE_TYPE_MODULE].libcap_type,
		(void **)module_container);
}

void glue_cap_remove(
	struct glue_cspace *cspace, 
	cptr_t c)
{
	glue_cspace_remove(cspace, c);
}
