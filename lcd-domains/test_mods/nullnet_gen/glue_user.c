#include <lcd_config/pre_hook.h>

#include "common.h"
#include <liblcd/liblcd.h>

#include <lcd_config/post_hook.h>

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

    glue_user_trace("Packing message in LCD");
	size_t len = data[0];
	memcpy(msg->regs, data, 7);
	glue_user_trace("Packed fast regs");
	if (len > 6) {
		glue_user_trace("Fetching slow regs");
		page = get_register_page(smp_processor_id());
		glue_user_trace("Fetched slow regs");
		memcpy(page->regs, &packet->slots[7], len - 6);
		glue_user_trace("Packed slow regs");
	}

    msg->rpc_id = id;
    glue_user_trace("Committing to KLCD call");
    vmfunc_wrapper(msg);

    glue_user_trace("Unpacking message in LCD");
	len = msg->regs[0];
	memcpy(data, msg->regs, 7);
	glue_user_trace("Unpacked fast regs");
	if (len > 6) {
		glue_user_trace("Fetching slow regs");
		page = get_register_page(smp_processor_id());
		glue_user_trace("Fetched slow regs");
		memcpy(&packet->slots[7], page->regs, len - 6);
		glue_user_trace("Unpacked slow regs");
	}

	glue_user_trace("Completed call in LCD");
}

void glue_user_call_client(uint64_t* data, size_t id)
{
    glue_user_panic("glue_user_call_client");
}

void* glue_user_map_to_shadow(const void* obj)
{
    glue_user_panic("glue_user_map_to_shadow");
    return 0;
}

const void* glue_user_map_from_shadow(const void* shadow)
{
    glue_user_panic("glue_user_map_from_shadow");
    return 0;
}

void glue_user_add_shadow(const void* ptr, void* shadow)
{
    glue_user_panic("glue_user_add_shadow");
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

// TODO
