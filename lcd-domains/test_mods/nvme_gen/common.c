#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel__lvd_alloc_workqueue__ret_workqueue_struct__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_alloc_workqueue_call_ctx const* ctx,
	struct workqueue_struct const* ptr)
{

}

void callee_unmarshal_kernel__lvd_alloc_workqueue__ret_workqueue_struct__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_alloc_workqueue_call_ctx const* ctx,
	struct workqueue_struct* ptr)
{

	{
	}
}

void callee_marshal_kernel__lvd_alloc_workqueue__ret_workqueue_struct__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_alloc_workqueue_call_ctx const* ctx,
	struct workqueue_struct const* ptr)
{

}

void caller_unmarshal_kernel__lvd_alloc_workqueue__ret_workqueue_struct__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_alloc_workqueue_call_ctx const* ctx,
	struct workqueue_struct* ptr)
{

	{
	}
}

void caller_marshal_kernel___global_pci_driver__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	struct pci_device_id* const* id_table_ptr = &ptr->id_table;
	struct pci_error_handlers* const* err_handler_ptr = &ptr->err_handler;
	fptr_pci_driver_probe const* probe_ptr = &ptr->probe;
	fptr_pci_driver_remove const* remove_ptr = &ptr->remove;
	fptr_pci_driver_shutdown const* shutdown_ptr = &ptr->shutdown;
	fptr_pci_driver_sriov_configure const* sriov_configure_ptr = &ptr->sriov_configure;

	{
		__maybe_unused const void* __adjusted = *name_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*name_ptr) {
			size_t i, len;
			char const* array = *name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		__maybe_unused const void* __adjusted = *id_table_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*id_table_ptr) {
			size_t i, len;
			struct pci_device_id sentinel = { 0 };
			struct pci_device_id const* array = *id_table_ptr;
			for (len = 0; memcmp(&array[len], &sentinel, sizeof(array[0])); ++len) ;
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				struct pci_device_id const* element = &array[i];
				caller_marshal_kernel__pci_device_id__in(__pos, __msg, __ext, element);
			}

		}

	}

	{
		__maybe_unused const void* __adjusted = *err_handler_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*err_handler_ptr) {
			caller_marshal_kernel___global_pci_error_handlers__in(__pos, __msg, __ext, *err_handler_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *probe_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *remove_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *shutdown_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *sriov_configure_ptr);
	}

}

void callee_unmarshal_kernel___global_pci_driver__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver* ptr)
{
	char const** name_ptr = &ptr->name;
	struct pci_device_id** id_table_ptr = &ptr->id_table;
	struct pci_error_handlers** err_handler_ptr = &ptr->err_handler;
	fptr_pci_driver_probe* probe_ptr = &ptr->probe;
	fptr_pci_driver_remove* remove_ptr = &ptr->remove;
	fptr_pci_driver_shutdown* shutdown_ptr = &ptr->shutdown;
	fptr_pci_driver_sriov_configure* sriov_configure_ptr = &ptr->sriov_configure;

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*name_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*name_ptr) {
			char* writable = (char*)*name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(__pos, __msg, __ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	{
		size_t __size = sizeof(struct pci_device_id) * glue_peek(__pos, __msg, __ext);
		*id_table_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct pci_device_id*, (__size), (DEFAULT_GFP_FLAGS));
		if (*id_table_ptr) {
			size_t i, len;
			struct pci_device_id* array = *id_table_ptr;
			len = glue_unpack(__pos, __msg, __ext, size_t) - 1;
			memset(&array[len], 0x0, sizeof(array[len]));
			for (i = 0; i < len; ++i) {
				struct pci_device_id* element = &array[i];
				callee_unmarshal_kernel__pci_device_id__in(__pos, __msg, __ext, element);
			}

		}

	}

	{
		size_t __size = sizeof(struct pci_error_handlers);
		*err_handler_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct pci_error_handlers*, (__size), (DEFAULT_GFP_FLAGS));
		if (*err_handler_ptr) {
			callee_unmarshal_kernel___global_pci_error_handlers__in(__pos, __msg, __ext, *err_handler_ptr);
		}

	}

	{
		*probe_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, pci_driver_probe);
	}

	{
		*remove_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, pci_driver_remove);
	}

	{
		*shutdown_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, pci_driver_shutdown);
	}

	{
		*sriov_configure_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, pci_driver_sriov_configure);
	}

	{
	}
}

void callee_marshal_kernel___global_pci_driver__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	struct pci_device_id* const* id_table_ptr = &ptr->id_table;
	struct pci_error_handlers* const* err_handler_ptr = &ptr->err_handler;

	{
		(void)name_ptr;
	}

	{
		(void)id_table_ptr;
	}

	{
		if (*err_handler_ptr) {
			callee_marshal_kernel___global_pci_error_handlers__in(__pos, __msg, __ext, *err_handler_ptr);
		}

	}

}

void caller_unmarshal_kernel___global_pci_driver__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver* ptr)
{
	char const** name_ptr = &ptr->name;
	struct pci_device_id** id_table_ptr = &ptr->id_table;
	struct pci_error_handlers** err_handler_ptr = &ptr->err_handler;

	{
		(void)name_ptr;
	}

	{
		(void)id_table_ptr;
	}

	{
		if (*err_handler_ptr) {
			caller_unmarshal_kernel___global_pci_error_handlers__in(__pos, __msg, __ext, *err_handler_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__pci_device_id__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_device_id const* ptr)
{
	unsigned int const* vendor_ptr = &ptr->vendor;
	unsigned int const* device_ptr = &ptr->device;
	unsigned int const* subvendor_ptr = &ptr->subvendor;
	unsigned int const* subdevice_ptr = &ptr->subdevice;
	unsigned int const* class_ptr = &ptr->class;
	unsigned int const* class_mask_ptr = &ptr->class_mask;
	unsigned long long const* driver_data_ptr = &ptr->driver_data;

	{
		glue_pack(__pos, __msg, __ext, *vendor_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *device_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *subvendor_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *subdevice_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *class_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *class_mask_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *driver_data_ptr);
	}

}

void callee_unmarshal_kernel__pci_device_id__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_device_id* ptr)
{
	unsigned int* vendor_ptr = &ptr->vendor;
	unsigned int* device_ptr = &ptr->device;
	unsigned int* subvendor_ptr = &ptr->subvendor;
	unsigned int* subdevice_ptr = &ptr->subdevice;
	unsigned int* class_ptr = &ptr->class;
	unsigned int* class_mask_ptr = &ptr->class_mask;
	unsigned long long* driver_data_ptr = &ptr->driver_data;

	{
		*vendor_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*device_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*subvendor_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*subdevice_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*class_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*class_mask_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*driver_data_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void callee_marshal_kernel__pci_device_id__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_device_id const* ptr)
{

}

void caller_unmarshal_kernel__pci_device_id__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_device_id* ptr)
{

	{
	}
}

void caller_marshal_kernel___global_pci_error_handlers__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers const* ptr)
{
	fptr_pci_error_handlers_error_detected const* error_detected_ptr = &ptr->error_detected;
	fptr_pci_error_handlers_reset_notify const* reset_notify_ptr = &ptr->reset_notify;
	fptr_pci_error_handlers_resume const* resume_ptr = &ptr->resume;
	fptr_pci_error_handlers_slot_reset const* slot_reset_ptr = &ptr->slot_reset;

	{
		glue_pack(__pos, __msg, __ext, *error_detected_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *reset_notify_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *resume_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *slot_reset_ptr);
	}

}

void callee_unmarshal_kernel___global_pci_error_handlers__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers* ptr)
{
	fptr_pci_error_handlers_error_detected* error_detected_ptr = &ptr->error_detected;
	fptr_pci_error_handlers_reset_notify* reset_notify_ptr = &ptr->reset_notify;
	fptr_pci_error_handlers_resume* resume_ptr = &ptr->resume;
	fptr_pci_error_handlers_slot_reset* slot_reset_ptr = &ptr->slot_reset;

	{
		*error_detected_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, pci_error_handlers_error_detected);
	}

	{
		*reset_notify_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, pci_error_handlers_reset_notify);
	}

	{
		*resume_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, pci_error_handlers_resume);
	}

	{
		*slot_reset_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, pci_error_handlers_slot_reset);
	}

	{
	}
}

void callee_marshal_kernel___global_pci_error_handlers__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers const* ptr)
{

}

void caller_unmarshal_kernel___global_pci_error_handlers__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers* ptr)
{

	{
	}
}

void caller_marshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct module const* ptr)
{

}

void callee_unmarshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct module* ptr)
{

	{
	}
}

void callee_marshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct module const* ptr)
{

}

void caller_unmarshal_kernel____pci_register_driver__owner__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __pci_register_driver_call_ctx const* ctx,
	struct module* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_cleanup_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_cleanup_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_cleanup_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	unsigned long const* queue_flags_ptr = &ptr->queue_flags;

	{
		glue_pack(__pos, __msg, __ext, *queue_flags_ptr);
	}

}

void caller_unmarshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_cleanup_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{
	unsigned long* queue_flags_ptr = &ptr->queue_flags;

	{
		*queue_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void caller_marshal_kernel__done__rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct done_call_ctx const* ctx,
	struct request const* ptr)
{

}

void callee_unmarshal_kernel__done__rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct done_call_ctx const* ctx,
	struct request* ptr)
{

	{
	}
}

void callee_marshal_kernel__done__rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct done_call_ctx const* ctx,
	struct request const* ptr)
{

}

void caller_unmarshal_kernel__done__rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct done_call_ctx const* ctx,
	struct request* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_execute_rq_nowait__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	unsigned long const* queue_flags_ptr = &ptr->queue_flags;

	{
		glue_pack(__pos, __msg, __ext, *queue_flags_ptr);
	}

}

void callee_unmarshal_kernel__blk_execute_rq_nowait__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct request_queue* ptr)
{
	unsigned long* queue_flags_ptr = &ptr->queue_flags;

	{
		*queue_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void callee_marshal_kernel__blk_execute_rq_nowait__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__blk_execute_rq_nowait__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_execute_rq_nowait__bd_disk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct gendisk const* ptr)
{

}

void callee_unmarshal_kernel__blk_execute_rq_nowait__bd_disk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct gendisk* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_execute_rq_nowait__bd_disk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct gendisk const* ptr)
{

}

void caller_unmarshal_kernel__blk_execute_rq_nowait__bd_disk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct gendisk* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_execute_rq_nowait__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct request const* ptr)
{
	struct request_queue* const* q_ptr = &ptr->q;
	unsigned int const* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long const* cmd_flags_ptr = &ptr->cmd_flags;

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_execute_rq_nowait__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_type_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_flags_ptr);
	}

}

void callee_unmarshal_kernel__blk_execute_rq_nowait__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct request* ptr)
{
	struct request_queue** q_ptr = &ptr->q;
	unsigned int* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long* cmd_flags_ptr = &ptr->cmd_flags;

	{
		*q_ptr = glue_unpack(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_execute_rq_nowait__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*cmd_type_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*cmd_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void callee_marshal_kernel__blk_execute_rq_nowait__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct request const* ptr)
{
	struct request_queue* const* q_ptr = &ptr->q;
	unsigned int const* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long const* cmd_flags_ptr = &ptr->cmd_flags;
	int const* errors_ptr = &ptr->errors;

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			callee_marshal_kernel__blk_execute_rq_nowait__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_type_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *errors_ptr);
	}

}

void caller_unmarshal_kernel__blk_execute_rq_nowait__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct request* ptr)
{
	struct request_queue** q_ptr = &ptr->q;
	unsigned int* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long* cmd_flags_ptr = &ptr->cmd_flags;
	int* errors_ptr = &ptr->errors;

	{
		*q_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_execute_rq_nowait__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*cmd_type_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*cmd_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*errors_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void caller_marshal_kernel__blk_execute_rq_nowait__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__blk_execute_rq_nowait__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_execute_rq_nowait__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__blk_execute_rq_nowait__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_execute_rq_nowait_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_get_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_get_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__blk_get_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_get_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_get_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_get_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__blk_get_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_get_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_alloc_tag_set__set__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{
	struct blk_mq_ops* const* ops_ptr = &ptr->ops;
	unsigned int const* nr_hw_queues_ptr = &ptr->nr_hw_queues;
	unsigned int const* queue_depth_ptr = &ptr->queue_depth;
	unsigned int const* cmd_size_ptr = &ptr->cmd_size;
	int const* numa_node_ptr = &ptr->numa_node;
	void* const* driver_data_ptr = &ptr->driver_data;
	struct blk_mq_tags** const* tags_ptr = &ptr->tags;

	{
		__maybe_unused const void* __adjusted = *ops_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ops_ptr) {
			caller_marshal_kernel___global_blk_mq_ops__in(__pos, __msg, __ext, *ops_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *nr_hw_queues_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_depth_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_size_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *numa_node_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *driver_data_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*driver_data_ptr) {
		}

	}

	{
		(void)tags_ptr;
	}

}

void callee_unmarshal_kernel__blk_mq_alloc_tag_set__set__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{
	struct blk_mq_ops** ops_ptr = &ptr->ops;
	unsigned int* nr_hw_queues_ptr = &ptr->nr_hw_queues;
	unsigned int* queue_depth_ptr = &ptr->queue_depth;
	unsigned int* cmd_size_ptr = &ptr->cmd_size;
	int* numa_node_ptr = &ptr->numa_node;
	void** driver_data_ptr = &ptr->driver_data;
	struct blk_mq_tags*** tags_ptr = &ptr->tags;

	{
		size_t __size = sizeof(struct blk_mq_ops);
		*ops_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct blk_mq_ops*, (__size), (DEFAULT_GFP_FLAGS));
		if (*ops_ptr) {
			callee_unmarshal_kernel___global_blk_mq_ops__in(__pos, __msg, __ext, *ops_ptr);
		}

	}

	{
		*nr_hw_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*queue_depth_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*cmd_size_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*numa_node_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		size_t __size = sizeof(void*);
		*driver_data_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, void*, (__size), (DEFAULT_GFP_FLAGS));
		if (*driver_data_ptr) {
		}

	}

	{
		(void)tags_ptr;
	}

	{
	}
}

