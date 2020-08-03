#ifndef __MEI_CALLEE_H__
#define __MEI_CALLEE_H__

#include "../mei_glue_helper.h"

int _cond_resched_callee(struct fipc_message *_request);
int free_irq_callee(struct fipc_message *_request);
int mei_cancel_work_callee(struct fipc_message *_request);
int mei_deregister_callee(struct fipc_message *_request);
int mei_device_init_callee(struct fipc_message *_request);
int mei_hbm_pg_callee(struct fipc_message *_request);
int mei_hbm_pg_resume_callee(struct fipc_message *_request);
int mei_irq_compl_handler_callee(struct fipc_message *_request);
int mei_irq_read_handler_callee(struct fipc_message *_request);
int mei_irq_write_handler_callee(struct fipc_message *_request);
int mei_register_callee(struct fipc_message *_request);
int mei_restart_callee(struct fipc_message *_request);
int mei_start_callee(struct fipc_message *_request);
int mei_stop_callee(struct fipc_message *_request);
int mei_write_is_idle_callee(struct fipc_message *_request);
int pci_disable_device_callee(struct fipc_message *_request);
int pci_disable_msi_callee(struct fipc_message *_request);
int pci_enable_device_callee(struct fipc_message *_request);
int pci_enable_msi_range_callee(struct fipc_message *_request);
int pci_iomap_callee(struct fipc_message *_request);
int pci_iounmap_callee(struct fipc_message *_request);
int __pci_register_driver_callee(struct fipc_message *_request);
int pci_release_regions_callee(struct fipc_message *_request);
int pci_request_regions_callee(struct fipc_message *_request);
int pci_set_master_callee(struct fipc_message *_request);
int finish_wait_callee(struct fipc_message *_request);
int prepare_to_wait_event_callee(struct fipc_message *_request);
int queue_delayed_work_on_callee(struct fipc_message *_request);
int queue_work_on_callee(struct fipc_message *_request);
int request_threaded_irq_callee(struct fipc_message *_request);
int schedule_timeout_callee(struct fipc_message *_request);
int __wake_up_callee(struct fipc_message *_request);
int dispatch_sync_loop(struct fipc_message *message);
int dispatch_async_loop(struct fipc_message *message);
int glue_mei_init(void);
void glue_mei_exit(void);

#endif	/* __MEI_CALLEE_H__ */
