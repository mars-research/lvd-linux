/*
 * bdi_caller.c - caller side of bdi interface
 */

#include <linux/backing-dev.h>
#include <lcd-domains/liblcd.h>
#include "../internal.h"

static cptr_t bdi_chnl;
static struct dstore *bdi_dstore;
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
	ret = lcd_dstore_init_dstore(&bdi_dstore);
	if (ret) {
		LIBLCD_ERR("dstore init");
		return ret;
	}

	return 0;
}

void glue_bdi_exit(void)
{
	/*
	 * Free bdi data store
	 */
	lcd_dstore_destroy(bdi_dstore);
}

/* CALLER FUNCTIONS -------------------------------------------------- */


int bdi_init(struct backing_dev_info *bdi)
{
	struct backing_dev_info_container *bdi_container;
	int ret;
	/*
	 * Get container
	 */
	bdi_container = container_of(bdi, 
				struct backing_dev_info_container,
				backing_dev_info);
	/*
	 * INSERT INTO DATA STORE ------------------------------
	 *
	 */
	ret = lcd_dstore_insert(bdi_dstore, 
				bdi_container,
				STRUCT_BACKING_DEV_INFO_TAG, 
				&bdi_container->my_ref);
	if (ret) {
		LIBLCD_ERR("lcd_dstore_insert");
		lcd_exit(ret); /* abort */
	}
	/*
	 * IPC MARSHALING --------------------------------------------------
	 *
	 */
	lcd_set_r1(dptr_val(bdi_container->my_ref));
	lcd_set_r2(bdi_container->backing_dev_info.ra_pages);
	lcd_set_r3(bdi_container->backing_dev_info.capabilities);
	/*
	 * IPC CALL ----------------------------------------
	 */

	lcd_set_r0(BDI_INIT);
	ret = lcd_call(bdi_chnl);
	if (ret) {
		LIBLCD_ERR("lcd_call");
		lcd_exit(ret);
	}

	/* IPC UNMARSHALING ---------------------------------------- */

	/*
	 * We expect a remote ref coming back
	 */
	bdi_container->bdi_ref = __dptr(lcd_r1());

	/* Clear capability register */
	lcd_set_cr1(LCD_CPTR_NULL);

	/*
	 * Pass back return value
	 */
	return lcd_r0();
}

void bdi_destroy(struct backing_dev_info *bdi)
{
	int ret;
	struct backing_dev_info_container *bdi_container;

	bdi_container = container_of(bdi,
				struct backing_dev_info_container,
				backing_dev_info);

	/* IPC MARSHALING ---------------------------------------- */

	/*
	 * Pass remote ref to bdi's copy
	 */
	lcd_set_r1(dptr_val(bdi_container->bdi_ref));

	/* IPC CALL ---------------------------------------- */

	lcd_set_r0(BDI_DESTROY);
	ret = lcd_call(bdi_chnl);
	if (ret) {
		LIBLCD_ERR("lcd_call");
		lcd_exit(ret);
	}

	/* POST-IPC ---------------------------------------- */

	/*
	 * Remove bdi from data store
	 */
	lcd_dstore_delete(bdi_dstore, bdi_container->my_ref);
	/*
	 * (no return value)
	 */
}

