#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../ixgbe_callee.h"
#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("net got " #x " msg")

int dispatch_sync_loop(void)
{
	int fn_type = lcd_r0();

	switch (fn_type) {

	default:
		LIBLCD_ERR("unexpected function label %d", fn_type);
		return -EINVAL;
	}

	return 0;
}

int dispatch_async_loop(struct thc_channel *_channel,
		struct fipc_message *message,
		struct glue_cspace *cspace,
		struct cptr sync_ep)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
		case __PCI_REGISTER_DRIVER:
			trace(__PCI_REGISTER_DRIVER);
			return __pci_register_driver_callee(message,
		_channel,
		cspace,
		sync_ep);

		case PCI_UNREGISTER_DRIVER:
			trace(PCI_UNREGISTER_DRIVER);
			return pci_unregister_driver_callee(message,
		_channel,
		cspace,
		sync_ep);

		case ALLOC_ETHERDEV_MQS:
			trace(ALLOC_ETHERDEV_MQS);
			return alloc_etherdev_mqs_callee(message,
		_channel,
		cspace,
		sync_ep);

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;

	}
	return 0;

}

