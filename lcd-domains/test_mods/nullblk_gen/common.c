#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel__release__disk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct release_call_ctx const* ctx,
	struct gendisk const* ptr)
{
	
}

void callee_unmarshal_kernel__release__disk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct release_call_ctx const* ctx,
	struct gendisk* ptr)
{
	
}

void callee_marshal_kernel__release__disk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct release_call_ctx const* ctx,
	struct gendisk const* ptr)
{
	
}

void caller_unmarshal_kernel__release__disk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct release_call_ctx const* ctx,
	struct gendisk* ptr)
{
	
}

void caller_marshal_kernel__del_gendisk__disk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct del_gendisk_call_ctx const* ctx,
	struct gendisk const* ptr)
{
	struct hd_struct const* part0_ptr = &ptr->part0;
	
	{
		caller_marshal_kernel__del_gendisk__hd_struct__in(__pos, msg, ext, ctx, part0_ptr);
	}

}

void callee_unmarshal_kernel__del_gendisk__disk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct del_gendisk_call_ctx const* ctx,
	struct gendisk* ptr)
{
	struct hd_struct* part0_ptr = &ptr->part0;
	
	{
		callee_unmarshal_kernel__del_gendisk__hd_struct__in(__pos, msg, ext, ctx, part0_ptr);
	}

}

void callee_marshal_kernel__del_gendisk__disk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct del_gendisk_call_ctx const* ctx,
	struct gendisk const* ptr)
{
	struct hd_struct const* part0_ptr = &ptr->part0;
	
	{
		callee_marshal_kernel__del_gendisk__hd_struct__in(__pos, msg, ext, ctx, part0_ptr);
	}

}

void caller_unmarshal_kernel__del_gendisk__disk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct del_gendisk_call_ctx const* ctx,
	struct gendisk* ptr)
{
	struct hd_struct* part0_ptr = &ptr->part0;
	
	{
		caller_unmarshal_kernel__del_gendisk__hd_struct__in(__pos, msg, ext, ctx, part0_ptr);
	}

}

void caller_marshal_kernel__del_gendisk__hd_struct__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct del_gendisk_call_ctx const* ctx,
	struct hd_struct const* ptr)
{
	
}

void callee_unmarshal_kernel__del_gendisk__hd_struct__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct del_gendisk_call_ctx const* ctx,
	struct hd_struct* ptr)
{
	
}

void callee_marshal_kernel__del_gendisk__hd_struct__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct del_gendisk_call_ctx const* ctx,
	struct hd_struct const* ptr)
{
	unsigned long long const* nr_sects_ptr = &ptr->nr_sects;
	
	{
		glue_pack(__pos, msg, ext, *nr_sects_ptr);
	}

}

void caller_unmarshal_kernel__del_gendisk__hd_struct__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct del_gendisk_call_ctx const* ctx,
	struct hd_struct* ptr)
{
	unsigned long long* nr_sects_ptr = &ptr->nr_sects;
	
	{
		*nr_sects_ptr = glue_unpack(__pos, msg, ext, unsigned long long);
	}

}

void caller_marshal_kernel__device_add_disk__parent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void callee_unmarshal_kernel__device_add_disk__parent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void callee_marshal_kernel__device_add_disk__parent__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct device const* ptr)
{
	
}

void caller_unmarshal_kernel__device_add_disk__parent__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct device* ptr)
{
	
}

void caller_marshal_kernel__device_add_disk__disk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct gendisk const* ptr)
{
	int const* major_ptr = &ptr->major;
	int const* first_minor_ptr = &ptr->first_minor;
	char const* disk_name_ptr = ptr->disk_name;
	struct hd_struct const* part0_ptr = &ptr->part0;
	struct block_device_operations* const* fops_ptr = &ptr->fops;
	struct request_queue* const* queue_ptr = &ptr->queue;
	int const* flags_ptr = &ptr->flags;
	
	{
		glue_pack(__pos, msg, ext, *major_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *first_minor_ptr);
	}

	{
		size_t i, len = 32;
		char const* array = disk_name_ptr;
		glue_pack(__pos, msg, ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(__pos, msg, ext, *element);
		}

	}

	{
		caller_marshal_kernel__device_add_disk__hd_struct__in(__pos, msg, ext, ctx, part0_ptr);
	}

	{
		const void* __adjusted = *fops_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*fops_ptr) {
			caller_marshal_kernel__device_add_disk__fops__in(__pos, msg, ext, ctx, *fops_ptr);
		}

	}

	{
		const void* __adjusted = *queue_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*queue_ptr) {
			caller_marshal_kernel__device_add_disk__queue__in(__pos, msg, ext, ctx, *queue_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *flags_ptr);
	}

}

