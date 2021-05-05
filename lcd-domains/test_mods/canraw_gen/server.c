#include <lcd_config/pre_hook.h>

#include "common.h"

#include <lcd_config/post_hook.h>

#define lvd_dev_put		dev_put
#define lvd_sock_put		sock_put
#define lvd_sock_orphan		sock_orphan
#define lvd_memcpy_to_msg	memcpy_to_msg
#define lvd_memcpy_from_msg	memcpy_from_msg
#define lvd_skb_reserve		skb_reserve
#define get_jiffies()		({ jiffies; })

int trmp_impl_proto_init(fptr_proto_init target, struct sock* sk)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock** sk_ptr = &sk;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_init_call_ctx call_ctx = {sk};
	__maybe_unused const struct proto_init_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *sk_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*sk_ptr) {
			caller_marshal_kernel__proto_init__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_proto_init);

	*__pos = 0;
	{
		if (*sk_ptr) {
			caller_unmarshal_kernel__proto_init__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_init)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_init) trmp_proto_init(struct sock* sk)
{
	volatile fptr_impl_proto_init impl;
	fptr_proto_init target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_init);
	impl = trmp_impl_proto_init;
	return impl(target, sk);
}

void dev_get_by_index_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net* net = 0;
	int ifindex = 0;
	int* ifindex_ptr = &ifindex;
	struct net_device* ret = 0;
	struct net_device** ret_ptr = &ret;
	
	__maybe_unused struct dev_get_by_index_call_ctx call_ctx = {net, ifindex};
	__maybe_unused struct dev_get_by_index_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ifindex_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = dev_get_by_index(&init_net, ifindex);

	*__pos = 0;
	{
	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__dev_get_by_index__ret_net_device__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int trmp_impl_proto_ops_bind(fptr_proto_ops_bind target, struct socket* sock, struct sockaddr* uaddr, int len)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket** sock_ptr = &sock;
	struct sockaddr** uaddr_ptr = &uaddr;
	int* len_ptr = &len;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_bind_call_ctx call_ctx = {sock, uaddr, len};
	__maybe_unused const struct proto_ops_bind_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *sock_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*sock_ptr) {
			caller_marshal_kernel__proto_ops_bind__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		struct sockaddr_can* __casted = (struct sockaddr_can*)*uaddr_ptr;
		struct sockaddr_can* const* __casted_ptr = &__casted;
		{
			__maybe_unused const void* __adjusted = *__casted_ptr;
			glue_pack(__pos, __msg, __ext, __adjusted);
			if (*__casted_ptr) {
				caller_marshal_kernel__proto_ops_bind__sockaddr_can__in(__pos, __msg, __ext, ctx, *__casted_ptr);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *len_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_proto_ops_bind);

	*__pos = 0;
	{
		if (*sock_ptr) {
			caller_unmarshal_kernel__proto_ops_bind__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_bind)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_bind) trmp_proto_ops_bind(struct socket* sock, struct sockaddr* uaddr, int len)
{
	volatile fptr_impl_proto_ops_bind impl;
	fptr_proto_ops_bind target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_bind);
	impl = trmp_impl_proto_ops_bind;
	return impl(target, sock, uaddr, len);
}

int trmp_impl_proto_ops_release(fptr_proto_ops_release target, struct socket* sock)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket** sock_ptr = &sock;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_release_call_ctx call_ctx = {sock};
	__maybe_unused const struct proto_ops_release_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *sock_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*sock_ptr) {
			caller_marshal_kernel__proto_ops_release__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_proto_ops_release);

	*__pos = 0;
	{
		if (*sock_ptr) {
			caller_unmarshal_kernel__proto_ops_release__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_release)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_release) trmp_proto_ops_release(struct socket* sock)
{
	volatile fptr_impl_proto_ops_release impl;
	fptr_proto_ops_release target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_release);
	impl = trmp_impl_proto_ops_release;
	return impl(target, sock);
}

