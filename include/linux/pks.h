/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PKS_H
#define _LINUX_PKS_H

#ifdef CONFIG_ARCH_ENABLE_SUPERVISOR_PKEYS

#include <linux/types.h>

#include <uapi/asm-generic/mman-common.h>

#include <asm/pks.h>

// extern char __start_text_monitor[];
// extern char __end_text_monitor[];
extern u32 pkrs_cache;
extern u32 previous_pkrs;

bool pks_available(void);
void pks_update_protection(u8 pkey, u8 protection);
void pks_update_exception(struct pt_regs *regs, u8 pkey, u8 protection);

/**
 * pks_set_noaccess() - Disable all access to the domain
 * @pkey: the pkey for which the access should change.
 *
 * Disable all access to the domain specified by pkey.  This is not a global
 * update and only affects the current running thread.
 */
static inline void pks_set_noaccess(u8 pkey)
{
	pks_update_protection(pkey, PKEY_DISABLE_ACCESS);
}

/**
 * pks_set_readwrite() - Make the domain Read/Write
 * @pkey: the pkey for which the access should change.
 *
 * Allow all access, read and write, to the domain specified by pkey.  This is
 * not a global update and only affects the current running thread.
 */
static inline void pks_set_readwrite(u8 pkey)
{
	pks_update_protection(pkey, PKEY_READ_WRITE);
}

static inline void pks_set_nowrite(u8 pkey)
{
	pks_update_protection(pkey, PKEY_DISABLE_WRITE);
}

typedef bool (*pks_key_callback)(struct pt_regs *regs, unsigned long address,
				 bool write);

#ifdef CONFIG_PKS_IPV6
bool ipv6_pks_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
#endif

#ifdef CONFIG_PKS_NF_TABLES
bool nf_tables_pks_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
#endif

#ifdef CONFIG_PKS_DM_ZERO
bool dm_zero_pks_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
#endif

#ifdef CONFIG_PKS_LKM20
bool pkey2_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
bool pkey3_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
bool pkey4_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
bool pkey5_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
bool pkey6_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
bool pkey7_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
bool pkey8_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
bool pkey9_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
bool pkey10_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
bool pkey11_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
bool pkey12_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
bool pkey13_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
bool pkey14_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
bool pkey15_fault_callback(struct pt_regs *regs, unsigned long address, bool write);
#endif

#else /* !CONFIG_ARCH_ENABLE_SUPERVISOR_PKEYS */

static inline bool pks_available(void)
{
	return false;
}

static inline void pks_set_noaccess(u8 pkey) {}
static inline void pks_set_readwrite(u8 pkey) {}
static inline void pks_set_nowrite(u8 pkey) {}
static inline void pks_update_exception(struct pt_regs *regs,
					u8 pkey,
					u8 protection)
{ }

#endif /* CONFIG_ARCH_ENABLE_SUPERVISOR_PKEYS */

#endif /* _LINUX_PKS_H */
