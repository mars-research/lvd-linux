
#include <lcd_config/pre_hook.h>

#include <linux/compiler.h>
#include <linux/spinlock_types.h>
#include <linux/ftrace_event.h>
#include <linux/perf_event.h>
#include <liblcd/liblcd.h>

#include <lcd_config/post_hook.h>

/*
 * Some globals to resolve
 * -----------------------
 *
 * We need a fake task struct for slab, inside kmem_freepages.
 */
struct task_struct fake = {0};
struct task_struct *current_task = &fake;
/*
 * Make sure these cause trouble. This kernel_stack value is non-canonical,
 * so will hopefully cause a GP exception. This phys_base sets bits past
 * what is most likely the highest allowed position (past MAX_PHYS).
 */
unsigned long kernel_stack = 0x800000badbadf00dUL;
unsigned long phys_base = 0x800000badbadf00dUL;
 
unsigned long _copy_from_user(void *to, const void __user *from, unsigned n)
{
	lcd_printk("resolve.c: called dummy _copy_from_user!");
	return 0UL;
}

void warn_slowpath_null(const char *file, int line)
{
	lcd_printk("resolve.c: called dummy warn_slowpath_null!");
	lcd_printk("warn_slowpath_null: file = %s, line = %d",
		file, line);
}

void perf_tp_event(u64 addr, u64 count, void *record,
		int entry_size, struct pt_regs *regs,
		struct hlist_head *head, int rctx,
		struct task_struct *task)
{
	lcd_printk("resolve.c: trying to call dummy perf_tp_event!");
	return;
}
               
void *perf_trace_buf_prepare(int size, unsigned short type,
				struct pt_regs *regs, int *rctxp)
{
	lcd_printk("resolve.c: trying to call dummy perf_trace_buf_prepare!");
	return NULL;
}

void *ring_buffer_event_data(struct ring_buffer_event *event)
{
	lcd_printk("resolve.c: trying to call dummy ring_buffer_event_data!");
	return NULL;
}

long strnlen_user(const char __user *str, long count)
{
	lcd_printk("resolve.c: trying to call dummy strnlen_user!");
	return 0L;
}

void __lockfunc _raw_spin_unlock_irqrestore(raw_spinlock_t *lock, 
					unsigned long flags)
{
	lcd_printk("resolve.c: trying to call dummy _raw_spin_unlock_irqrestore!");
	return;
}

/* MUTEXES ------------------------------------------------------------ */

void __mutex_init(struct mutex *lock, const char *name, 
		struct lock_class_key *key)
{
	return;
}

void mutex_lock(struct mutex *lock)
{
	return;
}

int mutex_lock_interruptible(struct mutex *lock)
{
	return 0;
}

int mutex_trylock(struct mutex *lock)
{
	return 1;
}

void mutex_unlock(struct mutex *lock)
{
	return;
}

/* SCHEDULING ------------------------------------------------------------ */

int _cond_resched(void) { return 0; } /* Never signal reschedule */

/* PRINTK -------------------------------------------------- */

int printk(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	_lcd_printk(fmt, args);
	va_end(args);
	return 0;
}
