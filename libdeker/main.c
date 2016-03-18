/*
 * main.c
 *
 * This contains the main function, as required by Smack.
 */
#include <lcd_config/pre_hook.h>

#include <linux/module.h>

#include <lcd_config/post_hook.h>

int main(void)
{
	return init_module();
}