int trmp_impl_proto_ops_getname(fptr_proto_ops_getname target, struct socket* sock, struct sockaddr* uaddr, int* len, int peer)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket** sock_ptr = &sock;
	struct sockaddr** uaddr_ptr = &uaddr;
	int** len_ptr = &len;
	int* peer_ptr = &peer;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_getname_call_ctx call_ctx = {sock, uaddr, len, peer};
	__maybe_unused const struct proto_ops_getname_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *sock_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*sock_ptr) {
			caller_marshal_kernel__proto_ops_getname__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		struct sockaddr_can* __casted = (struct sockaddr_can*)*uaddr_ptr;
		struct sockaddr_can* const* __casted_ptr = &__casted;
		{
			__maybe_unused const void* __adjusted = *__casted_ptr;
			glue_pack(__pos, __msg, __ext, __adjusted);
			if (*__casted_ptr) {
				caller_marshal_kernel__proto_ops_getname__sockaddr_can__in(__pos, __msg, __ext, ctx, *__casted_ptr);
			}

		}

	}

	{
		__maybe_unused const void* __adjusted = *len_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		(void)len_ptr;
	}

	{
		glue_pack(__pos, __msg, __ext, *peer_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_proto_ops_getname);

	*__pos = 0;
	{
		if (*sock_ptr) {
			caller_unmarshal_kernel__proto_ops_getname__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
	}

	{
		if (*len_ptr) {
			**len_ptr = glue_unpack(__pos, __msg, __ext, int);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_getname)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_getname) trmp_proto_ops_getname(struct socket* sock, struct sockaddr* uaddr, int* len, int peer)
{
	volatile fptr_impl_proto_ops_getname impl;
	fptr_proto_ops_getname target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_getname);
	impl = trmp_impl_proto_ops_getname;
	return impl(target, sock, uaddr, len, peer);
}

int trmp_impl_proto_ops_setsockopt(fptr_proto_ops_setsockopt target, struct socket* sock, int level, int optname, char* optval, unsigned int optlen)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket** sock_ptr = &sock;
	int* level_ptr = &level;
	int* optname_ptr = &optname;
	char** optval_ptr = &optval;
	unsigned int* optlen_ptr = &optlen;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_setsockopt_call_ctx call_ctx = {sock, level, optname, optval, optlen};
	__maybe_unused const struct proto_ops_setsockopt_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *sock_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*sock_ptr) {
			caller_marshal_kernel__proto_ops_setsockopt__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *level_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *optname_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *optval_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*optval_ptr) {
			size_t i, len = (optlen);
			char* array = *optval_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	{
		glue_pack(__pos, __msg, __ext, *optlen_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_proto_ops_setsockopt);

	*__pos = 0;
	{
		if (*sock_ptr) {
			caller_unmarshal_kernel__proto_ops_setsockopt__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
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
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_setsockopt)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_setsockopt) trmp_proto_ops_setsockopt(struct socket* sock, int level, int optname, char* optval, unsigned int optlen)
{
	volatile fptr_impl_proto_ops_setsockopt impl;
	fptr_proto_ops_setsockopt target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_setsockopt);
	impl = trmp_impl_proto_ops_setsockopt;
	return impl(target, sock, level, optname, optval, optlen);
}

int trmp_impl_proto_ops_getsockopt(fptr_proto_ops_getsockopt target, struct socket* sock, int level, int optname, char* optval, int* optlen)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket** sock_ptr = &sock;
	int* level_ptr = &level;
	int* optname_ptr = &optname;
	char** optval_ptr = &optval;
	int** optlen_ptr = &optlen;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_getsockopt_call_ctx call_ctx = {sock, level, optname, optval, optlen};
	__maybe_unused const struct proto_ops_getsockopt_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *sock_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*sock_ptr) {
			caller_marshal_kernel__proto_ops_getsockopt__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *level_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *optname_ptr);
	}

	{
		__maybe_unused const void* __adjusted = *optval_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		(void)optval_ptr;
	}

	{
		__maybe_unused const void* __adjusted = *optlen_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		(void)optlen_ptr;
	}

	glue_call_client(__pos, __msg, RPC_ID_proto_ops_getsockopt);

	*__pos = 0;
	{
		if (*sock_ptr) {
			caller_unmarshal_kernel__proto_ops_getsockopt__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
	}

	{
	}

	{
		if (*optval_ptr) {
			int i;
			char* array = *optval_ptr;
			size_t len = glue_unpack(__pos, __msg, __ext, size_t);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	{
		if (*optlen_ptr) {
			**optlen_ptr = glue_unpack(__pos, __msg, __ext, int);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_getsockopt)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_getsockopt) trmp_proto_ops_getsockopt(struct socket* sock, int level, int optname, char* optval, int* optlen)
{
	volatile fptr_impl_proto_ops_getsockopt impl;
	fptr_proto_ops_getsockopt target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_getsockopt);
	impl = trmp_impl_proto_ops_getsockopt;
	return impl(target, sock, level, optname, optval, optlen);
}

int trmp_impl_proto_ops_sendmsg(fptr_proto_ops_sendmsg target, struct socket* sock, struct msghdr* msg, unsigned long size)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket** sock_ptr = &sock;
	struct msghdr** msg_ptr = &msg;
	unsigned long* size_ptr = &size;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_sendmsg_call_ctx call_ctx = {sock, msg, size};
	__maybe_unused const struct proto_ops_sendmsg_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *sock_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*sock_ptr) {
			caller_marshal_kernel__proto_ops_sendmsg__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *msg_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*msg_ptr) {
			caller_marshal_kernel__proto_ops_sendmsg__msg__in(__pos, __msg, __ext, ctx, *msg_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *size_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_proto_ops_sendmsg);

	*__pos = 0;
	{
		if (*sock_ptr) {
			caller_unmarshal_kernel__proto_ops_sendmsg__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		if (*msg_ptr) {
			caller_unmarshal_kernel__proto_ops_sendmsg__msg__in(__pos, __msg, __ext, ctx, *msg_ptr);
		}

	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_sendmsg)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_sendmsg) trmp_proto_ops_sendmsg(struct socket* sock, struct msghdr* msg, unsigned long size)
{
	volatile fptr_impl_proto_ops_sendmsg impl;
	fptr_proto_ops_sendmsg target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_sendmsg);
	impl = trmp_impl_proto_ops_sendmsg;
	return impl(target, sock, msg, size);
}

int trmp_impl_proto_ops_recvmsg(fptr_proto_ops_recvmsg target, struct socket* sock, struct msghdr* msg, unsigned long size, int flags)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket** sock_ptr = &sock;
	struct msghdr** msg_ptr = &msg;
	unsigned long* size_ptr = &size;
	int* flags_ptr = &flags;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_recvmsg_call_ctx call_ctx = {sock, msg, size, flags};
	__maybe_unused const struct proto_ops_recvmsg_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *sock_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*sock_ptr) {
			caller_marshal_kernel__proto_ops_recvmsg__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *msg_ptr;
		msg->msg_namelen = 16;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*msg_ptr) {
			caller_marshal_kernel__proto_ops_recvmsg__msg__io(__pos, __msg, __ext, ctx, *msg_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *size_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_proto_ops_recvmsg);

	*__pos = 0;
	{
		if (*sock_ptr) {
			caller_unmarshal_kernel__proto_ops_recvmsg__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		*msg_ptr = glue_unpack(__pos, __msg, __ext, struct msghdr*);
		if (*msg_ptr) {
			caller_unmarshal_kernel__proto_ops_recvmsg__msg__io(__pos, __msg, __ext, ctx, *msg_ptr);
		}

	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_recvmsg)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_recvmsg) trmp_proto_ops_recvmsg(struct socket* sock, struct msghdr* msg, unsigned long size, int flags)
{
	volatile fptr_impl_proto_ops_recvmsg impl;
	fptr_proto_ops_recvmsg target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_recvmsg);
	impl = trmp_impl_proto_ops_recvmsg;
	return impl(target, sock, msg, size, flags);
}

int trmp_impl_proto_ops_mmap(fptr_proto_ops_mmap target, struct file* file, struct socket* sock, struct vm_area_struct* vma)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_mmap_call_ctx call_ctx = {file, sock, vma};
	__maybe_unused const struct proto_ops_mmap_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	glue_call_client(__pos, __msg, RPC_ID_proto_ops_mmap);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_mmap)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_mmap) trmp_proto_ops_mmap(struct file* file, struct socket* sock, struct vm_area_struct* vma)
{
	volatile fptr_impl_proto_ops_mmap impl;
	fptr_proto_ops_mmap target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_mmap);
	impl = trmp_impl_proto_ops_mmap;
	return impl(target, file, sock, vma);
}

int trmp_impl_proto_ops_shutdown(fptr_proto_ops_shutdown target, struct socket* sock, int how)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_shutdown_call_ctx call_ctx = {sock, how};
	__maybe_unused const struct proto_ops_shutdown_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	glue_call_client(__pos, __msg, RPC_ID_proto_ops_shutdown);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_shutdown)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_shutdown) trmp_proto_ops_shutdown(struct socket* sock, int how)
{
	volatile fptr_impl_proto_ops_shutdown impl;
	fptr_proto_ops_shutdown target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_shutdown);
	impl = trmp_impl_proto_ops_shutdown;
	return impl(target, sock, how);
}

int trmp_impl_proto_ops_listen(fptr_proto_ops_listen target, struct socket* sock, int backlog)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_listen_call_ctx call_ctx = {sock, backlog};
	__maybe_unused const struct proto_ops_listen_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	glue_call_client(__pos, __msg, RPC_ID_proto_ops_listen);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_listen)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_listen) trmp_proto_ops_listen(struct socket* sock, int backlog)
{
	volatile fptr_impl_proto_ops_listen impl;
	fptr_proto_ops_listen target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_listen);
	impl = trmp_impl_proto_ops_listen;
	return impl(target, sock, backlog);
}

int trmp_impl_proto_ops_accept(fptr_proto_ops_accept target, struct socket* sock, struct socket* newsock, int flags)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_accept_call_ctx call_ctx = {sock, newsock, flags};
	__maybe_unused const struct proto_ops_accept_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	glue_call_client(__pos, __msg, RPC_ID_proto_ops_accept);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_accept)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_accept) trmp_proto_ops_accept(struct socket* sock, struct socket* newsock, int flags)
{
	volatile fptr_impl_proto_ops_accept impl;
	fptr_proto_ops_accept target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_accept);
	impl = trmp_impl_proto_ops_accept;
	return impl(target, sock, newsock, flags);
}