void callee_marshal_kernel__blk_mq_alloc_tag_set__set__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{
	struct blk_mq_ops* const* ops_ptr = &ptr->ops;
	unsigned int const* nr_hw_queues_ptr = &ptr->nr_hw_queues;
	unsigned int const* queue_depth_ptr = &ptr->queue_depth;
	void* const* driver_data_ptr = &ptr->driver_data;
	struct blk_mq_tags** const* tags_ptr = &ptr->tags;

	{
		if (*ops_ptr) {
			callee_marshal_kernel___global_blk_mq_ops__in(__pos, __msg, __ext, *ops_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *nr_hw_queues_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_depth_ptr);
	}

	{
		(void)driver_data_ptr;
	}

	{
		__maybe_unused const void* __adjusted = *tags_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*tags_ptr) {
			size_t i, len = (ptr->nr_hw_queues);
			struct blk_mq_tags** array = *tags_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct blk_mq_tags** element = &array[i];
				__maybe_unused const void* __adjusted = *element;
				glue_pack(__pos, __msg, __ext, __adjusted);
				if (*element) {
					size_t i, len = 1;
					struct blk_mq_tags const* array = *element;
					glue_pack(__pos, __msg, __ext, len);
					// Warning: see David if this breaks
					glue_user_trace("Warning: see David if this breaks");
					for (i = 0; i < len; ++i) {
						struct blk_mq_tags const* element = &array[i];
						callee_marshal_kernel__blk_mq_alloc_tag_set__blk_mq_tag_set_tags__out(__pos, __msg, __ext, ctx, element);
					}

				}

			}

		}

	}

}

void caller_unmarshal_kernel__blk_mq_alloc_tag_set__set__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{
	struct blk_mq_ops** ops_ptr = &ptr->ops;
	unsigned int* nr_hw_queues_ptr = &ptr->nr_hw_queues;
	unsigned int* queue_depth_ptr = &ptr->queue_depth;
	void** driver_data_ptr = &ptr->driver_data;
	struct blk_mq_tags*** tags_ptr = &ptr->tags;

	{
		if (*ops_ptr) {
			caller_unmarshal_kernel___global_blk_mq_ops__in(__pos, __msg, __ext, *ops_ptr);
		}

	}

	{
		*nr_hw_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*queue_depth_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		(void)driver_data_ptr;
	}

	{
		size_t __size = sizeof(struct blk_mq_tags*) * glue_peek(__pos, __msg, __ext);
		*tags_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct blk_mq_tags**, (__size), (DEFAULT_GFP_FLAGS));
		if (*tags_ptr) {
			int i;
			struct blk_mq_tags** array = *tags_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				struct blk_mq_tags** element = &array[i];
				size_t __size = sizeof(struct blk_mq_tags) * glue_peek(__pos, __msg, __ext);
				*element = glue_unpack_new_shadow(__pos, __msg, __ext, struct blk_mq_tags*, (__size), (DEFAULT_GFP_FLAGS));
				if (*element) {
					int i;
					struct blk_mq_tags* array = *element;
					size_t len = glue_unpack(__pos, __msg, __ext, size_t);
					// Warning: see David if this breaks
					glue_user_trace("Warning: see David if this breaks");
					for (i = 0; i < len; ++i) {
						struct blk_mq_tags* element = &array[i];
						caller_unmarshal_kernel__blk_mq_alloc_tag_set__blk_mq_tag_set_tags__out(__pos, __msg, __ext, ctx, element);
					}

				}

			}

		}

	}

	{
	}
}

void caller_marshal_kernel___global_blk_mq_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops const* ptr)
{
	fptr_blk_mq_ops_complete const* complete_ptr = &ptr->complete;
	fptr_blk_mq_ops_exit_hctx const* exit_hctx_ptr = &ptr->exit_hctx;
	fptr_blk_mq_ops_init_hctx const* init_hctx_ptr = &ptr->init_hctx;
	fptr_blk_mq_ops_init_request const* init_request_ptr = &ptr->init_request;
	fptr_blk_mq_ops_map_queue const* map_queue_ptr = &ptr->map_queue;
	fptr_blk_mq_ops_poll const* poll_ptr = &ptr->poll;
	fptr_blk_mq_ops_queue_rq const* queue_rq_ptr = &ptr->queue_rq;
	fptr_blk_mq_ops_timeout const* timeout_ptr = &ptr->timeout;

	{
		glue_pack(__pos, __msg, __ext, *complete_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *exit_hctx_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *init_hctx_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *init_request_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *map_queue_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *poll_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *queue_rq_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *timeout_ptr);
	}

}

void callee_unmarshal_kernel___global_blk_mq_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops* ptr)
{
	fptr_blk_mq_ops_complete* complete_ptr = &ptr->complete;
	fptr_blk_mq_ops_exit_hctx* exit_hctx_ptr = &ptr->exit_hctx;
	fptr_blk_mq_ops_init_hctx* init_hctx_ptr = &ptr->init_hctx;
	fptr_blk_mq_ops_init_request* init_request_ptr = &ptr->init_request;
	fptr_blk_mq_ops_map_queue* map_queue_ptr = &ptr->map_queue;
	fptr_blk_mq_ops_poll* poll_ptr = &ptr->poll;
	fptr_blk_mq_ops_queue_rq* queue_rq_ptr = &ptr->queue_rq;
	fptr_blk_mq_ops_timeout* timeout_ptr = &ptr->timeout;

	{
		*complete_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, blk_mq_ops_complete);
	}

	{
		*exit_hctx_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, blk_mq_ops_exit_hctx);
	}

	{
		*init_hctx_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, blk_mq_ops_init_hctx);
	}

	{
		*init_request_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, blk_mq_ops_init_request);
	}

	{
		*map_queue_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, blk_mq_ops_map_queue);
	}

	{
		*poll_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, blk_mq_ops_poll);
	}

	{
		*queue_rq_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, blk_mq_ops_queue_rq);
	}

	{
		*timeout_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, blk_mq_ops_timeout);
	}

	{
	}
}

void callee_marshal_kernel___global_blk_mq_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops const* ptr)
{

}

void caller_unmarshal_kernel___global_blk_mq_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_alloc_tag_set__blk_mq_tag_set_tags__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* ctx,
	struct blk_mq_tags const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_alloc_tag_set__blk_mq_tag_set_tags__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* ctx,
	struct blk_mq_tags* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_alloc_tag_set__blk_mq_tag_set_tags__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* ctx,
	struct blk_mq_tags const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_alloc_tag_set__blk_mq_tag_set_tags__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_alloc_tag_set_call_ctx const* ctx,
	struct blk_mq_tags* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_complete_request__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct request const* ptr)
{
	struct request_queue* const* q_ptr = &ptr->q;
	unsigned int const* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long const* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int const* __data_len_ptr = &ptr->__data_len;
	unsigned long const* __sector_ptr = &ptr->__sector;
	struct bio* const* bio_ptr = &ptr->bio;
	struct gendisk* const* rq_disk_ptr = &ptr->rq_disk;
	unsigned long const* start_time_ptr = &ptr->start_time;

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_mq_complete_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_type_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__sector_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *bio_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*bio_ptr) {
			caller_marshal_kernel__blk_mq_complete_request__request_bio__io(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *rq_disk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_disk_ptr) {
			caller_marshal_kernel__blk_mq_complete_request__request_rq_disk__io(__pos, __msg, __ext, ctx, *rq_disk_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *start_time_ptr);
	}

}

void callee_unmarshal_kernel__blk_mq_complete_request__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct request* ptr)
{
	struct request_queue** q_ptr = &ptr->q;
	unsigned int* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int* __data_len_ptr = &ptr->__data_len;
	unsigned long* __sector_ptr = &ptr->__sector;
	struct bio** bio_ptr = &ptr->bio;
	struct gendisk** rq_disk_ptr = &ptr->rq_disk;
	unsigned long* start_time_ptr = &ptr->start_time;

	{
		*q_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_mq_complete_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*cmd_type_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*cmd_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*__data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__sector_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*bio_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct bio*);
		if (*bio_ptr) {
			callee_unmarshal_kernel__blk_mq_complete_request__request_bio__io(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		*rq_disk_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct gendisk*);
		if (*rq_disk_ptr) {
			callee_unmarshal_kernel__blk_mq_complete_request__request_rq_disk__io(__pos, __msg, __ext, ctx, *rq_disk_ptr);
		}

	}

	{
		*start_time_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void callee_marshal_kernel__blk_mq_complete_request__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct request const* ptr)
{
	struct request_queue* const* q_ptr = &ptr->q;
	unsigned long long const* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int const* __data_len_ptr = &ptr->__data_len;
	unsigned long const* __sector_ptr = &ptr->__sector;
	struct bio* const* bio_ptr = &ptr->bio;
	struct gendisk* const* rq_disk_ptr = &ptr->rq_disk;
	int const* errors_ptr = &ptr->errors;

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			callee_marshal_kernel__blk_mq_complete_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__sector_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *bio_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*bio_ptr) {
			callee_marshal_kernel__blk_mq_complete_request__request_bio__io(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *rq_disk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_disk_ptr) {
			callee_marshal_kernel__blk_mq_complete_request__request_rq_disk__io(__pos, __msg, __ext, ctx, *rq_disk_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *errors_ptr);
	}

}

void caller_unmarshal_kernel__blk_mq_complete_request__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct request* ptr)
{
	struct request_queue** q_ptr = &ptr->q;
	unsigned long long* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int* __data_len_ptr = &ptr->__data_len;
	unsigned long* __sector_ptr = &ptr->__sector;
	struct bio** bio_ptr = &ptr->bio;
	struct gendisk** rq_disk_ptr = &ptr->rq_disk;
	int* errors_ptr = &ptr->errors;

	{
		*q_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_mq_complete_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*cmd_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*__data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__sector_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*bio_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct bio*);
		if (*bio_ptr) {
			caller_unmarshal_kernel__blk_mq_complete_request__request_bio__io(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		*rq_disk_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct gendisk*);
		if (*rq_disk_ptr) {
			caller_unmarshal_kernel__blk_mq_complete_request__request_rq_disk__io(__pos, __msg, __ext, ctx, *rq_disk_ptr);
		}

	}

	{
		*errors_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void caller_marshal_kernel__blk_mq_complete_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_complete_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_complete_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_complete_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_complete_request__request_bio__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct bio const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_complete_request__request_bio__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct bio* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_complete_request__request_bio__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct bio const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_complete_request__request_bio__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct bio* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_complete_request__request_rq_disk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct gendisk const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_complete_request__request_rq_disk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct gendisk* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_complete_request__request_rq_disk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct gendisk const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_complete_request__request_rq_disk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct gendisk* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_end_request__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct request const* ptr)
{
	struct request_queue* const* q_ptr = &ptr->q;
	unsigned int const* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long const* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int const* __data_len_ptr = &ptr->__data_len;
	unsigned long const* __sector_ptr = &ptr->__sector;
	struct bio* const* bio_ptr = &ptr->bio;
	struct gendisk* const* rq_disk_ptr = &ptr->rq_disk;
	unsigned long const* start_time_ptr = &ptr->start_time;
	int const* tag_ptr = &ptr->tag;

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_mq_end_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_type_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__sector_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *bio_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*bio_ptr) {
			caller_marshal_kernel__blk_mq_end_request__request_bio__io(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *rq_disk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_disk_ptr) {
			caller_marshal_kernel__blk_mq_end_request__request_rq_disk__io(__pos, __msg, __ext, ctx, *rq_disk_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *start_time_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *tag_ptr);
	}

}

void callee_unmarshal_kernel__blk_mq_end_request__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct request* ptr)
{
	struct request_queue** q_ptr = &ptr->q;
	unsigned int* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int* __data_len_ptr = &ptr->__data_len;
	unsigned long* __sector_ptr = &ptr->__sector;
	struct bio** bio_ptr = &ptr->bio;
	struct gendisk** rq_disk_ptr = &ptr->rq_disk;
	unsigned long* start_time_ptr = &ptr->start_time;
	int* tag_ptr = &ptr->tag;

	{
		*q_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_mq_end_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*cmd_type_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*cmd_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*__data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__sector_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*bio_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct bio*);
		if (*bio_ptr) {
			callee_unmarshal_kernel__blk_mq_end_request__request_bio__io(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		*rq_disk_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct gendisk*);
		if (*rq_disk_ptr) {
			callee_unmarshal_kernel__blk_mq_end_request__request_rq_disk__io(__pos, __msg, __ext, ctx, *rq_disk_ptr);
		}

	}

	{
		*start_time_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*tag_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__blk_mq_end_request__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct request const* ptr)
{
	struct request_queue* const* q_ptr = &ptr->q;
	unsigned long long const* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int const* __data_len_ptr = &ptr->__data_len;
	unsigned long const* __sector_ptr = &ptr->__sector;
	struct bio* const* bio_ptr = &ptr->bio;
	struct gendisk* const* rq_disk_ptr = &ptr->rq_disk;
	int const* tag_ptr = &ptr->tag;
	int const* errors_ptr = &ptr->errors;

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			callee_marshal_kernel__blk_mq_end_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__sector_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *bio_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*bio_ptr) {
			callee_marshal_kernel__blk_mq_end_request__request_bio__io(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *rq_disk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_disk_ptr) {
			callee_marshal_kernel__blk_mq_end_request__request_rq_disk__io(__pos, __msg, __ext, ctx, *rq_disk_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tag_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *errors_ptr);
	}

}

void caller_unmarshal_kernel__blk_mq_end_request__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct request* ptr)
{
	struct request_queue** q_ptr = &ptr->q;
	unsigned long long* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int* __data_len_ptr = &ptr->__data_len;
	unsigned long* __sector_ptr = &ptr->__sector;
	struct bio** bio_ptr = &ptr->bio;
	struct gendisk** rq_disk_ptr = &ptr->rq_disk;
	int* tag_ptr = &ptr->tag;
	int* errors_ptr = &ptr->errors;

	{
		*q_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_mq_end_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*cmd_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*__data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__sector_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*bio_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct bio*);
		if (*bio_ptr) {
			caller_unmarshal_kernel__blk_mq_end_request__request_bio__io(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		*rq_disk_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct gendisk*);
		if (*rq_disk_ptr) {
			caller_unmarshal_kernel__blk_mq_end_request__request_rq_disk__io(__pos, __msg, __ext, ctx, *rq_disk_ptr);
		}

	}

	{
		*tag_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*errors_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void caller_marshal_kernel__blk_mq_end_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_end_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_end_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_end_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_end_request__request_bio__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct bio const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_end_request__request_bio__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct bio* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_end_request__request_bio__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct bio const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_end_request__request_bio__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct bio* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_end_request__request_rq_disk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct gendisk const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_end_request__request_rq_disk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct gendisk* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_end_request__request_rq_disk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct gendisk const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_end_request__request_rq_disk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct gendisk* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_free_request__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* ctx,
	struct request const* ptr)
{
	struct request_queue* const* q_ptr = &ptr->q;
	unsigned long long const* cmd_flags_ptr = &ptr->cmd_flags;
	int const* tag_ptr = &ptr->tag;

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_mq_free_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *tag_ptr);
	}

}

