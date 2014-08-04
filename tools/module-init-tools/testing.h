#ifndef _TESTING_H
#define _TESTING_H

/* Testing code. */
#ifdef JUST_TESTING

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/utsname.h>
#include <asm/unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>

/* We don't use all of these. */
static int modtest_uname(struct utsname *buf) __attribute__((unused));
static long modtest_create_module(const char *name, size_t size)
__attribute__((unused));
static void *modtest_fopen(const char *path, const char *mode)
__attribute__((unused));
static int modtest_open(const char *path, int flags, mode_t mode)
__attribute__((unused));
static int modtest_stat(const char *file_name, struct stat *buf)
__attribute__((unused));
static int modtest_lstat(const char *file_name, struct stat *buf)
__attribute__((unused));
static DIR *modtest_opendir(const char *name) __attribute__((unused));
static int modtest_system(const char *string) __attribute__((unused));
static int modtest_rename(const char *oldpath, const char *newpath)
__attribute__((unused));
static long modtest_init_module(void *map, unsigned long size,
				const char *optstring) __attribute__((unused));
static long modtest_delete_module(const char *modname, unsigned int flags)
__attribute__((unused));

static int modtest_readlink(const char *path, char *buf, size_t bufsiz)
__attribute__((unused));
static int modtest_unlink(const char *path)
__attribute__((unused));

static int modtest_uname(struct utsname *buf)
{
	char *release = NULL;

	strcpy(buf->sysname, "Linux");
	strcpy(buf->nodename, "fakenodename");
	if ((release = getenv("MODTEST_UNAME")))
		strcpy(buf->release, release);
	else {
		printf("MODTEST_OVERRIDE_ROOT used but MODTEST_UNAME not set.\n");
		exit(1);
	}
	strcpy(buf->version, "Fakeversion");
	strcpy(buf->machine, "fakemachine");
	return 0;
}

static long modtest_create_module(const char *name, size_t size)
{
	if (getenv("MODTEST_DO_CREATE_MODULE"))
		return 0;
	errno = ENOSYS;
	return -1;
}

static const struct timespec modtest_delay = {
	.tv_sec = 0,
	.tv_nsec = 500 * 1000 * 1000
};

static long modtest_init_module(void *map, unsigned long size,
				const char *optstring)
{

	if (getenv("MODPROBE_WAIT")) {
		int fd;
		const char *file = getenv("MODPROBE_WAIT");

		printf("Looping on %s\n", file);
		fflush(stdout);
		while ((fd = open(file, O_RDONLY)) < 0)
			nanosleep(&modtest_delay, NULL);
		close(fd);
		printf("Removing %s\n", file);
		unlink(file);
	}
	if (getenv("MODTEST_DUMP_INIT")) {
		while (size) {
			int ret;
			ret = write(2, map, size);
			if (ret < 0) exit(1);
			size -= ret;
			map += ret;
		}
	} else		
		printf("INIT_MODULE: %lu %s\n", size, optstring);
	
	return 0;
}

static long modtest_delete_module(const char *modname, unsigned int flags)
{
	char flagnames[100];

	if (getenv("MODPROBE_WAIT")) {
		int fd;
		const char *file = getenv("MODPROBE_WAIT");

		printf("Looping on %s\n", file);
		fflush(stdout);
		while ((fd = open(file, O_RDONLY)) < 0)
			nanosleep(&modtest_delay, NULL);
		close(fd);
		printf("Removing %s\n", file);
		fflush(stdout);
		unlink(file);
	}
	flagnames[0] = '\0';
	if (flags & O_EXCL)
		strcat(flagnames, "EXCL ");
	if (flags & O_TRUNC)
		strcat(flagnames, "TRUNC ");
	if (flags & O_NONBLOCK)
		strcat(flagnames, "NONBLOCK ");
	if (flags & ~(O_EXCL|O_TRUNC|O_NONBLOCK))
		strcat(flagnames, "UNKNOWN ");

	printf("DELETE_MODULE: %s %s\n", modname, flagnames);
	return 0;
}

/* Add prefix to absolute paths; relative paths are left unchanged */
static const char *modtest_mapname(const char *path, char *buf, size_t buflen)
{
	char *root; 

	if (path[0] != '/')
		return path;

	root = getenv("MODTEST_OVERRIDE_ROOT");
	if (!root)
		return path;

	snprintf(buf, buflen, "%s%s", root, path);
	return buf;
}

static void *modtest_fopen(const char *path, const char *mode)
{
	char path_buf[PATH_MAX];

	path = modtest_mapname(path, path_buf, sizeof(path_buf));
	return fopen(path, mode);
}

static int modtest_open(const char *path, int flags, mode_t mode)
{
	char path_buf[PATH_MAX];

	path = modtest_mapname(path, path_buf, sizeof(path_buf));
	return open(path, flags, mode);
}

static int modtest_stat(const char *path, struct stat *buf)
{
	char path_buf[PATH_MAX];

	path = modtest_mapname(path, path_buf, sizeof(path_buf));
	return stat(path, buf);
}

static int modtest_lstat(const char *path, struct stat *buf)
{
	char path_buf[PATH_MAX];

	path = modtest_mapname(path, path_buf, sizeof(path_buf));
	return lstat(path, buf);
}

static DIR *modtest_opendir(const char *path)
{
	char path_buf[PATH_MAX];

	path = modtest_mapname(path, path_buf, sizeof(path_buf));
	return opendir(path);
}

static int modtest_system(const char *string)
{
	if (getenv("MODTEST_DO_SYSTEM"))
		return system(string);
	printf("SYSTEM: %s\n", string);
	return 0;
}

static int modtest_rename(const char *oldpath, const char *newpath)
{
	char oldpath_buf[PATH_MAX];
	char newpath_buf[PATH_MAX];

	oldpath = modtest_mapname(oldpath, oldpath_buf, sizeof(oldpath_buf));
	newpath = modtest_mapname(newpath, newpath_buf, sizeof(newpath_buf));
	return rename(oldpath, newpath);
}

static int modtest_readlink(const char *path, char *buf, size_t bufsiz)
{
	char path_buf[PATH_MAX];

	path = modtest_mapname(path, path_buf, sizeof(path_buf));
	return readlink(path, buf, bufsiz);
}

static int modtest_unlink(const char *path)
{
	char path_buf[PATH_MAX];

	path = modtest_mapname(path, path_buf, sizeof(path_buf));
	return unlink(path);
}

#ifdef CONFIG_USE_ZLIB
#include <zlib.h>
static gzFile *modtest_gzopen(const char *path, const char *mode)
__attribute__((unused));

static gzFile *modtest_gzopen(const char *path, const char *mode)
{
	char path_buf[PATH_MAX];

	path = modtest_mapname(path, path_buf, sizeof(path_buf));
	return gzopen(path, mode);
}
#endif

/* create_module call */
#undef create_module
#define create_module modtest_create_module

#define uname modtest_uname
#define delete_module modtest_delete_module
#define init_module modtest_init_module
#define open modtest_open
#define fopen modtest_fopen
#define stat(name, ptr) modtest_stat(name, ptr)
#define lstat(name, ptr) modtest_lstat(name, ptr)
#define opendir modtest_opendir
#define system modtest_system
#define rename modtest_rename
#define readlink modtest_readlink
#define unlink modtest_unlink
#define gzopen modtest_gzopen

#endif /* JUST_TESTING */
#endif /* _TESTING_H */

