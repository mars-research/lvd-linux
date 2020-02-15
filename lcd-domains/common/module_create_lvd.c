/*
 * module_load.c
 *
 * For creating an empty LCD and loading a kernel
 * module in it.
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <linux/module.h>
#include <linux/kallsyms.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <libcap.h>
#include <liblcd/liblcd.h>
#include <lcd_domains/microkernel.h>
#include <asm/desc.h>
#include <linux/percpu.h>
#include <linux/perf_event.h>
#include <asm/perf_event.h>

#include <lcd_config/post_hook.h>

#define LVD_PERCPU_STACK
#define LVD_PERCPU_VMFUNC_STATE

/* MEMORY REGIONS -------------------------------------------------- */

/*
 * This assumes the PAT has Writeback in PAT0, Uncacheable in PAT1.
 *
 * See Intel SDM V3 11.12.3 for how PAT indexing is done.
 */
#define LCD_UNCACHEABLE_FLAGS (_PAGE_PRESENT | _PAGE_RW | _PAGE_PWT)
#define LCD_WRITEBACK_FLAGS (_PAGE_PRESENT | _PAGE_RW)

#define CHECK_NULL(offset, addr)	({		\
		int __isnull = 0;			\
		do { 					\
			if ((offset) == (unsigned long)(addr)) {	\
				printk("%s:%d Pointer likely null (%lx) for %s\n", __func__, __LINE__, (unsigned long)(addr), #addr); \
				__isnull = 1; 		\
			}				\
		} while(0);				\
		__isnull;				\
		})

int vmfunc_load_debug = 0;

struct lcd_mem_region {
	const char *name;
	unsigned long offset;
	unsigned long size;
	pteval_t flags;
};

extern union vmfunc_state_page vmfunc_state_page;

#define LCD_NR_MEM_REGIONS 7

static struct lcd_mem_region lcd_mem_regions[LCD_NR_MEM_REGIONS] = {
	{
		"miscellaneous",
		.offset = LCD_MISC_REGION_OFFSET,
		.size =   LCD_MISC_REGION_SIZE,
		.flags =  LCD_WRITEBACK_FLAGS,
	},
	{
		"stack",
		.offset = LCD_STACK_REGION_OFFSET,
		.size =   LCD_STACK_REGION_SIZE,
		.flags =  LCD_WRITEBACK_FLAGS,
	},
	{
		"heap",
		.offset = LCD_HEAP_REGION_OFFSET,
		.size =   LCD_HEAP_REGION_SIZE,
		.flags =  LCD_WRITEBACK_FLAGS,
	},
	{
		"direct-heap",
		.offset = LCD_DIRECT_HEAP_REGION_OFFSET,
		.size =   LCD_DIRECT_HEAP_REGION_SIZE,
		.flags =  LCD_WRITEBACK_FLAGS,
	},
	{
		"ram map",
		.offset = LCD_RAM_MAP_REGION_OFFSET,
		.size =   LCD_RAM_MAP_REGION_SIZE,
		.flags =  LCD_WRITEBACK_FLAGS,
	},
	{
		"ioremap",
		.offset = LCD_IOREMAP_REGION_OFFSET,
		.size =   LCD_IOREMAP_REGION_SIZE,
		.flags =  LCD_UNCACHEABLE_FLAGS,
	},
	{
		"kernel module",
		.offset = LCD_KERNEL_MODULE_REGION_OFFSET,
		.size =   LCD_KERNEL_MODULE_REGION_SIZE,
		.flags =  LCD_WRITEBACK_FLAGS,
	},
};

/*
 * ideally this page has to be LCD specific, but the interface of how to access
 * it from KLCD (probably by sending in some sort of LCD id) has to be thought
 * upon. So for now, let it remaina global per-cpu data structure
 */
DEFINE_PER_CPU_PAGE_ALIGNED(struct ext_registers, register_pages);
EXPORT_PER_CPU_SYMBOL(register_pages);

/* PHYSICAL ADDRESS SPACE -------------------------------------------------- */

static int do_grant_and_map_for_mem(cptr_t lcd, struct lcd_create_ctx *ctx,
				void *mem, gpa_t map_base,
				cptr_t *dest)
{
	int ret;
	cptr_t mo;
	unsigned long size, offset;
	/*
	 * Look up the cptr for the *creator*
	 */
	ret = lcd_virt_to_cptr(__gva((unsigned long)mem), &mo, &size,
			&offset);
	if (ret) {
		LIBLCD_ERR("lookup failed");
		goto fail1;
	}
	/*
	 * Alloc a cptr in the new LCD cptr cache
	 */
	ret = cptr_alloc(lcd_to_boot_cptr_cache(ctx), dest);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail2;
	}
	/*
	 * Grant and map the memory
	 */
	ret = lcd_memory_grant_and_map(lcd, mo, *dest, map_base);
	if (ret) {
		LIBLCD_ERR("grant and map failed");
		goto fail3;
	}

	return 0;

fail3:
	cptr_free(lcd_to_boot_cptr_cache(ctx), *dest);
fail2:
fail1:
	return ret;
}

static int do_grant_and_map_for_mem_hpa(cptr_t lcd, struct lcd_create_ctx *ctx,
				void *mem, gpa_t map_base, hpa_t hpa_base,
				cptr_t *dest)
{
	int ret;
	cptr_t mo;
	unsigned long size, offset;
	/*
	 * Look up the cptr for the *creator*
	 */
	ret = lcd_virt_to_cptr(__gva((unsigned long)mem), &mo, &size,
			&offset);
	if (ret) {
		LIBLCD_ERR("lookup failed");
		goto fail1;
	}
	/*
	 * Alloc a cptr in the new LCD cptr cache
	 */
	ret = cptr_alloc(lcd_to_boot_cptr_cache(ctx), dest);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail2;
	}
	/*
	 * Grant and map the memory
	 */
	ret = lcd_memory_grant_and_map_hpa(lcd, mo, *dest, map_base, hpa_base);
	if (ret) {
		LIBLCD_ERR("grant and map failed");
		goto fail3;
	}

	return 0;

fail3:
	cptr_free(lcd_to_boot_cptr_cache(ctx), *dest);
fail2:
fail1:
	return ret;
}

#if 0
static int do_grant_and_map_for_mem_cpu(cptr_t lcd, struct lcd_create_ctx *ctx,
				void *mem, gpa_t map_base,
				cptr_t *dest, int cpu)
{
	int ret;
	cptr_t mo;
	unsigned long size, offset;
	/*
	 * Look up the cptr for the *creator*
	 */
	ret = lcd_virt_to_cptr(__gva((unsigned long)mem), &mo, &size,
			&offset);
	if (ret) {
		LIBLCD_ERR("lookup failed");
		goto fail1;
	}
	/*
	 * Alloc a cptr in the new LCD cptr cache
	 */
	ret = cptr_alloc(lcd_to_boot_cptr_cache(ctx), dest);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail2;
	}
	/*
	 * Grant and map the memory
	 */
	ret = lcd_memory_grant_and_map_cpu(lcd, mo, *dest, map_base, cpu);
	if (ret) {
		LIBLCD_ERR("grant and map failed");
		goto fail3;
	}

	return 0;

fail3:
	cptr_free(lcd_to_boot_cptr_cache(ctx), *dest);
fail2:
fail1:
	return ret;
}
#endif

#ifdef LVD_PERCPU_VMFUNC_STATE
static int do_grant_and_map_for_mem_percpu(cptr_t lcd, struct lcd_create_ctx *ctx,
				void *mem, gpa_t map_base,
				cptr_t *dest, int cpu)
{
	int ret;
	cptr_t mo;
	unsigned long size, offset;
	/*
	 * Look up the cptr for the *creator*
	 */
	ret = lcd_virt_to_cptr(__gva((unsigned long)mem), &mo, &size,
			&offset);
	if (ret) {
		LIBLCD_ERR("lookup failed");
		goto fail1;
	}
	/*
	 * Alloc a cptr in the new LCD cptr cache
	 */
	ret = cptr_alloc(lcd_to_boot_cptr_cache(ctx), dest);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail2;
	}
	/*
	 * Grant and map the memory
	 */
	ret = lcd_memory_grant_and_map_percpu(lcd, mo, *dest, map_base, cpu);
	if (ret) {
		LIBLCD_ERR("grant and map failed");
		goto fail3;
	}

	return 0;

fail3:
	cptr_free(lcd_to_boot_cptr_cache(ctx), *dest);
fail2:
fail1:
	return ret;
}
#endif

static int do_grant_map_percpu_stack_pools(struct lcd_create_ctx *ctx, cptr_t lcd)
{
	struct lcd *lcd_struct;
	struct cnode *lcd_cnode;
	int ret;
	int cpu;

	ret = __lcd_get(current->lcd, lcd, &lcd_cnode, &lcd_struct);
	if (ret)
		goto fail1;
	__lcd_put(current->lcd, lcd_cnode, lcd_struct);

	for_each_online_cpu(cpu) {
		struct lcd_stack *this_stack = per_cpu_ptr(lcd_struct->lcd_stacks, cpu);
		void **stack_pools = this_stack->stacks;
		int i;
		cptr_t c;
		unsigned long gpa_stack = gpa_val(LCD_STACK_GP_ADDR) - (cpu * (NUM_STACKS_PER_CPU + 1) * LCD_STACK_SIZE);

		/*
		 * Since, we have percpu EPTs, let's map the pool of stacks on
		 * the same GPA on all EPTs. The pool will span from
		 * LCD_STACK_SIZE to NUM_STACKS_PER_CPU * LCD_STACK_SIZE
		 */
		for (i = 0; i < NUM_STACKS_PER_CPU; i++) {
			unsigned long _gpa_stack = gpa_stack - (i * LCD_STACK_SIZE);

			LIBLCD_MSG("mapping stack pages on cpu:%d stack: %d gpa: %lx hpa: %p",
					cpu,
					i,
					_gpa_stack,
					stack_pools[i]);
			ret = do_grant_and_map_for_mem(lcd, ctx, stack_pools[i],
					__gpa(_gpa_stack), &c);
		}
	}


fail1:
	return ret;
}