void callee_unmarshal_kernel__blk_mq_free_request__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* ctx,
	struct request* ptr)
{
	struct request_queue** q_ptr = &ptr->q;
	unsigned long long* cmd_flags_ptr = &ptr->cmd_flags;
	int* tag_ptr = &ptr->tag;

	{
		*q_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_mq_free_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*cmd_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*tag_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__blk_mq_free_request__rq__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* ctx,
	struct request const* ptr)
{
	struct request_queue* const* q_ptr = &ptr->q;
	unsigned long long const* cmd_flags_ptr = &ptr->cmd_flags;
	int const* tag_ptr = &ptr->tag;

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			callee_marshal_kernel__blk_mq_free_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *tag_ptr);
	}

}

void caller_unmarshal_kernel__blk_mq_free_request__rq__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* ctx,
	struct request* ptr)
{
	struct request_queue** q_ptr = &ptr->q;
	unsigned long long* cmd_flags_ptr = &ptr->cmd_flags;
	int* tag_ptr = &ptr->tag;

	{
		*q_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_mq_free_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*cmd_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*tag_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void caller_marshal_kernel__blk_mq_free_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_free_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_free_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_free_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_free_request_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_free_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_free_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_free_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_free_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_ops_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_ops_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_ops_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_ops_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_ops_map_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_ops_map_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_ops_map_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_ops_map_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_map_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* ctx,
	struct request const* ptr)
{
	struct request_queue* const* q_ptr = &ptr->q;
	unsigned int const* __data_len_ptr = &ptr->__data_len;
	unsigned short const* nr_phys_segments_ptr = &ptr->nr_phys_segments;

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__blk_mq_start_request__request_q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *__data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *nr_phys_segments_ptr);
	}

}

void callee_unmarshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* ctx,
	struct request* ptr)
{
	struct request_queue** q_ptr = &ptr->q;
	unsigned int* __data_len_ptr = &ptr->__data_len;
	unsigned short* nr_phys_segments_ptr = &ptr->nr_phys_segments;

	{
		*q_ptr = glue_unpack(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__blk_mq_start_request__request_q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*__data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*nr_phys_segments_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
	}
}

void callee_marshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* ctx,
	struct request const* ptr)
{
	struct request_queue* const* q_ptr = &ptr->q;
	unsigned short const* nr_phys_segments_ptr = &ptr->nr_phys_segments;

	{
		if (*q_ptr) {
			callee_marshal_kernel__blk_mq_start_request__request_q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *nr_phys_segments_ptr);
	}

}

void caller_unmarshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* ctx,
	struct request* ptr)
{
	struct request_queue** q_ptr = &ptr->q;
	unsigned short* nr_phys_segments_ptr = &ptr->nr_phys_segments;

	{
		if (*q_ptr) {
			caller_unmarshal_kernel__blk_mq_start_request__request_q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*nr_phys_segments_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
	}
}

void caller_marshal_kernel__blk_mq_start_request__request_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_start_request__request_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_start_request__request_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_start_request__request_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_start_request_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_start_stopped_hw_queues__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_start_stopped_hw_queues_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_start_stopped_hw_queues__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_start_stopped_hw_queues_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_start_stopped_hw_queues__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_start_stopped_hw_queues_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_start_stopped_hw_queues__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_start_stopped_hw_queues_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_stop_hw_queues__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_stop_hw_queues_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_stop_hw_queues__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_stop_hw_queues_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_stop_hw_queues__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_stop_hw_queues_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_stop_hw_queues__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_stop_hw_queues_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__tag_iter_fn__rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct tag_iter_fn_call_ctx const* ctx,
	struct request const* ptr)
{

}

void callee_unmarshal_kernel__tag_iter_fn__rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct tag_iter_fn_call_ctx const* ctx,
	struct request* ptr)
{

	{
	}
}

void callee_marshal_kernel__tag_iter_fn__rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct tag_iter_fn_call_ctx const* ctx,
	struct request const* ptr)
{

}

void caller_unmarshal_kernel__tag_iter_fn__rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct tag_iter_fn_call_ctx const* ctx,
	struct request* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_tagset_busy_iter__tagset__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_tagset_busy_iter_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_tagset_busy_iter__tagset__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_tagset_busy_iter_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_tagset_busy_iter__tagset__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_tagset_busy_iter_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_tagset_busy_iter__tagset__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_tagset_busy_iter_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_update_nr_hw_queues__set__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_update_nr_hw_queues_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{
	unsigned int const* nr_hw_queues_ptr = &ptr->nr_hw_queues;

	{
		glue_pack(__pos, __msg, __ext, *nr_hw_queues_ptr);
	}

}

void callee_unmarshal_kernel__blk_mq_update_nr_hw_queues__set__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_update_nr_hw_queues_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{
	unsigned int* nr_hw_queues_ptr = &ptr->nr_hw_queues;

	{
		*nr_hw_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__blk_mq_update_nr_hw_queues__set__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_update_nr_hw_queues_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{
	unsigned int const* nr_hw_queues_ptr = &ptr->nr_hw_queues;

	{
		glue_pack(__pos, __msg, __ext, *nr_hw_queues_ptr);
	}

}

void caller_unmarshal_kernel__blk_mq_update_nr_hw_queues__set__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_update_nr_hw_queues_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{
	unsigned int* nr_hw_queues_ptr = &ptr->nr_hw_queues;

	{
		*nr_hw_queues_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__blk_put_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_put_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__blk_put_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_put_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_put_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_put_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__blk_put_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_put_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__complete__x__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct complete_call_ctx const* ctx,
	struct completion const* ptr)
{
	unsigned int const* done_ptr = &ptr->done;

	{
		glue_pack(__pos, __msg, __ext, *done_ptr);
	}

}

void callee_unmarshal_kernel__complete__x__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct complete_call_ctx const* ctx,
	struct completion* ptr)
{
	unsigned int* done_ptr = &ptr->done;

	{
		*done_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__complete__x__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct complete_call_ctx const* ctx,
	struct completion const* ptr)
{
	unsigned int const* done_ptr = &ptr->done;

	{
		glue_pack(__pos, __msg, __ext, *done_ptr);
	}

}

void caller_unmarshal_kernel__complete__x__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct complete_call_ctx const* ctx,
	struct completion* ptr)
{
	unsigned int* done_ptr = &ptr->done;

	{
		*done_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* ctx,
	struct timer_list const* ptr)
{

}

void callee_unmarshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* ctx,
	struct timer_list* ptr)
{

	{
	}
}

void callee_marshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* ctx,
	struct timer_list const* ptr)
{

}

void caller_unmarshal_kernel__del_timer_sync__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct del_timer_sync_call_ctx const* ctx,
	struct timer_list* ptr)
{

	{
	}
}

void caller_marshal_kernel__destroy_workqueue__wq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct destroy_workqueue_call_ctx const* ctx,
	struct workqueue_struct const* ptr)
{

}

void callee_unmarshal_kernel__destroy_workqueue__wq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct destroy_workqueue_call_ctx const* ctx,
	struct workqueue_struct* ptr)
{

	{
	}
}

void callee_marshal_kernel__destroy_workqueue__wq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct destroy_workqueue_call_ctx const* ctx,
	struct workqueue_struct const* ptr)
{

}

void caller_unmarshal_kernel__destroy_workqueue__wq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct destroy_workqueue_call_ctx const* ctx,
	struct workqueue_struct* ptr)
{

	{
	}
}

void caller_marshal_kernel__device_release_driver__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct device_release_driver_call_ctx const* ctx,
	struct device const* ptr)
{

}

void callee_unmarshal_kernel__device_release_driver__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct device_release_driver_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void callee_marshal_kernel__device_release_driver__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct device_release_driver_call_ctx const* ctx,
	struct device const* ptr)
{

}

void caller_unmarshal_kernel__device_release_driver__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct device_release_driver_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void caller_marshal_kernel__flush_work__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct flush_work_call_ctx const* ctx,
	struct work_struct const* ptr)
{

}

void callee_unmarshal_kernel__flush_work__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct flush_work_call_ctx const* ctx,
	struct work_struct* ptr)
{

	{
	}
}

void callee_marshal_kernel__flush_work__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct flush_work_call_ctx const* ctx,
	struct work_struct const* ptr)
{

}

void caller_unmarshal_kernel__flush_work__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct flush_work_call_ctx const* ctx,
	struct work_struct* ptr)
{

	{
	}
}

void caller_marshal_kernel__get_device__ret_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_device_call_ctx const* ctx,
	struct device const* ptr)
{

}

void callee_unmarshal_kernel__get_device__ret_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_device_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void callee_marshal_kernel__get_device__ret_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_device_call_ctx const* ctx,
	struct device const* ptr)
{

}

void caller_unmarshal_kernel__get_device__ret_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_device_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void caller_marshal_kernel__get_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_device_call_ctx const* ctx,
	struct device const* ptr)
{

}

void callee_unmarshal_kernel__get_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_device_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void callee_marshal_kernel__get_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct get_device_call_ctx const* ctx,
	struct device const* ptr)
{

}

void caller_unmarshal_kernel__get_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct get_device_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void caller_marshal_kernel__lvd_setup_timer__timer__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_setup_timer_call_ctx const* ctx,
	struct timer_list const* ptr)
{

}

void callee_unmarshal_kernel__lvd_setup_timer__timer__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_setup_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{

	{
	}
}

void callee_marshal_kernel__lvd_setup_timer__timer__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_setup_timer_call_ctx const* ctx,
	struct timer_list const* ptr)
{
	unsigned long const* data_ptr = &ptr->data;

	{
		glue_pack(__pos, __msg, __ext, *data_ptr);
	}

}

void caller_unmarshal_kernel__lvd_setup_timer__timer__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_setup_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{
	unsigned long* data_ptr = &ptr->data;

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void caller_marshal_kernel__lvd_init_work__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_init_work_call_ctx const* ctx,
	struct work_struct const* ptr)
{

}

void callee_unmarshal_kernel__lvd_init_work__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_init_work_call_ctx const* ctx,
	struct work_struct* ptr)
{

	{
	}
}

void callee_marshal_kernel__lvd_init_work__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_init_work_call_ctx const* ctx,
	struct work_struct const* ptr)
{

}

void caller_unmarshal_kernel__lvd_init_work__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_init_work_call_ctx const* ctx,
	struct work_struct* ptr)
{

	{
	}
}

void caller_marshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* ctx,
	struct cpumask const* ptr)
{
	unsigned long const* bits_ptr = ptr->bits;

	{
		size_t i, len = 1;
		unsigned long const* array = bits_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned long const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

}

void callee_unmarshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* ctx,
	struct cpumask* ptr)
{
	unsigned long* bits_ptr = ptr->bits;

	{
		int i;
		unsigned long* array = bits_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned long* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned long);
		}

	}

	{
	}
}

void callee_marshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* ctx,
	struct cpumask const* ptr)
{

}

void caller_unmarshal_kernel__irq_set_affinity_hint__m__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct irq_set_affinity_hint_call_ctx const* ctx,
	struct cpumask* ptr)
{

	{
	}
}

void caller_marshal_kernel__mod_timer__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct timer_list const* ptr)
{

}

void callee_unmarshal_kernel__mod_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{

	{
	}
}

void callee_marshal_kernel__mod_timer__timer__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct timer_list const* ptr)
{

}

void caller_unmarshal_kernel__mod_timer__timer__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct mod_timer_call_ctx const* ctx,
	struct timer_list* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_ops_exit_hctx__hctx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_exit_hctx_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_ops_exit_hctx__hctx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_exit_hctx_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_ops_exit_hctx__hctx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_exit_hctx_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_ops_exit_hctx__hctx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_exit_hctx_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_ops_init_hctx__hctx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_init_hctx_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{
	struct blk_mq_tags* const* tags_ptr = &ptr->tags;

	{
		__maybe_unused const void* __adjusted = *tags_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*tags_ptr) {
			caller_marshal_kernel__blk_mq_ops_init_hctx__blk_mq_hw_ctx_tags__in(__pos, __msg, __ext, ctx, *tags_ptr);
		}

	}

}

