#ifndef COMMON_H
#define COMMON_H

#include <liblcd/trampoline.h>
#include <libfipc.h>
#include <liblcd/boot_info.h>
#include <asm/cacheflush.h>
#include <lcd_domains/microkernel.h>
#include <liblcd/liblcd.h>

#include "glue_user.h"

#define DEFAULT_GFP_FLAGS  (GFP_KERNEL)
#define verbose_debug 1
#define glue_pack(pos, msg, ext, value) glue_pack_impl((pos), (msg), (ext), (uint64_t)(value))
#define glue_pack_shadow(pos, msg, ext, value) glue_pack_shadow_impl((pos), (msg), (ext), (value))
#define glue_unpack(pos, msg, ext, type) (type)glue_unpack_impl((pos), (msg), (ext))
#define glue_unpack_shadow(pos, msg, ext, type) ({ \
	if (verbose_debug) \
		printk("%s:%d, unpack shadow for type %s\n", __func__, __LINE__, __stringify(type)); \
	(type)glue_unpack_shadow_impl(glue_unpack(pos, msg, ext, void*)); })

#define glue_unpack_new_shadow(pos, msg, ext, type, size, flags) ({ \
	if (verbose_debug) \
		printk("%s:%d, unpack new shadow for type %s | size %llu\n", __func__, __LINE__, __stringify(type), (uint64_t) size); \
	(type)glue_unpack_new_shadow_impl(glue_unpack(pos, msg, ext, void*), size, flags); })

#define glue_unpack_bind_or_new_shadow(pos, msg, ext, type, size) ({ \
	if (verbose_debug) \
		printk("%s:%d, unpack or bind new shadow for type %s | size %llu\n", __func__, __LINE__, __stringify(type), (uint64_t) size); \
	(type)glue_unpack_bind_or_new_shadow_impl(glue_unpack(pos, msg, ext, void*), size); })

#ifndef LCD_ISOLATE
#define glue_unpack_rpc_ptr(pos, msg, ext, name) \
	glue_peek(pos, msg, ext) ? (fptr_##name)glue_unpack_rpc_ptr_impl(glue_unpack(pos, msg, ext, void*), LCD_DUP_TRAMPOLINE(trmp_##name), LCD_TRAMPOLINE_SIZE(trmp_##name)) : NULL

#else
#define glue_unpack_rpc_ptr(pos, msg, ext, name) NULL; glue_user_panic("Trampolines cannot be used on LCD side")
#endif

#define glue_peek(pos, msg, ext) glue_peek_impl(pos, msg, ext)
#define glue_call_server(pos, msg, rpc_id) \
	msg->regs[0] = *pos; *pos = 0; glue_user_call_server(msg, rpc_id);

#define glue_remove_shadow(shadow) glue_user_remove_shadow(shadow)
#define glue_call_client(pos, msg, rpc_id) \
	msg->regs[0] = *pos; *pos = 0; glue_user_call_client(msg, rpc_id);

void glue_user_init(void);
void glue_user_panic(const char* msg);
void glue_user_trace(const char* msg);
void* glue_user_map_to_shadow(const void* obj, bool fail);
const void* glue_user_map_from_shadow(const void* shadow);
void glue_user_add_shadow(const void* ptr, void* shadow);
void* glue_user_alloc(size_t size, gfp_t flags);
void glue_user_free(void* ptr);
void glue_user_call_server(struct fipc_message* msg, size_t rpc_id);
void glue_user_call_client(struct fipc_message* msg, size_t rpc_id);
void glue_user_remove_shadow(void* shadow);

static inline void* glue_unpack_rpc_ptr_impl(void* target, struct lcd_trampoline_handle* handle, size_t size)
{
	if (!target)
		glue_user_panic("Target was NULL");

	if (!handle)
		glue_user_panic("Trmp was NULL");

	set_memory_x(((unsigned long)handle) & PAGE_MASK, ALIGN(size, PAGE_SIZE) >> PAGE_SHIFT);
	handle->hidden_args = target;
	return LCD_HANDLE_TO_TRAMPOLINE(handle);
}

static inline void
glue_pack_impl(size_t* pos, struct fipc_message* msg, struct ext_registers* ext, uint64_t value)
{
	if (*pos >= 512)
		glue_user_panic("Glue message was too large");
	if (*pos < 6)
		msg->regs[(*pos)++ + 1] = value;
	else
		ext->regs[(*pos)++ + 1] = value;
}

static inline uint64_t
glue_unpack_impl(size_t* pos, const struct fipc_message* msg, const struct ext_registers* ext)
{
	if (*pos >= msg->regs[0])
		glue_user_panic("Unpacked past end of glue message");
	if (*pos < 6)
		return msg->regs[(*pos)++ + 1];
	else
		return ext->regs[(*pos)++ + 1];
}

static inline uint64_t
glue_peek_impl(size_t* pos, const struct fipc_message* msg, const struct ext_registers* ext)
{
	if (*pos >= msg->regs[0])
		glue_user_panic("Peeked past end of glue message");
	if (*pos < 5)
		return msg->regs[*pos + 2];
	else
		return ext->regs[*pos + 2];
}

static inline void* glue_unpack_new_shadow_impl(const void* ptr, size_t size, gfp_t flags)
{
	void* shadow = 0;
	if (!ptr)
		return NULL;

	shadow = glue_user_alloc(size, flags);
	glue_user_add_shadow(ptr, shadow);
	return shadow;
}

static inline void* glue_unpack_bind_or_new_shadow_impl(const void* ptr, size_t size)
{
	void* shadow = 0;
	if (!ptr)
		return NULL;

	shadow = glue_user_map_to_shadow(ptr, false);
	if (!shadow) {
		shadow = glue_user_alloc(size, DEFAULT_GFP_FLAGS);
		glue_user_add_shadow(ptr, shadow);
	}
	return shadow;
}

static inline void* glue_unpack_shadow_impl(const void* ptr)
{
	return ptr ? glue_user_map_to_shadow(ptr, true) : NULL;
}

static inline void glue_pack_shadow_impl(size_t* pos, struct fipc_message* msg, struct ext_registers* ext, const void* ptr)
{
	glue_pack(pos, msg, ext, ptr ? glue_user_map_from_shadow(ptr) : NULL);
}

#ifdef LCD_ISOLATE
void shared_mem_init(void);
#else
void shared_mem_init_callee(struct fipc_message *msg, struct ext_registers* ext);
#endif	/* LCD_ISOLATE */


enum RPC_ID {
	MODULE_INIT,
	MODULE_EXIT,
	RPC_ID_shared_mem_init,
	RPC_ID_proto_init,
	RPC_ID_dev_get_by_index,
	RPC_ID_proto_ops_bind,
	RPC_ID_proto_ops_release,
	RPC_ID_proto_ops_getname,
	RPC_ID_proto_ops_setsockopt,
	RPC_ID_proto_ops_getsockopt,
	RPC_ID_proto_ops_sendmsg,
	RPC_ID_proto_ops_recvmsg,
	RPC_ID_proto_ops_mmap,
	RPC_ID_proto_ops_shutdown,
	RPC_ID_proto_ops_listen,
	RPC_ID_proto_ops_accept,
	RPC_ID_proto_ops_socketpair,
	RPC_ID_proto_ops_connect,
	RPC_ID_sock_alloc_send_skb,
	RPC_ID_warn_slowpath_fmt,
	RPC_ID_register_netdevice_notifier,
	RPC_ID_unregister_netdevice_notifier,
	RPC_ID_kfree_skb,
	RPC_ID_skb_clone,
	RPC_ID__raw_write_lock_bh,
	RPC_ID__raw_write_unlock_bh,
	RPC_ID___alloc_percpu,
	RPC_ID_free_percpu,
	RPC_ID_sock_queue_rcv_skb,
	RPC_ID_can_proto_unregister,
	RPC_ID_can_proto_register,
	RPC_ID_func,
	RPC_ID_can_rx_unregister,
	RPC_ID_can_rx_register,
	RPC_ID_can_send,
	RPC_ID_proto_ops_ioctl,
	RPC_ID_copy_from_iter,
	RPC_ID_copy_to_iter,
	RPC_ID_proto_ops_poll,
	RPC_ID_release_sock,
	RPC_ID_skb_free_datagram,
	RPC_ID_skb_recv_datagram,
	RPC_ID_proto_ops_sendpage,
	RPC_ID_lock_sock_nested,
	RPC_ID___sock_recv_ts_and_drops,
	RPC_ID___sock_tx_timestamp,
	RPC_ID_skb_put,
	RPC_ID_sk_free,
};

int try_dispatch(enum RPC_ID id, struct fipc_message* msg, struct ext_registers* ext);

typedef int (*fptr_proto_init)(struct sock* sk);
typedef int (*fptr_impl_proto_init)(fptr_proto_init target, struct sock* sk);

LCD_TRAMPOLINE_DATA(trmp_proto_init)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_init) trmp_proto_init(struct sock* sk);

