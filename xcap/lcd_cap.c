#undef __KERNEL__
#define __KERNEL__

#undef MODULE
#define MODULE

#include "lcd_cap.h"
#include <net/irda/parameters.h>
#include <../kvm/paging_tmpl.h>

int init_module(void)
{
	printk(KERN_INFO "Initializing LCD-Capability Module\n");
	return 0;    // Non-zero return means that the module couldn't be loaded.
}

void cleanup_module(void)
{
	printk(KERN_INFO "Cleaning up LCD-Capability Module\n");
}




bool lcd_cap_delete_internal(struct cte *cap, bool *last_reference)
{
  struct cap_space *cspace;
  struct cte *node;
  bool done = false;
  if (cap == NULL || last_reference == NULL)
    return false;
  *last_reference = false;
  node = cap - cap->index;
  if (node[0].ctetype != lcd_type_free)
    return false;
  cspace = node[0].slot.cspace;
  if (cspace == NULL)
    return false;
  
  while (!done)
  {
    // lock the cspace
    if (down_trylock(cspace->sem_cspace) == 0)
    {
      lcd_cap_delete_internal_lockless(cap, last_reference);
      done = true;
    }
    else
    {
      msleep_interruptible(1);
    }
  }
  return true;
}

uint32_t lcd_cap_delete_capability(struct task_struct *tcb, cap_id cid)
{
  struct cte *cap;
  bool last_reference = false;
  if (tcb == NULL)
  {
    LCD_PANIC("lcd_cap_delete_capability: Invalid Input\n");
    return -1;
  }
  
  cap = lcd_cap_lookup_capability(tcb, cid, true);
  if (cap == NULL)
  {
    return -1;
  }
  lcd_cap_delete_internal(cap, &last_reference);
  up(cap->cap.cdt_node->sem_cdt);
  if (last_reference == true)
  {
    // TBD: this is the place where we free the object associated with teh capability.
    // for now we just release the sem_cdt semaphore.
    kfree(cap->cap.cdt_node->sem_cdt);
  }
  return 0;
}

// this function will not lock any semaphore.
// assumption is that cdt and cspace are already locked.
bool lcd_cap_delete_internal_lockless(struct cte *cap, bool *last_reference)
{
  struct cte *table;
  struct cap_derivation_tree *p_cdt, *cdt, *c_cdt, *prev_cdt = NULL;
  int i = 1;
  *last_reference = false;
  
 
  if (cap == NULL)
  {
    LCD_PANIC("lcd_cap_delete_internal: Invalid Input\n");
    return false;
  }

  cdt = cap->cap.cdt_node;
  p_cdt = cdt->parent_ptr;
  c_cdt = cdt->child_ptr;
  if (p_cdt == NULL && c_cdt == NULL && cdt->next == NULL && cdt->prev == NULL)
    *last_reference = true;
  
  // update parent pointer
  while (c_cdt != NULL)
  {
    prev_cdt = c_cdt;
    c_cdt->parent_ptr = p_cdt;
    c_cdt = c_cdt->next;
  }
  c_cdt = cdt->child_ptr;
  
  // update sibling pointers
  if (c_cdt == NULL)
  {
    if (cdt->prev != NULL)
    {
      cdt->prev->next = cdt->next;
    }
    if (cdt->next != NULL)
    {
      cdt->next->prev = cdt->prev;
    }
  }
  else
  {
    if (cdt->prev != NULL)
    {
      cdt->prev->next = c_cdt;
      c_cdt->prev = cdt->prev;
      cdt->prev = NULL;
    }
    if (cdt->next != NULL)
    {
      prev_cdt->next = cdt->next;
      cdt->next->prev = c_cdt;
      cdt->next = NULL;
    }
  }
    
  // update child pointer
  if (p_cdt->child_ptr == cdt)
  {
    if (c_cdt != NULL)
    {
      p_cdt->child_ptr = c_cdt;
    }
    else
    {
      p_cdt->child_ptr = cdt->next;
    }
  }
  
  // delete the capability
  kfree(cdt);
  cap->ctetype = lcd_type_free;
  table = cap - cap->index;
  cap->slot.next_free_cap_slot = table[0].slot.next_free_cap_slot;
  table[0].slot.next_free_cap_slot = cap->index;
  return true;
}

