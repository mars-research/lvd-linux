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
	GLUE_TYPE_NET_DEVICE,
	GLUE_TYPE_NET_DEVICE_OPS,
	GLUE_TYPE_NLATTR,
	GLUE_TYPE_RTNL_LINK_OPS,
	GLUE_TYPE_RTNL_LINK_STATS64,
	GLUE_TYPE_SK_BUFF,
	GLUE_TYPE_SETUP,
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
			.name = "struct net_device",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct net_device_ops",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct nlattr",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct rtnl_link_ops",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct rtnl_link_stats64",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "struct sk_buff",
			.delete = dummy_func,
			.revoke = dummy_func,
		}
	},
	{
		{
			.name = "alloc_netdev_mqs: setup",
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

int glue_cap_insert_net_device_type(
	struct glue_cspace *cspace, 
	struct net_device_container *net_device_container,
	cptr_t *c_out)
{
	return glue_cspace_insert(cspace, net_device_container,  
				glue_libcap_type_ops[GLUE_TYPE_NET_DEVICE].libcap_type,
				c_out);
}

int glue_cap_insert_net_device_ops_type(
	struct glue_cspace *cspace, 
	struct net_device_ops_container *net_device_ops_container,
	cptr_t *c_out)
{
	return glue_cspace_insert(cspace, net_device_ops_container,  
				glue_libcap_type_ops[GLUE_TYPE_NET_DEVICE_OPS].libcap_type,
				c_out);
}

int glue_cap_insert_nlattr_type(
	struct glue_cspace *cspace, 
	struct nlattr_container *nlattr_container,
	cptr_t *c_out)
{
	return glue_cspace_insert(cspace, nlattr_container,  
				glue_libcap_type_ops[GLUE_TYPE_NLATTR].libcap_type,
				c_out);
}

int glue_cap_insert_rtnl_link_ops_type(
	struct glue_cspace *cspace, 
	struct rtnl_link_ops_container *rtnl_link_ops_container,
	cptr_t *c_out)
{
	return glue_cspace_insert(cspace, rtnl_link_ops_container,  
				glue_libcap_type_ops[GLUE_TYPE_RTNL_LINK_OPS].libcap_type,
				c_out);
}

int glue_cap_insert_rtnl_link_stats64_type(
	struct glue_cspace *cspace, 
	struct rtnl_link_stats64_container *rtnl_link_stats64_container,
	cptr_t *c_out)
{
	return glue_cspace_insert(cspace, rtnl_link_stats64_container,  
				glue_libcap_type_ops[GLUE_TYPE_RTNL_LINK_STATS64].libcap_type,
				c_out);
}

int glue_cap_insert_sk_buff_type(
	struct glue_cspace *cspace, 
	struct sk_buff_container *sk_buff_container,
	cptr_t *c_out)
{
	return glue_cspace_insert(cspace, sk_buff_container,  
				glue_libcap_type_ops[GLUE_TYPE_SK_BUFF].libcap_type,
				c_out);
}

int glue_cap_insert_setup_type(
	struct glue_cspace *cspace, 
	struct setup_container *setup_container,
	cptr_t *c_out)
{
	return glue_cspace_insert(cspace, setup_container,  
				glue_libcap_type_ops[GLUE_TYPE_SETUP].libcap_type,
				c_out);
}

int glue_cap_lookup_net_device_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct net_device_container **net_device_container)
{
	return glue_cspace_lookup(cspace, c, 
				glue_libcap_type_ops[GLUE_TYPE_NET_DEVICE].libcap_type,
				(void **)net_device_container);
}

int glue_cap_lookup_net_device_ops_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct net_device_ops_container **net_device_ops_container)
{
	return glue_cspace_lookup(cspace, c, 
				glue_libcap_type_ops[GLUE_TYPE_NET_DEVICE_OPS].libcap_type,
				(void **)net_device_ops_container);
}

int glue_cap_lookup_nlattr_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct nlattr_container **nlattr_container)
{
	return glue_cspace_lookup(
		cspace, c, 
		glue_libcap_type_ops[GLUE_TYPE_NLATTR].libcap_type,
		(void **)nlattr_container);
}

int glue_cap_lookup_rtnl_link_ops_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct rtnl_link_ops_container **rtnl_link_ops_container)
{
	return glue_cspace_lookup(
		cspace, c, 
		glue_libcap_type_ops[GLUE_TYPE_RTNL_LINK_OPS].libcap_type,
		(void **)rtnl_link_ops_container);
}

int glue_cap_lookup_rtnl_link_stats64_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct rtnl_link_stats64_container **rtnl_link_stats64_container)
{
	return glue_cspace_lookup(
		cspace, c, 
		glue_libcap_type_ops[GLUE_TYPE_RTNL_LINK_STATS64].libcap_type,
		(void **)rtnl_link_stats64_container);
}

int glue_cap_lookup_sk_buff_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct sk_buff_container **sk_buff_container)
{
	return glue_cspace_lookup(
		cspace, c, 
		glue_libcap_type_ops[GLUE_TYPE_SK_BUFF].libcap_type,
		(void **)sk_buff_container);
}

int glue_cap_lookup_setup_type(
	struct glue_cspace *cspace, 
	cptr_t c,
	struct setup_container **setup_container)
{
	return glue_cspace_lookup(
		cspace, c, 
		glue_libcap_type_ops[GLUE_TYPE_SETUP].libcap_type,
		(void **)setup_container);
}

void glue_cap_remove(
	struct glue_cspace *cspace, 
	cptr_t c)
{
	glue_cspace_remove(cspace, c);
}

