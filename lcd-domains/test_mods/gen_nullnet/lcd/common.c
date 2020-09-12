#include <lcd_config/pre_hook.h>
#include "../common.h"
#include <lcd_config/post_hook.h>

DEFINE_HASHTABLE(locals, 5);
DEFINE_HASHTABLE(remotes, 5);

void* fipc_get_remote_impl(void* local) {
	LIBLCD_MSG("Finding remote for %p", local);
	struct ptr_node* node;
	hash_for_each_possible(remotes, node, hentry, (unsigned long)local) {
		LIBLCD_MSG("Observed %p", node->key);
		if (node->key == local)
			return node->ptr;
	}

	BUG();
	return NULL;
}

void* fipc_get_local_impl(void* remote) {
	LIBLCD_MSG("Finding shadow for %p", remote);
	struct ptr_node* node;
	hash_for_each_possible(locals, node, hentry, (unsigned long)remote) {
		LIBLCD_MSG("Observed %p", node->key);
		if (node->key == remote)
			return node->ptr;
	}

	BUG();
	return NULL;
}

void* fipc_create_shadow_impl(void* remote, size_t size) {
	void* local = kmalloc(size, GFP_KERNEL);
	struct ptr_node* local_node = kmalloc(sizeof(struct ptr_node), GFP_KERNEL);
	struct ptr_node* remote_node = kmalloc(sizeof(struct ptr_node), GFP_KERNEL);
	local_node->ptr = local;
	local_node->key = remote;
	remote_node->ptr = remote;
	remote_node->key = local;
	hash_add(locals, &local_node->hentry, (unsigned long)remote);
	hash_add(remotes, &remote_node->hentry, (unsigned long)local);
	LIBLCD_MSG("Pointer %p assigned shadow %p", remote, local);
	return local;
}

void fipc_destroy_shadow_impl(void* remote) {
	struct ptr_node* local_node;
	struct ptr_node* remote_node;
	hash_for_each_possible(locals, local_node, hentry, (unsigned long)remote) {
		if (local_node->key == remote) {
			hash_del(&local_node->hentry);
			break;
		}
	}

	void* local = local_node->ptr;
	hash_for_each_possible(remotes, remote_node, hentry, (unsigned long)local) {
		if (remote_node->key == local) {
			hash_del(&remote_node->hentry);
			break;
		}
	}

	kfree(local);
	kfree(local_node);
	kfree(remote_node);
	LIBLCD_MSG("Destroyed shadow for %p", remote);
}

void* inject_trampoline_impl(struct lcd_trampoline_handle* handle, void* impl, size_t size, void* real_ptr) {
	struct trampoline_args* args = kmalloc(sizeof(struct trampoline_args), GFP_KERNEL);
	args->impl = impl;
	args->real_ptr = real_ptr;
	handle->hidden_args = args;
	set_memory_x(((unsigned long)handle) & PAGE_MASK, ALIGN(size, PAGE_SIZE) >> PAGE_SHIFT);
	return handle->trampoline;
}

void fipc_translate_impl(struct fipc_message* msg, enum dispatch_id* rpc, struct rpc_message* pckt) {
	pckt->end_slot = pckt->slots;

	unsigned slots_used = msg->rpc_id >> 16;
	*rpc = msg->rpc_id & 0xFFFF;

	unsigned fast_slots = min(slots_used, (unsigned)(FIPC_NR_REGS));
	unsigned slow_slots = (slots_used > FIPC_NR_REGS) ? slots_used - FIPC_NR_REGS : 0;

	for (unsigned i = 0; i < fast_slots; ++i) {
		pckt->slots[i] = msg->regs[i];
	}

	if (slow_slots) {
		struct ext_registers* ext = get_register_page(smp_processor_id());
		for (unsigned i = 0; i < slow_slots; ++i) {
			pckt->slots[i + FIPC_NR_REGS] = ext->regs[i];
		}
	}

}

void fipc_pack_impl(struct fipc_message* fmsg, enum dispatch_id rpc, struct rpc_message* msg) {
	unsigned slots_used = msg->end_slot - msg->slots;
	fmsg->vmfunc_id = VMFUNC_RPC_CALL;
	fmsg->rpc_id = rpc | (slots_used << 16);

	unsigned fast_slots = min(slots_used, (unsigned)(FIPC_NR_REGS));
	unsigned slow_slots = (slots_used > FIPC_NR_REGS) ? slots_used - FIPC_NR_REGS : 0;

	for (unsigned i = 0; i < fast_slots; ++i) {
		fmsg->regs[i] = msg->slots[i];
	}

	if (slow_slots) {
		struct ext_registers* ext = get_register_page(smp_processor_id());
		for (unsigned i = 0; i < slow_slots; ++i) {
			ext->regs[i] = msg->slots[i + FIPC_NR_REGS];
		}
	}

}

void fipc_send_to_klcd_impl(enum dispatch_id rpc, struct rpc_message* msg) {
	struct fipc_message fmsg;
	fipc_pack(&fmsg, rpc, msg);
	vmfunc_wrapper(&fmsg);
	fipc_translate(&fmsg, &rpc, msg);
}

void fipc_send_to_lcd_impl(enum dispatch_id rpc, struct rpc_message* msg) {
	struct fipc_message fmsg;
	fipc_pack(&fmsg, rpc, msg);
	vmfunc_klcd_wrapper(&fmsg, OTHER_DOMAIN);
	fipc_translate(&fmsg, &rpc, msg);
}