typedef int (*fptr_proto_ops_bind)(struct socket* sock, struct sockaddr* uaddr, int len);
typedef int (*fptr_impl_proto_ops_bind)(fptr_proto_ops_bind target, struct socket* sock, struct sockaddr* uaddr, int len);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_bind)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_bind) trmp_proto_ops_bind(struct socket* sock, struct sockaddr* uaddr, int len);

typedef int (*fptr_proto_ops_release)(struct socket* sock);
typedef int (*fptr_impl_proto_ops_release)(fptr_proto_ops_release target, struct socket* sock);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_release)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_release) trmp_proto_ops_release(struct socket* sock);

typedef int (*fptr_proto_ops_getname)(struct socket* sock, struct sockaddr* uaddr, int* len, int peer);
typedef int (*fptr_impl_proto_ops_getname)(fptr_proto_ops_getname target, struct socket* sock, struct sockaddr* uaddr, int* len, int peer);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_getname)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_getname) trmp_proto_ops_getname(struct socket* sock, struct sockaddr* uaddr, int* len, int peer);

typedef int (*fptr_proto_ops_setsockopt)(struct socket* sock, int level, int optname, char* optval, unsigned int optlen);
typedef int (*fptr_impl_proto_ops_setsockopt)(fptr_proto_ops_setsockopt target, struct socket* sock, int level, int optname, char* optval, unsigned int optlen);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_setsockopt)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_setsockopt) trmp_proto_ops_setsockopt(struct socket* sock, int level, int optname, char* optval, unsigned int optlen);

typedef int (*fptr_proto_ops_getsockopt)(struct socket* sock, int level, int optname, char* optval, int* optlen);
typedef int (*fptr_impl_proto_ops_getsockopt)(fptr_proto_ops_getsockopt target, struct socket* sock, int level, int optname, char* optval, int* optlen);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_getsockopt)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_getsockopt) trmp_proto_ops_getsockopt(struct socket* sock, int level, int optname, char* optval, int* optlen);

typedef int (*fptr_proto_ops_sendmsg)(struct socket* sock, struct msghdr* msg, unsigned long size);
typedef int (*fptr_impl_proto_ops_sendmsg)(fptr_proto_ops_sendmsg target, struct socket* sock, struct msghdr* msg, unsigned long size);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_sendmsg)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_sendmsg) trmp_proto_ops_sendmsg(struct socket* sock, struct msghdr* msg, unsigned long size);

typedef int (*fptr_proto_ops_recvmsg)(struct socket* sock, struct msghdr* msg, unsigned long size, int flags);
typedef int (*fptr_impl_proto_ops_recvmsg)(fptr_proto_ops_recvmsg target, struct socket* sock, struct msghdr* msg, unsigned long size, int flags);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_recvmsg)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_recvmsg) trmp_proto_ops_recvmsg(struct socket* sock, struct msghdr* msg, unsigned long size, int flags);