static int do_kernel_module_grant_map(cptr_t lcd, struct lcd_create_ctx *ctx,
				gva_t m_init_link_addr, gva_t m_core_link_addr,
				gva_t m_vmfunc_tr_page_addr,
				gva_t m_vmfunc_sb_page_addr,
				unsigned long m_init_size,
				unsigned long m_core_size,
				unsigned long m_vmfunc_tr_page_size,
				unsigned long m_vmfunc_sb_page_size
				)
{
	unsigned long offset;
	cptr_t *c;
	int ret;
	/*
	 * Map module init and core at correct offsets
	 *
	 * XXX: Assumes init and core link addresses come from
	 * the host's module mem area.
	 * (LCD_KERNEL_MODULE_REGION_GV_ADDR =
	 */
	offset = gva_val(m_init_link_addr) -
		gva_val(LCD_KERNEL_MODULE_REGION_GV_ADDR);
	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.module_init);
	LIBLCD_MSG("grant mem for init pages ctx %p | init_bits %lx | offset %lx, pa: %p",
			ctx, ctx->m_init_bits, offset, page_address(vmalloc_to_page(ctx->m_init_bits)));
	ret = do_grant_and_map_for_mem(lcd, ctx, ctx->m_init_bits,
				gpa_add(LCD_KERNEL_MODULE_REGION_GP_ADDR,
					offset),
				c);
	if (ret)
		goto fail1;

	offset = gva_val(m_core_link_addr) -
		gva_val(LCD_KERNEL_MODULE_REGION_GV_ADDR);
	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.module_core);
	LIBLCD_MSG("grant mem for core pages ctx %p | core_bits %lx | offset %lx", ctx, ctx->m_core_bits, offset);
	ret = do_grant_and_map_for_mem(lcd, ctx, ctx->m_core_bits,
				gpa_add(LCD_KERNEL_MODULE_REGION_GP_ADDR,
					offset),
				c);
	if (ret)
		goto fail2;

	offset = gva_val(m_vmfunc_tr_page_addr) -
		gva_val(LCD_KERNEL_MODULE_REGION_GV_ADDR);
	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.vmfunc_tr_page);
	LIBLCD_MSG("grant mem for vmfunc_tr pages ctx %p | vmfunc_tr_bits %lx | offset %lx", ctx, ctx->m_vmfunc_tr_bits, offset);
	ret = do_grant_and_map_for_mem(lcd, ctx, ctx->m_vmfunc_tr_bits,
				gpa_add(LCD_KERNEL_MODULE_REGION_GP_ADDR,
					offset),
				c);
	if (ret)
		goto fail3;

	offset = gva_val(m_vmfunc_sb_page_addr) -
		gva_val(LCD_KERNEL_MODULE_REGION_GV_ADDR);
	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.vmfunc_sb_page);
	LIBLCD_MSG("grant mem for vmfunc_sb pages ctx %p | vmfunc_sb_bits %lx | offset %lx", ctx, ctx->m_vmfunc_sb_bits, offset);
	ret = do_grant_and_map_for_mem(lcd, ctx, ctx->m_vmfunc_sb_bits,
				gpa_add(LCD_KERNEL_MODULE_REGION_GP_ADDR,
					offset),
				c);
	if (ret)
		goto fail4;

	lcd_to_boot_info(ctx)->module_init_base = m_init_link_addr;
	lcd_to_boot_info(ctx)->module_core_base = m_core_link_addr;
	lcd_to_boot_info(ctx)->module_vmfunc_tr_base = m_vmfunc_tr_page_addr;
	lcd_to_boot_info(ctx)->module_vmfunc_tr_size = m_vmfunc_tr_page_size;
	lcd_to_boot_info(ctx)->module_vmfunc_sb_base = m_vmfunc_sb_page_addr;
	lcd_to_boot_info(ctx)->module_vmfunc_sb_size = m_vmfunc_sb_page_size;
	lcd_to_boot_info(ctx)->module_init_size = m_init_size;
	lcd_to_boot_info(ctx)->module_core_size = m_core_size;

fail4:
fail3:
fail2:
fail1:
	return ret;
}

static int do_kernel_pages_grant_map(cptr_t lcd, struct lcd_create_ctx *ctx)
{
	unsigned long offset;
	unsigned long entry_text_start, entry_text_end;
	unsigned long entry_text_start_page, entry_text_end_page;
	unsigned long page_count = 1;
	unsigned long idt_page;
	struct page *p;
	int ret;
	cptr_t *c;
	unsigned long idtr_base;
	unsigned char idt_ptr[10];

	asm volatile("sidt %[idt_ptr]"
			:[idt_ptr]"=m"(idt_ptr));

	idtr_base = *(unsigned long *)&idt_ptr[2];

	entry_text_start = kallsyms_lookup_name("__entry_text_start");
	entry_text_end = kallsyms_lookup_name("__entry_text_end");
	idt_page = kallsyms_lookup_name("idt_table");

	entry_text_start_page = entry_text_start & PAGE_MASK;
	entry_text_end_page = entry_text_end & PAGE_MASK;

	page_count += (entry_text_end_page - entry_text_start_page) >> PAGE_SHIFT;

	LIBLCD_MSG("entry_text_start %lx , entry_text_end %lx, "
			"entry_text_start_page %lx , entry_text_end_page %lx, page_count %lx",
			entry_text_start, entry_text_end,
			entry_text_start_page, entry_text_end_page, page_count);

	LIBLCD_MSG("idt_page %lx", idt_page);

	p = virt_to_head_page((void*) entry_text_start_page);

	/* map .entry.text pages */
	lcd_create_mo_metadata(p, page_count * PAGE_SIZE, LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE);

	p = virt_to_head_page((void*) idt_page);

	/* IDT would be just one page */
	lcd_create_mo_metadata(p, PAGE_SIZE, LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE);

	/* lookup idtr_base */
	p = virt_to_head_page((void*) idtr_base);

	/* IDTR would be just one page */
	lcd_create_mo_metadata(p, PAGE_SIZE, LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE);

	///
	/* lookup ds and pebs */
	p = virt_to_head_page((void*) idtr_base);

	/* IDTR would be just one page */
	lcd_create_mo_metadata(p, PAGE_SIZE, LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE);



	offset = entry_text_start_page -
		gva_val(LCD_KERNEL_MODULE_REGION_GV_ADDR);

	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.entry_text_pages);

	LIBLCD_MSG("grant mem for entry_text pages ctx %p | vaddr %lx | offset %lx",
			ctx, (void*) entry_text_start, offset);

	ret = do_grant_and_map_for_mem(lcd, ctx, (void*) entry_text_start,
				gpa_add(LCD_KERNEL_MODULE_REGION_GP_ADDR,
					offset),
				c);
	if (ret)
		goto fail;

	offset = idt_page -
		gva_val(LCD_KERNEL_MODULE_REGION_GV_ADDR);

	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.idt_page);

	LIBLCD_MSG("grant mem for entry_text pages ctx %p | vaddr %lx | offset %lx",
			ctx, (void*) idt_page, offset);

	ret = do_grant_and_map_for_mem(lcd, ctx, (void*) idt_page,
				gpa_add(LCD_KERNEL_MODULE_REGION_GP_ADDR,
					offset),
				c);
	if (ret)
		goto fail;

	offset = idtr_base -
		gva_val(LCD_KERNEL_MODULE_REGION_GV_ADDR);

	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.idtr_base);

	LIBLCD_MSG("grant mem for idtr_base ctx %p | vaddr %lx | offset %lx",
			ctx, (void*) idtr_base, offset);

	ret = do_grant_and_map_for_mem_hpa(lcd, ctx, (void*) idtr_base,
				gpa_add(LCD_KERNEL_MODULE_REGION_GP_ADDR,
					offset),
				__hpa(__pa_symbol((void*)idt_page)),
				c);
	if (ret)
		goto fail;

#ifndef LVD_PERCPU_VMFUNC_STATE
	offset = (u64) &vmfunc_state_page -
		gva_val(LCD_KERNEL_MODULE_REGION_GV_ADDR);

	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.vmfunc_state_page);

	LIBLCD_MSG("grant mem for vmfunc_state_page ctx %p | vaddr %lx | offset %lx",
			ctx, ctx->vmfunc_state_page, offset);

	ret = do_grant_and_map_for_mem(lcd, ctx, ctx->vmfunc_state_page,
				gpa_add(LCD_KERNEL_MODULE_REGION_GP_ADDR,
					offset),
				c);
#endif
fail:
	return ret;
}

static void dump_virt_addr_space(struct lcd_create_ctx *ctx)
{
	int i;

	pgd_t *pgd = ctx->gv_pg_tables;

	printk("Pgtable root gva: %lx | gpa: %lx\n",
			gva_val(LCD_BOOTSTRAP_PAGE_TABLES_GV_ADDR),
			gpa_val(LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR));

	for (i = 0; i < 512; i++) {
		if (pgd[i].pgd)
			printk("PGD: idx = %d | gpa_val = %lx, gva_val = %lx\n",
					i,
					pgd[i].pgd & PAGE_MASK,
					gva_val(isolated_lcd_gpa2gva(__gpa(pgd[i].pgd & PAGE_MASK))));
	}
	pgd += 512;

	for (i = 0; i < 512; i++) {
		if (pgd[i].pgd)
			printk("PUD: idx = %d | gpa_val = %lx, gva_val = %lx\n",
					i,
					pgd[i].pgd & PAGE_MASK,
					gva_val(isolated_lcd_gpa2gva(__gpa(pgd[i].pgd & PAGE_MASK))));
	}

	pgd += (10 * 512);

	for (i = 0; i < 512; i++) {
		if (pgd[i].pgd)
			printk("PMD: idx = %d | gpa_val = %lx, gva_val = %lx\n",
					i,
					pgd[i].pgd & PAGE_MASK,
					gva_val(isolated_lcd_gpa2gva(__gpa(pgd[i].pgd & PAGE_MASK))));
	}
}


#ifndef PTX_MASK
#define PTX_MASK                0x1FF
#endif

#define PGD_INDEX_P(addr)               (((addr) >> PGDIR_SHIFT) & PTX_MASK)
#define PUD_INDEX_P(addr)               (((addr) >> PUD_SHIFT) & PTX_MASK)
#define PMD_INDEX_P(addr)               (((addr) >> PMD_SHIFT) & PTX_MASK)