void callee_unmarshal_kernel__device_add_disk__disk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct gendisk* ptr)
{
	int* major_ptr = &ptr->major;
	int* first_minor_ptr = &ptr->first_minor;
	char* disk_name_ptr = ptr->disk_name;
	struct hd_struct* part0_ptr = &ptr->part0;
	struct block_device_operations** fops_ptr = &ptr->fops;
	struct request_queue** queue_ptr = &ptr->queue;
	int* flags_ptr = &ptr->flags;
	
	{
		*major_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*first_minor_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		int i;
		char* array = disk_name_ptr;
		size_t len = glue_unpack(__pos, msg, ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(__pos, msg, ext, char);
		}

	}

	{
		callee_unmarshal_kernel__device_add_disk__hd_struct__in(__pos, msg, ext, ctx, part0_ptr);
	}

	{
		*fops_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct block_device_operations*, sizeof(struct block_device_operations));
		if (*fops_ptr) {
			callee_unmarshal_kernel__device_add_disk__fops__in(__pos, msg, ext, ctx, *fops_ptr);
		}

	}

	{
		*queue_ptr = glue_unpack(__pos, msg, ext, struct request_queue*);
		if (*queue_ptr) {
			callee_unmarshal_kernel__device_add_disk__queue__in(__pos, msg, ext, ctx, *queue_ptr);
		}

	}

	{
		*flags_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void callee_marshal_kernel__device_add_disk__disk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct gendisk const* ptr)
{
	int const* major_ptr = &ptr->major;
	int const* first_minor_ptr = &ptr->first_minor;
	struct hd_struct const* part0_ptr = &ptr->part0;
	struct block_device_operations* const* fops_ptr = &ptr->fops;
	struct request_queue* const* queue_ptr = &ptr->queue;
	int const* flags_ptr = &ptr->flags;
	
	{
		glue_pack(__pos, msg, ext, *major_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *first_minor_ptr);
	}

	{
		callee_marshal_kernel__device_add_disk__hd_struct__in(__pos, msg, ext, ctx, part0_ptr);
	}

	{
		if (*fops_ptr) {
			callee_marshal_kernel__device_add_disk__fops__in(__pos, msg, ext, ctx, *fops_ptr);
		}

	}

	{
		if (*queue_ptr) {
			callee_marshal_kernel__device_add_disk__queue__in(__pos, msg, ext, ctx, *queue_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *flags_ptr);
	}

}

void caller_unmarshal_kernel__device_add_disk__disk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct gendisk* ptr)
{
	int* major_ptr = &ptr->major;
	int* first_minor_ptr = &ptr->first_minor;
	struct hd_struct* part0_ptr = &ptr->part0;
	struct block_device_operations** fops_ptr = &ptr->fops;
	struct request_queue** queue_ptr = &ptr->queue;
	int* flags_ptr = &ptr->flags;
	
	{
		*major_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*first_minor_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		caller_unmarshal_kernel__device_add_disk__hd_struct__in(__pos, msg, ext, ctx, part0_ptr);
	}

	{
		if (*fops_ptr) {
			caller_unmarshal_kernel__device_add_disk__fops__in(__pos, msg, ext, ctx, *fops_ptr);
		}

	}

	{
		if (*queue_ptr) {
			caller_unmarshal_kernel__device_add_disk__queue__in(__pos, msg, ext, ctx, *queue_ptr);
		}

	}

	{
		*flags_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void caller_marshal_kernel__device_add_disk__hd_struct__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct hd_struct const* ptr)
{
	
}

void callee_unmarshal_kernel__device_add_disk__hd_struct__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct hd_struct* ptr)
{
	
}