typedef int (*fptr_proto_ops_mmap)(struct file* file, struct socket* sock, struct vm_area_struct* vma);
typedef int (*fptr_impl_proto_ops_mmap)(fptr_proto_ops_mmap target, struct file* file, struct socket* sock, struct vm_area_struct* vma);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_mmap)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_mmap) trmp_proto_ops_mmap(struct file* file, struct socket* sock, struct vm_area_struct* vma);

typedef int (*fptr_proto_ops_shutdown)(struct socket* sock, int how);
typedef int (*fptr_impl_proto_ops_shutdown)(fptr_proto_ops_shutdown target, struct socket* sock, int how);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_shutdown)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_shutdown) trmp_proto_ops_shutdown(struct socket* sock, int how);

typedef int (*fptr_proto_ops_listen)(struct socket* sock, int backlog);
typedef int (*fptr_impl_proto_ops_listen)(fptr_proto_ops_listen target, struct socket* sock, int backlog);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_listen)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_listen) trmp_proto_ops_listen(struct socket* sock, int backlog);

typedef int (*fptr_proto_ops_accept)(struct socket* sock, struct socket* newsock, int flags);
typedef int (*fptr_impl_proto_ops_accept)(fptr_proto_ops_accept target, struct socket* sock, struct socket* newsock, int flags);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_accept)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_accept) trmp_proto_ops_accept(struct socket* sock, struct socket* newsock, int flags);

typedef int (*fptr_proto_ops_socketpair)(struct socket* sock1, struct socket* sock2);
typedef int (*fptr_impl_proto_ops_socketpair)(fptr_proto_ops_socketpair target, struct socket* sock1, struct socket* sock2);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_socketpair)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_socketpair) trmp_proto_ops_socketpair(struct socket* sock1, struct socket* sock2);

typedef int (*fptr_proto_ops_connect)(struct socket* sock, struct sockaddr* saddr, int len, int flags);
typedef int (*fptr_impl_proto_ops_connect)(fptr_proto_ops_connect target, struct socket* sock, struct sockaddr* saddr, int len, int flags);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_connect)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_connect) trmp_proto_ops_connect(struct socket* sock, struct sockaddr* saddr, int len, int flags);

typedef void (*fptr_func)(struct sk_buff* skb, void* data);
typedef void (*fptr_impl_func)(fptr_func target, struct sk_buff* skb, void* data);

LCD_TRAMPOLINE_DATA(trmp_func)
void LCD_TRAMPOLINE_LINKAGE(trmp_func) trmp_func(struct sk_buff* skb, void* data);

typedef int (*fptr_proto_ops_ioctl)(struct socket* sock, unsigned int cmd, unsigned long arg);
typedef int (*fptr_impl_proto_ops_ioctl)(fptr_proto_ops_ioctl target, struct socket* sock, unsigned int cmd, unsigned long arg);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_ioctl)
int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_ioctl) trmp_proto_ops_ioctl(struct socket* sock, unsigned int cmd, unsigned long arg);

typedef unsigned int (*fptr_proto_ops_poll)(struct file* file, struct socket* sock, struct poll_table_struct* wait);
typedef unsigned int (*fptr_impl_proto_ops_poll)(fptr_proto_ops_poll target, struct file* file, struct socket* sock, struct poll_table_struct* wait);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_poll)
unsigned int LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_poll) trmp_proto_ops_poll(struct file* file, struct socket* sock, struct poll_table_struct* wait);

typedef long (*fptr_proto_ops_sendpage)(struct socket* sock, struct page* page, int offset, unsigned long size, int flags);
typedef long (*fptr_impl_proto_ops_sendpage)(fptr_proto_ops_sendpage target, struct socket* sock, struct page* page, int offset, unsigned long size, int flags);

LCD_TRAMPOLINE_DATA(trmp_proto_ops_sendpage)
long LCD_TRAMPOLINE_LINKAGE(trmp_proto_ops_sendpage) trmp_proto_ops_sendpage(struct socket* sock, struct page* page, int offset, unsigned long size, int flags);

struct proto_init_call_ctx {
	struct sock* sk;
};

struct dev_get_by_index_call_ctx {
	struct net* net;
	int ifindex;
};

struct proto_ops_bind_call_ctx {
	struct socket* sock;
	struct sockaddr* uaddr;
	int len;
};

struct proto_ops_release_call_ctx {
	struct socket* sock;
};

struct proto_ops_getname_call_ctx {
	struct socket* sock;
	struct sockaddr* uaddr;
	int* len;
	int peer;
};

struct proto_ops_setsockopt_call_ctx {
	struct socket* sock;
	int level;
	int optname;
	char* optval;
	unsigned int optlen;
};

struct proto_ops_getsockopt_call_ctx {
	struct socket* sock;
	int level;
	int optname;
	char* optval;
	int* optlen;
};

struct proto_ops_sendmsg_call_ctx {
	struct socket* sock;
	struct msghdr* msg;
	unsigned long size;
};

struct proto_ops_recvmsg_call_ctx {
	struct socket* sock;
	struct msghdr* msg;
	unsigned long size;
	int flags;
};

struct proto_ops_mmap_call_ctx {
	struct file* file;
	struct socket* sock;
	struct vm_area_struct* vma;
};

struct proto_ops_shutdown_call_ctx {
	struct socket* sock;
	int how;
};

struct proto_ops_listen_call_ctx {
	struct socket* sock;
	int backlog;
};

struct proto_ops_accept_call_ctx {
	struct socket* sock;
	struct socket* newsock;
	int flags;
};

struct proto_ops_socketpair_call_ctx {
	struct socket* sock1;
	struct socket* sock2;
};

