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
	unsigned short const* type_ptr = &ptr->type;
	
	{
		glue_pack(__pos, __msg, __ext, *type_ptr);
	}

}

void caller_unmarshal_kernel__dev_get_by_index__ret_net_device__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct dev_get_by_index_call_ctx const* ctx,
	struct net_device* ptr)
{
	unsigned short* type_ptr = &ptr->type;
	
	{
		*type_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

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
		glue_pack(__pos, __msg, __ext, __adjusted);
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
		size_t __size = sizeof(struct sock);
		*sk_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct sock*, __size);
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
	int const* sk_err_ptr = &ptr->sk_err;
	
	{
		glue_pack(__pos, __msg, __ext, *sk_err_ptr);
	}

}

void callee_unmarshal_kernel__proto_ops_bind__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct sock* ptr)
{
	int* sk_err_ptr = &ptr->sk_err;
	
	{
		*sk_err_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

}

void callee_marshal_kernel__proto_ops_bind__socket_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct sock const* ptr)
{
	int const* sk_err_ptr = &ptr->sk_err;
	
	{
		glue_pack(__pos, __msg, __ext, *sk_err_ptr);
	}

}

void caller_unmarshal_kernel__proto_ops_bind__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct sock* ptr)
{
	int* sk_err_ptr = &ptr->sk_err;
	
	{
		*sk_err_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

}

void caller_marshal_kernel__proto_ops_bind__sockaddr_can__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct sockaddr_can const* ptr)
{
	int const* can_ifindex_ptr = &ptr->can_ifindex;
	
	{
		glue_pack(__pos, __msg, __ext, *can_ifindex_ptr);
	}

}

void callee_unmarshal_kernel__proto_ops_bind__sockaddr_can__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct sockaddr_can* ptr)
{
	int* can_ifindex_ptr = &ptr->can_ifindex;
	
	{
		*can_ifindex_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

}

void callee_marshal_kernel__proto_ops_bind__sockaddr_can__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct sockaddr_can const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_bind__sockaddr_can__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_bind_call_ctx const* ctx,
	struct sockaddr_can* ptr)
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
		glue_pack(__pos, __msg, __ext, __adjusted);
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
		size_t __size = sizeof(struct sock);
		*sk_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct sock*, __size);
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
		glue_pack(__pos, __msg, __ext, __adjusted);
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
		size_t __size = sizeof(struct sock);
		*sk_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct sock*, __size);
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

void caller_marshal_kernel__proto_ops_getname__sockaddr_can__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct sockaddr_can const* ptr)
{
	unsigned short const* can_family_ptr = &ptr->can_family;
	int const* can_ifindex_ptr = &ptr->can_ifindex;
	
	{
		glue_pack(__pos, __msg, __ext, *can_family_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *can_ifindex_ptr);
	}

}

void callee_unmarshal_kernel__proto_ops_getname__sockaddr_can__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct sockaddr_can* ptr)
{
	unsigned short* can_family_ptr = &ptr->can_family;
	int* can_ifindex_ptr = &ptr->can_ifindex;
	
	{
		*can_family_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		*can_ifindex_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

}

void callee_marshal_kernel__proto_ops_getname__sockaddr_can__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct sockaddr_can const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_getname__sockaddr_can__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_getname_call_ctx const* ctx,
	struct sockaddr_can* ptr)
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
		glue_pack(__pos, __msg, __ext, __adjusted);
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
		size_t __size = sizeof(struct sock);
		*sk_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct sock*, __size);
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
		glue_pack(__pos, __msg, __ext, __adjusted);
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
		size_t __size = sizeof(struct sock);
		*sk_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct sock*, __size);
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
		glue_pack(__pos, __msg, __ext, __adjusted);
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
		size_t __size = sizeof(struct sock);
		*sk_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct sock*, __size);
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
	
	{
		char* __casted = (char*)*msg_name_ptr;
		char* const* __casted_ptr = &__casted;
		{
			__maybe_unused const void* __adjusted = *__casted_ptr;
			glue_pack(__pos, __msg, __ext, __adjusted);
			if (*__casted_ptr) {
				size_t i, len = (ptr->msg_namelen);
				char* array = *__casted_ptr;
				glue_pack(__pos, __msg, __ext, len);
				// Warning: see David if this breaks
				glue_user_trace("Warning: see David if this breaks");
				for (i = 0; i < len; ++i) {
					char* element = &array[i];
					glue_pack(__pos, __msg, __ext, *element);
				}

			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *msg_namelen_ptr);
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
	
	{
		char* __casted = (char*)*msg_name_ptr;
		char** __casted_ptr = &__casted;
		{
			size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
			*__casted_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char*, (__size), (DEFAULT_GFP_FLAGS));
			if (*__casted_ptr) {
				int i;
				char* array = *__casted_ptr;
				size_t len = glue_unpack(__pos, __msg, __ext, size_t);
				// Warning: see David if this breaks
				glue_user_trace("Warning: see David if this breaks");
				for (i = 0; i < len; ++i) {
					char* element = &array[i];
					*element = glue_unpack(__pos, __msg, __ext, char);
				}

			}

		}

		*msg_name_ptr = (void*)__casted;
	}

	{
		*msg_namelen_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

}

void callee_marshal_kernel__proto_ops_sendmsg__msg__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct msghdr const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_sendmsg__msg__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_sendmsg_call_ctx const* ctx,
	struct msghdr* ptr)
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
		glue_pack(__pos, __msg, __ext, __adjusted);
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
		size_t __size = sizeof(struct sock);
		*sk_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct sock*, __size);
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

