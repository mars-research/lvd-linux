/*
 * bdi_caller.c - caller side of bdi interface
 */

#include <lcd_config/pre_hook.h>

#include <linux/backing-dev.h>

#include <liblcd/liblcd.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/dispatch_loop.h>
#include "../internal.h"

#include <lcd_config/post_hook.h>

static cptr_t bdi_chnl;
static struct glue_cspace *bdi_cspace;
//static struct dispatch_ctx *loop_ctx;

/* INIT/EXIT -------------------------------------------------- */

int glue_bdi_init(cptr_t _bdi_channel, struct dispatch_ctx *ctx)
{
	int ret;

	/* Store a reference to the dispatch loop context, so we
	 * can dynamically add channels to the loop later. */
//	loop_ctx = ctx;

	/* Store reference to bdi channel so we can invoke functions
	 * on it later. */
	bdi_chnl = _bdi_channel;

	/* Initialize data store. */
	ret = glue_cap_create(&bdi_cspace);
	if (ret) {
		LIBLCD_ERR("create");
		return ret;
	}

	return 0;
}

void glue_bdi_exit(void)
{
	/*
	 * Free bdi cspace
	 */
	glue_cap_destroy(bdi_cspace);
}

/* CALLER FUNCTIONS -------------------------------------------------- */



