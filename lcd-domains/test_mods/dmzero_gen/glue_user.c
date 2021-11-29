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
    glue_user_trace("Committing to KLCD call");
    vmfunc_wrapper(msg);
	glue_user_trace("Completed call in LCD");
}

void glue_user_call_client(struct fipc_message* msg, size_t id)
{
    msg->vmfunc_id = VMFUNC_RPC_CALL;
    msg->rpc_id = id;
    glue_user_trace("Committing to LCD call");
    vmfunc_klcd_wrapper(msg, OTHER_DOMAIN);
	glue_user_trace("Completed call in KLCD");
}

void* glue_user_map_to_shadow(const void* obj, bool fail)
{
    struct shadow_link *link;

    LIBLCD_MSG("Lookup for key %p\n", obj);
    hash_for_each_possible(to_shadow_ht, link,
                    to_hentry, (unsigned long) obj) {
        if (!link)
            glue_user_panic("Null detected in shadow_ht");

        if (link->object == obj) {
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

    LIBLCD_MSG("Lookup for key %p\n", shadow);
    hash_for_each_possible(shadow_ht, link,
    		    hentry, (unsigned long) shadow) {
        if (!link)
            glue_user_panic("Null detected in shadow_ht");

        if (link->shadow == shadow) {
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
    LIBLCD_MSG("Inserted shadow with <key %p, ptr %p>\n", shadow, ptr);
}

void* glue_user_alloc(size_t size, gfp_t flags)
{
    void* ptr = kzalloc(size, flags);
    if (!ptr) {
        glue_user_panic("Couldn't allocate");
    }
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

// TODO
