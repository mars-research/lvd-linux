#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

void proto_init_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_init function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_init);
	struct sock* sk = 0;
	struct sock** sk_ptr = &sk;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct proto_init_call_ctx call_ctx = {sk};
	__maybe_unused struct proto_init_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sk_ptr = glue_unpack(__pos, msg, ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__proto_init__sk__in(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	ret = function_ptr(sk);

	*__pos = 0;
	{
		if (*sk_ptr) {
			callee_marshal_kernel__proto_init__sk__in(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct net_device* dev_get_by_index(struct net* net, int ifindex)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net** net_ptr = &net;
	int* ifindex_ptr = &ifindex;
	struct net_device* ret = 0;
	struct net_device** ret_ptr = &ret;
	
	__maybe_unused const struct dev_get_by_index_call_ctx call_ctx = {net, ifindex};
	__maybe_unused const struct dev_get_by_index_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *net_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*net_ptr) {
			caller_marshal_kernel__dev_get_by_index__net__in(__pos, msg, ext, ctx, *net_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ifindex_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_dev_get_by_index);

	*__pos = 0;
	{
		if (*net_ptr) {
			caller_unmarshal_kernel__dev_get_by_index__net__in(__pos, msg, ext, ctx, *net_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, struct net_device*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__dev_get_by_index__ret_net_device__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void proto_ops_bind_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_bind function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_bind);
	struct socket* sock = 0;
	struct sockaddr* uaddr = 0;
	int len = 0;
	struct socket** sock_ptr = &sock;
	struct sockaddr** uaddr_ptr = &uaddr;
	int* len_ptr = &len;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_bind_call_ctx call_ctx = {sock, uaddr, len};
	__maybe_unused struct proto_ops_bind_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__proto_ops_bind__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		*uaddr_ptr = glue_unpack(__pos, msg, ext, struct sockaddr*);
		if (*uaddr_ptr) {
			callee_unmarshal_kernel__proto_ops_bind__uaddr__in(__pos, msg, ext, ctx, *uaddr_ptr);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = function_ptr(sock, uaddr, len);

	*__pos = 0;
	{
		if (*sock_ptr) {
			callee_marshal_kernel__proto_ops_bind__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		if (*uaddr_ptr) {
			callee_marshal_kernel__proto_ops_bind__uaddr__in(__pos, msg, ext, ctx, *uaddr_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void proto_ops_release_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_release function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_release);
	struct socket* sock = 0;
	struct socket** sock_ptr = &sock;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_release_call_ctx call_ctx = {sock};
	__maybe_unused struct proto_ops_release_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__proto_ops_release__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	ret = function_ptr(sock);

	*__pos = 0;
	{
		if (*sock_ptr) {
			callee_marshal_kernel__proto_ops_release__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void proto_ops_getname_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_getname function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_getname);
	struct socket* sock = 0;
	struct sockaddr* uaddr = 0;
	int* len = 0;
	int peer = 0;
	struct socket** sock_ptr = &sock;
	struct sockaddr** uaddr_ptr = &uaddr;
	int** len_ptr = &len;
	int* peer_ptr = &peer;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_getname_call_ctx call_ctx = {sock, uaddr, len, peer};
	__maybe_unused struct proto_ops_getname_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__proto_ops_getname__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		*uaddr_ptr = glue_unpack(__pos, msg, ext, struct sockaddr*);
		if (*uaddr_ptr) {
			callee_unmarshal_kernel__proto_ops_getname__uaddr__in(__pos, msg, ext, ctx, *uaddr_ptr);
		}

	}

	{
		(void)len_ptr;
	}

	{
		*peer_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = function_ptr(sock, uaddr, len, peer);

	*__pos = 0;
	{
		if (*sock_ptr) {
			callee_marshal_kernel__proto_ops_getname__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		if (*uaddr_ptr) {
			callee_marshal_kernel__proto_ops_getname__uaddr__in(__pos, msg, ext, ctx, *uaddr_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *len_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*len_ptr) {
			glue_pack(__pos, msg, ext, **len_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void proto_ops_setsockopt_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_setsockopt function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_setsockopt);
	struct socket* sock = 0;
	int level = 0;
	int optname = 0;
	char* optval = 0;
	unsigned int optlen = 0;
	struct socket** sock_ptr = &sock;
	int* level_ptr = &level;
	int* optname_ptr = &optname;
	char** optval_ptr = &optval;
	unsigned int* optlen_ptr = &optlen;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_setsockopt_call_ctx call_ctx = {sock, level, optname, optval, optlen};
	__maybe_unused struct proto_ops_setsockopt_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__proto_ops_setsockopt__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		*level_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*optname_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*optval_ptr = glue_unpack(__pos, msg, ext, char*);
		if (*optval_ptr) {
			**optval_ptr = glue_unpack(__pos, msg, ext, char);
		}

	}

	{
		*optlen_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	ret = function_ptr(sock, level, optname, optval, optlen);

	*__pos = 0;
	{
		if (*sock_ptr) {
			callee_marshal_kernel__proto_ops_setsockopt__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
	}

	{
	}

	{
		(void)optval_ptr;
	}

	{
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void proto_ops_getsockopt_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_getsockopt function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_getsockopt);
	struct socket* sock = 0;
	int level = 0;
	int optname = 0;
	char* optval = 0;
	int* optlen = 0;
	struct socket** sock_ptr = &sock;
	int* level_ptr = &level;
	int* optname_ptr = &optname;
	char** optval_ptr = &optval;
	int** optlen_ptr = &optlen;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_getsockopt_call_ctx call_ctx = {sock, level, optname, optval, optlen};
	__maybe_unused struct proto_ops_getsockopt_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__proto_ops_getsockopt__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		*level_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*optname_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*optval_ptr = glue_unpack(__pos, msg, ext, char*);
		if (*optval_ptr) {
			**optval_ptr = glue_unpack(__pos, msg, ext, char);
		}

	}

	{
		*optlen_ptr = glue_unpack(__pos, msg, ext, int*);
		if (*optlen_ptr) {
			**optlen_ptr = glue_unpack(__pos, msg, ext, int);
		}

	}

	ret = function_ptr(sock, level, optname, optval, optlen);

	*__pos = 0;
	{
		if (*sock_ptr) {
			callee_marshal_kernel__proto_ops_getsockopt__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
	}

	{
	}

	{
		(void)optval_ptr;
	}

	{
		(void)optlen_ptr;
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void proto_ops_sendmsg_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_sendmsg function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_sendmsg);
	struct socket* sock = 0;
	struct msghdr* msg = 0;
	unsigned long size = 0;
	struct socket** sock_ptr = &sock;
	struct msghdr** msg_ptr = &msg;
	unsigned long* size_ptr = &size;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_sendmsg_call_ctx call_ctx = {sock, msg, size};
	__maybe_unused struct proto_ops_sendmsg_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__proto_ops_sendmsg__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		*msg_ptr = glue_unpack(__pos, msg, ext, struct msghdr*);
		if (*msg_ptr) {
			callee_unmarshal_kernel__proto_ops_sendmsg__msg__in(__pos, msg, ext, ctx, *msg_ptr);
		}

	}

	{
		*size_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	ret = function_ptr(sock, msg, size);

	*__pos = 0;
	{
		if (*sock_ptr) {
			callee_marshal_kernel__proto_ops_sendmsg__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		if (*msg_ptr) {
			callee_marshal_kernel__proto_ops_sendmsg__msg__in(__pos, msg, ext, ctx, *msg_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void proto_ops_recvmsg_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_recvmsg function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_recvmsg);
	struct socket* sock = 0;
	struct msghdr* msg = 0;
	unsigned long size = 0;
	int flags = 0;
	struct socket** sock_ptr = &sock;
	struct msghdr** msg_ptr = &msg;
	unsigned long* size_ptr = &size;
	int* flags_ptr = &flags;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_recvmsg_call_ctx call_ctx = {sock, msg, size, flags};
	__maybe_unused struct proto_ops_recvmsg_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__proto_ops_recvmsg__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		*msg_ptr = glue_unpack(__pos, msg, ext, struct msghdr*);
		if (*msg_ptr) {
			callee_unmarshal_kernel__proto_ops_recvmsg__msg__in(__pos, msg, ext, ctx, *msg_ptr);
		}

	}

	{
		*size_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		*flags_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = function_ptr(sock, msg, size, flags);

	*__pos = 0;
	{
		if (*sock_ptr) {
			callee_marshal_kernel__proto_ops_recvmsg__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		if (*msg_ptr) {
			callee_marshal_kernel__proto_ops_recvmsg__msg__in(__pos, msg, ext, ctx, *msg_ptr);
		}

	}

	{
	}

	{
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void proto_ops_mmap_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_mmap function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_mmap);
	struct file* file = 0;
	struct socket* sock = 0;
	struct vm_area_struct* vma = 0;
	struct socket** sock_ptr = &sock;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_mmap_call_ctx call_ctx = {file, sock, vma};
	__maybe_unused struct proto_ops_mmap_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__proto_ops_mmap__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	ret = function_ptr(file, sock, vma);

	*__pos = 0;
	{
		if (*sock_ptr) {
			callee_marshal_kernel__proto_ops_mmap__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void proto_ops_shutdown_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_shutdown function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_shutdown);
	struct socket* sock = 0;
	int how = 0;
	struct socket** sock_ptr = &sock;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_shutdown_call_ctx call_ctx = {sock, how};
	__maybe_unused struct proto_ops_shutdown_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__proto_ops_shutdown__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	ret = function_ptr(sock, how);

	*__pos = 0;
	{
		if (*sock_ptr) {
			callee_marshal_kernel__proto_ops_shutdown__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void proto_ops_listen_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_listen function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_listen);
	struct socket* sock = 0;
	int backlog = 0;
	struct socket** sock_ptr = &sock;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_listen_call_ctx call_ctx = {sock, backlog};
	__maybe_unused struct proto_ops_listen_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__proto_ops_listen__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	ret = function_ptr(sock, backlog);

	*__pos = 0;
	{
		if (*sock_ptr) {
			callee_marshal_kernel__proto_ops_listen__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void proto_ops_accept_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_accept function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_accept);
	struct socket* sock = 0;
	struct socket* newsock = 0;
	int flags = 0;
	struct socket** sock_ptr = &sock;
	struct socket** newsock_ptr = &newsock;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_accept_call_ctx call_ctx = {sock, newsock, flags};
	__maybe_unused struct proto_ops_accept_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__proto_ops_accept__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		*newsock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*newsock_ptr) {
			callee_unmarshal_kernel__proto_ops_accept__newsock__in(__pos, msg, ext, ctx, *newsock_ptr);
		}

	}

	ret = function_ptr(sock, newsock, flags);

	*__pos = 0;
	{
		if (*sock_ptr) {
			callee_marshal_kernel__proto_ops_accept__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		if (*newsock_ptr) {
			callee_marshal_kernel__proto_ops_accept__newsock__in(__pos, msg, ext, ctx, *newsock_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void proto_ops_socketpair_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_socketpair function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_socketpair);
	struct socket* sock1 = 0;
	struct socket* sock2 = 0;
	struct socket** sock1_ptr = &sock1;
	struct socket** sock2_ptr = &sock2;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_socketpair_call_ctx call_ctx = {sock1, sock2};
	__maybe_unused struct proto_ops_socketpair_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sock1_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock1_ptr) {
			callee_unmarshal_kernel__proto_ops_socketpair__sock1__in(__pos, msg, ext, ctx, *sock1_ptr);
		}

	}

	{
		*sock2_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock2_ptr) {
			callee_unmarshal_kernel__proto_ops_socketpair__sock2__in(__pos, msg, ext, ctx, *sock2_ptr);
		}

	}

	ret = function_ptr(sock1, sock2);

	*__pos = 0;
	{
		if (*sock1_ptr) {
			callee_marshal_kernel__proto_ops_socketpair__sock1__in(__pos, msg, ext, ctx, *sock1_ptr);
		}

	}

	{
		if (*sock2_ptr) {
			callee_marshal_kernel__proto_ops_socketpair__sock2__in(__pos, msg, ext, ctx, *sock2_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void proto_ops_connect_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_connect function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_connect);
	struct socket* sock = 0;
	struct sockaddr* saddr = 0;
	int len = 0;
	int flags = 0;
	struct socket** sock_ptr = &sock;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_connect_call_ctx call_ctx = {sock, saddr, len, flags};
	__maybe_unused struct proto_ops_connect_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__proto_ops_connect__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	ret = function_ptr(sock, saddr, len, flags);

	*__pos = 0;
	{
		if (*sock_ptr) {
			callee_marshal_kernel__proto_ops_connect__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct sk_buff* sock_alloc_send_skb(struct sock* sk, unsigned long size, int noblock, int* errcode)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock** sk_ptr = &sk;
	unsigned long* size_ptr = &size;
	int* noblock_ptr = &noblock;
	int** errcode_ptr = &errcode;
	struct sk_buff* ret = 0;
	struct sk_buff** ret_ptr = &ret;
	
	__maybe_unused const struct sock_alloc_send_skb_call_ctx call_ctx = {sk, size, noblock, errcode};
	__maybe_unused const struct sock_alloc_send_skb_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__sock_alloc_send_skb__sk__in(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *size_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *noblock_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *errcode_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*errcode_ptr) {
			glue_pack(__pos, msg, ext, **errcode_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_sock_alloc_send_skb);

	*__pos = 0;
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__sock_alloc_send_skb__sk__in(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	{
	}

	{
	}

	{
		(void)errcode_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, struct sk_buff*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__sock_alloc_send_skb__ret_sk_buff__out(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void warn_slowpath_fmt(char* file, int line, char* fmt)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	char** file_ptr = &file;
	int* line_ptr = &line;
	char** fmt_ptr = &fmt;
	
	__maybe_unused const struct warn_slowpath_fmt_call_ctx call_ctx = {file, line, fmt};
	__maybe_unused const struct warn_slowpath_fmt_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *file_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*file_ptr) {
			glue_pack(__pos, msg, ext, **file_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *line_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *fmt_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*fmt_ptr) {
			glue_pack(__pos, msg, ext, **fmt_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_warn_slowpath_fmt);

	*__pos = 0;
	{
		(void)file_ptr;
	}

	{
	}

	{
		(void)fmt_ptr;
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int register_netdevice_notifier(struct notifier_block* nb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct notifier_block** nb_ptr = &nb;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct register_netdevice_notifier_call_ctx call_ctx = {nb};
	__maybe_unused const struct register_netdevice_notifier_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *nb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*nb_ptr) {
			caller_marshal_kernel__register_netdevice_notifier__nb__in(__pos, msg, ext, ctx, *nb_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_register_netdevice_notifier);

	*__pos = 0;
	{
		if (*nb_ptr) {
			caller_unmarshal_kernel__register_netdevice_notifier__nb__in(__pos, msg, ext, ctx, *nb_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int unregister_netdevice_notifier(struct notifier_block* nb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct notifier_block** nb_ptr = &nb;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct unregister_netdevice_notifier_call_ctx call_ctx = {nb};
	__maybe_unused const struct unregister_netdevice_notifier_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *nb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*nb_ptr) {
			caller_marshal_kernel__unregister_netdevice_notifier__nb__in(__pos, msg, ext, ctx, *nb_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_unregister_netdevice_notifier);

	*__pos = 0;
	{
		if (*nb_ptr) {
			caller_unmarshal_kernel__unregister_netdevice_notifier__nb__in(__pos, msg, ext, ctx, *nb_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void kfree_skb(struct sk_buff* skb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	
	__maybe_unused const struct kfree_skb_call_ctx call_ctx = {skb};
	__maybe_unused const struct kfree_skb_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__kfree_skb__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_kfree_skb);

	*__pos = 0;
	{
		if (*skb_ptr) {
			caller_unmarshal_kernel__kfree_skb__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct sk_buff* skb_clone(struct sk_buff* skb, unsigned int gfp_mask)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	unsigned int* gfp_mask_ptr = &gfp_mask;
	struct sk_buff* ret = 0;
	struct sk_buff** ret_ptr = &ret;
	
	__maybe_unused const struct skb_clone_call_ctx call_ctx = {skb, gfp_mask};
	__maybe_unused const struct skb_clone_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__skb_clone__skb__io(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *gfp_mask_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_skb_clone);

	*__pos = 0;
	{
		*skb_ptr = glue_unpack_shadow(__pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			caller_unmarshal_kernel__skb_clone__skb__io(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack_shadow(__pos, msg, ext, struct sk_buff*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__skb_clone__ret_sk_buff__io(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void _raw_write_lock_bh(struct rwlock_t* lock)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct rwlock_t** lock_ptr = &lock;
	
	__maybe_unused const struct _raw_write_lock_bh_call_ctx call_ctx = {lock};
	__maybe_unused const struct _raw_write_lock_bh_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *lock_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*lock_ptr) {
			caller_marshal_kernel___raw_write_lock_bh__lock__in(__pos, msg, ext, ctx, *lock_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID__raw_write_lock_bh);

	*__pos = 0;
	{
		if (*lock_ptr) {
			caller_unmarshal_kernel___raw_write_lock_bh__lock__in(__pos, msg, ext, ctx, *lock_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void _raw_write_unlock_bh(struct rwlock_t* lock)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct rwlock_t** lock_ptr = &lock;
	
	__maybe_unused const struct _raw_write_unlock_bh_call_ctx call_ctx = {lock};
	__maybe_unused const struct _raw_write_unlock_bh_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *lock_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*lock_ptr) {
			caller_marshal_kernel___raw_write_unlock_bh__lock__in(__pos, msg, ext, ctx, *lock_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID__raw_write_unlock_bh);

	*__pos = 0;
	{
		if (*lock_ptr) {
			caller_unmarshal_kernel___raw_write_unlock_bh__lock__in(__pos, msg, ext, ctx, *lock_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void* __alloc_percpu(unsigned long size, unsigned long align)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long* size_ptr = &size;
	unsigned long* align_ptr = &align;
	void* ret = 0;
	void** ret_ptr = &ret;
	
	__maybe_unused const struct __alloc_percpu_call_ctx call_ctx = {size, align};
	__maybe_unused const struct __alloc_percpu_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *size_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *align_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID___alloc_percpu);

	*__pos = 0;
	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, void*);
		if (*ret_ptr) {
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void free_percpu(void* ptr)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	void** ptr_ptr = &ptr;
	
	__maybe_unused const struct free_percpu_call_ctx call_ctx = {ptr};
	__maybe_unused const struct free_percpu_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *ptr_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*ptr_ptr) {
		}

	}

	glue_call_server(__pos, msg, RPC_ID_free_percpu);

	*__pos = 0;
	{
		(void)ptr_ptr;
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int sock_queue_rcv_skb(struct sock* sk, struct sk_buff* skb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock** sk_ptr = &sk;
	struct sk_buff** skb_ptr = &skb;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct sock_queue_rcv_skb_call_ctx call_ctx = {sk, skb};
	__maybe_unused const struct sock_queue_rcv_skb_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__sock_queue_rcv_skb__sk__io(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__sock_queue_rcv_skb__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_sock_queue_rcv_skb);

	*__pos = 0;
	{
		*sk_ptr = glue_unpack_shadow(__pos, msg, ext, struct sock*);
		if (*sk_ptr) {
			caller_unmarshal_kernel__sock_queue_rcv_skb__sk__io(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	{
		if (*skb_ptr) {
			caller_unmarshal_kernel__sock_queue_rcv_skb__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void can_proto_unregister(struct can_proto* cp)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct can_proto** cp_ptr = &cp;
	
	__maybe_unused const struct can_proto_unregister_call_ctx call_ctx = {cp};
	__maybe_unused const struct can_proto_unregister_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *cp_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*cp_ptr) {
			caller_marshal_kernel__can_proto_unregister__cp__in(__pos, msg, ext, ctx, *cp_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_can_proto_unregister);

	*__pos = 0;
	{
		if (*cp_ptr) {
			caller_unmarshal_kernel__can_proto_unregister__cp__in(__pos, msg, ext, ctx, *cp_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int can_proto_register(struct can_proto* cp)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct can_proto** cp_ptr = &cp;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct can_proto_register_call_ctx call_ctx = {cp};
	__maybe_unused const struct can_proto_register_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *cp_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*cp_ptr) {
			caller_marshal_kernel__can_proto_register__cp__in(__pos, msg, ext, ctx, *cp_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_can_proto_register);

	*__pos = 0;
	{
		if (*cp_ptr) {
			caller_unmarshal_kernel__can_proto_register__cp__in(__pos, msg, ext, ctx, *cp_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void func_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_func function_ptr = glue_unpack(__pos, msg, ext, fptr_func);
	struct sk_buff* skb = 0;
	void* data = 0;
	struct sk_buff** skb_ptr = &skb;
	void** data_ptr = &data;
	
	__maybe_unused struct func_call_ctx call_ctx = {skb, data};
	__maybe_unused struct func_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack(__pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__func__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		*data_ptr = glue_unpack(__pos, msg, ext, void*);
		if (*data_ptr) {
		}

	}

	function_ptr(skb, data);

	*__pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel__func__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		(void)data_ptr;
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void can_rx_unregister(struct net_device* dev, unsigned int can_id, unsigned int mask, fptr_func func, void* data)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	unsigned int* can_id_ptr = &can_id;
	unsigned int* mask_ptr = &mask;
	fptr_func* func_ptr = &func;
	void** data_ptr = &data;
	
	__maybe_unused const struct can_rx_unregister_call_ctx call_ctx = {dev, can_id, mask, func, data};
	__maybe_unused const struct can_rx_unregister_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__can_rx_unregister__dev__io(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *can_id_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *mask_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *func_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*data_ptr) {
		}

	}

	glue_call_server(__pos, msg, RPC_ID_can_rx_unregister);

	*__pos = 0;
	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			caller_unmarshal_kernel__can_rx_unregister__dev__io(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
	}

	{
	}

	{
		(void)data_ptr;
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int can_rx_register(struct net_device* dev, unsigned int can_id, unsigned int mask, fptr_func func, void* data, char* ident)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device** dev_ptr = &dev;
	unsigned int* can_id_ptr = &can_id;
	unsigned int* mask_ptr = &mask;
	fptr_func* func_ptr = &func;
	void** data_ptr = &data;
	char** ident_ptr = &ident;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct can_rx_register_call_ctx call_ctx = {dev, can_id, mask, func, data, ident};
	__maybe_unused const struct can_rx_register_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *dev_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*dev_ptr) {
			caller_marshal_kernel__can_rx_register__dev__io(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *can_id_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *mask_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *func_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *data_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*data_ptr) {
		}

	}

	{
		__maybe_unused const void* __adjusted = *ident_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*ident_ptr) {
			glue_pack(__pos, msg, ext, **ident_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_can_rx_register);

	*__pos = 0;
	{
		*dev_ptr = glue_unpack_shadow(__pos, msg, ext, struct net_device*);
		if (*dev_ptr) {
			caller_unmarshal_kernel__can_rx_register__dev__io(__pos, msg, ext, ctx, *dev_ptr);
		}

	}

	{
	}

	{
	}

	{
	}

	{
		(void)data_ptr;
	}

	{
		(void)ident_ptr;
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

int can_send(struct sk_buff* skb, int loop)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	int* loop_ptr = &loop;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct can_send_call_ctx call_ctx = {skb, loop};
	__maybe_unused const struct can_send_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__can_send__skb__io(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *loop_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_can_send);

	*__pos = 0;
	{
		*skb_ptr = glue_unpack_shadow(__pos, msg, ext, struct sk_buff*);
		if (*skb_ptr) {
			caller_unmarshal_kernel__can_send__skb__io(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void proto_ops_ioctl_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_ioctl function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_ioctl);
	struct socket* sock = 0;
	unsigned int cmd = 0;
	unsigned long arg = 0;
	struct socket** sock_ptr = &sock;
	unsigned int* cmd_ptr = &cmd;
	unsigned long* arg_ptr = &arg;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_ioctl_call_ctx call_ctx = {sock, cmd, arg};
	__maybe_unused struct proto_ops_ioctl_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__proto_ops_ioctl__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		*cmd_ptr = glue_unpack(__pos, msg, ext, unsigned int);
	}

	{
		*arg_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	ret = function_ptr(sock, cmd, arg);

	*__pos = 0;
	{
		if (*sock_ptr) {
			callee_marshal_kernel__proto_ops_ioctl__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
	}

	{
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

unsigned long copy_from_iter(void* addr, unsigned long bytes, struct iov_iter* i)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	void** addr_ptr = &addr;
	unsigned long* bytes_ptr = &bytes;
	struct iov_iter** i_ptr = &i;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused const struct copy_from_iter_call_ctx call_ctx = {addr, bytes, i};
	__maybe_unused const struct copy_from_iter_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *addr_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*addr_ptr) {
		}

	}

	{
		glue_pack(__pos, msg, ext, *bytes_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *i_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*i_ptr) {
			caller_marshal_kernel__copy_from_iter__i__in(__pos, msg, ext, ctx, *i_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_copy_from_iter);

	*__pos = 0;
	{
		*addr_ptr = glue_unpack_shadow(__pos, msg, ext, void*);
		if (*addr_ptr) {
		}

	}

	{
	}

	{
		if (*i_ptr) {
			caller_unmarshal_kernel__copy_from_iter__i__in(__pos, msg, ext, ctx, *i_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

unsigned long copy_to_iter(void const* addr, unsigned long bytes, struct iov_iter* i)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	void const** addr_ptr = &addr;
	unsigned long* bytes_ptr = &bytes;
	struct iov_iter** i_ptr = &i;
	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused const struct copy_to_iter_call_ctx call_ctx = {addr, bytes, i};
	__maybe_unused const struct copy_to_iter_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *addr_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*addr_ptr) {
		}

	}

	{
		glue_pack(__pos, msg, ext, *bytes_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *i_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*i_ptr) {
			caller_marshal_kernel__copy_to_iter__i__in(__pos, msg, ext, ctx, *i_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_copy_to_iter);

	*__pos = 0;
	{
		*addr_ptr = glue_unpack_shadow(__pos, msg, ext, void const*);
		if (*addr_ptr) {
			void* writable = (void*)*addr_ptr;
		}

	}

	{
	}

	{
		if (*i_ptr) {
			caller_unmarshal_kernel__copy_to_iter__i__in(__pos, msg, ext, ctx, *i_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void proto_ops_poll_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_poll function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_poll);
	struct file* file = 0;
	struct socket* sock = 0;
	struct poll_table_struct* wait = 0;
	struct file** file_ptr = &file;
	struct socket** sock_ptr = &sock;
	struct poll_table_struct** wait_ptr = &wait;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_poll_call_ctx call_ctx = {file, sock, wait};
	__maybe_unused struct proto_ops_poll_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*file_ptr = glue_unpack(__pos, msg, ext, struct file*);
		if (*file_ptr) {
			callee_unmarshal_kernel__proto_ops_poll__file__in(__pos, msg, ext, ctx, *file_ptr);
		}

	}

	{
		*sock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__proto_ops_poll__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		*wait_ptr = glue_unpack(__pos, msg, ext, struct poll_table_struct*);
		if (*wait_ptr) {
			callee_unmarshal_kernel__proto_ops_poll__wait__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	ret = function_ptr(file, sock, wait);

	*__pos = 0;
	{
		if (*file_ptr) {
			callee_marshal_kernel__proto_ops_poll__file__in(__pos, msg, ext, ctx, *file_ptr);
		}

	}

	{
		if (*sock_ptr) {
			callee_marshal_kernel__proto_ops_poll__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		if (*wait_ptr) {
			callee_marshal_kernel__proto_ops_poll__wait__in(__pos, msg, ext, ctx, *wait_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void release_sock(struct sock* sk)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock** sk_ptr = &sk;
	
	__maybe_unused const struct release_sock_call_ctx call_ctx = {sk};
	__maybe_unused const struct release_sock_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__release_sock__sk__in(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_release_sock);

	*__pos = 0;
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__release_sock__sk__in(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void skb_free_datagram(struct sock* sk, struct sk_buff* skb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock** sk_ptr = &sk;
	struct sk_buff** skb_ptr = &skb;
	
	__maybe_unused const struct skb_free_datagram_call_ctx call_ctx = {sk, skb};
	__maybe_unused const struct skb_free_datagram_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__skb_free_datagram__sk__io(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__skb_free_datagram__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_skb_free_datagram);

	*__pos = 0;
	{
		*sk_ptr = glue_unpack_shadow(__pos, msg, ext, struct sock*);
		if (*sk_ptr) {
			caller_unmarshal_kernel__skb_free_datagram__sk__io(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	{
		if (*skb_ptr) {
			caller_unmarshal_kernel__skb_free_datagram__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

struct sk_buff* skb_recv_datagram(struct sock* sk, unsigned int flags, int noblock, int* err)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock** sk_ptr = &sk;
	unsigned int* flags_ptr = &flags;
	int* noblock_ptr = &noblock;
	int** err_ptr = &err;
	struct sk_buff* ret = 0;
	struct sk_buff** ret_ptr = &ret;
	
	__maybe_unused const struct skb_recv_datagram_call_ctx call_ctx = {sk, flags, noblock, err};
	__maybe_unused const struct skb_recv_datagram_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__skb_recv_datagram__sk__in(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *flags_ptr);
	}

	{
		glue_pack(__pos, msg, ext, *noblock_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *err_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*err_ptr) {
			glue_pack(__pos, msg, ext, **err_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_skb_recv_datagram);

	*__pos = 0;
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__skb_recv_datagram__sk__in(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	{
	}

	{
	}

	{
		*err_ptr = glue_unpack_shadow(__pos, msg, ext, int*);
		if (*err_ptr) {
			**err_ptr = glue_unpack(__pos, msg, ext, int);
		}

	}

	{
		*ret_ptr = glue_unpack_shadow(__pos, msg, ext, struct sk_buff*);
		if (*ret_ptr) {
			caller_unmarshal_kernel__skb_recv_datagram__ret_sk_buff__io(__pos, msg, ext, ctx, *ret_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void proto_ops_sendpage_callee(struct fipc_message* msg, struct ext_registers* ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	fptr_proto_ops_sendpage function_ptr = glue_unpack(__pos, msg, ext, fptr_proto_ops_sendpage);
	struct socket* sock = 0;
	struct page* page = 0;
	int offset = 0;
	unsigned long size = 0;
	int flags = 0;
	struct socket** sock_ptr = &sock;
	struct page** page_ptr = &page;
	int* offset_ptr = &offset;
	unsigned long* size_ptr = &size;
	int* flags_ptr = &flags;
	long ret = 0;
	long* ret_ptr = &ret;
	
	__maybe_unused struct proto_ops_sendpage_call_ctx call_ctx = {sock, page, offset, size, flags};
	__maybe_unused struct proto_ops_sendpage_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sock_ptr = glue_unpack(__pos, msg, ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__proto_ops_sendpage__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		*page_ptr = glue_unpack(__pos, msg, ext, struct page*);
		if (*page_ptr) {
			callee_unmarshal_kernel__proto_ops_sendpage__page__in(__pos, msg, ext, ctx, *page_ptr);
		}

	}

	{
		*offset_ptr = glue_unpack(__pos, msg, ext, int);
	}

	{
		*size_ptr = glue_unpack(__pos, msg, ext, unsigned long);
	}

	{
		*flags_ptr = glue_unpack(__pos, msg, ext, int);
	}

	ret = function_ptr(sock, page, offset, size, flags);

	*__pos = 0;
	{
		if (*sock_ptr) {
			callee_marshal_kernel__proto_ops_sendpage__sock__in(__pos, msg, ext, ctx, *sock_ptr);
		}

	}

	{
		if (*page_ptr) {
			callee_marshal_kernel__proto_ops_sendpage__page__in(__pos, msg, ext, ctx, *page_ptr);
		}

	}

	{
	}

	{
	}

	{
	}

	{
		glue_pack(__pos, msg, ext, *ret_ptr);
	}

	msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lock_sock_nested(struct sock* sk, int subclass)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock** sk_ptr = &sk;
	
	__maybe_unused const struct lock_sock_nested_call_ctx call_ctx = {sk, subclass};
	__maybe_unused const struct lock_sock_nested_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__lock_sock_nested__sk__in(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_lock_sock_nested);

	*__pos = 0;
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__lock_sock_nested__sk__in(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __sock_recv_ts_and_drops(struct msghdr* msg, struct sock* sk, struct sk_buff* skb)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct msghdr** msg_ptr = &msg;
	struct sock** sk_ptr = &sk;
	struct sk_buff** skb_ptr = &skb;
	
	__maybe_unused const struct __sock_recv_ts_and_drops_call_ctx call_ctx = {msg, sk, skb};
	__maybe_unused const struct __sock_recv_ts_and_drops_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *msg_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*msg_ptr) {
			caller_marshal_kernel____sock_recv_ts_and_drops__msg__in(__pos, msg, ext, ctx, *msg_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel____sock_recv_ts_and_drops__sk__in(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel____sock_recv_ts_and_drops__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID___sock_recv_ts_and_drops);

	*__pos = 0;
	{
		if (*msg_ptr) {
			caller_unmarshal_kernel____sock_recv_ts_and_drops__msg__in(__pos, msg, ext, ctx, *msg_ptr);
		}

	}

	{
		if (*sk_ptr) {
			caller_unmarshal_kernel____sock_recv_ts_and_drops__sk__in(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	{
		if (*skb_ptr) {
			caller_unmarshal_kernel____sock_recv_ts_and_drops__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __sock_tx_timestamp(unsigned short tsflags, unsigned char* tx_flags)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned short* tsflags_ptr = &tsflags;
	unsigned char** tx_flags_ptr = &tx_flags;
	
	__maybe_unused const struct __sock_tx_timestamp_call_ctx call_ctx = {tsflags, tx_flags};
	__maybe_unused const struct __sock_tx_timestamp_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		glue_pack(__pos, msg, ext, *tsflags_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *tx_flags_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*tx_flags_ptr) {
			glue_pack(__pos, msg, ext, **tx_flags_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID___sock_tx_timestamp);

	*__pos = 0;
	{
	}

	{
		*tx_flags_ptr = glue_unpack_shadow(__pos, msg, ext, unsigned char*);
		if (*tx_flags_ptr) {
			**tx_flags_ptr = glue_unpack(__pos, msg, ext, unsigned char);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

unsigned char* skb_put(struct sk_buff* skb, unsigned int len)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	unsigned int* len_ptr = &len;
	unsigned char* ret = 0;
	unsigned char** ret_ptr = &ret;
	
	__maybe_unused const struct skb_put_call_ctx call_ctx = {skb, len};
	__maybe_unused const struct skb_put_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__skb_put__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
		glue_pack(__pos, msg, ext, *len_ptr);
	}

	glue_call_server(__pos, msg, RPC_ID_skb_put);

	*__pos = 0;
	{
		if (*skb_ptr) {
			caller_unmarshal_kernel__skb_put__skb__in(__pos, msg, ext, ctx, *skb_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, msg, ext, unsigned char*);
		if (*ret_ptr) {
			**ret_ptr = glue_unpack(__pos, msg, ext, unsigned char);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

void sk_free(struct sock* sk)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *msg = &__buffer;
	struct ext_registers* ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock** sk_ptr = &sk;
	
	__maybe_unused const struct sk_free_call_ctx call_ctx = {sk};
	__maybe_unused const struct sk_free_call_ctx *ctx = &call_ctx;

	(void)ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack_shadow(__pos, msg, ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__sk_free__sk__io(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	glue_call_server(__pos, msg, RPC_ID_sk_free);

	*__pos = 0;
	{
		*sk_ptr = glue_unpack_shadow(__pos, msg, ext, struct sock*);
		if (*sk_ptr) {
			caller_unmarshal_kernel__sk_free__sk__io(__pos, msg, ext, ctx, *sk_ptr);
		}

	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext)
{
	switch(id) {
	case MODULE_INIT:
		glue_user_trace("MODULE_INIT");
		__module_lcd_init();
		shared_mem_init();
		break;

	case MODULE_EXIT:
		glue_user_trace("MODULE_EXIT");
		__module_lcd_exit();
		break;

	case RPC_ID_proto_init:
		glue_user_trace("proto_init\n");
		proto_init_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_bind:
		glue_user_trace("proto_ops_bind\n");
		proto_ops_bind_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_release:
		glue_user_trace("proto_ops_release\n");
		proto_ops_release_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_getname:
		glue_user_trace("proto_ops_getname\n");
		proto_ops_getname_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_setsockopt:
		glue_user_trace("proto_ops_setsockopt\n");
		proto_ops_setsockopt_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_getsockopt:
		glue_user_trace("proto_ops_getsockopt\n");
		proto_ops_getsockopt_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_sendmsg:
		glue_user_trace("proto_ops_sendmsg\n");
		proto_ops_sendmsg_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_recvmsg:
		glue_user_trace("proto_ops_recvmsg\n");
		proto_ops_recvmsg_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_mmap:
		glue_user_trace("proto_ops_mmap\n");
		proto_ops_mmap_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_shutdown:
		glue_user_trace("proto_ops_shutdown\n");
		proto_ops_shutdown_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_listen:
		glue_user_trace("proto_ops_listen\n");
		proto_ops_listen_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_accept:
		glue_user_trace("proto_ops_accept\n");
		proto_ops_accept_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_socketpair:
		glue_user_trace("proto_ops_socketpair\n");
		proto_ops_socketpair_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_connect:
		glue_user_trace("proto_ops_connect\n");
		proto_ops_connect_callee(msg, ext);
		break;

	case RPC_ID_func:
		glue_user_trace("func\n");
		func_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_ioctl:
		glue_user_trace("proto_ops_ioctl\n");
		proto_ops_ioctl_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_poll:
		glue_user_trace("proto_ops_poll\n");
		proto_ops_poll_callee(msg, ext);
		break;

	case RPC_ID_proto_ops_sendpage:
		glue_user_trace("proto_ops_sendpage\n");
		proto_ops_sendpage_callee(msg, ext);
		break;

	default:
		return 0;
	}

	return 1;
}

