#ifndef __ACPI_HWMON_COMMON_H__
#define __ACPI_HWMON_COMMON_H__

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

#include "acpi_hwmon_glue_helper.h"

enum dispatch_t {
	ACPI_BUS_GENERATE_NETLINK_EVENT,
	ACPI_BUS_GET_DEVICE,
	ACPI_BUS_REGISTER_DRIVER,
	ACPI_BUS_UNREGISTER_DRIVER,
	ACPI_EVALUATE_INTEGER,
	ACPI_EXTRACT_PACKAGE,
	ACPI_EVALUATE_OBJECT,
	ACPI_EXCEPTION,
	DEVICE_CREATE_FILE,
	DEVICE_REMOVE_FILE,
	GET_DEVICE,
	PUT_DEVICE,
	DMI_CHECK_SYSTEM,
	HWMON_DEVICE_REGISTER,
	HWMON_DEVICE_UNREGISTER,
	KOBJECT_CREATE_AND_ADD,
	KOBJECT_PUT,
	SYSFS_CREATE_LINK,
	SYSFS_REMOVE_LINK,
	SYSFS_NOTIFY,
	ACPI_OP_ADD,
	ACPI_OP_REMOVE,
	ACPI_OP_NOTIFY,
	MODULE_INIT,
	MODULE_EXIT,
};

/* CSPACES ------------------------------------------------------------ */
int glue_cap_init(void);

int glue_cap_create(struct glue_cspace **cspace);

void glue_cap_destroy(struct glue_cspace *cspace);

void glue_cap_exit(void);

void glue_cap_remove(
	struct glue_cspace *cspace,
	cptr_t c);

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

#endif /* __ACPI_HWMON_COMMON_H__ */
