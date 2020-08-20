#ifndef _RDTSC_HELPER_H
#define _RDTSC_HELPER_H

#include <linux/log2.h>

#define ABSOLUTE(x) ((x) > 0 ? (x) : -(x))
#define OUTLIER_THRESHOLD 3000

#define TS_DECL(name)	uint64_t st_##name, en_##name
#define TS_START(name)  st_##name = RDTSC_START()
#define TS_STOP(name)  en_##name = RDTSC_STOP()
#define TS_START_LCD(name)  st_##name = lcd_RDTSC_START()
#define TS_STOP_LCD(name)  en_##name = lcd_RDTSC_STOP()
#define TS_DIFF(name)  (en_##name - st_##name)

typedef union {
    struct {
        unsigned cyc_low;
        unsigned cyc_high;
    };
    uint64_t cycles;
} rdtsc_t;

typedef struct {
	rdtsc_t	start;
	rdtsc_t end;
} rdtsc_marker_t;

typedef struct fipc_test_stat_t
{
	uint64_t N;			// The size of the sample set
	uint64_t mean;			// The mean of the data
	uint64_t total;			// The mean of the data
	uint64_t stdev;		// The standard deviation
	uint64_t abdev;		// The mean absolute deviation
	uint64_t min;		// The minimum value
	uint64_t max;		// The maximum value
	uint64_t outliers;	// The number of data points > OUTLIER_THRESHOLD
	uint64_t normMean;		// The normalized mean, which account for outliers
	uint64_t normStdev;	// The normalized standard deviation
	uint64_t normAbdev;	// The normalized absolute deviation
	uint64_t normMax;	// The normalized maximum value
} test_stat_t;

static __always_inline void warmup_icache(void)
{
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
    asm volatile ("CPUID\n\t"
                "RDTSC\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
                "%rax", "%rbx", "%rcx", "%rdx");
    asm volatile("RDTSCP\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t"
                "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
                "%rbx", "%rcx", "%rdx");

    asm volatile ("CPUID\n\t"
                "RDTSC\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
                "%rax", "%rbx", "%rcx", "%rdx");

    asm volatile("RDTSCP\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t"
                "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
                "%rbx", "%rcx", "%rdx");
}

