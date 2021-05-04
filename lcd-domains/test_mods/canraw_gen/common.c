#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void caller_marshal_kernel__proto_init__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_init_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_init__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_init_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__proto_init__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_init_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_init__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_init_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__dev_get_by_index__ret_net_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_by_index_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__dev_get_by_index__ret_net_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_by_index_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__dev_get_by_index__ret_net_device__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_by_index_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__dev_get_by_index__ret_net_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_by_index_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__dev_get_by_index__net__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_by_index_call_ctx const* ctx,
	struct net const* ptr)
{
	
}

void callee_unmarshal_kernel__dev_get_by_index__net__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_by_index_call_ctx const* ctx,
	struct net* ptr)
{
	
}

void callee_marshal_kernel__dev_get_by_index__net__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct dev_get_by_index_call_ctx const* ctx,
	struct net const* ptr)
{
	
}

void caller_unmarshal_kernel__dev_get_by_index__net__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_by_index_call_ctx const* ctx,
	struct net* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_bind__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__proto_ops_bind__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_unmarshal_kernel__proto_ops_bind__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__proto_ops_bind__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_marshal_kernel__proto_ops_bind__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			callee_marshal_kernel__proto_ops_bind__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_unmarshal_kernel__proto_ops_bind__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__proto_ops_bind__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_marshal_kernel__proto_ops_bind__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_bind__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_bind__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_bind__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_bind__uaddr__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct sockaddr const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_bind__uaddr__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct sockaddr* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_bind__uaddr__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct sockaddr const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_bind__uaddr__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct sockaddr* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_release__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_release_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__proto_ops_release__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_unmarshal_kernel__proto_ops_release__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_release_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__proto_ops_release__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_marshal_kernel__proto_ops_release__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_release_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			callee_marshal_kernel__proto_ops_release__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_unmarshal_kernel__proto_ops_release__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_release_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__proto_ops_release__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_marshal_kernel__proto_ops_release__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_release_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_release__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_release_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_release__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_release_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_release__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_release_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_getname__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__proto_ops_getname__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_unmarshal_kernel__proto_ops_getname__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__proto_ops_getname__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_marshal_kernel__proto_ops_getname__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			callee_marshal_kernel__proto_ops_getname__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_unmarshal_kernel__proto_ops_getname__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__proto_ops_getname__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_marshal_kernel__proto_ops_getname__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_getname__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_getname__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_getname__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_getname__uaddr__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct sockaddr const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_getname__uaddr__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct sockaddr* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_getname__uaddr__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct sockaddr const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_getname__uaddr__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct sockaddr* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_setsockopt__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_setsockopt_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__proto_ops_setsockopt__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_unmarshal_kernel__proto_ops_setsockopt__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_setsockopt_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__proto_ops_setsockopt__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_marshal_kernel__proto_ops_setsockopt__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_setsockopt_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			callee_marshal_kernel__proto_ops_setsockopt__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_unmarshal_kernel__proto_ops_setsockopt__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_setsockopt_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__proto_ops_setsockopt__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_marshal_kernel__proto_ops_setsockopt__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_setsockopt_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_setsockopt__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_setsockopt_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_setsockopt__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_setsockopt_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_setsockopt__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_setsockopt_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_getsockopt__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_getsockopt_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__proto_ops_getsockopt__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_unmarshal_kernel__proto_ops_getsockopt__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_getsockopt_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__proto_ops_getsockopt__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_marshal_kernel__proto_ops_getsockopt__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_getsockopt_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			callee_marshal_kernel__proto_ops_getsockopt__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_unmarshal_kernel__proto_ops_getsockopt__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_getsockopt_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__proto_ops_getsockopt__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_marshal_kernel__proto_ops_getsockopt__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_getsockopt_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_getsockopt__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_getsockopt_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_getsockopt__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_getsockopt_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_getsockopt__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_getsockopt_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_sendmsg__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__proto_ops_sendmsg__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_unmarshal_kernel__proto_ops_sendmsg__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__proto_ops_sendmsg__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_marshal_kernel__proto_ops_sendmsg__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			callee_marshal_kernel__proto_ops_sendmsg__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_unmarshal_kernel__proto_ops_sendmsg__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__proto_ops_sendmsg__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_marshal_kernel__proto_ops_sendmsg__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_sendmsg__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_sendmsg__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_sendmsg__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_sendmsg__msg__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct msghdr const* ptr)
{
	void* const* msg_name_ptr = &ptr->msg_name;
	int const* msg_namelen_ptr = &ptr->msg_namelen;
	struct kiocb* const* msg_iocb_ptr = &ptr->msg_iocb;
	
	{
		__maybe_unused const void* __adjusted = *msg_name_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*msg_name_ptr) {
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *msg_namelen_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *msg_iocb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*msg_iocb_ptr) {
			caller_marshal_kernel__proto_ops_sendmsg__msghdr_msg_iocb__in(__pos, __msg, __ext, ctx, *msg_iocb_ptr);
		}

	}

}

