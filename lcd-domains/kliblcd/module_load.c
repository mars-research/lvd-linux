/*
 * module_load.c
 *
 * For loading a kernel module from disk into memory.
 *
 * Copyright: University of Utah
 */

#include <linux/slab.h>
#include <liblcd/liblcd.h>
#include <lcd_domains/microkernel.h>
#include <linux/kallsyms.h>

/* HOST LOAD/UNLOAD -------------------------------------------------- */

static inline size_t mpath_size(char *mdir, char *module_name)
{
	size_t psize = 0;
	/* mdir/ */
	psize += strlen(mdir) + 1;
	/* module_name.ko */
	psize += strlen(module_name) + 3;
        /* Allow for trailing \0 */
	return psize + 1;
}

int __kliblcd_module_host_load(char *mdir, char *module_name, 
			struct module **m_out)
{
	int ret;
	struct module *m;
	char *mpath;
	size_t psize;
	/*
	 * Form module path
	 */
	psize = mpath_size(mdir, module_name);
	if (psize >= LCD_MPATH_SIZE) { 
		LIBLCD_ERR("path to lcd's module .ko is too big");
		LIBLCD_ERR("   mdir = %s, name = %s, size = %llu",
			mdir, module_name, psize);
		ret = -EINVAL;
		goto fail;
	}
	mpath = kmalloc(psize, GFP_KERNEL);
	if (!mpath) {
		LIBLCD_ERR("malloc module path");
		ret = -ENOMEM;
		goto fail0;
	}
	snprintf(mpath, psize, "%s/%s.ko", mdir, module_name);
	/*
	 * Load the requested module
	 */
	ret = request_lcd_module(mpath);
	if (ret < 0) {
		LIBLCD_ERR("load module failed for %s (ret = %d)", mpath, ret);
               /* To facilitate debugging */
               if (ret == -ENOENT)
                       LIBLCD_ERR("Maybe /sbin/lcd-insmod is missing?");
		goto fail1;
	}
	/*
	 * Find loaded module, and inc its ref counter; must hold module mutex
	 * while finding module.
	 */
	mutex_lock(&module_mutex);
	m = find_module(module_name);
	mutex_unlock(&module_mutex);	
	if (!m) {
		LIBLCD_ERR("couldn't find module");
		ret = -EIO;
		goto fail2;
	}
	if(!try_module_get(m)) {
		LIBLCD_ERR("incrementing module ref count");
		ret = -EIO;
		goto fail3;
	}

	*m_out = m;

	kfree(mpath);

	return 0;

fail3:
	ret = do_sys_delete_module(module_name, 0, 1);
	if (ret)
		LIBLCD_ERR("double fault: deleting module");
fail2:
fail1:
	*m_out = NULL;
	kfree(mpath);
fail0:
fail:
	return ret;
}

void __kliblcd_module_host_unload(char *module_name)
{
	int ret;
	struct module *m;
	/*
	 * Delete module
	 *
	 * We need to look it up so we can do a put
	 */
 	mutex_lock(&module_mutex);
 	m = find_module(module_name);
 	mutex_unlock(&module_mutex);
	if (!m) {
		LIBLCD_ERR("couldn't find module");
		goto fail;
	}
	module_put(m);
	ret = do_sys_delete_module(module_name, 0, 1);
	if (ret) {
		LIBLCD_ERR("Error deleting module from host %d", ret);
		goto fail;
	}

	goto out;
out:
fail:
	return;
}

/* HIGHER-LEVEL LOAD/UNLOAD ---------------------------------------- */
static int create_metadata(hva_t pages_base, unsigned long size)
{
	return lcd_create_mo_metadata(pages_base, size);
}

static int dup_module_pages(hva_t pages_base, unsigned long size,
			void **dup_pages_bits,
			cptr_t *dup_pages_cap_out)
{
	int ret;
	void *dup_pages;
	unsigned long unused1, unused2;
	/*
	 * Alloc dup pages, and memcpy the bits over
	 */
	dup_pages = lcd_vmalloc(size);
	if (!dup_pages) {
		LIBLCD_ERR("failed to vmalloc dup pages");
		ret = -ENOMEM;
		goto fail1;
	}
	LIBLCD_MSG("dup_page %p", dup_pages);
	memcpy(dup_pages, hva2va(pages_base), size);
	/*
	 * Look up the cptr to the vmalloc memory object capability
	 */
	ret = lcd_virt_to_cptr(__gva((unsigned long)dup_pages),
			dup_pages_cap_out,
			&unused1, &unused2);
	if (ret) {
		LIBLCD_ERR("vmalloc mem object lookup failed");
		goto fail2;
	}

	*dup_pages_bits = dup_pages;

	return 0;

fail2:
	lcd_vfree(dup_pages);
fail1:
	return ret;
}

#ifndef CONFIG_LVD
static void dedup_module_pages(void *pages_bits)
{
	lcd_vfree(pages_bits);
}
#endif