void caller_marshal_kernel__proto_ops_recvmsg__msg__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct msghdr const* ptr)
{
	void* const* msg_name_ptr = &ptr->msg_name;
	unsigned int const* msg_flags_ptr = &ptr->msg_flags;
	
	{
		char* __casted = (char*)*msg_name_ptr;
		char* const* __casted_ptr = &__casted;
		{
			__maybe_unused const void* __adjusted = *__casted_ptr;
			glue_pack(__pos, __msg, __ext, __adjusted);
			if (*__casted_ptr) {
				size_t i, len = (ptr->msg_namelen);
				char* array = *__casted_ptr;
				glue_pack(__pos, __msg, __ext, len);
				// Warning: see David if this breaks
				glue_user_trace("Warning: see David if this breaks");
				for (i = 0; i < len; ++i) {
					char* element = &array[i];
					glue_pack(__pos, __msg, __ext, *element);
				}

			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *msg_flags_ptr);
	}

}

void callee_unmarshal_kernel__proto_ops_recvmsg__msg__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct msghdr* ptr)
{
	void** msg_name_ptr = &ptr->msg_name;
	unsigned int* msg_flags_ptr = &ptr->msg_flags;
	
	{
		char* __casted = (char*)*msg_name_ptr;
		char** __casted_ptr = &__casted;
		{
			size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
			*__casted_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char*, (__size), (DEFAULT_GFP_FLAGS));
			if (*__casted_ptr) {
				int i;
				char* array = *__casted_ptr;
				size_t len = glue_unpack(__pos, __msg, __ext, size_t);
				// Warning: see David if this breaks
				glue_user_trace("Warning: see David if this breaks");
				for (i = 0; i < len; ++i) {
					char* element = &array[i];
					*element = glue_unpack(__pos, __msg, __ext, char);
				}

			}

		}

		*msg_name_ptr = (void*)__casted;
	}

	{
		*msg_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

}

void callee_marshal_kernel__proto_ops_recvmsg__msg__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct msghdr const* ptr)
{
	int const* msg_namelen_ptr = &ptr->msg_namelen;
	void* const* msg_name_ptr = &ptr->msg_name;
	unsigned int const* msg_flags_ptr = &ptr->msg_flags;
	
	{
		glue_pack(__pos, __msg, __ext, *msg_namelen_ptr);
	}

	{
		char* __casted = (char*)*msg_name_ptr;
		char* const* __casted_ptr = &__casted;
		{
			__maybe_unused const void* __adjusted = *__casted_ptr;
			glue_pack_shadow(__pos, __msg, __ext, __adjusted);
			if (*__casted_ptr) {
				size_t i, len = (ptr->msg_namelen);
				char* array = *__casted_ptr;
				glue_pack(__pos, __msg, __ext, len);
				// Warning: see David if this breaks
				glue_user_trace("Warning: see David if this breaks");
				for (i = 0; i < len; ++i) {
					char* element = &array[i];
					glue_pack(__pos, __msg, __ext, *element);
				}

			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *msg_flags_ptr);
	}

}

void caller_unmarshal_kernel__proto_ops_recvmsg__msg__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_recvmsg_call_ctx const* ctx,
	struct msghdr* ptr)
{
	int* msg_namelen_ptr = &ptr->msg_namelen;
	void** msg_name_ptr = &ptr->msg_name;
	unsigned int* msg_flags_ptr = &ptr->msg_flags;
	
	{
		*msg_namelen_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		char* __casted = (char*)*msg_name_ptr;
		char** __casted_ptr = &__casted;
		{
			*__casted_ptr = glue_unpack(__pos, __msg, __ext, char*);
			if (*__casted_ptr) {
				int i;
				char* array = *__casted_ptr;
				size_t len = glue_unpack(__pos, __msg, __ext, size_t);
				// Warning: see David if this breaks
				glue_user_trace("Warning: see David if this breaks");
				for (i = 0; i < len; ++i) {
					char* element = &array[i];
					*element = glue_unpack(__pos, __msg, __ext, char);
				}

			}

		}

		*msg_name_ptr = (void*)__casted;
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
	unsigned char* const* data_ptr = &ptr->data;
	
	{
		(void)data_ptr;
	}

}

void callee_unmarshal_kernel__sock_alloc_send_skb__ret_sk_buff__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned char** data_ptr = &ptr->data;
	
	{
		(void)data_ptr;
	}

}