void callee_marshal_kernel__device_add_disk__hd_struct__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct hd_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__device_add_disk__hd_struct__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct hd_struct* ptr)
{
	
}

void caller_marshal_kernel__device_add_disk__fops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct block_device_operations const* ptr)
{
	fptr_open const* open_ptr = &ptr->open;
	fptr_release const* release_ptr = &ptr->release;
	
	{
		glue_pack(__pos, msg, ext, *open_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *release_ptr);
	}

}

void callee_unmarshal_kernel__device_add_disk__fops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct block_device_operations* ptr)
{
	fptr_open* open_ptr = &ptr->open;
	fptr_release* release_ptr = &ptr->release;
	
	{
		*open_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, open);
	}

	{
		*release_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, release);
	}

}

void callee_marshal_kernel__device_add_disk__fops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct block_device_operations const* ptr)
{
	
}

void caller_unmarshal_kernel__device_add_disk__fops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct block_device_operations* ptr)
{
	
}

void caller_marshal_kernel__device_add_disk__queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__device_add_disk__queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct request_queue* ptr)
{
	
}

void callee_marshal_kernel__device_add_disk__queue__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	
}

void caller_unmarshal_kernel__device_add_disk__queue__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct device_add_disk_call_ctx const* ctx,
	struct request_queue* ptr)
{
	
}

void caller_marshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_cleanup_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_cleanup_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{
	
}

void callee_marshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_cleanup_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	
}

void caller_unmarshal_kernel__blk_cleanup_queue__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_cleanup_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{
	
}

void caller_marshal_kernel__blk_mq_end_request__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct request const* ptr)
{
	int const* errors_ptr = &ptr->errors;
	
	{
		glue_pack(__pos, msg, ext, *errors_ptr);
	}

}

void callee_unmarshal_kernel__blk_mq_end_request__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct request* ptr)
{
	int* errors_ptr = &ptr->errors;
	
	{
		*errors_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void callee_marshal_kernel__blk_mq_end_request__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct request const* ptr)
{
	
}

void caller_unmarshal_kernel__blk_mq_end_request__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_end_request_call_ctx const* ctx,
	struct request* ptr)
{
	
}

void caller_marshal_kernel__complete__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct complete_call_ctx const* ctx,
	struct request const* ptr)
{
	
}

void callee_unmarshal_kernel__complete__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct complete_call_ctx const* ctx,
	struct request* ptr)
{
	
}

void callee_marshal_kernel__complete__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct complete_call_ctx const* ctx,
	struct request const* ptr)
{
	
}

void caller_unmarshal_kernel__complete__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct complete_call_ctx const* ctx,
	struct request* ptr)
{
	
}

void caller_marshal_kernel__open__bdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct open_call_ctx const* ctx,
	struct block_device const* ptr)
{
	
}

void callee_unmarshal_kernel__open__bdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct open_call_ctx const* ctx,
	struct block_device* ptr)
{
	
}

void callee_marshal_kernel__open__bdev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct open_call_ctx const* ctx,
	struct block_device const* ptr)
{
	
}

void caller_unmarshal_kernel__open__bdev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct open_call_ctx const* ctx,
	struct block_device* ptr)
{
	
}

void caller_marshal_kernel__alloc_disk_node__ret_gendisk__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* ctx,
	struct gendisk const* ptr)
{
	struct hd_struct const* part0_ptr = &ptr->part0;
	
	{
		caller_marshal_kernel__alloc_disk_node__hd_struct__out(__pos, msg, ext, ctx, part0_ptr);
	}

}

void callee_unmarshal_kernel__alloc_disk_node__ret_gendisk__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* ctx,
	struct gendisk* ptr)
{
	struct hd_struct* part0_ptr = &ptr->part0;
	
	{
		callee_unmarshal_kernel__alloc_disk_node__hd_struct__out(__pos, msg, ext, ctx, part0_ptr);
	}

}

void callee_marshal_kernel__alloc_disk_node__ret_gendisk__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* ctx,
	struct gendisk const* ptr)
{
	struct hd_struct const* part0_ptr = &ptr->part0;
	
	{
		callee_marshal_kernel__alloc_disk_node__hd_struct__out(__pos, msg, ext, ctx, part0_ptr);
	}

}

