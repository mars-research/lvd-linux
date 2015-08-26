/**
 * init.c
 *
 * Nothing much here. These are invoked by the microkernel when it boots,
 * so that individual parts can initialize.
 *
 * Authors:
 *   Charlie Jacobsen  <charlesj@cs.utah.edu>
 */

int __kliblcd_init(void)
{
	/* Do nothing for now */
	return 0;
}

void __kliblcd_exit(void)
{
	return;
}
