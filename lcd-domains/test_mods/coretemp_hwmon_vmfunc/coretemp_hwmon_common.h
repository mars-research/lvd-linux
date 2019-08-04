#ifndef __CORETEMP_HWMON_COMMON_H__
#define __CORETEMP_HWMON_COMMON_H__

#include <linux/kthread.h>
#include <linux/module.h>

#include <libcap.h>
#include <libfipc.h>
#include <thc_ipc.h>
#include <liblcd/glue_cspace.h>
#include <liblcd/liblcd.h>
#include <liblcd/sync_ipc_poll.h>
#include <linux/priv_mempool.h>
#include <asm/lcd_domains/libvmfunc.h>

#include "coretemp_hwmon_glue_helper.h"

enum dispatch_t {
	X86_MATCH_CPU,
	__PLATFORM_DRIVER_REGISTER,
	PLATFORM_DEVICE_UNREGISTER,
	__REGISTER_CPU_NOTIFIER,
	PLATFORM_DRIVER_UNREGISTER,
	__UNREGISTER_CPU_NOTIFIER,
	PLATFORM_DEVICE_ADD,
	PLATFORM_DEVICE_ALLOC,
	PLATFORM_DEVICE_PUT,
	PCI_GET_DOMAIN_BUS_AND_SLOT,
	DEVM_HWMON_DEVICE_REGISTER_WITH_GROUPS,
	RDMSR_SAFE_ON_CPU,
	RDMSR_ON_CPU,
	SYSFS_CREATE_GROUP,
	SYSFS_REMOVE_GROUP,
	NOTIFIER_FN_T,
	CPU_MAPS_UPDATE_BEGIN,
	CPU_MAPS_UPDATE_DONE,
	PROBE,
	REMOVE,
	MODULE_INIT,
	MODULE_EXIT,
	ATTR_SHOW,
	CPU_DATA,
};

/* CSPACES ------------------------------------------------------------ */
int glue_cap_init(void);

int glue_cap_create(struct glue_cspace **cspace);

void glue_cap_destroy(struct glue_cspace *cspace);

void glue_cap_exit(void);

void glue_cap_remove(
	struct glue_cspace *cspace,
	cptr_t c);

#define LOWER32_BITS	32
#define LOWER_HALF(x)	(x & ((1ULL << LOWER32_BITS) - 1))
#define UPPER_HALF(x)	(x >> LOWER32_BITS)

#define INIT_FIPC_MSG(m)		memset(m, 0x0, sizeof(*m))

static inline int
async_msg_get_fn_type(struct fipc_message *msg)
{
	return msg->rpc_id;
}

static inline void
async_msg_set_fn_type(struct fipc_message *msg, int type)
{
	msg->vmfunc_id = VMFUNC_RPC_CALL;
	msg->rpc_id = type;
}

#endif /* __CORETEMP_HWMON_COMMON_H__ */
