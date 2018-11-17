/*
 * callee.c
 *
 * Code for the "callee side" of the channel
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

/*
 * We use "noinline" because we want the function call to actually
 * happen.
 */

static unsigned long noinline 
null_invocation(void)
{
	return 9;
}

static unsigned long noinline
add_constant(unsigned long trans)
{
	return trans + 50;
}

static unsigned long noinline 
add_nums(unsigned long trans, unsigned long res1)
{
	return trans+res1;
}

static unsigned long noinline 
add_3_nums(unsigned long trans, unsigned long res1, unsigned long res2)
{
	return add_nums(trans, res1) + res2;
}

static unsigned long noinline
add_4_nums(unsigned long trans, unsigned long res1, unsigned long res2, 
	unsigned long res3)
{

	return add_nums(trans, res1) + add_nums(res2, res3);
}

static unsigned long noinline
add_5_nums(unsigned long trans, unsigned long res1, unsigned long res2, 
	unsigned long res3, unsigned long res4)
{
	return add_4_nums(trans,res1,res2,res3) + res4;
}

static unsigned long noinline
add_6_nums(unsigned long trans, unsigned long res1, unsigned long res2, 
	unsigned long res3, unsigned long res4, unsigned long res5)
{
	return add_3_nums(trans,res1,res2) + add_3_nums(res3,res4,res5);
}

static inline int send_response(struct fipc_ring_channel *chnl,
				struct fipc_message *recvd_msg,
				unsigned long val, enum fn_type type)
{
	int ret;
	struct fipc_message *response;
	/*
	 * Mark recvd msg slot as available
	 */
	ret = fipc_recv_msg_end(chnl, recvd_msg);
	if (ret) {
		pr_err("Error marking msg as recvd");
		return ret;
	}
	/*
	 * Response
	 */
	ret = test_fipc_blocking_send_start(chnl, &response);
	if (ret) {
		pr_err("Error getting send slot");
		return ret;
	}
    
	thc_set_msg_type(response, msg_type_response);
	thc_set_msg_id(response, thc_get_msg_id(recvd_msg));
	set_fn_type(response, type);
	response->regs[0] = val;

	ret = fipc_send_msg_end(chnl, response);
	if (ret) {
		pr_err("Error marking message as sent");
		return ret;
	}

	return 0;
}

int callee(struct fipc_ring_channel *chan)
{
	int ret = 0;
	unsigned long temp_res;
	struct fipc_message *recvd_msg;
	unsigned long transaction_id = 0;
	enum fn_type type;

	for (transaction_id = 0; 
	     transaction_id < TRANSACTIONS; 
	     transaction_id++) {
		/*
		 * Try to receive a message
		 */
		ret = test_fipc_blocking_recv_start(chan, &recvd_msg);
		if (ret) {
			pr_err("Error receiving message, ret = %d, exiting...", ret);
			goto out;
		}
		/*
		 * Dispatch on message type
		 */
		type = get_fn_type(recvd_msg);
		switch(type) {
		case NULL_INVOCATION:
			temp_res = null_invocation();
			break;
		case ADD_CONSTANT:
			temp_res = add_constant(fipc_get_reg0(recvd_msg));
			break;
		case ADD_NUMS:
			temp_res = add_nums(fipc_get_reg0(recvd_msg), 
					fipc_get_reg1(recvd_msg));
			break;
		case ADD_3_NUMS:
			temp_res = add_3_nums(fipc_get_reg0(recvd_msg), 
					fipc_get_reg1(recvd_msg), 
					fipc_get_reg2(recvd_msg));
			break;
		case ADD_4_NUMS:
			temp_res = add_4_nums(fipc_get_reg0(recvd_msg), 
					fipc_get_reg1(recvd_msg), 
					fipc_get_reg2(recvd_msg),
					fipc_get_reg3(recvd_msg));
			break;
		case ADD_5_NUMS:
			temp_res = add_5_nums(fipc_get_reg0(recvd_msg), 
					fipc_get_reg1(recvd_msg), 
					fipc_get_reg2(recvd_msg),
					fipc_get_reg3(recvd_msg), 
					fipc_get_reg4(recvd_msg));
			break;
		case ADD_6_NUMS:
			temp_res = add_6_nums(fipc_get_reg0(recvd_msg), 
					fipc_get_reg1(recvd_msg), 
					fipc_get_reg2(recvd_msg),
					fipc_get_reg3(recvd_msg), 
					fipc_get_reg4(recvd_msg),
					fipc_get_reg5(recvd_msg));
			break;
		default:
			pr_err("Bad function type %d, exiting...\n", type);
			ret = -EINVAL;
			goto out;
		}
		/*
		 * Send response back
		 */
		ret = send_response(chan, recvd_msg, temp_res, type);
		if (ret) {
			pr_err("Error sending response back, ret = %d, exiting...", ret);
			goto out;
		}
	}

	LIBLCD_MSG("Callee is done");

out:
	return ret;
}

