
#undef __KERNEL__
#define __KERNEL__

#undef MODULE
#define MODULE

#include <linux/module.h>    	// included for all kernel modules
#include <linux/kernel.h>    	// included for KERN_INFO
#include <linux/init.h>        	// included for __init and __exit macros
#include <linux/slab.h>        	// included for kmalloc
#include <linux/kthread.h>
#include <linux/sched.h>
#include "cap.h"

struct task_struct *capTask;
struct cte initCte; /* TODO: Copy this cte to TCB */

///////////////////////// Test Program ///////////////////////////////////////////////////////////////////////////////
int thread_fn(void* abc)
{
	/* TODO: CSpace for the first thread should be created before it runs */
	LCD_Create_CSpace();

	/* CSpace test cases */
	/* Addr is 0x0 */
	LCD_Untyped_Retype(0, ObjType_EndPoint, 0, LCD_CapInitThreadCNode, 0, 0, 0, 0);
	print_cnode(&initCte, 0);

	/* Addr is 0x400000000 */
	LCD_Untyped_Retype(0, ObjType_CNode, 2, LCD_CapInitThreadCNode, 0, 0, 0x2, 0); 
	print_cnode(&initCte, 0);

	/* Addr is 0x480000000 */
	LCD_Untyped_Retype(0, ObjType_EndPoint, 0, LCD_CapInitThreadCNode, 0x40000000, 3, 0x1, 0);
	print_cnode(&initCte, 0);

	LCD_CNode_Search(LCD_CapInitThreadCNode, 0x40000000, 3);

	LCD_CNode_Delete(LCD_CapInitThreadCNode, 0x48000000, 5);
	print_cnode(&initCte, 0);
	LCD_CNode_Delete(LCD_CapInitThreadCNode, 0x40000000, 3);
	print_cnode(&initCte, 0);
	LCD_CNode_Delete(LCD_CapInitThreadCNode, 0x00000000, 3);
	print_cnode(&initCte, 0);
	return 0;
}

