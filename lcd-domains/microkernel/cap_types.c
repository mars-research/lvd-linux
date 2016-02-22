/*
 * cap_types.c -- Capability types the microkernel uses
 *
 * Copyright: University of Utah
 */
#include <linux/gfp.h>
#include <linux/list.h>
#include <linux/bitops.h>
#include <linux/slab.h>
#include <linux/log2.h>
#include <libcap.h>
#include <lcd_domains/microkernel.h>

struct cap_type_system *lcd_libcap_type_system;

/* CAP DEBUGGING -------------------------------------------------- */

int cap_debug_level = LCD_LIBCAP_DEBUG_LVL;
EXPORT_SYMBOL(cap_debug_level);

/* ------------------------------------------------------------ */

/* struct for internal use to store all information about a type. */
struct type_ops_id {
	struct cap_type_ops ops;
	cap_type_t libcap_type;
};

/* REVOCATION CALLBACKS ---------------------------------------- */

static int mem_object_revoke(struct cspace *cspace, struct cnode *cnode,
			void *object)
{
	struct lcd_mapping_metadata *m;
	struct lcd_memory_object *mo = (struct lcd_memory_object *)object;
	/*
	 * Extract metadata
	 */
	m = cap_cnode_metadata(cnode);
	/*
	 * Unmap memory object
	 */
	__lcd_do_unmap_memory_object(cap_cspace_getowner(cspace), mo, m);
	/*
	 * Free metadata
	 */
	if (m)
		kfree(m);

	return 0;
}

static int page_revoke(struct cspace *cspace, struct cnode *cnode,
			void *object)
{
	return mem_object_revoke(cspace, cnode, object);
}

static int vmalloc_mem_revoke(struct cspace *cspace, struct cnode *cnode,
			void *object)
{
	return mem_object_revoke(cspace, cnode, object);
}

static int volunteered_page_revoke(struct cspace *cspace, struct cnode *cnode,
				void *object)
{
	return mem_object_revoke(cspace, cnode, object);
}

static int volunteered_dev_mem_revoke(struct cspace *cspace, 
				struct cnode *cnode, void *object)
{
	return mem_object_revoke(cspace, cnode, object);
}

static int volunteered_vmalloc_mem_revoke(struct cspace *cspace, 
					struct cnode *cnode, void *object)
{
	return mem_object_revoke(cspace, cnode, object);
}

static int sync_endpoint_revoke(struct cspace *cspace, struct cnode *cnode,
				void *object)
{
	int ret;
	struct list_head *cursor, *next;
	struct lcd_sync_endpoint *e;
	struct lcd *lcd;

	lcd = cap_cspace_getowner(cspace);
	e = (struct lcd_sync_endpoint *)object;
	
	/*
	 * Lock the endpoint, and see if lcd is in its queues
	 */
	ret = mutex_lock_interruptible(&e->lock);
	if (ret) {
		LCD_ERR("interrupted");
		goto out1;
	}
	
	list_for_each_safe(cursor, next, &e->senders) {
		if (cursor == &lcd->endpoint_queue) {
			/*
			 * Set xmit flag to fail so lcd knows
			 */
			atomic_set(&lcd->xmit_flag, LCD_XMIT_FAILED);
			list_del_init(cursor);
			goto out2;
		}
	}

	list_for_each_safe(cursor, next, &e->receivers) {
		if (cursor == &lcd->endpoint_queue) {
			/*
			 * Set xmit flag to fail so lcd knows
			 */
			atomic_set(&lcd->xmit_flag, LCD_XMIT_FAILED);
			list_del_init(cursor);
			goto out2;
		}
	}

out2:
	mutex_unlock(&e->lock);
out1:
	return 0;
}

static int lcd_revoke(struct cspace *cspace, struct cnode *cnode,
		void *object)
{
	/* no-op for now */
	return 0;
}

static int klcd_revoke(struct cspace *cspace, struct cnode *cnode,
			void *object)
{
	/* no-op for now */
	return 0;
}

/* DELETE CALLBACKS ---------------------------------------- */

static int page_delete(struct cspace *cspace, struct cnode *cnode,
			void *object)
{
	struct lcd_memory_object *mo = object;
	/*
	 * Remove memory object from global memory interval tree
	 */
	__lcd_mem_itree_delete(mo);
	/*
	 * Pages should be unmapped from all LCDs and not in use
	 * by any code. Free them.
	 */
	__free_pages(mo->object, ilog2(mo->nr_pages));
	/*
	 * Free mo, no one else should have a reference.
	 */
	kfree(mo);

	return 0;
}

static int vmalloc_mem_delete(struct cspace *cspace, struct cnode *cnode,
			void *object)
{
	struct lcd_memory_object *mo = object;
	/*
	 * Remove memory object from global memory interval tree
	 */
	__lcd_mem_itree_delete(mo);
	/*
	 * vmalloc memory should be unmapped from all LCDs and not in use
	 * by any code. Free it.
	 */
	vfree(mo->object);
	/*
	 * Free mo, no one else should have a reference.
	 */
	kfree(mo);

	return 0;
}