void callee_unmarshal_kernel__proto_ops_sendmsg__msg__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct msghdr* ptr)
{
	void** msg_name_ptr = &ptr->msg_name;
	int* msg_namelen_ptr = &ptr->msg_namelen;
	struct kiocb** msg_iocb_ptr = &ptr->msg_iocb;
	
	{
		*msg_name_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*msg_name_ptr) {
		}

	}

	{
		*msg_namelen_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*msg_iocb_ptr = glue_unpack(__pos, __msg, __ext, struct kiocb*);
		if (*msg_iocb_ptr) {
			callee_unmarshal_kernel__proto_ops_sendmsg__msghdr_msg_iocb__in(__pos, __msg, __ext, ctx, *msg_iocb_ptr);
		}

	}

}

void callee_marshal_kernel__proto_ops_sendmsg__msg__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct msghdr const* ptr)
{
	void* const* msg_name_ptr = &ptr->msg_name;
	struct kiocb* const* msg_iocb_ptr = &ptr->msg_iocb;
	
	{
		(void)msg_name_ptr;
	}

	{
		if (*msg_iocb_ptr) {
			callee_marshal_kernel__proto_ops_sendmsg__msghdr_msg_iocb__in(__pos, __msg, __ext, ctx, *msg_iocb_ptr);
		}

	}

}

void caller_unmarshal_kernel__proto_ops_sendmsg__msg__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct msghdr* ptr)
{
	void** msg_name_ptr = &ptr->msg_name;
	struct kiocb** msg_iocb_ptr = &ptr->msg_iocb;
	
	{
		(void)msg_name_ptr;
	}

	{
		if (*msg_iocb_ptr) {
			caller_unmarshal_kernel__proto_ops_sendmsg__msghdr_msg_iocb__in(__pos, __msg, __ext, ctx, *msg_iocb_ptr);
		}

	}

}

void caller_marshal_kernel__proto_ops_sendmsg__msghdr_msg_iocb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct kiocb const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_sendmsg__msghdr_msg_iocb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct kiocb* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_sendmsg__msghdr_msg_iocb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct kiocb const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_sendmsg__msghdr_msg_iocb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct kiocb* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_recvmsg__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__proto_ops_recvmsg__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_unmarshal_kernel__proto_ops_recvmsg__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__proto_ops_recvmsg__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_marshal_kernel__proto_ops_recvmsg__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			callee_marshal_kernel__proto_ops_recvmsg__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_unmarshal_kernel__proto_ops_recvmsg__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__proto_ops_recvmsg__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_marshal_kernel__proto_ops_recvmsg__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_recvmsg__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_recvmsg__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_recvmsg__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_recvmsg__msg__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct msghdr const* ptr)
{
	void* const* msg_name_ptr = &ptr->msg_name;
	int const* msg_namelen_ptr = &ptr->msg_namelen;
	unsigned int const* msg_flags_ptr = &ptr->msg_flags;
	
	{
		__maybe_unused const void* __adjusted = *msg_name_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*msg_name_ptr) {
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *msg_namelen_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *msg_flags_ptr);
	}

}

void callee_unmarshal_kernel__proto_ops_recvmsg__msg__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct msghdr* ptr)
{
	void** msg_name_ptr = &ptr->msg_name;
	int* msg_namelen_ptr = &ptr->msg_namelen;
	unsigned int* msg_flags_ptr = &ptr->msg_flags;
	
	{
		*msg_name_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*msg_name_ptr) {
		}

	}

	{
		*msg_namelen_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*msg_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

}

void callee_marshal_kernel__proto_ops_recvmsg__msg__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct msghdr const* ptr)
{
	void* const* msg_name_ptr = &ptr->msg_name;
	int const* msg_namelen_ptr = &ptr->msg_namelen;
	unsigned int const* msg_flags_ptr = &ptr->msg_flags;
	
	{
		(void)msg_name_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *msg_namelen_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *msg_flags_ptr);
	}

}

void caller_unmarshal_kernel__proto_ops_recvmsg__msg__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct msghdr* ptr)
{
	void** msg_name_ptr = &ptr->msg_name;
	int* msg_namelen_ptr = &ptr->msg_namelen;
	unsigned int* msg_flags_ptr = &ptr->msg_flags;
	
	{
		(void)msg_name_ptr;
	}

	{
		*msg_namelen_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*msg_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

}

void caller_marshal_kernel__proto_ops_mmap__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_mmap_call_ctx const* ctx,
	struct file const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_mmap__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_mmap_call_ctx const* ctx,
	struct file* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_mmap__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_mmap_call_ctx const* ctx,
	struct file const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_mmap__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_mmap_call_ctx const* ctx,
	struct file* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_mmap__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_mmap_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_mmap__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_mmap_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_mmap__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_mmap_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_mmap__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_mmap_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_mmap__vma__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_mmap_call_ctx const* ctx,
	struct vm_area_struct const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_mmap__vma__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_mmap_call_ctx const* ctx,
	struct vm_area_struct* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_mmap__vma__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_mmap_call_ctx const* ctx,
	struct vm_area_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_mmap__vma__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_mmap_call_ctx const* ctx,
	struct vm_area_struct* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_shutdown__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_shutdown_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_shutdown__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_shutdown_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_shutdown__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_shutdown_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_shutdown__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_shutdown_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_listen__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_listen_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_listen__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_listen_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_listen__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_listen_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_listen__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_listen_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_accept__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_accept_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_accept__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_accept_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_accept__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_accept_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_accept__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_accept_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_accept__newsock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_accept_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_accept__newsock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_accept_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_accept__newsock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_accept_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_accept__newsock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_accept_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_socketpair__sock1__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_socketpair_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_socketpair__sock1__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_socketpair_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_socketpair__sock1__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_socketpair_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_socketpair__sock1__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_socketpair_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_socketpair__sock2__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_socketpair_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_socketpair__sock2__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_socketpair_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_socketpair__sock2__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_socketpair_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_socketpair__sock2__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_socketpair_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_connect__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_connect_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_connect__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_connect_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_connect__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_connect_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_connect__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_connect_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_connect__saddr__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_connect_call_ctx const* ctx,
	struct sockaddr const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_connect__saddr__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_connect_call_ctx const* ctx,
	struct sockaddr* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_connect__saddr__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_connect_call_ctx const* ctx,
	struct sockaddr const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_connect__saddr__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_connect_call_ctx const* ctx,
	struct sockaddr* ptr)
{
	
}

