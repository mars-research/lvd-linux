#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../skx_edac_callee.h"
#include <lcd_config/post_hook.h>
int dispatch_async_loop(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
		case DEBUGFS_CREATE_DIR:
			trace(DEBUGFS_CREATE_DIR);
			return debugfs_create_dir_callee(message);

		case DEBUGFS_CREATE_FILE:
			trace(DEBUGFS_CREATE_FILE);
			return debugfs_create_file_callee(message);

		case DEBUGFS_REMOVE_RECURSIVE:
			trace(DEBUGFS_REMOVE_RECURSIVE);
			return debugfs_remove_recursive_callee(message);

		case EDAC_MC_ADD_MC_WITH_GROUPS:
			trace(EDAC_MC_ADD_MC_WITH_GROUPS);
			return edac_mc_add_mc_with_groups_callee(message);

		case EDAC_MC_ALLOC:
			trace(EDAC_MC_ALLOC);
			return edac_mc_alloc_callee(message);

		case EDAC_MC_DEL_MC:
			trace(EDAC_MC_DEL_MC);
			return edac_mc_del_mc_callee(message);

		case EDAC_MC_FREE:
			trace(EDAC_MC_FREE);
			return edac_mc_free_callee(message);

		case EDAC_MC_HANDLE_ERROR:
			trace(EDAC_MC_HANDLE_ERROR);
			return edac_mc_handle_error_callee(message);

		case MCE_REGISTER_DECODE_CHAIN:
			trace(MCE_REGISTER_DECODE_CHAIN);
			return mce_register_decode_chain_callee(message);

		case MCE_UNREGISTER_DECODE_CHAIN:
			trace(MCE_UNREGISTER_DECODE_CHAIN);
			return mce_unregister_decode_chain_callee(message);

		case PCI_DEV_GET:
			trace(PCI_DEV_GET);
			return pci_dev_get_callee(message);

		case PCI_DEV_PUT:
			trace(PCI_DEV_PUT);
			return pci_dev_put_callee(message);

		case PCI_ENABLE_DEVICE:
			trace(PCI_ENABLE_DEVICE);
			return pci_enable_device_callee(message);

		case PCI_GET_DEVICE:
			trace(PCI_GET_DEVICE);
			return pci_get_device_callee(message);

		case GENERIC_FILE_LLSEEK:
			trace(GENERIC_FILE_LLSEEK);
			return generic_file_llseek_callee(message);

		case SIMPLE_ATTR_READ:
			trace(SIMPLE_ATTR_READ);
			return simple_attr_read_callee(message);

		case SIMPLE_ATTR_RELEASE:
			trace(SIMPLE_ATTR_RELEASE);
			return simple_attr_release_callee(message);

		case SIMPLE_ATTR_WRITE:
			trace(SIMPLE_ATTR_WRITE);
			return simple_attr_write_callee(message);

		case SIMPLE_ATTR_OPEN:
			trace(SIMPLE_ATTR_OPEN);
			return simple_attr_open_callee(message);

		case X86_MATCH_CPU:
			trace(X86_MATCH_CPU);
			return x86_match_cpu_callee(message);

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;

	}
	return 0;

}

