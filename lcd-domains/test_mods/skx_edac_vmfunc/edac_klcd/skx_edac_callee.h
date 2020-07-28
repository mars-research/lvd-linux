#ifndef __SKX_EDAC_CALLEE_H__
#define __SKX_EDAC_CALLEE_H__

#include "../glue_helper.h"
enum dispatch_t {
	DEBUGFS_CREATE_DIR,
	DEBUGFS_CREATE_FILE,
	DEBUGFS_REMOVE_RECURSIVE,
	EDAC_MC_ADD_MC_WITH_GROUPS,
	EDAC_MC_ALLOC,
	EDAC_MC_DEL_MC,
	EDAC_MC_FREE,
	EDAC_MC_HANDLE_ERROR,
	MCE_REGISTER_DECODE_CHAIN,
	MCE_UNREGISTER_DECODE_CHAIN,
	PCI_DEV_GET,
	PCI_DEV_PUT,
	PCI_ENABLE_DEVICE,
	PCI_GET_DEVICE,
	GENERIC_FILE_LLSEEK,
	SIMPLE_ATTR_READ,
	SIMPLE_ATTR_RELEASE,
	SIMPLE_ATTR_WRITE,
	SIMPLE_ATTR_OPEN,
	X86_MATCH_CPU,
	GET,
	SET,

};
int debugfs_create_dir_callee(struct fipc_message *_request);
int debugfs_create_file_callee(struct fipc_message *_request);
int debugfs_remove_recursive_callee(struct fipc_message *_request);
int edac_mc_add_mc_with_groups_callee(struct fipc_message *_request);
int edac_mc_alloc_callee(struct fipc_message *_request);
int edac_mc_del_mc_callee(struct fipc_message *_request);
int edac_mc_free_callee(struct fipc_message *_request);
int edac_mc_handle_error_callee(struct fipc_message *_request);
int mce_register_decode_chain_callee(struct fipc_message *_request);
int mce_unregister_decode_chain_callee(struct fipc_message *_request);
int pci_dev_get_callee(struct fipc_message *_request);
int pci_dev_put_callee(struct fipc_message *_request);
int pci_enable_device_callee(struct fipc_message *_request);
int pci_get_device_callee(struct fipc_message *_request);
int generic_file_llseek_callee(struct fipc_message *_request);
int simple_attr_read_callee(struct fipc_message *_request);
int simple_attr_release_callee(struct fipc_message *_request);
int simple_attr_write_callee(struct fipc_message *_request);
int simple_attr_open_callee(struct fipc_message *_request);
int x86_match_cpu_callee(struct fipc_message *_request);
dispatch_sync_loop(struct fipc_message *message);
int dispatch_async_loop(struct fipc_message *message);
int glue_skx_edac_init(void);
void glue_skx_edac_exit(void);

#endif	/* __SKX_EDAC_CALLEE_H__ */