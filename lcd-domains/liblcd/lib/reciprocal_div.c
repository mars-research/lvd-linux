
/* BEGIN LCD */
#include <lcd-domains/liblcd-config.h>
/* END LCD */

#include <asm/div64.h>
#include <linux/reciprocal_div.h>
#include <linux/export.h>

/* BEGIN LCD */
#include <lcd-domains/liblcd-hacks.h>
/* END LCD */

u32 reciprocal_value(u32 k)
{
	u64 val = (1LL << 32) + (k - 1);
	do_div(val, k);
	return (u32)val;
}
EXPORT_SYMBOL(reciprocal_value);