static __always_inline uint64_t RDTSC_START(void)
{
    unsigned cycles_low, cycles_high;

//    asm volatile ("CPUID\n\t"
	asm volatile(
            "RDTSC\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t": "=r" (cycles_high), "=r"
            (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");

    return (((uint64_t)cycles_high << 32) | cycles_low);
}

static __always_inline uint64_t RDTSC_STOP(void)
{
    unsigned cycles_low1, cycles_high1;

    asm volatile("RDTSCP\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t"
            //"CPUID\n\t": "=r" (cycles_high1), "=r"
            : "=r" (cycles_high1), "=r"
            (cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx");

    return (((uint64_t)cycles_high1 << 32) | cycles_low1);
}

static __always_inline uint64_t lcd_RDTSC_START(void)
{
    unsigned cycles_low, cycles_high;

    asm volatile (
            "RDTSC\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t": "=r" (cycles_high), "=r"
            (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");

    return (((uint64_t)cycles_high << 32) | cycles_low);
}

static __always_inline uint64_t lcd_RDTSC_STOP(void)
{
    unsigned cycles_low1, cycles_high1;

    asm volatile("RDTSCP\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t": "=r" (cycles_high1), "=r"
            (cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx");

    return (((uint64_t)cycles_high1 << 32) | cycles_low1);
}

/**
 * This function returns the value with the specified zScore.
 */
static inline
uint64_t fipc_test_stat_zrange_value ( test_stat_t* stat, uint64_t zScore )
{
	return stat->normMean + zScore * stat->normStdev;
}

/**
 * This function returns the zScore with the specified value.
 */
static inline
uint64_t fipc_test_stat_zscore_value ( test_stat_t* stat, uint64_t value )
{
  return (value - stat->normMean) / stat->normStdev;
}

/**
 * This function counts the number of data points in the zScore range.
 */
static inline
uint64_t fipc_test_stat_count_in_zrange ( uint64_t* sample_set, uint64_t sample_size, test_stat_t* stat, uint64_t zScoreMin, uint64_t zScoreMax )
{
	uint64_t value_below = fipc_test_stat_zrange_value( stat, zScoreMin );
	uint64_t value_above = fipc_test_stat_zrange_value( stat, zScoreMax );

	uint64_t i;
	uint64_t count;

	for ( count = 0, i = 0; i < sample_size; ++i )
		if ( sample_set[i] >= value_below && sample_set[i] <  value_above )
			++count;

	return count;
}

/**
 * This function prints one bar in a histogram corresponding to the range given by zScoreMin and zScoreMax.
 */
static inline
int fipc_test_stat_print_zrange_bar ( uint64_t* sample_set, uint64_t sample_size, test_stat_t* stat, uint64_t zScoreMin, uint64_t zScoreMax )
{
	char bar[51];

	uint64_t count   = fipc_test_stat_count_in_zrange( sample_set, sample_size, stat, zScoreMin, zScoreMax );
	uint64_t X_value = sample_size / 50;
	uint64_t X_count = count / X_value;

	bar[50] = '\0';

	memset( bar, 'X', X_count );
	memset( bar + X_count, ' ', 50 - X_count );

	printk ( "%s : %llu", bar, count );
	return 0;
}

/**
 * This function prints a histogram of the data.
 */
static inline
int fipc_test_stat_print_zhistogram ( uint64_t* sample_set, uint64_t sample_size, test_stat_t* stat )
{

	uint64_t i = (fipc_test_stat_zscore_value( stat, stat->min ) + 1);

	uint64_t value_below = stat->min;
	uint64_t maxZScore;
	uint64_t value_above = fipc_test_stat_zrange_value( stat, i );
	
	printk ( "%llu -> %llu\t: ", value_below, value_above );
	fipc_test_stat_print_zrange_bar( sample_set, sample_size, stat, fipc_test_stat_zscore_value( stat, stat->min ), i );
	printk( "\n" );
	
	maxZScore = fipc_test_stat_zscore_value( stat, stat->max );
	for ( ; i < maxZScore && i < 3; i += 1 )
	{
		value_below = fipc_test_stat_zrange_value( stat, i );
		value_above = ( i+1 <= maxZScore ? fipc_test_stat_zrange_value( stat, i+1 ) : stat->max );
		
		printk ( "%llu -> %llu\t: ", value_below, value_above );
		fipc_test_stat_print_zrange_bar( sample_set, sample_size, stat, i, ( i+1 <= maxZScore ? i+1 : maxZScore ) );
		printk( "\n" );
	}

	return 0;
}

static inline uint64_t fipc_test_stat_get_mean ( uint64_t* sample_set, uint64_t sample_size, test_stat_t *stat )
{
	uint64_t sum;
	uint64_t i;

	for ( sum = 0, i = 0; i < sample_size; ++i )
	{
		sum += sample_set[i];
	}
	stat->total = sum;
	if (is_power_of_2(sample_size))
		return sum >> ilog2(sample_size);
	else
		return sum / sample_size;
}

/**
 * This function returns the basic statistics about the sample set
 */
static inline
int fipc_test_stat_get_stats ( uint64_t* sample_set, uint64_t sample_size, test_stat_t* stat )
{
	register uint64_t stdevSum = 0;
	register uint64_t abdevSum = 0;
	register uint64_t normSum = 0;
	register uint64_t normStdevSum = 0;
	register uint64_t normAbdevSum = 0;
	register uint64_t i;


	// Error Checking
	if ( stat == NULL )
		return -1;

	stat->mean = fipc_test_stat_get_mean ( sample_set, sample_size, stat);
	stat->N    = sample_size;

	stat->min = sample_set[0];
	stat->max = sample_set[0];
	stat->normMax  = sample_set[0];
	stat->outliers = 0;

	for ( i = 0; i < sample_size; ++i )
	{
		uint64_t dev = ABSOLUTE( sample_set[i] - stat->mean );
		abdevSum += dev;
		stdevSum += dev*dev;

		if ( sample_set[i] < stat->min )
			stat->min = sample_set[i];

		if ( sample_set[i] > stat->max )
			stat->max = sample_set[i];

		if ( sample_set[i] > OUTLIER_THRESHOLD )
		{
			stat->outliers++;
		}
		else
		{
			normSum += sample_set[i];
			normAbdevSum += dev;
			normStdevSum += dev*dev;

			if ( sample_set[i] > stat->normMax )
				stat->normMax = sample_set[i];
		}
	}

	if (is_power_of_2(sample_size)) {
		stat->stdev = int_sqrt(stdevSum
					>> ilog2(sample_size));
		stat->abdev = abdevSum >> ilog2(sample_size);
	} else {
		stat->stdev = int_sqrt( stdevSum / sample_size );
		stat->abdev = abdevSum / sample_size;
	}
	if (sample_size - stat->outliers) {
		stat->normMean = normSum /
				( sample_size - stat->outliers );
		stat->normStdev = int_sqrt( normStdevSum /
				( sample_size - stat->outliers ) );
		stat->normAbdev = normAbdevSum /
				( sample_size - stat->outliers );
	}

	return 0;
}

static inline int fipc_test_stat_print_info ( uint64_t* sample_set, uint64_t sample_size )
{
	test_stat_t stats;

	// Error Check
	if ( sample_set == NULL || sample_size == 0 )
		return -1;

	fipc_test_stat_get_stats( sample_set, sample_size, &stats );
	// Print Statistics
	printk ( "-------------------------------------------------------------------------------\n" );

	printk ( "Mean : %llu Total: %llu | Min: %llu | Max: %llu"
		 " | SD: %llu\n",
			stats.mean, stats.total,
			stats.min, stats.max, stats.stdev);

#if 0
	printk ( "Minimum value: %llu\n", stats.min );
	printk ( "Maximum value: %llu\n", stats.max );

	printk ( "Standard Deviation: %llu\n", stats.stdev );
	printk ( "Absolute Deviation: %llu\n", stats.abdev );
	printk ( "Outlier Count: %llu\n", stats.outliers );
	printk ( "Normalized Mean: %llu\n", stats.normMean );
	printk ( "Normalized Standard Deviation: %llu\n", stats.normStdev );
	printk ( "Normalized Absolute Deviation: %llu\n", stats.normAbdev );
	printk ( "Normalized Maximum value: %llu\n", stats.normMax );
	printk ( "\n" );
	//fipc_test_stat_print_zhistogram( sample_set, sample_size, &stats );
	printk ( "\n" );
	printk ( "Summary:\nnorm mean:\t%llu;\tnorm abs dev:\t%llu\nmin:\t\t%llu;\t\tmax:\t\t%llu\n", 
				stats.normMean, stats.normAbdev, stats.min, stats.max );
#endif
	printk ( "-------------------------------------------------------------------------------\n" );
	return 0;
}

#define EXPANDH(r)	r.cyc_high
#define EXPANDL(r)	r.cyc_low

#define ___RDTSC_START(r)   \
                    do { \
                    asm volatile ( \
                    "CPUID\n\t" \
                    "RDTSC\n\t" \
                    "mov %%edx, %0\n\t" \
                    "mov %%eax, %1\n\t" \
                    : "=r" (EXPANDH(r)), "=r" (EXPANDL(r)) \
                    : : "%rax", "%rbx", "%rcx", "%rdx"); \
                    } while(0)

#define ___RDTSC_END(r)   \
                    do { \
                    asm volatile ( \
                    "RDTSCP\n\t" \
                    "mov %%edx, %0\n\t" \
                    "mov %%eax, %1\n\t" \
                    "CPUID\n\t" \
                    : "=r" (EXPANDH(r)), "=r" (EXPANDL(r)) \
                    : : "%rax", "%rbx", "%rcx", "%rdx"); \
                    } while(0)


static __always_inline void __RDTSC_START(rdtsc_t *r)
{
    asm volatile ("CPUID\n\t"
            "RDTSC\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t": "=r" (r->cyc_high), "=r"
            (r->cyc_low):: "%rax", "%rbx", "%rcx", "%rdx");
}

static __always_inline void __RDTSC_STOP(rdtsc_t *r)
{
    asm volatile("RDTSCP\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t"
            "CPUID\n\t": "=r" (r->cyc_high), "=r"
            (r->cyc_low):: "%rax", "%rbx", "%rcx", "%rdx");
}

#define M(stamp)		marker_##stamp
#define _TS_DECL(mvar)		rdtsc_marker_t M(mvar)
#define _TS_START(mvar)		___RDTSC_START(M(mvar).start)
#define _TS_STOP(mvar)		___RDTSC_END(M(mvar).end)
#define _TS_DIFF(mvar)		(M(mvar).end.cycles - \
				M(mvar).start.cycles)	

#endif /* _RDTSC_HELPER_H */
