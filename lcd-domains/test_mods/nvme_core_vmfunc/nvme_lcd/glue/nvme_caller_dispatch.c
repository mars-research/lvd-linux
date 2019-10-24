#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../nvme_caller.h"
#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("net got " #x " msg")

int __nvme_lcd_init(void);
void __nvme_lcd_exit(void);

extern unsigned long loops_per_jiffy;

int handle_rpc_calls(struct fipc_message *message)
{
	int fn_type;

	fn_type = async_msg_get_fn_type(message);

	switch (fn_type) {
		case QUEUE_RQ_FN:
			//trace(QUEUE_RQ_FN);
			return queue_rq_fn_callee(message);

		case MAP_QUEUE_FN:
			trace(MAP_QUEUE_FN);
			return map_queue_fn_callee(message);

		case INIT_HCTX_FN:
			trace(INIT_HCTX_FN);
			return init_hctx_fn_callee(message);

		case EXIT_HCTX_FN:
			trace(EXIT_HCTX_FN);
			return exit_hctx_fn_callee(message);

		case INIT_REQUEST_FN:
			/* trace(INIT_REQUEST_FN); */
			return init_request_fn_callee(message);

		case COMPLETE_FN:
			//trace(COMPLETE_FN);
			return complete_fn_callee(message);

		case TIMEOUT_FN:
			trace(TIMEOUT_FN);
			return timeout_fn_callee(message);

		case POLL_FN:
			trace(POLL_FN);
			return poll_fn_callee(message);

		case FOPS_OPEN_FN:
			trace(FOPS_OPEN_FN);
			return fops_open_callee(message);

		case FOPS_RELEASE_FN:
			trace(FOPS_RELEASE_FN);
			return fops_release_callee(message);

		case FOPS_UNLOCKED_IOCTL_FN:
			trace(FOPS_UNLOCKED_IOCTL_FN);
			return fops_unlocked_ioctl_callee(message);

		case BD_OPEN_FN:
			trace(BD_OPEN_FN);
			return bd_open_callee(message);

		case BD_RELEASE_FN:
			trace(BD_RELEASE_FN);
			return bd_release_callee(message);

		case BD_IOCTL_FN:
			trace(BD_IOCTL_FN);
			return bd_ioctl_callee(message);

		case BD_GETGEO_FN:
			trace(BD_GETGEO_FN);
			return bd_getgeo_callee(message);

		case BD_REVALIDATE_DISK_FN:
			trace(BD_REVALIDATE_DISK_FN);
			return bd_revalidate_disk_callee(message);

		case PROBE:
			trace(PROBE);
			return probe_callee(message);

		case REMOVE:
			trace(REMOVE);
			return remove_callee(message);

		case MODULE_INIT:
			trace(MODULE_INIT);
			loops_per_jiffy = fipc_get_reg0(message);
			return __nvme_lcd_init();

		case MODULE_EXIT:
			trace(MODULE_EXIT);
			__nvme_lcd_exit();
			break;
		case MSIX_IRQ_HANDLER:
			msix_vector_handler_callee(message);
			break;
            
		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;
	}
	return 0;
}