int trmp_impl_proto_ops_socketpair(fptr_proto_ops_socketpair target, struct socket* sock1, struct socket* sock2)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_socketpair_call_ctx call_ctx = {sock1, sock2};
	__maybe_unused const struct proto_ops_socketpair_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	glue_call_client(__pos, __msg, RPC_ID_proto_ops_socketpair);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_socketpair)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_socketpair) trmp_proto_ops_socketpair(struct socket* sock1, struct socket* sock2)
{
	volatile fptr_impl_proto_ops_socketpair impl;
	fptr_proto_ops_socketpair target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_socketpair);
	impl = trmp_impl_proto_ops_socketpair;
	return impl(target, sock1, sock2);
}

int trmp_impl_proto_ops_connect(fptr_proto_ops_connect target, struct socket* sock, struct sockaddr* saddr, int len, int flags)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_connect_call_ctx call_ctx = {sock, saddr, len, flags};
	__maybe_unused const struct proto_ops_connect_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	glue_call_client(__pos, __msg, RPC_ID_proto_ops_connect);

	*__pos = 0;
	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_connect)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_connect) trmp_proto_ops_connect(struct socket* sock, struct sockaddr* saddr, int len, int flags)
{
	volatile fptr_impl_proto_ops_connect impl;
	fptr_proto_ops_connect target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_connect);
	impl = trmp_impl_proto_ops_connect;
	return impl(target, sock, saddr, len, flags);
}

