#ifdef CONFIG_ARCH_ENABLE_SUPERVISOR_PKEYS

#include <linux/sgtable.h>
#include <linux/pks.h>
#include <linux/module.h>

// #include <trace/events/tlb.h>

#include <asm/set_memory.h>
#include <asm/tlbflush.h>

unsigned long dm_zero_entry_gid = 1;
EXPORT_SYMBOL(dm_zero_entry_gid);
unsigned long dm_zero_exit_gid = 1;
EXPORT_SYMBOL(dm_zero_exit_gid);
unsigned long ipv6_entry_gid = 1;
EXPORT_SYMBOL(ipv6_entry_gid);
unsigned long ipv6_exit_gid = 1;
EXPORT_SYMBOL(ipv6_exit_gid);

struct switch_gate_table *init_switch_gate_table(unsigned long num_entries)
{
    struct switch_gate_table *table;
    unsigned long table_size, entries_size, table_order, entries_order;
    // pgd_t *current_pgd;
    // u16 current_asid;
    
    // Allocate page aligned memory for the switch gate table
    // current_pgd = this_cpu_read(cpu_tlbstate.loaded_mm)->pgd;
    // current_asid = this_cpu_read(cpu_tlbstate.loaded_mm_asid);
    // load_new_mm_cr3(init_mm.pgd, 500, false);
    table_size = sizeof(struct switch_gate_table);
    table_order = get_order(table_size);
    table = (struct switch_gate_table *)__get_free_pages(GFP_KERNEL, table_order);
    if (!table)
        return NULL;
    
    // Allocate page aligned memory for the switch gate entries
    entries_size = num_entries * sizeof(struct switch_gate_entry);
    entries_order = get_order(entries_size);
    table->entries = (struct switch_gate_entry *)__get_free_pages(GFP_KERNEL, entries_order);
    if (!table->entries) {
        free_pages((unsigned long)table, table_order);
        return NULL;
    }

    // Initialize the entries and other fields
    table->entries[0].target_pkrs = PKRS_MONITOR;
    table->entries[1].source_pkrs = PKRS_MONITOR;
    table->entries[1].target_pkrs = pkrs_default;
    table->num_entries = num_entries;
    table->cursor = 2;
    spin_lock_init(&table->lock);

#ifdef CONFIG_PKS_MONITOR
    set_memory_pks((unsigned long)table, 1 << table_order, PKS_KEY_MONITOR);
    set_memory_pks((unsigned long)table->entries, 1 << entries_order, PKS_KEY_MONITOR);
#endif /* CONFIG_PKS_MONITOR */
// #ifdef CONFIG_PKK
//     set_memory_pks((unsigned long)table, 1 << table_order, PKS_KEY_MONITOR);
//     set_memory_pks((unsigned long)table->entries, 1 << entries_order, PKS_KEY_MONITOR);
// #endif /* CONFIG_PKK*/
    // load_new_mm_cr3(current_pgd, current_asid, false);
    
    return table;
}

// Free the memory allocated for the switch gate table
void destroy_switch_gate_table(struct switch_gate_table *table)
{
    if (table) {
        kfree(table->entries);
        kfree(table);
    }
}

