#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../mei_caller.h"
#include <lcd_config/post_hook.h>

#define trace(msg)	LIBLCD_MSG("#msg")
int dispatch_async_loop(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
		case HOST_IS_READY:
			trace(HOST_IS_READY);
			return host_is_ready_callee(message);

		case HW_IS_READY:
			trace(HW_IS_READY);
			return hw_is_ready_callee(message);

		case HW_RESET:
			trace(HW_RESET);
			return hw_reset_callee(message);

		case HW_START:
			trace(HW_START);
			return hw_start_callee(message);

		case HW_CONFIG:
			trace(HW_CONFIG);
			return hw_config_callee(message);

		case FW_STATUS:
			trace(FW_STATUS);
			return fw_status_callee(message);

		case PG_STATE:
			trace(PG_STATE);
			return pg_state_callee(message);

		case PG_IN_TRANSITION:
			trace(PG_IN_TRANSITION);
			return pg_in_transition_callee(message);

		case PG_IS_ENABLED:
			trace(PG_IS_ENABLED);
			return pg_is_enabled_callee(message);

		case INTR_CLEAR:
			trace(INTR_CLEAR);
			return intr_clear_callee(message);

		case INTR_ENABLE:
			trace(INTR_ENABLE);
			return intr_enable_callee(message);

		case INTR_DISABLE:
			trace(INTR_DISABLE);
			return intr_disable_callee(message);

		case HBUF_FREE_SLOTS:
			trace(HBUF_FREE_SLOTS);
			return hbuf_free_slots_callee(message);

		case HBUF_IS_READY:
			trace(HBUF_IS_READY);
			return hbuf_is_ready_callee(message);

		case HBUF_MAX_LEN:
			trace(HBUF_MAX_LEN);
			return hbuf_max_len_callee(message);

		case WRITE:
			trace(WRITE);
			return write_callee(message);

		case RDBUF_FULL_SLOTS:
			trace(RDBUF_FULL_SLOTS);
			return rdbuf_full_slots_callee(message);

		case READ_HDR:
			trace(READ_HDR);
			return read_hdr_callee(message);

		case READ:
			trace(READ);
			return read_callee(message);

		case IRQ_HANDLER_T:
			trace(IRQ_HANDLER_T);
			return irq_handler_t_callee(message);

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;

	}
	return 0;

}