void callee_marshal_kernel__sock_alloc_send_skb__ret_sk_buff__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned char* const* data_ptr = &ptr->data;
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	unsigned int const* len_ptr = &ptr->len;
	unsigned int const* truesize_ptr = &ptr->truesize;
	
	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			size_t i, len = (ptr->truesize);
			unsigned char* array = *data_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *truesize_ptr);
	}

}

void caller_unmarshal_kernel__sock_alloc_send_skb__ret_sk_buff__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned char** data_ptr = &ptr->data;
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	unsigned int* len_ptr = &ptr->len;
	unsigned int* truesize_ptr = &ptr->truesize;
	
	{
		size_t __size = sizeof(unsigned char) * glue_peek(__pos, __msg, __ext);
		*data_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, unsigned char*, (__size), (DEFAULT_GFP_FLAGS));
		if (*data_ptr) {
			int i;
			unsigned char* array = *data_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, unsigned char);
			}

		}

	}

	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*truesize_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

}

void caller_marshal_kernel__sock_alloc_send_skb__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct sock const* ptr)
{
	unsigned int const* sk_priority_ptr = &ptr->sk_priority;
	
	{
		glue_pack(__pos, __msg, __ext, *sk_priority_ptr);
	}

}

void callee_unmarshal_kernel__sock_alloc_send_skb__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct sock* ptr)
{
	unsigned int* sk_priority_ptr = &ptr->sk_priority;
	
	{
		*sk_priority_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

}

void callee_marshal_kernel__sock_alloc_send_skb__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_alloc_send_skb__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_alloc_send_skb_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__notifier_call__nb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct notifier_call_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	
}

void callee_unmarshal_kernel__notifier_call__nb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct notifier_call_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	
}

void callee_marshal_kernel__notifier_call__nb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct notifier_call_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	
}

void caller_unmarshal_kernel__notifier_call__nb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct notifier_call_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	
}

void caller_marshal_kernel__notifier_call__info__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct notifier_call_call_ctx const* ctx,
	struct netdev_notifier_info const* ptr)
{
	struct net_device* const* dev_ptr = &ptr->dev;
	
	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__notifier_call__net_device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

}

void callee_unmarshal_kernel__notifier_call__info__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct notifier_call_call_ctx const* ctx,
	struct netdev_notifier_info* ptr)
{
	struct net_device** dev_ptr = &ptr->dev;
	
	{
		size_t __size = sizeof(struct net_device);
		*dev_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct net_device*, (__size), (DEFAULT_GFP_FLAGS));
		if (*dev_ptr) {
			callee_unmarshal_kernel__notifier_call__net_device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

}

void callee_marshal_kernel__notifier_call__info__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct notifier_call_call_ctx const* ctx,
	struct netdev_notifier_info const* ptr)
{
	struct net_device* const* dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			callee_marshal_kernel__notifier_call__net_device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

}

void caller_unmarshal_kernel__notifier_call__info__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct notifier_call_call_ctx const* ctx,
	struct netdev_notifier_info* ptr)
{
	struct net_device** dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__notifier_call__net_device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

}

void caller_marshal_kernel__notifier_call__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct notifier_call_call_ctx const* ctx,
	struct net_device const* ptr)
{
	int const* ifindex_ptr = &ptr->ifindex;
	unsigned short const* type_ptr = &ptr->type;
	
	{
		glue_pack(__pos, __msg, __ext, *ifindex_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *type_ptr);
	}

}

void callee_unmarshal_kernel__notifier_call__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct notifier_call_call_ctx const* ctx,
	struct net_device* ptr)
{
	int* ifindex_ptr = &ptr->ifindex;
	unsigned short* type_ptr = &ptr->type;
	
	{
		*ifindex_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*type_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

}

void callee_marshal_kernel__notifier_call__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct notifier_call_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__notifier_call__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct notifier_call_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__register_netdevice_notifier__nb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct register_netdevice_notifier_call_ctx const* ctx,
	struct notifier_block const* ptr)
{
	fptr_notifier_call const* notifier_call_ptr = &ptr->notifier_call;
	
	{
		glue_pack(__pos, __msg, __ext, *notifier_call_ptr);
	}

}

void callee_unmarshal_kernel__register_netdevice_notifier__nb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct register_netdevice_notifier_call_ctx const* ctx,
	struct notifier_block* ptr)
{
	fptr_notifier_call* notifier_call_ptr = &ptr->notifier_call;
	
	{
		*notifier_call_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, notifier_call);
	}

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
	
}

void callee_unmarshal_kernel__kfree_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct kfree_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void callee_marshal_kernel__kfree_skb__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct kfree_skb_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void caller_unmarshal_kernel__kfree_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct kfree_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void caller_marshal_kernel__skb_clone__ret_sk_buff__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	struct net_device* const* dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			caller_marshal_kernel__skb_clone__sk_buff_dev__out(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

}

