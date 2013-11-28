
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
#define OBJBITS_CTE 7

typedef uint32_t LCD_CPtr;
typedef uint32_t CapRights;
typedef uint32_t un; /* unknown */

typedef enum {ObjType_CNode, ObjType_Null} LCD_Type;

#define CAPRIGHTS_READ          (1 << 0)
#define CAPRIGHTS_WRITE         (1 << 1)
#define CAPRIGHTS_EXECUTE       (1 << 2)
#define CAPRIGHTS_GRANT         (1 << 3)
#define CAPRIGHTS_IDENTIFY      (1 << 4)
#define CAPRIGHTS_NUM           5

#define CAPRIGHTS_ALLRIGHTS     ((1 << CAPRIGHTS_NUM) - 1)
#define CAPRIGHTS_READ_WRITE    (CAPRIGHTS_READ | CAPRIGHTS_WRITE)
#define CAPRIGHTS_NORIGHTS      0

struct CapRights
{

};


typedef struct endpoint
{
	int a;
	int b;
} EndPoint;

typedef struct cnode
{
	int32_t bits;
	int32_t guard;
	int32_t guard_size;
	uint32_t* cnode;
}CNode;

union capability_u{
	CNode cnode;
	EndPoint endpoint;
	};

struct capability
{
	LCD_Type type;
	CapRights rights;
	union capability_u u;
};

struct cte
{
	struct capability cap;
};

struct cte * caps_locate_slot(uint32_t* cnode, size_t offset);

/**
 * \brief Create capabilities to kernel objects.
 *
 * This function creates kernel objects of 'type' into the memory
 * area, based at 'addr' and of size 2^'bits', so they completely fill the
 * area. For each created kernel object, a capability is created to it and
 * put consecutively into the array of CTEs pointed to by 'caps'. The array
 * needs to have the appropriate size to hold all created caps. Some kernel
 * objects can have a variable size. In that case, 'objbits' should be non-zero
 * and give the a size multiplier as 2^'objbits'.
 *
 * \param bits          Size of memory area as 2^bits.
 * \param dest_caps     Pointer to array of CTEs to hold created caps.
 *
 * \return Error code

 GFP_KERNEL
 seL4_Untyped_Retype(
 untyped_item,
 item_type, item_size,
 seL4_CapInitThreadCNode,
 allocator->root_cnode, allocator->root_cnode_depth,
 allocator->cslots.first + allocator->num_slots_used,
 num_items);
 */


LCD_CPtr create_CNode(uint32_t objAddr, int size_bits, LCD_CPtr root, int node_index, int node_depth, int node_offset, int num_objects);

int caps_lookup_slot(struct capability *cnode_cap, LCD_CPtr cptr,
		uint8_t vbits, struct cte **ret, CapRights rights);

LCD_CPtr LCD_Untyped_Retype(uint32_t objAddr, LCD_Type objType, int size_bits, LCD_CPtr root, int node_index, int node_depth, int node_offset, int num_objects)
{
	LCD_CPtr returnAddr = 0;
	switch (objType)
	{
		case ObjType_CNode:
			returnAddr = create_CNode(objAddr, size_bits, root, node_index, node_depth, node_offset, num_objects);
			break;
		default:
			break;	
	}
	return returnAddr;
}

LCD_CPtr create_CNode(uint32_t objAddr_, int size_bits, LCD_CPtr root, int node_index, int node_depth, int node_offset, int num_objects)
{
	/* Initialize the created capability */
	static struct capability src_cap;
	static struct capability *dest_cap;
	struct cte *tempCte = NULL;
	uint32_t *objAddr;
	memset(&src_cap, 0, sizeof(struct capability));

	src_cap.type = ObjType_CNode;
	src_cap.rights = CAPRIGHTS_ALLRIGHTS;

	objAddr = kmalloc((1UL << size_bits)*sizeof(struct cte), GFP_KERNEL);
	memset(objAddr, 0, 1UL << size_bits);

	// Initialize type specific fields
	src_cap.u.cnode.cnode = objAddr;
	src_cap.u.cnode.bits = size_bits;
	src_cap.u.cnode.guard = 0;
	src_cap.u.cnode.guard_size = 0;

	caps_lookup_slot ((struct capability*)root, node_index, node_depth, &tempCte,CAPRIGHTS_ALLRIGHTS); 
	tempCte = caps_locate_slot((uint32_t *)tempCte, node_offset);
	dest_cap = &(tempCte->cap);
	memcpy(&src_cap, dest_cap, sizeof(struct capability));
	return 0;
}

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
	next_slot = caps_locate_slot((uint32_t*)cnode_cap->u.cnode.cnode, offset);
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

struct cte * caps_locate_slot(uint32_t *cnode, size_t offset)
{
	return (struct cte*)(cnode + sizeof(struct cte) * offset);
}
module_init(hello_init);
module_exit(hello_cleanup);

