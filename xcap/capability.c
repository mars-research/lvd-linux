#undef __KERNEL__
#define __KERNEL__

#undef MODULE
#define MODULE

#include "capability.h"

int init_module(void)
{
	printk(KERN_INFO "Initializing LCD Module\n");
	return 0;    // Non-zero return means that the module couldn't be loaded.
}

void cleanup_module(void)
{
	printk(KERN_INFO "Cleaning up LCD Module\n");
}

void lcd_initialize_freelist(struct cte *cnode, int size, bool bFirstCNode)
{
  int startid = 1;
  int i = startid;
  
  if (cnode == NULL)
    return;
  
  if (bFirstCNode)
  {
    startid = LCD_CapFirstFreeSlot;
  }
  
  cnode[0].ctetype = lcd_type_free;
  cnode[0].next_free_slot = startid;
  cnode[startid].ctetype = lcd_type_free;
  i = startid;
  for (; i < MAX_SLOTS - 1; i++)
  {
    cnode[i].next_free_slot = i + 1;
    cnode[i+1].ctetype = lcd_type_free;
  }
  cnode[i].next_free_slot = 0;
}

// caller responsible for locking cspace.
// free slot will be booked for the caller.
// ie. it will not longer be free once this function returns it.
cap_id lcd_lookup_free_slot(struct cap_space *cspace, struct cte **cap)
{
  cap_id cid = 0;
  struct cte *cnode;
  bool found = false;
  
  if (cspace == NULL)
    return 0;
  cnode = cspace->root_cnode;
  
    while (!found)
  {
      int nxt = cnode[0].next_free_slot;
      // check if only one free slot is available
      if (cnode[nxt].next_free_slot == 0)
      {
        // only one free slot is available, allocate a new cnode and set free_cnode
         
      }
  }
  return cid;
}

struct cte * lcd_lookup_capability(struct cap_space *cspace, cap_id cid)
{
  struct cte *cap;
  return cap;
}


struct cap_space * lcd_create_cspace()
{
  struct cap_space *cspace = vmalloc(sizeof(struct cap_space));
  SAFE_EXIT_IF_ALLOC_FAILED(cspace, alloc_failure);
  
  // initialize semaphore
  sema_init(&(cspace->sem_cspace), 1);
  
  // allocate memory for the first cnode.
  cspace->root_cnode = kmalloc(PAGE_SIZE, GFP_KERNEL);
  SAFE_EXIT_IF_ALLOC_FAILED(cspace->root_cnode, alloc_failure);
  
  // initialize the free list
  lcd_initialize_freelist(cspace->root_cnode, PAGE_SIZE, true);

  goto success;
  
alloc_failure:
  if (cspace)
  {
    if (cspace->root_cnode)
    {
      kfree(cspace->root_cnode);
      cspace->root_cnode = NULL;
    }
    vfree(cspace);
    cspace = NULL;
  }
  
success:
  return cspace;
}


cap_id lcd_create_cap(void * ptcb, void * hobject, lcd_cap_rights crights)
{
  struct task_struct *tcb = (struct task_struct *)ptcb;
  struct cap_space *cspace;
  struct cte *cap;
  cap_id cid;
  
  if (ptcb == NULL)
    return 0;
  cspace = tcb->cspace;
  
  if (cspace == NULL || cspace->root_cnode == NULL)
    return 0;
  
  down_interruptible(&cspace->sem_cspace);
  cid = lcd_lookup_free_slot(cspace, &cap);
  cap->ctetype = lcd_type_capability;  
  cap->cap.crights = crights;
  cap->cap.hobject = hobject;
  cap->cap.cdt_list = NULL;
  up(&(cspace->sem_cspace));
  return cid;
}

cap_id lcd_cap_grant(void *src_tcb, cap_id src_cid, void * dst_tcb, lcd_cap_rights crights)
{
  cap_id cid = 0;
  struct task_struct *stcb, *dtcb;
  struct cte *src_cte = NULL, *dst_cte = NULL;
  bool done = false;
  
  if (src_tcb == NULL || dst_tcb == NULL || src_cid <= 0)
    return 0;
  
  stcb = (struct task_struct *)src_tcb;
  dtcb = (struct task_struct *)dst_tcb;
  
  while (!done)
  {
    // if (down_trylock() == 0) Lock source cspace. 
    if (down_trylock(stcb->cspace->sem_cspace) == 0)
    {
        //  if (down_trylock() == 0)  lock dst cspace 
        if (down_trylock(dtcb->cspace->sem_cspace) == 0)
        {
            // Lookup the source TCB and get a pointer to capability.
            src_cte = lcd_lookup_capability(stcb->cspace, src_cid);
            // get a free slot in destination.
            cid = lcd_lookup_free_slot(dtcb->cspace, &dst_cte);
            if (cid != 0){
              struct cap_derivation_list *cdt_node;
              // add the capability to destination.
              dst_cte->ctetype = lcd_type_capability;
              dst_cte->cap.crights = crights;
              dst_cte->cap.hobject = src_cte->cap.hobject;
              dst_cte->cap.cdt_list = NULL;
              
              // update the CDT of source
              cdt_node = kmalloc(sizeof(struct cap_derivation_list), GFP_KERNEL);
              if (cdt_node != NULL){
                cdt_node->next = src_cte->cap.cdt_list;
                src_cte->cap.cdt_list = cdt_node;
                cdt_node->remote_cid = src_cid;
                cdt_node->remote_TCB = src_tcb;
              }
              else{
                ASSERT(false, "cdt_node allocation failed");
              }
              done = true;
            }
            // release lock on dst cspace.
            up(dtcb->cspace->sem_cspace);
        }
        // release lock on source cspace.
        up(stcb->cspace->sem_cspace);
    }
    if (!done)
      msleep_interruptible(1);
  }
  return cid;
}