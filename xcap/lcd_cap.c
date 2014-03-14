#undef __KERNEL__
#define __KERNEL__

#undef MODULE
#define MODULE

#include "lcd_cap.h"
#include <net/irda/parameters.h>

int init_module(void)
{
	printk(KERN_INFO "Initializing LCD Module\n");
	return 0;    // Non-zero return means that the module couldn't be loaded.
}

void cleanup_module(void)
{
	printk(KERN_INFO "Cleaning up LCD Module\n");
}

// does not lock the cspace.
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
  cnode[0].index = 0;
  
  for (i = startid; i < CNODE_SLOTS_START - 1; i++)
  {
    cnode[i].slot.next_free_cap_slot = i + 1;
    cnode[i+1].ctetype = lcd_type_free;
    cnode[i].index = i;
  }
  cnode[i].slot.next_free_cap_slot = 0;
  cnode[i].index = i;
  
  startid = CNODE_SLOTS_START;
  cnode[0].slot.next_free_cnode_slot = startid;
  cnode[startid].ctetype = lcd_type_free;
  for (i = CNODE_SLOTS_START; i < MAX_SLOTS - 1; i++)
  {
    cnode[i].slot.next_free_cnode_slot = i + 1;
    cnode[i].index = i;
    cnode[i+1].ctetype = lcd_type_free;
  }
  cnode[i].slot.next_free_cnode_slot = 0;
  cnode[i].index = i;
}

struct cte * lcd_reserve_cap_slot(struct cte *cnode, cap_id *cid, int free_slot)
{
  struct cte *node = cnode->cnode.table;
  ASSERT(node[free_slot].ctetype == lcd_type_free, "Free List is corrupted\n");
  // a valid empty slot
  node[0].slot.next_free_cap_slot = node[free_slot].slot.next_free_cap_slot;
  lcd_set_bits_at_level(cnode, cid, free_slot);
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
      lcd_set_bits_at_level(cnode, &cid, free_cnode_slot);
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

// does not lock the cspace caller responsbile for the same.
struct cte * lcd_lookup_capability(struct task_struct *tcb, cap_id cid)
{
  struct cte *cap = NULL, *cnode = NULL;
  struct cap_space *cspace;
  cap_id id = cid;
  int index = 0;
  int mask = (~0);
  mask = mask << (CNODE_INDEX_BITS);
  mask = ~mask;
  
