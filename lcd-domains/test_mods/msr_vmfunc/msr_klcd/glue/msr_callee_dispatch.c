#include <lcd_config/pre_hook.h>
#include <liblcd/liblcd.h>
#include "../msr_callee.h"
#include <lcd_config/post_hook.h>
int dispatch_async_loop(struct fipc_message *message)
{
	int fn_type;
	fn_type = async_msg_get_fn_type(message);
	switch (fn_type) {
		case __CLASS_CREATE:
			trace(__CLASS_CREATE);
			return __class_create_callee(message);

		case CAPABLE:
			trace(CAPABLE);
			return capable_callee(message);

		case CLASS_DESTROY:
			trace(CLASS_DESTROY);
			return class_destroy_callee(message);

		case DEVICE_CREATE:
			trace(DEVICE_CREATE);
			return device_create_callee(message);

		case DEVICE_DESTROY:
			trace(DEVICE_DESTROY);
			return device_destroy_callee(message);

		case NO_SEEK_END_LLSEEK:
			trace(NO_SEEK_END_LLSEEK);
			return no_seek_end_llseek_callee(message);

		case CPU_MAPS_UPDATE_BEGIN:
			trace(CPU_MAPS_UPDATE_BEGIN);
			return cpu_maps_update_begin_callee(message);

		case CPU_MAPS_UPDATE_DONE:
			trace(CPU_MAPS_UPDATE_DONE);
			return cpu_maps_update_done_callee(message);

		case RDMSR_SAFE_ON_CPU:
			trace(RDMSR_SAFE_ON_CPU);
			return rdmsr_safe_on_cpu_callee(message);

		case RDMSR_SAFE_REGS_ON_CPU:
			trace(RDMSR_SAFE_REGS_ON_CPU);
			return rdmsr_safe_regs_on_cpu_callee(message);

		case __REGISTER_CHRDEV:
			trace(__REGISTER_CHRDEV);
			return __register_chrdev_callee(message);

		case __UNREGISTER_CHRDEV:
			trace(__UNREGISTER_CHRDEV);
			return __unregister_chrdev_callee(message);

		case WRMSR_SAFE_ON_CPU:
			trace(WRMSR_SAFE_ON_CPU);
			return wrmsr_safe_on_cpu_callee(message);

		case WRMSR_SAFE_REGS_ON_CPU:
			trace(WRMSR_SAFE_REGS_ON_CPU);
			return wrmsr_safe_regs_on_cpu_callee(message);

		default:
			LIBLCD_ERR("unexpected function label: %d",
					fn_type);
			return -EINVAL;

	}
	return 0;

}