struct proto_ops_connect_call_ctx {
	struct socket* sock;
	struct sockaddr* saddr;
	int len;
	int flags;
};

struct sock_alloc_send_skb_call_ctx {
	struct sock* sk;
	unsigned long size;
	int noblock;
	int* errcode;
};

struct warn_slowpath_fmt_call_ctx {
	char* file;
	int line;
	char* fmt;
};

struct register_netdevice_notifier_call_ctx {
	struct notifier_block* nb;
};

struct unregister_netdevice_notifier_call_ctx {
	struct notifier_block* nb;
};

struct kfree_skb_call_ctx {
	struct sk_buff* skb;
};

struct skb_clone_call_ctx {
	struct sk_buff* skb;
	unsigned int gfp_mask;
};

struct _raw_write_lock_bh_call_ctx {
	struct rwlock_t* lock;
};

struct _raw_write_unlock_bh_call_ctx {
	struct rwlock_t* lock;
};

struct __alloc_percpu_call_ctx {
	unsigned long size;
	unsigned long align;
};

struct free_percpu_call_ctx {
	void* ptr;
};

struct sock_queue_rcv_skb_call_ctx {
	struct sock* sk;
	struct sk_buff* skb;
};

struct can_proto_unregister_call_ctx {
	struct can_proto* cp;
};

struct can_proto_register_call_ctx {
	struct can_proto* cp;
};

struct func_call_ctx {
	struct sk_buff* skb;
	void* data;
};

struct can_rx_unregister_call_ctx {
	struct net_device* dev;
	unsigned int can_id;
	unsigned int mask;
	fptr_func func;
	void* data;
};

struct can_rx_register_call_ctx {
	struct net_device* dev;
	unsigned int can_id;
	unsigned int mask;
	fptr_func func;
	void* data;
	char* ident;
};

struct can_send_call_ctx {
	struct sk_buff* skb;
	int loop;
};

struct proto_ops_ioctl_call_ctx {
	struct socket* sock;
	unsigned int cmd;
	unsigned long arg;
};

struct copy_from_iter_call_ctx {
	void* addr;
	unsigned long bytes;
	struct iov_iter* i;
};

struct copy_to_iter_call_ctx {
	void const* addr;
	unsigned long bytes;
	struct iov_iter* i;
};

struct proto_ops_poll_call_ctx {
	struct file* file;
	struct socket* sock;
	struct poll_table_struct* wait;
};

struct release_sock_call_ctx {
	struct sock* sk;
};

struct skb_free_datagram_call_ctx {
	struct sock* sk;
	struct sk_buff* skb;
};

struct skb_recv_datagram_call_ctx {
	struct sock* sk;
	unsigned int flags;
	int noblock;
	int* err;
};

struct proto_ops_sendpage_call_ctx {
	struct socket* sock;
	struct page* page;
	int offset;
	unsigned long size;
	int flags;
};

struct lock_sock_nested_call_ctx {
	struct sock* sk;
	int subclass;
};

struct __sock_recv_ts_and_drops_call_ctx {
	struct msghdr* msg;
	struct sock* sk;
	struct sk_buff* skb;
};

struct __sock_tx_timestamp_call_ctx {
	unsigned short tsflags;
	unsigned char* tx_flags;
};

struct skb_put_call_ctx {
	struct sk_buff* skb;
	unsigned int len;
};

struct sk_free_call_ctx {
	struct sock* sk;
};

void caller_marshal_kernel__proto_init__sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_init_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__proto_init__sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_init_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__proto_init__sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_init_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__proto_init__sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_init_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__dev_get_by_index__ret_net_device__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dev_get_by_index_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__dev_get_by_index__ret_net_device__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dev_get_by_index_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__dev_get_by_index__ret_net_device__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dev_get_by_index_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__dev_get_by_index__ret_net_device__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dev_get_by_index_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__dev_get_by_index__net__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dev_get_by_index_call_ctx const* call_ctx,
	struct net const* ptr);

void callee_unmarshal_kernel__dev_get_by_index__net__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dev_get_by_index_call_ctx const* call_ctx,
	struct net* ptr);

void callee_marshal_kernel__dev_get_by_index__net__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct dev_get_by_index_call_ctx const* call_ctx,
	struct net const* ptr);

void caller_unmarshal_kernel__dev_get_by_index__net__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct dev_get_by_index_call_ctx const* call_ctx,
	struct net* ptr);

void caller_marshal_kernel__proto_ops_bind__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_bind_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_bind__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_bind_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_bind__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_bind_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_bind__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_bind_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_bind__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_bind_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__proto_ops_bind__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_bind_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__proto_ops_bind__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_bind_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__proto_ops_bind__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_bind_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__proto_ops_bind__uaddr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_bind_call_ctx const* call_ctx,
	struct sockaddr const* ptr);

void callee_unmarshal_kernel__proto_ops_bind__uaddr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_bind_call_ctx const* call_ctx,
	struct sockaddr* ptr);

void callee_marshal_kernel__proto_ops_bind__uaddr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_bind_call_ctx const* call_ctx,
	struct sockaddr const* ptr);

void caller_unmarshal_kernel__proto_ops_bind__uaddr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_bind_call_ctx const* call_ctx,
	struct sockaddr* ptr);

void caller_marshal_kernel__proto_ops_release__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_release_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_release__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_release_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_release__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_release_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_release__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_release_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_release__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_release_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__proto_ops_release__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_release_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__proto_ops_release__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_release_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__proto_ops_release__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_release_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__proto_ops_getname__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_getname_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_getname__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_getname_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_getname__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_getname_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_getname__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_getname_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_getname__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_getname_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__proto_ops_getname__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_getname_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__proto_ops_getname__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_getname_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__proto_ops_getname__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_getname_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__proto_ops_getname__uaddr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_getname_call_ctx const* call_ctx,
	struct sockaddr const* ptr);