#ifdef CONFIG_ASID_SWITCH4PKS
// Register a switch gate entry in the table
unsigned long register_switch_gate(struct switch_gate_table *table, u32 source_pkrs, u32 target_pkrs, pgd_t *source_pgdir, pgd_t *target_pgdir,
                          u16 source_asid, u16 target_asid, unsigned long gate_address, unsigned long destination_address)
{
    bool insert;
    u32 pkrs_pre;
    unsigned long call_gate_id, return_gate_id;

    call_gate_id = table->cursor;
    return_gate_id = call_gate_id + 1;

    // Check if the gate_id is within the valid range
    if (return_gate_id >= table->num_entries) {
        pr_err("Too many switch gate table entries!\n");
        return 0;
    }

    insert = pks_switch(true, 0);
    
    spin_lock(&table->lock);

    if (insert) {
        pkrs_pre = __this_cpu_read(previous_pkrs);
        table->entries[1].target_pkrs = pkrs_pre;
    }

    // Populate the switch gate entry
    table->entries[call_gate_id].source_pkrs = source_pkrs;
    table->entries[call_gate_id].target_pkrs = target_pkrs;
    table->entries[call_gate_id].source_pgdir = source_pgdir;
    table->entries[call_gate_id].target_pgdir = target_pgdir;
    table->entries[call_gate_id].source_asid = source_asid;
    table->entries[call_gate_id].target_asid = target_asid;
    table->entries[call_gate_id].gate_address = gate_address;
    table->entries[call_gate_id].destination_address = destination_address;
    table->entries[return_gate_id].source_pkrs = target_pkrs;
    table->entries[return_gate_id].target_pkrs = source_pkrs;
    table->entries[return_gate_id].source_pgdir = target_pgdir;
    table->entries[return_gate_id].target_pgdir = source_pgdir;
    table->entries[return_gate_id].source_asid = target_asid;
    table->entries[return_gate_id].target_asid = source_asid;
    table->entries[return_gate_id].gate_address = destination_address;
    table->entries[return_gate_id].destination_address = gate_address;
    table->cursor += 2;
    
    spin_unlock(&table->lock);
    pks_switch(insert, 1);
    return call_gate_id;
}
#else /* !CONFIG_ASID_SWITCH4PKS */
// Register a switch gate entry in the table
unsigned long register_switch_gate(struct switch_gate_table *table, u32 source_pkrs, u32 target_pkrs, 
                          unsigned long gate_address, unsigned long destination_address)
{
    bool insert;
    u32 pkrs_pre;
    unsigned long call_gate_id, return_gate_id;

    call_gate_id = table->cursor;
    return_gate_id = call_gate_id + 1;

    // Check if the gate_id is within the valid range
    if (return_gate_id >= table->num_entries) {
        pr_err("Too many switch gate table entries!\n");
        return 0;
    }

    insert = pks_switch(true, 0);
    
    spin_lock(&table->lock);

    if (insert) {
        pkrs_pre = __this_cpu_read(previous_pkrs);
        table->entries[1].target_pkrs = pkrs_pre;
    }

    // Populate the switch gate entry
    table->entries[call_gate_id].source_pkrs = source_pkrs;
    table->entries[call_gate_id].target_pkrs = target_pkrs;
    table->entries[call_gate_id].gate_address = gate_address;
    table->entries[call_gate_id].destination_address = destination_address;
    table->entries[return_gate_id].source_pkrs = target_pkrs;
    table->entries[return_gate_id].target_pkrs = source_pkrs;
    table->entries[return_gate_id].gate_address = destination_address;
    table->entries[return_gate_id].destination_address = gate_address;
    table->cursor += 2;
    
    spin_unlock(&table->lock);
    pks_switch(insert, 1);
    return call_gate_id;
}
#endif /* CONFIG_ASID_SWITCH4PKS */
EXPORT_SYMBOL(register_switch_gate);

// Retrieve the switch gate entry from the table
struct switch_gate_entry *lookup_switch_gate(struct switch_gate_table *table, unsigned long gate_id)
{
    struct switch_gate_entry *entry = NULL;
    
    spin_lock(&table->lock);
    
    // Check if the gate_id is within the valid range
    if (gate_id < table->num_entries)
        entry = &table->entries[gate_id];
    
    spin_unlock(&table->lock);
    
    return entry;
}
EXPORT_SYMBOL(lookup_switch_gate);

#ifdef CONFIG_PKS_STACK
DEFINE_PER_CPU(void *, pstacks);
EXPORT_SYMBOL(pstacks);

void init_private_stacks(int pkey)
{
    int cpu;
    void **per_cpu_ptr;
    for_each_possible_cpu(cpu) {
        per_cpu_ptr = per_cpu_ptr(&pstacks, cpu);
        *per_cpu_ptr = (void *)__get_free_pages(GFP_KERNEL, 4);
        set_memory_pks((unsigned long)*per_cpu_ptr, 1 << 4, pkey);
        if (!(*per_cpu_ptr)) {
            pr_err("Private stack memory allocation failed for CPU %d\n", cpu);
        } else {
            pr_info("Private stack memory allocation for CPU %d: %016lx\n", cpu, (unsigned long)*per_cpu_ptr);
        }
    }
}
EXPORT_SYMBOL(init_private_stacks);
#endif