static int __init hello_init(void)
{
	printk(KERN_INFO "Hello world!\n");
	capTask = kthread_run(thread_fn, NULL, "CapTest");
	return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

//////////////////////////External Interfaces////////////////////////////////////////////////////////////////////////////////
static int LCD_Create_CSpace(void)
{
	/* Create CNode and put the capability to a slot in a TCB */
	static struct capability *src_cap;
	struct cte *threadInitCte= NULL;
	uint32_t size_bits = 4;

	printk(KERN_INFO "LCD_Create_CSpace\n");
	/* Create CNode for internal use*/
	src_cap = &(initCte.cap);
	cnode_create_internal(src_cap, size_bits);

	/* Create CNode for user */
	threadInitCte= caps_locate_slot(initCte.cap.u.cnode.cnode, LCD_CapInitThreadCNode);
	src_cap = &(threadInitCte->cap);
	size_bits= 3;
	cnode_create_internal(src_cap, size_bits);

	printk(KERN_INFO "CSpace is created\n");
	print_cnode(&initCte, 0);
	return 0;
}

int LCD_Untyped_Retype(LCD_Untyped _service, LCD_Type objType, int size_bits, LCD_CNode root, int node_index, int node_depth, int node_offset, int num_objects)
{
	int returnVal = 0;
	switch (objType)
	{
		case ObjType_CNode:
			create_CNode(_service, size_bits, root, node_index, node_depth, node_offset, num_objects);
			break;
		case ObjType_EndPoint:
			create_EndPoint(_service, size_bits, root, node_index, node_depth, node_offset, num_objects);
			break;
		default:
			break;	
	}
	return returnVal;
}

static void *LCD_CNode_Search(LCD_CNode _service, int node_index, int node_depth)
{
	struct cte *tempCte = NULL;
	struct cte *rootCte= NULL;
	
	rootCte = caps_locate_slot(initCte.cap.u.cnode.cnode, _service);
	caps_lookup_slot(&rootCte->cap, node_index, node_depth, 0, &tempCte, CAPRIGHTS_ALLRIGHTS); 

	/* free alloced memory */
	switch(tempCte->cap.type)
	{
		case ObjType_CNode:
			return (void*)tempCte->cap.u.cnode.cnode;
			break;
		case ObjType_EndPoint:
			return NULL;
			break;
		default:
			return NULL;
			break;
	}

	return NULL;
}

static int LCD_CNode_Delete(LCD_CNode _service, int node_index, int node_depth)
{
	struct cte *tempCte = NULL;
	struct cte *rootCte= NULL;
	
	rootCte = caps_locate_slot(initCte.cap.u.cnode.cnode, _service);
	caps_lookup_slot(&rootCte->cap, node_index, node_depth, 0, &tempCte, CAPRIGHTS_ALLRIGHTS); 

	/* free alloced memory */
	switch(tempCte->cap.type)
	{
		case ObjType_CNode:
			kfree(tempCte->cap.u.cnode.cnode);
			break;
		case ObjType_EndPoint:
			break;
		default:
			break;
	}

	memset(tempCte, 0, sizeof(struct cte));
	return 0;
}

////////////////////////Internal Functions///////////////////////////////////////////////////////////////////////////////////////////////

int cnode_create_internal(struct capability *src_cap, int size_bits)
{
	struct cte *objAddr;
	objAddr = (struct cte*)kmalloc((1UL << size_bits)*sizeof(struct cte), GFP_KERNEL);
	if (objAddr == 0)
	{
		printk(KERN_INFO "Fail to Alloc mem for initial CNode\n");
		return 1;
	}
	memset(objAddr, 0, (1UL << size_bits)*sizeof(struct cte));
	memset(src_cap, 0, sizeof(struct capability));
	src_cap->type = ObjType_CNode;
	src_cap->rights = CAPRIGHTS_ALLRIGHTS;
	src_cap->u.cnode.cnode = objAddr;
	src_cap->u.cnode.bits = size_bits;
	src_cap->u.cnode.guard = 0;
	src_cap->u.cnode.guard_size = 0;
	return 0;
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
	memset(objAddr, 0, (1UL << size_bits)*sizeof(struct cte));

	// Initialize type specific fields
	src_cap.u.cnode.cnode = objAddr;
	src_cap.u.cnode.bits = size_bits;
	src_cap.u.cnode.guard = 0;
	src_cap.u.cnode.guard_size = 0;

	/* tempCte is pointing cnode*/
	rootCte = caps_locate_slot(initCte.cap.u.cnode.cnode, root);
	caps_lookup_slot (&rootCte->cap, node_index, node_depth, 0, &tempCte, CAPRIGHTS_ALLRIGHTS); 

	/* tempCte is pointing the first element in the cnode*/
	tempCte = tempCte->cap.u.cnode.cnode; 

	/* tempCte is pointing the right element in the cnode*/
	tempCte = caps_locate_slot(tempCte, node_offset);

	memcpy(&tempCte->cap, &src_cap, sizeof(struct capability));

	return 0;
}

LCD_CPtr create_EndPoint(LCD_Untyped _service, int size_bits, LCD_CNode root, int node_index, int node_depth, int node_offset, int num_objects)
{
	/* Initialize the created capability */
	static struct capability src_cap;
	struct cte *tempCte = NULL;
	struct cte *rootCte= NULL;
	struct cte *objAddr;
	memset(&src_cap, 0, sizeof(struct capability));

	src_cap.type = ObjType_EndPoint;
	src_cap.rights = CAPRIGHTS_ALLRIGHTS;

	objAddr = (struct cte*)kmalloc(100, GFP_KERNEL);
	memset(objAddr, 0, 100);

	/* tempCte is pointing cnode*/
	rootCte = caps_locate_slot(initCte.cap.u.cnode.cnode, root);
	caps_lookup_slot (&rootCte->cap, node_index, node_depth, 0, &tempCte, CAPRIGHTS_ALLRIGHTS); 

	/* tempCte is pointing the first element in the cnode*/
	tempCte = tempCte->cap.u.cnode.cnode; 

	/* tempCte is pointing the right element in the cnode*/
	tempCte = caps_locate_slot(tempCte, node_offset);

	memcpy(&tempCte->cap, &src_cap, sizeof(struct capability));
	return 0;
}

int caps_lookup_slot(struct capability *cnode_cap, LCD_CPtr cptr, uint8_t vbits, uint8_t resolved_bits, struct cte **ret, CapRights rights)
{
	uint8_t bits_resolved;
	LCD_CPtr cptr_guard;
	size_t offset;
	struct cte *next_slot;
	int bitsleft;

	// If vbit == 0, return root
	if(vbits == 0) {
		*ret = caps_locate_slot(initCte.cap.u.cnode.cnode, LCD_CapInitThreadCNode);
		return 0;
	}

	printk("caps_lookup_slot: Cnode bits = %u, guard size = %u, valid bits = %u\n",
				cnode_cap->u.cnode.bits, cnode_cap->u.cnode.guard_size,
				vbits);
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
	offset = (cptr >> (32 - resolved_bits - bits_resolved)) &
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
	return caps_lookup_slot(&next_slot->cap, cptr, bitsleft, bits_resolved+resolved_bits, ret, rights);
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
	int j = 0;
	int size = 0;

	myCap = &myCte->cap;
	if (depth == 0)
	{
		printk(KERN_INFO "=========print_code start=========\n");
	}

	if (myCap->type == ObjType_CNode)
	{
		size = myCap->u.cnode.bits;
		size = (1UL<<size);
		for (j = 0; j < depth; j++)
			printk("  ");
		printk("<CNode size: %d>\n", size);
		for (i = 0 ; i < size; i++)
		{
			iterCte = caps_locate_slot(myCap->u.cnode.cnode, i);
			iterCap = &iterCte->cap;
			for (j = 0; j < depth; j++)
				printk("  ");
			printk("Depth: %d::: Type: %d\n", depth+1, iterCap->type);
			if (iterCap->type == ObjType_CNode)
			{
				print_cnode(iterCte, depth+1);
			}
		}
	}

	if (depth == 0)
	{
		printk(KERN_INFO "==========print_code end==========\n");
	}
}

module_init(hello_init);
module_exit(hello_cleanup);

