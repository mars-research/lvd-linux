/**
 * @File     : benchmark.h
 * @Author   : Abhiram Balasubramanian
 *
 * Tracing function execution with rdtsc and some
 * statistics
 *
 */

#ifndef _BENCHMARK_H_
#define _BENCHMARK_H_

/* headers for writing to file */
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/fcntl.h>

#define BENCH_MAX_ENTRIES 8000000
#define MARKER_MAX_ENTRIES 8000000

static __always_inline void reset_counter(u64 *cnt, u64 *ov) {
	*(cnt) = 0;
	*ov = *ov + 1;
	return;
}

#define INIT_MARKER(name) \
	u64 name##_start[MARKER_MAX_ENTRIES] = {0};   \
       	static u64 name##_cnt;          	\
       	static char name##_id[256] = #name;     \

#define MARKER_BEGIN(name) \
	name##_start[name##_cnt] = rdtsc_begin(); \
	name##_cnt++; \
	((name##_cnt >= MARKER_MAX_ENTRIES) ?  (name##_cnt = 0) : name##_cnt);

#define MARKER_DUMP(name) \
	marker_dump_data(name##_start, name##_id);

#define INIT_BENCHMARK_DATA(name) 		\
       	u64 name##_start;     \
       	u64 name##_end;       \
       	u64 name##_diff[BENCH_MAX_ENTRIES] = {0};      \
       	static u64 name##_cnt;          		\
       	static u64 name##_ov;          		\
       	static char name##_id[256] = #name;          		\

#define INIT_BENCHMARK_DATA_LCD(name) 		\
       	u64 name##_start;     \
       	u64 name##_end;       \
       	u64 name##_diff[BENCH_MAX_ENTRIES] = {0};      \
       	static u64 name##_cnt;          		\
       	static u64 name##_ov;          		\
       	static char name##_id[256] = #name;          		\

#define BENCH_BEGIN(name) 	\
	name##_start = rdtsc_begin(); \

#define BENCH_END(name) 	\
	name##_end = rdtscp_end(); \
	name##_diff[name##_cnt] = name##_end - name##_start; \
	name##_cnt++; \
	((name##_cnt >= BENCH_MAX_ENTRIES) ?  (reset_counter(&name##_cnt, &name##_ov)) : name##_cnt);

#define BENCH_BEGIN_LCD(name) 	\
	name##_start = rdtsc_begin_lcd(); \

#define BENCH_END_LCD(name) 	\
	name##_end = rdtscp_end_lcd(); \
	name##_diff[name##_cnt] = name##_end - name##_start; \
	name##_cnt++; \
	((name##_cnt >= BENCH_MAX_ENTRIES) ?  (reset_counter(&name##_cnt, &name##_ov)) : name##_cnt);

#define BENCH_COMPUTE_STAT(name) \
	bench_compute_stat(name##_diff,name##_id,name##_cnt,name##_ov); \
	name##_cnt = 0; \
	name##_ov = 0; \

#define BENCH_DUMP_DATA(name) \
	bench_dump_data(name##_diff,name##_id);

/**
 * int_sqrt - rough approximation to sqrt
 * @x: integer of which to calculate the sqrt
 *
 * A very rough approximation to the sqrt() function.
 */
static inline unsigned long int_sqrt_local(unsigned long x)
{
        unsigned long b, m, y = 0;

        if (x <= 1)
                return x;

        m = 1UL << (BITS_PER_LONG - 2); 
        while (m != 0) {
                b = y + m;
                y >>= 1;

                if (x >= b) {
                        x -= b;
                        y += m;
                }
                m >>= 2;
        }

        return y;
}

struct bench_stat {
	u64 mean;
	u64 sum;
	u64 min;
	u64 max;
	u64 stddev;
	u64 lower;
	u64 upper;
	long var;
};

static void inline marker_dump_data(u64 *data, char *id) 
{
	int i;
	struct file *fp;
	mm_segment_t oldfs;
	loff_t offset = 0;
        char filename[128];
        char buf[512] = {0};
	int rc = -ENXIO;
	char *func;
	
	if (!data) {
		printk("[BENCH_DUMP] cannot dump data \n");
		return;
	}
	
	oldfs = get_fs();
	set_fs(get_ds());
	
	func = kzalloc((strlen(id) + 1), GFP_KERNEL);
	if(!func) {
		printk("[BENCH_DUMP] error no mem \n");
		return;
	}

	strcpy(func, id);
	sprintf(filename, "/local/devel/plot/%s",strcat(id, ".log"));

	fp = filp_open(filename, O_RDWR|O_CREAT, 0644);
	if (IS_ERR(fp)) {
		printk("[BENCH_DUMP] cannot open file to dump\n");
        	return;
    	}
	
	 printk("dumping data into file: %s \n", filename);	
	for(i = 0; i < MARKER_MAX_ENTRIES; i++) {
		
		sprintf(buf,"entry: %s ts: %llu\n", func, data[i]);

                rc = vfs_write(fp, (char __user*)buf, strlen(buf), &offset);
                if (rc < 0) {
                        printk("[BENCH_DUMP] cannot write into file \n");
                        goto out;

                }
	}

out:	
	kfree(func);
	filp_close(fp, NULL);
}

static void inline bench_dump_data(u64 *data, char *id) 
{
	int i;

	if (!data) {
		printk("[BENCH_DUMP] cannot dump data \n");
		return;
	}
	
	printk("------------------- [MARKER_DUMP - %s ] --------------------- \n", id);
	for(i = 0; i < MARKER_MAX_ENTRIES; i++) {
		if(data[i] != 0) {
			printk("entry: %s ts: %llu \n",id, data[i]);
		}
	}
	printk("------------------- [_DUMP - %s] --------------------- \n", id);
}



/* XXX May be there is a better way to do this! */
static u64 inline bench_find_base(u64 number)
{
	u64 base;
	int i = 1;

	while(1) {
		if(number / i) {
			base = i;		
			i *= 10;
		}
		else {
			break;
		}
	}
	return base;
}

static int inline bench_compute_values_within_intervals(u64 *data, u64 lower, u64 upper, u64 entries)
{
	int i;
	int count = 0;

	//for (i = 0; i < BENCH_MAX_ENTRIES; i++)
	for (i = 0; i < entries; i++)
	{
		if((data[i] >= lower) && (data[i] < upper)) {
			count ++;
		}
	}
	return count;
}

#define BENCH_MAX_BARS 10
static void inline bench_draw_histogram(u64 *data, struct bench_stat *stat, u64 entries)
{
	u64 range = 0;
	u64 base_lower = 0;
	u64 base_upper = 0;
	u64 i;

	base_lower = rounddown(stat->lower, bench_find_base(stat->lower));	
	base_upper = roundup(stat->upper, bench_find_base(stat->upper));

	printk("[BENCH_HIST] base_lower %llu, base_upper %llu \n", base_lower, base_upper);
	range = (base_upper - base_lower) / BENCH_MAX_BARS;

	for (i = base_lower; i < base_upper; i += range) {
		printk("[ %llu - %llu ]\t |-------------------| (%d) \n", i, i + range, 
			bench_compute_values_within_intervals(data, i, i+range, entries));
	}

	return;
}

/* 1.96 gives 95% of data, we will stick to 2 because of lack of float support */
#define BENCH_ZSCORE 2
static void inline bench_compute_confidence_intervals(struct bench_stat *stat)
{
	/* This gives a tight range*/
//	*lower = mean - (BENCH_ZSCORE * (sd / (int_sqrt(BENCH_MAX_ENTRIES))));
//	*upper = mean + (BENCH_ZSCORE * (sd / (int_sqrt(BENCH_MAX_ENTRIES))));
	
	/* Bound within SD */
//	stat->lower = stat->mean - (BENCH_ZSCORE * (stat->stddev));
	stat->lower = stat->min;
	stat->upper = stat->mean + (BENCH_ZSCORE * (stat->stddev));
}


static void inline bench_compute_stat(u64 *data, char *id, u64 cnt, u64 ov)
{
	struct bench_stat *stat;
	int diff = 0;
	int i;
	u64 sum;
	u64 entries = 0;

	printk("[BENCH_COMPUTE] computing stat for %s \n",id);
	
	stat = kzalloc(sizeof(*stat), GFP_KERNEL);
	if(!stat) {
		printk("[BENCH_COMPUTE] cannot allocate mem \n");
		return;
	}
	
	if(!data) {
		printk("[BENCH_COMPUTE] cannot compute stat, no data \n");
		kfree(stat);
		return;
	}
	
	entries = cnt + (ov * BENCH_MAX_ENTRIES);
	if (entries == 0) {
		printk("[BENCH_COMPUTE] not enough samples collected \n");
		return;
	}

	//if((cnt < (BENCH_MAX_ENTRIES - 1)) && (ov == 0)) {
	//	printk("[BENCH_COMPUTE] not enough samples collected \n");
	//	return;
	//}
	
	//for(sum = 0, i = 0; i < BENCH_MAX_ENTRIES; i++) {
	for(sum = 0, i = 0; i < entries; i++) {
		sum += data[i];
	}

	if(sum == 0) {
		printk("[BENCH_COMPUTE] no entries collected! \n");
		return;
	}

	//stat->mean = sum / BENCH_MAX_ENTRIES;
	stat->mean = sum / entries;

	stat->min = stat->max = data[0];
	//for(i = 0; i < BENCH_MAX_ENTRIES; i++) {
	for(i = 0; i < entries; i++) {
		if(data[i] < stat->min) {
			stat->min = data[i];
		}	
	
		if(data[i] > stat->max) {
			stat->max = data[i];
		}
		
		diff = data[i] -stat->mean;
		stat->var += diff * diff;
	}

	//stat->stddev = int_sqrt_local(stat->var / BENCH_MAX_ENTRIES);
	stat->stddev = int_sqrt_local(stat->var / entries);

	bench_compute_confidence_intervals(stat);



	printk("---------------- [BENCH_SUMMARY - %s] --------------- \n "
			"samples: \t %llu\n avg: \t %llu \n min: \t %llu \n max: \t %llu"
			"\n sdv: \t %llu \n", id, /*BENCH_MAX_ENTRIES*/ entries, stat->mean, stat->min, 
			stat->max, stat->stddev);

	bench_draw_histogram(data, stat, entries);	

	printk("[BENCH_VALUES] Total number of values in the confidence interval [%llu - %llu]"
		       "- %d \n", stat->lower, stat->upper,
			bench_compute_values_within_intervals(data, stat->lower, stat->upper, entries));
	cnt = 0;
	kfree(stat);
}	

/* rdtsc helpers */
static __always_inline u64 rdtsc_begin(void)
{

        unsigned cycles_low, cycles_high;

        asm volatile ( "CPUID\n\t"
                                   "RDTSC\n\t"
                                   "mov %%edx, %0\n\t"
                                   "mov %%eax, %1\n\t"
                                   : "=r" (cycles_high), "=r" (cycles_low)::
                                   "%rax", "%rbx", "%rcx", "%rdx");

        return ((u64) cycles_high << 32) | cycles_low;
}

static  __always_inline u64 rdtscp_end(void)
{
        unsigned cycles_low, cycles_high;

        asm volatile( "RDTSCP\n\t"
                                  "mov %%edx, %0\n\t"
                                  "mov %%eax, %1\n\t"
                                  "CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low)::
                                  "%rax", "%rbx", "%rcx", "%rdx");
    
        return ((u64) cycles_high << 32) | cycles_low;
}

/* rdtsc helpers */
static __always_inline u64 rdtsc_begin_lcd(void)
{

        unsigned cycles_low, cycles_high;

        asm volatile ("RDTSC\n\t"
                                   "mov %%edx, %0\n\t"
                                   "mov %%eax, %1\n\t"
                                   : "=r" (cycles_high), "=r" (cycles_low)::
                                   "%rax", "%rbx", "%rcx", "%rdx");

        return ((u64) cycles_high << 32) | cycles_low;
}

static  __always_inline u64 rdtscp_end_lcd(void)
{
        unsigned cycles_low, cycles_high;

        asm volatile("RDTSCP\n\t"
                                  "mov %%edx, %0\n\t"
                                  "mov %%eax, %1\n\t"
                                  : "=r" (cycles_high), "=r" (cycles_low)::
                                  "%rax", "%rbx", "%rcx", "%rdx");
    
        return ((u64) cycles_high << 32) | cycles_low;
}

#endif
