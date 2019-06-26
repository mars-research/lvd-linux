#ifndef NVME_STUB_H
#define NVME_STUB_H



/* TODO: Move this to a common header, say arch/x86/include/asm/udelay.h
 * As of now, lpj is passed by boot module to LCD. Devise a way to retrieve
 * this during vtx container creation so that arch module can handle this
 * seamlessly
 */
extern unsigned long loops_per_jiffy;

static void delay_loop(unsigned long loops)
{
	asm volatile(
		"	test %0,%0	\n"
		"	jz 3f		\n"
		"	jmp 1f		\n"

		".align 16		\n"
		"1:	jmp 2f		\n"

		".align 16		\n"
		"2:	dec %0		\n"
		"	jnz 2b		\n"
		"3:	dec %0		\n"

		: /* we don't need output */
		:"a" (loops)
	);
}

void __const_udelay(unsigned long xloops)
{
	int d0;

	xloops *= 4;
	asm("mull %%edx"
		:"=d" (xloops), "=&a" (d0)
		:"1" (xloops), "0" (loops_per_jiffy * (HZ/4)));

	delay_loop(++xloops);
}

/* XXX: Huh? Yeah, a tight loop for msleep is *ugly*. There is
 * no way to enable timers inside LCD. Replace this with a real
 * timer when such a support is added to LCDs.
 */
void msleep(unsigned int msecs)
{
	udelay(msecs * 1000);
}

void __udelay(unsigned long usecs)
{
	__const_udelay(usecs * 0x000010c7); /* 2**32 / 1000000 (rounded up) */
}

void usleep_range(unsigned long min, unsigned long max)
{
	udelay((max + min) >> 1);
}

#endif /* NVME_STUB_H */