void caller_marshal_kernel__sock_alloc_send_skb__ret_sk_buff__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_alloc_send_skb__ret_sk_buff__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void callee_marshal_kernel__sock_alloc_send_skb__ret_sk_buff__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_alloc_send_skb__ret_sk_buff__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void caller_marshal_kernel__sock_alloc_send_skb__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct sock const* ptr)
{
	int const* sk_err_ptr = &ptr->sk_err;
	struct socket* const* sk_socket_ptr = &ptr->sk_socket;
	
	{
		glue_pack(__pos, __msg, __ext, *sk_err_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *sk_socket_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*sk_socket_ptr) {
			caller_marshal_kernel__sock_alloc_send_skb__sock_sk_socket__in(__pos, __msg, __ext, ctx, *sk_socket_ptr);
		}

	}

}

void callee_unmarshal_kernel__sock_alloc_send_skb__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct sock* ptr)
{
	int* sk_err_ptr = &ptr->sk_err;
	struct socket** sk_socket_ptr = &ptr->sk_socket;
	
	{
		*sk_err_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*sk_socket_ptr = glue_unpack(__pos, __msg, __ext, struct socket*);
		if (*sk_socket_ptr) {
			callee_unmarshal_kernel__sock_alloc_send_skb__sock_sk_socket__in(__pos, __msg, __ext, ctx, *sk_socket_ptr);
		}

	}

}

void callee_marshal_kernel__sock_alloc_send_skb__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct sock const* ptr)
{
	struct socket* const* sk_socket_ptr = &ptr->sk_socket;
	
	{
		if (*sk_socket_ptr) {
			callee_marshal_kernel__sock_alloc_send_skb__sock_sk_socket__in(__pos, __msg, __ext, ctx, *sk_socket_ptr);
		}

	}

}

void caller_unmarshal_kernel__sock_alloc_send_skb__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct sock* ptr)
{
	struct socket** sk_socket_ptr = &ptr->sk_socket;
	
	{
		if (*sk_socket_ptr) {
			caller_unmarshal_kernel__sock_alloc_send_skb__sock_sk_socket__in(__pos, __msg, __ext, ctx, *sk_socket_ptr);
		}

	}

}

void caller_marshal_kernel__sock_alloc_send_skb__sock_sk_socket__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_alloc_send_skb__sock_sk_socket__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__sock_alloc_send_skb__sock_sk_socket__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_alloc_send_skb__sock_sk_socket__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__register_netdevice_notifier__nb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct register_netdevice_notifier_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	
}

void callee_unmarshal_kernel__register_netdevice_notifier__nb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct register_netdevice_notifier_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	
}

void callee_marshal_kernel__register_netdevice_notifier__nb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct register_netdevice_notifier_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	
}

void caller_unmarshal_kernel__register_netdevice_notifier__nb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct register_netdevice_notifier_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	
}

void caller_marshal_kernel__unregister_netdevice_notifier__nb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct unregister_netdevice_notifier_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	
}

void callee_unmarshal_kernel__unregister_netdevice_notifier__nb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct unregister_netdevice_notifier_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	
}

void callee_marshal_kernel__unregister_netdevice_notifier__nb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct unregister_netdevice_notifier_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	
}

void caller_unmarshal_kernel__unregister_netdevice_notifier__nb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct unregister_netdevice_notifier_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	
}

void caller_marshal_kernel__kfree_skb__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct kfree_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	
	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

}

void callee_unmarshal_kernel__kfree_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct kfree_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	
	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

}

void callee_marshal_kernel__kfree_skb__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct kfree_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned char* const* head_ptr = &ptr->head;
	
	{
		(void)head_ptr;
	}

}

void caller_unmarshal_kernel__kfree_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct kfree_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned char** head_ptr = &ptr->head;
	
	{
		(void)head_ptr;
	}

}

