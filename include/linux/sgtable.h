#ifndef SWITCH_GATE_TABLE_H
#define SWITCH_GATE_TABLE_H

#ifdef CONFIG_ARCH_ENABLE_SUPERVISOR_PKEYS

#include <linux/types.h>
#include <linux/spinlock.h>

//#define MAX_GATE_ENTRIES 1024 // Maximum number of switch gate entries

struct switch_gate_entry {
    u32 source_pkrs;
    u32 target_pkrs;
    // u8 source_pkey;
    // u8 target_pkey;
    // int source_vkey;
    // int target_vkey;
    // bool as_switch;
#ifdef CONFIG_ASID_SWITCH4PKS
    pgd_t *source_pgdir;
    pgd_t *target_pgdir;
    u16 source_asid;
    u16 target_asid;
#endif /* CONFIG_ASID_SWITCH4PKS */
    // unsigned long source_cr3;
    // unsigned long target_cr3;
    unsigned long gate_address;
    unsigned long destination_address;
    //unsigned int destination_domain;
};

struct switch_gate_table {
    struct switch_gate_entry *entries;
    unsigned long cursor;
    unsigned long num_entries;
    spinlock_t lock;
};

extern struct switch_gate_table *sgtable;
extern unsigned long dm_zero_entry_gid;
extern unsigned long dm_zero_exit_gid;
extern unsigned long ipv6_entry_gid;
extern unsigned long ipv6_exit_gid;
#ifdef CONFIG_ASID_SWITCH4PKS
extern void load_new_mm_cr3(pgd_t *pgdir, u16 new_asid, bool need_flush);
extern pgd_t *_pgd_alloc(void);
// extern void unmap_pud_range(p4d_t *p4d, unsigned long start, unsigned long end);
#endif /* CONFIG_ASID_SWITCH4PKS */

struct switch_gate_table *init_switch_gate_table(unsigned long num_entries);

void destroy_switch_gate_table(struct switch_gate_table *table);

#ifdef CONFIG_ASID_SWITCH4PKS
unsigned long register_switch_gate(struct switch_gate_table *table, u32 source_pkrs, u32 target_pkrs, pgd_t *source_pgdir, pgd_t *target_pgdir,
                          u16 source_asid, u16 target_asid, unsigned long gate_address, unsigned long destination_address);
#else /* !CONFIG_ASID_SWITCH4PKS */
unsigned long register_switch_gate(struct switch_gate_table *table, u32 source_pkrs, u32 target_pkrs, 
                          unsigned long gate_address, unsigned long destination_address);
#endif /* CONFIG_ASID_SWITCH4PKS */

struct switch_gate_entry *lookup_switch_gate(struct switch_gate_table *table, unsigned long gate_id);

bool pks_switch(bool insert, unsigned long gate_id);

unsigned long pks_module_setup(struct module *mod);

int pks_module_insert(const char *mod_name, struct module *mod);

#ifdef CONFIG_PKS_STACK
void init_private_stacks(int pkey);
#endif /*CONFIG_PKS_STACK*/

#endif /* CONFIG_ARCH_ENABLE_SUPERVISOR_PKEYS */

#endif /* SWITCH_GATE_TABLE_H */