  if (tcb == NULL || cid == 0)
    return NULL;
  cspace = tcb->cspace;
  // check if input is valid
  if (cspace == NULL || cspace->root_cnode.cnode.table == NULL)
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

// caller should not lock the cspace.
void lcd_update_cdt(void *ptcb)
{
  struct task_struct *tcb = ptcb;
  struct cte *cnode, *node;
  struct kfifo cnode_q;
  struct cap_space *tcb_cspace;
  bool cspace_locked;
  int i = 0;
  if (tcb == NULL)
    return;
  tcb_cspace = tcb->cspace;
  // lock the cspace.
  if (down_interruptible(&(tcb_cspace->sem_cspace)))
  {
    ASSERT(false, "lcd_update_cdt: Signal interrupted lock, CDT will be corrupted\n");
    return;
  }
  cspace_locked = true;
  cnode = &(tcb_cspace->root_cnode);
  if (cnode == NULL)
    goto safe_return;
  
  if (kfifo_alloc(&cnode_q, sizeof(struct cte) * 512, GFP_KERNEL) != 0)
  {
    ASSERT(false, "lcd_update_cdt: Failed to allcoate kfifo, CDT will be corrupted\n");
    goto safe_return;
  }
  
  // add root cnode to kfifo
  kfifo_in(&cnode_q, cnode, 1);
  while (!kfifo_is_empty(&cnode_q))
  {
    // pop a cnode.
    kfifo_out(&cnode_q, cnode, 1);
loop:
    if (!cspace_locked)
    {
      if (down_interruptible(&(tcb_cspace->sem_cspace)))
      {
        ASSERT(false, "lcd_update_cdt: Signal interrupted lock, CDT will be corrupted\n");
        kfifo_free(&cnode_q);
        return;
      }
      cspace_locked = true;
    }
    
    node = cnode->cnode.table;
    if (node == NULL || cnode->ctetype == lcd_type_free)
      continue;
    
    for (i = 1; i < CNODE_SLOTS_START; i++)
    {
      bool parent_lock_acquired = false;
      struct cap_derivation_tree *p_cdt, *cdt, *c_cdt, *next_cdt;
      if (node[i].ctetype == lcd_type_free)
        continue;
      cdt = node[i].cap.cdt_node;
      p_cdt = cdt->parent_ptr;
      c_cdt = cdt->child_ptr;
    
      while (c_cdt != NULL)
      {
        if (parent_lock_acquired || p_cdt == NULL || down_trylock(&(p_cdt->cspace->sem_cspace)) == 0)
        {
          // multiple children have the same parent so no need to reacquire lock.
          parent_lock_acquired = true;
          // lock acquired
          if (down_trylock(&(c_cdt->cspace->sem_cspace)) == 0)
          {
            // all locks are acquired
            // update parent pointer of child to point to parent of cdt.
            c_cdt->parent_ptr = p_cdt;
            if (p_cdt && p_cdt->child_ptr == cdt)
            {
              p_cdt->child_ptr = c_cdt;
            }
            cdt->child_ptr = c_cdt->next;
            up(&(c_cdt->cspace->sem_cspace));
            next_cdt = c_cdt->next;
            c_cdt->cap->cap.cdt_node = NULL;
            kfree(c_cdt);
            c_cdt = next_cdt;
            continue;
          }
          else
          {
            // failed to acquire child lock
            if (p_cdt != NULL)
              up(&(p_cdt->cspace->sem_cspace));
            parent_lock_acquired = false;
            up(&(tcb_cspace->sem_cspace));
            cspace_locked = false;  
            goto loop;
          }
        } // if (parent_lock_acquired || p_cdt == NULL ...
        else
        {
          // failure to acquire parent lock
          up(&(tcb_cspace->sem_cspace));
          cspace_locked = false;  
          goto loop;
        }
      } // while c_cdt != NULL
      if (p_cdt != NULL)
          up(&(p_cdt->cspace->sem_cspace));
      parent_lock_acquired = false;
    } // for loop for cap slot
    
    for (i = CNODE_SLOTS_START; i < MAX_SLOTS; i++)
    {
      if (node[i].ctetype != lcd_type_free)
      {
        kfifo_in(&cnode_q, &node[i], 1);
      }
    }
  } // while !kfifo_is_empty()
  
safe_return:
  up(&(tcb_cspace->sem_cspace));
  kfifo_free(&cnode_q);
  return;
}

// Does not lock cspace caller responsible for locking cspace.
// does not perform input validation, caller responsible for the same.
uint32_t lcd_cap_delete_internal(struct cte *cap_cte)
{
  struct cte *table;
  struct cap_derivation_tree *cdt;
  struct cap_derivation_tree *ptr, *parent, *prev_ptr = NULL;
    
  cdt = cap_cte->cap.cdt_node;
  if (cdt == NULL)
    return 0;
  // patch the cdt. 
  ptr = cdt->child_ptr;  
  parent = cdt->parent_ptr;
  
  // update parent pointers
  while (ptr)
  {
    prev_ptr = ptr;
    ptr->parent_ptr = parent;
    ptr = ptr->next;
  }
  
  // update sibling pointers
  if (cdt->child_ptr != NULL)
  {
    if (cdt->next != NULL)
    {
      prev_ptr->next = cdt->next;
      cdt->next->prev = prev_ptr;
    }
    if (cdt->prev != NULL)
    {
      cdt->prev->next = cdt->child_ptr;
      cdt->child_ptr->prev = cdt->prev;
    }
  }
  else
  {
    if (cdt->next != NULL)
    {
      cdt->next->prev = cdt->prev;
    }
    if (cdt->prev != NULL)
    {
      cdt->prev->next = cdt->next;
    }
  }
    
  // update child pointer
  if (parent != NULL && parent->child_ptr == cdt)
  {
    parent->child_ptr = cdt->child_ptr;
  }
  
  kfree(cdt);
  // free the slot
  table = cap_cte - cap_cte->index;
  ASSERT(table->ctetype == lcd_type_free, "lcd_cap_delete: Could not find cnode table\n");
  cap_cte->ctetype = lcd_type_free;
  cap_cte->slot.next_free_cap_slot = table[0].slot.next_free_cap_slot;
  table[0].slot.next_free_cap_slot = cap_cte->index;

  return 0;
}

struct cap_space * lcd_create_cspace(void *objects[], lcd_cap_rights rights[])
{
  int i;
  struct cap_derivation_tree *cdtnode;
  struct task_struct *tcb = objects[LCD_CapInitThreadTCB];
  struct cap_space *cspace = kmalloc(sizeof(struct cap_space), GFP_KERNEL);
  SAFE_EXIT_IF_ALLOC_FAILED(cspace, alloc_failure);
  
