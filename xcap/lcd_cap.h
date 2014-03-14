#ifndef __LCD_CAP_H__
#define __LCD_CAP_H__

#include <linux/sched.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/semaphore.h>
#include <linux/log2.h>
#include <linux/delay.h>
#include <linux/kfifo.h>
#include <asm/page.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR ("FLUX-LAB University of Utah");

typedef uint32_t lcd_cnode;         // a pointer to the cnode
typedef uint64_t cap_id ;           // a locally unique identifier (address within cspace)
typedef uint32_t lcd_cnode_entry;   // a pointer to an entry within a cnode
typedef uint64_t   lcd_tcb;     // a pointer/handle to the thread contrl block
typedef uint16_t   lcd_cap_rights;  // holds the rights associated with a capability.
typedef uint16_t   lcd_cap_type;

#define MAX_SLOTS               (PAGE_SIZE/sizeof(struct cte))
#define CNODE_SLOTS_PER_CNODE   64
#define CNODE_SLOTS_START       (MAX_SLOTS - CNODE_SLOTS_PER_CNODE)
#define CNODE_INDEX_BITS        (ilog2(MAX_SLOTS))
#define CAP_ID_SIZE             (sizeof(cap_id) * 8)
#define MAX_DEPTH               ((CAP_ID_SIZE + 1)/CNODE_INDEX_BITS)

#define SAFE_EXIT_IF_ALLOC_FAILED(ptr, label)    \
if (ptr == NULL)                            \
{                                           \
  goto label;                               \
}                                           \