#define PAGE_PA_MASK            (0xFFFFFFFFFULL << PAGE_SHIFT)
#define PAGE_PA(page)           ((page) & PAGE_PA_MASK)

static inline u64 *get_hva_pgtable_addr(struct lcd_create_ctx *ctx, u64 *pte)
{
	unsigned long offset;

        if (!*pte)
                return 0;

	offset = (PAGE_PA(*pte) - gpa_val(LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR));

	return (u64*)(ctx->gv_pg_tables + offset);
}

static inline u64 *pgd_root(struct lcd_create_ctx *ctx)
{
	return (u64*) ctx->gv_pg_tables;
}

static inline u64 get_free_pgtable_page_offset(struct lcd_create_ctx *ctx)
{
	u64 new_page;
	u64 offset;
	/*
	 * We use 12 pages for mapping our 512 GB address space of LCDs. Refer
	 * to address_spaces.h file. We actively use only 10 GB
	 * - utcb, stack, heap and ioremap - 1G each
	 * - rammap - 4G
	 * - .ko - 2G
	 * Since we use 2MB pages, a page can map 1G of vaddr space.  We need
	 * 10 pages for mapping the above 10G vaddr range + 1 page for
	 * pgdir_root + 1 for pud
	 */
	if (!ctx->last_pgtable_page)
		ctx->last_pgtable_page = ctx->gv_pg_tables + (11 * PAGE_SIZE);

	new_page = (u64) ctx->last_pgtable_page + PAGE_SIZE;

	ctx->last_pgtable_page = (void *)new_page;

	offset = new_page - (u64)ctx->gv_pg_tables;

	return offset;
}

static inline gpa_t get_free_pgtable_page_gpa(struct lcd_create_ctx *ctx)
{
	u64 offset = get_free_pgtable_page_offset(ctx);

	return gpa_add(LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR, offset);
}

static inline void *get_free_pgtable_page_hva(struct lcd_create_ctx *ctx, gpa_t gp_addr)
{
	u64 offset = gpa_val(gp_addr) - gpa_val(LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR);

	if ((offset >> PAGE_SHIFT) > 16) {
		LIBLCD_ERR("%s, we have allocated only 16 pages for page table pages!");
	}

	/* return the hva of the PT page to construct page tables */
	return ctx->gv_pg_tables + offset;
}

static inline u64 *get_page_addr(u64 *pte) {

	if (!*pte)
		return 0;

	return (u64*)PAGE_PA(*pte);
}

static inline u64 walk_lvd_pgtable(void *addr, struct lcd_create_ctx *ctx)
{
	/* we need to map a 2MB page */
	u64 *_pgd_root = pgd_root(ctx);
	u64 pgd_idx;
	u64 pud_idx;
	u64 pmd_idx;
	u64 *pud_page;
	u64 *pmd_page;
	u64 *pte_page;
	u64 *hva_pud_page;
	u64 *hva_pmd_page;
	u64 *hva_pte_page;
	u64 gva = (u64) addr;

	pgd_idx = PGD_INDEX_P(gva);
	pud_page = &_pgd_root[pgd_idx];
	hva_pud_page = get_hva_pgtable_addr(ctx, pud_page);

	printk("%s: PGD_ROOT %p, ctx->gv_pg_tables %p\n", __func__, _pgd_root, ctx->gv_pg_tables);

	printk("%s: PGD gva: %llx, pgd_idx %llu, pud_page %llx | hva_pud_page %p\n",
			__func__, gva, pgd_idx, PAGE_PA(*pud_page), hva_pud_page);

	pud_page = get_page_addr(pud_page);

	/* no PUD */
	if (!pud_page) {
		return 0;
	}

	pud_idx = PUD_INDEX_P(gva);
	pmd_page = &hva_pud_page[pud_idx];
	hva_pmd_page = get_hva_pgtable_addr(ctx, pmd_page);

	printk("%s: PUD gva: %llx, pud_idx %llu, pmd_page %llx | hva_pmd_page %p\n",
			__func__, gva, pud_idx, PAGE_PA(*pmd_page), hva_pmd_page);

	pmd_page = get_page_addr(pmd_page);

	if (!pmd_page) {
		return 0;
	}

	pmd_idx = PMD_INDEX_P(gva);
	pte_page = &hva_pmd_page[pmd_idx];
	hva_pte_page = get_hva_pgtable_addr(ctx, pte_page);

	printk("%s: PMD gva: %llx, pmd_idx %llu, pte_page %llx | hva_pte_page %p\n",
			__func__, gva, pmd_idx, PAGE_PA(*pte_page), hva_pte_page);

	pte_page = get_page_addr(pte_page);

	if (!pte_page) {
		return 0;
	}

	pte_page = &hva_pmd_page[pmd_idx];

	return PAGE_PA(*pte_page);
}

static int map_cpu_page(void *addr, struct lcd_create_ctx *ctx)
{
	/* we need to map a 2MB page */
	u64 *_pgd_root = pgd_root(ctx);
	u64 pgd_idx;
	u64 pud_idx;
	u64 pmd_idx;
	u64 *pud_page;
	u64 *pmd_page;
	u64 *pte_page;
	u64 *hva_pud_page;
	u64 *hva_pmd_page;
	u64 *hva_pte_page;
	u64 gva = (u64) addr;

	pgd_idx = PGD_INDEX_P(gva);
	pud_page = &_pgd_root[pgd_idx];
	hva_pud_page = get_hva_pgtable_addr(ctx, pud_page);

	printk("%s: PGD_ROOT %p, ctx->gv_pg_tables %p\n", __func__, _pgd_root, ctx->gv_pg_tables);

	printk("%s: PGD gva: %llx, pgd_idx %llu, pud_page %llx | hva_pud_page %p\n",
			__func__, gva, pgd_idx, PAGE_PA(*pud_page), hva_pud_page);

	pud_page = get_page_addr(pud_page);

	/* no PUD */
	if (!pud_page) {
		gpa_t pud_gpa = get_free_pgtable_page_gpa(ctx);
		unsigned int flags = 0;
		pgd_t *pgd_entry;

		flags |= _PAGE_PRESENT;
		flags |= _PAGE_RW;

		hva_pud_page = get_free_pgtable_page_hva(ctx, pud_gpa);

		pgd_entry = (pgd_t*) &_pgd_root[pgd_idx];

		printk("%s, set pgd entry %llu to %lx\n", __func__, pgd_idx, gpa_val(pud_gpa));
		set_pgd(pgd_entry,
			__pgd(gpa_val(pud_gpa) | flags));
	}

	pud_idx = PUD_INDEX_P(gva);
	pmd_page = &hva_pud_page[pud_idx];
	hva_pmd_page = get_hva_pgtable_addr(ctx, pmd_page);

	printk("%s: PUD gva: %llx, pud_idx %llu, pmd_page %llx | hva_pmd_page %p\n",
			__func__, gva, pud_idx, PAGE_PA(*pmd_page), hva_pmd_page);

	pmd_page = get_page_addr(pmd_page);

	if (!pmd_page) {
		gpa_t pmd_gpa = get_free_pgtable_page_gpa(ctx);
		unsigned int flags = 0;
		pud_t *pud_entry;

		flags |= _PAGE_PRESENT;
		flags |= _PAGE_RW;

		hva_pmd_page = get_free_pgtable_page_hva(ctx, pmd_gpa);

		pud_entry = (pud_t*) &hva_pud_page[pud_idx];

		printk("%s, set pud entry for %llu to %lx at pgtable vaddr:%p | entry addr %p\n", __func__,
				pud_idx, gpa_val(pmd_gpa), hva_pud_page, pud_entry);

		set_pud(pud_entry, __pud(gpa_val(pmd_gpa) | flags));
	}

	pmd_idx = PMD_INDEX_P(gva);
	pte_page = &hva_pmd_page[pmd_idx];
	hva_pte_page = get_hva_pgtable_addr(ctx, pte_page);

	printk("%s: PMD gva: %llx, pmd_idx %llu, pte_page %llx | hva_pte_page %p\n",
			__func__, gva, pmd_idx, PAGE_PA(*pte_page), hva_pte_page);

	pte_page = get_page_addr(pte_page);

	if (!pte_page) {
		u64 gp;
		unsigned int flags = 0;

		flags |= _PAGE_PRESENT;
		flags |= _PAGE_RW;

		gp = ((((u64)addr) & HPAGE_MASK) - LCD_DIRECT_MAP_VIRT_BASE) + LCD_PHYS_DIRECT_MAP_REGION_OFFSET;
		//gp = gpa_val(isolated_lcd_gva2gpa(__gva((u64)addr & HPAGE_MASK)));
		printk("%s, set PTE gva: %llx, pmd_idx %llu, gp_addr %llx at vaddr:%p\n", __func__,
					gva, pmd_idx, gp, &hva_pmd_page[pmd_idx]);

		set_pmd((pmd_t*)&hva_pmd_page[pmd_idx], __pmd(gp | flags | _PAGE_PSE));
	}

	pmd_page = &hva_pmd_page[pud_idx];

	return PAGE_PA(*pmd_page);
}

/* size = exception_stack_size */
DECLARE_PER_CPU_PAGE_ALIGNED(char, exception_stacks
        [(N_EXCEPTION_STACKS - 1) * EXCEPTION_STKSZ + DEBUG_STKSZ]);

static int __do_ept_mapping(cptr_t lcd, void *addr, struct lcd_create_ctx *ctx, int size)
{
	unsigned long offset;
	cptr_t *c;
	int ret = 0;
	struct page *p;

	addr = (void*) ((u64) addr & PAGE_MASK);

	p = virt_to_head_page(addr);

	/* create volunteer metadata */
	lcd_create_mo_metadata(p, size, LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE);

	offset = ((u64) addr -
		gva_val(LCD_PHYS_DIRECT_MAP_GV_ADDR)) & PAGE_MASK;

	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.percpu_pages);

	LIBLCD_MSG("grant mem for per_cpu pages ctx %p | vaddr %lx |"
		       " offset %lx | gpa %lx",
			ctx, addr, offset, gpa_add(LCD_PHYS_DIRECT_MAP_GP_ADDR, offset));
	ret = do_grant_and_map_for_mem(lcd, ctx, addr,
				gpa_add(LCD_PHYS_DIRECT_MAP_GP_ADDR,
					offset),
				c);
	return ret;
}

