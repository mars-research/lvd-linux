/* The nasty work of reading 32 and 64-bit modules is in here. */
#include <elf.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "depmod.h"
#include "util.h"
#include "logging.h"
#include "elfops.h"
#include "tables.h"
#include "zlibsupport.h"

#include "testing.h"

/* Symbol types, returned by load_dep_syms */
static const char *weak_sym = "W";
static const char *undef_sym = "U";

/* dump_modversions helper */
static const char *skip_dot(const char *str)
{
       /* For our purposes, .foo matches foo.  PPC64 needs this. */
       if (str && str[0] == '.')
               return str + 1;
       return str;
}

#define ELF32BIT
#include "elfops_core.c"
#undef ELF32BIT

#define ELF64BIT
#include "elfops_core.c"
#undef ELF64BIT

/*
 * Check ELF file header.
 */
static int elf_ident(void *file, unsigned long fsize, int *conv)
{
	/* "\177ELF" <byte> where byte = 001 for 32-bit, 002 for 64 */
	unsigned char *ident = file;

	if (fsize < EI_CLASS || memcmp(file, ELFMAG, SELFMAG) != 0)
		return -ENOEXEC;	/* Not an ELF object */
	if (ident[EI_DATA] == 0 || ident[EI_DATA] > 2)
		return -EINVAL;		/* Unknown endianness */

	if (conv != NULL)
		*conv = native_endianness() != ident[EI_DATA];
	return ident[EI_CLASS];
}

/*
 * grab_elf_file - read ELF file into memory
 * @pathame: file to load
 *
 * Returns NULL, and errno set on error.
 */
struct elf_file *grab_elf_file(const char *pathname)
{
	struct elf_file *file;

	file = malloc(sizeof(*file));
	if (!file) {
		errno = ENOMEM;
		goto fail;
	}
	file->pathname = strdup(pathname);
	if (!file->pathname) {
		errno = ENOMEM;
		goto fail_free_file;
	}
	file->data = grab_file(pathname, &file->len);
	if (!file->data)
		goto fail_free_pathname;

	switch (elf_ident(file->data, file->len, &file->conv)) {
	case ELFCLASS32:
		file->ops = &mod_ops32;
		break;
	case ELFCLASS64:
		file->ops = &mod_ops64;
		break;
	case -ENOEXEC: /* Not an ELF object */
	case -EINVAL: /* Unknown endianness */
	default: /* Unknown word size */
		errno = ENOEXEC;
		goto fail;
	}
	return file;

fail_free_pathname:
	free(file->pathname);
fail_free_file:
	free(file);
fail:
	return NULL;
}

void release_elf_file(struct elf_file *file)
{
	int err = errno;

	if (!file)
		return;

	release_file(file->data, file->len);
	free(file->pathname);
	free(file);

	errno = err;
}