void callee_unmarshal_kernel__blk_mq_ops_init_hctx__hctx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_init_hctx_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{
	struct blk_mq_tags** tags_ptr = &ptr->tags;

	{
		*tags_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct blk_mq_tags*);
		if (*tags_ptr) {
			callee_unmarshal_kernel__blk_mq_ops_init_hctx__blk_mq_hw_ctx_tags__in(__pos, __msg, __ext, ctx, *tags_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__blk_mq_ops_init_hctx__hctx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_init_hctx_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{
	struct blk_mq_tags* const* tags_ptr = &ptr->tags;

	{
		if (*tags_ptr) {
			callee_marshal_kernel__blk_mq_ops_init_hctx__blk_mq_hw_ctx_tags__in(__pos, __msg, __ext, ctx, *tags_ptr);
		}

	}

}

void caller_unmarshal_kernel__blk_mq_ops_init_hctx__hctx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_init_hctx_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{
	struct blk_mq_tags** tags_ptr = &ptr->tags;

	{
		if (*tags_ptr) {
			caller_unmarshal_kernel__blk_mq_ops_init_hctx__blk_mq_hw_ctx_tags__in(__pos, __msg, __ext, ctx, *tags_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__blk_mq_ops_init_hctx__blk_mq_hw_ctx_tags__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_init_hctx_call_ctx const* ctx,
	struct blk_mq_tags const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_ops_init_hctx__blk_mq_hw_ctx_tags__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_init_hctx_call_ctx const* ctx,
	struct blk_mq_tags* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_ops_init_hctx__blk_mq_hw_ctx_tags__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_init_hctx_call_ctx const* ctx,
	struct blk_mq_tags const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_ops_init_hctx__blk_mq_hw_ctx_tags__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_init_hctx_call_ctx const* ctx,
	struct blk_mq_tags* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_ops_init_request__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_init_request_call_ctx const* ctx,
	struct request const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_ops_init_request__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_init_request_call_ctx const* ctx,
	struct request* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_ops_init_request__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_init_request_call_ctx const* ctx,
	struct request const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_ops_init_request__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_init_request_call_ctx const* ctx,
	struct request* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_alloc_request__ret_request__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* ctx,
	struct request const* ptr)
{

}

void callee_unmarshal_kernel__nvme_alloc_request__ret_request__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* ctx,
	struct request* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_alloc_request__ret_request__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* ctx,
	struct request const* ptr)
{
	unsigned int const* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long const* cmd_flags_ptr = &ptr->cmd_flags;

	{
		glue_pack(__pos, __msg, __ext, *cmd_type_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_flags_ptr);
	}

}

void caller_unmarshal_kernel__nvme_alloc_request__ret_request__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* ctx,
	struct request* ptr)
{
	unsigned int* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long* cmd_flags_ptr = &ptr->cmd_flags;

	{
		*cmd_type_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*cmd_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void caller_marshal_kernel__nvme_alloc_request__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	unsigned long const* queue_flags_ptr = &ptr->queue_flags;

	{
		glue_pack(__pos, __msg, __ext, *queue_flags_ptr);
	}

}

void callee_unmarshal_kernel__nvme_alloc_request__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* ctx,
	struct request_queue* ptr)
{
	unsigned long* queue_flags_ptr = &ptr->queue_flags;

	{
		*queue_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void callee_marshal_kernel__nvme_alloc_request__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__nvme_alloc_request__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_alloc_request__cmd__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* ctx,
	struct nvme_command const* ptr)
{

}

void callee_unmarshal_kernel__nvme_alloc_request__cmd__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* ctx,
	struct nvme_command* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_alloc_request__cmd__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* ctx,
	struct nvme_command const* ptr)
{

}

void caller_unmarshal_kernel__nvme_alloc_request__cmd__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_alloc_request_call_ctx const* ctx,
	struct nvme_command* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_cancel_request__req__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct request const* ptr)
{
	struct request_queue* const* q_ptr = &ptr->q;
	unsigned int const* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long const* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int const* __data_len_ptr = &ptr->__data_len;
	unsigned long const* __sector_ptr = &ptr->__sector;
	struct bio* const* bio_ptr = &ptr->bio;
	struct gendisk* const* rq_disk_ptr = &ptr->rq_disk;
	unsigned long const* start_time_ptr = &ptr->start_time;
	int const* tag_ptr = &ptr->tag;
	int const* errors_ptr = &ptr->errors;

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__nvme_cancel_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_type_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__sector_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *bio_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*bio_ptr) {
			caller_marshal_kernel__nvme_cancel_request__request_bio__io(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *rq_disk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_disk_ptr) {
			caller_marshal_kernel__nvme_cancel_request__request_rq_disk__io(__pos, __msg, __ext, ctx, *rq_disk_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *start_time_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *tag_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *errors_ptr);
	}

}

void callee_unmarshal_kernel__nvme_cancel_request__req__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct request* ptr)
{
	struct request_queue** q_ptr = &ptr->q;
	unsigned int* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int* __data_len_ptr = &ptr->__data_len;
	unsigned long* __sector_ptr = &ptr->__sector;
	struct bio** bio_ptr = &ptr->bio;
	struct gendisk** rq_disk_ptr = &ptr->rq_disk;
	unsigned long* start_time_ptr = &ptr->start_time;
	int* tag_ptr = &ptr->tag;
	int* errors_ptr = &ptr->errors;

	{
		*q_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__nvme_cancel_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*cmd_type_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*cmd_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*__data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__sector_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*bio_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct bio*);
		if (*bio_ptr) {
			callee_unmarshal_kernel__nvme_cancel_request__request_bio__io(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		*rq_disk_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct gendisk*);
		if (*rq_disk_ptr) {
			callee_unmarshal_kernel__nvme_cancel_request__request_rq_disk__io(__pos, __msg, __ext, ctx, *rq_disk_ptr);
		}

	}

	{
		*start_time_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*tag_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*errors_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__nvme_cancel_request__req__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct request const* ptr)
{
	struct request_queue* const* q_ptr = &ptr->q;
	unsigned long long const* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int const* __data_len_ptr = &ptr->__data_len;
	unsigned long const* __sector_ptr = &ptr->__sector;
	struct bio* const* bio_ptr = &ptr->bio;
	struct gendisk* const* rq_disk_ptr = &ptr->rq_disk;
	int const* tag_ptr = &ptr->tag;
	int const* errors_ptr = &ptr->errors;

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			callee_marshal_kernel__nvme_cancel_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__sector_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *bio_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*bio_ptr) {
			callee_marshal_kernel__nvme_cancel_request__request_bio__io(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *rq_disk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*rq_disk_ptr) {
			callee_marshal_kernel__nvme_cancel_request__request_rq_disk__io(__pos, __msg, __ext, ctx, *rq_disk_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tag_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *errors_ptr);
	}

}

void caller_unmarshal_kernel__nvme_cancel_request__req__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct request* ptr)
{
	struct request_queue** q_ptr = &ptr->q;
	unsigned long long* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int* __data_len_ptr = &ptr->__data_len;
	unsigned long* __sector_ptr = &ptr->__sector;
	struct bio** bio_ptr = &ptr->bio;
	struct gendisk** rq_disk_ptr = &ptr->rq_disk;
	int* tag_ptr = &ptr->tag;
	int* errors_ptr = &ptr->errors;

	{
		*q_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			caller_unmarshal_kernel__nvme_cancel_request__request_q__io(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*cmd_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*__data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__sector_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*bio_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct bio*);
		if (*bio_ptr) {
			caller_unmarshal_kernel__nvme_cancel_request__request_bio__io(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		*rq_disk_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct gendisk*);
		if (*rq_disk_ptr) {
			caller_unmarshal_kernel__nvme_cancel_request__request_rq_disk__io(__pos, __msg, __ext, ctx, *rq_disk_ptr);
		}

	}

	{
		*tag_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*errors_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void caller_marshal_kernel__nvme_cancel_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__nvme_cancel_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_cancel_request__request_q__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__nvme_cancel_request__request_q__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_cancel_request__request_bio__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct bio const* ptr)
{

}

void callee_unmarshal_kernel__nvme_cancel_request__request_bio__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct bio* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_cancel_request__request_bio__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct bio const* ptr)
{

}

void caller_unmarshal_kernel__nvme_cancel_request__request_bio__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct bio* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_cancel_request__request_rq_disk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct gendisk const* ptr)
{

}

void callee_unmarshal_kernel__nvme_cancel_request__request_rq_disk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct gendisk* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_cancel_request__request_rq_disk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct gendisk const* ptr)
{

}

void caller_unmarshal_kernel__nvme_cancel_request__request_rq_disk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_cancel_request_call_ctx const* ctx,
	struct gendisk* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_change_ctrl_state__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_change_ctrl_state_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	unsigned int const* state_ptr = &ptr->state;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

}

void callee_unmarshal_kernel__nvme_change_ctrl_state__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_change_ctrl_state_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	unsigned int* state_ptr = &ptr->state;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__nvme_change_ctrl_state__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_change_ctrl_state_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	unsigned int const* state_ptr = &ptr->state;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

}

void caller_unmarshal_kernel__nvme_change_ctrl_state__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_change_ctrl_state_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	unsigned int* state_ptr = &ptr->state;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__nvme_complete_async_event__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void callee_unmarshal_kernel__nvme_complete_async_event__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_complete_async_event__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void caller_unmarshal_kernel__nvme_complete_async_event__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_complete_async_event__cqe__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* ctx,
	struct nvme_completion const* ptr)
{
	unsigned short const* status_ptr = &ptr->status;

	{
		glue_pack(__pos, __msg, __ext, *status_ptr);
	}

}

void callee_unmarshal_kernel__nvme_complete_async_event__cqe__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* ctx,
	struct nvme_completion* ptr)
{
	unsigned short* status_ptr = &ptr->status;

	{
		*status_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
	}
}

void callee_marshal_kernel__nvme_complete_async_event__cqe__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* ctx,
	struct nvme_completion const* ptr)
{

}

void caller_unmarshal_kernel__nvme_complete_async_event__cqe__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_complete_async_event_call_ctx const* ctx,
	struct nvme_completion* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_ops_complete__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_complete_call_ctx const* ctx,
	struct request const* ptr)
{
	unsigned int const* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long const* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned long const* start_time_ptr = &ptr->start_time;
	int const* errors_ptr = &ptr->errors;
	unsigned int const* timeout_ptr = &ptr->timeout;

	{
		glue_pack(__pos, __msg, __ext, *cmd_type_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *start_time_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *errors_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *timeout_ptr);
	}

}

void callee_unmarshal_kernel__blk_mq_ops_complete__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_complete_call_ctx const* ctx,
	struct request* ptr)
{
	unsigned int* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned long* start_time_ptr = &ptr->start_time;
	int* errors_ptr = &ptr->errors;
	unsigned int* timeout_ptr = &ptr->timeout;

	{
		*cmd_type_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*cmd_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*start_time_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*errors_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*timeout_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__blk_mq_ops_complete__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_complete_call_ctx const* ctx,
	struct request const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_ops_complete__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_complete_call_ctx const* ctx,
	struct request* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_disable_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_disable_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	unsigned int const* ctrl_config_ptr = &ptr->ctrl_config;

	{
		glue_pack(__pos, __msg, __ext, *ctrl_config_ptr);
	}

}

void callee_unmarshal_kernel__nvme_disable_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_disable_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	unsigned int* ctrl_config_ptr = &ptr->ctrl_config;

	{
		*ctrl_config_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__nvme_disable_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_disable_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	unsigned int const* ctrl_config_ptr = &ptr->ctrl_config;

	{
		glue_pack(__pos, __msg, __ext, *ctrl_config_ptr);
	}

}

void caller_unmarshal_kernel__nvme_disable_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_disable_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	unsigned int* ctrl_config_ptr = &ptr->ctrl_config;

	{
		*ctrl_config_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__nvme_enable_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_enable_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	unsigned int const* ctrl_config_ptr = &ptr->ctrl_config;

	{
		glue_pack(__pos, __msg, __ext, *ctrl_config_ptr);
	}

}

void callee_unmarshal_kernel__nvme_enable_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_enable_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	unsigned int* ctrl_config_ptr = &ptr->ctrl_config;

	{
		*ctrl_config_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__nvme_enable_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_enable_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	unsigned int const* ctrl_config_ptr = &ptr->ctrl_config;
	unsigned int const* page_size_ptr = &ptr->page_size;

	{
		glue_pack(__pos, __msg, __ext, *ctrl_config_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *page_size_ptr);
	}

}

void caller_unmarshal_kernel__nvme_enable_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_enable_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	unsigned int* ctrl_config_ptr = &ptr->ctrl_config;
	unsigned int* page_size_ptr = &ptr->page_size;

	{
		*ctrl_config_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*page_size_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__pci_cleanup_aer_uncorrect_error_status__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_cleanup_aer_uncorrect_error_status_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_cleanup_aer_uncorrect_error_status__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_cleanup_aer_uncorrect_error_status_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_cleanup_aer_uncorrect_error_status__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_cleanup_aer_uncorrect_error_status_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_cleanup_aer_uncorrect_error_status__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_cleanup_aer_uncorrect_error_status_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_error_handlers_error_detected__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_error_detected_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_error_handlers_error_detected__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_error_detected_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_error_handlers_error_detected__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_error_detected_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_error_handlers_error_detected__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_error_detected_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_error_handlers_resume__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_resume_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_error_handlers_resume__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_resume_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_error_handlers_resume__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_resume_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_error_handlers_resume__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_resume_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__reg_read32__nvme_ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct reg_read32_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void callee_unmarshal_kernel__reg_read32__nvme_ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct reg_read32_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void callee_marshal_kernel__reg_read32__nvme_ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct reg_read32_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void caller_unmarshal_kernel__reg_read32__nvme_ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct reg_read32_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void caller_marshal_kernel__reg_write32__nvme_ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct reg_write32_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void callee_unmarshal_kernel__reg_write32__nvme_ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct reg_write32_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void callee_marshal_kernel__reg_write32__nvme_ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct reg_write32_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void caller_unmarshal_kernel__reg_write32__nvme_ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct reg_write32_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void caller_marshal_kernel__reg_read64__nvme_ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct reg_read64_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void callee_unmarshal_kernel__reg_read64__nvme_ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct reg_read64_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void callee_marshal_kernel__reg_read64__nvme_ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct reg_read64_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void caller_unmarshal_kernel__reg_read64__nvme_ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct reg_read64_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void caller_marshal_kernel__reset_ctrl__nvme_ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct reset_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void callee_unmarshal_kernel__reset_ctrl__nvme_ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct reset_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void callee_marshal_kernel__reset_ctrl__nvme_ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct reset_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void caller_unmarshal_kernel__reset_ctrl__nvme_ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct reset_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void caller_marshal_kernel__free_ctrl__nvme_ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct free_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void callee_unmarshal_kernel__free_ctrl__nvme_ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct free_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void callee_marshal_kernel__free_ctrl__nvme_ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct free_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void caller_unmarshal_kernel__free_ctrl__nvme_ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct free_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void caller_marshal_kernel__post_scan__nvme_ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct post_scan_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void callee_unmarshal_kernel__post_scan__nvme_ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct post_scan_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void callee_marshal_kernel__post_scan__nvme_ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct post_scan_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void caller_unmarshal_kernel__post_scan__nvme_ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct post_scan_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void caller_marshal_kernel__submit_async_event__nvme_ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct submit_async_event_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void callee_unmarshal_kernel__submit_async_event__nvme_ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct submit_async_event_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void callee_marshal_kernel__submit_async_event__nvme_ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct submit_async_event_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void caller_unmarshal_kernel__submit_async_event__nvme_ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct submit_async_event_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_init_ctrl__ctrl__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	unsigned int const* ctrl_config_ptr = &ptr->ctrl_config;
	struct kref const* kref_ptr = &ptr->kref;
	int const* instance_ptr = &ptr->instance;
	struct device* const* device_ptr = &ptr->device;

	{
		glue_pack(__pos, __msg, __ext, *ctrl_config_ptr);
	}

	{
		caller_marshal_kernel__nvme_init_ctrl__kref__io(__pos, __msg, __ext, ctx, kref_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *instance_ptr);
	}

	{
		if (*device_ptr) {
			caller_marshal_kernel__nvme_init_ctrl__nvme_ctrl_device__out(__pos, __msg, __ext, ctx, *device_ptr);
		}

	}

}

void callee_unmarshal_kernel__nvme_init_ctrl__ctrl__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	unsigned int* ctrl_config_ptr = &ptr->ctrl_config;
	struct kref* kref_ptr = &ptr->kref;
	int* instance_ptr = &ptr->instance;
	struct device** device_ptr = &ptr->device;