static int do_misc_pages_grant_map(cptr_t lcd, struct lcd_create_ctx *ctx)
{
	/* tss is exported */
	int cpu;
	unsigned int * except_stack_sz = (unsigned int*) kallsyms_lookup_name("exception_stack_sizes");

	/* map per-cpu TSS */
	for_each_online_cpu(cpu) {
		struct tss_struct *tss = &per_cpu(cpu_tss, cpu);
		struct gdt_page *gdt = &per_cpu(gdt_page, cpu);
		char *ist_stacks = per_cpu(exception_stacks, cpu);
		int i, j;

		map_cpu_page(tss, ctx);
		__do_ept_mapping(lcd, tss, ctx, PAGE_SIZE);

		map_cpu_page(gdt, ctx);
		__do_ept_mapping(lcd, gdt, ctx, PAGE_SIZE);

		for (i = 0; i < N_EXCEPTION_STACKS; i++) {
			unsigned int num_pages = except_stack_sz[i] >> PAGE_SHIFT;

			for (j = 0; j < num_pages; j++)
				map_cpu_page(ist_stacks + j * PAGE_SIZE, ctx);

			__do_ept_mapping(lcd, ist_stacks, ctx, except_stack_sz[i]);

			ist_stacks += except_stack_sz[i];
		}

	}

	for_each_possible_cpu(cpu) {
		/* lookup idtr_base */
		char *ds = (char*) per_cpu(lvd_cpu_hw_events_ds, cpu);
		char *pebs =  (char*) per_cpu(lvd_cpu_hw_events_pebs, cpu);

		LIBLCD_MSG("grant mem for cpu_hw_events.ds %p", per_cpu(lvd_cpu_hw_events_ds, cpu));

		map_cpu_page(ds, ctx);
		__do_ept_mapping(lcd, ds, ctx, PAGE_SIZE);

		LIBLCD_MSG("grant mem for cpu_hw_events.pebs %p", per_cpu(lvd_cpu_hw_events_pebs, cpu));

		map_cpu_page(pebs, ctx);
		__do_ept_mapping(lcd, pebs, ctx, PAGE_SIZE);

	};


	return 0;
}

#ifdef LVD_PERCPU_VMFUNC_STATE
static int __do_ept_mapping_vmfunc_state(cptr_t lcd, void *addr, struct lcd_create_ctx *ctx,
					int size, int cpu)
{
	unsigned long offset;
	cptr_t c;
	int ret = 0;
	struct page *p;
	gpa_t gpa_addr;

	addr = (void*) ((u64) addr & PAGE_MASK);

	p = virt_to_page(addr);

	/* create volunteer metadata */
	lcd_create_mo_metadata(p, size, LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE);

	offset = (u64) &vmfunc_state_page -
			gva_val(LCD_KERNEL_MODULE_REGION_GV_ADDR);

	gpa_addr = gpa_add(LCD_KERNEL_MODULE_REGION_GP_ADDR, offset);

	//c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.vmfunc_state_page);

	LIBLCD_MSG("grant mem for vmfunc_state_page ctx %p | vaddr %lx | offset %lx",
			ctx, addr, gpa_addr);

	ret = do_grant_and_map_for_mem_percpu(lcd, ctx, addr, gpa_addr, &c, cpu);

	return ret;
}

static int do_grant_map_percpu_vmfunc_state_page(struct lcd_create_ctx *ctx, cptr_t lcd)
{
	int ret = 0;
	int cpu;

	//map_cpu_page(&vmfunc_state_page, ctx);
	walk_lvd_pgtable((void *)&vmfunc_state_page, ctx);
	for_each_online_cpu(cpu) {
		union vmfunc_state_page *this_state = per_cpu_ptr(ctx->vmfunc_state_page, cpu);

		/*
		 * vmfunc_state_page will be mapped on the same GVA for all
		 * cpus. The HPA mapping would be different for different CPUS.
		 */
		printk("%s, mapping vaddr: %p size: %lu\n", __func__, this_state, PAGE_SIZE);
		ret = __do_ept_mapping_vmfunc_state(lcd, this_state, ctx, PAGE_SIZE, cpu);
		if (ret)
			goto fail;
	}
fail:
	return ret;
}
#endif

static int do_grant_and_map_for_mem_cpu_percpu(cptr_t lcd, struct lcd_create_ctx *ctx,
				void *mem, gpa_t map_base,
				cptr_t *dest, int cpu)
{
	int ret;
	cptr_t mo;
	unsigned long size, offset;
	/*
	 * Look up the cptr for the *creator*
	 */
	ret = lcd_virt_to_cptr(__gva((unsigned long)mem), &mo, &size,
			&offset);
	if (ret) {
		LIBLCD_ERR("lookup failed");
		goto fail1;
	}
	/*
	 * Alloc a cptr in the new LCD cptr cache
	 */
	ret = cptr_alloc(lcd_to_boot_cptr_cache(ctx), dest);
	if (ret) {
		LIBLCD_ERR("cptr alloc failed");
		goto fail2;
	}
	/*
	 * Grant and map the memory
	 */
	ret = lcd_memory_grant_and_map_cpu(lcd, mo, *dest, map_base, cpu);
	if (ret) {
		LIBLCD_ERR("grant and map failed");
		goto fail3;
	}

	return 0;

fail3:
	cptr_free(lcd_to_boot_cptr_cache(ctx), *dest);
fail2:
fail1:
	return ret;
}

static int __do_ept_mapping_register_page(cptr_t lcd, void *addr,
		struct lcd_create_ctx *ctx, int size, int cpu)
{
	cptr_t c;
	int ret = 0;
	struct page *p;
	gpa_t gpa_addr;

	addr = (void*) ((u64) addr & PAGE_MASK);

	p = virt_to_page(addr);

	printk("%s, vaddr: %p page: %p\n", __func__, addr, p);
	/* create volunteer metadata */
	lcd_create_mo_metadata(p, size, LCD_MICROKERNEL_TYPE_ID_VOLUNTEERED_PAGE);

	gpa_addr = LCD_REGISTER_PAGE_GP_ADDR;

	LIBLCD_MSG("grant mem for register_page ctx %p | vaddr %lx | offset %lx",
			ctx, addr, gpa_addr);

	ret = do_grant_and_map_for_mem_cpu_percpu(lcd, ctx, addr, gpa_addr, &c, cpu);

	return ret;
}

static int do_grant_map_percpu_register_page(struct lcd_create_ctx *ctx, cptr_t lcd)
{
	int ret = 0;
	int cpu;

	for_each_online_cpu(cpu) {
		struct ext_registers *this_register_page =
			&per_cpu(register_pages, cpu);
		/*
		 * register_page will be mapped on the same GVA for all
		 * cpus. The HPA mapping would be different for different CPUS.
		 */
		printk("%s, mapping vaddr: %p size: %lu\n", __func__,
				this_register_page, PAGE_SIZE);
		ret = __do_ept_mapping_register_page(lcd, this_register_page,
				ctx, PAGE_SIZE, cpu);
		if (ret)
			goto fail;
	}
fail:
	return ret;
}

static int setup_phys_addr_space(cptr_t lcd, struct lcd_create_ctx *ctx,
				gva_t m_init_link_addr, gva_t m_core_link_addr,
				gva_t m_vmfunc_tr_page_addr,
				gva_t m_vmfunc_sb_page_addr,
				unsigned long m_init_size,
				unsigned long m_core_size,
				unsigned long m_vmfunc_tr_page_size,
				unsigned long m_vmfunc_sb_page_size
				)
{
	int ret;
	cptr_t *c;
	/*
	 * Map and grant bootstrap pages, page tables, and stack
	 */
	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.boot_pages);
	LIBLCD_MSG("mapping bootstrap pages gpa: %lx hva: %p",
			gpa_val(LCD_BOOTSTRAP_PAGES_GP_ADDR), ctx->lcd_boot_info);
	ret = do_grant_and_map_for_mem(lcd, ctx, ctx->lcd_boot_info,
				LCD_BOOTSTRAP_PAGES_GP_ADDR,
				c);
	if (ret)
		goto fail1;

	/* in vmfunc LCDS, there are no additional guest page tables for LCDs */
	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.gv);
	LIBLCD_MSG("mapping bootstrap pgtable pages gpa: %lx hva: %p",
			gpa_val(LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR), ctx->gv_pg_tables);
	ret = do_grant_and_map_for_mem(lcd, ctx, ctx->gv_pg_tables,
				LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR, c);
	if (ret)
		goto fail2;

#ifdef LVD_PERCPU_STACK
	do_grant_map_percpu_stack_pools(ctx, lcd);
#else
	c = &(lcd_to_boot_info(ctx)->lcd_boot_cptrs.stack);
	LIBLCD_MSG("mapping stack pages gpa: %lx hva: %p",
			gpa_val(LCD_STACK_GP_ADDR), ctx->stack);
	ret = do_grant_and_map_for_mem(lcd, ctx, ctx->stack,
				LCD_STACK_GP_ADDR,
				c);
#endif
	if (ret)
		goto fail3;
	/*
	 * Map and grant kernel module
	 */
	ret = do_kernel_module_grant_map(lcd, ctx,
					m_init_link_addr, m_core_link_addr,
					m_vmfunc_tr_page_addr,
					m_vmfunc_sb_page_addr,
					m_init_size, m_core_size,
					m_vmfunc_tr_page_size,
					m_vmfunc_sb_page_size
					);
	if (ret)
		goto fail4;

	ret = do_kernel_pages_grant_map(lcd, ctx);

	if (ret)
		goto fail5;

	ret = do_misc_pages_grant_map(lcd, ctx);

	if (ret)
		goto fail6;

#ifdef LVD_PERCPU_VMFUNC_STATE
	ret = do_grant_map_percpu_vmfunc_state_page(ctx, lcd);
