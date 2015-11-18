
#include <linux/fs.h>
#include <lcd-domains/liblcd.h>

int register_filesystem(struct file_system_type *fs)
{
	return 0;
}

int unregister_filesystem(struct file_system_type *fs)
{
	return 0;
}
