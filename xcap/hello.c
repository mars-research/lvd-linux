
// Defining __KERNEL__ and MODULE allows us to access kernel-level code not usually available to userspace programs.
#undef __KERNEL__
#define __KERNEL__

#undef MODULE
#define MODULE

// Linux Kernel/LKM headers: module.h is needed by all modules and kernel.h is needed for KERN_INFO.
#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>        // included for __init and __exit macros
#include <linux/slab.h>        // included for kmalloc
#include <linux/kthread.h>
#include <linux/sched.h>
#include "cap.h"

struct task_struct *capTask;

static void __exit hello_cleanup(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

static int LCD_Create_CSpace(void)
{
	char *cspace;
	cspace = (char*) kmalloc(100, GFP_KERNEL);
	printk(KERN_INFO "CSpace is created\n");
	return 0;
}

static int LCD_Create_Object(objType_t type)
{
	return 0;
}

int thread_fn(void* abc)
{
	LCD_Create_CSpace();
	LCD_Create_Object(TYPE_ENDPOINT);
	return 0;
}

static int __init hello_init(void)
{
	printk(KERN_INFO "Hello world!\n");
	capTask = kthread_create(thread_fn, NULL, "CapTest");
	return 0;    // Non-zero return means that the module couldn't be loaded.
}

/// Sets the specified number of low-order bits to 1
#define MASK(bits)      ((1UL << bits) - 1)


typedef uint32_t LCD_CPtr;
typedef uint32_t CapRights;
typedef uint32_t un; /* unknown */

typedef enum {ObjType_CNode, ObjType_Null} LCD_Type;


struct CapRights
{

};

typedef struct cnode
{
	int32_t bits;
	int32_t guard;
	int32_t guard_size;
	un cnode;
}CNode;

struct capability
{
	LCD_Type type;
	CapRights rights;
	union myU{
		CNode cnode;
		CNode cnode2;
	}u;
};

struct cte
{
	struct capability cap;
};

struct cte * caps_locate_slot(un cnode, size_t offset);

/*
 * cnode_cap: CNode
 * vbits: Remaining valid bits
 */

int caps_lookup_slot(struct capability *cnode_cap, LCD_CPtr cptr,
		uint8_t vbits, struct cte **ret, CapRights rights)
{
	uint8_t bits_resolved;
	LCD_CPtr cptr_guard;
	size_t offset;
	struct cte *next_slot;
	int bitsleft;
	/* Can only resolve CNode type */    
	if (cnode_cap->type != ObjType_CNode) {
		printk("caps_lookup_slot: Cap to lookup not of type CNode\n"
				"cnode_cap->type = %u\n", cnode_cap->type);
		return -1;
	}

	/* Apply rights to this CNode */
	if ((cnode_cap->rights & rights) != rights) {
		printk("caps_lookup_slot: Rights mismatch\n"
				"Passed rights = %u, cnode_cap->rights = %u\n",
				rights, cnode_cap->rights);
		return -1;
	}

	/* Number of bits resolved by this cnode (bits + guard_size) */
	bits_resolved = cnode_cap->u.cnode.bits +
		cnode_cap->u.cnode.guard_size;

	// If lookup exceeded expected depth then table is malformed
	if (bits_resolved > vbits) {
		printk("caps_lookup_slot: Lookup exceeded valid bits\n"
				"Cnode bits = %u, guard size = %u, valid bits = %u\n",
				cnode_cap->u.cnode.bits, cnode_cap->u.cnode.guard_size,
				vbits);
		return -1;
	}

	/* Guard-check (bit-mask of guard in cptr must match guard in cnode cap) */
	cptr_guard = (cptr >> (vbits - cnode_cap->u.cnode.guard_size))
		& MASK(cnode_cap->u.cnode.guard_size);

	if (cptr_guard != cnode_cap->u.cnode.guard) {
		printk("caps_lookup_slot: guard check failed\n");
		return -1;
	}

	/* Locate capability in this cnode */
	// Offset into the cnode
	offset = (cptr >> (vbits - bits_resolved)) &
		MASK(cnode_cap->u.cnode.bits);
	// The capability at the offset
	next_slot = caps_locate_slot(cnode_cap->u.cnode.cnode, offset);
	// Do not return NULL type capability
	if (next_slot->cap.type == ObjType_Null) {
		return -1;
	}

	/* Number of bits left to resolve */
	bitsleft = vbits - bits_resolved;
	// If all bits have been resolved, return the capability
	if(bitsleft == 0) {
		*ret = next_slot;
		return 0;
	}

	/* If next capability is not of type cnode, return it */
	// XXX: Is this consistent?
	if (next_slot->cap.type != ObjType_CNode) {
		*ret = next_slot;
		return 0;
	}

	/* Descend to next level */
	return caps_lookup_slot(&next_slot->cap, cptr, bitsleft, ret, rights);
}

struct cte * caps_locate_slot(un cnode, size_t offset)
{
	return NULL;
}
module_init(hello_init);
module_exit(hello_cleanup);

