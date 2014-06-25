/*
 * Code for mapping and running blob in lcd.
 *
 * Depending on where linux headers are installed,
 * may need to change header includes. For example,
 *
 *   sudo headers_install INSTALL_HDR_PATH=/usr/include/temp
 *
 * will install all include/uapi headers in /usr/include/temp/include/...
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <temp/include/linux/lcd-domains.h>

#define PAGE_SIZE 4096
#define DEVICE_NAME "/dev/lcd"

int do_mmap(int blob_fd, size_t order, unsigned char **blob_addr_out)
{
	void *ret;
	size_t len;

	len = (1 << order) * PAGE_SIZE;

	ret = mmap(NULL, len, 
		PROT_EXEC | PROT_READ | PROT_WRITE, 
		MAP_PRIVATE, blob_fd, 0);
	if (ret == MAP_FAILED) {
		return -1;
	}

	*blob_addr_out = (unsigned char *)ret;

	return 0;
}

void usage(void)
{
	fprintf(stderr, "blob-run: usage: ./blob <blob fname> <blob order>\n");
	fprintf(stderr, "  blob should be 2^(blob order) pages\n");
	exit(1);
}

int main(int argc, char *argv[])
{
	int ret;
	struct lcd_blob_info bi;
	char *fname;
	int order;
	int blob_fd;
	int lcd_fd;
	
	/*
	 * Get blob file name
	 */
	if (argc != 3) {
		usage();
	}
	fname = argv[1];
	order = atoi(argv[2]);

	/*
	 * Open and map blob
	 */
	blob_fd = open(fname, O_RDONLY);
	if (blob_fd < 0) {
		fprintf(stderr, "blob-run: error opening file %s\n",
			fname);
		goto fail;
	}
	if (do_mmap(blob_fd, order, &(bi.blob))) {
		fprintf(stderr, "blob-run: error mapping file %s\n",
			fname);
		goto fail_map;
	}
	bi.blob_order = order;

	/*
	 * Run in lcd
	 */
	lcd_fd = open(DEVICE_NAME, O_RDONLY);
	if (lcd_fd < 0) {
		perror("blob-run");
		printf("blob-run: error opening lcd device\n");
		goto fail_open;
	}
	ret = ioctl(lcd_fd, LCD_RUN_BLOB, &bi);
	if (ret) {
		printf("blob-run: ioctl returned error %d\n", ret);
		goto fail_ioctl;
	}

	close(lcd_fd);
	close(blob_fd);

	return 0;

fail_ioctl:
	close(lcd_fd);
fail_open:
fail_map:
	close(blob_fd);
fail:
	return -1;
}