void lcd_cap_destroy_cspace(struct task_struct *tcb)
{
  struct cte *cnode, *node, level_separator;
  struct kfifo cnode_q;
  int size = sizeof(struct cte);
  struct cap_space *cspace;
  int depth_count = 0;
  bool cspace_locked = false, table_visited = false, last_reference = false;
  
  if (tcb == NULL)
  {
    ASSERT(false, "lcd_cap_destroy_cspace: Invalid Input\n");
    return;
  }
  cspace = tcb->cspace;

  if (kfifo_alloc(&cnode_q, sizeof(struct cte) * 512, GFP_KERNEL) != 0)
  {
    ASSERT(false, "lcd_cap_destroy_cspace: Failed to allcoate kfifo, cspace cannot be destroyed\n");
    return;
  }
  if (cspace->root_cnode.cnode.table != NULL)
    kfifo_in(&cnode_q, cspace->root_cnode, size);
  level_separator.ctetype = lcd_type_separator;
  kfifo_in(&cnode_q, &level_separator, size);
  
  while (!kfifo_is_empty(&cnode_q) && depth_count < MAX_DEPTH)
  {
    if (cspace_locked || down_trylock(&(cspace->sem_cspace)) == 0)
    {
      cspace_locked = true;
      cspace->root_cnode.ctetype = lcd_type_invalid;
      if (!table_visited)
      {
        kfifo_out(&cnode_q, cnode, size);
        if (cnode->ctetype == lcd_type_separator)
        {
            depth_count++;
            if (kfifo_is_empty(&cnode_q))
            {
              up(&(cspace->sem_cspace));
              cspace_locked = false;
              break;
            }
            else
            {
              kfifo_in(&cnode_q, level_separator, size);
              continue;
            }
        }
        node = cnode->cnode.table;
        if (node == NULL)
        {
          // we may have a cycle or the tables are corrupted
          up(&(cspace->sem_cspace));
          cspace_locked = false;
          break;
        }
        // push all its cnode to cnode_q
        for (i = CNODE_SLOTS_START; i < MAX_SLOTS; i++)
        {
          if (node[i].ctetype == lcd_type_cnode)
          {
            if (node[i].cnode.table != NULL)
              kfifo_in(&cnode_q, &node[i], size);
          }
        }
        table_visited = true;
        i = 1;
      } // if (!table_visited)
      for (; i < CNODE_SLOTS_START; i++)
      {
        if (node[i].ctetype == lcd_type_free)
          continue;
        // try to get the CDT lock ... recipe for deadlock- Revoke, Delete and delete_internal
        // access the locks in the order CDT Lock and then CSPACE lock, but this function
        // tries to access them in order CSPACE and then CDT. Be very careful while modifying
        // this function or the lock access patterns in this and the related functions.
        if (down_trylock(node[i].cap.cdt_node->sem_cdt) == 0)
        {
          last_reference = false;
          lcd_cap_delete_internal_lockless(&(node[i]), &last_reference);
          up(node[i].cap.cdt_node->sem_cdt);
          if (last_reference == true)
          {
            // TBD: this is the place where we can free the object associated with the cap.
            // for now just free sem_cdt.
            kfree(node[i].cap.cdt_node->sem_cdt);
          }
        }
        else
        {
          up(&(cspace->sem_cspace));
          cspace_locked = false;
          break;
        }
      }
      if (i == CNODE_SLOTS_START)
      {
        kfree(node);
        table_visited = false;
      }
      else
      {
        msleep_interruptible(1);
      }
      if (kfifo_is_empty(*cnode_q) || depth_count >= MAX_DEPTH)
      {
        up(cspace->sem_cspace);
        cspace_locked = false;
      }
    } // if down_trylock(cspace->sem_cspace)
  } // while (!kfifo_is_empty(&cnode_q) && depth_count < MAX_DEPTH)
  kfifo_free(&cnode_q);
  kfree(cspace);
  tcb->cspace = NULL;
  return;
}

