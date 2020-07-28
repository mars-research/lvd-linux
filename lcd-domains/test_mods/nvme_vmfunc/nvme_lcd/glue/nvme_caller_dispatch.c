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
        
		case NVME_PCI_REG_READ32:
			trace(NVME_PCI_REG_READ32);
			return nvme_pci_reg_read32_callee(message);

		case NVME_PCI_REG_WRITE32:
			trace(NVME_PCI_REG_WRITE32);
			return nvme_pci_reg_write32_callee(message);

		case NVME_PCI_REG_READ64:
			trace(NVME_PCI_REG_READ64);
			return nvme_pci_reg_read64_callee(message);

		case NVME_PCI_RESET_CTRL:
			trace(NVME_PCI_RESET_CTRL);
			return nvme_pci_reset_ctrl_callee(message);

		case NVME_PCI_FREE_CTRL:
			trace(NVME_PCI_FREE_CTRL);
			return nvme_pci_free_ctrl_callee(message);

		case NVME_PCI_POST_SCAN:
			trace(NVME_PCI_POST_SCAN);
			return nvme_pci_post_scan_callee(message);

		case NVME_PCI_SUBMIT_ASYNC_EVENT:
			trace(NVME_PCI_SUBMIT_ASYNC_EVENT);
			return nvme_pci_submit_async_event_callee(message);

		case QUEUE_RQ_FN:
			trace(QUEUE_RQ_FN);
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
			trace(INIT_REQUEST_FN);
			return init_request_fn_callee(message);

		case SOFTIRQ_DONE_FN:
			trace(SOFTIRQ_DONE_FN);
			return softirq_done_fn_callee(message);

		case POLL_FN:
			trace(POLL_FN);
			return poll_fn_callee(message);

	        // case OPEN:
			// trace(OPEN);
			// return open_callee(message);

		// case RELEASE:
			// trace(RELEASE);
			// return release_callee(message);
            
		case PROBE:
			trace(PROBE);
			return probe_callee(message);

		// case REMOVE:
			// trace(REMOVE);
			// return remove_callee(message);


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