void caller_unmarshal_kernel__alloc_disk_node__ret_gendisk__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* ctx,
	struct gendisk* ptr)
{
	struct hd_struct* part0_ptr = &ptr->part0;
	
	{
		caller_unmarshal_kernel__alloc_disk_node__hd_struct__out(__pos, msg, ext, ctx, part0_ptr);
	}

}

void caller_marshal_kernel__alloc_disk_node__hd_struct__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* ctx,
	struct hd_struct const* ptr)
{
	
}

void callee_unmarshal_kernel__alloc_disk_node__hd_struct__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* ctx,
	struct hd_struct* ptr)
{
	
}

void callee_marshal_kernel__alloc_disk_node__hd_struct__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* ctx,
	struct hd_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__alloc_disk_node__hd_struct__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct alloc_disk_node_call_ctx const* ctx,
	struct hd_struct* ptr)
{
	
}

void caller_marshal_kernel__queue_rq__hctx__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_rq_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{
	
}

void callee_unmarshal_kernel__queue_rq__hctx__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_rq_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{
	
}

void callee_marshal_kernel__queue_rq__hctx__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_rq_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{
	
}

void caller_unmarshal_kernel__queue_rq__hctx__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_rq_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{
	
}

void caller_marshal_kernel__queue_rq__bd__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_rq_call_ctx const* ctx,
	struct blk_mq_queue_data const* ptr)
{
	struct request* const* rq_ptr = &ptr->rq;
	
	{
		const void* __adjusted = *rq_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*rq_ptr) {
			caller_marshal_kernel__queue_rq__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

}

void callee_unmarshal_kernel__queue_rq__bd__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_rq_call_ctx const* ctx,
	struct blk_mq_queue_data* ptr)
{
	struct request** rq_ptr = &ptr->rq;
	
	{
		*rq_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct request*, (sizeof(struct request)));
		if (*rq_ptr) {
			callee_unmarshal_kernel__queue_rq__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

}

void callee_marshal_kernel__queue_rq__bd__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_rq_call_ctx const* ctx,
	struct blk_mq_queue_data const* ptr)
{
	struct request* const* rq_ptr = &ptr->rq;
	
	{
		if (*rq_ptr) {
			callee_marshal_kernel__queue_rq__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

}

void caller_unmarshal_kernel__queue_rq__bd__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_rq_call_ctx const* ctx,
	struct blk_mq_queue_data* ptr)
{
	struct request** rq_ptr = &ptr->rq;
	
	{
		if (*rq_ptr) {
			caller_unmarshal_kernel__queue_rq__rq__in(__pos, msg, ext, ctx, *rq_ptr);
		}

	}

}

void caller_marshal_kernel__queue_rq__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_rq_call_ctx const* ctx,
	struct request const* ptr)
{
	
}

void callee_unmarshal_kernel__queue_rq__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_rq_call_ctx const* ctx,
	struct request* ptr)
{
	
}

void callee_marshal_kernel__queue_rq__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct queue_rq_call_ctx const* ctx,
	struct request const* ptr)
{
	
}

void caller_unmarshal_kernel__queue_rq__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct queue_rq_call_ctx const* ctx,
	struct request* ptr)
{
	
}

void caller_marshal_kernel__init_hctx__hctx__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct init_hctx_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{
	
}

void callee_unmarshal_kernel__init_hctx__hctx__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct init_hctx_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{
	
}

void callee_marshal_kernel__init_hctx__hctx__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct init_hctx_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{
	
}

void caller_unmarshal_kernel__init_hctx__hctx__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct init_hctx_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{
	
}

void caller_marshal_kernel__map_queue__blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{
	
}

void callee_unmarshal_kernel__map_queue__blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{
	
}

void callee_marshal_kernel__map_queue__blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{
	
}

void caller_unmarshal_kernel__map_queue__blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{
	
}

void caller_marshal_kernel__map_queue__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct map_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__map_queue__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct map_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{
	
}

void callee_marshal_kernel__map_queue__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct map_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	
}

void caller_unmarshal_kernel__map_queue__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct map_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{
	
}

void caller_marshal_kernel__set__kp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct set_call_ctx const* ctx,
	struct kernel_param const* ptr)
{
	
}