bool pks_switch(bool insert, unsigned long gate_id)
{
    // uint64_t start, end;
	// unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
	u32 pkrs, target_pkrs;
    struct switch_gate_entry sgt_entry;
#ifdef CONFIG_ASID_SWITCH4PKS
    pgd_t *current_pgd;
    u16 asid;
#endif /* CONFIG_ASID_SWITCH4PKS */

    // asm volatile("cpuid\n\t"
	// 			"rdtsc\n\t"
	// 			"mov %%edx, %0\n\t"
	// 			"mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
	// 			"%rax", "%rbx", "%rcx", "%rdx");

#ifdef CONFIG_PKS_STACK
    bool islocked = false;
    // int pkey = (gate_id / 2) + 1;
    // bool isReturn = !(gate_id % 2);
	unsigned long saved_rsp, new_rsp;
    unsigned long new_stack;
    void **ppstack = this_cpu_ptr(&pstacks);
	unsigned long flags = native_save_fl();

	if (X86_EFLAGS_IF & flags) {
		native_irq_disable();	
	} else {
		islocked = true;
	}

	new_stack = (unsigned long) *ppstack;
	// new_stack = __get_free_pages(GFP_KERNEL, 2);
    asm volatile ("mov %%rsp, %0" : "=m"(saved_rsp):);
	// new_rsp = (unsigned long) new_stack + (unsigned long)(saved_rsp - (unsigned long) current->stack);
	new_rsp = (unsigned long) new_stack + (unsigned long)(4096 * 2);
	asm volatile ("mov %0, %%rsp": :"m"(saved_rsp));
	// pr_info("pstack1: saved_rsp:%016lx, new_rsp:%016lx, new_stack:%016lx\n", saved_rsp, new_rsp, new_stack);
	if (!islocked)
		native_irq_enable();
#endif

    // asm volatile("cpuid\n\t"
	// 			"rdtsc\n\t"
	// 			"mov %%edx, %0\n\t"
	// 			"mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
	// 			"%rax", "%rbx", "%rcx", "%rdx");
#ifdef CONFIG_PKS_MONITOR   
    if (!pks_tables_inited())
        return false;
#endif
    
    if (!insert)
        return false;
	
	// start = rdtsc();
	// asm volatile("cpuid\n\t"
	// 			"rdtsc\n\t"
	// 			"mov %%edx, %0\n\t"
	// 			"mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
	// 			"%rax", "%rbx", "%rcx", "%rdx");
    preempt_disable();
    //local_irq_disable();
    sgt_entry = sgtable->entries[gate_id];

#ifdef CONFIG_PKS_HIT
    if (pks_key_max > 113) { //(system_state == SYSTEM_RUNNING) {
        pr_info("gate_id: %lu\n", gate_id);
    }
#endif

#ifdef CONFIG_ASID_SWITCH4PKS
    if(gate_id % 16 == 2) {
        current_pgd = this_cpu_read(cpu_tlbstate.loaded_mm)->pgd; //init_mm.pgd; //this_cpu_read(cpu_tlbstate.loaded_mm)->pgd;
        asid = this_cpu_read(cpu_tlbstate.loaded_mm_asid);
        if (cpu_feature_enabled(X86_FEATURE_PCID)) {
            load_new_mm_cr3(current_pgd, asid, false); //need_flush
        } else {
            load_new_mm_cr3(current_pgd, asid, true); //need_flush
        }
        // flush_tlb_all();
        // trace_tlb_flush(TLB_FLUSH_ON_TASK_SWITCH, TLB_FLUSH_ALL);
    }
#endif /* CONFIG_ASID_SWITCH4PKS */

    pkrs = __this_cpu_read(pkrs_cache);
    target_pkrs = sgt_entry.target_pkrs;

	if (pkrs == target_pkrs)
		return false;
    __this_cpu_write(previous_pkrs, pkrs);
	__this_cpu_write(pkrs_cache, target_pkrs);
	//start = rdtsc();
	// asm volatile("cpuid\n\t"
	// 			"rdtsc\n\t"
	// 			"mov %%edx, %0\n\t"
	// 			"mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
	// 			"%rax", "%rbx", "%rcx", "%rdx");
#ifdef CONFIG_PKK
    write_pkru(target_pkrs);
#else /* !CONFIG_PKK */
	wrmsrl(MSR_IA32_PKRS, target_pkrs);
#endif /* CONFIG_PKK */
    current->thread.pkrs = target_pkrs;
	//end = rdtsc();
	// asm volatile("rdtscp\n\t"
	// 			"mov %%edx, %0\n\t"
	// 			"mov %%eax, %1\n\t"
	// 			"cpuid\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
	// 			"%rax", "%rbx", "%rcx", "%rdx");
	// start = ( ((uint64_t)cycles_high << 32) | cycles_low );
	// end = ( ((uint64_t)cycles_high1 << 32) | cycles_low1 );
	// printk(KERN_INFO "pkrs:%#x, start:%llu, end:%llu, cycles:%llu\n", target_pkrs, start, end, end-start);
    //printk(KERN_INFO "(pks_switch)pkrs:%#x\n", target_pkrs);
    preempt_enable();
    //local_irq_enable();
	return true;
}
EXPORT_SYMBOL(pks_switch);

