#include <lcd_config/pre_hook.h>

#include "common.h"
#include <liblcd/liblcd.h>
#include <libfipc_types.h>
#include <asm/lcd_domains/libvmfunc.h>
#include <linux/hashtable.h>

#include <lcd_config/post_hook.h>

DEFINE_HASHTABLE(shadow_ht, 4);
DEFINE_HASHTABLE(to_shadow_ht, 4);
bool initialized = 0;

struct shadow_link {
    struct hlist_node hentry;
    struct hlist_node to_hentry;
    void* shadow;
    const void* object;
};

void glue_user_panic(const char* msg)
{
    LIBLCD_ERR(msg);
    while (true) {};
}

void glue_user_trace(const char* msg)
{
    LIBLCD_MSG(msg);
}

void glue_user_call_server(struct fipc_message* msg, size_t id)
{
    msg->vmfunc_id = VMFUNC_RPC_CALL;
    msg->rpc_id = id;
    if (verbose_debug)
        glue_user_trace("Committing to KLCD call");
    vmfunc_wrapper(msg);
    if (verbose_debug)
        glue_user_trace("Completed call in LCD");
}

void glue_user_call_client(struct fipc_message* msg, size_t id)
{
    msg->vmfunc_id = VMFUNC_RPC_CALL;
    msg->rpc_id = id;
    if (verbose_debug)
        glue_user_trace("Committing to LCD call");
    vmfunc_klcd_wrapper(msg, OTHER_DOMAIN);
    if (verbose_debug)
	    glue_user_trace("Completed call in KLCD");
}

void* glue_user_map_to_shadow(const void* obj, bool fail)
{
    struct shadow_link *link;

    if (verbose_debug)
        LIBLCD_MSG("Lookup for key %p\n", obj);
    hash_for_each_possible(to_shadow_ht, link,
                    to_hentry, (unsigned long) obj) {
        if (!link)
            glue_user_panic("Null detected in shadow_ht");

        if (link->object == obj) {
            if (verbose_debug)
                glue_user_trace("Found remote for shadow");
            if (!link->shadow)
                glue_user_panic("Remote for shadow was NULL");

            return link->shadow;
        }
    }

    if (fail)
        glue_user_panic("Remote for shadow was not found in to_shadow_ht");

    return NULL;
}

const void* glue_user_map_from_shadow(const void* shadow)
{
    struct shadow_link *link;

    if (verbose_debug)
        LIBLCD_MSG("Lookup for key %p\n", shadow);
    hash_for_each_possible(shadow_ht, link,
    		    hentry, (unsigned long) shadow) {
        if (!link)
            glue_user_panic("Null detected in shadow_ht");

        if (link->shadow == shadow) {
            if (verbose_debug)
                glue_user_trace("Found remote for shadow");
            if (!link->object)
                glue_user_panic("Remote for shadow was NULL");

            return link->object;
        }
    }

    glue_user_panic("Remote for shadow was not found in shadow_ht");

    return NULL;
}

void glue_user_add_shadow(const void* ptr, void* shadow)
{
    struct shadow_link *link = kzalloc(sizeof(*link), GFP_KERNEL);

    if (!ptr)
        glue_user_panic("Remote for shadowing was NULL");

    if (!shadow)
        glue_user_panic("New shadow was NULL");

    if (!link) {
        glue_user_panic("Couldn't allocate");
    }

    link->shadow = shadow;
    link->object = ptr;

    /* use shadow pointer as the key */
    hash_add(shadow_ht, &link->hentry, (unsigned long) shadow);
    hash_add(to_shadow_ht, &link->to_hentry, (unsigned long) link->object);
    if (verbose_debug)
        LIBLCD_MSG("Inserted shadow with <key %p, ptr %p>\n", shadow, ptr);
}

void glue_user_remove_shadow(void* shadow)
{
    if (!shadow)
        glue_user_panic("Shadow was NULL");

    // TODO: implement
    //
    LIBLCD_MSG("Remove shadow for shadow %p\n", shadow);
}


void* glue_user_alloc(size_t size, gfp_t flags)
{
    void* ptr = kzalloc(size, flags);
    if (!ptr) {
        glue_user_panic("Couldn't allocate");
    }
    if (verbose_debug)
        printk("%s, allocated %p | size %ld\n", __func__, ptr, size);

    return ptr;
}

void glue_user_free(void* ptr)
{
    kfree(ptr);
}