void callee_unmarshal_kernel__set__kp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct set_call_ctx const* ctx,
	struct kernel_param* ptr)
{
	
}

void callee_marshal_kernel__set__kp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct set_call_ctx const* ctx,
	struct kernel_param const* ptr)
{
	
}

void caller_unmarshal_kernel__set__kp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct set_call_ctx const* ctx,
	struct kernel_param* ptr)
{
	
}

void caller_marshal_kernel__param_get_int__kp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct param_get_int_call_ctx const* ctx,
	struct kernel_param const* ptr)
{
	
}

void callee_unmarshal_kernel__param_get_int__kp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct param_get_int_call_ctx const* ctx,
	struct kernel_param* ptr)
{
	
}

void callee_marshal_kernel__param_get_int__kp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct param_get_int_call_ctx const* ctx,
	struct kernel_param const* ptr)
{
	
}

void caller_unmarshal_kernel__param_get_int__kp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct param_get_int_call_ctx const* ctx,
	struct kernel_param* ptr)
{
	
}

void caller_marshal_kernel__prepare_to_wait__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void callee_unmarshal_kernel__prepare_to_wait__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void callee_marshal_kernel__prepare_to_wait__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void caller_unmarshal_kernel__prepare_to_wait__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void caller_marshal_kernel__prepare_to_wait__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* ctx,
	struct __wait_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__prepare_to_wait__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* ctx,
	struct __wait_queue* ptr)
{
	
}

void callee_marshal_kernel__prepare_to_wait__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* ctx,
	struct __wait_queue const* ptr)
{
	unsigned int const* flags_ptr = &ptr->flags;
	
	{
		glue_pack(__pos, msg, ext, *flags_ptr);
	}

}

void caller_unmarshal_kernel__prepare_to_wait__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct prepare_to_wait_call_ctx const* ctx,
	struct __wait_queue* ptr)
{
	unsigned int* flags_ptr = &ptr->flags;
	
	{
		*flags_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

}

void caller_marshal_kernel__finish_wait__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void callee_unmarshal_kernel__finish_wait__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void callee_marshal_kernel__finish_wait__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue_head const* ptr)
{
	
}

void caller_unmarshal_kernel__finish_wait__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue_head* ptr)
{
	
}

void caller_marshal_kernel__finish_wait__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__finish_wait__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue* ptr)
{
	
}

void callee_marshal_kernel__finish_wait__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue const* ptr)
{
	
}

void caller_unmarshal_kernel__finish_wait__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct finish_wait_call_ctx const* ctx,
	struct __wait_queue* ptr)
{
	
}

void caller_marshal_kernel__autoremove_wake_function__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct autoremove_wake_function_call_ctx const* ctx,
	struct __wait_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__autoremove_wake_function__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct autoremove_wake_function_call_ctx const* ctx,
	struct __wait_queue* ptr)
{
	
}

void callee_marshal_kernel__autoremove_wake_function__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct autoremove_wake_function_call_ctx const* ctx,
	struct __wait_queue const* ptr)
{
	
}

void caller_unmarshal_kernel__autoremove_wake_function__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct autoremove_wake_function_call_ctx const* ctx,
	struct __wait_queue* ptr)
{
	
}

void caller_marshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_free_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{
	
}

void callee_unmarshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_free_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{
	
}

void callee_marshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_free_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{
	
}

void caller_unmarshal_kernel__blk_mq_free_tag_set__set__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_free_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{
	
}

void caller_marshal_kernel__blk_mq_alloc_tag_set__set__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_alloc_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{
	struct blk_mq_ops* const* ops_ptr = &ptr->ops;
	unsigned int const* nr_hw_queues_ptr = &ptr->nr_hw_queues;
	unsigned int const* queue_depth_ptr = &ptr->queue_depth;
	unsigned int const* cmd_size_ptr = &ptr->cmd_size;
	unsigned int const* flags_ptr = &ptr->flags;
	int const* numa_node_ptr = &ptr->numa_node;
	void* const* driver_data_ptr = &ptr->driver_data;
	
	{
		const void* __adjusted = *ops_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*ops_ptr) {
			caller_marshal_kernel___global_blk_mq_ops__in(__pos, msg, ext, *ops_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *nr_hw_queues_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *queue_depth_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *cmd_size_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *flags_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *numa_node_ptr);
	}

	{
		const void* __adjusted = *driver_data_ptr;
		glue_pack(__pos, msg, ext, __adjusted);
		if (*driver_data_ptr) {
		}

	}

}