#define ASSERT(condition, expr) \
        if(!(condition)) { \
                printk("\nAssertion Failed at: %d\n",__LINE__); \
                panic(#expr); \
        }

enum __lcd_cap_type
{
  lcd_type_free,
  lcd_type_capability,
  lcd_type_cnode,
  lcd_type_endpoint,
  lcd_type_separator
};

/* caps with fixed slot potitions in the root CNode */
enum {
	LCD_CapNull                =  0, /* null cap */
	LCD_CapInitThreadTCB       =  1, /* initial thread's TCB cap */
	LCD_CapInitThreadPD        =  2, /* initial thread' page directory */
	LCD_CapIRQControl          =  3, /* global IRQ controller */
	LCD_CapInitThreadIPCBuffer =  4,  /* initial thread's IPC buffer frame cap */
	LCD_CapFirstFreeSlot       =  5
}; 



#define CAPRIGHTS_READ          (1 << 0)
#define CAPRIGHTS_WRITE         (1 << 1)
#define CAPRIGHTS_EXECUTE       (1 << 2)
#define CAPRIGHTS_GRANT         (1 << 3)
#define CAPRIGHTS_NUM           4

#define CAPRIGHTS_ALL	         ((1 << CAPRIGHTS_NUM) - 1)
#define CAPRIGHTS_RW 		 (CAPRIGHTS_READ | CAPRIGHTS_WRITE)
#define CAPRIGHTS_RWX           (CAPRIGHTS_RW | CAPRIGHTS_EXECUTE)
#define CAPRIGHTS_NORIGHTS      0

struct cap_derivation_tree
{
  struct cte *cap;      
  struct cap_space *cspace;
  struct cap_derivation_tree *next;
  struct cap_derivation_tree *prev;
  struct cap_derivation_tree *parent_ptr;
  struct cap_derivation_tree *child_ptr;  
};

struct capability
{
  cap_id     cid;    // locally unique to each thread
};

struct capability_internal 
{
	void          *hobject;      // a pointer to a kernel object
	struct cap_derivation_tree   *cdt_node; // list of domain ids to whom this capability is granted
	lcd_cap_rights crights;      // specifies the rights the domain has over this capability
};

struct cte;

struct cap_node
{
    cap_id cnode_id;
    struct cte *table; /* points to another cnode table */
    uint16_t table_level;
};

struct free_slot_t
{
  int next_free_cap_slot;
  int next_free_cnode_slot;
};

struct cte // capability table entry
{
    union{
          struct cap_node cnode;
          struct capability_internal cap;
          struct free_slot_t slot;
    };
    lcd_cap_type ctetype;
    uint16_t index;
};

struct cap_space
{
	struct cte root_cnode;
    struct semaphore sem_cspace;
};


/* Helper Functions */

static inline int lcd_get_bits_at_level(cap_id id, int level)
{
  int bits = 0;
  id = id << ((MAX_DEPTH - level - 1) * CNODE_INDEX_BITS);
  id = id >> ((MAX_DEPTH - 1)         * CNODE_INDEX_BITS);
  bits = (int) id;
  return bits;
}

static inline void lcd_clear_bits_at_level(cap_id *id, int level)
{
  cap_id mask = (~0);
  // clear all higher order bits.
  mask = mask << ((MAX_DEPTH - 1) * CNODE_INDEX_BITS);
  // clear lower order bits
  mask = mask >> ((MAX_DEPTH - 1) * CNODE_INDEX_BITS);
  // get the mask to appropriate position
  mask = mask << (level * CNODE_INDEX_BITS);
  mask = ~mask;
  *id = (*id) & mask;
}

static inline void lcd_set_bits_at_level(struct cte *cnode, cap_id *cid, int free_slot)
{
  int level = cnode->cnode.table_level;
  cap_id id = free_slot;
  
  *cid = cnode->cnode.cnode_id;
  lcd_clear_bits_at_level(cid, level);
  id = id << (level * CNODE_INDEX_BITS);
  *cid = *cid | id;
}

struct cte * lcd_reserve_cap_slot(struct cte *cnode, cap_id *cid, int free_slot);

// initializes the free slots available in the cnode structure.
void lcd_initialize_freelist(struct cte *cnode, bool bFirstCNode);

// will be used to search for the cnode which has a free slot available.
// if no such cnode exists will make a call to create lcd_create_cnode to create an
// empty cnode.
cap_id lcd_lookup_free_slot(struct cap_space *cspace, struct cte **cap);

void lcd_update_cdt(void *ptcb);

uint32_t lcd_cap_delete_internal(struct cte *cap_cte);

/* External Interface */

// creates a cspace for a thread. should be called during lcd creation.
// expects the following objects as input with the rights for those objects:
// ****(LCD_CapInitThreadTCB is compulsory)
//     Objects               Index within input array   Comments
//     LCD_CapNull                0                     Should be a NULL entry in objects array                
// ****LCD_CapInitThreadTCB       1                     Pointer to task_struct of thread.
//     LCD_CapInitThreadPD        2                     Pointer to the page directory of process.
//     LCD_CapIRQControl          3                     ?? will be used later.
//     LCD_CapInitThreadIPCBuffer 4                     Pointer to the IPC buffer which will be used.
//  total number of object pointers expected is equal to LCD_CapFirstFreeSlot.
//  Put a NULL pointer for entries which the thread is not expected to have.
//  e.g. if the thread does not need an IPC buffer then objects[4] should be NULL.
// the returned value goes into the TCB of the caller.

struct cap_space * lcd_create_cspace(void *objects[], lcd_cap_rights rights[]);

struct cte * lcd_lookup_capability(struct task_struct *tcb, cap_id cid);

// creates a new capability, inserts it into cspace of caller and 
// returns the capability identifier.
// it is unclear who will have the right to perform this operation.
cap_id lcd_create_cap(void * ptcb, void * hobject, lcd_cap_rights crights);

// will be used to grant a capability to another thread
// returns the address of the capability within the cspace of the receiver thread.
// a logical AND of the crights and rights on the capability being granted will decide the 
// final rights the granted capability will have.
cap_id lcd_cap_grant(void *src_tcb, cap_id src_cid, void * dst_tcb, lcd_cap_rights crights);

// will be called to delete a particular capability in the calling threads 
// cspace. threads have right to delete capabilities in their own cspace.
uint32_t lcd_cap_delete(void * ptcb, cap_id cid);

// will be called to delete the capability and all its children.
// the children can be present in cspace belonging to different threads.
// as such the thread owning the parent capability has a right to delete 
// a capability which is its child or was derieved from it.
uint32_t lcd_cap_revoke(void * ptcb, cap_id cid);

// should be called when the thread exits.
// this is extremely heavy function which updates the CDT for all capabilities present
// in the cspace of the exiting thread.
void lcd_destroy_cspace(void *ptcb);

// will be used to get the rights available with a capability.
uint32_t lcd_get_cap_rights(void * ptcb, cap_id cid, lcd_cap_rights *rights);

#endif // __LCD_CAP_H__