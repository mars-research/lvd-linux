#ifndef __LCD_CAPABILITY_H__
#define __LCD_CAPABILITY_H__

#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab_def.h>
#include <linux/fs.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <asm/page.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR ("FLUX-LAB University of Utah");

#define LCD_CAPABILITY

#define SAFE_EXIT_IF_ALLOC_FAILED(ptr, label)    \
if (ptr == NULL)                            \
{                                           \
  goto label;                               \
}                                           \

typedef enum _lcd_cap_type
{
  lcd_type_free,
  lcd_type_untyped,
  lcd_type_cnode,
  lcd_type_endpoint
}lcd_cap_type;

/* caps with fixed slot potitions in the root CNode */
enum {
	LCD_CapNull                =  0, /* null cap */
	LCD_CapInitThreadTCB       =  1, /* initial thread's TCB cap */
	LCD_CapInitThreadCNode     =  2, /* initial thread's root CNode cap */
	LCD_CapInitThreadPD        =  3, /* initial thread' page directory */
	LCD_CapIRQControl          =  4, /* global IRQ controller */
	LCD_CapInitThreadIPCBuffer =  5,  /* initial thread's IPC buffer frame cap */
	LCD_CapFirstFreeSlot       =  6
}; 

typedef uint32_t lcd_cnode; 		// a pointer to the cnode
typedef uint32_t cap_id	;    		// a locally unique identifier (address within cspace)
typedef uint32_t lcd_cnode_entry;	// a pointer to an entry within a cnode
typedef uint64_t   lcd_tcb;		// a pointer/handle to the thread contrl block
typedef uint16_t   lcd_cap_rights;	// holds the rights associated with a capability.

#define CAPRIGHTS_READ          (1 << 0)
#define CAPRIGHTS_WRITE         (1 << 1)
#define CAPRIGHTS_EXECUTE       (1 << 2)
#define CAPRIGHTS_GRANT         (1 << 3)
#define CAPRIGHTS_NUM           4

#define CAPRIGHTS_ALL	         ((1 << CAPRIGHTS_NUM) - 1)
#define CAPRIGHTS_RW 		 (CAPRIGHTS_READ | CAPRIGHTS_WRITE)
#define CAPRIGHTS_RWX           (CAPRIGHTS_RW | CAPRIGHTS_EXECUTE)
#define CAPRIGHTS_NORIGHTS      0

typedef struct _cap_derivation_list
{
  void             *remote_TCB;  // reference to the thread which was granted this capability
  cap_id           remote_cid;   // address in the remote threads capability space where this 
                                 // capability is stored.
  struct _cap_derivation_list *next;
}cap_derivation_list;

typedef struct _capability
{
  cap_id     cid;    // locally unique to each thread
}capability;

typedef struct _capability_internal 
{
	void          *hobject;      // a pointer to a kernel object
	struct cap_derivation_list   *cdt_list; // list of domain ids to whom this capability is granted
	lcd_cap_rights crights;      // specifies the rights the domain has over this capability
}capability_internal;

struct _cap_node;

typedef struct _cte // capability table entry
{
    lcd_cap_type ctetype;
    union{
          struct _cap_node *cnode;
          capability_internal *cap;
          int next_free_slot;
    };
}cte;

typedef struct _cap_node
{
	struct _guard
	{
	  int32_t  guard_bits:27; // actual guard bits
	  int32_t  guard_size:5;  // number of valid bits in guard_bits
	}guard;
    int max_slots;
	cte *cap_entry; /* may point to another cnode or to a capability */
}cap_node;



typedef struct _cap_space
{
	struct cte *root_cnode;
    struct semaphore sem_cspace;
}cap_space;


/* Helper Functions */

// initializes the free slots available in the cnode structure.
void initialize_freelist(cap_node *cnode, int size, bool bFirstCNode=false);

// will be used to search for the cnode which has a free slot available.
// if no such cnode exists will make a call to create lcd_create_cnode to create an
// empty cnode.
cap_id lcd_lookup_free_slot();

// will be used to allocate memory for a cnode.
cap_node * lcd_create_cnode(uint16_t size);

// will be used to de-allocate memory of a cnode.
uint32_t lcd_delete_cnode(cap_id cid);



/* External Interface */

// creates a cspace for a thread. should be called during lcd creation.
// initially the cspace contains just three capabilities:
// one for its TCB
// one for its cspace
// one for the endpoint.
// the returned value goes into the TCB of the caller.
cap_space * lcd_create_cspace();

// creates a new capability, inserts it into cspace of caller and 
// returns the capability identifier.
// it is unclear who will have the right to perform this operation.
cap_id lcd_create_cap(void * ptcb, void * hobject, lcd_cap_rights crights);

// will be used to grant a capability to another thread
// returns the address of the capability within the cspace of the receiver thread.
// a logical AND of the crights and rights on the capability being granted will decide the 
// final rights the granted capability will have.
cap_id lcd_cap_grant(void * dst_tcb, cap_id src_cid, lcd_cap_rights crights);

// will be called to delete a particular capability in the calling threads 
// cspace. threads have right to delete capabilities in their own cspace.
uint32_t lcd_cap_delete(void * ptcb, cap_id cid);

// will be called to delete the capability and all its children.
// the children can be present in cspace belonging to different threads.
// as such the thread owning the parent capability has a right to delete 
// a capability which is its child or was derieved from it.
uint32_t lcd_cap_revoke(void * ptcb, cap_id cid);

// will be used to get the rights available with a capability.
uint32_t lcd_get_cap_rights(void * ptcb, cap_id cid, lcd_cap_rights *rights);

#endif // __LCD_CAPABILITY_H__