void caller_marshal_kernel__skb_clone__ret_sk_buff__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void callee_unmarshal_kernel__skb_clone__ret_sk_buff__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void callee_marshal_kernel__skb_clone__ret_sk_buff__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void caller_unmarshal_kernel__skb_clone__ret_sk_buff__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void caller_marshal_kernel__skb_clone__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	struct net_device* const* dev_ptr = &ptr->dev;
	char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;
	
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__skb_clone__sk_buff_sk__io(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__skb_clone__sk_buff_dev__io(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		size_t i, len = 48;
		char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__skb_clone__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	struct net_device** dev_ptr = &ptr->dev;
	char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;
	
	{
		*sk_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__skb_clone__sk_buff_sk__io(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__skb_clone__sk_buff_dev__io(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		int i;
		char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

}

void callee_marshal_kernel__skb_clone__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	struct net_device* const* dev_ptr = &ptr->dev;
	char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;
	
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			callee_marshal_kernel__skb_clone__sk_buff_sk__io(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			callee_marshal_kernel__skb_clone__sk_buff_dev__io(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		size_t i, len = 48;
		char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void caller_unmarshal_kernel__skb_clone__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	struct net_device** dev_ptr = &ptr->dev;
	char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;
	
	{
		*sk_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			caller_unmarshal_kernel__skb_clone__sk_buff_sk__io(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			caller_unmarshal_kernel__skb_clone__sk_buff_dev__io(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		int i;
		char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

}

void caller_marshal_kernel__skb_clone__sk_buff_sk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__skb_clone__sk_buff_sk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__skb_clone__sk_buff_sk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__skb_clone__sk_buff_sk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__skb_clone__sk_buff_dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__skb_clone__sk_buff_dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__skb_clone__sk_buff_dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__skb_clone__sk_buff_dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__sock_queue_rcv_skb__sk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_queue_rcv_skb_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_queue_rcv_skb__sk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_queue_rcv_skb_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__sock_queue_rcv_skb__sk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_queue_rcv_skb_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_queue_rcv_skb__sk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_queue_rcv_skb_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__sock_queue_rcv_skb__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_queue_rcv_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_queue_rcv_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_queue_rcv_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void callee_marshal_kernel__sock_queue_rcv_skb__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_queue_rcv_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_queue_rcv_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_queue_rcv_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void caller_marshal_kernel__can_proto_unregister__cp__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_proto_unregister_call_ctx const* ctx,
	struct can_proto const* ptr)
{
	int const* protocol_ptr = &ptr->protocol;
	struct proto_ops* const* ops_ptr = &ptr->ops;
	struct proto* const* prot_ptr = &ptr->prot;
	
	{
		glue_pack(__pos, __msg, __ext, *protocol_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *ops_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ops_ptr) {
			caller_marshal_kernel___global_proto_ops__in(__pos, __msg, __ext, *ops_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *prot_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*prot_ptr) {
			caller_marshal_kernel__can_proto_unregister__can_proto_prot__in(__pos, __msg, __ext, ctx, *prot_ptr);
		}

	}

}

void callee_unmarshal_kernel__can_proto_unregister__cp__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_proto_unregister_call_ctx const* ctx,
	struct can_proto* ptr)
{
	int* protocol_ptr = &ptr->protocol;
	struct proto_ops** ops_ptr = &ptr->ops;
	struct proto** prot_ptr = &ptr->prot;
	
	{
		*protocol_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*ops_ptr = glue_unpack(__pos, __msg, __ext, struct proto_ops*);
		if (*ops_ptr) {
			callee_unmarshal_kernel___global_proto_ops__in(__pos, __msg, __ext, *ops_ptr);
		}

	}

	{
		*prot_ptr = glue_unpack(__pos, __msg, __ext, struct proto*);
		if (*prot_ptr) {
			callee_unmarshal_kernel__can_proto_unregister__can_proto_prot__in(__pos, __msg, __ext, ctx, *prot_ptr);
		}

	}

}

void callee_marshal_kernel__can_proto_unregister__cp__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_proto_unregister_call_ctx const* ctx,
	struct can_proto const* ptr)
{
	struct proto_ops* const* ops_ptr = &ptr->ops;
	struct proto* const* prot_ptr = &ptr->prot;
	
	{
		if (*ops_ptr) {
			callee_marshal_kernel___global_proto_ops__in(__pos, __msg, __ext, *ops_ptr);
		}

	}

	{
		if (*prot_ptr) {
			callee_marshal_kernel__can_proto_unregister__can_proto_prot__in(__pos, __msg, __ext, ctx, *prot_ptr);
		}

	}

}

void caller_unmarshal_kernel__can_proto_unregister__cp__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_proto_unregister_call_ctx const* ctx,
	struct can_proto* ptr)
{
	struct proto_ops** ops_ptr = &ptr->ops;
	struct proto** prot_ptr = &ptr->prot;
	
	{
		if (*ops_ptr) {
			caller_unmarshal_kernel___global_proto_ops__in(__pos, __msg, __ext, *ops_ptr);
		}

	}

	{
		if (*prot_ptr) {
			caller_unmarshal_kernel__can_proto_unregister__can_proto_prot__in(__pos, __msg, __ext, ctx, *prot_ptr);
		}

	}

}

void caller_marshal_kernel___global_proto_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops const* ptr)
{
	fptr_proto_ops_accept const* accept_ptr = &ptr->accept;
	fptr_proto_ops_bind const* bind_ptr = &ptr->bind;
	fptr_proto_ops_connect const* connect_ptr = &ptr->connect;
	fptr_proto_ops_getname const* getname_ptr = &ptr->getname;
	fptr_proto_ops_getsockopt const* getsockopt_ptr = &ptr->getsockopt;
	fptr_proto_ops_ioctl const* ioctl_ptr = &ptr->ioctl;
	fptr_proto_ops_listen const* listen_ptr = &ptr->listen;
	fptr_proto_ops_mmap const* mmap_ptr = &ptr->mmap;
	fptr_proto_ops_poll const* poll_ptr = &ptr->poll;
	fptr_proto_ops_recvmsg const* recvmsg_ptr = &ptr->recvmsg;
	fptr_proto_ops_release const* release_ptr = &ptr->release;
	fptr_proto_ops_sendmsg const* sendmsg_ptr = &ptr->sendmsg;
	fptr_proto_ops_sendpage const* sendpage_ptr = &ptr->sendpage;
	fptr_proto_ops_setsockopt const* setsockopt_ptr = &ptr->setsockopt;
	fptr_proto_ops_shutdown const* shutdown_ptr = &ptr->shutdown;
	fptr_proto_ops_socketpair const* socketpair_ptr = &ptr->socketpair;
	
	{
		glue_pack(__pos, __msg, __ext, *accept_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *bind_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *connect_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *getname_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *getsockopt_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ioctl_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *listen_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mmap_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *poll_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *recvmsg_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *release_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *sendmsg_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *sendpage_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *setsockopt_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *shutdown_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *socketpair_ptr);
	}

}

void callee_unmarshal_kernel___global_proto_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops* ptr)
{
	fptr_proto_ops_accept* accept_ptr = &ptr->accept;
	fptr_proto_ops_bind* bind_ptr = &ptr->bind;
	fptr_proto_ops_connect* connect_ptr = &ptr->connect;
	fptr_proto_ops_getname* getname_ptr = &ptr->getname;
	fptr_proto_ops_getsockopt* getsockopt_ptr = &ptr->getsockopt;
	fptr_proto_ops_ioctl* ioctl_ptr = &ptr->ioctl;
	fptr_proto_ops_listen* listen_ptr = &ptr->listen;
	fptr_proto_ops_mmap* mmap_ptr = &ptr->mmap;
	fptr_proto_ops_poll* poll_ptr = &ptr->poll;
	fptr_proto_ops_recvmsg* recvmsg_ptr = &ptr->recvmsg;
	fptr_proto_ops_release* release_ptr = &ptr->release;
	fptr_proto_ops_sendmsg* sendmsg_ptr = &ptr->sendmsg;
	fptr_proto_ops_sendpage* sendpage_ptr = &ptr->sendpage;
	fptr_proto_ops_setsockopt* setsockopt_ptr = &ptr->setsockopt;
	fptr_proto_ops_shutdown* shutdown_ptr = &ptr->shutdown;
	fptr_proto_ops_socketpair* socketpair_ptr = &ptr->socketpair;
	
	{
		*accept_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_accept);
	}

	{
		*bind_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_bind);
	}

	{
		*connect_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_connect);
	}

	{
		*getname_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_getname);
	}

	{
		*getsockopt_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_getsockopt);
	}

	{
		*ioctl_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_ioctl);
	}

	{
		*listen_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_listen);
	}

	{
		*mmap_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_mmap);
	}

	{
		*poll_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_poll);
	}

	{
		*recvmsg_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_recvmsg);
	}

	{
		*release_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_release);
	}

	{
		*sendmsg_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_sendmsg);
	}

	{
		*sendpage_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_sendpage);
	}

	{
		*setsockopt_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_setsockopt);
	}

	{
		*shutdown_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_shutdown);
	}

	{
		*socketpair_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_socketpair);
	}

}