static int setup_channel(cptr_t tx_cptr, cptr_t rx_cptr,
			struct fipc_ring_channel **chnl_out)
{
	int ret;
	gva_t tx_addr, rx_addr;
	struct fipc_ring_channel *chnl;
	unsigned int pg_order = ASYNC_RPC_EXAMPLE_BUFFER_ORDER - PAGE_SHIFT;
	/*
	 * Map buffers somewhere
	 */
	ret = lcd_map_virt(tx_cptr, pg_order, &tx_addr);
	if (ret) {
		LIBLCD_ERR("error mapping tx");
		goto fail1;
	}
	ret = lcd_map_virt(rx_cptr, pg_order, &rx_addr);
	if (ret) {
		LIBLCD_ERR("error mapping rx");
		goto fail2;
	}
	/*
	 * Alloc and init channel header (buffers already prep'd by caller)
	 */
	chnl = kmalloc(sizeof(*chnl), GFP_KERNEL);
	if (!chnl) {
		LIBLCD_ERR("chnl alloc");
		ret = -ENOMEM;
		goto fail3;
	}
	ret = fipc_ring_channel_init(chnl, ASYNC_RPC_EXAMPLE_BUFFER_ORDER,
				(void *)gva_val(tx_addr),
				(void *)gva_val(rx_addr));
	if (ret) {

		LIBLCD_ERR("ring chnl init");
		goto fail4;
	}

	*chnl_out = chnl;

	return 0;

fail4:
fail3:
fail2:
fail1:
	/* we just fail; everything will be torn down when we die */
	return ret; 
}

static int get_buffers(cptr_t sync_chnl, cptr_t *tx, cptr_t *rx)
{
	int ret;
	/*
	 * Alloc cptr's for buffers
	 */
	ret = lcd_cptr_alloc(tx);
	if (ret) {
		LIBLCD_ERR("cptr alloc 1");
		goto fail1;
	}
	ret = lcd_cptr_alloc(rx);
	if (ret) {
		LIBLCD_ERR("cptr alloc 2");
		goto fail2;
	}
	/*
	 * Prepare to receive capabilities
	 */
	lcd_set_cr0(*tx); /* caller sends our tx in cr0 */
	lcd_set_cr1(*rx); /* caller sends our rx in cr1 */

	ret = lcd_sync_recv(sync_chnl);
	if (ret) {
		LIBLCD_ERR("sync recv failed");
		goto fail3;
	}

	return 0;

fail3:
fail2:
fail1:
	return ret; /* we just tear down anyway */
}

static int __noreturn callee_main(void)
{
	int ret;
	cptr_t tx, rx;
	struct fipc_ring_channel *chnl = NULL;
	/*
	 * Boot LCD
	 */
	ret = lcd_enter();
	if (ret)
		goto out;
	/*
	 * Get the ipc buffers from the caller, and set up our channel header
	 */
	ret = get_buffers(lcd_get_boot_info()->cptrs[0],
			&tx, &rx);
	if (ret)
		goto out;
	ret = setup_channel(tx, rx, &chnl);
	if (ret)
		goto out;
	/*
	 * Do async ipc
	 */
	ret = callee(chnl);
	if (ret)
		goto out;
	/*
	 * Done (everything is just torn down when we die; we could write
	 * tear down code, but there's no point, and just adds irrelevant
	 * complexity)
	 */
	goto out;

out:
	lcd_exit(ret);
}

static int vmfunc_callee_init(void)
{
	int ret = 0;

	LCD_MAIN({

			ret = callee_main();

		});

	return ret;
}

static void __exit vmfunc_callee_exit(void)
{
	return;
}

module_init(vmfunc_callee_init);
module_exit(vmfunc_callee_exit);
MODULE_LICENSE("GPL");
