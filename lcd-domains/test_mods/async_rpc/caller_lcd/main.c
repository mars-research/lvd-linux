/*
 * caller.c
 *
 * The "caller side" of the channel
 *
 * Copyright: University of Utah
 */

#include <linux/random.h>
#include "rpc.h"
#include <thc_ipc.h>
#include <thc.h>
#include <thcinternal.h>
#include <awe_mapper.h>
#include "../test_helpers.h"

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
    ret = thc_ipc_recv(chan, msg_id, &resp);
	if (ret) {
		pr_err("failed to get a response, ret = %d\n", ret);
		goto fail2;
	}
	*response = resp;
    printk(KERN_ERR "got result\n");

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
    msg_id                = awe_mapper_create_id();

    THC_MSG_TYPE(request) = msg_type_request;
    THC_MSG_ID(request)   = msg_id;
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

int caller(void *_caller_channel_header)
{
    struct fipc_ring_channel *chan = _caller_channel_header;
	unsigned long transaction_id = 0;
	int ret = 0;
    volatile void ** frame = (volatile void**)__builtin_frame_address(0);
    volatile void *ret_addr = *(frame + 1);
    *(frame + 1) = NULL;

    thc_init();
	/*
	 * Add nums
	 */
    DO_FINISH({
        while(transaction_id < TRANSACTIONS)
        {
            ASYNC({
                transaction_id++;
                ret = async_add_nums(chan, transaction_id, 1000);

                if (ret) {
                    pr_err("error doing null invocation, ret = %d, exiting...\n",
                        ret);
                }
            });

        }
    });
	pr_err("Complete\n");
    thc_done();

    *(frame + 1) = ret_addr;
	return ret;
}