cap_id lcd_cap_grant_capability(struct task_struct *stcb, cap_id src_cid, struct task_struct *dtcb, lcd_cap_rights crights)
{
  cap_id cid = 0;
  struct cap_space *src_cspace, *dst_cspace;
  struct cte *src_cte = NULL, *dst_cte = NULL;
  bool done = false;
  struct cap_derivation_tree *dst_cdt_node = kmalloc(sizeof(struct cap_derivation_tree), GFP_KERNEL);
  if (dst_cdt_node == NULL)
  {
    ASSERT(false, "lcd_cap_grant: Failed to allocate cdt node");
    return 0;
  }

  if (stcb == NULL || dtcb == NULL || src_cid <= 0)
  {
    ASSERT(false, "lcd_cap_grant: Invalid Inputs\n");
    kfree(dst_cdt_node);
    return 0;
  }
  src_cspace = stcb->cspace;
  dst_cspace = dtcb->cspace;
  while (!done)
  {
    struct cap_derivation_tree *src_cdt_node;
    struct cap_derivation_tree *cdtnode;
    
    // Lookup the source TCB to get a pointer to capability and keep the cdt locked.
    src_cte = lcd_cap_lookup_capability(stcb, src_cid, true);
    if (src_cte == NULL || src_cte->ctetype != lcd_type_capability)
    {
      LCD_PANIC("lcd_cap_grant_capability: Invalid Capability\n");
      up(src_cte->cap.cdt_node->sem_cdt);
      kfree(dst_cdt_node);
      return 0;
    }
    
    src_cdt_node = src_cte->cap.cdt_node;
    cdtnode = src_cdt_node->child_ptr;
    // lock the destination cspace
    if (down_trylock(dst_cspace->sem_cspace) == 0)
    {
      if (dst_cspace->root_cnode.ctetype == lcd_type_invalid)
      {
        LCD_PANIC("lcd_cap_grant_capability: Destroy may be in progress, aborting operation\n");
        up(dst_cspace->sem_cspace);
        up(src_cte->cap.cdt_node->sem_cdt);
        kfree(dst_cdt_node);
        return 0;
      }
      
      // get a free slot in destination.
      if (cid == 0)
      {
        cid = lcd_cap_lookup_freeslot(dst_cspace, &dst_cte);
      }
      if (dst_cte == NULL || dst_cte->ctetype != lcd_type_free)
      {
        LCD_PANIC("lcd_cap_grant_capability: No free slot\n");
        up(dst_cspace->sem_cspace);
        up(src_cte->cap.cdt_node->sem_cdt);
        kfree(dst_cdt_node);
        return 0;
      }
      
      // add the capability to destination.
      dst_cte->cap.crights = (crights & src_cte->cap.crights);
      dst_cte->cap.hobject = src_cte->cap.hobject;
      dst_cdt_node->sem_cdt = src_cte->cap.cdt_node->sem_cdt;
      dst_cte->cap.cdt_node = dst_cdt_node;
    
      src_cdt_node->child_ptr = dst_cdt_node;
      dst_cdt_node->parent_ptr = src_cdt_node;
      dst_cdt_node->child_ptr  = NULL;
      dst_cdt_node->next = cdtnode;
      dst_cdt_node->prev = NULL;
      
      if (cdtnode)
      {
        cdtnode->prev = dst_cdt_node;
      }
      dst_cte->ctetype = lcd_type_capability;
      done = true;
      up(dst_cspace->sem_cspace);
      up(src_cte->cap.cdt_node->sem_cdt);
      break;
    }
    up(src_cte->cap.cdt_node->sem_cdt);
    if (!done)
      msleep_interruptible(1);
  }
  return cid;
}