static int volunteered_page_delete(struct cspace *cspace, struct cnode *cnode,
				void *object)
{
	struct lcd_memory_object *mo = object;
	/*
	 * Remove memory object from global memory interval tree
	 */
	__lcd_mem_itree_delete(mo);
	/*
	 * Do not free the actual pages. The thread that "volunteered" 
	 * the pages is responsible for freeing them after they are removed
	 * from the capability system.
	 *
	 * Free the struct memory_object though (this is what is actually
	 * stored in the cspace, and contains a further pointer to the
	 * struct page).
	 */
	kfree(mo);

	return 0;
}

static int volunteered_dev_mem_delete(struct cspace *cspace, 
				struct cnode *cnode, void *object)
{
	struct lcd_memory_object *mo = object;
	/*
	 * Remove memory object from global memory interval tree
	 */
	__lcd_mem_itree_delete(mo);
	/*
	 * The device memory itself (e.g., high memory, ISA memory) shouldn't
	 * need to be "freed".
	 *
	 * XXX: Correct? There might be more sophisticated tear-down
	 * required in the future.
	 *
	 * Free the struct memory_object.
	 */
	kfree(mo);
	
	return 0;
}

static int volunteered_vmalloc_mem_delete(struct cspace *cspace, 
					struct cnode *cnode, void *object)
{
	struct lcd_memory_object *mo = object;
	/*
	 * Remove memory object from global memory interval tree
	 */
	__lcd_mem_itree_delete(mo);
	/*
	 * Just like volunteered pages, the original volunteer is
	 * responsible for freeing the memory.
	 */
	kfree(mo);

	return 0;
}

static int sync_endpoint_delete(struct cspace *cspace, struct cnode *cnode,
				void *object)
{
	/*
	 * No one has a capability to the endpoint, so no one should
	 * be in the queues; destroy endpoint
	 */
	__lcd_destroy_sync_endpoint__(object);

	return 0;
}

static int lcd_delete(struct cspace *cspace, struct cnode *cnode,
		void *object)
{
	__lcd_destroy(object);
	
	return 0;
}

static int klcd_delete(struct cspace *cspace, struct cnode *cnode,
			void *object)
{
	__lcd_destroy_no_vm(object);

	return 0;
}

/* INTERNAL DATA -------------------------------------------------- */

static struct type_ops_id mk_type_ops_id[LCD_MICROKERNEL_NUM_CAP_TYPES] = {
	{
		{
			.name = "page",
			.delete = page_delete,
			.revoke = page_revoke,
		}
	},
	{
		{
			.name = "vmalloc memory",
			.delete = vmalloc_mem_delete,
			.revoke = vmalloc_mem_revoke,
		}
	},
	{
		{
			.name = "volunteered page",
			.delete = volunteered_page_delete,
			.revoke = volunteered_page_revoke,
		}
	},
	{
		{
			.name = "volunteered device memory",
			.delete = volunteered_dev_mem_delete,
			.revoke = volunteered_dev_mem_revoke,
		}
	},
	{
		{
			.name = "volunteered vmalloc memory",
			.delete = volunteered_vmalloc_mem_delete,
			.revoke = volunteered_vmalloc_mem_revoke,
		}
	},
	{
		{
			.name = "lcd",
			.delete = lcd_delete,
			.revoke = lcd_revoke,
		}
	},
	{
		{
			.name = "klcd",
			.delete = klcd_delete,
			.revoke = klcd_revoke,
		}
	},
	{
		{
			.name = "sync ipc endpoint",
			.delete = sync_endpoint_delete,
			.revoke = sync_endpoint_revoke,
		}
	},
};

/* MISC -------------------------------------------------- */

inline cap_type_t __lcd_get_libcap_type(enum lcd_microkernel_type_id t_id)
{
	return mk_type_ops_id[t_id].libcap_type;
}

/* INIT/EXIT -------------------------------------------------- */

int __lcd_init_cap_types(void)
{
	int ret;
	int i;
	cap_type_t libcap_type;
	/*
	 * Alloc and init microkernel type system
	 */
	ret = cap_type_system_alloc(&lcd_libcap_type_system);
	if (ret) {
		LCD_ERR("alloc microkernel type system failed");
		goto fail1;
	}
	ret = cap_type_system_init(lcd_libcap_type_system);
	if (ret) {
		LCD_ERR("init microkernel type system failed");
		goto fail2;
	}
	/*
	 * Add types
	 */
	for (i = 0; i < LCD_MICROKERNEL_NUM_CAP_TYPES; i++) {

		libcap_type = cap_register_private_type(
			lcd_libcap_type_system,
			0,
			&mk_type_ops_id[i].ops);
		if (libcap_type == CAP_TYPE_ERR) {
			LCD_ERR("failed to register microkernel cap type %s",
				mk_type_ops_id[i].ops.name);
			ret = -EIO;
			goto fail3;
		}
		mk_type_ops_id[i].libcap_type = libcap_type;

	}

	return 0;

fail3:
	cap_type_system_destroy(lcd_libcap_type_system);
fail2:
	cap_type_system_free(lcd_libcap_type_system);
	lcd_libcap_type_system = NULL;
fail1:
	return ret;
}


void __lcd_exit_cap_types(void)
{
	/*
	 * Destroy and free type system if necessary
	 */
	if (lcd_libcap_type_system) {
		cap_type_system_destroy(lcd_libcap_type_system);
		cap_type_system_free(lcd_libcap_type_system);
		lcd_libcap_type_system = NULL;
	}
}
