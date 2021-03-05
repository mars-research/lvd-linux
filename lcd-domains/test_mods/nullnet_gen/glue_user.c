#include <lcd_config/pre_hook.h>

#include "common.h"
#include <liblcd/liblcd.h>
#include <libfipc_types.h>
#include <asm/lcd_domains/libvmfunc.h>
#include <linux/hashtable.h>

#include <lcd_config/post_hook.h>

DEFINE_HASHTABLE(shadow_ht, 4);
DEFINE_HASHTABLE(from_shadow, 4);
bool initialized = 0;

struct shadow_link {
    struct hlist_node hentry;
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

void glue_user_call_server(uint64_t* data, size_t id)
{
    struct fipc_message buffer = {0};
    struct fipc_message* msg = &buffer;
    struct ext_registers* page = 0;
	size_t len = data[0];
    int i;

    glue_user_trace("Original sent:");
    for (i = 0; i < data[0] + 1; ++i) {
        LIBLCD_MSG("%llx", data[i]);
    }

    glue_user_trace("Packing message in LCD");
	memcpy(msg->regs, data, 7 * sizeof(uint64_t));
	glue_user_trace("Packed fast regs");
	if (len > 6) {
		glue_user_trace("Fetching slow regs");
		page = get_register_page(smp_processor_id());
		glue_user_trace("Fetched slow regs");
		memcpy(page->regs, &data[7], (len - 6) * sizeof(uint64_t));
		glue_user_trace("Packed slow regs");
	}

    glue_user_trace("Fast regs sent:");
    for (i = 0; i < 7; ++i) {
        LIBLCD_MSG("%llx", msg->regs[i]);
    }

    if (data[0] > 6) {
        glue_user_trace("Slow regs sent:");
        for (i = 7; i < msg->regs[0] - 6; ++i) {
            LIBLCD_MSG("%llx", page->regs[i]);
        }
    }

    msg->vmfunc_id = VMFUNC_RPC_CALL;
    msg->rpc_id = id;
    glue_user_trace("Committing to KLCD call");
    vmfunc_wrapper(msg);

    glue_user_trace("Unpacking message in LCD");
	len = msg->regs[0];
	memcpy(data, msg->regs, 7 * sizeof(uint64_t));
	glue_user_trace("Unpacked fast regs");
	if (len > 6) {
		glue_user_trace("Fetching slow regs");
		page = get_register_page(smp_processor_id());
		glue_user_trace("Fetched slow regs");
		memcpy(&data[7], page->regs, (len - 6) * sizeof(uint64_t));
		glue_user_trace("Unpacked slow regs");
	}

	glue_user_trace("Completed call in LCD");
}

void glue_user_call_client(uint64_t* data, size_t id)
{
    struct fipc_message buffer = {0};
    struct fipc_message* msg = &buffer;
    struct ext_registers* page = 0;
	size_t len = data[0];
    int i;

    glue_user_trace("Original sent:");
    for (i = 0; i < data[0] + 1; ++i) {
        LIBLCD_MSG("%llx", data[i]);
    }

    glue_user_trace("Packing message in KLCD");
	memcpy(msg->regs, data, 7);
	glue_user_trace("Packed fast regs");
	if (len > 6) {
		glue_user_trace("Fetching slow regs");
		page = get_register_page(smp_processor_id());
		glue_user_trace("Fetched slow regs");
		memcpy(page->regs, &data[7], len - 6);
		glue_user_trace("Packed slow regs");
	}

    glue_user_trace("Fast regs sent:");
    for (i = 0; i < 7; ++i) {
        LIBLCD_MSG("%llx", msg->regs[i]);
    }

    if (data[0] > 6) {
        glue_user_trace("Slow regs sent:");
        for (i = 7; i < msg->regs[0] - 6; ++i) {
            LIBLCD_MSG("%llx", page->regs[i]);
        }
    }

    msg->vmfunc_id = VMFUNC_RPC_CALL;
    msg->rpc_id = id;
    glue_user_trace("Committing to LCD call");
    vmfunc_klcd_wrapper(msg, 1);

    glue_user_trace("Unpacking message in KLCD");
	len = msg->regs[0];
	memcpy(data, msg->regs, 7);
	glue_user_trace("Unpacked fast regs");
	if (len > 6) {
		glue_user_trace("Fetching slow regs");
		page = get_register_page(smp_processor_id());
		glue_user_trace("Fetched slow regs");
		memcpy(&data[7], page->regs, len - 6);
		glue_user_trace("Unpacked slow regs");
	}

	glue_user_trace("Completed call in KLCD");
}

void* glue_user_map_to_shadow(const void* obj)
{
    glue_user_panic("glue_user_map_to_shadow");
    return 0;
}

const void* glue_user_map_from_shadow(const void* shadow)
{
    struct shadow_link *link;

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

    glue_user_panic("Remote for shadow was not found");

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
    glue_user_trace("Inserted shadow");
}

void* glue_user_alloc(size_t size)
{
    void* ptr = kzalloc(size, GFP_KERNEL);
    if (!ptr) {
        glue_user_panic("Couldn't allocate");
    }

    return ptr;
}

void glue_user_free(void* ptr)
{
    kfree(ptr);
}

void glue_user_init(void)
{
    hash_init(shadow_ht);
    hash_init(from_shadow);
}

// TODO
