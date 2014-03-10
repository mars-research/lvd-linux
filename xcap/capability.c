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

void lcd_initialize_freelist(struct cte *cnode, bool bFirstCNode)
{
  int startid = 1;
  int i;
  
  if (cnode == NULL)
    return;
  
  if (bFirstCNode)
  {
    startid = LCD_CapFirstFreeSlot;
  }
  
  cnode[0].ctetype = lcd_type_free;
  cnode[0].slot.next_free_cap_slot = startid;
  cnode[startid].ctetype = lcd_type_free;
  
  for (i = startid; i < CNODE_SLOTS_START; i++)
  {
    cnode[i].slot.next_free_cap_slot = i + 1;
    cnode[i+1].ctetype = lcd_type_free;
  }
  cnode[i].slot.next_free_cap_slot = 0;
  
  startid = CNODE_SLOTS_START;
  cnode[0].slot.next_free_cnode_slot = startid;
  cnode[startid].ctetype = lcd_type_free;
  for (i = CNODE_SLOTS_START; i < MAX_SLOTS - 1; i++)
  {
    cnode[i].slot.next_free_cnode_slot = i + 1;
    cnode[i+1].ctetype = lcd_type_free;
  }
  cnode[i].slot.next_free_cnode_slot = 0;
}

struct cte * lcd_reserve_cap_slot(struct cte *cnode, cap_id *cid, int free_slot)
{
  struct cte *node = cnode->cnode.table;
  ASSERT(node[free_slot].ctetype == lcd_type_free, "Free List is corrupted\n");
  // a valid empty slot
  node[0].slot.next_free_cap_slot = node[free_slot].slot.next_free_cap_slot;
  lcd_set_level_bits(cnode, cid, free_slot);
  return &node[free_slot];
}

// caller responsible for locking cspace.
// free slot will be booked for the caller.
// ie. it will not longer be free once this function returns it.
// naming convention:
// cnode = struct cte entry in the table, which points to another table.
// node = pointer the the array of struct cte entries/capability table i.e. cnode->cnode.table
cap_id lcd_lookup_free_slot(struct cap_space *cspace, struct cte **cap)
{
  cap_id cid = 0;
  bool found = false;
  int i = 0;
  struct kfifo cnode_q;
  
  if (cspace == NULL || cspace->root_cnode.cnode.table == NULL || cap == NULL)
    return 0;
  
  if (kfifo_alloc(&cnode_q, sizeof(struct cte) * 512, GFP_KERNEL) != 0)
  {
    cid = 0;
    goto free_kfifo;
  }

  kfifo_in(&cnode_q, &(cspace->root_cnode), 1);
  
  while (!found && !kfifo_is_empty(&cnode_q))
  {
    int free_cap_slot = 0, free_cnode_slot = 0;
    struct cte *node = NULL, *cnode = NULL;
    kfifo_out(&cnode_q, cnode, 1);
  
    if (cnode == NULL)
    {
      cid = 0;
      goto free_kfifo;
    }
    node = cnode->cnode.table;
    
    free_cap_slot = node[0].slot.next_free_cap_slot;
    free_cnode_slot = node[0].slot.next_free_cnode_slot;
    
    if (free_cap_slot != 0 && free_cap_slot < CNODE_SLOTS_START)
    {
      *cap = lcd_reserve_cap_slot(cnode, &cid, free_cap_slot);
      goto free_kfifo;
    }
    else if (free_cnode_slot != 0 && free_cnode_slot >= CNODE_SLOTS_START && free_cnode_slot < MAX_SLOTS)
    {
      // there is no slot free for capability
      // 1. Check if free slots are available at next level
      for (i = CNODE_SLOTS_START; i < free_cap_slot; i++)
      {
        if (node[i].ctetype == lcd_type_cnode)
        {
          struct cte * next_cnode = &node[i];
          struct cte *next_node = node[i].cnode.table;
          free_cap_slot = next_node[0].slot.next_free_cap_slot;
          if (free_cap_slot != 0 && free_cap_slot < CNODE_SLOTS_START)
          {
            *cap = lcd_reserve_cap_slot(next_cnode, &cid, free_cap_slot);
            goto free_kfifo;
          }
        }
      }
      // we will have to allocate a new cnode
      node[free_cnode_slot].cnode.table = kmalloc(PAGE_SIZE, GFP_KERNEL);
      if (node[free_cnode_slot].cnode.table == NULL)
      {
        cid = 0;
        goto free_kfifo;
      }
      node[0].slot.next_free_cnode_slot = node[free_cnode_slot].slot.next_free_cnode_slot;
      node[free_cnode_slot].ctetype = lcd_type_cnode;
      lcd_initialize_freelist(node[free_cnode_slot].cnode.table, false);
      node[free_cnode_slot].cnode.table_level = cnode->cnode.table_level + 1;
      lcd_set_level_bits(cnode, &cid, free_cnode_slot);
      node[free_cnode_slot].cnode.cnode_id = cid;
      
      cnode = &node[free_cnode_slot];
      node = node[free_cnode_slot].cnode.table;
      free_cap_slot = node[0].slot.next_free_cap_slot;
      if (free_cap_slot != 0 && free_cap_slot < CNODE_SLOTS_START)
      {
        *cap = lcd_reserve_cap_slot(cnode, &cid, free_cap_slot);
        goto free_kfifo;
      }
      cid = 0;
      goto free_kfifo;
    }
    else 
    {
      // nothing is free in this cnode 
      // kep lookin at all its children
      for (i = CNODE_SLOTS_START; i < MAX_SLOTS; i++)
      {
        kfifo_in(&cnode_q, &node[i], 1);
      }
    }
  }
free_kfifo:
  kfifo_free(&cnode_q);
  return cid;
}