#ifdef CONFIG_ASID_SWITCH4PKS
// unsigned long pks_module_setup(struct module *mod) 
// {
//     int pkey;
//     u32 pkrs;
//     unsigned long gate_id;
//     u16 source_asid = 0;
//     u16 target_asid = 0;
//     pgd_t *new_pgd, *source_pgdir, *target_pgdir;
//     // pud_t *new_pud;
//     // int i;

//     source_pgdir = this_cpu_read(cpu_tlbstate.loaded_mm)->pgd;
//     target_pgdir = this_cpu_read(cpu_tlbstate.loaded_mm)->pgd;
//     if (pks_key_max == 1) {
//         pkey = 1;
//         // sgtable->entries[sgtable->cursor].as_switch = true;
//         new_pgd = _pgd_alloc();
//         // old_pgd = init_mm.pgd;
//         if (!new_pgd) {
//             pr_err("New _pgd_alloc failed!\n");
//             new_pgd = swapper_pg_dir;
//         } else {
//             clone_pgd_range(new_pgd + KERNEL_PGD_BOUNDARY,
// 				swapper_pg_dir + KERNEL_PGD_BOUNDARY,
// 				KERNEL_PGD_PTRS);
//             // for (i = 0; i < PTRS_PER_PGD; i++) {
//             //     if (unlikely(pgd_none(old_pgd[i])))
//             //         continue;

//             //     new_pud = pud_alloc(&init_mm, new_pgd)
//             // }
//         }
//         /*
//         * cpu_tlbstate.loaded_mm should match CR3 whenever interrupts
//         * are on.  This means that it may not match current->active_mm,
//         * which will contain the previous user mm when we're in lazy TLB
//         * mode even if we've already switched back to swapper_pg_dir.
//         *
//         * During switch_mm_irqs_off(), loaded_mm will be set to
//         * LOADED_MM_SWITCHING during the brief interrupts-off window
//         * when CR3 and loaded_mm would otherwise be inconsistent.  This
//         * is for nmi_uaccess_okay()'s benefit.
//         */
//         target_pgdir = new_pgd;
//         source_asid = this_cpu_read(cpu_tlbstate.loaded_mm_asid);
//         target_asid = 1000;//this_cpu_read(cpu_tlbstate.next_asid);
//     } else {
//         pkey = pks_new_key();
//     }
//     pkrs = PKEY2PKRS(pkey);
//     pkrs_default = pkrs | (PKEY_DISABLE_WRITE << (pkey * PKR_BITS_PER_PKEY));
//     //update table->entries[1].target_pkrs in register_switch_gate(), must before pks_switch during set_memory_pks
//     __this_cpu_write(pkrs_cache, pkrs_default);
//     gate_id = register_switch_gate(sgtable, pkrs_default, pkrs, source_pgdir, target_pgdir, source_asid, target_asid, 0, 0);
//     load_new_mm_cr3(target_pgdir, target_asid, false);
//     set_memory_pks((unsigned long)mod->init_layout.base, mod->init_layout.size >> PAGE_SHIFT, pkey);
// 	set_memory_pks((unsigned long)mod->core_layout.base, mod->core_layout.size >> PAGE_SHIFT, pkey);
//     load_new_mm_cr3(source_pgdir, source_asid, false);
//     if (strcmp(mod->name, "dm_zero") == 0) {
//         dm_zero_entry_gid = gate_id;
//         dm_zero_exit_gid = gate_id + 1;
//     }
//     pr_info("PKS module %s setup!\n", mod->name);
    
