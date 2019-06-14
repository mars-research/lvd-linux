
#include <lcd_config/pre_hook.h>

#include <linux/compiler.h>
#include <linux/spinlock_types.h>
#include <linux/ring_buffer.h>
#include <linux/perf_event.h>
#include <liblcd/liblcd.h>

#include <lcd_config/post_hook.h>

/*
 * Some globals to resolve
 * -----------------------
 *
 * We need a fake task struct for slab, inside kmem_freepages.
 */
struct cred fake_cred;
struct task_struct fake_task = {
	.pid = 12345678,
	.cred = &fake_cred,
};
struct task_struct *current_task = &fake_task;

struct cpumask __cpu_possible_mask;
struct cpumask __cpu_online_mask;
struct cpumask __cpu_present_mask;
struct cpumask __cpu_active_mask;

/*
 * Make sure these cause trouble. This kernel_stack value is non-canonical,
 * so will hopefully cause a GP exception. This phys_base sets bits past
 * what is most likely the highest allowed position (past MAX_PHYS).
 */
unsigned long kernel_stack = 0x800000badbadf00dUL;
unsigned long phys_base = 0x800000badbadf00dUL;
extern union vmfunc_state_page vmfunc_state_page;
 
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

void perf_tp_event(u16 event_type, u64 count, void *record,
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

void get_random_bytes(void *buf, int nbytes)
{
	return;
}

unsigned long get_seconds(void)
{
	return 0;
}

void __init_waitqueue_head(wait_queue_head_t *q, const char *name, 
			struct lock_class_key *k)
{
	return;
}

struct task_struct fake_kthread;
struct task_struct *kthread_create_on_node(int (*threadfn)(void *data),
					void *data, int node,
					const char namefmt[],
					...)
{
	return &fake_kthread;
}

bool kthread_should_stop(void)
{
	return false;
}

int kthread_stop(struct task_struct *k)
{
	return 0;
}

int wake_up_process(struct task_struct *tsk)
{
	return 0;
}

void __wake_up(wait_queue_head_t *q, unsigned int mode, int nr, void *key)
{
	return;
}

/* IOREMAPs -------------------------------------------------- */

void *ioremap_cache(resource_size_t phys_addr, unsigned long size)
{
	return lcd_ioremap_cache(phys_addr, size);
}

void *ioremap_cache_ro(resource_size_t phys_addr, unsigned long size)
{
	return lcd_ioremap_cache(phys_addr, size);
}

void *ioremap_hpage_cache(resource_size_t phys_addr, unsigned long size)
{
	return lcd_ioremap_cache(phys_addr, size);
}

void *ioremap_hpage_cache_ro(resource_size_t phys_addr, unsigned long size)
{
	return lcd_ioremap_cache(phys_addr, size);
}

void iounmap(volatile void __iomem *addr)
{
	lcd_iounmap((void *)addr);
}

void __release_region(struct resource *parent, resource_size_t start,
		resource_size_t n)
{
	return;
}

struct resource iomem_resource;
struct resource fake_resource;
struct resource * __request_region(struct resource *parent,
				resource_size_t start, resource_size_t n,
				const char *name, int flags)
{
	return &fake_resource;
}

/* RCU ------------------------------------------------------------ */

void call_rcu_sched(struct rcu_head *head,
		void (*func)(struct rcu_head *rcu))
{
	/* Just call it */
	func(head);
}

/* DUMP STACK -------------------------------------------------- */

void dump_stack(void)
{
	lcd_dump_stack();
}