  if (tcb == NULL)
  {
    ASSERT(false, "lcd_create_cspace: LCD_CapInitThreadTCB ");
  }
  if ((sizeof(objects)/sizeof(objects[0]) != LCD_CapFirstFreeSlot) ||
      (sizeof(rights)/sizeof(rights[0]) != LCD_CapFirstFreeSlot))
  {
    ASSERT(false, "lcd_create_cspace: Invalid inputs, size of arrays should be equal to LCD_CapFirstFreeSlot\n");
    return NULL;
  }
  
  // initialize semaphore
  sema_init(&(cspace->sem_cspace), 1);
  
  // allocate memory for the first cnode.
  cspace->root_cnode.ctetype = lcd_type_cnode;
  cspace->root_cnode.cnode.cnode_id = 0;
  cspace->root_cnode.cnode.table_level = 0;
  cspace->root_cnode.cnode.table = kmalloc(PAGE_SIZE, GFP_KERNEL);
  SAFE_EXIT_IF_ALLOC_FAILED(cspace->root_cnode.cnode.table, alloc_failure);

  // Get the intitial capabilities into the cspace.
  for (i = 0; i < LCD_CapFirstFreeSlot; i++)
  {
    if (objects[i] == NULL)
      continue;
    cdtnode = kmalloc(sizeof(struct cap_derivation_tree), GFP_KERNEL);
    SAFE_EXIT_IF_ALLOC_FAILED(cdtnode, alloc_failure);
    cspace->root_cnode.cnode.table[i].cap.hobject = objects[i];
    cspace->root_cnode.cnode.table[i].cap.crights = rights[i];
    cdtnode->cap = &(cspace->root_cnode.cnode.table[i]);
    cdtnode->cspace = cspace;
    cdtnode->parent_ptr = NULL;
    cdtnode->child_ptr = NULL;
    cdtnode->next = NULL;
    cdtnode->prev = NULL;
    cspace->root_cnode.cnode.table[i].cap.cdt_node = cdtnode;
  }
  
  // initialize the free list
  lcd_initialize_freelist(cspace->root_cnode.cnode.table, true);
  