void callee_unmarshal_kernel__skb_clone__ret_sk_buff__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	struct net_device** dev_ptr = &ptr->dev;
	
	{
		if (*dev_ptr) {
			callee_unmarshal_kernel__skb_clone__sk_buff_dev__out(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

}

void callee_marshal_kernel__skb_clone__ret_sk_buff__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	char const* cb_ptr = ptr->cb;
	struct net_device* const* dev_ptr = &ptr->dev;
	
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
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			callee_marshal_kernel__skb_clone__sk_buff_dev__out(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

}

void caller_unmarshal_kernel__skb_clone__ret_sk_buff__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	char* cb_ptr = ptr->cb;
	struct net_device** dev_ptr = &ptr->dev;
	
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
		size_t __size = sizeof(struct net_device);
		*dev_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct net_device*, __size);
		if (*dev_ptr) {
			caller_unmarshal_kernel__skb_clone__sk_buff_dev__out(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

}

void caller_marshal_kernel__skb_clone__sk_buff_dev__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__skb_clone__sk_buff_dev__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__skb_clone__sk_buff_dev__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct net_device const* ptr)
{
	int const* ifindex_ptr = &ptr->ifindex;
	
	{
		glue_pack(__pos, __msg, __ext, *ifindex_ptr);
	}

}

void caller_unmarshal_kernel__skb_clone__sk_buff_dev__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct net_device* ptr)
{
	int* ifindex_ptr = &ptr->ifindex;
	
	{
		*ifindex_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

}

void caller_marshal_kernel__skb_clone__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void callee_unmarshal_kernel__skb_clone__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void callee_marshal_kernel__skb_clone__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void caller_unmarshal_kernel__skb_clone__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_clone_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void caller_marshal_kernel__sock_queue_rcv_skb__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_queue_rcv_skb_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_queue_rcv_skb__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_queue_rcv_skb_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__sock_queue_rcv_skb__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_queue_rcv_skb_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_queue_rcv_skb__sk__in(
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
	char const* cb_ptr = ptr->cb;
	
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

}

void callee_unmarshal_kernel__sock_queue_rcv_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_queue_rcv_skb_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	char* cb_ptr = ptr->cb;
	
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
	
}

void callee_unmarshal_kernel__can_proto_unregister__cp__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_proto_unregister_call_ctx const* ctx,
	struct can_proto* ptr)
{
	
}

void callee_marshal_kernel__can_proto_unregister__cp__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_proto_unregister_call_ctx const* ctx,
	struct can_proto const* ptr)
{
	
}

void caller_unmarshal_kernel__can_proto_unregister__cp__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_proto_unregister_call_ctx const* ctx,
	struct can_proto* ptr)
{
	
}

void caller_marshal_kernel__can_proto_register__cp__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_proto_register_call_ctx const* ctx,
	struct can_proto const* ptr)
{
	int const* type_ptr = &ptr->type;
	int const* protocol_ptr = &ptr->protocol;
	struct proto_ops* const* ops_ptr = &ptr->ops;
	struct proto* const* prot_ptr = &ptr->prot;
	
	{
		glue_pack(__pos, __msg, __ext, *type_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *protocol_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *ops_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ops_ptr) {
			caller_marshal_kernel___global_proto_ops__in(__pos, __msg, __ext, *ops_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *prot_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
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
	int* type_ptr = &ptr->type;
	int* protocol_ptr = &ptr->protocol;
	struct proto_ops** ops_ptr = &ptr->ops;
	struct proto** prot_ptr = &ptr->prot;
	
	{
		*type_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*protocol_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		size_t __size = sizeof(struct proto_ops);
		*ops_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct proto_ops*, (__size), (DEFAULT_GFP_FLAGS));
		if (*ops_ptr) {
			callee_unmarshal_kernel___global_proto_ops__in(__pos, __msg, __ext, *ops_ptr);
		}

	}

	{
		size_t __size = sizeof(struct proto);
		*prot_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct proto*, (__size), (DEFAULT_GFP_FLAGS));
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

void caller_marshal_kernel___global_proto_ops__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops const* ptr)
{
	int const* family_ptr = &ptr->family;
	fptr_proto_ops_release const* release_ptr = &ptr->release;
	fptr_proto_ops_bind const* bind_ptr = &ptr->bind;
	fptr_proto_ops_connect const* connect_ptr = &ptr->connect;
	fptr_proto_ops_socketpair const* socketpair_ptr = &ptr->socketpair;
	fptr_proto_ops_accept const* accept_ptr = &ptr->accept;
	fptr_proto_ops_getname const* getname_ptr = &ptr->getname;
	fptr_proto_ops_poll const* poll_ptr = &ptr->poll;
	fptr_proto_ops_ioctl const* ioctl_ptr = &ptr->ioctl;
	fptr_proto_ops_listen const* listen_ptr = &ptr->listen;
	fptr_proto_ops_shutdown const* shutdown_ptr = &ptr->shutdown;
	fptr_proto_ops_setsockopt const* setsockopt_ptr = &ptr->setsockopt;
	fptr_proto_ops_getsockopt const* getsockopt_ptr = &ptr->getsockopt;
	fptr_proto_ops_sendmsg const* sendmsg_ptr = &ptr->sendmsg;
	fptr_proto_ops_recvmsg const* recvmsg_ptr = &ptr->recvmsg;
	fptr_proto_ops_mmap const* mmap_ptr = &ptr->mmap;
	fptr_proto_ops_sendpage const* sendpage_ptr = &ptr->sendpage;
	
	{
		glue_pack(__pos, __msg, __ext, *family_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *release_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *bind_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *connect_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *socketpair_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *accept_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *getname_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *poll_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ioctl_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *listen_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *shutdown_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *setsockopt_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *getsockopt_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *sendmsg_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *recvmsg_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *mmap_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *sendpage_ptr);
	}

}

void callee_unmarshal_kernel___global_proto_ops__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops* ptr)
{
	int* family_ptr = &ptr->family;
	fptr_proto_ops_release* release_ptr = &ptr->release;
	fptr_proto_ops_bind* bind_ptr = &ptr->bind;
	fptr_proto_ops_connect* connect_ptr = &ptr->connect;
	fptr_proto_ops_socketpair* socketpair_ptr = &ptr->socketpair;
	fptr_proto_ops_accept* accept_ptr = &ptr->accept;
	fptr_proto_ops_getname* getname_ptr = &ptr->getname;
	fptr_proto_ops_poll* poll_ptr = &ptr->poll;
	fptr_proto_ops_ioctl* ioctl_ptr = &ptr->ioctl;
	fptr_proto_ops_listen* listen_ptr = &ptr->listen;
	fptr_proto_ops_shutdown* shutdown_ptr = &ptr->shutdown;
	fptr_proto_ops_setsockopt* setsockopt_ptr = &ptr->setsockopt;
	fptr_proto_ops_getsockopt* getsockopt_ptr = &ptr->getsockopt;
	fptr_proto_ops_sendmsg* sendmsg_ptr = &ptr->sendmsg;
	fptr_proto_ops_recvmsg* recvmsg_ptr = &ptr->recvmsg;
	fptr_proto_ops_mmap* mmap_ptr = &ptr->mmap;
	fptr_proto_ops_sendpage* sendpage_ptr = &ptr->sendpage;
	
	{
		*family_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*release_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_release);
	}