void callee_unmarshal_kernel__proto_ops_getname__uaddr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_getname_call_ctx const* call_ctx,
	struct sockaddr* ptr);

void callee_marshal_kernel__proto_ops_getname__uaddr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_getname_call_ctx const* call_ctx,
	struct sockaddr const* ptr);

void caller_unmarshal_kernel__proto_ops_getname__uaddr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_getname_call_ctx const* call_ctx,
	struct sockaddr* ptr);

void caller_marshal_kernel__proto_ops_setsockopt__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_setsockopt_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_setsockopt__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_setsockopt_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_setsockopt__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_setsockopt_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_setsockopt__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_setsockopt_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_setsockopt__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_setsockopt_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__proto_ops_setsockopt__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_setsockopt_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__proto_ops_setsockopt__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_setsockopt_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__proto_ops_setsockopt__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_setsockopt_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__proto_ops_getsockopt__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_getsockopt_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_getsockopt__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_getsockopt_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_getsockopt__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_getsockopt_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_getsockopt__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_getsockopt_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_getsockopt__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_getsockopt_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__proto_ops_getsockopt__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_getsockopt_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__proto_ops_getsockopt__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_getsockopt_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__proto_ops_getsockopt__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_getsockopt_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__proto_ops_sendmsg__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_sendmsg__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_sendmsg__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_sendmsg__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_sendmsg__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__proto_ops_sendmsg__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__proto_ops_sendmsg__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__proto_ops_sendmsg__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__proto_ops_sendmsg__msg__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct msghdr const* ptr);

void callee_unmarshal_kernel__proto_ops_sendmsg__msg__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct msghdr* ptr);

void callee_marshal_kernel__proto_ops_sendmsg__msg__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct msghdr const* ptr);

void caller_unmarshal_kernel__proto_ops_sendmsg__msg__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct msghdr* ptr);

void caller_marshal_kernel__proto_ops_sendmsg__msghdr_msg_iocb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct kiocb const* ptr);

void callee_unmarshal_kernel__proto_ops_sendmsg__msghdr_msg_iocb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct kiocb* ptr);

void callee_marshal_kernel__proto_ops_sendmsg__msghdr_msg_iocb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct kiocb const* ptr);

void caller_unmarshal_kernel__proto_ops_sendmsg__msghdr_msg_iocb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_sendmsg_call_ctx const* call_ctx,
	struct kiocb* ptr);

void caller_marshal_kernel__proto_ops_recvmsg__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_recvmsg_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_recvmsg__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_recvmsg_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_recvmsg__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_recvmsg_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_recvmsg__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_recvmsg_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_recvmsg__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_recvmsg_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__proto_ops_recvmsg__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_recvmsg_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__proto_ops_recvmsg__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_recvmsg_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__proto_ops_recvmsg__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_recvmsg_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__proto_ops_recvmsg__msg__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_recvmsg_call_ctx const* call_ctx,
	struct msghdr const* ptr);

void callee_unmarshal_kernel__proto_ops_recvmsg__msg__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_recvmsg_call_ctx const* call_ctx,
	struct msghdr* ptr);

void callee_marshal_kernel__proto_ops_recvmsg__msg__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_recvmsg_call_ctx const* call_ctx,
	struct msghdr const* ptr);

void caller_unmarshal_kernel__proto_ops_recvmsg__msg__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_recvmsg_call_ctx const* call_ctx,
	struct msghdr* ptr);

void caller_marshal_kernel__proto_ops_mmap__file__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_mmap_call_ctx const* call_ctx,
	struct file const* ptr);

void callee_unmarshal_kernel__proto_ops_mmap__file__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_mmap_call_ctx const* call_ctx,
	struct file* ptr);

void callee_marshal_kernel__proto_ops_mmap__file__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_mmap_call_ctx const* call_ctx,
	struct file const* ptr);

void caller_unmarshal_kernel__proto_ops_mmap__file__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_mmap_call_ctx const* call_ctx,
	struct file* ptr);

void caller_marshal_kernel__proto_ops_mmap__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_mmap_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_mmap__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_mmap_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_mmap__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_mmap_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_mmap__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_mmap_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_mmap__vma__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_mmap_call_ctx const* call_ctx,
	struct vm_area_struct const* ptr);

void callee_unmarshal_kernel__proto_ops_mmap__vma__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_mmap_call_ctx const* call_ctx,
	struct vm_area_struct* ptr);

void callee_marshal_kernel__proto_ops_mmap__vma__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_mmap_call_ctx const* call_ctx,
	struct vm_area_struct const* ptr);

void caller_unmarshal_kernel__proto_ops_mmap__vma__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_mmap_call_ctx const* call_ctx,
	struct vm_area_struct* ptr);

void caller_marshal_kernel__proto_ops_shutdown__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_shutdown_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_shutdown__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_shutdown_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_shutdown__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_shutdown_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_shutdown__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_shutdown_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_listen__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_listen_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_listen__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_listen_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_listen__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_listen_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_listen__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_listen_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_accept__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_accept_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_accept__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_accept_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_accept__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_accept_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_accept__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_accept_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_accept__newsock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_accept_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_accept__newsock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_accept_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_accept__newsock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_accept_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_accept__newsock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_accept_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_socketpair__sock1__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_socketpair_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_socketpair__sock1__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_socketpair_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_socketpair__sock1__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_socketpair_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_socketpair__sock1__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_socketpair_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_socketpair__sock2__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_socketpair_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_socketpair__sock2__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_socketpair_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_socketpair__sock2__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_socketpair_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_socketpair__sock2__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_socketpair_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_connect__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_connect_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_connect__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_connect_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_connect__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_connect_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_connect__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_connect_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_connect__saddr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_connect_call_ctx const* call_ctx,
	struct sockaddr const* ptr);

void callee_unmarshal_kernel__proto_ops_connect__saddr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_connect_call_ctx const* call_ctx,
	struct sockaddr* ptr);

