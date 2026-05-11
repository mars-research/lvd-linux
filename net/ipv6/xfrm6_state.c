// SPDX-License-Identifier: GPL-2.0
/*
 * xfrm6_state.c: based on xfrm4_state.c
 *
 * Authors:
 *	Mitsuru KANDA @USAGI
 *	Kazunori MIYAZAWA @USAGI
 *	Kunihiro Ishiguro <kunihiro@ipinfusion.com>
 *		IPv6 support
 *	YOSHIFUJI Hideaki @USAGI
 *		Split up af-specific portion
 *
 */

#include <linux/sgtable.h>
#include <net/xfrm.h>

static struct xfrm_state_afinfo xfrm6_state_afinfo = {
	.family			= AF_INET6,
	.proto			= IPPROTO_IPV6,
	.output			= xfrm6_output,
	.transport_finish	= xfrm6_transport_finish,
	.local_error		= xfrm6_local_error,
};

int __init xfrm6_state_init(void)
{
	// pr_info("ipv6_entry_gid: %lu\n", ipv6_entry_gid);
	// pr_info("ipv6_exit_gid: %lu\n", ipv6_exit_gid);
	unsigned long ii = ipv6_entry_gid + ipv6_exit_gid;
	return xfrm_state_register_afinfo(&xfrm6_state_afinfo);
}

void xfrm6_state_fini(void)
{
	xfrm_state_unregister_afinfo(&xfrm6_state_afinfo);
}
