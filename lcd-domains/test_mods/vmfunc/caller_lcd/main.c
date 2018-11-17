/*
 * caller.c
 *
 * The "caller side" of the channel
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <linux/slab.h>
#include <liblcd/liblcd.h>
#include "../rpc.h"
#include <libfipc.h>
#include <thc_ipc.h>
#include <thc.h>
#include <awe_mapper.h>
#include <linux/module.h>

#include <lcd_config/post_hook.h>

struct thc_channel *tchnl;

static inline int send_and_get_response(
	struct fipc_ring_channel *chan,
	struct fipc_message *request,
	struct fipc_message **response,
	uint32_t msg_id)
{
	int ret;
	struct fipc_message *resp;

	/*
	 * Mark the request as sent
	 */
	ret = fipc_send_msg_end(chan, request);
	if (ret) {
		pr_err("failed to mark request as sent, ret = %d\n", ret);
		goto fail1;
	}
	/*
	 * Try to get the response
	 */
	ret = thc_ipc_recv_response(tchnl, msg_id, &resp);
	if (ret) {
		pr_err("failed to get a response, ret = %d\n", ret);
		goto fail2;
	}
	*response = resp;

	return 0;

fail2:
fail1:
	return ret;
}

static inline int finish_response_check_fn_type(struct fipc_ring_channel *chnl,
						struct fipc_message *response,
						enum fn_type expected_type)
{
	int ret;
	enum fn_type actual_type = get_fn_type(response);
	ret = fipc_recv_msg_end(chnl, response);
	if (ret) {
		pr_err("Error finishing receipt of response, ret = %d\n", ret);
		return ret;
	} else if (actual_type != expected_type) {
		pr_err("Unexpected fn type: actual = %d, expected = %d\n",
			actual_type, expected_type);
		return -EINVAL;
	} else {
		return 0;
	}
}

static inline int finish_response_check_fn_type_and_reg0(
	struct fipc_ring_channel *chnl,
	struct fipc_message *response,
	enum fn_type expected_type,
	unsigned long expected_reg0)
{
	int ret;
	enum fn_type actual_type = get_fn_type(response);
	unsigned long actual_reg0 = fipc_get_reg0(response);

	ret = fipc_recv_msg_end(chnl, response);
	if (ret) {
		pr_err("Error finishing receipt of response, ret = %d\n", ret);
		return ret;
	} else if (actual_type != expected_type) {
		pr_err("Unexpected fn type: actual = %d, expected = %d\n",
			actual_type, expected_type);
		return -EINVAL;
	} else if (actual_reg0 != expected_reg0) {
		pr_err("Unexpected return value (reg0): actual = 0x%lx, expected = 0x%lx\n",
			actual_reg0, expected_reg0);
		return -EINVAL;

	} else {
		return 0;
	}
}

static int noinline __used
async_add_nums(struct fipc_ring_channel *chan, unsigned long trans, 
	unsigned long res1)
{
	struct fipc_message *request;
	struct fipc_message *response;
	uint32_t msg_id;
	int ret;
	/*
	 * Set up request
	 */
	ret = test_fipc_blocking_send_start(chan, &request);
	if (ret) {
		pr_err("Error getting send message, ret = %d\n", ret);
		goto fail;
	}
	awe_mapper_create_id(&msg_id);

	thc_set_msg_type(request, msg_type_request);
	thc_set_msg_id(request, msg_id);
	set_fn_type(request, ADD_NUMS);
	fipc_set_reg0(request, trans);
	fipc_set_reg1(request, res1);
	/*
	 * Send request, and get response
	 */
	ret = send_and_get_response(chan, request, &response, msg_id);
	if (ret) {
		pr_err("Error getting response, ret = %d\n", ret);
		goto fail;
	}
	/*
	 * Maybe check message
	 */
	return finish_response_check_fn_type_and_reg0(
		chan,
		response, 
		ADD_NUMS,
		trans + res1);

fail:
	return ret;
}

static int caller(struct fipc_ring_channel *chan)
{
	unsigned long transaction_id = 0;
	int ret = 0;
	/*
	 * Add nums
	 */
	DO_FINISH({
			while(transaction_id < TRANSACTIONS)
			{
				ASYNC({
						transaction_id++;
						ret = async_add_nums(chan, 
								transaction_id, 1000);

						if (ret) {
							LIBLCD_ERR("error doing null invocation, ret = %d, exiting...\n",
								ret);
						}
					});
				
			}

		});

	LIBLCD_MSG("Caller async rpc complete");

	return ret;
}