	{
		*ctrl_config_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		callee_unmarshal_kernel__nvme_init_ctrl__kref__io(__pos, __msg, __ext, ctx, kref_ptr);
	}

	{
		*instance_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		if (*device_ptr) {
			callee_unmarshal_kernel__nvme_init_ctrl__nvme_ctrl_device__out(__pos, __msg, __ext, ctx, *device_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__nvme_init_ctrl__ctrl__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	unsigned int const* state_ptr = &ptr->state;
	unsigned int const* ctrl_config_ptr = &ptr->ctrl_config;
	struct kref const* kref_ptr = &ptr->kref;
	int const* instance_ptr = &ptr->instance;
	struct device* const* device_ptr = &ptr->device;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ctrl_config_ptr);
	}

	{
		callee_marshal_kernel__nvme_init_ctrl__kref__io(__pos, __msg, __ext, ctx, kref_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *instance_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *device_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*device_ptr) {
			callee_marshal_kernel__nvme_init_ctrl__nvme_ctrl_device__out(__pos, __msg, __ext, ctx, *device_ptr);
		}

	}

}

void caller_unmarshal_kernel__nvme_init_ctrl__ctrl__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	unsigned int* state_ptr = &ptr->state;
	unsigned int* ctrl_config_ptr = &ptr->ctrl_config;
	struct kref* kref_ptr = &ptr->kref;
	int* instance_ptr = &ptr->instance;
	struct device** device_ptr = &ptr->device;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*ctrl_config_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		caller_unmarshal_kernel__nvme_init_ctrl__kref__io(__pos, __msg, __ext, ctx, kref_ptr);
	}

	{
		*instance_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		size_t __size = sizeof(struct device);
		*device_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct device*, (__size), (DEFAULT_GFP_FLAGS));
		if (*device_ptr) {
			caller_unmarshal_kernel__nvme_init_ctrl__nvme_ctrl_device__out(__pos, __msg, __ext, ctx, *device_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__nvme_init_ctrl__kref__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct kref const* ptr)
{

}

void callee_unmarshal_kernel__nvme_init_ctrl__kref__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct kref* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_init_ctrl__kref__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct kref const* ptr)
{

}

void caller_unmarshal_kernel__nvme_init_ctrl__kref__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct kref* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_init_ctrl__nvme_ctrl_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct device const* ptr)
{

}

void callee_unmarshal_kernel__nvme_init_ctrl__nvme_ctrl_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_init_ctrl__nvme_ctrl_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct device const* ptr)
{

}

void caller_unmarshal_kernel__nvme_init_ctrl__nvme_ctrl_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_init_ctrl__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct device const* ptr)
{

}

void callee_unmarshal_kernel__nvme_init_ctrl__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_init_ctrl__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct device const* ptr)
{

}

void caller_unmarshal_kernel__nvme_init_ctrl__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_init_ctrl__ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct nvme_ctrl_ops const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	struct module* const* module_ptr = &ptr->module;
	fptr_reg_read32 const* reg_read32_ptr = &ptr->reg_read32;
	fptr_reg_write32 const* reg_write32_ptr = &ptr->reg_write32;
	fptr_reg_read64 const* reg_read64_ptr = &ptr->reg_read64;
	fptr_reset_ctrl const* reset_ctrl_ptr = &ptr->reset_ctrl;
	fptr_free_ctrl const* free_ctrl_ptr = &ptr->free_ctrl;
	fptr_post_scan const* post_scan_ptr = &ptr->post_scan;
	fptr_submit_async_event const* submit_async_event_ptr = &ptr->submit_async_event;

	{
		__maybe_unused const void* __adjusted = *name_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*name_ptr) {
			size_t i, len;
			char const* array = *name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		__maybe_unused const void* __adjusted = *module_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*module_ptr) {
			caller_marshal_kernel__nvme_init_ctrl__module__in(__pos, __msg, __ext, ctx, *module_ptr);
		}
	}

	{
		glue_pack(__pos, __msg, __ext, *reg_read32_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *reg_write32_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *reg_read64_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *reset_ctrl_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *free_ctrl_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *post_scan_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *submit_async_event_ptr);
	}

}

void callee_unmarshal_kernel__nvme_init_ctrl__ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct nvme_ctrl_ops* ptr)
{
	char const** name_ptr = &ptr->name;
	struct module** module_ptr = &ptr->module;
	fptr_reg_read32* reg_read32_ptr = &ptr->reg_read32;
	fptr_reg_write32* reg_write32_ptr = &ptr->reg_write32;
	fptr_reg_read64* reg_read64_ptr = &ptr->reg_read64;
	fptr_reset_ctrl* reset_ctrl_ptr = &ptr->reset_ctrl;
	fptr_free_ctrl* free_ctrl_ptr = &ptr->free_ctrl;
	fptr_post_scan* post_scan_ptr = &ptr->post_scan;
	fptr_submit_async_event* submit_async_event_ptr = &ptr->submit_async_event;

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*name_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*name_ptr) {
			char* writable = (char*)*name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(__pos, __msg, __ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	{
		size_t __size = sizeof(struct module);
		*module_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct module*, (__size), (DEFAULT_GFP_FLAGS));
		if (*module_ptr) {
			callee_unmarshal_kernel__nvme_init_ctrl__module__in(__pos, __msg, __ext, ctx, *module_ptr);
		}
		*module_ptr = THIS_MODULE;

	}

	{
		*reg_read32_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, reg_read32);
	}

	{
		*reg_write32_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, reg_write32);
	}

	{
		*reg_read64_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, reg_read64);
	}

	{
		*reset_ctrl_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, reset_ctrl);
	}

	{
		*free_ctrl_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, free_ctrl);
	}

	{
		*post_scan_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, post_scan);
	}

	{
		*submit_async_event_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, submit_async_event);
	}

	{
	}
}

void callee_marshal_kernel__nvme_init_ctrl__ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct nvme_ctrl_ops const* ptr)
{
	char const* const* name_ptr = &ptr->name;
	struct module* const* module_ptr = &ptr->module;

	{
		(void)name_ptr;
	}

	{
		if (*module_ptr) {
			callee_marshal_kernel__nvme_init_ctrl__module__in(__pos, __msg, __ext, ctx, *module_ptr);
		}

	}

}

void caller_unmarshal_kernel__nvme_init_ctrl__ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct nvme_ctrl_ops* ptr)
{
	char const** name_ptr = &ptr->name;
	struct module** module_ptr = &ptr->module;

	{
		(void)name_ptr;
	}

	{
		if (*module_ptr) {
			caller_unmarshal_kernel__nvme_init_ctrl__module__in(__pos, __msg, __ext, ctx, *module_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__nvme_init_ctrl__module__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct module const* ptr)
{

}

void callee_unmarshal_kernel__nvme_init_ctrl__module__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct module* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_init_ctrl__module__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct module const* ptr)
{

}

void caller_unmarshal_kernel__nvme_init_ctrl__module__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_ctrl_call_ctx const* ctx,
	struct module* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_init_identify__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	struct request_queue* const* admin_q_ptr = &ptr->admin_q;
	struct device* const* device_ptr = &ptr->device;
	struct atomic64_t const* abort_limit_ptr = &ptr->abort_limit;

	{
		__maybe_unused const void* __adjusted = *admin_q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*admin_q_ptr) {
			caller_marshal_kernel__nvme_init_identify__nvme_ctrl_admin_q__in(__pos, __msg, __ext, ctx, *admin_q_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *device_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*device_ptr) {
			caller_marshal_kernel__nvme_init_identify__nvme_ctrl_device__in(__pos, __msg, __ext, ctx, *device_ptr);
		}

	}

	{
		caller_marshal_kernel__nvme_init_identify__atomic64_t__out(__pos, __msg, __ext, ctx, abort_limit_ptr);
	}

}

void callee_unmarshal_kernel__nvme_init_identify__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	struct request_queue** admin_q_ptr = &ptr->admin_q;
	struct device** device_ptr = &ptr->device;
	struct atomic64_t* abort_limit_ptr = &ptr->abort_limit;

	{
		*admin_q_ptr = glue_unpack(__pos, __msg, __ext, struct request_queue*);
		if (*admin_q_ptr) {
			callee_unmarshal_kernel__nvme_init_identify__nvme_ctrl_admin_q__in(__pos, __msg, __ext, ctx, *admin_q_ptr);
		}

	}

	{
		*device_ptr = glue_unpack(__pos, __msg, __ext, struct device*);
		if (*device_ptr) {
			callee_unmarshal_kernel__nvme_init_identify__nvme_ctrl_device__in(__pos, __msg, __ext, ctx, *device_ptr);
		}

	}

	{
		callee_unmarshal_kernel__nvme_init_identify__atomic64_t__out(__pos, __msg, __ext, ctx, abort_limit_ptr);
	}

	{
	}
}

void callee_marshal_kernel__nvme_init_identify__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	struct request_queue* const* admin_q_ptr = &ptr->admin_q;
	struct device* const* device_ptr = &ptr->device;
	struct atomic64_t const* abort_limit_ptr = &ptr->abort_limit;

	{
		if (*admin_q_ptr) {
			callee_marshal_kernel__nvme_init_identify__nvme_ctrl_admin_q__in(__pos, __msg, __ext, ctx, *admin_q_ptr);
		}

	}

	{
		if (*device_ptr) {
			callee_marshal_kernel__nvme_init_identify__nvme_ctrl_device__in(__pos, __msg, __ext, ctx, *device_ptr);
		}

	}

	{
		callee_marshal_kernel__nvme_init_identify__atomic64_t__out(__pos, __msg, __ext, ctx, abort_limit_ptr);
	}

}

void caller_unmarshal_kernel__nvme_init_identify__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	struct request_queue** admin_q_ptr = &ptr->admin_q;
	struct device** device_ptr = &ptr->device;
	struct atomic64_t* abort_limit_ptr = &ptr->abort_limit;

	{
		if (*admin_q_ptr) {
			caller_unmarshal_kernel__nvme_init_identify__nvme_ctrl_admin_q__in(__pos, __msg, __ext, ctx, *admin_q_ptr);
		}

	}

	{
		if (*device_ptr) {
			caller_unmarshal_kernel__nvme_init_identify__nvme_ctrl_device__in(__pos, __msg, __ext, ctx, *device_ptr);
		}

	}

	{
		caller_unmarshal_kernel__nvme_init_identify__atomic64_t__out(__pos, __msg, __ext, ctx, abort_limit_ptr);
	}

	{
	}
}

void caller_marshal_kernel__nvme_init_identify__nvme_ctrl_admin_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__nvme_init_identify__nvme_ctrl_admin_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_init_identify__nvme_ctrl_admin_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__nvme_init_identify__nvme_ctrl_admin_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_init_identify__nvme_ctrl_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct device const* ptr)
{

}

void callee_unmarshal_kernel__nvme_init_identify__nvme_ctrl_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_init_identify__nvme_ctrl_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct device const* ptr)
{

}

void caller_unmarshal_kernel__nvme_init_identify__nvme_ctrl_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_init_identify__atomic64_t__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct atomic64_t const* ptr)
{

}

void callee_unmarshal_kernel__nvme_init_identify__atomic64_t__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct atomic64_t* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_init_identify__atomic64_t__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct atomic64_t const* ptr)
{
	long const* counter_ptr = &ptr->counter;

	{
		glue_pack(__pos, __msg, __ext, *counter_ptr);
	}

}

void caller_unmarshal_kernel__nvme_init_identify__atomic64_t__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_init_identify_call_ctx const* ctx,
	struct atomic64_t* ptr)
{
	long* counter_ptr = &ptr->counter;

	{
		*counter_ptr = glue_unpack(__pos, __msg, __ext, long);
	}

	{
	}
}

void caller_marshal_kernel__nvme_kill_queues__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_kill_queues_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void callee_unmarshal_kernel__nvme_kill_queues__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_kill_queues_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_kill_queues__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_kill_queues_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void caller_unmarshal_kernel__nvme_kill_queues__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_kill_queues_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_driver_sriov_configure__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_sriov_configure_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_driver_sriov_configure__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_sriov_configure_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_driver_sriov_configure__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_sriov_configure_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_driver_sriov_configure__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_sriov_configure_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_ops_poll__hctx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_poll_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_ops_poll__hctx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_poll_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_ops_poll__hctx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_poll_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_ops_poll__hctx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_poll_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	unsigned int const* devfn_ptr = &ptr->devfn;
	unsigned short const* vendor_ptr = &ptr->vendor;
	unsigned short const* device_ptr = &ptr->device;
	unsigned short const* subsystem_vendor_ptr = &ptr->subsystem_vendor;
	unsigned short const* subsystem_device_ptr = &ptr->subsystem_device;
	unsigned char const* revision_ptr = &ptr->revision;
	unsigned int __is_virtfn = ptr->is_virtfn;
	unsigned int const* __is_virtfn_ptr = &__is_virtfn;
	struct device const* dev_ptr = &ptr->dev;
	struct resource const* resource_ptr = ptr->resource;

	{
		__maybe_unused const void* __adjusted = *bus_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*bus_ptr) {
			caller_marshal_kernel__pci_driver_probe__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *devfn_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *vendor_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *device_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *subsystem_vendor_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *subsystem_device_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *revision_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__is_virtfn_ptr);
	}

	{
		caller_marshal_kernel__pci_driver_probe__device__in(__pos, __msg, __ext, ctx, dev_ptr);
	}

	{
		size_t i, len = 11;
		struct resource const* array = resource_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			struct resource const* element = &array[i];
			caller_marshal_kernel__pci_driver_probe__resource__in(__pos, __msg, __ext, ctx, element);
		}

	}

}

