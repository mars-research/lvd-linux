#ifndef __LCD_CAP_H__
#define __LCD_CAP_H__

#include <linux/types.h>
#include <linux/semaphore.h>

#include <uapi/linux/lcd-cap.h>

typedef uint32_t   lcd_cnode;         // a pointer to the cnode
typedef uint32_t   lcd_cnode_entry;   // a pointer to an entry within a cnode
typedef uint64_t   lcd_tcb;           // a pointer/handle to the thread contrl block
typedef uint16_t   lcd_cap_rights;    // holds the rights associated with a capability.
typedef uint16_t   lcd_cap_type;


enum __lcd_cap_type
{
	lcd_type_invalid,
	lcd_type_free,
	lcd_type_capability,
	lcd_type_cnode,
	lcd_type_endpoint,
	lcd_type_separator
};


struct cap_derivation_tree
{
	struct cte                 *cap;      
	struct semaphore           *sem_cdt;
	struct cap_derivation_tree *next;
	struct cap_derivation_tree *prev;
	struct cap_derivation_tree *parent_ptr;
	struct cap_derivation_tree *child_ptr;  
};

struct capability_internal 
{
	void                       *hobject;  // a pointer to a kernel object
	struct cap_derivation_tree *cdt_node; // list of domain ids to whom this 
	                                      //capability is granted
	lcd_cap_rights crights; // specifies the rights the domain has over this capability
};

struct cte;

struct cap_node
{
	capability_t     cnode_id;
	struct cte *table; /* points to another cnode table */
	uint16_t   table_level;
};

struct free_slot_t
{
	int next_free_cap_slot;
	int next_free_cnode_slot;
	struct cap_space *cspace;
};

struct cte // capability table entry
{
	union
	{
		struct cap_node cnode;
		struct capability_internal cap;
		struct free_slot_t slot;
	};
	lcd_cap_type ctetype;
	uint16_t     index;
};

struct cap_space
{
	struct cte       root_cnode;
	struct semaphore sem_cspace;
};


#endif
