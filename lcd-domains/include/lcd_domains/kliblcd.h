/*
 * internal.h
 *
 * Some kliblcd-internal definitions.
 *
 */
#ifndef KLIBLCD_INTERNAL_H
#define KLIBLCD_INTERNAL_H

#include <linux/module.h>

int __kliblcd_module_host_load(char *mdir, char *module_name, 
			struct module **m_out);
void __kliblcd_module_host_unload(char *module_name);


#endif /* KLIBLCD_INTERNAL_H */