void callee_unmarshal_kernel__blk_mq_alloc_tag_set__set__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_alloc_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{
	struct blk_mq_ops** ops_ptr = &ptr->ops;
	unsigned int* nr_hw_queues_ptr = &ptr->nr_hw_queues;
	unsigned int* queue_depth_ptr = &ptr->queue_depth;
	unsigned int* cmd_size_ptr = &ptr->cmd_size;
	unsigned int* flags_ptr = &ptr->flags;
	int* numa_node_ptr = &ptr->numa_node;
	void** driver_data_ptr = &ptr->driver_data;
	
	{
		*ops_ptr = glue_unpack_new_shadow(__pos, msg, ext, struct blk_mq_ops*, sizeof(struct blk_mq_ops));
		if (*ops_ptr) {
			callee_unmarshal_kernel___global_blk_mq_ops__in(__pos, msg, ext, *ops_ptr);
		}

	}

	{
		*nr_hw_queues_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*queue_depth_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*cmd_size_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*flags_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*numa_node_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*driver_data_ptr = glue_unpack_new_shadow(__pos, msg, ext, void*, (sizeof(void*)));
		if (*driver_data_ptr) {
		}

	}

}

void callee_marshal_kernel__blk_mq_alloc_tag_set__set__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_alloc_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{
	struct blk_mq_ops* const* ops_ptr = &ptr->ops;
	unsigned int const* nr_hw_queues_ptr = &ptr->nr_hw_queues;
	unsigned int const* queue_depth_ptr = &ptr->queue_depth;
	unsigned int const* flags_ptr = &ptr->flags;
	void* const* driver_data_ptr = &ptr->driver_data;
	
	{
		if (*ops_ptr) {
			callee_marshal_kernel___global_blk_mq_ops__in(__pos, msg, ext, *ops_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *nr_hw_queues_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *queue_depth_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *flags_ptr);
	}

	{
		(void)driver_data_ptr;
	}

}

void caller_unmarshal_kernel__blk_mq_alloc_tag_set__set__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_alloc_tag_set_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{
	struct blk_mq_ops** ops_ptr = &ptr->ops;
	unsigned int* nr_hw_queues_ptr = &ptr->nr_hw_queues;
	unsigned int* queue_depth_ptr = &ptr->queue_depth;
	unsigned int* flags_ptr = &ptr->flags;
	void** driver_data_ptr = &ptr->driver_data;
	
	{
		if (*ops_ptr) {
			caller_unmarshal_kernel___global_blk_mq_ops__in(__pos, msg, ext, *ops_ptr);
		}

	}

	{
		*nr_hw_queues_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*queue_depth_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*flags_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		(void)driver_data_ptr;
	}

}

void caller_marshal_kernel___global_blk_mq_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_ops const* ptr)
{
	fptr_complete const* complete_ptr = &ptr->complete;
	fptr_init_hctx const* init_hctx_ptr = &ptr->init_hctx;
	fptr_map_queue const* map_queue_ptr = &ptr->map_queue;
	fptr_queue_rq const* queue_rq_ptr = &ptr->queue_rq;
	
	{
		glue_pack(__pos, msg, ext, *complete_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *init_hctx_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *map_queue_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *queue_rq_ptr);
	}

}

void callee_unmarshal_kernel___global_blk_mq_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_ops* ptr)
{
	fptr_complete* complete_ptr = &ptr->complete;
	fptr_init_hctx* init_hctx_ptr = &ptr->init_hctx;
	fptr_map_queue* map_queue_ptr = &ptr->map_queue;
	fptr_queue_rq* queue_rq_ptr = &ptr->queue_rq;
	
	{
		*complete_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, complete);
	}

	{
		*init_hctx_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, init_hctx);
	}

	{
		*map_queue_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, map_queue);
	}

	{
		*queue_rq_ptr = glue_unpack_rpc_ptr(__pos, msg, ext, queue_rq);
	}

}