void callee_marshal_kernel___global_proto_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops const* ptr)
{
	
}

void caller_unmarshal_kernel___global_proto_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops* ptr)
{
	
}

void caller_marshal_kernel__can_proto_unregister__can_proto_prot__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_proto_unregister_call_ctx const* ctx,
	struct proto const* ptr)
{
	fptr_proto_init const* init_ptr = &ptr->init;
	
	{
		glue_pack(__pos, __msg, __ext, *init_ptr);
	}

}

void callee_unmarshal_kernel__can_proto_unregister__can_proto_prot__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_proto_unregister_call_ctx const* ctx,
	struct proto* ptr)
{
	fptr_proto_init* init_ptr = &ptr->init;
	
	{
		*init_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_init);
	}

}

void callee_marshal_kernel__can_proto_unregister__can_proto_prot__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_proto_unregister_call_ctx const* ctx,
	struct proto const* ptr)
{
	
}

void caller_unmarshal_kernel__can_proto_unregister__can_proto_prot__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_proto_unregister_call_ctx const* ctx,
	struct proto* ptr)
{
	
}

void caller_marshal_kernel__can_proto_register__cp__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_proto_register_call_ctx const* ctx,
	struct can_proto const* ptr)
{
	int const* protocol_ptr = &ptr->protocol;
	struct proto_ops* const* ops_ptr = &ptr->ops;
	struct proto* const* prot_ptr = &ptr->prot;
	
	{
		glue_pack(__pos, __msg, __ext, *protocol_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *ops_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*ops_ptr) {
			caller_marshal_kernel___global_proto_ops__in(__pos, __msg, __ext, *ops_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *prot_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*prot_ptr) {
			caller_marshal_kernel__can_proto_register__can_proto_prot__in(__pos, __msg, __ext, ctx, *prot_ptr);
		}

	}

}

void callee_unmarshal_kernel__can_proto_register__cp__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_proto_register_call_ctx const* ctx,
	struct can_proto* ptr)
{
	int* protocol_ptr = &ptr->protocol;
	struct proto_ops** ops_ptr = &ptr->ops;
	struct proto** prot_ptr = &ptr->prot;
	
