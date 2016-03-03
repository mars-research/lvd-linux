/*
 * module_load.h
 *
 * For loading a kernel module from disk.
 *
 * Copyright: University of Utah
 */

#include <libcap.h>
#include <lcd_domains/types.h>

/**
 * lcd_load_module -- Load a kernel module from disk
 * @mdir: the directory on the host file system that contains the module
 * @mname: the name of the module (e.g. mname=foo for module foo.ko)
 * @m_init_bits: out param, points to module's init code in caller's
 *               virtual address space
 * @m_init_pages: cptr to memory object capability for init bits
 * @m_init_link_addr: the address that the module's init code was linked for
 * @m_init_size: size in bytes of the module init part of the image
 * @m_core_size: size in bytes of the module core part of the image
 * @m_struct_module_core_offset: offset into .ko's core where struct module
 *                               is located
 *
 * The *_core_* out params have similar meaning.
 *
 * You can access the module's program bits via the virtual addresses
 * passed via the *_bits variables.
 *
 * You can grant other LCDs access to the module code and data via 
 * the *_pages cptr's.
 *
 * You can use the *_link_addr to know where the module bits should be
 * mapped so that you can execute the code in the module.
 */
int lcd_load_module(char *mdir, char *mname,
		void **m_init_bits,
		void **m_core_bits,
		cptr_t *m_init_pages,
		cptr_t *m_core_pages,
		gva_t *m_init_link_addr,
		gva_t *m_core_link_addr,
		unsigned long *m_init_size,
		unsigned long *m_core_size,
		unsigned long *m_struct_module_core_offset);
/**
 * lcd_release_module -- Unmap module bits and cap delete the pages
 * @m_init_bits: virtual address of pages that contain module's init
 * @m_core_bits: similar
 *
 * Frees module code and data from caller's address space, and deletes
 * capabilities to the bits. If you didn't grant anyone else access
 * to the pages (or you revoked access), they will be reclaimed in 
 * the host. (Hence the name "release" - the module won't necessarily
 * be completely freed.)
 */
void lcd_release_module(void *m_init_bits, void *m_core_bits);
