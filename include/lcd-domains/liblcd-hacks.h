
#include <lcd-domains/liblcd-config.h>
#include <lcd-domains/liblcd.h>

/*
 * Misc macros, etc.
 */

#undef BUG
#define BUG() do {							\
		lcd_printk("BUG! in %s:%s:%d", __FILE__, __func__,	\
			__LINE__);					\
		lcd_exit(-1);						\
	} while (0)

#undef BUG_ON
#define BUG_ON(x) do {							\
	if (x) {							\
		lcd_printk("BUG! in %s:%s:%d", __FILE__, __func__,	\
			__LINE__);					\
		lcd_exit(-1);						\
	}								\
	} while (0)

#undef VM_BUG_ON
#define VM_BUG_ON(x) do {						\
	if (x) {							\
		lcd_printk("VM_BUG! in %s:%s:%d", __FILE__, __func__,	\
			__LINE__);					\
		lcd_exit(-1);						\
	}								\
	} while (0)

#undef WARN_ON_ONCE
#define WARN_ON_ONCE(x) ({ x; })

#undef EXPORT_SYMBOL
#define EXPORT_SYMBOL(x)

#undef EXPORT_SYMBOL_GPL
#define EXPORT_SYMBOL_GPL(x)

#undef EXPORT_TRACEPOINT_SYMBOL
#define EXPORT_TRACEPOINT_SYMBOL(x)

#undef panic
#define panic(fmt, args...) do {			\
	lcd_printk(fmt, ##args);				\
	lcd_exit(-1);					\
	} while(0)

#undef printk
#define printk(fmt, args...) do {		\
	lcd_printk(fmt, ##args);			\
	} while(0)

#undef printk_ratelimit
#define printk_ratelimit() true

#undef kasprintf
#define kasprintf(x, fmt, args...) ({ lcd_printk(fmt, args); (char *)1; });

#undef dump_stack
#define dump_stack() do { } while (0)

#undef unlikely
#define unlikely(x) x

#undef likely
#define likely(x) x

/*
 * NUMA / SMP / PERCPU
 */

#undef numa_mem_id
#define numa_mem_id() 0

#undef nr_cpus_node
#define nr_cpus_node(x) 1

#undef smp_processor_id
#define smp_processor_id() 0

#undef num_possible_nodes
#define num_possible_nodes() 1

#undef DEFINE_PER_CPU
#define DEFINE_PER_CPU(type, name) __typeof__(type) name

#undef percpu
#define percpu(name, cpu) name

#undef __initcall
#define __initcall(x)

#undef on_each_cpu
#define on_each_cpu(func, arg, wait) do { func(arg); } while(0)

/*
 * mm
 */
#undef gfp_pfmemalloc_allowed
#define gfp_pfmemalloc_allowed(x) true

#undef sk_memalloc_socks
#define sk_memalloc_socks() false

#undef register_cpu_notifier
#define register_cpu_notifier(x) do { } while(0)

#undef add_zone_page_state
#define add_zone_page_state(x, y, z) do { } while(0)

#undef sub_zone_page_state
#define sub_zone_page_state(x, y, z) do { } while(0)

#undef prefetchw
#define prefetchw(x) do { } while(0)

#undef totalram_pages
#define totalram_pages 0UL

#undef mm_populate
#define mm_populate(x, y) do { } while(0)

#undef gfp_allowed_mask
#define gfp_allowed_mask __GFP_BITS_MASK

#undef do_mmap_pgoff
#define do_mmap_pgoff(x1,x2,x3,x4,x5,x6,x7) 0UL

#undef KSTK_ESP
#define KSTK_ESP(x) 0UL

/*
 * Locking, interrupts, etc.
 *
 * All locking etc. is elided.
 */

#undef DEFINE_MUTEX
#define DEFINE_MUTEX(x) struct mutex x

#undef get_online_cpus
#define get_online_cpus() do { } while(0)

#undef put_online_cpus
#define put_online_cpus() do { } while(0)

#undef mutex_init
#define mutex_init(x) do { } while(0)

#undef mutex_lock
#define mutex_lock(x) do { } while(0)

