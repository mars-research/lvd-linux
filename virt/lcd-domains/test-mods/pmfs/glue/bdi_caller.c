
#include <linux/backing-dev.h>

int bdi_init(struct backing_dev_info *bdi)
{
	return 0;
}

void bdi_destroy(struct backing_dev_info *bdi)
{
	return;
}