void sock_alloc_send_skb_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock* sk = 0;
	unsigned long size = 0;
	int noblock = 0;
	struct sock** sk_ptr = &sk;
	unsigned long* size_ptr = &size;
	int* noblock_ptr = &noblock;
	int __err;
	int* err = &__err;
	int** err_ptr = &err;
	struct sk_buff* ret = 0;
	struct sk_buff** ret_ptr = &ret;
	
	__maybe_unused struct sock_alloc_send_skb_call_ctx call_ctx = {sk, size, noblock, err};
	__maybe_unused struct sock_alloc_send_skb_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__sock_alloc_send_skb__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		*size_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*noblock_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		(void)err_ptr;
	}

	ret = sock_alloc_send_skb(sk, size, noblock, err);

	printk("%s:%d skb truesize %u | skb->len %u \n", __func__, __LINE__, ret->truesize, ret->len);
	*__pos = 0;
	{
		if (*sk_ptr) {
			callee_marshal_kernel__sock_alloc_send_skb__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
	}

	{
	}

	{
		if (*err_ptr) {
			glue_pack(__pos, __msg, __ext, **err_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__sock_alloc_send_skb__ret_sk_buff__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int trmp_impl_notifier_call(fptr_notifier_call target, struct notifier_block* nb, unsigned long msg, void* ptr)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct notifier_block** nb_ptr = &nb;
	unsigned long* msg_ptr = &msg;
	void** ptr_ptr = &ptr;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct notifier_call_call_ctx call_ctx = {nb, msg, ptr};
	__maybe_unused const struct notifier_call_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *nb_ptr;
		glue_pack_shadow(__pos, __msg, __ext, __adjusted);
		if (*nb_ptr) {
			caller_marshal_kernel__notifier_call__nb__in(__pos, __msg, __ext, ctx, *nb_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *msg_ptr);
	}

	{
		struct netdev_notifier_info* __casted = (struct netdev_notifier_info*)*ptr_ptr;
		struct netdev_notifier_info* const* __casted_ptr = &__casted;
		{
			__maybe_unused const void* __adjusted = *__casted_ptr;
			glue_pack(__pos, __msg, __ext, __adjusted);
			if (*__casted_ptr) {
				caller_marshal_kernel__notifier_call__info__in(__pos, __msg, __ext, ctx, *__casted_ptr);
			}

		}

	}

	glue_call_client(__pos, __msg, RPC_ID_notifier_call);

	*__pos = 0;
	{
		if (*nb_ptr) {
			caller_unmarshal_kernel__notifier_call__nb__in(__pos, __msg, __ext, ctx, *nb_ptr);
		}

	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_notifier_call)
int LCD_TRAMPOLINE_LINKAGE(trmp_notifier_call) trmp_notifier_call(struct notifier_block* nb, unsigned long msg, void* ptr)
{
	volatile fptr_impl_notifier_call impl;
	fptr_notifier_call target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_notifier_call);
	impl = trmp_impl_notifier_call;
	return impl(target, nb, msg, ptr);
}

void register_netdevice_notifier_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct notifier_block* nb = 0;
	struct notifier_block** nb_ptr = &nb;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct register_netdevice_notifier_call_ctx call_ctx = {nb};
	__maybe_unused struct register_netdevice_notifier_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct notifier_block);
		*nb_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct notifier_block*, (__size), (DEFAULT_GFP_FLAGS));
		if (*nb_ptr) {
			callee_unmarshal_kernel__register_netdevice_notifier__nb__in(__pos, __msg, __ext, ctx, *nb_ptr);
		}

	}

	ret = register_netdevice_notifier(nb);

	*__pos = 0;
	{
		if (*nb_ptr) {
			callee_marshal_kernel__register_netdevice_notifier__nb__in(__pos, __msg, __ext, ctx, *nb_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void unregister_netdevice_notifier_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct notifier_block* nb = 0;
	struct notifier_block** nb_ptr = &nb;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct unregister_netdevice_notifier_call_ctx call_ctx = {nb};
	__maybe_unused struct unregister_netdevice_notifier_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*nb_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct notifier_block*);
		if (*nb_ptr) {
			callee_unmarshal_kernel__unregister_netdevice_notifier__nb__in(__pos, __msg, __ext, ctx, *nb_ptr);
		}

	}

	ret = unregister_netdevice_notifier(nb);

	*__pos = 0;
	{
		if (*nb_ptr) {
			callee_marshal_kernel__unregister_netdevice_notifier__nb__in(__pos, __msg, __ext, ctx, *nb_ptr);
		}

		glue_remove_shadow(*nb_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void kfree_skb_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* skb = 0;
	struct sk_buff** skb_ptr = &skb;
	
	__maybe_unused struct kfree_skb_call_ctx call_ctx = {skb};
	__maybe_unused struct kfree_skb_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__kfree_skb__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	kfree_skb(skb);

	*__pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel__kfree_skb__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void skb_clone_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* skb = 0;
	unsigned int gfp_mask = 0;
	struct sk_buff** skb_ptr = &skb;
	unsigned int* gfp_mask_ptr = &gfp_mask;
	struct sk_buff* ret = 0;
	struct sk_buff** ret_ptr = &ret;
	
	__maybe_unused struct skb_clone_call_ctx call_ctx = {skb, gfp_mask};
	__maybe_unused struct skb_clone_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__skb_clone__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		*gfp_mask_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	ret = skb_clone(skb, gfp_mask);

	*__pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel__skb_clone__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__skb_clone__ret_sk_buff__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __alloc_percpu_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long size = 0;
	unsigned long align = 0;
	unsigned long* size_ptr = &size;
	unsigned long* align_ptr = &align;
	void* ret = 0;
	void** ret_ptr = &ret;
	
	__maybe_unused struct __alloc_percpu_call_ctx call_ctx = {size, align};
	__maybe_unused struct __alloc_percpu_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*size_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*align_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	ret = __alloc_percpu(size, align);

	*__pos = 0;
	{
	}

	{
	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ret_ptr) {
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void free_percpu_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	void* ptr = 0;
	void** ptr_ptr = &ptr;
	
	__maybe_unused struct free_percpu_call_ctx call_ctx = {ptr};
	__maybe_unused struct free_percpu_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*ptr_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*ptr_ptr) {
		}

	}

	free_percpu(ptr);

	*__pos = 0;
	{
		(void)ptr_ptr;
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void sock_queue_rcv_skb_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock* sk = 0;
	struct sk_buff* skb = 0;
	struct sock** sk_ptr = &sk;
	struct sk_buff** skb_ptr = &skb;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct sock_queue_rcv_skb_call_ctx call_ctx = {sk, skb};
	__maybe_unused struct sock_queue_rcv_skb_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__sock_queue_rcv_skb__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		*skb_ptr = glue_unpack(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__sock_queue_rcv_skb__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	ret = sock_queue_rcv_skb(sk, skb);

	*__pos = 0;
	{
		if (*sk_ptr) {
			callee_marshal_kernel__sock_queue_rcv_skb__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		if (*skb_ptr) {
			callee_marshal_kernel__sock_queue_rcv_skb__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void can_proto_unregister_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct can_proto const* cp = 0;
	struct can_proto const** cp_ptr = &cp;
	
	__maybe_unused struct can_proto_unregister_call_ctx call_ctx = {cp};
	__maybe_unused struct can_proto_unregister_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*cp_ptr = glue_unpack_shadow(__pos, __msg, __ext, struct can_proto const*);
		if (*cp_ptr) {
			struct can_proto* writable = (struct can_proto*)*cp_ptr;
			callee_unmarshal_kernel__can_proto_unregister__cp__in(__pos, __msg, __ext, ctx, writable);
		}

	}

	can_proto_unregister(cp);

	*__pos = 0;
	{
		if (*cp_ptr) {
			callee_marshal_kernel__can_proto_unregister__cp__in(__pos, __msg, __ext, ctx, *cp_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void can_proto_register_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct can_proto const* cp = 0;
	struct can_proto const** cp_ptr = &cp;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct can_proto_register_call_ctx call_ctx = {cp};
	__maybe_unused struct can_proto_register_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct can_proto);
		*cp_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, struct can_proto const*, (__size), (DEFAULT_GFP_FLAGS));
		if (*cp_ptr) {
			struct can_proto* writable = (struct can_proto*)*cp_ptr;
			callee_unmarshal_kernel__can_proto_register__cp__in(__pos, __msg, __ext, ctx, writable);
		}

	}

	ret = can_proto_register(cp);

	*__pos = 0;
	{
		if (*cp_ptr) {
			callee_marshal_kernel__can_proto_register__cp__in(__pos, __msg, __ext, ctx, *cp_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void trmp_impl_func(fptr_func target, struct sk_buff* skb, void* data)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff** skb_ptr = &skb;
	void** data_ptr = &data;
	
	__maybe_unused const struct func_call_ctx call_ctx = {skb, data};
	__maybe_unused const struct func_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *skb_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*skb_ptr) {
			caller_marshal_kernel__func__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		struct sock* __casted = (struct sock*)*data_ptr;
		struct sock* const* __casted_ptr = &__casted;
		{
			__maybe_unused const void* __adjusted = *__casted_ptr;
			glue_pack(__pos, __msg, __ext, __adjusted);
			if (*__casted_ptr) {
				caller_marshal_kernel__func__sock__in(__pos, __msg, __ext, ctx, *__casted_ptr);
			}

		}

	}

	glue_call_client(__pos, __msg, RPC_ID_func);

	*__pos = 0;
	{
		if (*skb_ptr) {
			caller_unmarshal_kernel__func__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

LCD_TRAMPOLINE_DATA(trmp_func)
void LCD_TRAMPOLINE_LINKAGE(trmp_func) trmp_func(struct sk_buff* skb, void* data)
{
	volatile fptr_impl_func impl;
	fptr_func target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_func);
	impl = trmp_impl_func;
	return impl(target, skb, data);
}

void can_rx_unregister_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	unsigned int can_id = 0;
	unsigned int mask = 0;
	fptr_func func = 0;
	void* data = 0;
	struct net_device** dev_ptr = &dev;
	unsigned int* can_id_ptr = &can_id;
	unsigned int* mask_ptr = &mask;
	fptr_func* func_ptr = &func;
	void** data_ptr = &data;
	
	__maybe_unused struct can_rx_unregister_call_ctx call_ctx = {dev, can_id, mask, func, data};
	__maybe_unused struct can_rx_unregister_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__can_rx_unregister__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*can_id_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mask_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*func_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, func);
	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*data_ptr) {
		}

	}

	can_rx_unregister(dev, can_id, mask, func, data);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__can_rx_unregister__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void can_rx_register_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	unsigned int can_id = 0;
	unsigned int mask = 0;
	fptr_func func = 0;
	void* data = 0;
	char* ident = 0;
	struct net_device** dev_ptr = &dev;
	unsigned int* can_id_ptr = &can_id;
	unsigned int* mask_ptr = &mask;
	fptr_func* func_ptr = &func;
	void** data_ptr = &data;
	char** ident_ptr = &ident;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct can_rx_register_call_ctx call_ctx = {dev, can_id, mask, func, data, ident};
	__maybe_unused struct can_rx_register_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__can_rx_register__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	{
		*can_id_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*mask_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*func_ptr = glue_unpack_rpc_ptr(__pos, __msg, __ext, func);
	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*data_ptr) {
		}

	}

	{
		size_t __size = sizeof(char) * glue_peek(__pos, __msg, __ext);
		*ident_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, char*, (__size), (DEFAULT_GFP_FLAGS));
		if (*ident_ptr) {
			size_t i, len;
			char* array = *ident_ptr;
			len = glue_unpack(__pos, __msg, __ext, size_t) - 1;
			array[len] = '\0';
			for (i = 0; i < len; ++i) {
				char* element = &array[i];
				*element = glue_unpack(__pos, __msg, __ext, char);
			}

		}

	}

	ret = can_rx_register(dev, can_id, mask, func, data, ident);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__can_rx_register__dev__in(__pos, __msg, __ext, ctx, *dev_ptr);
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
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void can_send_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* skb = 0;
	int loop = 0;
	struct sk_buff** skb_ptr = &skb;
	int* loop_ptr = &loop;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct can_send_call_ctx call_ctx = {skb, loop};
	__maybe_unused struct can_send_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__can_send__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		*loop_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = can_send(skb, loop);

	*__pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel__can_send__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void can_ioctl_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket* sock = 0;
	unsigned int cmd = 0;
	unsigned long arg = 0;
	struct socket** sock_ptr = &sock;
	unsigned int* cmd_ptr = &cmd;
	unsigned long* arg_ptr = &arg;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct can_ioctl_call_ctx call_ctx = {sock, cmd, arg};
	__maybe_unused struct can_ioctl_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		size_t __size = sizeof(struct socket);
		*sock_ptr = glue_unpack_bind_or_new_shadow(__pos, __msg, __ext, struct socket*, __size);
		if (*sock_ptr) {
			callee_unmarshal_kernel__can_ioctl__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		*cmd_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*arg_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	ret = can_ioctl(sock, cmd, arg);

	*__pos = 0;
	{
		if (*sock_ptr) {
			callee_marshal_kernel__can_ioctl__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void datagram_poll_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct file* file = 0;
	struct socket* sock = 0;
	struct poll_table_struct* wait = 0;
	struct file** file_ptr = &file;
	struct socket** sock_ptr = &sock;
	struct poll_table_struct** wait_ptr = &wait;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused struct datagram_poll_call_ctx call_ctx = {file, sock, wait};
	__maybe_unused struct datagram_poll_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*file_ptr = glue_unpack(__pos, __msg, __ext, struct file*);
		if (*file_ptr) {
			callee_unmarshal_kernel__datagram_poll__file__in(__pos, __msg, __ext, ctx, *file_ptr);
		}

	}

	{
		*sock_ptr = glue_unpack(__pos, __msg, __ext, struct socket*);
		if (*sock_ptr) {
			callee_unmarshal_kernel__datagram_poll__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		*wait_ptr = glue_unpack(__pos, __msg, __ext, struct poll_table_struct*);
		if (*wait_ptr) {
			callee_unmarshal_kernel__datagram_poll__wait__in(__pos, __msg, __ext, ctx, *wait_ptr);
		}

	}

	ret = datagram_poll(file, sock, wait);

	*__pos = 0;
	{
		if (*file_ptr) {
			callee_marshal_kernel__datagram_poll__file__in(__pos, __msg, __ext, ctx, *file_ptr);
		}

	}

	{
		if (*sock_ptr) {
			callee_marshal_kernel__datagram_poll__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		if (*wait_ptr) {
			callee_marshal_kernel__datagram_poll__wait__in(__pos, __msg, __ext, ctx, *wait_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int trmp_impl_proto_ops_ioctl(fptr_proto_ops_ioctl target, struct socket* sock, unsigned int cmd, unsigned long arg)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket** sock_ptr = &sock;
	unsigned int* cmd_ptr = &cmd;
	unsigned long* arg_ptr = &arg;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_ioctl_call_ctx call_ctx = {sock, cmd, arg};
	__maybe_unused const struct proto_ops_ioctl_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *sock_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*sock_ptr) {
			caller_marshal_kernel__proto_ops_ioctl__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *cmd_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *arg_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_proto_ops_ioctl);

	*__pos = 0;
	{
		if (*sock_ptr) {
			caller_unmarshal_kernel__proto_ops_ioctl__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_ioctl)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_ioctl) trmp_proto_ops_ioctl(struct socket* sock, unsigned int cmd, unsigned long arg)
{
	volatile fptr_impl_proto_ops_ioctl impl;
	fptr_proto_ops_ioctl target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_ioctl);
	impl = trmp_impl_proto_ops_ioctl;
	return impl(target, sock, cmd, arg);
}

unsigned int trmp_impl_proto_ops_poll(fptr_proto_ops_poll target, struct file* file, struct socket* sock, struct poll_table_struct* wait)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct file** file_ptr = &file;
	struct socket** sock_ptr = &sock;
	struct poll_table_struct** wait_ptr = &wait;
	unsigned int ret = 0;
	unsigned int* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_poll_call_ctx call_ctx = {file, sock, wait};
	__maybe_unused const struct proto_ops_poll_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *file_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*file_ptr) {
			caller_marshal_kernel__proto_ops_poll__file__in(__pos, __msg, __ext, ctx, *file_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *sock_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*sock_ptr) {
			caller_marshal_kernel__proto_ops_poll__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *wait_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*wait_ptr) {
			caller_marshal_kernel__proto_ops_poll__wait__in(__pos, __msg, __ext, ctx, *wait_ptr);
		}

	}

	glue_call_client(__pos, __msg, RPC_ID_proto_ops_poll);

	*__pos = 0;
	{
		if (*file_ptr) {
			caller_unmarshal_kernel__proto_ops_poll__file__in(__pos, __msg, __ext, ctx, *file_ptr);
		}

	}

	{
		if (*sock_ptr) {
			caller_unmarshal_kernel__proto_ops_poll__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		if (*wait_ptr) {
			caller_unmarshal_kernel__proto_ops_poll__wait__in(__pos, __msg, __ext, ctx, *wait_ptr);
		}

	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_poll)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_poll) trmp_proto_ops_poll(struct file* file, struct socket* sock, struct poll_table_struct* wait)
{
	volatile fptr_impl_proto_ops_poll impl;
	fptr_proto_ops_poll target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_poll);
	impl = trmp_impl_proto_ops_poll;
	return impl(target, file, sock, wait);
}

void release_sock_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock* sk = 0;
	struct sock** sk_ptr = &sk;
	
	__maybe_unused struct release_sock_call_ctx call_ctx = {sk};
	__maybe_unused struct release_sock_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__release_sock__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	release_sock(sk);

	*__pos = 0;
	{
		if (*sk_ptr) {
			callee_marshal_kernel__release_sock__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void skb_free_datagram_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock* sk = 0;
	struct sk_buff* skb = 0;
	struct sock** sk_ptr = &sk;
	struct sk_buff** skb_ptr = &skb;
	
	__maybe_unused struct skb_free_datagram_call_ctx call_ctx = {sk, skb};
	__maybe_unused struct skb_free_datagram_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__skb_free_datagram__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		*skb_ptr = glue_unpack(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__skb_free_datagram__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	skb_free_datagram(sk, skb);

	*__pos = 0;
	{
		if (*sk_ptr) {
			callee_marshal_kernel__skb_free_datagram__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		if (*skb_ptr) {
			callee_marshal_kernel__skb_free_datagram__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void skb_recv_datagram_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock* sk = 0;
	unsigned int flags = 0;
	int noblock = 0;
	struct sock** sk_ptr = &sk;
	unsigned int* flags_ptr = &flags;
	int* noblock_ptr = &noblock;
	int __err;
	int* err = &__err;
	int** err_ptr = &err;
	struct sk_buff* ret = 0;
	struct sk_buff** ret_ptr = &ret;
	
	__maybe_unused struct skb_recv_datagram_call_ctx call_ctx = {sk, flags, noblock, err};
	__maybe_unused struct skb_recv_datagram_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__skb_recv_datagram__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		*flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	{
		*noblock_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		(void)err_ptr;
	}

	ret = skb_recv_datagram(sk, flags, noblock, err);

	*__pos = 0;
	{
		if (*sk_ptr) {
			callee_marshal_kernel__skb_recv_datagram__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
	}

	{
	}

	{
		if (*err_ptr) {
			glue_pack(__pos, __msg, __ext, **err_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ret_ptr) {
			callee_marshal_kernel__skb_recv_datagram__ret_sk_buff__out(__pos, __msg, __ext, ctx, *ret_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

long trmp_impl_proto_ops_sendpage(fptr_proto_ops_sendpage target, struct socket* sock, struct page* page, int offset, unsigned long size, int flags)
{
	struct fipc_message __buffer = {0};
	struct fipc_message *__msg = &__buffer;
	struct ext_registers* __ext = get_register_page(smp_processor_id());
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket** sock_ptr = &sock;
	struct page** page_ptr = &page;
	int* offset_ptr = &offset;
	unsigned long* size_ptr = &size;
	int* flags_ptr = &flags;
	long ret = 0;
	long* ret_ptr = &ret;
	
	__maybe_unused const struct proto_ops_sendpage_call_ctx call_ctx = {sock, page, offset, size, flags};
	__maybe_unused const struct proto_ops_sendpage_call_ctx *ctx = &call_ctx;

	(void)__ext;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	glue_pack(__pos, __msg, __ext, target);
	{
		__maybe_unused const void* __adjusted = *sock_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*sock_ptr) {
			caller_marshal_kernel__proto_ops_sendpage__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		__maybe_unused const void* __adjusted = *page_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*page_ptr) {
			caller_marshal_kernel__proto_ops_sendpage__page__in(__pos, __msg, __ext, ctx, *page_ptr);
		}

	}

	{
		glue_pack(__pos, __msg, __ext, *offset_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *size_ptr);
	}

	{
		glue_pack(__pos, __msg, __ext, *flags_ptr);
	}

	glue_call_client(__pos, __msg, RPC_ID_proto_ops_sendpage);

	*__pos = 0;
	{
		if (*sock_ptr) {
			caller_unmarshal_kernel__proto_ops_sendpage__sock__in(__pos, __msg, __ext, ctx, *sock_ptr);
		}

	}

	{
		if (*page_ptr) {
			caller_unmarshal_kernel__proto_ops_sendpage__page__in(__pos, __msg, __ext, ctx, *page_ptr);
		}

	}

	{
	}

	{
	}

	{
	}

	{
		*ret_ptr = glue_unpack(__pos, __msg, __ext, long);
	}

	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
	return ret;
}

LCD_TRAMPOLINE_DATA(trmp_proto_ops_sendpage)
long LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_sendpage) trmp_proto_ops_sendpage(struct socket* sock, struct page* page, int offset, unsigned long size, int flags)
{
	volatile fptr_impl_proto_ops_sendpage impl;
	fptr_proto_ops_sendpage target;
	LCD_TRAMPOLINE_PROLOGUE(target, trmp_proto_ops_sendpage);
	impl = trmp_impl_proto_ops_sendpage;
	return impl(target, sock, page, offset, size, flags);
}

void lock_sock_nested_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock* sk = 0;
	int subclass = 0;
	struct sock** sk_ptr = &sk;
	
	__maybe_unused struct lock_sock_nested_call_ctx call_ctx = {sk, subclass};
	__maybe_unused struct lock_sock_nested_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__lock_sock_nested__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	lock_sock_nested(sk, subclass);

	*__pos = 0;
	{
		if (*sk_ptr) {
			callee_marshal_kernel__lock_sock_nested__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __sock_recv_ts_and_drops_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct msghdr* msg = 0;
	struct sock* sk = 0;
	struct sk_buff* skb = 0;
	struct msghdr** msg_ptr = &msg;
	struct sock** sk_ptr = &sk;
	struct sk_buff** skb_ptr = &skb;
	
	__maybe_unused struct __sock_recv_ts_and_drops_call_ctx call_ctx = {msg, sk, skb};
	__maybe_unused struct __sock_recv_ts_and_drops_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*msg_ptr = glue_unpack(__pos, __msg, __ext, struct msghdr*);
		if (*msg_ptr) {
			callee_unmarshal_kernel____sock_recv_ts_and_drops__msg__in(__pos, __msg, __ext, ctx, *msg_ptr);
		}

	}

	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel____sock_recv_ts_and_drops__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		*skb_ptr = glue_unpack(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel____sock_recv_ts_and_drops__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	__sock_recv_ts_and_drops(msg, sk, skb);

	*__pos = 0;
	{
		if (*msg_ptr) {
			callee_marshal_kernel____sock_recv_ts_and_drops__msg__in(__pos, __msg, __ext, ctx, *msg_ptr);
		}

	}

	{
		if (*sk_ptr) {
			callee_marshal_kernel____sock_recv_ts_and_drops__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	{
		if (*skb_ptr) {
			callee_marshal_kernel____sock_recv_ts_and_drops__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void __sock_tx_timestamp_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned short tsflags = 0;
	unsigned char* tx_flags = 0;
	unsigned short* tsflags_ptr = &tsflags;
	unsigned char** tx_flags_ptr = &tx_flags;
	
	__maybe_unused struct __sock_tx_timestamp_call_ctx call_ctx = {tsflags, tx_flags};
	__maybe_unused struct __sock_tx_timestamp_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*tsflags_ptr = glue_unpack(__pos, __msg, __ext, unsigned short);
	}

	{
		size_t __size = sizeof(unsigned char);
		*tx_flags_ptr = glue_unpack_new_shadow(__pos, __msg, __ext, unsigned char*, (__size), (DEFAULT_GFP_FLAGS));
		if (*tx_flags_ptr) {
			**tx_flags_ptr = glue_unpack(__pos, __msg, __ext, unsigned char);
		}

	}

	__sock_tx_timestamp(tsflags, tx_flags);

	*__pos = 0;
	{
	}

	{
		if (*tx_flags_ptr) {
			glue_pack(__pos, __msg, __ext, **tx_flags_ptr);
		}

		glue_remove_shadow(*tx_flags_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void skb_put_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* skb = 0;
	unsigned int len = 0;
	struct sk_buff** skb_ptr = &skb;
	unsigned int* len_ptr = &len;
	unsigned char* ret = 0;
	unsigned char** ret_ptr = &ret;
	
	__maybe_unused struct skb_put_call_ctx call_ctx = {skb, len};
	__maybe_unused struct skb_put_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__skb_put__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, unsigned int);
	}

	ret = skb_put(skb, len);

	*__pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel__skb_put__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
	}

	{
		__maybe_unused const void* __adjusted = *ret_ptr;
		glue_pack(__pos, __msg, __ext, __adjusted);
		if (*ret_ptr) {
			size_t i, len = (len);
			unsigned char* array = *ret_ptr;
			glue_pack(__pos, __msg, __ext, len);
			// Warning: see David if this breaks
			glue_user_trace("Warning: see David if this breaks");
			for (i = 0; i < len; ++i) {
				unsigned char* element = &array[i];
				glue_pack(__pos, __msg, __ext, *element);
			}

		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_sock_put_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock* sk = 0;
	struct sock** sk_ptr = &sk;
	
	__maybe_unused struct lvd_sock_put_call_ctx call_ctx = {sk};
	__maybe_unused struct lvd_sock_put_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__lvd_sock_put__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	lvd_sock_put(sk);

	*__pos = 0;
	{
		if (*sk_ptr) {
			callee_marshal_kernel__lvd_sock_put__sk__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void sock_no_mmap_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct file* file = 0;
	struct socket* sock = 0;
	struct vm_area_struct* vma = 0;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct sock_no_mmap_call_ctx call_ctx = {file, sock, vma};
	__maybe_unused struct sock_no_mmap_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = sock_no_mmap(file, sock, vma);

	*__pos = 0;
	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void sock_no_shutdown_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket* sock = 0;
	int how = 0;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct sock_no_shutdown_call_ctx call_ctx = {sock, how};
	__maybe_unused struct sock_no_shutdown_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = sock_no_shutdown(sock, how);

	*__pos = 0;
	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void sock_no_listen_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket* sock = 0;
	int backlog = 0;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct sock_no_listen_call_ctx call_ctx = {sock, backlog};
	__maybe_unused struct sock_no_listen_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = sock_no_listen(sock, backlog);

	*__pos = 0;
	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void sock_no_accept_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket* sock = 0;
	struct socket* newsock = 0;
	int flags = 0;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct sock_no_accept_call_ctx call_ctx = {sock, newsock, flags};
	__maybe_unused struct sock_no_accept_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = sock_no_accept(sock, newsock, flags);

	*__pos = 0;
	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void sock_no_socketpair_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket* sock1 = 0;
	struct socket* sock2 = 0;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct sock_no_socketpair_call_ctx call_ctx = {sock1, sock2};
	__maybe_unused struct sock_no_socketpair_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = sock_no_socketpair(sock1, sock2);

	*__pos = 0;
	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void sock_no_connect_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket* sock = 0;
	struct sockaddr* saddr = 0;
	int len = 0;
	int flags = 0;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct sock_no_connect_call_ctx call_ctx = {sock, saddr, len, flags};
	__maybe_unused struct sock_no_connect_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = sock_no_connect(sock, saddr, len, flags);

	*__pos = 0;
	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void sock_no_sendpage_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct socket* sock = 0;
	struct page* page = 0;
	int offset = 0;
	unsigned long size = 0;
	int flags = 0;
	int* offset_ptr = &offset;
	unsigned long* size_ptr = &size;
	int* flags_ptr = &flags;
	long ret = 0;
	long* ret_ptr = &ret;
	
	__maybe_unused struct sock_no_sendpage_call_ctx call_ctx = {sock, page, offset, size, flags};
	__maybe_unused struct sock_no_sendpage_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*offset_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	{
		*size_ptr = glue_unpack(__pos, __msg, __ext, unsigned long);
	}

	{
		*flags_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = sock_no_sendpage(sock, page, offset, size, flags);

	*__pos = 0;
	{
	}

	{
	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_memcpy_to_msg_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct msghdr* msg = 0;
	void* data = 0;
	int len = 0;
	struct msghdr** msg_ptr = &msg;
	void** data_ptr = &data;
	int* len_ptr = &len;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct lvd_memcpy_to_msg_call_ctx call_ctx = {msg, data, len};
	__maybe_unused struct lvd_memcpy_to_msg_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*msg_ptr = glue_unpack(__pos, __msg, __ext, struct msghdr*);
		if (*msg_ptr) {
			callee_unmarshal_kernel__lvd_memcpy_to_msg__msg__in(__pos, __msg, __ext, ctx, *msg_ptr);
		}

	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*data_ptr) {
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	printk("%s, msg %p | data %p | len %d\n", __func__, msg, data, len);

	ret = lvd_memcpy_to_msg(msg, data, len);

	*__pos = 0;
	{
		if (*msg_ptr) {
			callee_marshal_kernel__lvd_memcpy_to_msg__msg__in(__pos, __msg, __ext, ctx, *msg_ptr);
		}

	}

	{
		(void)data_ptr;
	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_memcpy_from_msg_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	void* data = 0;
	struct msghdr* msg = 0;
	int len = 0;
	void** data_ptr = &data;
	struct msghdr** msg_ptr = &msg;
	int* len_ptr = &len;
	int ret = 0;
	int* ret_ptr = &ret;
	
	__maybe_unused struct lvd_memcpy_from_msg_call_ctx call_ctx = {data, msg, len};
	__maybe_unused struct lvd_memcpy_from_msg_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*data_ptr = glue_unpack(__pos, __msg, __ext, void*);
		if (*data_ptr) {
		}

	}

	{
		*msg_ptr = glue_unpack(__pos, __msg, __ext, struct msghdr*);
		if (*msg_ptr) {
			callee_unmarshal_kernel__lvd_memcpy_from_msg__msg__in(__pos, __msg, __ext, ctx, *msg_ptr);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	ret = lvd_memcpy_from_msg(data, msg, len);

	*__pos = 0;
	{
		(void)data_ptr;
	}

	{
		if (*msg_ptr) {
			callee_marshal_kernel__lvd_memcpy_from_msg__msg__in(__pos, __msg, __ext, ctx, *msg_ptr);
		}

	}

	{
	}

	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_dev_put_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct net_device* dev = 0;
	struct net_device** dev_ptr = &dev;
	
	__maybe_unused struct lvd_dev_put_call_ctx call_ctx = {dev};
	__maybe_unused struct lvd_dev_put_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*dev_ptr = glue_unpack(__pos, __msg, __ext, struct net_device*);
		if (*dev_ptr) {
			callee_unmarshal_kernel__lvd_dev_put__net_device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	lvd_dev_put(dev);

	*__pos = 0;
	{
		if (*dev_ptr) {
			callee_marshal_kernel__lvd_dev_put__net_device__in(__pos, __msg, __ext, ctx, *dev_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_sock_orphan_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sock* sk = 0;
	struct sock** sk_ptr = &sk;
	
	__maybe_unused struct lvd_sock_orphan_call_ctx call_ctx = {sk};
	__maybe_unused struct lvd_sock_orphan_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*sk_ptr = glue_unpack(__pos, __msg, __ext, struct sock*);
		if (*sk_ptr) {
			callee_unmarshal_kernel__lvd_sock_orphan__sock__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	lvd_sock_orphan(sk);

	*__pos = 0;
	{
		if (*sk_ptr) {
			callee_marshal_kernel__lvd_sock_orphan__sock__in(__pos, __msg, __ext, ctx, *sk_ptr);
		}

	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void get_jiffies_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	unsigned long ret = 0;
	unsigned long* ret_ptr = &ret;
	
	__maybe_unused struct get_jiffies_call_ctx call_ctx = {};
	__maybe_unused struct get_jiffies_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	ret = get_jiffies();

	*__pos = 0;
	{
		glue_pack(__pos, __msg, __ext, *ret_ptr);
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

void lvd_skb_reserve_callee(struct fipc_message* __msg, struct ext_registers* __ext)
{
	size_t n_pos = 0;
	size_t* __pos = &n_pos;

	struct sk_buff* skb = 0;
	int len = 0;
	struct sk_buff** skb_ptr = &skb;
	int* len_ptr = &len;
	
	__maybe_unused struct lvd_skb_reserve_call_ctx call_ctx = {skb, len};
	__maybe_unused struct lvd_skb_reserve_call_ctx *ctx = &call_ctx;

	if (verbose_debug) {
		printk("%s:%d, entered!\n", __func__, __LINE__);
	}

	{
		*skb_ptr = glue_unpack(__pos, __msg, __ext, struct sk_buff*);
		if (*skb_ptr) {
			callee_unmarshal_kernel__lvd_skb_reserve__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
		*len_ptr = glue_unpack(__pos, __msg, __ext, int);
	}

	lvd_skb_reserve(skb, len);

	*__pos = 0;
	{
		if (*skb_ptr) {
			callee_marshal_kernel__lvd_skb_reserve__skb__in(__pos, __msg, __ext, ctx, *skb_ptr);
		}

	}

	{
	}

	__msg->regs[0] = *__pos;
	if (verbose_debug) {
		printk("%s:%d, returned!\n", __func__, __LINE__);
	}
}

int try_dispatch(enum RPC_ID id, struct fipc_message* __msg, struct ext_registers* __ext)
{
	switch(id) {
	case RPC_ID_shared_mem_init:
		glue_user_trace("shared_mem_init\n");
		shared_mem_init_callee(__msg, __ext);
		break;

	case RPC_ID_dev_get_by_index:
		glue_user_trace("dev_get_by_index\n");
		dev_get_by_index_callee(__msg, __ext);
		break;

	case RPC_ID_sock_alloc_send_skb:
		glue_user_trace("sock_alloc_send_skb\n");
		sock_alloc_send_skb_callee(__msg, __ext);
		break;

	case RPC_ID_register_netdevice_notifier:
		glue_user_trace("register_netdevice_notifier\n");
		register_netdevice_notifier_callee(__msg, __ext);
		break;

	case RPC_ID_unregister_netdevice_notifier:
		glue_user_trace("unregister_netdevice_notifier\n");
		unregister_netdevice_notifier_callee(__msg, __ext);
		break;

	case RPC_ID_kfree_skb:
		glue_user_trace("kfree_skb\n");
		kfree_skb_callee(__msg, __ext);
		break;

	case RPC_ID_skb_clone:
		glue_user_trace("skb_clone\n");
		skb_clone_callee(__msg, __ext);
		break;

	case RPC_ID___alloc_percpu:
		glue_user_trace("__alloc_percpu\n");
		__alloc_percpu_callee(__msg, __ext);
		break;

	case RPC_ID_free_percpu:
		glue_user_trace("free_percpu\n");
		free_percpu_callee(__msg, __ext);
		break;

	case RPC_ID_sock_queue_rcv_skb:
		glue_user_trace("sock_queue_rcv_skb\n");
		sock_queue_rcv_skb_callee(__msg, __ext);
		break;

	case RPC_ID_can_proto_unregister:
		glue_user_trace("can_proto_unregister\n");
		can_proto_unregister_callee(__msg, __ext);
		break;

	case RPC_ID_can_proto_register:
		glue_user_trace("can_proto_register\n");
		can_proto_register_callee(__msg, __ext);
		break;

	case RPC_ID_can_rx_unregister:
		glue_user_trace("can_rx_unregister\n");
		can_rx_unregister_callee(__msg, __ext);
		break;

	case RPC_ID_can_rx_register:
		glue_user_trace("can_rx_register\n");
		can_rx_register_callee(__msg, __ext);
		break;

	case RPC_ID_can_send:
		glue_user_trace("can_send\n");
		can_send_callee(__msg, __ext);
		break;

	case RPC_ID_can_ioctl:
		glue_user_trace("can_ioctl\n");
		can_ioctl_callee(__msg, __ext);
		break;

	case RPC_ID_datagram_poll:
		glue_user_trace("datagram_poll\n");
		datagram_poll_callee(__msg, __ext);
		break;

	case RPC_ID_release_sock:
		glue_user_trace("release_sock\n");
		release_sock_callee(__msg, __ext);
		break;

	case RPC_ID_skb_free_datagram:
		glue_user_trace("skb_free_datagram\n");
		skb_free_datagram_callee(__msg, __ext);
		break;

	case RPC_ID_skb_recv_datagram:
		glue_user_trace("skb_recv_datagram\n");
		skb_recv_datagram_callee(__msg, __ext);
		break;

	case RPC_ID_lock_sock_nested:
		glue_user_trace("lock_sock_nested\n");
		lock_sock_nested_callee(__msg, __ext);
		break;

	case RPC_ID___sock_recv_ts_and_drops:
		glue_user_trace("__sock_recv_ts_and_drops\n");
		__sock_recv_ts_and_drops_callee(__msg, __ext);
		break;

	case RPC_ID___sock_tx_timestamp:
		glue_user_trace("__sock_tx_timestamp\n");
		__sock_tx_timestamp_callee(__msg, __ext);
		break;

	case RPC_ID_skb_put:
		glue_user_trace("skb_put\n");
		skb_put_callee(__msg, __ext);
		break;

	case RPC_ID_lvd_sock_put:
		glue_user_trace("lvd_sock_put\n");
		lvd_sock_put_callee(__msg, __ext);
		break;

	case RPC_ID_sock_no_mmap:
		glue_user_trace("sock_no_mmap\n");
		sock_no_mmap_callee(__msg, __ext);
		break;

	case RPC_ID_sock_no_shutdown:
		glue_user_trace("sock_no_shutdown\n");
		sock_no_shutdown_callee(__msg, __ext);
		break;

	case RPC_ID_sock_no_listen:
		glue_user_trace("sock_no_listen\n");
		sock_no_listen_callee(__msg, __ext);
		break;

	case RPC_ID_sock_no_accept:
		glue_user_trace("sock_no_accept\n");
		sock_no_accept_callee(__msg, __ext);
		break;

	case RPC_ID_sock_no_socketpair:
		glue_user_trace("sock_no_socketpair\n");
		sock_no_socketpair_callee(__msg, __ext);
		break;

	case RPC_ID_sock_no_connect:
		glue_user_trace("sock_no_connect\n");
		sock_no_connect_callee(__msg, __ext);
		break;

	case RPC_ID_sock_no_sendpage:
		glue_user_trace("sock_no_sendpage\n");
		sock_no_sendpage_callee(__msg, __ext);
		break;

	case RPC_ID_lvd_memcpy_to_msg:
		glue_user_trace("lvd_memcpy_to_msg\n");
		lvd_memcpy_to_msg_callee(__msg, __ext);
		break;

	case RPC_ID_lvd_memcpy_from_msg:
		glue_user_trace("lvd_memcpy_from_msg\n");
		lvd_memcpy_from_msg_callee(__msg, __ext);
		break;

	case RPC_ID_lvd_dev_put:
		glue_user_trace("lvd_dev_put\n");
		lvd_dev_put_callee(__msg, __ext);
		break;

	case RPC_ID_lvd_sock_orphan:
		glue_user_trace("lvd_sock_orphan\n");
		lvd_sock_orphan_callee(__msg, __ext);
		break;

	case RPC_ID_get_jiffies:
		glue_user_trace("get_jiffies\n");
		get_jiffies_callee(__msg, __ext);
		break;

	case RPC_ID_lvd_skb_reserve:
		glue_user_trace("lvd_skb_reserve\n");
		lvd_skb_reserve_callee(__msg, __ext);
		break;

	default:
		return 0;
	}

	return 1;
}