void callee_marshal_kernel___global_blk_mq_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_ops const* ptr)
{
	
}

void caller_unmarshal_kernel___global_blk_mq_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_ops* ptr)
{
	
}

void caller_marshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{
	
}

void callee_marshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	
}

void caller_unmarshal_kernel__blk_mq_init_queue__ret_request_queue__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{
	
}

void caller_marshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{
	int const* numa_node_ptr = &ptr->numa_node;
	
	{
		glue_pack(__pos, msg, ext, *numa_node_ptr);
	}

}

void callee_unmarshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{
	int* numa_node_ptr = &ptr->numa_node;
	
	{
		*numa_node_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void callee_marshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct blk_mq_tag_set const* ptr)
{
	
}

void caller_unmarshal_kernel__blk_mq_init_queue__set__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_init_queue_call_ctx const* ctx,
	struct blk_mq_tag_set* ptr)
{
	
}

void caller_marshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{
	
}

void callee_unmarshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{
	
}

void callee_marshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx const* ptr)
{
	
}

void caller_unmarshal_kernel__blk_mq_map_queue__ret_blk_mq_hw_ctx__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct blk_mq_hw_ctx* ptr)
{
	
}

void caller_marshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{
	
}

void callee_marshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	
}

void caller_unmarshal_kernel__blk_mq_map_queue__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_map_queue_call_ctx const* ctx,
	struct request_queue* ptr)
{
	
}

void caller_marshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_start_request_call_ctx const* ctx,
	struct request const* ptr)
{
	
}

void callee_unmarshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_start_request_call_ctx const* ctx,
	struct request* ptr)
{
	
}

void callee_marshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_start_request_call_ctx const* ctx,
	struct request const* ptr)
{
	
}

void caller_unmarshal_kernel__blk_mq_start_request__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_start_request_call_ctx const* ctx,
	struct request* ptr)
{
	
}

void caller_marshal_kernel__blk_mq_complete_request__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct request const* ptr)
{
	
}

void callee_unmarshal_kernel__blk_mq_complete_request__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct request* ptr)
{
	
}

void callee_marshal_kernel__blk_mq_complete_request__rq__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct request const* ptr)
{
	int const* errors_ptr = &ptr->errors;
	
	{
		glue_pack(__pos, msg, ext, *errors_ptr);
	}

}

void caller_unmarshal_kernel__blk_mq_complete_request__rq__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_mq_complete_request_call_ctx const* ctx,
	struct request* ptr)
{
	int* errors_ptr = &ptr->errors;
	
	{
		*errors_ptr = glue_unpack(__pos, msg, ext, int);
	}

}

void caller_marshal_kernel__blk_queue_physical_block_size__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_queue_physical_block_size_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__blk_queue_physical_block_size__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_queue_physical_block_size_call_ctx const* ctx,
	struct request_queue* ptr)
{
	
}

void callee_marshal_kernel__blk_queue_physical_block_size__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_queue_physical_block_size_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	
}

void caller_unmarshal_kernel__blk_queue_physical_block_size__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_queue_physical_block_size_call_ctx const* ctx,
	struct request_queue* ptr)
{
	
}

void caller_marshal_kernel__blk_queue_logical_block_size__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_queue_logical_block_size_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	
}

void callee_unmarshal_kernel__blk_queue_logical_block_size__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_queue_logical_block_size_call_ctx const* ctx,
	struct request_queue* ptr)
{
	
}

void callee_marshal_kernel__blk_queue_logical_block_size__q__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct blk_queue_logical_block_size_call_ctx const* ctx,
	struct request_queue const* ptr)
{
	
}

void caller_unmarshal_kernel__blk_queue_logical_block_size__q__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct blk_queue_logical_block_size_call_ctx const* ctx,
	struct request_queue* ptr)
{
	
}


#ifdef LCD_ISOLATE
__attribute__((weak)) void shared_mem_init(void) {
	LIBLCD_MSG("Weak shared_mem_init does nothing! Override if you want!");
}
#else
__attribute__((weak)) void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext) {
	LIBLCD_MSG("Weak shared_mem_init_callee does nothing! Override if you want!");
}
#endif	/* LCD_ISOLATE */