int lcd_load_module(char *mdir, char *mname,
		void **m_init_bits,
		void **m_core_bits,
#ifdef VMFUNC_PAGE_REMAP
		void **m_vmfunc_bits,
#endif
		cptr_t *m_init_pages,
		cptr_t *m_core_pages,
#ifdef VMFUNC_PAGE_REMAP
		cptr_t *m_vmfunc_pages,
#endif
		gva_t *m_init_link_addr,
		gva_t *m_core_link_addr,
		unsigned long *m_init_size,
		unsigned long *m_core_size,
		gva_t *m_init_func_addr,
		unsigned long *m_struct_module_core_offset
#ifdef VMFUNC_PAGE_REMAP
		,
		gva_t *m_vmfunc_page_addr,
		unsigned long *m_vmfunc_page_size
#endif
		)
{
	int ret;
	struct module *m;
#ifdef VMFUNC_PAGE_REMAP
	unsigned long vmfunc_load_addr;
	unsigned long vmfunc_load_addr_lcd;
	unsigned long vmfunc_page_size;
#endif

#ifdef VMFUNC_PAGE_REMAP
	/*
	 * Do these lookups before loading the LCD module. Otherwise, kallsyms
	 * is going to return this modules vmfunc_load_addr, whereas we want
	 * KLCD's vmfunc_load_addr
	 */
	vmfunc_load_addr = kallsyms_lookup_name("vmfunc_load_addr");
	vmfunc_page_size = kallsyms_lookup_name("vmfunc_page_size");
#endif
	/*
	 * Load module in host
	 */
	ret = __kliblcd_module_host_load(mdir, mname, &m);
	if (ret)
		goto fail1;

#ifndef CONFIG_LVD
	/*
	 * Dup init and core pages so that LCD will use a copy
	 * separate from the host (this will protect things
	 * like the struct module, the module's symbol table,
	 * etc. that the host uses for certain operations).
	 */
	ret = dup_module_pages(va2hva(m->init_layout.base), m->init_layout.size,
			m_init_bits, m_init_pages);
	if (ret) {
		LIBLCD_ERR("failed to load module's init");
		goto fail2;
	}
	ret = dup_module_pages(va2hva(m->core_layout.base), m->core_layout.size,
			m_core_bits, m_core_pages);
	if (ret) {
		LIBLCD_ERR("failed to load module's core");
		goto fail3;
	}
#else
	/* for LCDs with no page tables, there is no need to take a copy.
	 * Let the module get loaded onto host normally and we map those
	 * pages in our new EPT
	 */
	create_metadata(va2hva(m->init_layout.base), m->init_layout.size);
	create_metadata(va2hva(m->core_layout.base), m->core_layout.size);

	*m_init_bits = m->init_layout.base;
	*m_core_bits = m->core_layout.base;
#endif

#ifdef VMFUNC_PAGE_REMAP
	vmfunc_load_addr_lcd = kallsyms_lookup_name("vmfunc_load_addr");
	if (vmfunc_load_addr && vmfunc_page_size) {
		*m_vmfunc_page_addr = __gva(*((unsigned long *)vmfunc_load_addr));
		*m_vmfunc_page_size = *((unsigned long*)vmfunc_page_size);
	} else
		*m_vmfunc_page_addr = __gva(0UL);

	/* Only for vmfunc page, we make an additional copy */
	ret = dup_module_pages(va2hva((void*)(*(unsigned long*)vmfunc_load_addr_lcd)), *((unsigned long*)vmfunc_page_size),
			m_vmfunc_bits, m_vmfunc_pages);
	if (ret) {
		LIBLCD_ERR("failed to load module's core");
		goto fail4;
	}
#endif
	/*
	 * Extract addresses where init and core should be mapped (the
	 * link base addresses), sizes, and location of struct
	 * module in .ko image.
	 */
	*m_init_link_addr = __gva((unsigned long)m->init_layout.base);
	*m_core_link_addr = __gva((unsigned long)m->core_layout.base);
	*m_init_size = m->init_layout.size;
	*m_core_size = m->core_layout.size;
	*m_init_func_addr = __gva((unsigned long)m->init);
	*m_struct_module_core_offset = 
		((unsigned long)m) - ((unsigned long)m->core_layout.base);

	LIBLCD_MSG("Loaded %s.ko: ", mname);
	printk("    init addr 0x%p init size 0x%x\n",
		m->init_layout.base, m->init_layout.size);
	printk("    core addr 0x%p core size 0x%x\n",
		m->core_layout.base, m->core_layout.size);
#ifdef VMFUNC_PAGE_REMAP
	printk("    vmfunc addr 0x%lx vmfunc size 0x%lx\n",
		gva_val(*m_vmfunc_page_addr), *m_vmfunc_page_size);
#endif

	/*
	 * Unload module from host -- we don't need the host module
	 * loader to hang onto it now that we've got the program
	 * bits.
	 */
#ifndef VMFUNC_PAGE_REMAP
	/* Do not unload the module from host, yet. we need to patch a few
	 * relocs before finalizing the module. Do it once we setup the address
	 * spaces.
	 */
	__kliblcd_module_host_unload(mname);
#endif
	return 0;
#ifdef VMFUNC_PAGE_REMAP
fail4:
#endif
#ifndef CONFIG_LVD
	dedup_module_pages(*m_core_bits);
fail3:
	dedup_module_pages(*m_init_bits);
fail2:
	__kliblcd_module_host_unload(mname);
#endif /* CONFIG_LVD */
fail1:
	return ret;
}

void lcd_release_module(void *m_init_bits, void *m_core_bits)
{
	/*
	 * Delete duplicates
	 */
#ifndef CONFIG_LVD
	dedup_module_pages(m_init_bits);
	dedup_module_pages(m_core_bits);
#endif
}

/* EXPORTS -------------------------------------------------- */

EXPORT_SYMBOL(lcd_load_module);
EXPORT_SYMBOL(lcd_release_module);