	{
		*protocol_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*ops_ptr = glue_unpack(__pos, __msg, __ext, struct proto_ops*);
		if (*ops_ptr) {
			callee_unmarshal_kernel___global_proto_ops__in(__pos, __msg, __ext, *ops_ptr);
		}

	}

	{
		*prot_ptr = glue_unpack(__pos, __msg, __ext, struct proto*);
		if (*prot_ptr) {
			callee_unmarshal_kernel__can_proto_register__can_proto_prot__in(__pos, __msg, __ext, ctx, *prot_ptr);
		}

	}

}

void callee_marshal_kernel__can_proto_register__cp__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_proto_register_call_ctx const* ctx,
	struct can_proto const* ptr)
{
	struct proto_ops* const* ops_ptr = &ptr->ops;
	struct proto* const* prot_ptr = &ptr->prot;
	
	{
		if (*ops_ptr) {
			callee_marshal_kernel___global_proto_ops__in(__pos, __msg, __ext, *ops_ptr);
		}

	}

	{
		if (*prot_ptr) {
			callee_marshal_kernel__can_proto_register__can_proto_prot__in(__pos, __msg, __ext, ctx, *prot_ptr);
		}

	}

}

void caller_unmarshal_kernel__can_proto_register__cp__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_proto_register_call_ctx const* ctx,
	struct can_proto* ptr)
{
	struct proto_ops** ops_ptr = &ptr->ops;
	struct proto** prot_ptr = &ptr->prot;
	
	{
		if (*ops_ptr) {
			caller_unmarshal_kernel___global_proto_ops__in(__pos, __msg, __ext, *ops_ptr);
		}

	}

	{
		if (*prot_ptr) {
			caller_unmarshal_kernel__can_proto_register__can_proto_prot__in(__pos, __msg, __ext, ctx, *prot_ptr);
		}

	}

}

void caller_marshal_kernel__can_proto_register__can_proto_prot__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_proto_register_call_ctx const* ctx,
	struct proto const* ptr)
{
	fptr_proto_init const* init_ptr = &ptr->init;
	
	{
		glue_pack(__pos, __msg, __ext, *init_ptr);
	}

}

void callee_unmarshal_kernel__can_proto_register__can_proto_prot__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_proto_register_call_ctx const* ctx,
	struct proto* ptr)
{
	fptr_proto_init* init_ptr = &ptr->init;
	
	{
		*init_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_init);
	}

}

void callee_marshal_kernel__can_proto_register__can_proto_prot__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_proto_register_call_ctx const* ctx,
	struct proto const* ptr)
{
	
}

void caller_unmarshal_kernel__can_proto_register__can_proto_prot__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_proto_register_call_ctx const* ctx,
	struct proto* ptr)
{
	
}

void caller_marshal_kernel__func__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void callee_unmarshal_kernel__func__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void callee_marshal_kernel__func__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void caller_unmarshal_kernel__func__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void caller_marshal_kernel__can_rx_unregister__dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_rx_unregister_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned short const* type_ptr = &ptr->type;
	
	{
		glue_pack(__pos, __msg, __ext, *type_ptr);
	}

}

void callee_unmarshal_kernel__can_rx_unregister__dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_rx_unregister_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned short* type_ptr = &ptr->type;
	
	{
		*type_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

}

void callee_marshal_kernel__can_rx_unregister__dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_rx_unregister_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned short const* type_ptr = &ptr->type;
	
	{
		glue_pack(__pos, __msg, __ext, *type_ptr);
	}

}

void caller_unmarshal_kernel__can_rx_unregister__dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_rx_unregister_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned short* type_ptr = &ptr->type;
	
	{
		*type_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

}

void caller_marshal_kernel__can_rx_register__dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_rx_register_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned short const* type_ptr = &ptr->type;
	
	{
		glue_pack(__pos, __msg, __ext, *type_ptr);
	}

}

void callee_unmarshal_kernel__can_rx_register__dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_rx_register_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned short* type_ptr = &ptr->type;
	
	{
		*type_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

}

void callee_marshal_kernel__can_rx_register__dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_rx_register_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned short const* type_ptr = &ptr->type;
	
	{
		glue_pack(__pos, __msg, __ext, *type_ptr);
	}

}

void caller_unmarshal_kernel__can_rx_register__dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_rx_register_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned short* type_ptr = &ptr->type;
	
	{
		*type_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

}

