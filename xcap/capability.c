
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

void initialize_freelist(cte *cnode, int size, bool bFirstCNode=false)
{
  if (cnode == NULL)
    return;
  int max_slots = (size / sizeof(cte));
  
  int startid = 1;
  if (bFirstCNode)
  {
    startid = LCD_CapFirstFreeSlot;
  }
  cnode[0].ctetype = lcd_type_free;
  cnode[0].next_free_slot = startid;
  cnode[startid].ctetype = lcd_type_free;
  
  for (int i = startid; i < max_slots-1; i++)
  {
    cnode[i].next_free_slot = i + 1;
    cnode[i+1].ctetype = lcd_type_free;
  }
}


cap_space * lcd_create_cspace()
{
  cap_space *cspace = (cap_space *)vmalloc(sizeof(cap_space));
  SAFE_EXIT_IF_ALLOC_FAILED(cspace, alloc_failure);
  
  // initialize semaphore
  sema_init(&(cspace->sem_cspace), 1);
  
  // allocate memory for the first cnode.
  cspace->root_cnode = (cte *)kmalloc(PAGE_SIZE, GFP_KERNEL);
  SAFE_EXIT_IF_ALLOC_FAILED(cspace->root_cnode, alloc_failure);
  
  // initialize the free list
  initialize_freelist(cspace->root_cnode, PAGE_SIZE, true);

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
  
  //1. lock the cspace.
  //2. walk the cspace to find empty slots.
  //   if only one empty slot left in cnode, create a new cnode, initialize its free list.
  //3. Get the first free slot, set its type to capability.
  //    set its hObject, and rights.
  //4. return the cap_id within this cspace.
  
}