static int setup_channel(cptr_t *buf1_cptr_out, cptr_t *buf2_cptr_out,
			struct fipc_ring_channel **chnl_out)
{
	int ret;
	cptr_t buf1_cptr, buf2_cptr;
	gva_t buf1_addr, buf2_addr;
	struct fipc_ring_channel *chnl;
	unsigned int pg_order = ASYNC_RPC_EXAMPLE_BUFFER_ORDER - PAGE_SHIFT;
	/*
	 * Allocate buffers
	 *
	 * (We use the lower level alloc. If we used the heap, even though
	 * we may alloc only 1 - 2 pages, we would end up sharing around
	 * 4 MB chunks of memory, since the heap uses coarse microkernel
	 * allocations.)
	 */
	ret = _lcd_alloc_pages(GFP_KERNEL, 
			pg_order,
			&buf1_cptr);
	if (ret) {
		LIBLCD_ERR("buf1 alloc");
		goto fail1;
	}
	ret = _lcd_alloc_pages(GFP_KERNEL, 
			pg_order,
			&buf2_cptr);
	if (ret) {
		LIBLCD_ERR("buf2 alloc");
		goto fail2;
	}

	/*
	 * Map them somewhere
	 */
	ret = lcd_map_virt(buf1_cptr, pg_order, &buf1_addr);
	if (ret) {
		LIBLCD_ERR("error mapping buf1");
		goto fail3;
	}
	ret = lcd_map_virt(buf2_cptr, pg_order, &buf2_addr);
	if (ret) {
		LIBLCD_ERR("error mapping buf2");
		goto fail4;
	}
	/*
	 * Prep buffers for rpc
	 */
	ret = fipc_prep_buffers(ASYNC_RPC_EXAMPLE_BUFFER_ORDER,
				(void *)gva_val(buf1_addr),
				(void *)gva_val(buf2_addr));
	if (ret) {
		LIBLCD_ERR("prep buffers");
		goto fail5;
	}
	/*
	 * Alloc and init channel header
	 */
	chnl = kmalloc(sizeof(*chnl), GFP_KERNEL);
	if (!chnl) {
		ret = -ENOMEM;
		LIBLCD_ERR("chnl alloc");
		goto fail6;
	}
	ret = fipc_ring_channel_init(chnl, ASYNC_RPC_EXAMPLE_BUFFER_ORDER,
				(void *)gva_val(buf1_addr),
				(void *)gva_val(buf2_addr));
	if (ret) {
		LIBLCD_ERR("ring chnl init");
		goto fail7;
	}

	*buf1_cptr_out = buf1_cptr;
	*buf2_cptr_out = buf2_cptr;
	*chnl_out = chnl;

	return 0;

fail7:
fail6:
fail5:
fail4:
fail3:
fail2:
fail1:
	/* we just fail; everything will be torn down when we die */
	return ret; 
}

static int grant_buffs_to_callee(cptr_t sync_chnl, cptr_t tx, cptr_t rx)
{
	lcd_set_cr0(rx); /* caller rx == callee tx */
	lcd_set_cr1(tx); /* caller tx == callee rx */

	return lcd_sync_send(sync_chnl);
}

static int __noreturn caller_main(void)
{
	int ret;
	cptr_t tx = {0}, rx = {0};
	struct fipc_ring_channel *chnl = NULL;
	/*
	 * Boot
	 */
	ret = lcd_enter();
	if (ret)
		goto out;
	/*
	 * Set up async ipc channel, and grant access to callee
	 *
	 * We expect the sync ipc channel to be in our boot info
	 */
	ret = setup_channel(&tx, &rx, &chnl);
	if (ret)
		goto out;
	ret = grant_buffs_to_callee(lcd_get_boot_info()->cptrs[0],
				tx, rx);
	if (ret)
		goto out;

	tchnl = kzalloc(sizeof(*tchnl), GFP_KERNEL);
	if (!tchnl) {
		ret = -ENOMEM;
		LIBLCD_ERR("alloc failed");
	}

	ret = thc_channel_init(tchnl, chnl);
	if (ret) {
		LIBLCD_ERR("error init'ing async channel group item");
	}

	/*
	 * Do ipc
	 */
	ret = caller(chnl);
	if (ret)
		goto out;
	/*
	 * Done; just exit (everything will be torn down when we die)
	 */
	goto out;
	       
out:
	lcd_exit(ret);
}

static int vmfunc_caller_init(void)
{
	int ret;

	LCD_MAIN({

			ret = caller_main();

		});

	return ret;
}

static void __exit vmfunc_caller_exit(void)
{
	return;
}

module_init(vmfunc_caller_init);
module_exit(vmfunc_caller_exit);
MODULE_LICENSE("GPL");