void callee_marshal_kernel__proto_ops_connect__saddr__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_connect_call_ctx const* call_ctx,
	struct sockaddr const* ptr);

void caller_unmarshal_kernel__proto_ops_connect__saddr__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_connect_call_ctx const* call_ctx,
	struct sockaddr* ptr);

void caller_marshal_kernel__sock_alloc_send_skb__ret_sk_buff__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sock_alloc_send_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__sock_alloc_send_skb__ret_sk_buff__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sock_alloc_send_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__sock_alloc_send_skb__ret_sk_buff__out(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sock_alloc_send_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__sock_alloc_send_skb__ret_sk_buff__out(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sock_alloc_send_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__sock_alloc_send_skb__sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sock_alloc_send_skb_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__sock_alloc_send_skb__sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sock_alloc_send_skb_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__sock_alloc_send_skb__sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sock_alloc_send_skb_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__sock_alloc_send_skb__sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sock_alloc_send_skb_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__sock_alloc_send_skb__sock_sk_socket__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sock_alloc_send_skb_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__sock_alloc_send_skb__sock_sk_socket__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sock_alloc_send_skb_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__sock_alloc_send_skb__sock_sk_socket__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sock_alloc_send_skb_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__sock_alloc_send_skb__sock_sk_socket__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sock_alloc_send_skb_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__register_netdevice_notifier__nb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct register_netdevice_notifier_call_ctx const* call_ctx,
	struct notifier_block const* ptr);

void callee_unmarshal_kernel__register_netdevice_notifier__nb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct register_netdevice_notifier_call_ctx const* call_ctx,
	struct notifier_block* ptr);

void callee_marshal_kernel__register_netdevice_notifier__nb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct register_netdevice_notifier_call_ctx const* call_ctx,
	struct notifier_block const* ptr);

void caller_unmarshal_kernel__register_netdevice_notifier__nb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct register_netdevice_notifier_call_ctx const* call_ctx,
	struct notifier_block* ptr);

void caller_marshal_kernel__unregister_netdevice_notifier__nb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct unregister_netdevice_notifier_call_ctx const* call_ctx,
	struct notifier_block const* ptr);

void callee_unmarshal_kernel__unregister_netdevice_notifier__nb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct unregister_netdevice_notifier_call_ctx const* call_ctx,
	struct notifier_block* ptr);

void callee_marshal_kernel__unregister_netdevice_notifier__nb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct unregister_netdevice_notifier_call_ctx const* call_ctx,
	struct notifier_block const* ptr);

void caller_unmarshal_kernel__unregister_netdevice_notifier__nb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct unregister_netdevice_notifier_call_ctx const* call_ctx,
	struct notifier_block* ptr);

void caller_marshal_kernel__kfree_skb__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct kfree_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__kfree_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct kfree_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__kfree_skb__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct kfree_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__kfree_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct kfree_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__skb_clone__ret_sk_buff__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__skb_clone__ret_sk_buff__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__skb_clone__ret_sk_buff__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__skb_clone__ret_sk_buff__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__skb_clone__skb__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__skb_clone__skb__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__skb_clone__skb__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__skb_clone__skb__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__skb_clone__sk_buff_sk__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__skb_clone__sk_buff_sk__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__skb_clone__sk_buff_sk__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__skb_clone__sk_buff_sk__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__skb_clone__sk_buff_dev__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__skb_clone__sk_buff_dev__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__skb_clone__sk_buff_dev__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__skb_clone__sk_buff_dev__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_clone_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel___raw_write_lock_bh__lock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct _raw_write_lock_bh_call_ctx const* call_ctx,
	struct rwlock_t const* ptr);

void callee_unmarshal_kernel___raw_write_lock_bh__lock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct _raw_write_lock_bh_call_ctx const* call_ctx,
	struct rwlock_t* ptr);

void callee_marshal_kernel___raw_write_lock_bh__lock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct _raw_write_lock_bh_call_ctx const* call_ctx,
	struct rwlock_t const* ptr);

void caller_unmarshal_kernel___raw_write_lock_bh__lock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct _raw_write_lock_bh_call_ctx const* call_ctx,
	struct rwlock_t* ptr);

void caller_marshal_kernel___raw_write_unlock_bh__lock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct _raw_write_unlock_bh_call_ctx const* call_ctx,
	struct rwlock_t const* ptr);

void callee_unmarshal_kernel___raw_write_unlock_bh__lock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct _raw_write_unlock_bh_call_ctx const* call_ctx,
	struct rwlock_t* ptr);

void callee_marshal_kernel___raw_write_unlock_bh__lock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct _raw_write_unlock_bh_call_ctx const* call_ctx,
	struct rwlock_t const* ptr);

void caller_unmarshal_kernel___raw_write_unlock_bh__lock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct _raw_write_unlock_bh_call_ctx const* call_ctx,
	struct rwlock_t* ptr);

void caller_marshal_kernel__sock_queue_rcv_skb__sk__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sock_queue_rcv_skb_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__sock_queue_rcv_skb__sk__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sock_queue_rcv_skb_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__sock_queue_rcv_skb__sk__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sock_queue_rcv_skb_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__sock_queue_rcv_skb__sk__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sock_queue_rcv_skb_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__sock_queue_rcv_skb__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sock_queue_rcv_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__sock_queue_rcv_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sock_queue_rcv_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__sock_queue_rcv_skb__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sock_queue_rcv_skb_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__sock_queue_rcv_skb__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sock_queue_rcv_skb_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__can_proto_unregister__cp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_proto_unregister_call_ctx const* call_ctx,
	struct can_proto const* ptr);

void callee_unmarshal_kernel__can_proto_unregister__cp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_proto_unregister_call_ctx const* call_ctx,
	struct can_proto* ptr);