#undef mutex_trylock
#define mutex_trylock(x) 1

#undef mutex_unlock
#define mutex_unlock(x) do { } while(0)

#undef mutex_lock_interruptible
#define mutex_lock_interruptible(x) 0

#undef spin_lock_init
#define spin_lock_init(x) do { } while(0)

#undef spin_lock_irqsave
#define spin_lock_irqsave(x,flags) do { } while (0)

#undef spin_lock
#define spin_lock(x) do { } while (0)

#undef spin_lock_irq
#define spin_lock_irq(x) do { } while (0)

#undef spin_lock_irqrestore
#define spin_lock_irqrestore(x,flags) do { } while (0)

#undef spin_unlock
#define spin_unlock(x) do { } while(0)

#undef spin_unlock_irq
#define spin_unlock_irq(x) do { } while(0)

#undef rcu_barrier
#define rcu_barrier() do { smp_mb(); } while(0)

#undef call_rcu
#define call_rcu(arg, func) do { func(arg); } while(0)

#undef local_irq_enable
#define local_irq_enable() do { } while(0)

#undef local_irq_disable
#define local_irq_disable() do { } while(0)

#undef might_sleep_if
#define might_sleep_if(x) do { } while (0)

#undef _raw_spin_lock
#define _raw_spin_lock(x) do { } while (0)

#undef _raw_spin_unlock
#define _raw_spin_unlock(x) do { } while (0)

static inline void force_down_read(void *x)
{
}
static inline void force_down_write(void *x)
{
}

#undef down_read
#define down_read(x) force_down_read(x)

#undef down_write
#define down_write(x) force_down_write(x)

static inline void force_up_read(void *x)
{
}
static inline void force_up_write(void *x)
{
}

#undef up_read
#define up_read(x) force_up_read(x)

#undef up_write
#define up_write(x) force_up_write(x)

#undef might_fault
#define might_fault() do { } while(0)

#undef msleep
#define msleep(x) do { } while(0)

/*
 * Copy to/from user
 */

#undef get_user_pages
#define get_user_pages(x1,x2,x3,x4,x5,x6,x7,x8) 0L

/*
 * Tracing
 */              
#undef filter_current_check_discard
#define filter_current_check_discard(x,y,z,u) 0

#undef ftrace_event_reg
#define ftrace_event_reg(x, y, z) 0

#undef ftrace_print_flags_seq
#define ftrace_print_flags_seq(x, y, z, u) NULL

#undef ftrace_raw_output_prep
#define ftrace_raw_output_prep(x, y) 0

#undef trace_buffer_unlock_commit
#define trace_buffer_unlock_commit(x, y, z, u) do { } while(0)

#undef trace_define_field
#define trace_define_field(x1,x2,x3,x4,x5,x6,x7) 0

#undef trace_event_buffer_lock_reserve
#define trace_event_buffer_lock_reserve(x1,x2,x3,x4,x5,x6) NULL

#undef trace_event_raw_init
#define trace_event_raw_init(x) 0

#undef trace_hardirqs_off
#define trace_hardirqs_off() do { } while (0)

#undef trace_hardirqs_on
#define trace_hardirqs_on() do { } while(0)

/*
 * Arch
 */
#undef cache_line_size
#define cache_line_size() 64

#undef jiffies
#define jiffies 0UL

/*
 * Security
 */

#undef security_mmap_file
#define security_mmap_file(x, y, z) 0

#undef has_capability_noaudit
#define has_capability_noaudit(x, y) 1

/*
 * Low-level page alloc hacks
 */
#undef virt_to_head_page
#define virt_to_head_page(addr) lcd_virt_to_head_page(addr)

#undef virt_to_page
#define virt_to_page(addr) lcd_virt_to_head_page(addr)

#undef alloc_pages_exact_node
#define alloc_pages_exact_node(x, y, z) lcd_alloc_pages_exact_node(x,y,z)

#undef free_memcg_kmem_pages
#define free_memcg_kmem_pages(x, y) lcd_free_memcg_kmem_pages(x, y)

#undef page_address
#define page_address(page) lcd_page_address(page)