void glue_user_init(void)
{
    glue_user_trace("Initialized glue layer");
    hash_init(shadow_ht);
    hash_init(to_shadow_ht);
}

#if 0
#ifdef LCD_ISOLATE

void *skb_data_pool = NULL;

#define SKB_DATA_OBJ_SIZE	2048
#define SKB_DATA_POOL	0
void shared_mem_init(void) {
	struct fipc_message buffer = {0};
	struct fipc_message *msg = &buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	unsigned int pool_ord;
	cptr_t pool_cptr;
	gva_t pool_addr;
	int ret;

	ret = lcd_cptr_alloc(&pool_cptr);
	if (ret) {
		LIBLCD_ERR("failed to get cptr");
		goto fail_cptr;
	}

	// obj size
	glue_pack(pos, msg, ext, SKB_DATA_OBJ_SIZE);
	// cptr
	glue_pack(pos, msg, ext, cptr_val(pool_cptr));
	// pool type
	glue_pack(pos, msg, ext, SKB_DATA_POOL);

	glue_call_server(pos, msg, RPC_ID_shared_mem_init);

	*pos = 0;

	pool_ord = glue_unpack(pos, msg, ext, unsigned int);

	ret = lcd_map_virt(pool_cptr, pool_ord, &pool_addr);

	if (ret) {
		LIBLCD_ERR("failed to map pool");
		goto fail_pool;
	}

	LIBLCD_MSG("%s, mapping private pool %p | ord %d", __func__,
			gva_val(pool_addr), pool_ord);

	skb_data_pool = (void*)gva_val(pool_addr);
fail_pool:
fail_cptr:
	LIBLCD_ERR("%s, skb_data_pool uninitialized!", __func__);
	return;
}

#else

#include <linux/priv_mempool.h>
#include <linux/vmalloc.h>
#include <lcd_domains/microkernel.h>

#define SHARED_POOL_SIZE	(256UL << 20)
#define SHARED_POOL_PAGES	(SHARED_POOL_SIZE >> PAGE_SHIFT)
#define SHARED_POOL_ORDER	ilog2(roundup_pow_of_two(SHARED_POOL_PAGES))


priv_pool_t *skb_pool;
void *skb_data_pool = NULL;

void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* pos = &n_pos;

	size_t obj_size;
	int ret;
	size_t pool_size;
	unsigned int pool_ord;
	cptr_t pool_cptr;
	cptr_t lcd_pool_cptr;
	pool_type_t ptype;

	obj_size = glue_unpack(pos, msg, ext, size_t);
	lcd_pool_cptr = __cptr(glue_unpack(pos, msg, ext, uint64_t));

	ptype = glue_unpack(pos, msg, ext, pool_type_t);

	/*switch (ptype) {
		case SKB_DATA_POOL:
		case SKB_FRAG_POOL:
		case SKB_CONTAINER_POOL:
		case BLK_USER_BUF_POOL:
			break;
		default:
			printk("%s, Unknown pool type %d requested. Cannot initialize!\n", __func__, ptype);
			glue_user_panic("Check ptype arg!");
			break;
	}*/

	// allocate memory
	pool_size = SHARED_POOL_SIZE;
	skb_data_pool = vzalloc(pool_size);

	// initialize private pool
	skb_pool = priv_pool_init(ptype, (void*) skb_data_pool, pool_size, obj_size);
	// dump stats - debug info
	priv_pool_dumpstats(skb_pool);

	ret = lcd_volunteer_vmalloc_mem(__gva((unsigned long)skb_pool->pool),
			SHARED_POOL_PAGES, &pool_cptr);
	pool_ord = SHARED_POOL_ORDER;

	if (ret) {
		LIBLCD_ERR("volunteer shared region");
		goto fail_vol;
	}

	copy_msg_cap_vmfunc(current->lcd, current->vmfunc_lcd, pool_cptr,
			lcd_pool_cptr);

	*pos = 0;
	// return pool order
	glue_pack(pos, msg, ext, SHARED_POOL_ORDER);

	return;

fail_vol:
	// return pool order
	glue_pack(pos, msg, ext, 0);
	return;
}

void skb_data_shared_mem_uninit(void)
{
	if (skb_data_pool)
		vfree(skb_data_pool);

	if (skb_pool)
		priv_pool_destroy(skb_pool);
}
#endif
#endif