void callee_unmarshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	unsigned int* devfn_ptr = &ptr->devfn;
	unsigned short* vendor_ptr = &ptr->vendor;
	unsigned short* device_ptr = &ptr->device;
	unsigned short* subsystem_vendor_ptr = &ptr->subsystem_vendor;
	unsigned short* subsystem_device_ptr = &ptr->subsystem_device;
	unsigned char* revision_ptr = &ptr->revision;
	unsigned int __is_virtfn = ptr->is_virtfn;
	unsigned int* __is_virtfn_ptr = &__is_virtfn;
	struct device* dev_ptr = &ptr->dev;
	struct resource* resource_ptr = ptr->resource;

	{
		size_t __size = sizeof(struct pci_bus);
		*bus_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct pci_bus*, (__size), (DEFAULT_GFP_FLAGS));
		if (*bus_ptr) {
			callee_unmarshal_kernel__pci_driver_probe__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
		*devfn_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*vendor_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*device_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*subsystem_vendor_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*subsystem_device_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*revision_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
		*__is_virtfn_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		callee_unmarshal_kernel__pci_driver_probe__device__in(__pos, __msg, __ext, ctx, dev_ptr);
	}

	{
		int i;
		struct resource* array = resource_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			struct resource* element = &array[i];
			callee_unmarshal_kernel__pci_driver_probe__resource__in(__pos, __msg, __ext, ctx, element);
		}

	}

	{
		ptr->is_virtfn = *__is_virtfn_ptr;
	}
}

void callee_marshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct pci_bus* const* bus_ptr = &ptr->bus;
	struct device const* dev_ptr = &ptr->dev;

	{
		if (*bus_ptr) {
			callee_marshal_kernel__pci_driver_probe__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
		callee_marshal_kernel__pci_driver_probe__device__in(__pos, __msg, __ext, ctx, dev_ptr);
	}

}

void caller_unmarshal_kernel__pci_driver_probe__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct pci_bus** bus_ptr = &ptr->bus;
	struct device* dev_ptr = &ptr->dev;

	{
		if (*bus_ptr) {
			caller_unmarshal_kernel__pci_driver_probe__pci_dev_bus__in(__pos, __msg, __ext, ctx, *bus_ptr);
		}

	}

	{
		caller_unmarshal_kernel__pci_driver_probe__device__in(__pos, __msg, __ext, ctx, dev_ptr);
	}

	{
	}
}

void caller_marshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_bus const* ptr)
{
	unsigned char const* number_ptr = &ptr->number;

	{
		glue_pack(__pos, __msg, __ext, *number_ptr);
	}

}

void callee_unmarshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_bus* ptr)
{
	unsigned char* number_ptr = &ptr->number;

	{
		*number_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void callee_marshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_bus const* ptr)
{

}

void caller_unmarshal_kernel__pci_driver_probe__pci_dev_bus__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_bus* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_driver_probe__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct device const* ptr)
{
	unsigned long long* const* dma_mask_ptr = &ptr->dma_mask;
	struct kobject const* kobj_ptr = &ptr->kobj;

	{
		__maybe_unused const void* __adjusted = *dma_mask_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dma_mask_ptr) {
			size_t i, len = 1;
			unsigned long long const* array = *dma_mask_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned long long const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		caller_marshal_kernel__pci_driver_probe__kobject__in(__pos, __msg, __ext, ctx, kobj_ptr);
	}

}

void callee_unmarshal_kernel__pci_driver_probe__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct device* ptr)
{
	unsigned long long** dma_mask_ptr = &ptr->dma_mask;
	struct kobject* kobj_ptr = &ptr->kobj;

	{
		size_t __size = sizeof(unsigned long long) * glue_peek(__pos, __msg, __ext);
		*dma_mask_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, unsigned long long*, (__size), (DEFAULT_GFP_FLAGS));
		if (*dma_mask_ptr) {
			int i;
			unsigned long long* array = *dma_mask_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned long long* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, unsigned long long);
			}

		}

	}

	{
		callee_unmarshal_kernel__pci_driver_probe__kobject__in(__pos, __msg, __ext, ctx, kobj_ptr);
	}

	{
	}
}

void callee_marshal_kernel__pci_driver_probe__device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct device const* ptr)
{
	unsigned long long* const* dma_mask_ptr = &ptr->dma_mask;
	struct kobject const* kobj_ptr = &ptr->kobj;

	{
		(void)dma_mask_ptr;
	}

	{
		callee_marshal_kernel__pci_driver_probe__kobject__in(__pos, __msg, __ext, ctx, kobj_ptr);
	}

}

void caller_unmarshal_kernel__pci_driver_probe__device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct device* ptr)
{
	unsigned long long** dma_mask_ptr = &ptr->dma_mask;
	struct kobject* kobj_ptr = &ptr->kobj;

	{
		(void)dma_mask_ptr;
	}

	{
		caller_unmarshal_kernel__pci_driver_probe__kobject__in(__pos, __msg, __ext, ctx, kobj_ptr);
	}

	{
	}
}

void caller_marshal_kernel__pci_driver_probe__kobject__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct kobject const* ptr)
{
	char const* const* name_ptr = &ptr->name;

	{
		__maybe_unused const void* __adjusted = *name_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*name_ptr) {
			size_t i, len;
			char const* array = *name_ptr;
			for (len = 0; array[len]; ++len);
			glue_pack(__pos, __msg, __ext, len + 1);
			for (i = 0; i < len; ++i) {
				char const* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

}

void callee_unmarshal_kernel__pci_driver_probe__kobject__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct kobject* ptr)
{
	char const** name_ptr = &ptr->name;

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*name_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, char const*, __size);
		if (*name_ptr) {
			char* writable = (char*)*name_ptr;
			size_t i, len;
			char* array = writable;
			len = glue_unpack(__pos, __msg, __ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	{
	}
}

void callee_marshal_kernel__pci_driver_probe__kobject__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct kobject const* ptr)
{
	char const* const* name_ptr = &ptr->name;

	{
		(void)name_ptr;
	}

}

void caller_unmarshal_kernel__pci_driver_probe__kobject__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct kobject* ptr)
{
	char const** name_ptr = &ptr->name;

	{
		(void)name_ptr;
	}

	{
	}
}

void caller_marshal_kernel__pci_driver_probe__resource__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct resource const* ptr)
{
	unsigned long long const* start_ptr = &ptr->start;
	unsigned long long const* end_ptr = &ptr->end;

	{
		glue_pack(__pos, __msg, __ext, *start_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

}

void callee_unmarshal_kernel__pci_driver_probe__resource__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct resource* ptr)
{
	unsigned long long* start_ptr = &ptr->start;
	unsigned long long* end_ptr = &ptr->end;

	{
		*start_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
	}
}

void callee_marshal_kernel__pci_driver_probe__resource__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct resource const* ptr)
{

}

void caller_unmarshal_kernel__pci_driver_probe__resource__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct resource* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_device_id const* ptr)
{
	unsigned long const* driver_data_ptr = &ptr->driver_data;

	{
		glue_pack(__pos, __msg, __ext, *driver_data_ptr);
	}

}

void callee_unmarshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_device_id* ptr)
{
	unsigned long* driver_data_ptr = &ptr->driver_data;

	{
		*driver_data_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void callee_marshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_device_id const* ptr)
{

}

void caller_unmarshal_kernel__pci_driver_probe__ent__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_probe_call_ctx const* ctx,
	struct pci_device_id* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_put_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	struct kref const* kref_ptr = &ptr->kref;

	{
		caller_marshal_kernel__nvme_put_ctrl__kref__in(__pos, __msg, __ext, ctx, kref_ptr);
	}

}

void callee_unmarshal_kernel__nvme_put_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	struct kref* kref_ptr = &ptr->kref;

	{
		callee_unmarshal_kernel__nvme_put_ctrl__kref__in(__pos, __msg, __ext, ctx, kref_ptr);
	}

	{
	}
}

void callee_marshal_kernel__nvme_put_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	struct kref const* kref_ptr = &ptr->kref;

	{
		callee_marshal_kernel__nvme_put_ctrl__kref__in(__pos, __msg, __ext, ctx, kref_ptr);
	}

}

void caller_unmarshal_kernel__nvme_put_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	struct kref* kref_ptr = &ptr->kref;

	{
		caller_unmarshal_kernel__nvme_put_ctrl__kref__in(__pos, __msg, __ext, ctx, kref_ptr);
	}

	{
	}
}

void caller_marshal_kernel__nvme_put_ctrl__kref__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* ctx,
	struct kref const* ptr)
{

}

void callee_unmarshal_kernel__nvme_put_ctrl__kref__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* ctx,
	struct kref* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_put_ctrl__kref__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* ctx,
	struct kref const* ptr)
{

}

void caller_unmarshal_kernel__nvme_put_ctrl__kref__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_put_ctrl_call_ctx const* ctx,
	struct kref* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_queue_async_events__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_queue_async_events_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void callee_unmarshal_kernel__nvme_queue_async_events__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_queue_async_events_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_queue_async_events__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_queue_async_events_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void caller_unmarshal_kernel__nvme_queue_async_events__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_queue_async_events_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_ops_queue_rq__hctx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{
	struct request_queue* const* queue_ptr = &ptr->queue;

	{
		__maybe_unused const void* __adjusted = *queue_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*queue_ptr) {
			caller_marshal_kernel__blk_mq_ops_queue_rq__blk_mq_hw_ctx_queue__in(__pos, __msg, __ext, ctx, *queue_ptr);
		}

	}

}

void callee_unmarshal_kernel__blk_mq_ops_queue_rq__hctx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{
	struct request_queue** queue_ptr = &ptr->queue;

	{
		*queue_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request_queue*);
		if (*queue_ptr) {
			callee_unmarshal_kernel__blk_mq_ops_queue_rq__blk_mq_hw_ctx_queue__in(__pos, __msg, __ext, ctx, *queue_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__blk_mq_ops_queue_rq__hctx__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{
	struct request_queue* const* queue_ptr = &ptr->queue;

	{
		if (*queue_ptr) {
			callee_marshal_kernel__blk_mq_ops_queue_rq__blk_mq_hw_ctx_queue__in(__pos, __msg, __ext, ctx, *queue_ptr);
		}

	}

}

void caller_unmarshal_kernel__blk_mq_ops_queue_rq__hctx__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{
	struct request_queue** queue_ptr = &ptr->queue;

	{
		if (*queue_ptr) {
			caller_unmarshal_kernel__blk_mq_ops_queue_rq__blk_mq_hw_ctx_queue__in(__pos, __msg, __ext, ctx, *queue_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__blk_mq_ops_queue_rq__blk_mq_hw_ctx_queue__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_ops_queue_rq__blk_mq_hw_ctx_queue__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_ops_queue_rq__blk_mq_hw_ctx_queue__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_ops_queue_rq__blk_mq_hw_ctx_queue__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_ops_queue_rq__bd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct blk_mq_queue_data const* ptr)
{
	struct request* const* rq_ptr = &ptr->rq;

	{
		__maybe_unused const void* __adjusted = *rq_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*rq_ptr) {
			caller_marshal_kernel__blk_mq_ops_queue_rq__blk_mq_queue_data_rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

}

void callee_unmarshal_kernel__blk_mq_ops_queue_rq__bd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct blk_mq_queue_data* ptr)
{
	struct request** rq_ptr = &ptr->rq;

	{
		*rq_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct request*);
		if (*rq_ptr) {
			callee_unmarshal_kernel__blk_mq_ops_queue_rq__blk_mq_queue_data_rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__blk_mq_ops_queue_rq__bd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct blk_mq_queue_data const* ptr)
{
	struct request* const* rq_ptr = &ptr->rq;

	{
		if (*rq_ptr) {
			callee_marshal_kernel__blk_mq_ops_queue_rq__blk_mq_queue_data_rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

}

void caller_unmarshal_kernel__blk_mq_ops_queue_rq__bd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct blk_mq_queue_data* ptr)
{
	struct request** rq_ptr = &ptr->rq;

	{
		if (*rq_ptr) {
			caller_unmarshal_kernel__blk_mq_ops_queue_rq__blk_mq_queue_data_rq__in(__pos, __msg, __ext, ctx, *rq_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__blk_mq_ops_queue_rq__blk_mq_queue_data_rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct request const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_ops_queue_rq__blk_mq_queue_data_rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct request* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_ops_queue_rq__blk_mq_queue_data_rq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct request const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_ops_queue_rq__blk_mq_queue_data_rq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_queue_rq_call_ctx const* ctx,
	struct request* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_queue_scan__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_queue_scan_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	unsigned int const* state_ptr = &ptr->state;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

}

void callee_unmarshal_kernel__nvme_queue_scan__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_queue_scan_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	unsigned int* state_ptr = &ptr->state;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__nvme_queue_scan__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_queue_scan_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void caller_unmarshal_kernel__nvme_queue_scan__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_queue_scan_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_remove_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_remove_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_remove_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_driver_remove__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_remove_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_remove_namespaces__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_remove_namespaces_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	unsigned int const* state_ptr = &ptr->state;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

}

void callee_unmarshal_kernel__nvme_remove_namespaces__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_remove_namespaces_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	unsigned int* state_ptr = &ptr->state;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__nvme_remove_namespaces__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_remove_namespaces_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void caller_unmarshal_kernel__nvme_remove_namespaces__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_remove_namespaces_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_requeue_req__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* ctx,
	struct request const* ptr)
{
	struct request_queue* const* q_ptr = &ptr->q;
	unsigned long long const* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int const* __data_len_ptr = &ptr->__data_len;
	unsigned short const* nr_phys_segments_ptr = &ptr->nr_phys_segments;

	{
		__maybe_unused const void* __adjusted = *q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*q_ptr) {
			caller_marshal_kernel__nvme_requeue_req__request_q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *nr_phys_segments_ptr);
	}

}

void callee_unmarshal_kernel__nvme_requeue_req__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* ctx,
	struct request* ptr)
{
	struct request_queue** q_ptr = &ptr->q;
	unsigned long long* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int* __data_len_ptr = &ptr->__data_len;
	unsigned short* nr_phys_segments_ptr = &ptr->nr_phys_segments;

	{
		*q_ptr = glue_unpack(__pos, __msg, __ext, struct request_queue*);
		if (*q_ptr) {
			callee_unmarshal_kernel__nvme_requeue_req__request_q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*cmd_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*__data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*nr_phys_segments_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
	}
}

void callee_marshal_kernel__nvme_requeue_req__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* ctx,
	struct request const* ptr)
{
	struct request_queue* const* q_ptr = &ptr->q;
	unsigned long long const* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned short const* nr_phys_segments_ptr = &ptr->nr_phys_segments;

	{
		if (*q_ptr) {
			callee_marshal_kernel__nvme_requeue_req__request_q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *nr_phys_segments_ptr);
	}

}

void caller_unmarshal_kernel__nvme_requeue_req__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* ctx,
	struct request* ptr)
{
	struct request_queue** q_ptr = &ptr->q;
	unsigned long long* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned short* nr_phys_segments_ptr = &ptr->nr_phys_segments;

	{
		if (*q_ptr) {
			caller_unmarshal_kernel__nvme_requeue_req__request_q__in(__pos, __msg, __ext, ctx, *q_ptr);
		}

	}

	{
		*cmd_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*nr_phys_segments_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
	}
}

void caller_marshal_kernel__nvme_requeue_req__request_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__nvme_requeue_req__request_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_requeue_req__request_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__nvme_requeue_req__request_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_requeue_req_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_error_handlers_reset_notify__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_reset_notify_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_error_handlers_reset_notify__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_reset_notify_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_error_handlers_reset_notify__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_reset_notify_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_error_handlers_reset_notify__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_reset_notify_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_set_queue_count__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	struct request_queue* const* admin_q_ptr = &ptr->admin_q;

	{
		__maybe_unused const void* __adjusted = *admin_q_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*admin_q_ptr) {
			caller_marshal_kernel__nvme_set_queue_count__nvme_ctrl_admin_q__in(__pos, __msg, __ext, ctx, *admin_q_ptr);
		}

	}

}

void callee_unmarshal_kernel__nvme_set_queue_count__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	struct request_queue** admin_q_ptr = &ptr->admin_q;

	{
		*admin_q_ptr = glue_unpack(__pos, __msg, __ext, struct request_queue*);
		if (*admin_q_ptr) {
			callee_unmarshal_kernel__nvme_set_queue_count__nvme_ctrl_admin_q__in(__pos, __msg, __ext, ctx, *admin_q_ptr);
		}

	}

	{
	}
}

void callee_marshal_kernel__nvme_set_queue_count__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	struct request_queue* const* admin_q_ptr = &ptr->admin_q;

	{
		if (*admin_q_ptr) {
			callee_marshal_kernel__nvme_set_queue_count__nvme_ctrl_admin_q__in(__pos, __msg, __ext, ctx, *admin_q_ptr);
		}

	}

}

void caller_unmarshal_kernel__nvme_set_queue_count__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	struct request_queue** admin_q_ptr = &ptr->admin_q;

	{
		if (*admin_q_ptr) {
			caller_unmarshal_kernel__nvme_set_queue_count__nvme_ctrl_admin_q__in(__pos, __msg, __ext, ctx, *admin_q_ptr);
		}

	}

	{
	}
}

void caller_marshal_kernel__nvme_set_queue_count__nvme_ctrl_admin_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__nvme_set_queue_count__nvme_ctrl_admin_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_set_queue_count__nvme_ctrl_admin_q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__nvme_set_queue_count__nvme_ctrl_admin_q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_set_queue_count_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_setup_cmd__ns__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct nvme_ns const* ptr)
{
	int const* lba_shift_ptr = &ptr->lba_shift;
	unsigned short const* ms_ptr = &ptr->ms;
	unsigned char const* pi_type_ptr = &ptr->pi_type;

	{
		glue_pack(__pos, __msg, __ext, *lba_shift_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ms_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *pi_type_ptr);
	}

}