void callee_marshal_kernel__can_proto_unregister__cp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_proto_unregister_call_ctx const* call_ctx,
	struct can_proto const* ptr);

void caller_unmarshal_kernel__can_proto_unregister__cp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_proto_unregister_call_ctx const* call_ctx,
	struct can_proto* ptr);

void caller_marshal_kernel___global_proto_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops const* ptr);

void callee_unmarshal_kernel___global_proto_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops* ptr);

void callee_marshal_kernel___global_proto_ops__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops const* ptr);

void caller_unmarshal_kernel___global_proto_ops__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops* ptr);

void caller_marshal_kernel__can_proto_unregister__can_proto_prot__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_proto_unregister_call_ctx const* call_ctx,
	struct proto const* ptr);

void callee_unmarshal_kernel__can_proto_unregister__can_proto_prot__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_proto_unregister_call_ctx const* call_ctx,
	struct proto* ptr);

void callee_marshal_kernel__can_proto_unregister__can_proto_prot__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_proto_unregister_call_ctx const* call_ctx,
	struct proto const* ptr);

void caller_unmarshal_kernel__can_proto_unregister__can_proto_prot__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_proto_unregister_call_ctx const* call_ctx,
	struct proto* ptr);

void caller_marshal_kernel__can_proto_register__cp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_proto_register_call_ctx const* call_ctx,
	struct can_proto const* ptr);

void callee_unmarshal_kernel__can_proto_register__cp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_proto_register_call_ctx const* call_ctx,
	struct can_proto* ptr);

void callee_marshal_kernel__can_proto_register__cp__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_proto_register_call_ctx const* call_ctx,
	struct can_proto const* ptr);

void caller_unmarshal_kernel__can_proto_register__cp__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_proto_register_call_ctx const* call_ctx,
	struct can_proto* ptr);

void caller_marshal_kernel__can_proto_register__can_proto_prot__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_proto_register_call_ctx const* call_ctx,
	struct proto const* ptr);

void callee_unmarshal_kernel__can_proto_register__can_proto_prot__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_proto_register_call_ctx const* call_ctx,
	struct proto* ptr);

void callee_marshal_kernel__can_proto_register__can_proto_prot__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_proto_register_call_ctx const* call_ctx,
	struct proto const* ptr);

void caller_unmarshal_kernel__can_proto_register__can_proto_prot__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_proto_register_call_ctx const* call_ctx,
	struct proto* ptr);

void caller_marshal_kernel__func__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct func_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__func__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct func_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__func__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct func_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__func__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct func_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__can_rx_unregister__dev__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_rx_unregister_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__can_rx_unregister__dev__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_rx_unregister_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__can_rx_unregister__dev__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_rx_unregister_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__can_rx_unregister__dev__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_rx_unregister_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__can_rx_register__dev__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_rx_register_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__can_rx_register__dev__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_rx_register_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__can_rx_register__dev__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_rx_register_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__can_rx_register__dev__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_rx_register_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__can_send__skb__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_send_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__can_send__skb__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_send_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__can_send__skb__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_send_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__can_send__skb__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_send_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__can_send__sk_buff_sk__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_send_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__can_send__sk_buff_sk__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_send_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__can_send__sk_buff_sk__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_send_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__can_send__sk_buff_sk__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_send_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__can_send__sk_buff_dev__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_send_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__can_send__sk_buff_dev__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_send_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__can_send__sk_buff_dev__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct can_send_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__can_send__sk_buff_dev__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct can_send_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__proto_ops_ioctl__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_ioctl_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_ioctl__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_ioctl_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_ioctl__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_ioctl_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_ioctl__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_ioctl_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_ioctl__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_ioctl_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__proto_ops_ioctl__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_ioctl_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__proto_ops_ioctl__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_ioctl_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__proto_ops_ioctl__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_ioctl_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__copy_from_iter__i__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct copy_from_iter_call_ctx const* call_ctx,
	struct iov_iter const* ptr);

void callee_unmarshal_kernel__copy_from_iter__i__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct copy_from_iter_call_ctx const* call_ctx,
	struct iov_iter* ptr);

void callee_marshal_kernel__copy_from_iter__i__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct copy_from_iter_call_ctx const* call_ctx,
	struct iov_iter const* ptr);

void caller_unmarshal_kernel__copy_from_iter__i__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct copy_from_iter_call_ctx const* call_ctx,
	struct iov_iter* ptr);

void caller_marshal_kernel__copy_to_iter__i__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct copy_to_iter_call_ctx const* call_ctx,
	struct iov_iter const* ptr);

void callee_unmarshal_kernel__copy_to_iter__i__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct copy_to_iter_call_ctx const* call_ctx,
	struct iov_iter* ptr);

void callee_marshal_kernel__copy_to_iter__i__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct copy_to_iter_call_ctx const* call_ctx,
	struct iov_iter const* ptr);

void caller_unmarshal_kernel__copy_to_iter__i__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct copy_to_iter_call_ctx const* call_ctx,
	struct iov_iter* ptr);

void caller_marshal_kernel__proto_ops_poll__file__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct file const* ptr);

void callee_unmarshal_kernel__proto_ops_poll__file__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct file* ptr);

void callee_marshal_kernel__proto_ops_poll__file__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct file const* ptr);

void caller_unmarshal_kernel__proto_ops_poll__file__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct file* ptr);

void caller_marshal_kernel__proto_ops_poll__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_poll__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_poll__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_poll__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_poll__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__proto_ops_poll__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__proto_ops_poll__socket_sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__proto_ops_poll__socket_sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__proto_ops_poll__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct poll_table_struct const* ptr);

void callee_unmarshal_kernel__proto_ops_poll__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct poll_table_struct* ptr);

void callee_marshal_kernel__proto_ops_poll__wait__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct poll_table_struct const* ptr);

