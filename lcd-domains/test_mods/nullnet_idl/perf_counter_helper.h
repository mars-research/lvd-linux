#ifndef _PERF_COUNTER_HELPER_H
#define _PERF_COUNTER_HELPER_H

#include <asm/msr.h>    /* for r(w)dmsrl_safe */

typedef union {
    struct {
        u8 evt_sel;
        u8 umask;
        u8 usr_mode:1,
        os_mode:1,
        edge_detect:1,
        pin_ctrl:1,
        int_enable:1,
        any_thread:1,
        enable:1,
        invert_cmask:1;
        u8 ctr_mask;
        u32 reserved;
    };
    u64 reg;
} evt_sel_t;

typedef enum {
    OS_MODE = 1,
    USR_MODE = 2,
} ev_mode_t;

/* TODO: Generalize this for different cpus */
typedef enum {
	EVENT_SEL0,
	EVENT_SEL1,
	EVENT_SEL2,
	EVENT_SEL3,
	EVENT_SEL_MAX,
} evtsel_idx;

typedef enum {
	FIXED_CTR0,
	FIXED_CTR1,
	FIXED_CTR2,
	FIXED_CTR_MAX,
} fixed_ctr_idx;

#define L1D_CACHE_LD_I_STATE	(0x40, 0x01)
#define L1D_CACHE_LD_S_STATE	(0x40, 0x02)
#define L1D_CACHE_LD_E_STATE	(0x40, 0x04)
#define L1D_CACHE_LD_M_STATE	(0x40, 0x08)

#define VAL(x)		cval_##x

#define DECL_EVENT(ev)	evt_sel_t ev; u64 VAL(ev)

#define FILL_EVENT_OS(name, EVENT, UMASK)	\
			fill_event(EVENT, UMASK, OS_MODE, name)

#define PROG_EVENT(ev, idx)	\
			write_event(ev, idx)

#define STOP_EVENT(idx)	stop_event(idx)

#define RESET_COUNTER(idx)	reset_counter(idx)

#define READ_PMC(val, idx)	read_counter(val, idx)

/* Architectural performance counters - Addresses remain constant
 * across cpu families
 */
/* From arch/x86/include/asm/msr-index.h */
/* Intel Core-based CPU performance counters */
#define MSR_CORE_PERF_FIXED_CTR0	    0x00000309
#define MSR_CORE_PERF_FIXED_CTR1	    0x0000030a
#define MSR_CORE_PERF_FIXED_CTR2	    0x0000030b
#define MSR_CORE_PERF_FIXED_CTR_CTRL	0x0000038d
#define MSR_CORE_PERF_GLOBAL_STATUS	    0x0000038e
#define MSR_CORE_PERF_GLOBAL_CTRL	    0x0000038f
#define MSR_CORE_PERF_GLOBAL_OVF_CTRL	0x00000390

/* event select register of GP perf counter */
#define MSR_IA32s_EVNTSEL0		        0x00000186
#define MSR_IA32_PERFCTR0		        0x000000c1

#define FIXED_CTR_CTRL_ENABLE_MASK(x)	(0x3 << (x*4))
#define FIXED_CTR_CTRL_DISABLE_MASK(x)	\
				(~FIXED_CTR_CTRL_ENABLE_MASK(x))

static inline void fill_event(u8 evt_sel, u8 umask, ev_mode_t mode,
					evt_sel_t *e)
{
    e->reg = 0ull;
    e->evt_sel = evt_sel;
    e->umask = umask;
    if (mode == OS_MODE)
        e->os_mode = 1;
    else
        e->usr_mode = 1;

    e->enable = 1;
}

static inline void write_event(evt_sel_t *e, u64 gpidx)
{
        wrmsrl_safe(MSR_IA32s_EVNTSEL0 + gpidx, e->reg);
}

static inline void stop_event(u64 gpidx)
{
        wrmsrl_safe(MSR_IA32s_EVNTSEL0 + gpidx, 0ull);
}

static inline void read_counter(u64 *val, u64 gpidx)
{
	u64 _val;
	rdmsrl_safe(MSR_IA32_PERFCTR0 + gpidx, &_val);
	*val = _val;
}

static inline void reset_counter(u64 gpidx)
{
	wrmsrl_safe(MSR_IA32_PERFCTR0 + gpidx, 0ull);
}

static inline void reset_fcounter(u64 gpidx)
{
	wrmsrl_safe(MSR_CORE_PERF_FIXED_CTR0 + gpidx, 0ull);
}

/* TODO: or and write the regs */
static inline void stop_fcounter(u64 gpidx)
{
	wrmsrl_safe(MSR_CORE_PERF_FIXED_CTR_CTRL,
			FIXED_CTR_CTRL_DISABLE_MASK(gpidx));
}

static inline void start_fcounter(u64 gpidx)
{
	wrmsrl_safe(MSR_CORE_PERF_FIXED_CTR_CTRL,
			FIXED_CTR_CTRL_ENABLE_MASK(gpidx));
}
#endif /* _PERF_COUNTER_HELPER_H */
