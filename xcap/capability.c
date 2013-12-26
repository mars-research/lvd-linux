
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

struct cte initCte; /* TODO: Copy this cte to TCB */
static int LCD_Create_CSpace(void)
{
	/* Create CNode and put the capability to a slot in a TCB */
	static struct capability *src_cap;
	struct cte *objAddr;
	uint32_t size_bits = 4;

	printk(KERN_INFO "LCD_Create_CSpace\n");

	src_cap = &(initCte.cap);
	objAddr = (struct cte*)kmalloc((1UL << size_bits)*sizeof(struct cte), GFP_KERNEL);
	memset(src_cap, 0, sizeof(struct capability));
	src_cap->type = ObjType_CNode;
	src_cap->rights = CAPRIGHTS_ALLRIGHTS;

	if (objAddr == 0)
	{
		printk(KERN_INFO "Fail to Alloc mem for initial CNode\n");
		return 1;
	}
	memset(objAddr, 0, (1UL << size_bits)*sizeof(struct cte));

	src_cap->u.cnode.cnode = objAddr;
	src_cap->u.cnode.bits = size_bits;
	src_cap->u.cnode.guard = 0;
	src_cap->u.cnode.guard_size = 0;

	/* Make a CNode for Task */
	printk(KERN_INFO "size of cte is: %ld\n", sizeof(struct cte));
	printk(KERN_INFO "CSpace is created, InitCte is pointing: %p\n", objAddr);
	LCD_Untyped_Retype(0, ObjType_CNode, 4, 0, 0, 0, LCD_CapInitThreadCNode, 1);
	print_cnode(&initCte, 0);

	return 0;
}

int thread_fn(void* abc)
{
	/* TODO: CSpace for the first thread should be created before it runs */
	LCD_Create_CSpace();

	//LCD_Untyped_Retype(LCD_Untyped _service, LCD_Type objType, int size_bits, LCD_CNode root, int node_index, int node_depth, int node_offset, int num_objects)
	//Rights?
//	LCD_Untyped_Retype(0, ObjType_CNode, 4, LCD_CapInitThreadCNode, 0x0, 4, 0, 0);
	return 0;
}

static int __init hello_init(void)
{
	printk(KERN_INFO "Hello world!\n");
	capTask = kthread_run(thread_fn, NULL, "CapTest");
	return 0;    // Non-zero return means that the module couldn't be loaded.
}
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

int caps_lookup_slot(struct capability * cap, LCD_CPtr cptr,
		uint8_t vbits, struct cte **ret, CapRights rights);

/*
 TODO: Implement CDT
 return value: 0 if successful.
*/

int LCD_Untyped_Retype(LCD_Untyped _service, LCD_Type objType, int size_bits, LCD_CNode root, int node_index, int node_depth, int node_offset, int num_objects)
{
	int returnVal = 0;
	switch (objType)
	{
		case ObjType_CNode:
			create_CNode(_service, size_bits, root, node_index, node_depth, node_offset, num_objects);
			break;
		default:
			break;	
	}
	return returnVal;
}

LCD_CPtr create_CNode(LCD_Untyped _service, int size_bits, LCD_CNode root, int node_index, int node_depth, int node_offset, int num_objects)
{
	/* Initialize the created capability */
	static struct capability src_cap;
	struct cte *tempCte = NULL;
	struct cte *rootCte= NULL;
	struct cte *objAddr;
	memset(&src_cap, 0, sizeof(struct capability));

	src_cap.type = ObjType_CNode;
	src_cap.rights = CAPRIGHTS_ALLRIGHTS;

	objAddr = (struct cte*)kmalloc((1UL << size_bits)*sizeof(struct cte), GFP_KERNEL);
	memset(objAddr, 0, 1UL << size_bits*sizeof(struct cte));

	// Initialize type specific fields
	src_cap.u.cnode.cnode = objAddr;
	src_cap.u.cnode.bits = size_bits;
	src_cap.u.cnode.guard = 0;
	src_cap.u.cnode.guard_size = 0;

	rootCte = caps_locate_slot(initCte.cap.u.cnode.cnode, root);
	caps_lookup_slot (&rootCte->cap, node_index, node_depth, &tempCte, CAPRIGHTS_ALLRIGHTS); 
	tempCte = caps_locate_slot(tempCte, node_offset);
	memcpy(&tempCte->cap, &src_cap, sizeof(struct capability));
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

	// If vbit == 0, it's initialization
	if(vbits == 0) {
		*ret = initCte.cap.u.cnode.cnode;
		return 0;
	}

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

struct cte * caps_locate_slot(struct cte * topCte, size_t offset)
{
	return (struct cte*)(topCte+ offset);
}

void print_cnode(struct cte * myCte, int depth)
{
	struct capability * myCap;
	struct capability * iterCap;
	struct cte * iterCte;
	int i = 0;
	int size = 0;

	myCap = &myCte->cap;
	if (depth == 0)
	{
		printk(KERN_INFO "=========print_code start=========\n");
	}
	printk(KERN_INFO "Depth: %d::: Type: %d\n", depth, myCap->type);

	if (myCap->type == ObjType_CNode)
	{
		size = myCap->u.cnode.bits;
		size = (1UL<<size);
		printk(KERN_INFO "CNode size: %d\n", size);
		for (i = 0 ; i < size; i++)
		{
			iterCte = caps_locate_slot(myCap->u.cnode.cnode, i);
			iterCap = &iterCte->cap;
			printk(KERN_INFO "Depth: %d::: Type: %d\n", depth+1, iterCap->type);
		}
		/*
		printk(KERN_INFO "One more depth start\n");
		print_cnode(myCap->u.cnode.cnode, depth+1);
		printk(KERN_INFO "One more depth end\n");
		*/
	}

	if (depth == 0)
	{
		printk(KERN_INFO "==========print_code end==========\n");
	}
}

module_init(hello_init);
module_exit(hello_cleanup);