#endif
	if (ret)
		goto fail7;

	ret = do_grant_map_percpu_register_page(ctx, lcd);

	if (ret)
		goto fail8;
	return 0;
fail8:
fail7:
fail6:
fail5:
fail4:  /* Just return; caller should kill new LCD and free up resources. */
fail3:
fail2:
fail1:
	return ret;
}

/* VIRTUAL ADDRESS SPACE ------------------------------ */
static void setup_lcd_pmd(struct lcd_mem_region *reg, pmd_t *pmd,
			unsigned int gigabyte_idx, bool verbose)
{
	unsigned int k;
	unsigned long gp;
	unsigned long flags = 0;

	for (k = 0; k < 512; k++) {
		/*
		 * Guest physical address we put in the table entry is:
		 *
		 *    base +
		 *    offset to the 1GB region for this pmd +
		 *    offset for this table entry (some multiple of 2MBs)
		 */
		gp = LCD_PHYS_BASE + gigabyte_idx * (1UL << 30) +
			k * (1UL << 21);

		/* if the region is MISC region */
		if (reg->offset == LCD_MISC_REGION_OFFSET) {
			/*
			 * Make the 2nd 2MB page read only. These are the
			 * pagetable pages
			 */
			if (k == 2) {
				flags = _PAGE_PRESENT;
			} else {
				flags = reg->flags;
			}
			verbose = 1;
		} else {
			flags = reg->flags;
			verbose = 0;
		}

		if (verbose)
			printk("%s, k:%d, gbyte_idx: %x, gp: %lx at pmd[%d]: %p\n",\
					__func__, k, gigabyte_idx, gp, k, &pmd[k]);

		set_pmd(&pmd[k], __pmd(gp | flags | _PAGE_PSE));
	}
}

static void setup_lcd_pmds(pmd_t *pmds)
{
	unsigned int i, j, lo, hi;
	pmd_t *pmd_entry;
	struct lcd_mem_region *reg;
	/*
	 * Map each memory region, filling out entire pmd's so that we're
	 * mapping multiples of 1GB
	 */
	pmd_entry = pmds;
	for (i = 0; i < LCD_NR_MEM_REGIONS; i++) {
		reg = &lcd_mem_regions[i];
		/*
		 * Set up enough pmd's to fill in memory region
		 */
		lo = reg->offset >> 30;
		hi = lo + (reg->size >> 30);
		for (j = lo; j < hi; j++) {
			if (i == LCD_NR_MEM_REGIONS - 1)
				setup_lcd_pmd(reg, pmd_entry, j, true);
			else
				setup_lcd_pmd(reg, pmd_entry, j, false);
			pmd_entry += 512;
		}
	}

}

static void setup_lcd_pud(pud_t *pud)
{
	unsigned int i, j, lo, hi;
	pud_t *pud_entry;
	struct lcd_mem_region *reg;
	gpa_t pmd_gpa;
	/*
	 * pmd's come after pgd and pud
	 */
	pmd_gpa = gpa_add(LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR, 2 * PAGE_SIZE);
	/*
	 * Point to correct pmd's for each memory region
	 */
	for (i = 0; i < LCD_NR_MEM_REGIONS; i++) {
		reg = &lcd_mem_regions[i];
		/*
		 * Point to correct pmd's
		 */
		//lo = (reg->offset - LCD_TOP_512GB_REGION_OFFSET) >> 30;
		lo = reg->offset >> 30;
		hi = lo + (reg->size >> 30);
		if (1)
			printk("%s, region %u, lo : %u, hi : %u\n",
					__func__, i, lo, hi);
		for (j = lo; j < hi; j++) {
			pud_entry = &pud[j];
			set_pud(pud_entry,
				__pud(gpa_val(pmd_gpa) | reg->flags));
			pmd_gpa = gpa_add(pmd_gpa, PAGE_SIZE);
		}
	}
}

static void setup_lcd_pgd(pgd_t *pgd)
{
	pgd_t *pgd_entry;
	gpa_t pud_gpa;
	pteval_t flags = 0;

	pgd_entry = &pgd[511]; /* only map last pud for high 512 GBs */

	/* pud comes after pgd */
	pud_gpa = gpa_add(LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR, PAGE_SIZE);

	flags |= _PAGE_PRESENT;
	flags |= _PAGE_RW;

	set_pgd(pgd_entry,
		__pgd(gpa_val(pud_gpa) | flags));
}

static void setup_virt_addr_space(struct lcd_create_ctx *ctx)
{
	/*
	 * XXX: Assumes 512 entries / table
	 */
	void *cursor = ctx->gv_pg_tables;
	/*
	 * page tables should already be zero'd out
	 *
	 * Set up root pgd
	 */
	setup_lcd_pgd(cursor);
	cursor += 512 * sizeof(pgd_t);
	/*
	 * Set up pud (only one for high 512 GBs)
	 * (skip over pgd)
	 */
	setup_lcd_pud(cursor);
	cursor += 512 * sizeof(pud_t);
	/*
	 * Set up pmd's (one for each 1GB region)
	 * (skip over pgd and pud)
	 */
	setup_lcd_pmds(cursor);
}

int lcd_arch_ept_gpa_to_hpa_cpu(struct lcd_arch *lcd, gpa_t ga, hpa_t *ha_out, bool verbose, int cpu);

static int setup_addr_spaces_lvd(cptr_t lcd, struct lcd_create_ctx *ctx,
			gva_t m_init_link_addr, gva_t m_core_link_addr,
			gva_t m_vmfunc_tr_page_addr,
			gva_t m_vmfunc_sb_page_addr,
			unsigned long m_init_size,
			unsigned long m_core_size,
			unsigned long m_vmfunc_tr_page_size,
			unsigned long m_vmfunc_sb_page_size
			)
{
	int ret;
	/*
	 * Set up physical address space (except UTCB - that's done
	 * via lcd_config)
	 */
	ret = setup_phys_addr_space(lcd, ctx, m_init_link_addr,
				m_core_link_addr,
				m_vmfunc_tr_page_addr,
				m_vmfunc_sb_page_addr,
				m_init_size, m_core_size,
				m_vmfunc_tr_page_size,
				m_vmfunc_sb_page_size
				);
	if (ret) {
		LIBLCD_ERR("error setting up phys addr space");
		goto fail1;
	}
	/*
	 * Set up virtual address space
	 */
	setup_virt_addr_space(ctx);

	dump_virt_addr_space(ctx);

	{
		struct lcd *lcd_struct;
		struct cnode *lcd_cnode;
		gpa_t ga;
		hpa_t hpa_out;
		u64 addr, offset, gva;
		unsigned char idt_ptr[10];
		unsigned long idtr_base;


		asm volatile("sidt %[idt_ptr]"
				:[idt_ptr]"=m"(idt_ptr));

		idtr_base = *(unsigned long *)&idt_ptr[2];

		ret = __lcd_get(current->lcd, lcd, &lcd_cnode, &lcd_struct);
		if (ret)
			goto fail1;

		LIBLCD_MSG("Walking kernel_module_region %lx", gva_val(LCD_KERNEL_MODULE_REGION_GV_ADDR));
		walk_lvd_pgtable((void *)gva_val(LCD_KERNEL_MODULE_REGION_GV_ADDR), ctx);

		LIBLCD_MSG("Walking kernel_module_region %lx", gva_val(gva_add(LCD_STACK_GV_ADDR, LCD_STACK_SIZE)) - 8);
		walk_lvd_pgtable((void *)gva_val(gva_add(LCD_STACK_GV_ADDR, LCD_STACK_SIZE)) - 8, ctx);

		LIBLCD_MSG("Walking m_vmfunc_tr_page_addr %lx", gva_val(m_vmfunc_tr_page_addr));
		offset = gva_val(m_vmfunc_tr_page_addr) & ~HPAGE_MASK;
		addr = walk_lvd_pgtable((void *)gva_val(m_vmfunc_tr_page_addr), ctx);
		ga = __gpa(addr + offset);
		ret = lcd_arch_ept_gpa_to_hpa_cpu(lcd_struct->lcd_arch, ga, &hpa_out, true, smp_processor_id());
		LIBLCD_MSG("ept walk gpa = %lx , hpa = %lx, ret = %d", gpa_val(ga), hpa_val(hpa_out), ret);

		//0xffffffffff57c000
		gva = idtr_base;
		LIBLCD_MSG("Walking idtr_base_addr %lx", gva);
		offset = gva & ~HPAGE_MASK;
		addr = walk_lvd_pgtable((void *)gva, ctx);
		ga = __gpa(addr + offset);
		ret = lcd_arch_ept_gpa_to_hpa_cpu(lcd_struct->lcd_arch, ga, &hpa_out, true, smp_processor_id());
		LIBLCD_MSG("ept walk gpa = %lx , hpa = %lx, ret = %d", gpa_val(ga), hpa_val(hpa_out), ret);

		__lcd_put(current->lcd, lcd_cnode, lcd_struct);
	}

	printk("LCD_KERNEL_MODULE_REGION_GP_ADDR = %lx | LCD_KERNEL_MODULE_REGION_GV_ADDR = %lx\n",
			gpa_val(LCD_KERNEL_MODULE_REGION_GP_ADDR),
			gva_val(LCD_KERNEL_MODULE_REGION_GV_ADDR));
	printk("LCD_IOREMAP_GP_ADDR = %lx | LCD_IOREMAP_GV_ADDR = %lx\n",
			gpa_val(LCD_IOREMAP_GP_ADDR),
			gva_val(LCD_IOREMAP_GV_ADDR));
	printk("LCD_RAM_MAP_GP_ADDR = %lx | LCD_RAM_MAP_GV_ADDR = %lx\n",
			gpa_val(LCD_RAM_MAP_GP_ADDR),
			gva_val(LCD_RAM_MAP_GV_ADDR));
	printk("LCD_HEAP_GP_ADDR = %lx | LCD_HEAP_GV_ADDR = %lx\n",
			gpa_val(LCD_HEAP_GP_ADDR),
			gva_val(LCD_HEAP_GV_ADDR));
	printk("LCD_STACK_GP_ADDR = %lx | LCD_STACK_GV_ADDR = %lx\n",
			gpa_val(LCD_STACK_GP_ADDR),
			gva_val(LCD_STACK_GV_ADDR));
	printk("LCD_UTCB_GP_ADDR = %lx | LCD_UTCB_GV_ADDR = %lx\n",
			gpa_val(LCD_UTCB_GP_ADDR),
			gva_val(LCD_UTCB_GV_ADDR));
	printk("LCD_PHYS_DIRECT_MAP_GP_ADDR = %lx | LCD_PHYS_DIRECT_MAP_GV_ADDR = %lx\n",
			gpa_val(LCD_PHYS_DIRECT_MAP_GP_ADDR),
			gva_val(LCD_PHYS_DIRECT_MAP_GV_ADDR));
	return 0;

fail1: /* just return non-zero ret; caller will free mem */
	return ret;
}