void caller_marshal_kernel__can_send__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	struct net_device* const* dev_ptr = &ptr->dev;
	char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;
	
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__can_send__sk_buff_sk__io(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__can_send__sk_buff_dev__io(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		size_t i, len = 48;
		char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__can_send__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	struct net_device** dev_ptr = &ptr->dev;
	char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;
	
	{
		*sk_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__can_send__sk_buff_sk__io(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__can_send__sk_buff_dev__io(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		int i;
		char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

}

void callee_marshal_kernel__can_send__skb__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	struct net_device* const* dev_ptr = &ptr->dev;
	char const* cb_ptr = ptr->cb;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;
	
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			callee_marshal_kernel__can_send__sk_buff_sk__io(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			callee_marshal_kernel__can_send__sk_buff_dev__io(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		size_t i, len = 48;
		char const* array = cb_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void caller_unmarshal_kernel__can_send__skb__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	struct net_device** dev_ptr = &ptr->dev;
	char* cb_ptr = ptr->cb;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;
	
	{
		*sk_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			caller_unmarshal_kernel__can_send__sk_buff_sk__io(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		*dev_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			caller_unmarshal_kernel__can_send__sk_buff_dev__io(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		int i;
		char* array = cb_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, char);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack_shadow(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

}

void caller_marshal_kernel__can_send__sk_buff_sk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__can_send__sk_buff_sk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__can_send__sk_buff_sk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__can_send__sk_buff_sk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__can_send__sk_buff_dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned short const* type_ptr = &ptr->type;
	
	{
		glue_pack(__pos, __msg, __ext, *type_ptr);
	}

}

void callee_unmarshal_kernel__can_send__sk_buff_dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned short* type_ptr = &ptr->type;
	
	{
		*type_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

}

void callee_marshal_kernel__can_send__sk_buff_dev__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct net_device const* ptr)
{
	unsigned short const* type_ptr = &ptr->type;
	
	{
		glue_pack(__pos, __msg, __ext, *type_ptr);
	}

}

void caller_unmarshal_kernel__can_send__sk_buff_dev__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned short* type_ptr = &ptr->type;
	
	{
		*type_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

}

void caller_marshal_kernel__proto_ops_ioctl__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_ioctl_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__proto_ops_ioctl__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_unmarshal_kernel__proto_ops_ioctl__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_ioctl_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__proto_ops_ioctl__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_marshal_kernel__proto_ops_ioctl__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_ioctl_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			callee_marshal_kernel__proto_ops_ioctl__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_unmarshal_kernel__proto_ops_ioctl__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_ioctl_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__proto_ops_ioctl__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_marshal_kernel__proto_ops_ioctl__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_ioctl_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_ioctl__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_ioctl_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_ioctl__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_ioctl_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_ioctl__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_ioctl_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__copy_from_iter__i__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct copy_from_iter_call_ctx const* ctx,
	struct iov_iter const* ptr)
{
	
}

void callee_unmarshal_kernel__copy_from_iter__i__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct copy_from_iter_call_ctx const* ctx,
	struct iov_iter* ptr)
{
	
}

void callee_marshal_kernel__copy_from_iter__i__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct copy_from_iter_call_ctx const* ctx,
	struct iov_iter const* ptr)
{
	
}

void caller_unmarshal_kernel__copy_from_iter__i__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct copy_from_iter_call_ctx const* ctx,
	struct iov_iter* ptr)
{
	
}

void caller_marshal_kernel__copy_to_iter__i__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct copy_to_iter_call_ctx const* ctx,
	struct iov_iter const* ptr)
{
	
}

void callee_unmarshal_kernel__copy_to_iter__i__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct copy_to_iter_call_ctx const* ctx,
	struct iov_iter* ptr)
{
	
}

void callee_marshal_kernel__copy_to_iter__i__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct copy_to_iter_call_ctx const* ctx,
	struct iov_iter const* ptr)
{
	
}

void caller_unmarshal_kernel__copy_to_iter__i__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct copy_to_iter_call_ctx const* ctx,
	struct iov_iter* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_poll__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct file const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_poll__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct file* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_poll__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct file const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_poll__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct file* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_poll__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__proto_ops_poll__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_unmarshal_kernel__proto_ops_poll__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__proto_ops_poll__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void callee_marshal_kernel__proto_ops_poll__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct socket const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			callee_marshal_kernel__proto_ops_poll__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_unmarshal_kernel__proto_ops_poll__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct socket* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__proto_ops_poll__socket_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_marshal_kernel__proto_ops_poll__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_poll__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_poll__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_poll__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_poll__wait__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct poll_table_struct const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_poll__wait__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct poll_table_struct* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_poll__wait__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct poll_table_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_poll__wait__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct poll_table_struct* ptr)
{
	
}

void caller_marshal_kernel__release_sock__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct release_sock_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__release_sock__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct release_sock_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__release_sock__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct release_sock_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__release_sock__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct release_sock_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__skb_free_datagram__sk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_free_datagram_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__skb_free_datagram__sk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_free_datagram_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__skb_free_datagram__sk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_free_datagram_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__skb_free_datagram__sk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_free_datagram_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__skb_free_datagram__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_free_datagram_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	
	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

}

void callee_unmarshal_kernel__skb_free_datagram__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_free_datagram_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	
	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

}

void callee_marshal_kernel__skb_free_datagram__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_free_datagram_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned char* const* head_ptr = &ptr->head;
	
	{
		(void)head_ptr;
	}

}

void caller_unmarshal_kernel__skb_free_datagram__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_free_datagram_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned char** head_ptr = &ptr->head;
	
	{
		(void)head_ptr;
	}

}

void caller_marshal_kernel__skb_recv_datagram__ret_sk_buff__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_recv_datagram_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void callee_unmarshal_kernel__skb_recv_datagram__ret_sk_buff__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_recv_datagram_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void callee_marshal_kernel__skb_recv_datagram__ret_sk_buff__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_recv_datagram_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void caller_unmarshal_kernel__skb_recv_datagram__ret_sk_buff__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_recv_datagram_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void caller_marshal_kernel__skb_recv_datagram__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_recv_datagram_call_ctx const* ctx,
	struct sock const* ptr)
{
	int const* sk_err_ptr = &ptr->sk_err;
	
