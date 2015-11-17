
#include <linux/fs.h>

int register_filesystem(struct file_system_type *)
{
	return 0;
}

int unregister_filesystem(struct file_system_type *)
{
	return 0;
}