uint32_t lcd_cap_get_rights(struct task_struct *tcb, cap_id cid, lcd_cap_rights *rights)
{
  struct cte       *cap;
  
  if (tcb == NULL || tcb->cspace == NULL || cid == 0 || rights == NULL)
  {
    ASSERT(false, "lcd_get_cap_rights: Invalid Inputs\n");
    return -1;
  }
  cap = lcd_lookup_capability(tcb, cid);
  if (cap == NULL || cap->ctetype != lcd_type_capability)
  {
    ASSERT(false, "lcd_get_cap_rights: Invalid capability identifier\n");
    return -1;
  }
  *rights = cap->cap.crights;
  return 0;
}

// does not lock the cspace caller responsbile for the same.
// Given a task_struct and a capability identifier, will return the pointer to the 
// capability table entry associated with that identifier within the cspace of the thread.
struct cte * lcd_cap_lookup_capability(struct task_struct *tcb, cap_id cid, bool keep_locked)
{
  struct cte *cap = NULL, *node = NULL;
  struct cap_space *cspace;
  cap_id id = cid;
  int index = 0;
  int mask = (~0);
    
  // check if input is valid
  if (tcb == NULL || cid == 0)
  {
    ASSERT(false, "lcd_lookup_capability: Invalid Inputs\n");
    return NULL;
  }
  cspace = tcb->cspace;
  
  mask = mask << (CNODE_INDEX_BITS);
  mask = ~mask;
  
  if (cspace == NULL || cspace->root_cnode.cnode.table == NULL)
  {
    ASSERT(false, "lcd_lookup_capability: Invalid/Corrupted cspace\n");
    return NULL;
  }
  node = cspace->root_cnode.cnode.table;
  
  while (id > 0)
  {
    index = (int)(id) & mask;
    id = id >> (CNODE_INDEX_BITS);
    if (node[index].ctetype == lcd_type_capability && id == 0)
    {
      // delete/revoke could be accessing the same node
      // try to lock it and confirm if still valid.
      if (down_interruptible(node[index].cap.cdt_node->sem_cdt) == 0)
      {
        if (node[index].ctetype == lcd_type_capability)
        {
          cap = &node[index];
        }
        else
        {
          up(node[index].cap.cdt_node->sem_cdt);
        }
        if (!keep_locked)
          up(node[index].cap.cdt_node->sem_cdt);
      }
      break;
    }
    else if (node[index].ctetype == lcd_type_cnode && id != 0)
    {
      node = node[index].cnode.table;
    }
    else
    {
      ASSERT(false, "lcd_lookup_capability: Invalid Capability Identifier\n");
      break;
    }
  }
  
  return cap;
}