  goto success;
  
alloc_failure:
  if (cspace)
  {
    if (cspace->root_cnode.cnode.table)
    {
      for (i = 1; i < LCD_CapFirstFreeSlot; i++)
      {
          if (cspace->root_cnode.cnode.table[i].cap.cdt_node != NULL)
          {
            kfree(cspace->root_cnode.cnode.table[i].cap.cdt_node);
          }
      }
      kfree(cspace->root_cnode.cnode.table);
      cspace->root_cnode.cnode.table = NULL;
    }
    kfree(cspace);
    cspace = NULL;
  }
  
success:
  tcb->cspace = cspace;
  return cspace;
}


cap_id lcd_create_cap(void * ptcb, void * hobject, lcd_cap_rights crights)
{
  struct task_struct *tcb = ptcb;
  struct cap_space *cspace;
  struct cte *cap;
  struct cap_derivation_tree *cdtnode = kmalloc(sizeof(struct cap_derivation_tree), GFP_KERNEL);
  cap_id cid;
  
  if (cdtnode == NULL)
  {
    ASSERT(false, "CDT Node allocation failed\n");
    return 0;
  }
  
  if (ptcb == NULL)
  {
    kfree(cdtnode);
    return 0;
  }
  cspace = tcb->cspace;
  
  if (cspace == NULL || cspace->root_cnode.cnode.table == NULL)
  {
    kfree(cdtnode);
    return 0;
  }
  
  if (down_interruptible(&(cspace->sem_cspace)) == -EINTR)
  {
    kfree(cdtnode);
    return 0;
  }
  cid = lcd_lookup_free_slot(cspace, &cap);
  if (cid == 0)
  {
    up(&(cspace->sem_cspace));
    kfree(cdtnode);
    return 0;
  }
  cap->ctetype = lcd_type_capability;  
  cap->cap.crights = crights;
  cap->cap.hobject = hobject;
  cap->cap.cdt_node = cdtnode;
  cap->cap.cdt_node->cap = cap;
  cap->cap.cdt_node->cspace = cspace;
  cap->cap.cdt_node->child_ptr  = NULL;
  cap->cap.cdt_node->parent_ptr = NULL;
  cap->cap.cdt_node->prev = NULL;
  cap->cap.cdt_node->next = NULL;
  up(&(cspace->sem_cspace));
  return cid;
}

cap_id lcd_cap_grant(void *src_tcb, cap_id src_cid, void * dst_tcb, lcd_cap_rights crights)
{
  cap_id cid = 0;
  struct task_struct *stcb, *dtcb;
  struct cap_space *stcb_cspace, *dtcb_cspace;
  struct cte *src_cte = NULL, *dst_cte = NULL;
  bool done = false;
  struct cap_derivation_tree *dst_cdt_node = kmalloc(sizeof(struct cap_derivation_tree), GFP_KERNEL);
  if (dst_cdt_node == NULL)
  {
    ASSERT(false, "lcd_cap_grant: Failed to allocate cdt node");
    return 0;
  }
    
  if (src_tcb == NULL || dst_tcb == NULL || src_cid <= 0)
  {
    kfree(dst_cdt_node);
    return 0;
  }
  
  stcb = (struct task_struct *)src_tcb;
  dtcb = (struct task_struct *)dst_tcb;
  stcb_cspace = stcb->cspace;
  dtcb_cspace = dtcb->cspace;
  while (!done)
  {
    // Lock source cspace. 
    if (down_trylock(&(stcb_cspace->sem_cspace)) == 0)
    {
        //  if (down_trylock() == 0)  lock dst cspace 
        if (down_trylock(&(dtcb_cspace->sem_cspace)) == 0)
        {
            // Lookup the source TCB and get a pointer to capability.
            src_cte = lcd_lookup_capability(stcb, src_cid);
            // get a free slot in destination.
            cid = lcd_lookup_free_slot(dtcb_cspace, &dst_cte);
            if (cid != 0 && src_cte != NULL && dst_cte != NULL)
            {
              struct cap_derivation_tree *src_cdt_node = src_cte->cap.cdt_node;
              struct cap_derivation_tree *cdtnode = src_cdt_node->child_ptr;
              
              // add the capability to destination.
              dst_cte->ctetype = lcd_type_capability;
              dst_cte->cap.crights = crights;
              dst_cte->cap.hobject = src_cte->cap.hobject;
              dst_cte->cap.cdt_node = dst_cdt_node;
              
              src_cdt_node->child_ptr = dst_cdt_node;
              dst_cdt_node->cspace = dtcb_cspace;
              dst_cdt_node->parent_ptr = src_cdt_node;
              dst_cdt_node->child_ptr  = NULL;
              dst_cdt_node->next = cdtnode;
              dst_cdt_node->prev = NULL;
              
              if (cdtnode)
              {
                cdtnode->prev = dst_cdt_node;
              }
              done = true;
            }
            else
            {
              ASSERT(false, "Source capability not found or no free slot in destination");
            }
            // release lock on dst cspace.
            up(&(dtcb_cspace->sem_cspace));
        }
        // release lock on source cspace.
        up(&(stcb_cspace->sem_cspace));
    }
    if (!done)
      msleep_interruptible(1);
  }
  return cid;
}

uint32_t lcd_cap_delete(void * ptcb, cap_id cid)
{
  int ret;
  struct cte *cap_cte;
  struct task_struct *tcb = ptcb;
  struct cap_space *tcb_cspace;
  if (ptcb == NULL || cid == 0)
    return -1;
  tcb_cspace = tcb->cspace;
  if (down_interruptible(&(tcb_cspace->sem_cspace)))
  {
    ASSERT(false, "lcd_cap_delete: Failed to acquire lock, cannot delete capability\n");
    return -1;
  }
  
  cap_cte = lcd_lookup_capability(tcb, cid);
  if (cap_cte == NULL || cap_cte->cap.cdt_node == NULL)
  {
    up(&(tcb_cspace->sem_cspace));
    return 0;
  }
  ret = lcd_cap_delete_internal(cap_cte);
  up(&(tcb_cspace->sem_cspace));
  return ret;
}

uint32_t lcd_cap_revoke(void * ptcb, cap_id cid)
{
  struct task_struct *tcb = ptcb;
  struct cap_derivation_tree *cdt;
  struct cap_space *tcb_cspace;
  struct kfifo cdt_q;
  struct cte *cap;
  
  if (ptcb == NULL || cid == 0)
    return -1;
  tcb_cspace = tcb->cspace;
  
  if (kfifo_alloc(&cdt_q, sizeof(struct cap_derivation_tree) * 512, GFP_KERNEL) != 0)
  {
    ASSERT(false, "lcd_cap_revoke: Failed to allocate kfifo\n");
    return -1;
  }
  if (down_interruptible(&(tcb_cspace->sem_cspace)))
  {
    ASSERT(false, "lcd_cap_revoke: Signal interrupted cspace lock, CDT will be corrupted\n");
    kfifo_free(&cdt_q);
    return -1;
  }
  cap = lcd_lookup_capability(tcb, cid);
  up(&(tcb_cspace->sem_cspace));
  
  if (cap == NULL)
  {
    ASSERT(false, "lcd_cap_revoke: Capability not found\n");  
    kfifo_free(&cdt_q);
    return -1;
  }
  kfifo_in(&cdt_q, cap->cap.cdt_node, 1);
  while (!kfifo_is_empty(&cdt_q))
  {
    kfifo_out(&cdt_q, cdt, 1);
    if (cdt == NULL)
    {
      ASSERT(false, "lcd_cap_revoke: Corrupted CDT\n");
      kfifo_free(&cdt_q);
      return -1;
    }
    if (cdt->cap && cdt->cap->ctetype == lcd_type_capability)
    {
      struct cap_derivation_tree *ptr;
      struct cap_space *cspace = cdt->cspace;
      if (down_interruptible(&(cspace->sem_cspace)))
      {
        ASSERT(false, "lcd_cap_revoke: Signal interrupted cspace lock, CDT will be corrupted\n");
        kfifo_free(&cdt_q);
        return -1;
      }
      ptr = cdt->child_ptr;
      while (ptr)
      {
        kfifo_in(&cdt_q, ptr, 1);
        ptr = ptr->next;
      }
      lcd_cap_delete_internal(cdt->cap);
      up(&(cspace->sem_cspace));
    }
  }
  kfifo_free(&cdt_q);
  return 0;
}

void lcd_destroy_cspace(void *ptcb)
{
  struct task_struct *tcb = ptcb;
  struct cte *node, level_separator;
  struct kfifo node_q;
  struct cap_space *tcb_cspace;
  int depth_count = 0;
  if (tcb == NULL)
    return;
  tcb_cspace = tcb->cspace;
  // patch the cdt
  lcd_update_cdt(tcb);
  
  // free all the cnodes.
  if (kfifo_alloc(&node_q, sizeof(struct cte) * 512, GFP_KERNEL) != 0)
  {
    ASSERT(false, "lcd_update_cdt: Failed to allcoate kfifo, CDT will be corrupted\n");
    return;
  }
  level_separator.ctetype = lcd_type_separator;
  if (down_interruptible(&(tcb_cspace->sem_cspace)))
  {
    ASSERT(false, "lcd_update_cdt: Signal interrupted lock, CDT will be corrupted\n");
    kfifo_free(&node_q);
    return;
  }
  node = tcb_cspace->root_cnode.cnode.table;
  if (node != NULL)
    kfifo_in(&node_q, node, 1);
  kfifo_in(&node_q, &level_separator, 1);
  while (!kfifo_is_empty(&node_q) && depth_count < MAX_DEPTH)
  {
    int i;
    kfifo_out(&node_q, node, 1);
    if (node->ctetype == lcd_type_separator)
    {
      depth_count++;
      if (!kfifo_is_empty(&node_q))
      {
        kfifo_in(&node_q, &level_separator, 1);
        continue;
      }
      else
      {
        break;
      }
    }

    for (i = CNODE_SLOTS_START; i < MAX_SLOTS; i++)
    {
      if (node[i].ctetype != lcd_type_free && node[i].cnode.table != NULL)
      {
        kfifo_in(&node_q, node[i].cnode.table, 1);
      }
    }
    kfree(node);
  }  
  tcb_cspace->root_cnode.cnode.table = NULL;
  tcb_cspace->root_cnode.ctetype = lcd_type_free;
  up(&(tcb_cspace->sem_cspace));
  kfree(tcb_cspace);
  tcb->cspace = NULL;
  kfifo_free(&node_q);
}

uint32_t lcd_get_cap_rights(void * ptcb, cap_id cid, lcd_cap_rights *rights)
{
  struct task_struct *tcb = ptcb;
  struct cte *cap;
  struct cap_space *tcb_cspace;
  if (tcb == NULL || tcb->cspace == NULL || cid == 0 || rights == NULL)
    return -1;
  tcb_cspace = tcb->cspace;
  if (down_interruptible(&(tcb_cspace->sem_cspace)))
    return -1;
    
  cap = lcd_lookup_capability(tcb, cid);
  if (cap == NULL || cap->ctetype != lcd_type_capability)
  {
    up(&(tcb_cspace->sem_cspace));
    return -1;
  }
  *rights = cap->cap.crights;
  up(&(tcb_cspace->sem_cspace));
  return 0;
}