void callee_unmarshal_kernel__nvme_setup_cmd__ns__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct nvme_ns* ptr)
{
	int* lba_shift_ptr = &ptr->lba_shift;
	unsigned short* ms_ptr = &ptr->ms;
	unsigned char* pi_type_ptr = &ptr->pi_type;

	{
		*lba_shift_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*ms_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*pi_type_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
	}

	{
	}
}

void callee_marshal_kernel__nvme_setup_cmd__ns__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct nvme_ns const* ptr)
{

}

void caller_unmarshal_kernel__nvme_setup_cmd__ns__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct nvme_ns* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_setup_cmd__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct request const* ptr)
{
	unsigned int const* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long const* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int const* __data_len_ptr = &ptr->__data_len;
	unsigned long const* __sector_ptr = &ptr->__sector;
	struct bio* const* bio_ptr = &ptr->bio;
	int const* tag_ptr = &ptr->tag;

	{
		glue_pack(__pos, __msg, __ext, *cmd_type_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_flags_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__data_len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__sector_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *bio_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*bio_ptr) {
			caller_marshal_kernel__nvme_setup_cmd__request_bio__in(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tag_ptr);
	}

}

void callee_unmarshal_kernel__nvme_setup_cmd__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct request* ptr)
{
	unsigned int* cmd_type_ptr = &ptr->cmd_type;
	unsigned long long* cmd_flags_ptr = &ptr->cmd_flags;
	unsigned int* __data_len_ptr = &ptr->__data_len;
	unsigned long* __sector_ptr = &ptr->__sector;
	struct bio** bio_ptr = &ptr->bio;
	int* tag_ptr = &ptr->tag;

	{
		*cmd_type_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*cmd_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long long);
	}

	{
		*__data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__sector_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*bio_ptr = glue_unpack(__pos, __msg, __ext, struct bio*);
		if (*bio_ptr) {
			callee_unmarshal_kernel__nvme_setup_cmd__request_bio__in(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		*tag_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__nvme_setup_cmd__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct request const* ptr)
{
	unsigned int const* __data_len_ptr = &ptr->__data_len;
	struct bio* const* bio_ptr = &ptr->bio;
	unsigned short const* nr_phys_segments_ptr = &ptr->nr_phys_segments;

	{
		glue_pack(__pos, __msg, __ext, *__data_len_ptr);
	}

	{
		if (*bio_ptr) {
			callee_marshal_kernel__nvme_setup_cmd__request_bio__in(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *nr_phys_segments_ptr);
	}

}

void caller_unmarshal_kernel__nvme_setup_cmd__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct request* ptr)
{
	unsigned int* __data_len_ptr = &ptr->__data_len;
	struct bio** bio_ptr = &ptr->bio;
	unsigned short* nr_phys_segments_ptr = &ptr->nr_phys_segments;

	{
		*__data_len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		if (*bio_ptr) {
			caller_unmarshal_kernel__nvme_setup_cmd__request_bio__in(__pos, __msg, __ext, ctx, *bio_ptr);
		}

	}

	{
		*nr_phys_segments_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
	}
}

void caller_marshal_kernel__nvme_setup_cmd__request_bio__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct bio const* ptr)
{

}

void callee_unmarshal_kernel__nvme_setup_cmd__request_bio__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct bio* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_setup_cmd__request_bio__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct bio const* ptr)
{

}

void caller_unmarshal_kernel__nvme_setup_cmd__request_bio__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct bio* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_setup_cmd__cmd__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct nvme_command const* ptr)
{

}

void callee_unmarshal_kernel__nvme_setup_cmd__cmd__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct nvme_command* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_setup_cmd__cmd__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct nvme_command const* ptr)
{

}

void caller_unmarshal_kernel__nvme_setup_cmd__cmd__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_setup_cmd_call_ctx const* ctx,
	struct nvme_command* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_shutdown_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_shutdown_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_driver_shutdown_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_driver_shutdown__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_driver_shutdown_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_shutdown_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_shutdown_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	unsigned int const* ctrl_config_ptr = &ptr->ctrl_config;

	{
		glue_pack(__pos, __msg, __ext, *ctrl_config_ptr);
	}

}

void callee_unmarshal_kernel__nvme_shutdown_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_shutdown_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	unsigned int* ctrl_config_ptr = &ptr->ctrl_config;

	{
		*ctrl_config_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__nvme_shutdown_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_shutdown_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	unsigned int const* ctrl_config_ptr = &ptr->ctrl_config;

	{
		glue_pack(__pos, __msg, __ext, *ctrl_config_ptr);
	}

}

void caller_unmarshal_kernel__nvme_shutdown_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_shutdown_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	unsigned int* ctrl_config_ptr = &ptr->ctrl_config;

	{
		*ctrl_config_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__pci_error_handlers_slot_reset__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_slot_reset_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_error_handlers_slot_reset__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_slot_reset_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_error_handlers_slot_reset__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_error_handlers_slot_reset_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_error_handlers_slot_reset__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_error_handlers_slot_reset_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_start_queues__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_start_queues_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void callee_unmarshal_kernel__nvme_start_queues__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_start_queues_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_start_queues__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_start_queues_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void caller_unmarshal_kernel__nvme_start_queues__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_start_queues_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_stop_queues__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_stop_queues_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void callee_unmarshal_kernel__nvme_stop_queues__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_stop_queues_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_stop_queues__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_stop_queues_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void caller_unmarshal_kernel__nvme_stop_queues__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_stop_queues_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_submit_sync_cmd__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	unsigned long const* queue_flags_ptr = &ptr->queue_flags;

	{
		glue_pack(__pos, __msg, __ext, *queue_flags_ptr);
	}

}

void callee_unmarshal_kernel__nvme_submit_sync_cmd__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* ctx,
	struct request_queue* ptr)
{
	unsigned long* queue_flags_ptr = &ptr->queue_flags;

	{
		*queue_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
	}
}

void callee_marshal_kernel__nvme_submit_sync_cmd__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__nvme_submit_sync_cmd__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__nvme_submit_sync_cmd__cmd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* ctx,
	struct nvme_command const* ptr)
{

}

void callee_unmarshal_kernel__nvme_submit_sync_cmd__cmd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* ctx,
	struct nvme_command* ptr)
{

	{
	}
}

void callee_marshal_kernel__nvme_submit_sync_cmd__cmd__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* ctx,
	struct nvme_command const* ptr)
{

}

void caller_unmarshal_kernel__nvme_submit_sync_cmd__cmd__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_submit_sync_cmd_call_ctx const* ctx,
	struct nvme_command* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_ops_timeout__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_timeout_call_ctx const* ctx,
	struct request const* ptr)
{
	int const* tag_ptr = &ptr->tag;

	{
		glue_pack(__pos, __msg, __ext, *tag_ptr);
	}

}

void callee_unmarshal_kernel__blk_mq_ops_timeout__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_timeout_call_ctx const* ctx,
	struct request* ptr)
{
	int* tag_ptr = &ptr->tag;

	{
		*tag_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__blk_mq_ops_timeout__req__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_ops_timeout_call_ctx const* ctx,
	struct request const* ptr)
{
	int const* errors_ptr = &ptr->errors;

	{
		glue_pack(__pos, __msg, __ext, *errors_ptr);
	}

}

void caller_unmarshal_kernel__blk_mq_ops_timeout__req__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_ops_timeout_call_ctx const* ctx,
	struct request* ptr)
{
	int* errors_ptr = &ptr->errors;

	{
		*errors_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void caller_marshal_kernel__nvme_uninit_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_uninit_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{
	unsigned int const* state_ptr = &ptr->state;
	int const* instance_ptr = &ptr->instance;

	{
		glue_pack(__pos, __msg, __ext, *state_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *instance_ptr);
	}

}

void callee_unmarshal_kernel__nvme_uninit_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_uninit_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{
	unsigned int* state_ptr = &ptr->state;
	int* instance_ptr = &ptr->instance;

	{
		*state_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*instance_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
	}
}

void callee_marshal_kernel__nvme_uninit_ctrl__ctrl__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct nvme_uninit_ctrl_call_ctx const* ctx,
	struct nvme_ctrl const* ptr)
{

}

void caller_unmarshal_kernel__nvme_uninit_ctrl__ctrl__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct nvme_uninit_ctrl_call_ctx const* ctx,
	struct nvme_ctrl* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_device_is_present__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_device_is_present_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_device_is_present__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_device_is_present_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_device_is_present__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_device_is_present_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_device_is_present__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_device_is_present_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct atomic64_t const* enable_cnt_ptr = &ptr->enable_cnt;

	{
		caller_marshal_kernel__pci_disable_device__atomic64_t__in(__pos, __msg, __ext, ctx, enable_cnt_ptr);
	}

}

void callee_unmarshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct atomic64_t* enable_cnt_ptr = &ptr->enable_cnt;

	{
		callee_unmarshal_kernel__pci_disable_device__atomic64_t__in(__pos, __msg, __ext, ctx, enable_cnt_ptr);
	}

	{
	}
}

void callee_marshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct atomic64_t const* enable_cnt_ptr = &ptr->enable_cnt;

	{
		callee_marshal_kernel__pci_disable_device__atomic64_t__in(__pos, __msg, __ext, ctx, enable_cnt_ptr);
	}

}

void caller_unmarshal_kernel__pci_disable_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct atomic64_t* enable_cnt_ptr = &ptr->enable_cnt;

	{
		caller_unmarshal_kernel__pci_disable_device__atomic64_t__in(__pos, __msg, __ext, ctx, enable_cnt_ptr);
	}

	{
	}
}

void caller_marshal_kernel__pci_disable_device__atomic64_t__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct atomic64_t const* ptr)
{
	long const* counter_ptr = &ptr->counter;

	{
		glue_pack(__pos, __msg, __ext, *counter_ptr);
	}

}

void callee_unmarshal_kernel__pci_disable_device__atomic64_t__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct atomic64_t* ptr)
{
	long* counter_ptr = &ptr->counter;

	{
		*counter_ptr = glue_unpack(__pos, __msg, __ext, long);
	}

	{
	}
}

