#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/lcd-domains.h>

int do_mmap(FILE *f, size_t order, unsigned char **blob_addr_out)
{
	void *ret;
	size_t len;

	len = (1 << size) * PAGE_SIZE;

	ret = mmap(NULL, len, 
		PROT_EXEC | PROT_READ | PROT_WRITE, 
		MAP_ANONYMOUS, fileno(f), 0);
	if (ret == MAP_FAILED) {
		return -1;
	}

	*blob_addr_out = (unsigned char *)ret;
	return 0;
}

void usage(void)
{
	fprintf(stderr, "blob: usage: ./blob <blob fname> <blob order>\n");
	fprintf(stderr, "  blob should be 2^(blob order) pages\n");
	exit(1);
}

int main(int argc, char *argv[])
{
	int ret;
	struct lcd_blob_info bi;
	char *fname;
	int order;
	FILE *f;
	
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
	f = fopen(fname, "r");
	if (!f) {
		fprintf(stderr, "blob: error opening file %s\n",
			fname);
		exit(-1);
	}
	if (do_mmap(f, size, &(bi.blob))) {
		fprintf(stderr, "blob: error mapping file %s\n",
			fname);
		fclose(f);
		exit(-1);
	}
	bi.blob_order = order;

	/*
	 * Run in lcd
	 */
	ret = ioctl(LCD_MINOR, LCD_RUN_BLOB, &bi);
	if (ret) {
		printf("blob: ioctl returned error %d\n", ret);
		return -1;
	}

	return 0;
}