//     return gate_id;
// }
unsigned long pks_module_setup(struct module *mod) 
{
    int pkey;
    u32 pkrs;
    unsigned long gate_id;
    u16 source_asid = 0;
    u16 target_asid = 0;
    pgd_t *new_pgd, *source_pgdir, *target_pgdir; //mod_pgd_start/end
    unsigned long i, j, k, m, pgd_index_start, pgd_index_end;
    unsigned long pud_index_start, pud_index_end, pmd_index_start, pmd_index_end, pte_index_start, pte_index_end;
    // p4d_t *p4d;
    pud_t *new_pud, *old_pud;
    pmd_t *new_pmd, *old_pmd;
    pte_t *new_pte, *old_pte;
    unsigned long address_start = (unsigned long)mod->core_layout.base;
    unsigned long address_end = (unsigned long)mod->core_layout.base + mod->core_layout.size;

    source_pgdir = this_cpu_read(cpu_tlbstate.loaded_mm)->pgd;
    target_pgdir = this_cpu_read(cpu_tlbstate.loaded_mm)->pgd;
    source_asid = this_cpu_read(cpu_tlbstate.loaded_mm_asid);
    target_asid = this_cpu_read(cpu_tlbstate.next_asid);
    pgd_index_start = pgd_index(address_start);
    pgd_index_end = pgd_index(address_end);
    pud_index_start = pud_index(address_start);
    pud_index_end = pud_index(address_end);
    pmd_index_start = pmd_index(address_start);
    pmd_index_end = pmd_index(address_end);
    pte_index_start = pte_index(address_start);
    pte_index_end = pte_index(address_end);
    old_pud = p4d_pgtable(*((p4d_t *)(source_pgdir + pgd_index_start)));
    old_pmd = pud_pgtable(*(old_pud + pud_index_start));
    old_pte = (pte_t *)pmd_page_vaddr(*(old_pmd + pmd_index_start));
    
    pkey = pks_new_key();
    pkey = pkey % 14;
    if (pkey < 2) {
        pkey = pkey + 14;
    }
    pkrs = PKEY2PKRS(pkey);
    pkrs_default = pkrs | (PKEY_DISABLE_WRITE << (pkey * PKR_BITS_PER_PKEY));
    //update table->entries[1].target_pkrs in register_switch_gate(), must before pks_switch during set_memory_pks
    __this_cpu_write(pkrs_cache, pkrs_default);
    gate_id = register_switch_gate(sgtable, pkrs_default, pkrs, source_pgdir, target_pgdir, source_asid, target_asid, 0, 0);
    // load_new_mm_cr3(target_pgdir, target_asid, false);
    set_memory_pks((unsigned long)mod->init_layout.base, mod->init_layout.size >> PAGE_SHIFT, pkey);
	set_memory_pks((unsigned long)mod->core_layout.base, mod->core_layout.size >> PAGE_SHIFT, pkey);
    // load_new_mm_cr3(source_pgdir, source_asid, false);
    if (strcmp(mod->name, "dm_zero") == 0) {
        dm_zero_entry_gid = gate_id;
        dm_zero_exit_gid = gate_id + 1;
    } else if (strcmp(mod->name, "ipv6") == 0) {
        ipv6_entry_gid = gate_id;
        ipv6_exit_gid = gate_id + 1;
    }

    if (pks_key_max % 16 == 1 ) {
        // sgtable->entries[sgtable->cursor].as_switch = true;
        new_pgd = _pgd_alloc();
        // old_pgd = init_mm.pgd;
        if (!new_pgd) {
            pr_err("New _pgd_alloc failed!\n");
            new_pgd = swapper_pg_dir;
        } else {
            // enable_pgtable_write();
            for (i = KERNEL_PGD_BOUNDARY; i < PTRS_PER_PGD; i++) {
                if (i >= pgd_index_start && i <= pgd_index_end) {
                    new_pud = pud_alloc(&init_mm, (p4d_t *)(new_pgd + i), address_start);
                    if (!new_pud) {
                        pr_err("pud_alloc failed!\n");
                    }
                    new_pud -= pud_index_start;
                    for (j = 0; j < PTRS_PER_PUD; j++) {
                        if (j >= pud_index_start && j <= pud_index_end) {
                            new_pmd = pmd_alloc(&init_mm, new_pud + j, address_start);
                            if (!new_pmd) {
                                pr_err("pmd_alloc failed!\n");
                            }
                            new_pmd -= pmd_index_start;
                            for (k = 0; k < PTRS_PER_PMD; k++) {
                                if (k >= pmd_index_start && k <= pmd_index_end) {
                                    new_pte = pte_alloc_kernel(new_pmd + k, address_start);
                                    if (!new_pte) {
                                        pr_err("pte_alloc_kernel failed!\n");
                                    }
                                    new_pte -= pte_index_start;
                                    for (m = 0; m < PTRS_PER_PTE; m++) {
                                        if (m >= pte_index_start && m <= pte_index_end) {
                                            set_pte(new_pte + m, __pte(0));
                                            continue;
                                        }
                                        enable_pgtable_write();
                                        memcpy(new_pte + m, old_pte + m, sizeof(pte_t));
                                        disable_pgtable_write();
                                    }
                                    continue;
                                }
                                enable_pgtable_write();
                                memcpy(new_pmd + k, old_pmd + k, sizeof(pmd_t));
                                disable_pgtable_write();
                            }
                            continue;
                        }
                        enable_pgtable_write();
                        memcpy(new_pud + j, old_pud + j, sizeof(pud_t));
                        disable_pgtable_write();
                    }
                    continue;
                }
                enable_pgtable_write();
                memcpy(new_pgd + i, source_pgdir + i, sizeof(pgd_t));
                disable_pgtable_write();
            }
            // disable_pgtable_write();

            // clone_pgd_range(new_pgd + KERNEL_PGD_BOUNDARY,
			// 	source_pgdir + KERNEL_PGD_BOUNDARY,
			// 	KERNEL_PGD_PTRS);
            // unmap_pud_range((p4d_t *)mod_pgd, (unsigned long)mod->core_layout.base, (unsigned long)mod->core_layout.base + mod->core_layout.size);
            // clone_pgd_range(swapper_pg_dir + KERNEL_PGD_BOUNDARY,
			// 	new_pgd + KERNEL_PGD_BOUNDARY,
			// 	KERNEL_PGD_PTRS);
            // for (i = 0; i < PTRS_PER_PGD; i++) {
            //     if (unlikely(pgd_none(old_pgd[i])))
            //         continue;

            //     new_pud = pud_alloc(&init_mm, new_pgd)
            // }
        }
        /*
        * cpu_tlbstate.loaded_mm should match CR3 whenever interrupts
        * are on.  This means that it may not match current->active_mm,
        * which will contain the previous user mm when we're in lazy TLB
        * mode even if we've already switched back to swapper_pg_dir.
        *
        * During switch_mm_irqs_off(), loaded_mm will be set to
        * LOADED_MM_SWITCHING during the brief interrupts-off window
        * when CR3 and loaded_mm would otherwise be inconsistent.  This
        * is for nmi_uaccess_okay()'s benefit.
        */
        // source_pgdir = new_pgd;
        // load_new_mm_cr3(new_pgd, target_asid, false);
    }

    pr_info("PKS module %s setup!\n", mod->name);
    
    return gate_id;
}
#else /* !CONFIG_ASID_SWITCH4PKS */
unsigned long pks_module_setup(struct module *mod) 
{
    int pkey;
    u32 pkrs;
    unsigned long gate_id;

    pkey = pks_new_key();
    pkrs = PKEY2PKRS(pkey);
    pkrs_default = pkrs | (PKEY_DISABLE_WRITE << (pkey * PKR_BITS_PER_PKEY));
    //update table->entries[1].target_pkrs in register_switch_gate(), must before pks_switch during set_memory_pks
    __this_cpu_write(pkrs_cache, pkrs_default);
    gate_id = register_switch_gate(sgtable, pkrs_default, pkrs, 0, 0);
    set_memory_pks((unsigned long)mod->init_layout.base, mod->init_layout.size >> PAGE_SHIFT, pkey);
	set_memory_pks((unsigned long)mod->core_layout.base, mod->core_layout.size >> PAGE_SHIFT, pkey);
    if (strcmp(mod->name, "dm_zero") == 0) {
        dm_zero_entry_gid = gate_id;
        dm_zero_exit_gid = gate_id + 1;
    } else if (strcmp(mod->name, "ipv6") == 0) {
        ipv6_entry_gid = gate_id;
        ipv6_exit_gid = gate_id + 1;
    }

    pr_info("PKS module %s setup!\n", mod->name);
    
    return gate_id;
}
#endif /* CONFIG_ASID_SWITCH4PKS */

int pks_module_insert(const char *mod_name, struct module *mod)
{
    pr_info("PKS module %s insert done!\n", mod->name);
    return 0;
}

#endif /* CONFIG_ARCH_ENABLE_SUPERVISOR_PKEYS */