	{
		*bind_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_bind);
	}

	{
		*connect_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_connect);
	}

	{
		*socketpair_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_socketpair);
	}

	{
		*accept_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_accept);
	}

	{
		*getname_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_getname);
	}

	{
		*poll_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_poll);
	}

	{
		*ioctl_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_ioctl);
	}

	{
		*listen_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_listen);
	}

	{
		*shutdown_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_shutdown);
	}

	{
		*setsockopt_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_setsockopt);
	}

	{
		*getsockopt_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_getsockopt);
	}

	{
		*sendmsg_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_sendmsg);
	}

	{
		*recvmsg_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_recvmsg);
	}

	{
		*mmap_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_mmap);
	}

	{
		*sendpage_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, proto_ops_sendpage);
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

void caller_marshal_kernel__can_proto_register__can_proto_prot__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_proto_register_call_ctx const* ctx,
	struct proto const* ptr)
{
	char const* name_ptr = ptr->name;
	struct module* const* owner_ptr = &ptr->owner;
	unsigned int const* obj_size_ptr = &ptr->obj_size;
	fptr_proto_init const* init_ptr = &ptr->init;
	
	{
		size_t i, len = 32;
		char const* array = name_ptr;
		glue_pack(__pos, __msg, __ext, len);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char const* element = &array[i];
			glue_pack(__pos, __msg, __ext, *element);
		}

	}

	{
		__maybe_unused const void* __adjusted = *owner_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*owner_ptr) {
			caller_marshal_kernel__can_proto_register__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *obj_size_ptr);
	}

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
	char* name_ptr = ptr->name;
	struct module** owner_ptr = &ptr->owner;
	unsigned int* obj_size_ptr = &ptr->obj_size;
	fptr_proto_init* init_ptr = &ptr->init;
	
	{
		int i;
		char* array = name_ptr;
		size_t len = glue_unpack(__pos, __msg, __ext, size_t);
		// Warning: see David if this breaks
		glue_user_trace("Warning: see David if this breaks");
		for (i = 0; i < len; ++i) {
			char* element = &array[i];
			*element = glue_unpack(__pos, __msg, __ext, char);
		}

	}

	{
		size_t __size = sizeof(struct module);
		*owner_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct module*, (__size), (DEFAULT_GFP_FLAGS));
		if (*owner_ptr) {
			callee_unmarshal_kernel__can_proto_register__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

	{
		*obj_size_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

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
	struct module* const* owner_ptr = &ptr->owner;
	
	{
		if (*owner_ptr) {
			callee_marshal_kernel__can_proto_register__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

}

void caller_unmarshal_kernel__can_proto_register__can_proto_prot__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_proto_register_call_ctx const* ctx,
	struct proto* ptr)
{
	struct module** owner_ptr = &ptr->owner;
	
	{
		if (*owner_ptr) {
			caller_unmarshal_kernel__can_proto_register__owner__in(__pos, __msg, __ext, ctx, *owner_ptr);
		}

	}

}

void caller_marshal_kernel__can_proto_register__owner__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_proto_register_call_ctx const* ctx,
	struct module const* ptr)
{
	
}

void callee_unmarshal_kernel__can_proto_register__owner__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_proto_register_call_ctx const* ctx,
	struct module* ptr)
{
	
}