void callee_marshal_kernel__pci_disable_device__atomic64_t__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct atomic64_t const* ptr)
{

}

void caller_unmarshal_kernel__pci_disable_device__atomic64_t__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_device_call_ctx const* ctx,
	struct atomic64_t* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int const* irq_ptr = &ptr->irq;
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int const* __msi_enabled_ptr = &__msi_enabled;

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__msi_enabled_ptr);
	}

}

void callee_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int* irq_ptr = &ptr->irq;
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int* __msi_enabled_ptr = &__msi_enabled;

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__msi_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->msi_enabled = *__msi_enabled_ptr;
	}
}

void callee_marshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int const* irq_ptr = &ptr->irq;
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int const* __msi_enabled_ptr = &__msi_enabled;

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__msi_enabled_ptr);
	}

}

void caller_unmarshal_kernel__pci_disable_msi__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msi_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int* irq_ptr = &ptr->irq;
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int* __msi_enabled_ptr = &__msi_enabled;

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__msi_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->msi_enabled = *__msi_enabled_ptr;
	}
}

void caller_marshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int const* __msix_enabled_ptr = &__msix_enabled;

	{
		glue_pack(__pos, __msg, __ext, *__msix_enabled_ptr);
	}

}

void callee_unmarshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int* __msix_enabled_ptr = &__msix_enabled;

	{
		*__msix_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->msix_enabled = *__msix_enabled_ptr;
	}
}

void callee_marshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int const* __msix_enabled_ptr = &__msix_enabled;

	{
		glue_pack(__pos, __msg, __ext, *__msix_enabled_ptr);
	}

}

void caller_unmarshal_kernel__pci_disable_msix__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_msix_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int* __msix_enabled_ptr = &__msix_enabled;

	{
		*__msix_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->msix_enabled = *__msix_enabled_ptr;
	}
}

void caller_marshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct atomic64_t const* enable_cnt_ptr = &ptr->enable_cnt;

	{
		caller_marshal_kernel__pci_enable_device_mem__atomic64_t__io(__pos, __msg, __ext, ctx, enable_cnt_ptr);
	}

}

void callee_unmarshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct atomic64_t* enable_cnt_ptr = &ptr->enable_cnt;

	{
		callee_unmarshal_kernel__pci_enable_device_mem__atomic64_t__io(__pos, __msg, __ext, ctx, enable_cnt_ptr);
	}

	{
	}
}

void callee_marshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	struct atomic64_t const* enable_cnt_ptr = &ptr->enable_cnt;

	{
		callee_marshal_kernel__pci_enable_device_mem__atomic64_t__io(__pos, __msg, __ext, ctx, enable_cnt_ptr);
	}

}

void caller_unmarshal_kernel__pci_enable_device_mem__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	struct atomic64_t* enable_cnt_ptr = &ptr->enable_cnt;

	{
		caller_unmarshal_kernel__pci_enable_device_mem__atomic64_t__io(__pos, __msg, __ext, ctx, enable_cnt_ptr);
	}

	{
	}
}

void caller_marshal_kernel__pci_enable_device_mem__atomic64_t__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct atomic64_t const* ptr)
{
	long const* counter_ptr = &ptr->counter;

	{
		glue_pack(__pos, __msg, __ext, *counter_ptr);
	}

}

void callee_unmarshal_kernel__pci_enable_device_mem__atomic64_t__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct atomic64_t* ptr)
{
	long* counter_ptr = &ptr->counter;

	{
		*counter_ptr = glue_unpack(__pos, __msg, __ext, long);
	}

	{
	}
}

void callee_marshal_kernel__pci_enable_device_mem__atomic64_t__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct atomic64_t const* ptr)
{
	long const* counter_ptr = &ptr->counter;

	{
		glue_pack(__pos, __msg, __ext, *counter_ptr);
	}

}

void caller_unmarshal_kernel__pci_enable_device_mem__atomic64_t__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_device_mem_call_ctx const* ctx,
	struct atomic64_t* ptr)
{
	long* counter_ptr = &ptr->counter;

	{
		*counter_ptr = glue_unpack(__pos, __msg, __ext, long);
	}

	{
	}
}

void caller_marshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int const* __msi_enabled_ptr = &__msi_enabled;
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int const* __msix_enabled_ptr = &__msix_enabled;
	unsigned int const* irq_ptr = &ptr->irq;

	{
		glue_pack(__pos, __msg, __ext, *__msi_enabled_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__msix_enabled_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *irq_ptr);
	}

}

void caller_unmarshal_kernel__pci_enable_msi_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msi_range_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int* __msi_enabled_ptr = &__msi_enabled;
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int* __msix_enabled_ptr = &__msix_enabled;
	unsigned int* irq_ptr = &ptr->irq;

	{
		*__msi_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__msix_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*irq_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->msi_enabled = *__msi_enabled_ptr;
		ptr->msix_enabled = *__msix_enabled_ptr;
	}
}

void caller_marshal_kernel__pci_enable_msix__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_enable_msix__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_enable_msix__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int const* __msi_enabled_ptr = &__msi_enabled;
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int const* __msix_enabled_ptr = &__msix_enabled;

	{
		glue_pack(__pos, __msg, __ext, *__msi_enabled_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__msix_enabled_ptr);
	}

}

void caller_unmarshal_kernel__pci_enable_msix__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int* __msi_enabled_ptr = &__msi_enabled;
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int* __msix_enabled_ptr = &__msix_enabled;

	{
		*__msi_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__msix_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->msi_enabled = *__msi_enabled_ptr;
		ptr->msix_enabled = *__msix_enabled_ptr;
	}
}

void caller_marshal_kernel__pci_enable_msix__entries__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* ctx,
	struct msix_entry const* ptr)
{
	unsigned short const* entry_ptr = &ptr->entry;

	{
		glue_pack(__pos, __msg, __ext, *entry_ptr);
	}

}

void callee_unmarshal_kernel__pci_enable_msix__entries__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* ctx,
	struct msix_entry* ptr)
{
	unsigned short* entry_ptr = &ptr->entry;

	{
		*entry_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
	}
}

void callee_marshal_kernel__pci_enable_msix__entries__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* ctx,
	struct msix_entry const* ptr)
{
	unsigned int const* vector_ptr = &ptr->vector;
	unsigned short const* entry_ptr = &ptr->entry;

	{
		glue_pack(__pos, __msg, __ext, *vector_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *entry_ptr);
	}

}

void caller_unmarshal_kernel__pci_enable_msix__entries__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_call_ctx const* ctx,
	struct msix_entry* ptr)
{
	unsigned int* vector_ptr = &ptr->vector;
	unsigned short* entry_ptr = &ptr->entry;

	{
		*vector_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*entry_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
	}
}

void caller_marshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct pci_dev const* ptr)
{
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int const* __msi_enabled_ptr = &__msi_enabled;
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int const* __msix_enabled_ptr = &__msix_enabled;

	{
		glue_pack(__pos, __msg, __ext, *__msi_enabled_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *__msix_enabled_ptr);
	}

}

void caller_unmarshal_kernel__pci_enable_msix_range__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct pci_dev* ptr)
{
	unsigned int __msi_enabled = ptr->msi_enabled;
	unsigned int* __msi_enabled_ptr = &__msi_enabled;
	unsigned int __msix_enabled = ptr->msix_enabled;
	unsigned int* __msix_enabled_ptr = &__msix_enabled;

	{
		*__msi_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*__msix_enabled_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		ptr->msi_enabled = *__msi_enabled_ptr;
		ptr->msix_enabled = *__msix_enabled_ptr;
	}
}

void caller_marshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct msix_entry const* ptr)
{
	unsigned short const* entry_ptr = &ptr->entry;

	{
		glue_pack(__pos, __msg, __ext, *entry_ptr);
	}

}

void callee_unmarshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct msix_entry* ptr)
{
	unsigned short* entry_ptr = &ptr->entry;

	{
		*entry_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
	}
}

void callee_marshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct msix_entry const* ptr)
{
	unsigned int const* vector_ptr = &ptr->vector;
	unsigned short const* entry_ptr = &ptr->entry;

	{
		glue_pack(__pos, __msg, __ext, *vector_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *entry_ptr);
	}

}

void caller_unmarshal_kernel__pci_enable_msix_range__entries__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_msix_range_call_ctx const* ctx,
	struct msix_entry* ptr)
{
	unsigned int* vector_ptr = &ptr->vector;
	unsigned short* entry_ptr = &ptr->entry;

	{
		*vector_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*entry_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
	}
}

void caller_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_enable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_enable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_disable_pcie_error_reporting__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_disable_pcie_error_reporting_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_release_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_release_selected_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_request_selected_regions__pdev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_request_selected_regions_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_restore_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_restore_state_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_save_state__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_save_state_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_select_bars__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_select_bars_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void callee_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev const* ptr)
{

}

void caller_unmarshal_kernel__pci_set_master__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_set_master_call_ctx const* ctx,
	struct pci_dev* ptr)
{

	{
	}
}

void caller_marshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* ctx,
	struct pci_driver const* ptr)
{

}

void callee_unmarshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* ctx,
	struct pci_driver* ptr)
{

	{
	}
}

void callee_marshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* ctx,
	struct pci_driver const* ptr)
{

}

void caller_unmarshal_kernel__pci_unregister_driver__drv__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct pci_unregister_driver_call_ctx const* ctx,
	struct pci_driver* ptr)
{

	{
	}
}

void caller_marshal_kernel__put_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct put_device_call_ctx const* ctx,
	struct device const* ptr)
{

}

void callee_unmarshal_kernel__put_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct put_device_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void callee_marshal_kernel__put_device__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct put_device_call_ctx const* ctx,
	struct device const* ptr)
{

}

void caller_unmarshal_kernel__put_device__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct put_device_call_ctx const* ctx,
	struct device* ptr)
{

	{
	}
}

void caller_marshal_kernel__work_fn__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct work_fn_call_ctx const* ctx,
	struct work_struct const* ptr)
{

}

void callee_unmarshal_kernel__work_fn__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct work_fn_call_ctx const* ctx,
	struct work_struct* ptr)
{

	{
	}
}

void callee_marshal_kernel__work_fn__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct work_fn_call_ctx const* ctx,
	struct work_struct const* ptr)
{

}

void caller_unmarshal_kernel__work_fn__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct work_fn_call_ctx const* ctx,
	struct work_struct* ptr)
{

	{
	}
}

void caller_marshal_kernel__lvd_queue_work__wq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_queue_work_call_ctx const* ctx,
	struct workqueue_struct const* ptr)
{

}

void callee_unmarshal_kernel__lvd_queue_work__wq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_queue_work_call_ctx const* ctx,
	struct workqueue_struct* ptr)
{

	{
	}
}

void callee_marshal_kernel__lvd_queue_work__wq__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_queue_work_call_ctx const* ctx,
	struct workqueue_struct const* ptr)
{

}

void caller_unmarshal_kernel__lvd_queue_work__wq__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_queue_work_call_ctx const* ctx,
	struct workqueue_struct* ptr)
{

	{
	}
}

void caller_marshal_kernel__lvd_queue_work__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_queue_work_call_ctx const* ctx,
	struct work_struct const* ptr)
{

}

void callee_unmarshal_kernel__lvd_queue_work__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_queue_work_call_ctx const* ctx,
	struct work_struct* ptr)
{

	{
	}
}

void callee_marshal_kernel__lvd_queue_work__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_queue_work_call_ctx const* ctx,
	struct work_struct const* ptr)
{

}

void caller_unmarshal_kernel__lvd_queue_work__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_queue_work_call_ctx const* ctx,
	struct work_struct* ptr)
{

	{
	}
}

void caller_marshal_kernel__wait_for_completion_io_timeout__x__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct wait_for_completion_io_timeout_call_ctx const* ctx,
	struct completion const* ptr)
{
	unsigned int const* done_ptr = &ptr->done;

	{
		glue_pack(__pos, __msg, __ext, *done_ptr);
	}

}

void callee_unmarshal_kernel__wait_for_completion_io_timeout__x__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct wait_for_completion_io_timeout_call_ctx const* ctx,
	struct completion* ptr)
{
	unsigned int* done_ptr = &ptr->done;

	{
		*done_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void callee_marshal_kernel__wait_for_completion_io_timeout__x__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct wait_for_completion_io_timeout_call_ctx const* ctx,
	struct completion const* ptr)
{
	unsigned int const* done_ptr = &ptr->done;

	{
		glue_pack(__pos, __msg, __ext, *done_ptr);
	}

}

void caller_unmarshal_kernel__wait_for_completion_io_timeout__x__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct wait_for_completion_io_timeout_call_ctx const* ctx,
	struct completion* ptr)
{
	unsigned int* done_ptr = &ptr->done;

	{
		*done_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
	}
}

void caller_marshal_kernel__work_busy__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct work_busy_call_ctx const* ctx,
	struct work_struct const* ptr)
{

}

void callee_unmarshal_kernel__work_busy__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct work_busy_call_ctx const* ctx,
	struct work_struct* ptr)
{

	{
	}
}

void callee_marshal_kernel__work_busy__work__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct work_busy_call_ctx const* ctx,
	struct work_struct const* ptr)
{

}

void caller_unmarshal_kernel__work_busy__work__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct work_busy_call_ctx const* ctx,
	struct work_struct* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{

	{
	}
}

void caller_marshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void callee_unmarshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void callee_marshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{

}

void caller_unmarshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{

	{
	}
}

void caller_marshal_kernel__wq__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct workqueue_struct const* ptr)
{

}

void callee_unmarshal_kernel__wq__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct workqueue_struct* ptr)
{

	{
	}
}

void callee_marshal_kernel__wq__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct workqueue_struct const* ptr)
{

}

void caller_unmarshal_kernel__wq__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct workqueue_struct* ptr)
{

	{
	}
}


#ifdef LCD_ISOLATE
__attribute__((weak)) void shared_mem_init(void) {
	LIBLCD_MSG("Weak shared_mem_init does nothing! Override if you want!");
}
#else
__attribute__((weak)) void shared_mem_init_callee(struct fipc_message *__msg, struct ext_registers* __ext) {
	LIBLCD_MSG("Weak shared_mem_init_callee does nothing! Override if you want!");
}
#endif	/* LCD_ISOLATE */

