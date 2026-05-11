/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_PKS_KEYS_H
#define _LINUX_PKS_KEYS_H

/*
 * The contents of this header should be limited to assigning PKS keys and
 * default values to avoid intricate header dependencies.
 */

#ifdef CONFIG_ARCH_ENABLE_SUPERVISOR_PKEYS

#include <asm/pkeys_common.h>
#include <uapi/asm-generic/mman-common.h>

/* PKS_KEY_DEFAULT must be 0 */
#define PKS_KEY_DEFAULT		0
#define PKS_KEY_MONITOR 	1
/* PKS_KEY_DEFAULT_INIT must be RW */
#define PKS_KEY_DEFAULT_INIT	PKR_RW_MASK(PKS_KEY_DEFAULT)
#define PKS_KEY_MONITOR_INIT 	PKR_WD_MASK(PKS_KEY_MONITOR)

#define PKS_ALL_AD_MASK \
	GENMASK(PKS_NUM_PKEYS * PKR_BITS_PER_PKEY, \
		2 * PKR_BITS_PER_PKEY)

#define PKS_INIT_VALUE ((PKS_ALL_AD & PKS_ALL_AD_MASK) | \
			PKS_KEY_DEFAULT_INIT | \
			PKS_KEY_MONITOR_INIT \
			)

#define PKEY2PKRS(pkey) (pkrs_default & \
			(~(PKEY_ACCESS_MASK << (pkey * PKR_BITS_PER_PKEY))))

#define PKRS_MONITOR PKEY2PKRS(PKS_KEY_MONITOR)
extern unsigned int pkrs_default;

extern int pks_key_max;

static inline int pks_new_key()
{
	pks_key_max++;
	return pks_key_max;
}

// #define PKS_NEW_KEY(prev, config) \
// 	(prev + __is_defined(config))
// #define PKS_DECLARE_INIT_VALUE(pkey, value, config) \
// 	(PKR_##value##_MASK(pkey) * __is_defined(config))

/**
 * DOC: PKS_KEY_ALLOCATION
 *
 * Users reserve a key value in 5 steps.
 *	1) Use PKS_NEW_KEY to create a new key
 *	2) Ensure that the last key value is specified in the PKS_NEW_KEY macro
 *	3) Adjust PKS_KEY_MAX to use the newly defined key value
 *	4) Use PKS_DECLARE_INIT_VALUE to define an initial value
 *	5) Add the new PKS default value to PKS_INIT_VALUE
 *
 * The PKS_NEW_KEY and PKS_DECLARE_INIT_VALUE macros require the Kconfig
 * option to be specified to automatically adjust the number of keys used.
 *
 * PKS_KEY_DEFAULT must remain 0 with a default of PKS_DECLARE_INIT_VALUE(...,
 * RW, ...) to support non-pks protected pages.
 *
 * Example: to configure a key for 'MY_FEATURE' with a default of Write
 * Disabled.
 *
 * .. code-block:: c
 *
 *	#define PKS_KEY_DEFAULT		0
 *
 *	// 1) Use PKS_NEW_KEY to create a new key
 *	// 2) Ensure that the last key value is specified (eg PKS_KEY_DEFAULT)
 *	#define PKS_KEY_MY_FEATURE PKS_NEW_KEY(PKS_KEY_DEFAULT, CONFIG_MY_FEATURE)
 *
 *	// 3) Adjust PKS_KEY_MAX
 *	#define PKS_KEY_MAX	   PKS_NEW_KEY(PKS_KEY_MY_FEATURE, 1)
 *
 *	// 4) Define initial value
 *	#define PKS_KEY_MY_FEATURE_INIT PKS_DECLARE_INIT_VALUE(PKS_KEY_MY_FEATURE, \
 *								WD, CONFIG_MY_FEATURE)
 *
 *
 *	// 5) Add initial value to PKS_INIT_VALUE
 *	#define PKS_INIT_VALUE ((PKS_ALL_AD & PKS_ALL_AD_MASK) | \
 *				PKS_KEY_DEFAULT_INIT | \
 *				PKS_KEY_MY_FEATURE_INIT \
 *				)
 */

// /* PKS_KEY_DEFAULT must be 0 */
// #define PKS_KEY_DEFAULT		0
// #define PKS_KEY_MONITOR PKS_NEW_KEY(PKS_KEY_DEFAULT, CONFIG_PKS_MONITOR) //CONFIG_PKS_MONITOR_SGT)
// #define PKS_KEY_IPV6 PKS_NEW_KEY(PKS_KEY_MONITOR, CONFIG_PKS_IPV6)
// #define PKS_KEY_DM_ZERO PKS_NEW_KEY(PKS_KEY_IPV6, CONFIG_PKS_DM_ZERO)
// #define PKS_KEY_MAX		PKS_NEW_KEY(PKS_KEY_DM_ZERO, 1)

// /* PKS_KEY_DEFAULT_INIT must be RW */
// #define PKS_KEY_DEFAULT_INIT	PKS_DECLARE_INIT_VALUE(PKS_KEY_DEFAULT, RW, 1)
// #define PKS_KEY_MONITOR_INIT PKS_DECLARE_INIT_VALUE(PKS_KEY_MONITOR, \
// 								WD, CONFIG_PKS_MONITOR) //CONFIG_PKS_MONITOR_SGT)
// #define PKS_KEY_IPV6_INIT PKS_DECLARE_INIT_VALUE(PKS_KEY_IPV6, \
// 								WD, CONFIG_PKS_IPV6)
// #define PKS_KEY_DM_ZERO_INIT PKS_DECLARE_INIT_VALUE(PKS_KEY_DM_ZERO, \
// 								WD, CONFIG_PKS_DM_ZERO)

// #define PKS_ALL_AD_MASK \
// 	GENMASK(PKS_NUM_PKEYS * PKR_BITS_PER_PKEY, \
// 		PKS_KEY_MAX * PKR_BITS_PER_PKEY)

// #define PKS_INIT_VALUE ((PKS_ALL_AD & PKS_ALL_AD_MASK) | \
// 			PKS_KEY_DEFAULT_INIT | \
// 			PKS_KEY_MONITOR_INIT | \
// 			PKS_KEY_IPV6_INIT | \
// 			PKS_KEY_DM_ZERO_INIT \
// 			)

// #define PKEY2PKRS(pkey) (PKS_INIT_VALUE & \
// 			(~(PKEY_ACCESS_MASK << (pkey * PKR_BITS_PER_PKEY))))

// #define PKRS_DEFAULT PKEY2PKRS(PKS_KEY_DEFAULT)
// #define PKRS_MONITOR PKEY2PKRS(PKS_KEY_MONITOR)
// #define PKRS_IPV6 PKEY2PKRS(PKS_KEY_IPV6)
// #define PKRS_DM_ZERO PKEY2PKRS(PKS_KEY_DM_ZERO)

#endif /* CONFIG_ARCH_ENABLE_SUPERVISOR_PKEYS */

#endif /* _LINUX_PKS_KEYS_H */
