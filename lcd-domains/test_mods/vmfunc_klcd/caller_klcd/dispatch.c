#include <lcd_config/pre_hook.h>

#include <liblcd/liblcd.h>
#include "../rdtsc_helper.h"
#include "../rpc.h"
#include <linux/module.h>
#include <linux/rtnetlink.h>
#include <linux/kthread.h>

#include <lcd_config/post_hook.h>

unsigned long noinline
null_invocation(void)
{
       return 10;
}

unsigned long noinline
dummy(struct fipc_message *msg)
{
	rtnl_lock();
	null_invocation();
	rtnl_unlock();
	return 0;
}

unsigned long noinline
bar(struct fipc_message *msg)
{
	printk("%s, bar called with reg0: %lx, reg1: %lx, reg2, %lx, reg3: %lx, reg4: %lx, reg5: %lx reg6: %lx\n",
			__func__,
			fipc_get_reg0(msg),
			fipc_get_reg1(msg),
			fipc_get_reg2(msg),
			fipc_get_reg3(msg),
			fipc_get_reg4(msg),
			fipc_get_reg5(msg),
			fipc_get_reg6(msg));
	fipc_set_reg1(msg, 0x100);
	schedule();
	kthread_bind(current, 4);
	return 0;
}

static void hwbp_handler(struct perf_event *bp,
             struct perf_sample_data *data,
             struct pt_regs *regs)
{
	dump_stack();
	printk(KERN_INFO "Dump stack from sample_hbp_handler\n");
}

int register_wide_hw_breakpoint_callee(struct fipc_message *request)
{
	struct perf_event * __percpu *sample_hbp;
	struct perf_event_attr attr;
	int ret = 0;

	hw_breakpoint_init(&attr);

	attr.bp_addr = fipc_get_reg3(request);
	attr.bp_len = fipc_get_reg1(request);
	attr.bp_type = fipc_get_reg2(request);

	sample_hbp = register_wide_hw_breakpoint(&attr,
			(perf_overflow_handler_t) hwbp_handler, NULL);

	if (IS_ERR((void __force *)sample_hbp)) {
		ret = PTR_ERR((void __force *)sample_hbp);
		printk(KERN_INFO "Breakpoint registration failed: %d\n", ret);
		fipc_set_reg0(request, 0);
	} else {
		printk("%s registered hwbp for addr %llx perf: %p\n", __func__,
				attr.bp_addr, sample_hbp);
		fipc_set_reg0(request, (unsigned long) sample_hbp);
	}
	return ret;
}

int unregister_wide_hw_breakpoint_callee(struct fipc_message *request)
{
	struct perf_event * __percpu *sample_hbp;

	sample_hbp = (struct perf_event * __percpu *) fipc_get_reg0(request);
	printk("%s perf: %p\n", __func__, sample_hbp);

	unregister_wide_hw_breakpoint(sample_hbp);

	return 0;
}

int handle_rpc_calls_klcd(struct fipc_message *msg)
{
	switch(msg->rpc_id) {
	case NULL_INVOCATION:
		null_invocation();
		break;
	case BAR:
		bar(msg);
		break;
	case DUMMY:
		dummy(msg);
		break;

	case REGISTER_WIDE_HW_BREAKPOINT:
		//trace(REGISTER_WIDE_HW_BREAKPOINT);
		return register_wide_hw_breakpoint_callee(msg);

	case UNREGISTER_WIDE_HW_BREAKPOINT:
		//trace(REGISTER_WIDE_HW_BREAKPOINT);
		return unregister_wide_hw_breakpoint_callee(msg);

	default:
		break;
	};
	return 0;
}