void caller_unmarshal_kernel__proto_ops_poll__wait__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_poll_call_ctx const* call_ctx,
	struct poll_table_struct* ptr);

void caller_marshal_kernel__release_sock__sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct release_sock_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__release_sock__sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct release_sock_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__release_sock__sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct release_sock_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__release_sock__sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct release_sock_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__skb_free_datagram__sk__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_free_datagram_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__skb_free_datagram__sk__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_free_datagram_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__skb_free_datagram__sk__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_free_datagram_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__skb_free_datagram__sk__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_free_datagram_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__skb_free_datagram__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_free_datagram_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__skb_free_datagram__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_free_datagram_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__skb_free_datagram__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_free_datagram_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__skb_free_datagram__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_free_datagram_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__skb_recv_datagram__ret_sk_buff__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_recv_datagram_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__skb_recv_datagram__ret_sk_buff__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_recv_datagram_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__skb_recv_datagram__ret_sk_buff__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_recv_datagram_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__skb_recv_datagram__ret_sk_buff__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_recv_datagram_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__skb_recv_datagram__sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_recv_datagram_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__skb_recv_datagram__sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_recv_datagram_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__skb_recv_datagram__sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_recv_datagram_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__skb_recv_datagram__sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_recv_datagram_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel__proto_ops_sendpage__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_sendpage_call_ctx const* call_ctx,
	struct socket const* ptr);

void callee_unmarshal_kernel__proto_ops_sendpage__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_sendpage_call_ctx const* call_ctx,
	struct socket* ptr);

void callee_marshal_kernel__proto_ops_sendpage__sock__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_sendpage_call_ctx const* call_ctx,
	struct socket const* ptr);

void caller_unmarshal_kernel__proto_ops_sendpage__sock__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_sendpage_call_ctx const* call_ctx,
	struct socket* ptr);

void caller_marshal_kernel__proto_ops_sendpage__page__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_sendpage_call_ctx const* call_ctx,
	struct page const* ptr);

void callee_unmarshal_kernel__proto_ops_sendpage__page__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_sendpage_call_ctx const* call_ctx,
	struct page* ptr);

void callee_marshal_kernel__proto_ops_sendpage__page__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct proto_ops_sendpage_call_ctx const* call_ctx,
	struct page const* ptr);

void caller_unmarshal_kernel__proto_ops_sendpage__page__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct proto_ops_sendpage_call_ctx const* call_ctx,
	struct page* ptr);

void caller_marshal_kernel__lock_sock_nested__sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct lock_sock_nested_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__lock_sock_nested__sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct lock_sock_nested_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__lock_sock_nested__sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct lock_sock_nested_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__lock_sock_nested__sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct lock_sock_nested_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel____sock_recv_ts_and_drops__msg__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __sock_recv_ts_and_drops_call_ctx const* call_ctx,
	struct msghdr const* ptr);

void callee_unmarshal_kernel____sock_recv_ts_and_drops__msg__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __sock_recv_ts_and_drops_call_ctx const* call_ctx,
	struct msghdr* ptr);

void callee_marshal_kernel____sock_recv_ts_and_drops__msg__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __sock_recv_ts_and_drops_call_ctx const* call_ctx,
	struct msghdr const* ptr);

void caller_unmarshal_kernel____sock_recv_ts_and_drops__msg__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __sock_recv_ts_and_drops_call_ctx const* call_ctx,
	struct msghdr* ptr);

void caller_marshal_kernel____sock_recv_ts_and_drops__sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __sock_recv_ts_and_drops_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel____sock_recv_ts_and_drops__sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __sock_recv_ts_and_drops_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel____sock_recv_ts_and_drops__sk__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __sock_recv_ts_and_drops_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel____sock_recv_ts_and_drops__sk__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __sock_recv_ts_and_drops_call_ctx const* call_ctx,
	struct sock* ptr);

void caller_marshal_kernel____sock_recv_ts_and_drops__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __sock_recv_ts_and_drops_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel____sock_recv_ts_and_drops__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __sock_recv_ts_and_drops_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel____sock_recv_ts_and_drops__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct __sock_recv_ts_and_drops_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel____sock_recv_ts_and_drops__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct __sock_recv_ts_and_drops_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__skb_put__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_put_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void callee_unmarshal_kernel__skb_put__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_put_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void callee_marshal_kernel__skb_put__skb__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_put_call_ctx const* call_ctx,
	struct sk_buff const* ptr);

void caller_unmarshal_kernel__skb_put__skb__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_put_call_ctx const* call_ctx,
	struct sk_buff* ptr);

void caller_marshal_kernel__skb_put__sk_buff_dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_put_call_ctx const* call_ctx,
	struct net_device const* ptr);

void callee_unmarshal_kernel__skb_put__sk_buff_dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_put_call_ctx const* call_ctx,
	struct net_device* ptr);

void callee_marshal_kernel__skb_put__sk_buff_dev__in(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct skb_put_call_ctx const* call_ctx,
	struct net_device const* ptr);

void caller_unmarshal_kernel__skb_put__sk_buff_dev__in(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct skb_put_call_ctx const* call_ctx,
	struct net_device* ptr);

void caller_marshal_kernel__sk_free__sk__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sk_free_call_ctx const* call_ctx,
	struct sock const* ptr);

void callee_unmarshal_kernel__sk_free__sk__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sk_free_call_ctx const* call_ctx,
	struct sock* ptr);

void callee_marshal_kernel__sk_free__sk__io(
	size_t* __pos,
	struct fipc_message* msg,
	struct ext_registers* ext,
	struct sk_free_call_ctx const* call_ctx,
	struct sock const* ptr);

void caller_unmarshal_kernel__sk_free__sk__io(
	size_t* __pos,
	const struct fipc_message* msg,
	const struct ext_registers* ext,
	struct sk_free_call_ctx const* call_ctx,
	struct sock* ptr);


#endif
