#ifndef _LIBLCD_SKBUFF_H_
#define _LIBLCD_SKBUFF_H_

#include <linux/skbuff.h>
#include <libcap_types.h>

struct sk_buff_container {
	struct sk_buff skb;
	cptr_t other_ref;
	cptr_t my_ref;
};

void lcd_consume_skb(struct sk_buff *skb);
#endif /* _LIBLCD_SKBUFF_H_ */