void callee_marshal_kernel__can_proto_register__owner__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_proto_register_call_ctx const* ctx,
	struct module const* ptr)
{
	
}

void caller_unmarshal_kernel__can_proto_register__owner__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_proto_register_call_ctx const* ctx,
	struct module* ptr)
{
	
}

void caller_marshal_kernel__func__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	unsigned int const* len_ptr = &ptr->len;
	
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__func__sk_buff_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

}

void callee_unmarshal_kernel__func__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	unsigned int* len_ptr = &ptr->len;
	
	{
		*sk_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__func__sk_buff_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

}

void callee_marshal_kernel__func__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			callee_marshal_kernel__func__sk_buff_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_unmarshal_kernel__func__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__func__sk_buff_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

}

void caller_marshal_kernel__func__sk_buff_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__func__sk_buff_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__func__sk_buff_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__func__sk_buff_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__func__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__func__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__func__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__func__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct func_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__can_rx_unregister__dev__in(
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

void callee_unmarshal_kernel__can_rx_unregister__dev__in(
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

void callee_marshal_kernel__can_rx_unregister__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_rx_unregister_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__can_rx_unregister__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_rx_unregister_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__can_rx_register__dev__in(
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

void callee_unmarshal_kernel__can_rx_register__dev__in(
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

void callee_marshal_kernel__can_rx_register__dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_rx_register_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__can_rx_register__dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_rx_register_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__can_send__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	struct net_device* const* dev_ptr = &ptr->dev;
	unsigned int const* priority_ptr = &ptr->priority;
	
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__can_send__sk_buff_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__can_send__sk_buff_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *priority_ptr);
	}

}

void callee_unmarshal_kernel__can_send__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	struct net_device** dev_ptr = &ptr->dev;
	unsigned int* priority_ptr = &ptr->priority;
	
	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__can_send__sk_buff_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__can_send__sk_buff_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*priority_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

}

void callee_marshal_kernel__can_send__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	struct sock* const* sk_ptr = &ptr->sk;
	struct net_device* const* dev_ptr = &ptr->dev;
	
	{
		if (*sk_ptr) {
			callee_marshal_kernel__can_send__sk_buff_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		if (*dev_ptr) {
			callee_marshal_kernel__can_send__sk_buff_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

}

void caller_unmarshal_kernel__can_send__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	struct sock** sk_ptr = &ptr->sk;
	struct net_device** dev_ptr = &ptr->dev;
	
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__can_send__sk_buff_sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		if (*dev_ptr) {
			caller_unmarshal_kernel__can_send__sk_buff_dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

}

void caller_marshal_kernel__can_send__sk_buff_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__can_send__sk_buff_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__can_send__sk_buff_sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__can_send__sk_buff_sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__can_send__sk_buff_dev__in(
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

void callee_unmarshal_kernel__can_send__sk_buff_dev__in(
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

void callee_marshal_kernel__can_send__sk_buff_dev__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__can_send__sk_buff_dev__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_send_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__can_ioctl__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_ioctl_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__can_ioctl__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_ioctl_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__can_ioctl__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct can_ioctl_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__can_ioctl__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct can_ioctl_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__datagram_poll__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct datagram_poll_call_ctx const* ctx,
	struct file const* ptr)
{
	
}

void callee_unmarshal_kernel__datagram_poll__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct datagram_poll_call_ctx const* ctx,
	struct file* ptr)
{
	
}

void callee_marshal_kernel__datagram_poll__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct datagram_poll_call_ctx const* ctx,
	struct file const* ptr)
{
	
}

void caller_unmarshal_kernel__datagram_poll__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct datagram_poll_call_ctx const* ctx,
	struct file* ptr)
{
	
}

void caller_marshal_kernel__datagram_poll__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct datagram_poll_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__datagram_poll__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct datagram_poll_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__datagram_poll__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct datagram_poll_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__datagram_poll__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct datagram_poll_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__datagram_poll__wait__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct datagram_poll_call_ctx const* ctx,
	struct poll_table_struct const* ptr)
{
	
}

void callee_unmarshal_kernel__datagram_poll__wait__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct datagram_poll_call_ctx const* ctx,
	struct poll_table_struct* ptr)
{
	
}

void callee_marshal_kernel__datagram_poll__wait__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct datagram_poll_call_ctx const* ctx,
	struct poll_table_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__datagram_poll__wait__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct datagram_poll_call_ctx const* ctx,
	struct poll_table_struct* ptr)
{
	
}

void caller_marshal_kernel__proto_ops_ioctl__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_ioctl_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__proto_ops_ioctl__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_ioctl_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_ioctl__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_ioctl_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_ioctl__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_ioctl_call_ctx const* ctx,
	struct socket* ptr)
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
	
}

void callee_unmarshal_kernel__proto_ops_poll__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__proto_ops_poll__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__proto_ops_poll__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct proto_ops_poll_call_ctx const* ctx,
	struct socket* ptr)
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