	{
		glue_pack(__pos, __msg, __ext, *sk_err_ptr);
	}

}

void callee_unmarshal_kernel__skb_recv_datagram__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_recv_datagram_call_ctx const* ctx,
	struct sock* ptr)
{
	int* sk_err_ptr = &ptr->sk_err;
	
	{
		*sk_err_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

}

void callee_marshal_kernel__skb_recv_datagram__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_recv_datagram_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__skb_recv_datagram__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_recv_datagram_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_sendpage__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_sendpage_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_sendpage__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_sendpage_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_sendpage__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_sendpage_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_sendpage__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_sendpage_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_sendpage__page__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_sendpage_call_ctx const* ctx,
	struct page const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_sendpage__page__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_sendpage_call_ctx const* ctx,
	struct page* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_sendpage__page__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_sendpage_call_ctx const* ctx,
	struct page const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_sendpage__page__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_sendpage_call_ctx const* ctx,
	struct page* ptr)
{
	
}

void caller_marshal_kernel__lock_sock_nested__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lock_sock_nested_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__lock_sock_nested__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lock_sock_nested_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__lock_sock_nested__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lock_sock_nested_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__lock_sock_nested__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lock_sock_nested_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel____sock_recv_ts_and_drops__msg__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __sock_recv_ts_and_drops_call_ctx const* ctx,
	struct msghdr const* ptr)
{
	
}

void callee_unmarshal_kernel____sock_recv_ts_and_drops__msg__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __sock_recv_ts_and_drops_call_ctx const* ctx,
	struct msghdr* ptr)
{
	
}

void callee_marshal_kernel____sock_recv_ts_and_drops__msg__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __sock_recv_ts_and_drops_call_ctx const* ctx,
	struct msghdr const* ptr)
{
	
}

void caller_unmarshal_kernel____sock_recv_ts_and_drops__msg__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __sock_recv_ts_and_drops_call_ctx const* ctx,
	struct msghdr* ptr)
{
	
}

void caller_marshal_kernel____sock_recv_ts_and_drops__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __sock_recv_ts_and_drops_call_ctx const* ctx,
	struct sock const* ptr)
{
	unsigned short const* sk_tsflags_ptr = &ptr->sk_tsflags;
	
	{
		glue_pack(__pos, __msg, __ext, *sk_tsflags_ptr);
	}

}

void callee_unmarshal_kernel____sock_recv_ts_and_drops__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __sock_recv_ts_and_drops_call_ctx const* ctx,
	struct sock* ptr)
{
	unsigned short* sk_tsflags_ptr = &ptr->sk_tsflags;
	
	{
		*sk_tsflags_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

}

void callee_marshal_kernel____sock_recv_ts_and_drops__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __sock_recv_ts_and_drops_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel____sock_recv_ts_and_drops__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __sock_recv_ts_and_drops_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel____sock_recv_ts_and_drops__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __sock_recv_ts_and_drops_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void callee_unmarshal_kernel____sock_recv_ts_and_drops__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __sock_recv_ts_and_drops_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void callee_marshal_kernel____sock_recv_ts_and_drops__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct __sock_recv_ts_and_drops_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void caller_unmarshal_kernel____sock_recv_ts_and_drops__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __sock_recv_ts_and_drops_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void caller_marshal_kernel__skb_put__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	struct net_device* const* dev_ptr = &ptr->dev;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;
	
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__skb_put__sk_buff_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *head_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*head_ptr) {
			glue_pack(__pos, __msg, __ext, **head_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			glue_pack(__pos, __msg, __ext, **data_ptr);
		}

	}

}

void callee_unmarshal_kernel__skb_put__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	struct net_device** dev_ptr = &ptr->dev;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;
	
	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__skb_put__sk_buff_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*head_ptr) {
			**head_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char*);
		if (*data_ptr) {
			**data_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

}

void callee_marshal_kernel__skb_put__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	struct net_device* const* dev_ptr = &ptr->dev;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* headers_end_ptr = ptr->headers_end;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned char* const* head_ptr = &ptr->head;
	unsigned char* const* data_ptr = &ptr->data;
	
	{
		if (*dev_ptr) {
			callee_marshal_kernel__skb_put__sk_buff_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		size_t i, len = 0;
		unsigned int const* array = headers_end_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		(void)head_ptr;
	}

	{
		(void)data_ptr;
	}

}

void caller_unmarshal_kernel__skb_put__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	struct net_device** dev_ptr = &ptr->dev;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* headers_end_ptr = ptr->headers_end;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned char** head_ptr = &ptr->head;
	unsigned char** data_ptr = &ptr->data;
	
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__skb_put__sk_buff_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		int i;
		unsigned int* array = headers_end_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			unsigned int* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, unsigned int);
		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		(void)head_ptr;
	}

	{
		(void)data_ptr;
	}

}

void caller_marshal_kernel__skb_put__sk_buff_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__skb_put__sk_buff_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__skb_put__sk_buff_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__skb_put__sk_buff_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__sk_free__sk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sk_free_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__sk_free__sk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sk_free_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__sk_free__sk__io(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sk_free_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__sk_free__sk__io(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sk_free_call_ctx const* ctx,
	struct sock* ptr)
{
	
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

