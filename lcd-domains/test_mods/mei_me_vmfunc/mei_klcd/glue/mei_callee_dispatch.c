#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../mei_callee.h"
#include <lcd_config/post_hook.h>

#define trace(msg)	LIBLCD_MSG("#msg")
int dispatch_async_loop(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
		case _COND_RESCHED:
			trace(_COND_RESCHED);
			return _cond_resched_callee(message);

		case FREE_IRQ:
			trace(FREE_IRQ);
			return free_irq_callee(message);

		case MEI_CANCEL_WORK:
			trace(MEI_CANCEL_WORK);
			return mei_cancel_work_callee(message);

		case MEI_DEREGISTER:
			trace(MEI_DEREGISTER);
			return mei_deregister_callee(message);

		case MEI_DEVICE_INIT:
			trace(MEI_DEVICE_INIT);
			return mei_device_init_callee(message);

		case MEI_HBM_PG:
			trace(MEI_HBM_PG);
			return mei_hbm_pg_callee(message);

		case MEI_HBM_PG_RESUME:
			trace(MEI_HBM_PG_RESUME);
			return mei_hbm_pg_resume_callee(message);

		case MEI_IRQ_COMPL_HANDLER:
			trace(MEI_IRQ_COMPL_HANDLER);
			return mei_irq_compl_handler_callee(message);

		case MEI_IRQ_READ_HANDLER:
			trace(MEI_IRQ_READ_HANDLER);
			return mei_irq_read_handler_callee(message);

		case MEI_IRQ_WRITE_HANDLER:
			trace(MEI_IRQ_WRITE_HANDLER);
			return mei_irq_write_handler_callee(message);

		case MEI_REGISTER:
			trace(MEI_REGISTER);
			return mei_register_callee(message);

		case MEI_RESTART:
			trace(MEI_RESTART);
			return mei_restart_callee(message);

		case MEI_START:
			trace(MEI_START);
			return mei_start_callee(message);

		case MEI_STOP:
			trace(MEI_STOP);
			return mei_stop_callee(message);

		case MEI_WRITE_IS_IDLE:
			trace(MEI_WRITE_IS_IDLE);
			return mei_write_is_idle_callee(message);

		case PCI_DISABLE_DEVICE:
			trace(PCI_DISABLE_DEVICE);
			return pci_disable_device_callee(message);

		case PCI_DISABLE_MSI:
			trace(PCI_DISABLE_MSI);
			return pci_disable_msi_callee(message);

		case PCI_ENABLE_DEVICE:
			trace(PCI_ENABLE_DEVICE);
			return pci_enable_device_callee(message);

		case PCI_ENABLE_MSI_RANGE:
			trace(PCI_ENABLE_MSI_RANGE);
			return pci_enable_msi_range_callee(message);

		case PCI_IOMAP:
			trace(PCI_IOMAP);
			return pci_iomap_callee(message);

		case PCI_IOUNMAP:
			trace(PCI_IOUNMAP);
			return pci_iounmap_callee(message);

		case __PCI_REGISTER_DRIVER:
			trace(__PCI_REGISTER_DRIVER);
			return __pci_register_driver_callee(message);

		case PCI_RELEASE_REGIONS:
			trace(PCI_RELEASE_REGIONS);
			return pci_release_regions_callee(message);

		case PCI_REQUEST_REGIONS:
			trace(PCI_REQUEST_REGIONS);
			return pci_request_regions_callee(message);

		case PCI_SET_MASTER:
			trace(PCI_SET_MASTER);
			return pci_set_master_callee(message);

		case FINISH_WAIT:
			trace(FINISH_WAIT);
			return finish_wait_callee(message);

		case PREPARE_TO_WAIT_EVENT:
			trace(PREPARE_TO_WAIT_EVENT);
			return prepare_to_wait_event_callee(message);

		case QUEUE_DELAYED_WORK_ON:
			trace(QUEUE_DELAYED_WORK_ON);
			return queue_delayed_work_on_callee(message);

		case QUEUE_WORK_ON:
			trace(QUEUE_WORK_ON);
			return queue_work_on_callee(message);

		case REQUEST_THREADED_IRQ:
			trace(REQUEST_THREADED_IRQ);
			return request_threaded_irq_callee(message);

		case SCHEDULE_TIMEOUT:
			trace(SCHEDULE_TIMEOUT);
			return schedule_timeout_callee(message);

		case __WAKE_UP:
			trace(__WAKE_UP);
			return __wake_up_callee(message);

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;

	}
	return 0;

}