void caller_marshal_kernel__skb_free_datagram__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_free_datagram_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__skb_free_datagram__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_free_datagram_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__skb_free_datagram__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_free_datagram_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__skb_free_datagram__sk__in(
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
	
}

void callee_unmarshal_kernel__skb_free_datagram__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_free_datagram_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void callee_marshal_kernel__skb_free_datagram__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_free_datagram_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	
}

void caller_unmarshal_kernel__skb_free_datagram__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_free_datagram_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	
}

void caller_marshal_kernel__skb_recv_datagram__ret_sk_buff__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_recv_datagram_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned char* const* data_ptr = &ptr->data;
	
	{
		(void)data_ptr;
	}

}

void callee_unmarshal_kernel__skb_recv_datagram__ret_sk_buff__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_recv_datagram_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned char** data_ptr = &ptr->data;
	
	{
		(void)data_ptr;
	}

}

void callee_marshal_kernel__skb_recv_datagram__ret_sk_buff__out(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_recv_datagram_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* len_ptr = &ptr->len;
	unsigned char* const* data_ptr = &ptr->data;
	char const* cb_ptr = ptr->cb;
	
	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*data_ptr) {
			size_t i, len = (ptr->len);
			unsigned char* array = *data_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

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

}

void caller_unmarshal_kernel__skb_recv_datagram__ret_sk_buff__out(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_recv_datagram_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* len_ptr = &ptr->len;
	unsigned char** data_ptr = &ptr->data;
	char* cb_ptr = ptr->cb;
	
	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		size_t __size = sizeof(unsigned char) * glue_peek(__pos, __msg, __ext);
		*data_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, unsigned char*, (__size), (DEFAULT_GFP_FLAGS));
		if (*data_ptr) {
			int i;
			unsigned char* array = *data_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, unsigned char);
			}

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
	unsigned long const* sk_flags_ptr = &ptr->sk_flags;
	
	{
		glue_pack(__pos, __msg, __ext, *sk_flags_ptr);
	}

}

void callee_unmarshal_kernel____sock_recv_ts_and_drops__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __sock_recv_ts_and_drops_call_ctx const* ctx,
	struct sock* ptr)
{
	unsigned long* sk_flags_ptr = &ptr->sk_flags;
	
	{
		*sk_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
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
	unsigned int const* priority_ptr = &ptr->priority;
	
	{
		glue_pack(__pos, __msg, __ext, *priority_ptr);
	}

}

void caller_unmarshal_kernel____sock_recv_ts_and_drops__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct __sock_recv_ts_and_drops_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* priority_ptr = &ptr->priority;
	
	{
		*priority_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

}

void caller_marshal_kernel__skb_put__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	
	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

}

void callee_unmarshal_kernel__skb_put__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	
	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

}

void callee_marshal_kernel__skb_put__skb__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct sk_buff const* ptr)
{
	unsigned int const* tail_ptr = &ptr->tail;
	unsigned int const* end_ptr = &ptr->end;
	
	{
		glue_pack(__pos, __msg, __ext, *tail_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *end_ptr);
	}

}

void caller_unmarshal_kernel__skb_put__skb__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct skb_put_call_ctx const* ctx,
	struct sk_buff* ptr)
{
	unsigned int* tail_ptr = &ptr->tail;
	unsigned int* end_ptr = &ptr->end;
	
	{
		*tail_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*end_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

}

void caller_marshal_kernel__lvd_sock_put__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_sock_put_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__lvd_sock_put__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_sock_put_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__lvd_sock_put__sk__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_sock_put_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__lvd_sock_put__sk__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_sock_put_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void caller_marshal_kernel__sock_no_mmap__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_mmap_call_ctx const* ctx,
	struct file const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_no_mmap__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_mmap_call_ctx const* ctx,
	struct file* ptr)
{
	
}

void callee_marshal_kernel__sock_no_mmap__file__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_mmap_call_ctx const* ctx,
	struct file const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_no_mmap__file__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_mmap_call_ctx const* ctx,
	struct file* ptr)
{
	
}

void caller_marshal_kernel__sock_no_mmap__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_mmap_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_no_mmap__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_mmap_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__sock_no_mmap__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_mmap_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_no_mmap__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_mmap_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__sock_no_mmap__vma__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_mmap_call_ctx const* ctx,
	struct vm_area_struct const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_no_mmap__vma__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_mmap_call_ctx const* ctx,
	struct vm_area_struct* ptr)
{
	
}

void callee_marshal_kernel__sock_no_mmap__vma__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_mmap_call_ctx const* ctx,
	struct vm_area_struct const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_no_mmap__vma__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_mmap_call_ctx const* ctx,
	struct vm_area_struct* ptr)
{
	
}