/* -------------------------------------------------- */

static int init_create_ctx(struct lcd_create_ctx **ctx_out, char *mname)
{
	struct lcd_create_ctx *ctx;
	int ret;
	/*
	 * Alloc ctx
	 */
	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx) {
		LIBLCD_ERR("kzalloc failed");
		ret = -ENOMEM;
		goto fail1;
	}

	strncpy(ctx->mname, mname, LCD_MODULE_NAME_MAX);

	*ctx_out = ctx;

	return 0;

fail1:
	return ret;
}

static void destroy_create_ctx(struct lcd_create_ctx *ctx)
{
	/*
	 * Remove pages from our address space and delete our
	 * capabilities to them. If these are the last caps,
	 * pages will be freed by microkernel.
	 */
	if (ctx->m_init_bits)
		lvd_release_module(ctx->m_init_bits, ctx->m_core_bits, ctx->m_vmfunc_tr_bits, ctx->m_vmfunc_sb_bits);
	if (ctx->stack)
		lcd_free_pages(lcd_virt_to_head_page(ctx->stack),
			LCD_STACK_ORDER);
	if (ctx->gv_pg_tables)
		lcd_free_pages(lcd_virt_to_head_page(ctx->gv_pg_tables),
			LCD_BOOTSTRAP_PAGE_TABLES_ORDER);
	if (ctx->lcd_boot_info)
		lcd_free_pages(lcd_virt_to_head_page(ctx->lcd_boot_info),
			LCD_BOOTSTRAP_PAGES_ORDER);
	/*
	 * Free the ctx
	 */
	kfree(ctx);
}

static int do_cptr_cache_init(struct cptr_cache *cache)
{
	int ret;
	cptr_t unused;

	ret = cptr_cache_init(cache);
	if (ret) {
		LIBLCD_ERR("error init'ing cptr cache");
		return ret;
	}
	/*
	 * Reserve first two slots for call/reply caps (just alloc them)
	 */
	ret = cptr_alloc(cache, &unused);
	if (ret) {
		LIBLCD_ERR("cptr cache alloc1");
		return ret;
	}
	ret = cptr_alloc(cache, &unused);
	if (ret) {
		LIBLCD_ERR("cptr cache alloc2");
		return ret;
	}

	return 0;
}

void *lcd_stack;
EXPORT_SYMBOL(lcd_stack);

void **lcd_stacks;
EXPORT_SYMBOL(lcd_stacks);

/*
 * For each cpu, create a bitmap and a pool of stacks
 */
int create_lvd_stack_pools(struct lcd_create_ctx *ctx, cptr_t lcd)
{
	struct lcd *lcd_struct;
	struct cnode *lcd_cnode;
	int ret;
	int cpu;

	ret = __lcd_get(current->lcd, lcd, &lcd_cnode, &lcd_struct);

	if (ret)
		goto fail1;

	__lcd_put(current->lcd, lcd_cnode, lcd_struct);

	lcd_struct->lcd_stacks = __alloc_percpu(sizeof(struct lcd_stack), 0);

	if (!lcd_struct->lcd_stacks) {
		LIBLCD_ERR("%s, alloc_percpu failed", __func__);
		goto fail1;
	}

	for_each_online_cpu(cpu) {
		struct lcd_stack *this_stack = per_cpu_ptr(lcd_struct->lcd_stacks, cpu);
		int i;

		this_stack->bitmap = ~0ll;
		this_stack->stacks = kzalloc(sizeof(void*) * NUM_STACKS_PER_CPU, GFP_KERNEL);
		this_stack->lazy_bitmap = 0ll;
		spin_lock_init(&this_stack->lazy_bm_lock);
		this_stack->lazy_updated = false;

		for (i = 0; i < NUM_STACKS_PER_CPU; i++) {
			struct page *p;
			p = lcd_alloc_pages(0, LCD_STACK_ORDER);
			if (!p) {
				LIBLCD_ERR("%s, stack page allocation failed", __func__);
				goto fail2;
			}
			this_stack->stacks[i] = lcd_page_address(p);
		}
	}

fail2:
fail1:
	return ret;
}

struct ext_registers *get_register_page(int cpu) {
	return &per_cpu(register_pages, cpu);
}
EXPORT_SYMBOL(get_register_page);
/*
 * For each cpu, create a register page for passing more arguments
 */
int create_lvd_register_page(struct lcd_create_ctx *ctx, cptr_t lcd)
{
	int ret = 0;
	int cpu;

	for_each_online_cpu(cpu) {
		struct ext_registers *this_reg_page = &per_cpu(register_pages, cpu);
		memset(this_reg_page, 0, sizeof(struct ext_registers));
	}

	return ret;
}

/*
 * For each cpu, create a vmfunc_state_page and populate cpuid
 */
int create_lvd_vmfunc_state_page(struct lcd_create_ctx *ctx)
{
	int ret = 0;
	int cpu;

	ctx->vmfunc_state_page = __alloc_percpu(sizeof(union vmfunc_state_page), PAGE_SIZE);

	if (!ctx->vmfunc_state_page) {
		LIBLCD_ERR("%s, alloc_percpu failed", __func__);
		ret = -ENOMEM;
		goto fail1;
	}

	for_each_online_cpu(cpu) {
		union vmfunc_state_page *this_state = per_cpu_ptr(ctx->vmfunc_state_page, cpu);
		memset(this_state, 0, sizeof(union vmfunc_state_page));
		printk("%s, cpu: %d state_page: %p __pa: %llx\n", __func__, cpu, this_state, per_cpu_ptr_to_phys(this_state));
		this_state->vmfunc_state.cpuid = cpu;
	}

fail1:
	return ret;
}

static int get_pages_for_lvd(struct lcd_create_ctx *ctx)
{
	struct page *p1, *p2, *p3;
#ifndef LVD_PERCPU_VMFUNC_STATE
	struct page *p4;
#endif
	int ret;

	/*
	 * We explicity zero things out. If this code is used inside
	 * an LCD, it may not zero out the alloc'd pages.
	 *
	 * Alloc boot pages
	 */
	p1 = lcd_alloc_pages(0, LCD_BOOTSTRAP_PAGES_ORDER);
	if (!p1) {
		LIBLCD_ERR("alloc boot pages failed");
		ret = -ENOMEM;
		goto fail1;
	}
	memset(lcd_page_address(p1), 0, LCD_BOOTSTRAP_PAGES_SIZE);
	ctx->lcd_boot_info = lcd_page_address(p1);

	/*
	 * Initialize boot cptr cache
	 */
	ret = do_cptr_cache_init(lcd_to_boot_cptr_cache(ctx));
	if (ret) {
		LIBLCD_ERR("failed to init cptr cache");
		goto fail2;
	}
	/*
	 * Alloc boot page tables
	 */
	p2 = lcd_alloc_pages(0, LCD_BOOTSTRAP_PAGE_TABLES_ORDER);
	if (!p2) {
		LIBLCD_ERR("alloc boot page tables failed");
		ret = -ENOMEM;
		goto fail3;
	}
	memset(lcd_page_address(p2), 0, LCD_BOOTSTRAP_PAGE_TABLES_SIZE);
	ctx->gv_pg_tables = lcd_page_address(p2);

	p3 = lcd_alloc_pages(0, LCD_STACK_ORDER);
	if (!p3) {
		LIBLCD_ERR("alloc stack pages failed");
		ret = -ENOMEM;
		goto fail4;
	}
	memset(lcd_page_address(p3), 0, LCD_STACK_SIZE);
	ctx->stack = lcd_page_address(p3);
	lcd_stack = (void*) gva_val(gva_add(LCD_STACK_GV_ADDR, LCD_STACK_SIZE - sizeof(void *)));

#ifndef LVD_PERCPU_VMFUNC_STATE
	/*
	 * Alloc vmfunc_state_page
	 */
	p4 = lcd_alloc_pages(GFP_KERNEL, 0);
	if (!p4) {
		LIBLCD_ERR("alloc vmfunc_state_page failed");
		ret = -ENOMEM;
		goto fail5;
	}
	ctx->vmfunc_state_page = lcd_page_address(p4);
#endif
	return 0;

#ifndef LVD_PERCPU_VMFUNC_STATE
fail5:
	lcd_free_pages(p3, LCD_STACK_ORDER);
#endif
fail4:
	lcd_free_pages(p2, LCD_BOOTSTRAP_PAGE_TABLES_ORDER);
fail3:
	cptr_cache_destroy(lcd_to_boot_cptr_cache(ctx));
fail2:
	lcd_free_pages(p1, LCD_BOOTSTRAP_PAGES_ORDER);
fail1:
	return ret;
}

#ifndef LCD_ISOLATE

static void fix_struct_module_addrs(struct module *mod, void *m_core_bits)
{
	unsigned long symtab_offset, strtab_offset;
	/*
	 * Point symbol table and string table to new home
	 */
	symtab_offset = ((unsigned long)mod->core_kallsyms.symtab) -
		((unsigned long)mod->core_layout.base);
	strtab_offset = ((unsigned long)mod->core_kallsyms.strtab) -
		((unsigned long)mod->core_layout.base);
	mod->core_kallsyms.symtab = m_core_bits + symtab_offset;
	mod->core_kallsyms.strtab = m_core_bits + strtab_offset;
}

static int set_struct_module(cptr_t lcd, void *m_core_bits,
			unsigned long m_struct_module_core_offset)
{
	struct module *mod = m_core_bits + m_struct_module_core_offset;

	fix_struct_module_addrs(mod, m_core_bits);

	return lcd_set_struct_module_hva(lcd, mod);
}
#endif

