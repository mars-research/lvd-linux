#include <lcd_config/pre_hook.h>

#include "common.h"
#include <liblcd/liblcd.h>

#include <lcd_config/post_hook.h>

void glue_user_panic(const char* msg)
{
    panic(msg);
}

void glue_user_trace(const char* msg)
{
    LIBLCD_MSG(msg);
}

void glue_user_call_server(uint64_t* data, size_t id)
{
    LIBLCD_ERR("Didn't implement server calls\n");
}

void glue_user_call_client(uint64_t* data, size_t id)
{
    LIBLCD_ERR("Didn't implement client calls\n");
}

// TODO
