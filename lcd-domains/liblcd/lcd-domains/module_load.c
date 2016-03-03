/*
 * module_load.c
 *
 * For loading a kernel module from disk.
 *
 * Copyright: University of Utah
 */

#include <lcd_config/pre_hook.h>

#include <liblcd/module_load.h>

#include <lcd_config/post_hook.h>


int lcd_load_module(char *mdir, char *mname,
		void **m_init_bits,
		void **m_core_bits,
		cptr_t *m_init_pages,
		cptr_t *m_core_pages,
		gva_t *m_init_link_addr,
		gva_t *m_core_link_addr,
		unsigned long *m_init_size,
		unsigned long *m_core_size)
{
	return -ENOSYS; /* not implemented */
}

void lcd_release_module(void *m_init_bits, void *m_core_bits)
{
	return; /* not implemented */
}
