/*
 * dispatch.c
 *
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../nullnet_callee.h"

#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("net got " #x " msg")

struct glue_cspace *cspace;