void caller_marshal_kernel__sock_no_shutdown__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_shutdown_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_no_shutdown__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_shutdown_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__sock_no_shutdown__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_shutdown_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_no_shutdown__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_shutdown_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__sock_no_listen__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_listen_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_no_listen__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_listen_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__sock_no_listen__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_listen_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_no_listen__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_listen_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__sock_no_accept__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_accept_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_no_accept__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_accept_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__sock_no_accept__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_accept_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_no_accept__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_accept_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__sock_no_accept__newsock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_accept_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_no_accept__newsock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_accept_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__sock_no_accept__newsock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_accept_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_no_accept__newsock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_accept_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__sock_no_socketpair__sock1__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_socketpair_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_no_socketpair__sock1__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_socketpair_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__sock_no_socketpair__sock1__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_socketpair_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_no_socketpair__sock1__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_socketpair_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__sock_no_socketpair__sock2__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_socketpair_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_no_socketpair__sock2__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_socketpair_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__sock_no_socketpair__sock2__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_socketpair_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_no_socketpair__sock2__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_socketpair_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__sock_no_connect__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_connect_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_no_connect__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_connect_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__sock_no_connect__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_connect_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_no_connect__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_connect_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__sock_no_connect__saddr__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_connect_call_ctx const* ctx,
	struct sockaddr const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_no_connect__saddr__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_connect_call_ctx const* ctx,
	struct sockaddr* ptr)
{
	
}

void callee_marshal_kernel__sock_no_connect__saddr__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_connect_call_ctx const* ctx,
	struct sockaddr const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_no_connect__saddr__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_connect_call_ctx const* ctx,
	struct sockaddr* ptr)
{
	
}

void caller_marshal_kernel__sock_no_sendpage__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_sendpage_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_no_sendpage__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_sendpage_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void callee_marshal_kernel__sock_no_sendpage__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_sendpage_call_ctx const* ctx,
	struct socket const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_no_sendpage__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_sendpage_call_ctx const* ctx,
	struct socket* ptr)
{
	
}

void caller_marshal_kernel__sock_no_sendpage__page__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_sendpage_call_ctx const* ctx,
	struct page const* ptr)
{
	
}

void callee_unmarshal_kernel__sock_no_sendpage__page__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_sendpage_call_ctx const* ctx,
	struct page* ptr)
{
	
}

void callee_marshal_kernel__sock_no_sendpage__page__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct sock_no_sendpage_call_ctx const* ctx,
	struct page const* ptr)
{
	
}

void caller_unmarshal_kernel__sock_no_sendpage__page__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct sock_no_sendpage_call_ctx const* ctx,
	struct page* ptr)
{
	
}

void caller_marshal_kernel__lvd_memcpy_to_msg__msg__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_memcpy_to_msg_call_ctx const* ctx,
	struct msghdr const* ptr)
{
	
}

void callee_unmarshal_kernel__lvd_memcpy_to_msg__msg__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_memcpy_to_msg_call_ctx const* ctx,
	struct msghdr* ptr)
{
	
}

void callee_marshal_kernel__lvd_memcpy_to_msg__msg__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_memcpy_to_msg_call_ctx const* ctx,
	struct msghdr const* ptr)
{
	
}

void caller_unmarshal_kernel__lvd_memcpy_to_msg__msg__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_memcpy_to_msg_call_ctx const* ctx,
	struct msghdr* ptr)
{
	
}

void caller_marshal_kernel__lvd_memcpy_from_msg__msg__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_memcpy_from_msg_call_ctx const* ctx,
	struct msghdr const* ptr)
{
	
}

void callee_unmarshal_kernel__lvd_memcpy_from_msg__msg__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_memcpy_from_msg_call_ctx const* ctx,
	struct msghdr* ptr)
{
	
}

void callee_marshal_kernel__lvd_memcpy_from_msg__msg__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_memcpy_from_msg_call_ctx const* ctx,
	struct msghdr const* ptr)
{
	
}

void caller_unmarshal_kernel__lvd_memcpy_from_msg__msg__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_memcpy_from_msg_call_ctx const* ctx,
	struct msghdr* ptr)
{
	
}

void caller_marshal_kernel__lvd_dev_put__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_dev_put_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void callee_unmarshal_kernel__lvd_dev_put__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_dev_put_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void callee_marshal_kernel__lvd_dev_put__net_device__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_dev_put_call_ctx const* ctx,
	struct net_device const* ptr)
{
	
}

void caller_unmarshal_kernel__lvd_dev_put__net_device__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_dev_put_call_ctx const* ctx,
	struct net_device* ptr)
{
	
}

void caller_marshal_kernel__lvd_sock_orphan__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_sock_orphan_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void callee_unmarshal_kernel__lvd_sock_orphan__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_sock_orphan_call_ctx const* ctx,
	struct sock* ptr)
{
	
}

void callee_marshal_kernel__lvd_sock_orphan__sock__in(
	size_t* __pos,
	struct fipc_message* __msg,
	struct ext_registers* __ext,
	struct lvd_sock_orphan_call_ctx const* ctx,
	struct sock const* ptr)
{
	
}

void caller_unmarshal_kernel__lvd_sock_orphan__sock__in(
	size_t* __pos,
	const struct fipc_message* __msg,
	const struct ext_registers* __ext,
	struct lvd_sock_orphan_call_ctx const* ctx,
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