extern void __kliblcd_module_host_unload(char *module_name);

/* taken from apply_relocation_add function defined at arch/x86/kernel/module.c */
static int __apply_relocate_add(Elf64_Shdr *sechdrs,
		   const char *strtab,
		   unsigned int symindex,
		   unsigned int relsec,
		   struct load_info *info,
		   const char *target_symbol,
		   gva_t target_new_addr,
		   gva_t m_core_link_addr,
		   struct lcd_create_ctx *ctx)
{
	unsigned int i;
	Elf64_Rela *rel;
	Elf64_Sym *sym;
	void *loc;
	void *loc_load;
	u64 val;
	unsigned long long loc_offset;

	/*
	 * Why do we populate from info->hdr instead of section headers?
	 * info->hdr is the place where the ELF header has been copied to.
	 * sechdrs has only partial information
	 */
	sechdrs[relsec].sh_addr = (size_t) info->hdr + sechdrs[relsec].sh_offset;

	rel = (Elf64_Rela *) sechdrs[relsec].sh_addr;

	printk(" AF sh_addr %llx | entrysz: %zu\n", sechdrs[relsec].sh_addr, sizeof(*rel));

	print_hex_dump(KERN_DEBUG, "relavmwrap: ", DUMP_PREFIX_ADDRESS, 24, 1,
			(void*)rel, sechdrs[relsec].sh_size, false);


	for (i = 0; i < sechdrs[relsec].sh_size / sizeof(*rel); i++) {
		/* This is where to make the change */
		loc_load = loc = (void *)sechdrs[sechdrs[relsec].sh_info].sh_addr
			+ rel[i].r_offset;

		/* loc_load is made RO by kernel after loading.
		 * So directly patch the addresses in our
		 * duplicated pages that would be loaded into
		 * the LCDs.
		 */
		/* extract offset */
		loc_offset = (unsigned long) loc - gva_val(m_core_link_addr);

		printk("%s, loc %p | loc_offset %Lx | ctx->m_core_bits %p\n",
				__func__, loc, loc_offset, ctx->m_core_bits);

		/* our duplicate page's virtual address is here in m_core_bits */
		loc = (char*)ctx->m_core_bits + loc_offset;

		/* This is the symbol it is referring to. Note that all
		 * undefined symbols have been resolved.
		 */
		sym = (Elf64_Sym *)sechdrs[symindex].sh_addr
			+ ELF64_R_SYM(rel[i].r_info);

		if (!strcmp(strtab + sym->st_name, target_symbol)) {
			/* patch target address */
			sym->st_value = gva_val(target_new_addr);
		} else {
			/* do nothing for other reloc entries they are patched already */
			continue;
		}

		val = sym->st_value + rel[i].r_addend;

		printk("idx: %d | type %d | st_value %Lx | st_name %s | r_addend %Lx | loc %Lx | curval %x | val %Lx\n",
				i, (int)ELF64_R_TYPE(rel[i].r_info),
				sym->st_value,
				strtab + sym->st_name,
				rel[i].r_addend,
				(u64)loc,
				*(u32*)loc,
				val);

		switch (ELF64_R_TYPE(rel[i].r_info)) {
		case R_X86_64_NONE:
			break;
		case R_X86_64_64:
			printk("\t64:old value %Lx\n", *(u64 *)loc);
			*(u64 *)loc = val;
			printk("\t64:patched value %Lx\n", *(u64 *)loc);
			break;
		case R_X86_64_32:
			printk("\t32:old value %x\n", *(u32 *)loc);
			*(u32 *)loc = val;
			printk("\t32:patched value %x\n", *(u32 *)loc);
			if (val != *(u32 *)loc) {
				pr_err("overflow in relocation type %d val %Lx\n",
				       (int)ELF64_R_TYPE(rel[i].r_info), val);
				return -ENOEXEC;
			}
			break;
		case R_X86_64_32S:
			printk("\t32:old value %x\n", *(s32 *)loc);
			*(s32 *)loc = val;
			printk("\t32S:patched value %x\n", *(s32 *)loc);
			if ((s64)val != *(s32 *)loc) {
				pr_err("overflow in relocation type %d val %Lx\n",
				       (int)ELF64_R_TYPE(rel[i].r_info), val);
				return -ENOEXEC;
			}
			break;
		case R_X86_64_PC32:
			printk("\tPC32: old value %x\n", *(u32 *)loc);
			val -= (u64)loc_load;
			*(u32 *)loc = val;
			printk("\tPC32: patched value %x\n", *(u32 *)loc);
#if 0
			if ((s64)val != *(s32 *)loc)
				goto overflow;
#endif
			break;
		default:
			pr_err("Unknown rela relocation: %llu\n",
					ELF64_R_TYPE(rel[i].r_info));
			return -ENOEXEC;
		}
	}
	return 0;
}

/* taken from apply_relocation_add function defined at arch/x86/kernel/module.c */
static int __reapply_relocate_add(Elf64_Shdr *sechdrs,
		   const char *strtab,
		   unsigned int symindex,
		   unsigned int relsec,
		   struct load_info *info,
		   gva_t m_vmfunc_page_addr,
		   struct lcd_create_ctx *ctx)
{
	unsigned int i;
	Elf64_Rela *rel;
	Elf64_Sym *sym;
	void *loc;
	void *loc_load;
	u64 val;
	unsigned long long loc_offset;

	/*
	 * Why do we populate from info->hdr instead of section headers?
	 * info->hdr is the place where the ELF header has been copied to.
	 * sechdrs has only partial information
	 */
	sechdrs[relsec].sh_addr = (size_t) info->hdr + sechdrs[relsec].sh_offset;

	rel = (Elf64_Rela *) sechdrs[relsec].sh_addr;

	printk(" AF sh_addr %llx | entrysz: %zu\n", sechdrs[relsec].sh_addr, sizeof(*rel));

	print_hex_dump(KERN_DEBUG, "relavmfunc: ", DUMP_PREFIX_ADDRESS, 24, 1,
			(void*)rel, sechdrs[relsec].sh_size, false);

	/*
	 * XXX: .vmfunc.trampoline.text would completely be moved to match with the
	 * kernel.  Update the sectionhdrs sh_addr to reflect the new load
	 * address. All other offsets are hopefully relative.
	 */
	sechdrs[sechdrs[relsec].sh_info].sh_addr = gva_val(m_vmfunc_page_addr);

	for (i = 0; i < sechdrs[relsec].sh_size / sizeof(*rel); i++) {
		/* This points to the gva of new vmfunc.trampoline page */
		loc_load = loc = (void *)sechdrs[sechdrs[relsec].sh_info].sh_addr
			+ rel[i].r_offset;

		/* loc_load is made RO by kernel after loading.
		 * So directly patch the addresses in our
		 * duplicated pages that would be loaded into
		 * the LCDs.
		 */
		/* extract offset */
		loc_offset = (unsigned long) loc - gva_val(m_vmfunc_page_addr);

		printk("%s, loc %p | loc_offset %Lx | ctx->m_vmfunc_tr_bits %p\n",
				__func__, loc, loc_offset, ctx->m_vmfunc_tr_bits);

		/* our duplicate page's virtual address is here in m_core_bits */
		loc = (char*)ctx->m_vmfunc_tr_bits + loc_offset;

		/* This is the symbol it is referring to. Note that all
		 * undefined symbols have been resolved.
		 */
		sym = (Elf64_Sym *)sechdrs[symindex].sh_addr
			+ ELF64_R_SYM(rel[i].r_info);

		val = sym->st_value + rel[i].r_addend;

		if (strcmp(strtab + sym->st_name, "vmfunc_dispatch")) {
			continue;
		}

		printk("idx: %d | type %d | st_value %Lx | st_name %s | r_addend %Lx | loc %Lx | curval %x | val %Lx\n",
				i, (int)ELF64_R_TYPE(rel[i].r_info),
				sym->st_value,
				strtab + sym->st_name,
				rel[i].r_addend,
				(u64)loc,
				*(u32*)loc,
				val);

		/*
		 * Go ahead and patch all the reloc entries in this text section.
		 */
		switch (ELF64_R_TYPE(rel[i].r_info)) {
		case R_X86_64_NONE:
			break;
		case R_X86_64_64:
			printk("\t64:old value %Lx\n", *(u64 *)loc);
			*(u64 *)loc = val;
			printk("\t64:patched value %Lx\n", *(u64 *)loc);
			break;
		case R_X86_64_32:
			printk("\t32:old value %x\n", *(u32 *)loc);
			*(u32 *)loc = val;
			printk("\t32:patched value %x\n", *(u32 *)loc);
			if (val != *(u32 *)loc) {
				pr_err("overflow in relocation type %d val %Lx\n",
				       (int)ELF64_R_TYPE(rel[i].r_info), val);
				return -ENOEXEC;
			}
			break;
		case R_X86_64_32S:
			printk("\t32:old value %x\n", *(s32 *)loc);
			*(s32 *)loc = val;
			printk("\t32S:patched value %x\n", *(s32 *)loc);
			if ((s64)val != *(s32 *)loc) {
				pr_err("overflow in relocation type %d val %Lx\n",
				       (int)ELF64_R_TYPE(rel[i].r_info), val);
				return -ENOEXEC;
			}
			break;
		case R_X86_64_PC32:
			printk("\tPC32: old value %x\n", *(u32 *)loc);
			val -= (u64)loc_load;
			*(u32 *)loc = val;
			printk("\tPC32: patched value %x\n", *(u32 *)loc);
#if 0
			if ((s64)val != *(s32 *)loc)
				goto overflow;
#endif
			break;
		default:
			pr_err("Unknown rela relocation: %llu\n",
					ELF64_R_TYPE(rel[i].r_info));
			return -ENOEXEC;
		}
	}
	return 0;
}