cap_id lcd_cap_create_capability(struct task_struct *tcb, void * hobject, lcd_cap_rights crights)
{
  struct cap_space *cspace;
  struct cte       *cap;
  cap_id           cid;
  struct cap_derivation_tree *cdtnode;
  
  if (tcb == NULL)
  {
    LCD_PANIC("lcd_cap_create_capability: Invalid Input\n");
    return 0;
  }
  cspace = tcb->cspace;
  
  if (cspace == NULL || cspace->root_cnode.cnode.table == NULL)
  {
    LCD_PANIC("lcd_cap_create_capability: Invalid TCB\n");
    return 0;
  }
  
  cdtnode = kmalloc(sizeof(struct cap_derivation_tree), GFP_KERNEL);
  if (cdtnode == NULL)
  {
    LCD_PANIC("lcd_cap_create_capability: CDT Node allocation failed\n");
    return 0;
  }
  
  cdtnode->sem_cdt = kmalloc(sizeof(struct semaphore), GFP_KERNEL);
  if (cdtnode == NULL)
  {
    LCD_PANIC("lcd_cap_create_capability: CDT Semaphore allocation failed\n");
    kfree(cdtnode);
    return 0;
  }
  
  if (down_interruptible(&(cspace->sem_cspace)))
  {
    if (cspace->root_cnode.ctetype == lcd_type_invalid)
    {
      LCD_PANIC("lcd_cap_create_capability: Destroy may be in progress, operation aborted\n");
      up(&(cspace->sem_cspace));
      kfree(cdtnode->sem_cdt);
      kfree(cdtnode);
      return 0;
    }
    cid = lcd_cap_lookup_freeslot(cspace, &cap);
    if (cid == 0)
    {
      LCD_PANIC("lcd_cap_create_capability: No Free Slot found\n");
      kfree(cdtnode->sem_cdt);
      kfree(cdtnode);
      up(&(cspace->sem_cspace);
      return 0;
    }
    
    cap->ctetype = lcd_type_capability;  
    cap->cap.crights = crights;
    cap->cap.hobject = hobject;
    sema_init(cdtnode->sem_cdt, 1);
    cap->cap.cdt_node = cdtnode;
    cap->cap.cdt_node->cap = cap;
    cap->cap.cdt_node->child_ptr  = NULL;
    cap->cap.cdt_node->parent_ptr = NULL;
    cap->cap.cdt_node->prev = NULL;
    cap->cap.cdt_node->next = NULL;
    up(&(cspace->sem_cspace);
  }
  else
  {
    LCD_PANIC("lcd_cap_create_capability: Signal interrupted cspace lock acquire\n");
    kfree(cdtnode->sem_cdt);
    kfree(cdtnode);
  }
  return cid;
}

struct cap_space * lcd_cap_create_cspace(void *objects[], lcd_cap_rights rights[])
{
  int i;
  bool success = false;
  struct cte *table;
  struct cap_derivation_tree *cdtnode;
  struct task_struct *tcb = objects[LCD_CapInitThreadTCB];
  struct cap_space *cspace = kmalloc(sizeof(struct cap_space), GFP_KERNEL);
  SAFE_EXIT_IF_ALLOC_FAILED(cspace, create_cspace_safe_exit);
  cspace->root_cnode.cnode.table = NULL;
  if (tcb == NULL)
  {
    LCD_PANIC("lcd_create_cspace: LCD_CapInitThreadTCB cannot be NULL\n");
    goto create_cspace_safe_exit;
  }
  if ((sizeof(objects)/sizeof(objects[0]) != LCD_CapFirstFreeSlot) ||
      (sizeof(rights)/sizeof(rights[0]) != LCD_CapFirstFreeSlot))
  {
    LCD_PANIC("lcd_create_cspace: Invalid inputs, size of arrays should be equal to LCD_CapFirstFreeSlot\n");
    goto create_cspace_safe_exit;
  }
  
  // initialize semaphore
  sema_init(&(cspace->sem_cspace), 1);
  if (down_interruptible(&(cspace->root_cnode.sem_cspace)))
  {
    // allocate memory for the first cnode.
    cspace->root_cnode.ctetype = lcd_type_cnode;
    cspace->root_cnode.cnode.cnode_id = 0;
    cspace->root_cnode.cnode.table_level = 0;
    cspace->root_cnode.cnode.table = kmalloc(PAGE_SIZE, GFP_KERNEL);
    if (cspace->root_cnode.cnode.table == NULL)
    {
      LCD_PANIC("lcd_cap_create_cspace: Failed to allocate cnode table\n");
      goto create_cspace_safe_exit;
    }

    table = cspace->root_cnode.cnode.table;
    table[0].ctetype = lcd_type_free;
    // Get the intitial capabilities into the cspace.
    for (i = 1; i < LCD_CapFirstFreeSlot; i++)
    {
      table[i].cap.hobject = objects[i];
      table[i].cap.crights = rights[i];
      if (objects[i] == NULL)
      {
        table[i].ctetype = lcd_type_invalid;
        continue;
      }
      table[i].ctetype = lcd_type_capability;
      cdtnode = kmalloc(sizeof(struct cap_derivation_tree), GFP_KERNEL);
      if (cdtnode == NULL)
      {
        LCD_PANIC("lcd_cap_create_cspace: Failed to allocate cdt node\n");
        goto create_cspace_safe_exit;
      }
      cdtnode->cap = &(cspace->root_cnode.cnode.table[i]);
      cdtnode->sem_cdt = kmalloc(sizeof(struct semaphore), GFP_KERNEL);
      if (cdtnode->sem_cdt == NULL)
      {
        LCD_PANIC("lcd_cap_create_cspace: Failed to allocate cdt semaphore\n");
        goto create_cspace_safe_exit;
      }
      sema_init(cdt->sem_cdt, 1);
      cdtnode->parent_ptr = NULL;
      cdtnode->child_ptr = NULL;
      cdtnode->next = NULL;
      cdtnode->prev = NULL;
      table[i].cap.cdt_node = cdtnode;
    }

    // initialize the free list
    success = lcd_cap_initialize_freelist(cspace, cspace->root_cnode.cnode.table, true);
    up(&(cspace->sem_cspace));
    if (!success)
    {
      LCD_PANIC("lcd_cap_create_cspace: Failed to initialize free list\n");
      goto create_cspace_safe_exit;
    }
  }
  else
  {
    LCD_PANIC("lcd_cap_create_cspace: Failed to acquire cspace lock\n");
    goto create_cspace_safe_exit;
  }
  
  tcb->cspace = cspace;
  return cspace;
  
create_cspace_safe_exit:
  if (cspace)
  {
    if (cspace->root_cnode.cnode.table)
    {
      for (i = 1; i < LCD_CapFirstFreeSlot; i++)
      {
          if (cspace->root_cnode.cnode.table[i].cap.cdt_node != NULL)
          {
            if (cspace->root_cnode.cnode.table[i].cap.cdt_node->sem_cdt != NULL)
            {
              kfree(cspace->root_cnode.cnode.table[i].cap.cdt_node->sem_cdt);
            }
            kfree(cspace->root_cnode.cnode.table[i].cap.cdt_node);
          }
      }
      kfree(cspace->root_cnode.cnode.table);
      cspace->root_cnode.cnode.table = NULL;
    }
    kfree(cspace);
    tcb->cspace = NULL;
  }
  return NULL;
}

// does not lock the cspace.
// First entry of every cnode table will be the head of the free slots available
// in the table. This function will just populate the free list.
bool lcd_cap_initialize_freelist(struct cap_space *cspace, struct cte *cnode, bool bFirstCNode)
{
  int startid = 1;
  int i;
  
  if (cnode == NULL || cspace == NULL)
    return;
  
  if (bFirstCNode)
  {
    startid = LCD_CapFirstFreeSlot;
  }
  
  cnode[0].ctetype = lcd_type_invalid;
  cnode[0].slot.cspace = cspace;
  cnode[0].ctetype = lcd_type_free;
  cnode[0].index = 0;
  cnode[0].slot.next_free_cap_slot = startid;
  cnode[startid].ctetype = lcd_type_free;
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

// Removes the free_slot from free list.
struct cte * lcd_cap_reserve_slot(struct cte *cnode, cap_id *cid, int free_slot)
{
  struct cte *node = cnode->cnode.table;
  ASSERT(node[free_slot].ctetype == lcd_type_free, "Free List is corrupted\n");
  // a valid empty slot
  node[0].slot.next_free_cap_slot = node[free_slot].slot.next_free_cap_slot;
  lcd_set_bits_at_level(cnode, cid, free_slot);
  return &node[free_slot];
}


// free slot will be booked for the caller.
// ie. it will not longer be free once this function returns it.
// naming convention:
// cnode = struct cte entry in the table, which points to another table.
// node = pointer the the array of struct cte entries/capability table i.e. cnode->cnode.table
cap_id lcd_cap_lookup_freeslot(struct cap_space *cspace, struct cte **cap)
{
  cap_id cid = 0;
  bool found = false;
  int i = 0;
  int size = sizeof(struct cte);
  struct kfifo cnode_q;
  
  if (cspace == NULL || cspace->root_cnode.cnode.table == NULL || cap == NULL)
  {
    LCD_PANIC("lcd_cap_lookup_freeslot: Invalid Inputs\n");
    return 0;
  }
  
  if (kfifo_alloc(&cnode_q, sizeof(struct cte) * 512, GFP_KERNEL) != 0)
  {
    LCD_PANIC("lcd_cap_lookup_freeslot: Failed to allocate FIFO buffer\n");
    return 0;
  }

  kfifo_in(&cnode_q, &(cspace->root_cnode), size);
  
  while (!found && !kfifo_is_empty(&cnode_q))
  {
    int free_cap_slot = 0, free_cnode_slot = 0, cnode_id;
    struct cte *node = NULL, *cnode = NULL;
    
    kfifo_out(&cnode_q, cnode, size);
    if (cnode == NULL)
    {
      break;
    }
    
    node = cnode->cnode.table;
    
    free_cap_slot   = node[0].slot.next_free_cap_slot;
    free_cnode_slot = node[0].slot.next_free_cnode_slot;
    
    if (free_cap_slot != 0 && free_cap_slot < CNODE_SLOTS_START)
    {
      *cap = lcd_cap_reserve_slot(cnode, &cid, free_cap_slot);
      break;
    }
    
    if (free_cnode_slot != 0 && free_cnode_slot >= CNODE_SLOTS_START && free_cnode_slot < MAX_SLOTS)
    {
      // there is no slot free for capability
      // 1. Check if free slots are available at next level
      for (i = CNODE_SLOTS_START; i < free_cnode_slot; i++)
      {
        struct cte * next_cnode = &node[i];
        struct cte *next_node = node[i].cnode.table; 
        free_cap_slot = next_node[0].slot.next_free_cap_slot;
        if (free_cap_slot != 0 && free_cap_slot < CNODE_SLOTS_START)
        {
          *cap = lcd_cap_reserve_slot(next_cnode, &cid, free_cap_slot);
          found = true;
          break;
        }
      }
      if (found)
        break;
      
      // we will have to allocate a new cnode
      node[free_cnode_slot].cnode.table = kmalloc(PAGE_SIZE, GFP_KERNEL);
      if (node[free_cnode_slot].cnode.table == NULL)
      {
        LCD_PANIC("lcd_cap_lookup_freeslot: Not able to allocate cnode\n");
        break;
      }
      node[0].slot.next_free_cnode_slot = node[free_cnode_slot].slot.next_free_cnode_slot;
      node[free_cnode_slot].ctetype = lcd_type_cnode;
      lcd_cap_initialize_freelist(cspace, node[free_cnode_slot].cnode.table, false);
      node[free_cnode_slot].cnode.table_level = cnode->cnode.table_level + 1;
      lcd_set_bits_at_level(cnode, &cnode_id, free_cnode_slot);
      node[free_cnode_slot].cnode.cnode_id = cnode_id;
      
      cnode = &node[free_cnode_slot];
      node = node[free_cnode_slot].cnode.table;
      free_cap_slot = node[0].slot.next_free_cap_slot;
      if (free_cap_slot != 0 && free_cap_slot < CNODE_SLOTS_START)
      {
        *cap = lcd_cap_reserve_slot(cnode, &cid, free_cap_slot);
      }
      break;
    } // found a free cnode slot
    else 
    {
      // nothing is free in this cnode 
      // kep lookin at all its children
      for (i = CNODE_SLOTS_START; i < MAX_SLOTS; i++)
      {
        kfifo_in(&cnode_q, &node[i], size);
      }
    }
  } // while (!kfifo_is_empty())
lookup_freeslot_safe_exit:
  kfifo_free(&cnode_q);
  return cid;
}

