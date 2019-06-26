#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../nvme_caller.h"
#include <lcd_config/post_hook.h>

#define trace(x) LIBLCD_MSG("net got " #x " msg")

int __nvme_lcd_init(void);
void __nvme_lcd_exit(void);

extern unsigned long loops_per_jiffy;

int handle_rpc_calls(struct fipc_message *message)
{
	int fn_type;

	fn_type = async_msg_get_fn_type(message);

	switch (fn_type) {
		case MODULE_INIT:
			trace(MODULE_INIT);
			loops_per_jiffy = fipc_get_reg0(message);
			return __nvme_lcd_init();

		case MODULE_EXIT:
			trace(MODULE_EXIT);
			__nvme_lcd_exit();
			break;
		case MSIX_IRQ_HANDLER:
			msix_vector_handler_callee(message);
			break;
            
		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;
	}
	return 0;
}