int lvd_create_module_lvd(char *mdir, char *mname, cptr_t *lcd_out,
			struct lcd_create_ctx **ctx_out, int lvd_id)
{
	int ret;
	cptr_t m_init_cptr, m_core_cptr;
	gva_t m_init_link_addr, m_core_link_addr, m_init_func_addr;
	unsigned long m_init_size, m_core_size;
	cptr_t m_vmfunc_tr_cptr;
	cptr_t m_vmfunc_sb_cptr;
	gva_t m_vmfunc_tr_page_addr;
	gva_t m_vmfunc_sb_page_addr;
	unsigned long m_vmfunc_tr_page_size;
	unsigned long m_vmfunc_sb_page_size;
	struct module *mod;
	struct klp_modinfo *klp_info;
	struct load_info *info;
	int relsec;
	unsigned long m_struct_module_core_offset;
	struct lcd_create_ctx *ctx;
	cptr_t lcd;

	/*
	 * Initialize create ctx
	 */
	ret = init_create_ctx(&ctx, mname);
	if (ret) {
		LIBLCD_ERR("error creating ctx");
		goto fail1;
	}
	/*
	 * Alloc boot pages, stack pages, etc. for LCD
	 */
	ret = get_pages_for_lvd(ctx);
	if (ret) {
		LIBLCD_ERR("error alloc'ing boot, stack pages for LCD");
		goto fail2;
	}
	/*
	 * Load kernel module into caller's address space
	 */

	ret = lvd_load_module(mdir, mname,
			&ctx->m_init_bits, &ctx->m_core_bits,
			&ctx->m_vmfunc_tr_bits,
			&ctx->m_vmfunc_sb_bits,
			&m_init_cptr, &m_core_cptr,
			&m_vmfunc_tr_cptr,
			&m_vmfunc_sb_cptr,
			&m_init_link_addr, &m_core_link_addr,
			&m_init_size, &m_core_size,
			&m_init_func_addr,
			&m_struct_module_core_offset,
			&m_vmfunc_tr_page_addr,
			&m_vmfunc_tr_page_size,
			&m_vmfunc_sb_page_addr,
			&m_vmfunc_sb_page_size
			);
	if (ret) {
		LIBLCD_ERR("error loading kernel module, ret = %d", ret);
		goto fail3;
	}

	/*
	 * At this point, we have all of the data that will go in the LCD
	 * (the microkernel has the UTCB page)
	 *
	 * Initialize empty LCD
	 */
	ret = lvd_create(&lcd, lvd_id);
	if (ret) {
		LIBLCD_ERR("error creating empty LCD");
		goto fail4;
	}

	create_lvd_stack_pools(ctx, lcd);

	create_lvd_register_page(ctx, lcd);

#ifdef LVD_PERCPU_VMFUNC_STATE
	create_lvd_vmfunc_state_page(ctx);
#endif
	/*
	 * vmfuncwrapper.text hosts vmfunc wrapper. SHT_RELA of this text
	 * section has to be patched because we have moved the vmfunc page to
	 * align it with the host's vmfunc page address.
	 *
	 */
	mutex_lock(&module_mutex);
	mod = find_module(mname);
	mutex_unlock(&module_mutex);

	if (!mod)
		goto skip;

	if (!mod->klp_info) {
		LIBLCD_ERR("module does not have ELF information. did you enable LIVE_PATCH?");
		goto fail4;
	}
	/* get klp_info pointer */
	klp_info = mod->klp_info;
	info = &klp_info->info;

	/* find offset of the function that will be called from vmfunc_wrapper */
	if (vmfunc_load_debug) {
		unsigned int strtab_shsize = 0;
		int is_null = 0;
		int i;

		printk("%s, sechdrs %p | index.sym %x | info %p | info->hdr %p | klp_info->hdr.e_shnum %x\n", __func__,
				info->sechdrs, info->index.sym, info, info->hdr, mod->klp_info->hdr.e_shnum);

		is_null |= CHECK_NULL(offsetof(struct load_info, hdr), info->hdr);
		if (!is_null)
			is_null |= CHECK_NULL(offsetof(struct elf64_hdr, e_shnum), info->hdr->e_shnum);

		is_null |= CHECK_NULL(offsetof(struct load_info, sechdrs), info->sechdrs);
		is_null |= CHECK_NULL(offsetof(struct load_info, secstrings), info->secstrings);

		if (is_null) {
			printk("%s, one or more pointers are null\n", __func__);
			goto skip;
		}

		/* search through the headers to find symtab section */
		for (i = 1; i < klp_info->hdr.e_shnum; i++) {
			if (klp_info->sechdrs[i].sh_type == SHT_SYMTAB) {
				unsigned int strtab_idx = klp_info->sechdrs[i].sh_link;
				info->strtab = (char *)info->hdr
					+ klp_info->sechdrs[strtab_idx].sh_offset;
				strtab_shsize = klp_info->sechdrs[strtab_idx].sh_size;
				break;
			}
		}

		printk("%s, hdr: %p | string table: %p | sh_size %x\n",
				__func__, info->hdr, info->strtab,
				strtab_shsize);

		print_hex_dump(KERN_DEBUG, "strtab: ", DUMP_PREFIX_ADDRESS, 32,
				1, info->strtab, strtab_shsize, true);

		printk("%s, hdr: %p | symtab: %llx | sh_size %llx\n", __func__,
				info->hdr,
				klp_info->sechdrs[klp_info->symndx].sh_addr,
				klp_info->sechdrs[klp_info->symndx].sh_size);

		print_hex_dump(KERN_DEBUG, "symtab: ", DUMP_PREFIX_ADDRESS, 32,
				1,
				(void*)klp_info->sechdrs[klp_info->symndx].sh_addr,
				klp_info->sechdrs[klp_info->symndx].sh_size,
				false);

		printk("%s, hdr: %p | symtab: %p | sh_size %lx\n", __func__,
				info->hdr, mod->core_kallsyms.symtab,
				mod->core_kallsyms.num_symtab *
				sizeof(Elf_Sym));

		print_hex_dump(KERN_DEBUG, "symtab: ", DUMP_PREFIX_ADDRESS, 32,
				1, (void*)mod->core_kallsyms.symtab,
				mod->core_kallsyms.num_symtab *
				sizeof(Elf_Sym), false);

	}

	for (relsec = 1; relsec < klp_info->hdr.e_shnum; relsec++) {
		Elf64_Shdr *shdr = &klp_info->sechdrs[relsec];
		const char *sh_name = klp_info->secstrings + shdr->sh_name;

		if (shdr->sh_type == SHT_RELA) {
			Elf64_Shdr *sechdrs = klp_info->sechdrs;
			unsigned int sh_size = shdr->sh_size;
			unsigned int sh_offset = shdr->sh_offset;
			unsigned int symindex = klp_info->symndx;
			const char *strtab = mod->core_kallsyms.strtab;

			if (!strcmp(".rela.vmfunc.trampoline.text", sh_name)) {
				printk("%s, shdr: %p | sh_name: %s | sh_size %u	| sh_offset %x | b4 sh_addr %llx | ", __func__, shdr,
						sh_name, sh_size, sh_offset, shdr->sh_addr);

				__reapply_relocate_add(sechdrs, strtab, symindex,
						relsec, info,
						gva_add(m_vmfunc_tr_page_addr, 0),
						ctx);

			} else if (!strcmp(".rela.vmfuncwrapper.text", sh_name)) {
				printk("%s, shdr: %p | sh_name: %s | sh_size %u	| sh_offset %x | b4 sh_addr %llx | ", __func__, shdr,
						sh_name, sh_size, sh_offset, shdr->sh_addr);

				__apply_relocate_add(sechdrs, strtab, symindex,
						relsec, info, "vmfunc_springboard_entry",
						/*
						 * FIXME: This should be the location of the above function on
						 * the newly loaded page. Fill the offset below.
						 */
						gva_add(m_vmfunc_sb_page_addr, 0),
						m_core_link_addr, ctx);
			}
		}
	}

	print_hex_dump(KERN_DEBUG, "vmfuncwrapper: ", DUMP_PREFIX_ADDRESS,
		32, 1, (void*)((char*)ctx->m_core_bits + 0x1000), 0x100, false);

	/* unload the module. we don't need it anymore */
	__kliblcd_module_host_unload(mname);
skip:

	/*
	 * Set up address spaces
	 */
	ret = setup_addr_spaces_lvd(lcd, ctx, m_init_link_addr,
				m_core_link_addr,
				m_vmfunc_tr_page_addr,
				m_vmfunc_sb_page_addr,
				m_init_size, m_core_size,
				m_vmfunc_tr_page_size,
				m_vmfunc_sb_page_size
				);
	if (ret) {
		LIBLCD_ERR("error setting up address spaces");
		goto fail5;
	}
	/*
	 * Configure initial control registers, etc. for LCD
	 */
	ret = lcd_config_registers(lcd, m_init_func_addr,
				/* implicity put a null return address and
				 * frame address */
				gva_add(LCD_STACK_GV_ADDR,
					LCD_STACK_SIZE - sizeof(void *)),
				LCD_BOOTSTRAP_PAGE_TABLES_GP_ADDR,
				LCD_UTCB_GP_ADDR);
	if (ret) {
		LIBLCD_ERR("error configuring LCDs registers");
		goto fail6;
	}
#ifndef LCD_ISOLATE
	/*
	 * For non-isolated code running this, we also remember where
	 * the struct module copy is located so we can do stack traces.
	 */
	ret = set_struct_module(lcd, ctx->m_core_bits,
				m_struct_module_core_offset);
	if (ret) {
		LIBLCD_ERR("error setting struct module hva");
		goto fail7;
	}
#endif

	/*
	 * Return context and lcd
	 */
	*ctx_out = ctx;
	*lcd_out = lcd;

	lcd_save_cr3(lcd, ctx->gv_pg_tables);

	return 0;

#ifndef LCD_ISOLATE
fail7:
#endif
fail6:
fail5:
	lcd_cap_delete(lcd);
fail4:
	lvd_release_module(ctx->m_init_bits, ctx->m_core_bits, ctx->m_vmfunc_tr_bits, ctx->m_vmfunc_sb_bits);
fail3:
fail2:
	destroy_create_ctx(ctx);
fail1:
	return ret;
}

void lvd_destroy_create_ctx(struct lcd_create_ctx *ctx)
{
	destroy_create_ctx(ctx);
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(lvd_create_module_lvd);
EXPORT_SYMBOL(lvd_destroy_create_ctx);