struct cte * lcd_lookup_capability(struct cap_space *cspace, cap_id cid)
{
  struct cte *cap = NULL, *cnode = NULL;
  cap_id id = cid;
  int index = 0;
  int mask = (~0);
  mask = mask << (CNODE_INDEX_BITS);
  mask = ~mask;
  
  // check if input is valid
  if (cspace == NULL || cid == 0 || cspace->root_cnode.cnode.table == NULL)
    return NULL;
  cnode = cspace->root_cnode.cnode.table;
  
  while (id > 0)
  {
    index = (int)(id) & mask;
    id = id >> (CNODE_INDEX_BITS);
    if (cnode[index].ctetype == lcd_type_capability && id == 0)
    {
      cap = &cnode[index];
      break;
    }
    else if (cnode[index].ctetype == lcd_type_cnode && id != 0)
    {
      cnode = cnode[index].cnode.table;
    }
    else
    {
      break;
    }
  }
  
  return cap;
}


struct cap_space * lcd_create_cspace()
{
  struct cap_space *cspace = vmalloc(sizeof(struct cap_space));
  SAFE_EXIT_IF_ALLOC_FAILED(cspace, alloc_failure);
  
  // initialize semaphore
  sema_init(&(cspace->sem_cspace), 1);
  
  // allocate memory for the first cnode.
  cspace->root_cnode.ctetype = lcd_type_cnode;
  cspace->root_cnode.cnode.cnode_id = 0;
  cspace->root_cnode.cnode.table_level = 0;
  cspace->root_cnode.cnode.table = kmalloc(PAGE_SIZE, GFP_KERNEL);
  SAFE_EXIT_IF_ALLOC_FAILED(cspace->root_cnode.cnode.table, alloc_failure);
  // initialize the free list
  lcd_initialize_freelist(cspace->root_cnode.cnode.table, true);

  goto success;
  
alloc_failure:
  if (cspace)
  {
    if (cspace->root_cnode.cnode.table)
    {
      kfree(cspace->root_cnode.cnode.table);
      cspace->root_cnode.cnode.table = NULL;
    }
    vfree(cspace);
    cspace = NULL;
  }
  
success:
  return cspace;
}


cap_id lcd_create_cap(void * ptcb, void * hobject, lcd_cap_rights crights)
{
  struct task_struct *tcb = ptcb;
  struct cap_space *cspace;
  struct cte *cap;
  cap_id cid;
  
  if (ptcb == NULL)
    return 0;
  cspace = tcb->cspace;
  
  if (cspace == NULL || cspace->root_cnode.cnode.table == NULL)
    return 0;
  
  down_interruptible(&cspace->sem_cspace);
  cid = lcd_lookup_free_slot(cspace, &cap);
  if (cid == 0)
  {
    up(&cspace->sem_cspace);
    return 0;
  }
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
    // Lock source cspace. 
    if (down_trylock(stcb->cspace->sem_cspace) == 0)
    {
        //  if (down_trylock() == 0)  lock dst cspace 
        if (down_trylock(dtcb->cspace->sem_cspace) == 0)
        {
            // Lookup the source TCB and get a pointer to capability.
            src_cte = lcd_lookup_capability(stcb->cspace, src_cid);
            // get a free slot in destination.
            cid = lcd_lookup_free_slot(dtcb->cspace, &dst_cte);
            if (cid != 0 && src_cte != NULL && dst_cte != NULL)
            {
              struct cap_derivation_list *cdt_node;
              // add the capability to destination.
              dst_cte->ctetype = lcd_type_capability;
              dst_cte->cap.crights = crights;
              dst_cte->cap.hobject = src_cte->cap.hobject;
              dst_cte->cap.cdt_list = NULL;
              
              // update the CDT of source
              cdt_node = kmalloc(sizeof(struct cap_derivation_list), GFP_KERNEL);
              if (cdt_node != NULL)
              {
                cdt_node->next = src_cte->cap.cdt_list;
                src_cte->cap.cdt_list = cdt_node;
                cdt_node->remote_cid = cid;
                cdt_node->remote_TCB = dst_tcb;
              }
              else
              {
                ASSERT(false, "cdt_node allocation failed");
              }
              done = true;
            }
            else
            {
              ASSERT(false, "Source capability not found or no free slot in destination");
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

uint32_t lcd_get_cap_rights(void * ptcb, cap_id cid, lcd_cap_rights *rights)
{
  struct task_struct *tcb = ptcb;
  struct cte *cap;
  if (tcb == NULL || tcb->cspace == NULL || cid == 0 || rights == NULL)
    return -1;
  down_interruptible(tcb->cspace->sem_cspace);
  cap = lcd_lookup_capability(tcb->cspace, cid);
  if (cap == NULL || cap->ctetype != lcd_type_capability)
  {
    up(tcb->cspace->sem_cspace);
    return -1;
  }
  *rights = cap->cap.crights;
  up(tcb->cspace->sem_cspace);
  return 0;
}