/*
 * callee.c
 *
 * Code for the "callee side" of the channel
 *
 * Copyright: University of Utah
 */

#include <linux/kernel.h>
#include <libfipc.h>
#include "rpc.h"
#include "../test_helpers.h"
#include <thc_ipc.h>

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
    
    THC_MSG_TYPE(response) = msg_type_response;
    THC_MSG_ID(response)   = THC_MSG_ID(recvd_msg);
	set_fn_type(response, type);
	response->regs[0] = val;

	ret = fipc_send_msg_end(chnl, response);
	if (ret) {
		pr_err("Error marking message as sent");
		return ret;
	}

	return 0;
}

int callee(void *_callee_channel_header)
{
	int ret = 0;
	unsigned long temp_res;
        struct fipc_ring_channel *chan = _callee_channel_header;
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

out:
	return ret;
}
