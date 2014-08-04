/*
 * depmod.c: generate module dependency meta-data (aliases, etc.)
 *
 * (C) 2002 Rusty Russell IBM Corporation
 * (C) 2006-2011 Jon Masters <jcm@jonmasters.org>, and others.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
#define _GNU_SOURCE /* asprintf */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/utsname.h>
#include <sys/mman.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#include "util.h"
#include "zlibsupport.h"
#include "depmod.h"
#include "logging.h"
#include "index.h"
#include "elfops.h"
#include "tables.h"
#include "config_filter.h"

#include "testing.h"

#ifndef MODULE_DIR
#define MODULE_DIR "/lib/modules/"
#endif

#ifndef MODULE_BUILTIN_KEY
#define MODULE_BUILTIN_KEY "built-in"
#endif

/* used to replace one module with another based on conf override entries */
struct module_overrides
{
	/* Next override */
	struct module_overrides *next;

	/* overridden module */
	char *modfile;
};

/* used to specify the order in which /lib/modules subdirs are searched */
struct module_search
{
	/* Next search */
	struct module_search *next;

	/* search path */
	char *search_path;
	size_t len;
};

static char sym_prefix; /* used for -P option */
static unsigned int skipchars; /* prefix target part of basedir to skip over */
static unsigned int make_map_files = 1; /* default to on */
static unsigned int force_map_files = 0; /* default to on */

#define SYMBOL_HASH_SIZE 1024
struct symbol
{
	struct symbol *next;
	struct module *owner;
	uint64_t ver;
	char name[0];
};

static struct symbol *symbolhash[SYMBOL_HASH_SIZE];

/**
 * tdb_hash - calculate hash entry for a symbol (algorithm from gdbm, via tdb)
 *
 * @name:	symbol name
 *
 */
static inline unsigned int tdb_hash(const char *name)
{
	unsigned value;	/* Used to compute the hash value.  */
	unsigned   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	for (value = 0x238F13AF * strlen(name), i=0; name[i]; i++)
		value = (value + (((unsigned char *)name)[i] << (i*5 % 24)));

	return (1103515243 * value + 12345);
}

/**
 * skip_symprefix - remove extraneous prefix character on some architectures
 *
 * @symname:	symbol name to process for prefix character removal
 *
 */
static const char *skip_symprefix(const char *symname)
{
	return symname + (symname[0] == sym_prefix ? 1 : 0);
}

/**
 * add_symbol - add a symbol to the symbol hash list
 *
 * @name:	symbol name
 * @ver:	symbol version (checksum)
 * @owner:	module owning this symbol
 *
 */
static void add_symbol(const char *name, uint64_t ver, struct module *owner)
{
	unsigned int hash;
	struct symbol *new = NOFAIL(malloc(sizeof *new + strlen(name) + 1));

	new->owner = owner;
	new->ver = ver;
	strcpy(new->name, name);

	hash = tdb_hash(name) % SYMBOL_HASH_SIZE;
	new->next = symbolhash[hash];
	symbolhash[hash] = new;
}

static int print_unknown, check_symvers;

/**
 * find_symbol - lookup module owning a symbol in the symbol hash list
 *
 * @name:	symbol name
 * @ver:	symbol version
 * @modname:	pathname of module requesting lookup (being processed)
 * @weak:	whether the symbol is weakly defined or not
 *
 * This function is used during dependency calculation to match the
 * dependencies a module says it requires with symbols we have seen.
 * calculate_deps calls us after it has load_dep_syms on a module.
 *
 */
static struct module *find_symbol(const char *name, uint64_t ver,
		const char *modname, int weak)
{
	struct symbol *s;

	/* For our purposes, .foo matches foo.  PPC64 needs this. */
	if (name[0] == '.')
		name++;
	name = skip_symprefix(name);

	for (s = symbolhash[tdb_hash(name) % SYMBOL_HASH_SIZE]; s; s=s->next) {
		if (streq(s->name, name))
			break;
	}
	if (s) {
		if (ver && s->ver && s->ver != ver && print_unknown && !weak)
			warn("%s disagrees about version of symbol %s\n",
					modname, name);
		return s->owner;
	}

	if (print_unknown && !weak)
		warn("%s needs unknown symbol %s\n", modname, name);

	return NULL;
}

/**
 * add_dep - add a module dependency
 *
 * @mod:	module name
 * @depends_on:	new module dependency
 *
 */
static void add_dep(struct module *mod, struct module *depends_on)
{
	unsigned int i;

	for (i = 0; i < mod->num_deps; i++)
		if (mod->deps[i] == depends_on)
			return;

	mod->deps = NOFAIL(realloc(mod->deps, sizeof(mod->deps[0])*(mod->num_deps+1)));
	mod->deps[mod->num_deps++] = depends_on;
}

/**
 * add_fake_syms - symbols not explicitly otherwise provided
 *
 * The kernel provides a few dependencies within the module loader.
 *
 */
static void add_fake_syms(void)
{
	/* __this_module is magic inserted by kernel loader. */
	add_symbol("__this_module", 0, NULL);
	/* On S390, this is faked up too */
	add_symbol("_GLOBAL_OFFSET_TABLE_", 0, NULL);
}

/**
 * load_system_map - load list of symbols from the System.map
 *
 * @filename:	path to the System.map-like file
 *
 */
static void load_system_map(const char *filename)
{
	FILE *system_map;
	char line[10240];
	const char ksymstr[] = "__ksymtab_";
	const int ksymstr_len = strlen(ksymstr);

	system_map = fopen(filename, "r");
	if (!system_map)
		fatal("Could not open '%s': %s\n", filename, strerror(errno));

	/* eg. c0294200 R __ksymtab_devfs_alloc_devnum */
	while (fgets(line, sizeof(line)-1, system_map)) {
		char *ptr;
		const char *cptr;

		/* Snip \n */
		ptr = strchr(line, '\n');
		if (ptr)
			*ptr = '\0';

		ptr = strchr(line, ' ');
		if (!ptr || !(ptr = strchr(ptr + 1, ' ')))
			continue;

		/* Skip the space before symbol name */
		cptr = skip_symprefix(ptr + 1);

		/* Covers gpl-only and normal symbols. */
		if (strstarts(cptr, ksymstr))
			add_symbol(cptr + ksymstr_len, 0, NULL);
	}

	fclose(system_map);
	add_fake_syms();
}

/**
 * load_module_symvers - load list of symbol versions from the module.symvers
 *
 * @filename:	path to the module.symvers-like file
 *
 */
static void load_module_symvers(const char *filename)
{
	FILE *module_symvers;
	char line[10240];

	module_symvers = fopen(filename, "r");
	if (!module_symvers)
		fatal("Could not open '%s': %s\n", filename, strerror(errno));

	/* eg. "0xb352177e\tfind_first_bit\tvmlinux\tEXPORT_SYMBOL" */
	while (fgets(line, sizeof(line)-1, module_symvers)) {
		const char *ver, *sym, *where;

		ver = strtok(line, " \t");
		sym = strtok(NULL, " \t");
		where = strtok(NULL, " \t");
		if (!ver || !sym || !where)
			continue;

		if (streq(where, "vmlinux"))
			add_symbol(skip_symprefix(sym), strtoull(ver, NULL, 16), NULL);
	}

	fclose(module_symvers);
	add_fake_syms();
}

static const struct option options[] = { { "all", 0, NULL, 'a' },
				   { "quick", 0, NULL, 'A' },
				   { "basedir", 1, NULL, 'b' },
				   { "config", 1, NULL, 'C' },
				   { "symvers", 1, NULL, 'E' },
				   { "filesyms", 1, NULL, 'F' },
				   { "errsyms", 0, NULL, 'e' },
				   { "unresolved-error", 0, NULL, 'u' },
				   { "quiet", 0, NULL, 'q' },
				   { "root", 0, NULL, 'r' },
				   { "verbose", 0, NULL, 'v' },
				   { "show", 0, NULL, 'n' },
				   { "dry-run", 0, NULL, 'n' },
				   { "symbol-prefix", 0, NULL, 'P' },
				   { "help", 0, NULL, 'h' },
				   { "version", 0, NULL, 'V' },
				   { "warn", 0, NULL, 'w' },
				   { "map", 0, NULL, 'm' },
				   { NULL, 0, NULL, 0 } };

/**
 * is_version_number - is the option a kernel version or module name
 *
 * @version:	possible version number
 *
 */
static int is_version_number(const char *version)
{
	unsigned int dummy;

	return (sscanf(version, "%u.%u", &dummy, &dummy) == 2);
}

/**
 * old_module_version - is the kernel version too old for these tools
 *
 * @version:	version number
 *
 */
static int old_module_version(const char *version)
{
	/* Expect three part version (but won't fail it only two part). */
	unsigned int major, sub, minor;

	sscanf(version, "%u.%u.%u", &major, &sub, &minor);

	if (major > 2) return 0;
	if (major < 2) return 1;

	/* 2.x */
	if (sub > 5) return 0;
	if (sub < 5) return 1;

	/* 2.5.x */
	if (minor >= 48) return 0;
	return 1;
}

/**
 * print_usage - output a list of all possible options
 *
 * @name: not currently used
 *
 */
static void print_usage(const char *name)
{
	fprintf(stderr,
	"%s " VERSION " -- part of " PACKAGE "\n"
	"%s -[aA] [-n -e -v -q -V -r -u -w -m]\n"
	"      [-b basedirectory] [forced_version]\n"
	"depmod [-n -e -v -q -r -u -w] [-F kernelsyms] module1.ko module2.ko ...\n"
	"If no arguments (except options) are given, \"depmod -a\" is assumed\n"
	"\n"
	"depmod will output a dependancy list suitable for the modprobe utility.\n"
	"\n"
	"\n"
	"Options:\n"
	"\t-a, --all            Probe all modules\n"
	"\t-A, --quick          Only does the work if there's a new module\n"
	"\t-e, --errsyms        Report not supplied symbols\n"
	"\t-m, --map            Create the legacy map files\n"
	"\t-n, --show           Write the dependency file on stdout only\n"
	"\t-P, --symbol-prefix  Architecture symbol prefix\n"
	"\t-V, --version        Print the release version\n"
	"\t-v, --verbose        Enable verbose mode\n"
	"\t-w, --warn           Warn on duplicates\n"
	"\t-h, --help           Print this usage message\n"
	"\n"
	"The following options are useful for people managing distributions:\n"
	"\t-b basedirectory\n"
	"\t    --basedir basedirectory    Use an image of a module tree.\n"
	"\t-F kernelsyms\n"
	"\t    --filesyms kernelsyms      Use the file instead of the\n"
	"\t                               current kernel symbols.\n"
	"\t-E Module.symvers\n"
	"\t    --symvers Module.symvers   Use Module.symvers file to check\n"
	"\t                               symbol versions.\n",
	"depmod", "depmod");
}

/**
 * ends_in - check file extension
 *
 * @name:	filename
 * @ext:	extension to check
 *
 */
static int ends_in(const char *name, const char *ext)
{
	unsigned int namelen, extlen;

	/* Grab lengths */
	namelen = strlen(name);
	extlen = strlen(ext);

	if (namelen < extlen) return 0;

	if (streq(name + namelen - extlen, ext))
		return 1;
	return 0;
}

/**
 * grab_module - open module ELF file and load symbol data
 *
 * @dirname:	path prefix
 * @filename:	filename within path
 *
 */
static struct module *grab_module(const char *dirname, const char *filename)
{
	struct module *new;

	new = NOFAIL(malloc(sizeof(*new)
			    + strlen(dirname?:"") + 1 + strlen(filename) + 1));
	if (dirname)
		sprintf(new->pathname, "%s/%s", dirname, filename);
	else
		strcpy(new->pathname, filename);
	new->basename = my_basename(new->pathname);

	INIT_LIST_HEAD(&new->dep_list);
	new->order = INDEX_PRIORITY_MIN;

	new->file = grab_elf_file(new->pathname);
	if (!new->file) {
		warn("Can't read module %s: %s\n",
		     new->pathname, strerror(errno));
		free(new);
		return NULL;
	}
	return new;
}

/* We use this on-stack structure to track recursive calls to has_dep_loop */
struct module_traverse
{
	struct module_traverse *prev;
	struct module *mod;
};

/**
 * in_loop - determine if a module dependency loop exists
 *
 * @mod:	current module
 * @traverse:	on-stack structure created as module deps were processed
 *
 */
static int in_loop(struct module *mod, const struct module_traverse *traverse)
{
	const struct module_traverse *i;

	for (i = traverse; i; i = i->prev) {
		if (i->mod == mod)
			return 1;
	}
	return 0;
}

/**
 * report_loop - report (once) that a dependency loop exists for a module
 *
 * @mod:	module with dep loop
 * @traverse:	on-stack structure created as module deps were processed
 *
 */
static void report_loop(const struct module *mod,
			const struct module_traverse *traverse)
{
	const struct module_traverse *i;

	/* Check that start is least alphabetically.  eg.  a depends
	   on b depends on a will get reported for a, not b.  */
	for (i = traverse->prev; i->prev; i = i->prev) {
		if (strcmp(mod->pathname, i->mod->pathname) > 0)
			return;
	}

	/* Is start in the loop?  If not, don't report now. eg. a
	   depends on b which depends on c which depends on b.  Don't
	   report when generating depends for a. */
	if (mod != i->mod)
		return;

	warn("Loop detected: %s ", mod->pathname);
	for (i = traverse->prev; i->prev; i = i->prev)
		fprintf(stderr, "needs %s ", i->mod->basename);
	fprintf(stderr, "which needs %s again!\n", i->mod->basename);
}

/**
 * has_dep_loop - iterate over all module deps and check for loops
 *
 * @module:	module to process
 * @prev:	previously processed dependency
 *
 * This function is called recursively, following every dep and creating
 * a module_traverse on the stack describing each dependency encountered.
 * Determining a loop is as simple (and slow) as finding repetitions.
 *
 * This is slow, but we can't leave the user without any modules, so we
 * need to detect loops and just fail those modules that cause loops.
 *
 */
static int has_dep_loop(struct module *module, struct module_traverse *prev)
{
	unsigned int i;
	struct module_traverse traverse = { .prev = prev, .mod = module };

	if (in_loop(module, prev)) {
		report_loop(module, &traverse);
		return 1;
	}

	for (i = 0; i < module->num_deps; i++)
		if (has_dep_loop(module->deps[i], &traverse))
			return 1;
	return 0;
}

/**
 * order_dep_list - expand all module deps recursively and in order
 *
 * @start:	module being processed
 * @mod:	recursive dep
 *
 * We expand all of the dependencies of the dependencies of a module
 * and ensure that the lowest dependency is loaded first, etc.
 *
 */
static void order_dep_list(struct module *start, struct module *mod)
{
	unsigned int i;

	for (i = 0; i < mod->num_deps; i++) {
		/* If it was previously depended on, move it to the
		   tail.  ie. if a needs b and c, and c needs b, we
		   must order b after c. */
		list_del(&mod->deps[i]->dep_list);
		list_add_tail(&mod->deps[i]->dep_list, &start->dep_list);
		order_dep_list(start, mod->deps[i]);
	}
}

static struct module *deleted = NULL;

/**
 * del_module - remove from list of modules
 *
 * @modules:	list of modules
 * @delme:	module to remove
 *
 */
static void del_module(struct module **modules, struct module *delme)
{
	struct module **i;

	/* Find pointer to it. */ 
	if (modules) {
		for (i = modules; *i != delme; i = &(*i)->next);
		
		*i = delme->next;
	}
	
	/* Save on a list to quiet valgrind.
	   Can't free - other modules may depend on them */
	delme->next = deleted;
	deleted = delme;
}

/**
 * compress_path - strip out common path prefix for modules
 *
 * @path:	path to module
 * @basedir:	top level modules directory
 *
 * Modules are typically located in /lib/modules. There is no need to
 * store the same common path prefix for all modules - make paths
 * relative to directory that contains the dep information files.
 *
 */
static const char *compress_path(const char *path, const char *basedir)
{
	int len = strlen(basedir);

	if (strncmp(path, basedir, len) == 0)
		path += len + 1;
	return path;
}

/**
 * output_deps - create ascii text file representation of module deps
 *
 * @modules:	list of modules
 * @out:	output file
 * @dirname:	output directory
 *
 */
static int output_deps(struct module *modules,
			FILE *out, char *dirname)
{
	struct module *i;

	for (i = modules; i; i = i->next) {
		struct list_head *j, *tmp;
		order_dep_list(i, i);

		fprintf(out, "%s:", compress_path(i->pathname, dirname));
		list_for_each_safe(j, tmp, &i->dep_list) {
			struct module *dep
				= list_entry(j, struct module, dep_list);
			fprintf(out, " %s",
			        compress_path(dep->pathname, dirname));
			list_del_init(j);
		}
		fprintf(out, "\n");
	}
	return 1;
}

/* warn whenever duplicate module aliases, deps, or symbols are found. */
static int warn_dups = 0;

/**
 * output_deps_bin - create binary trie representation of module deps
 *
 * @modules:	list of modules
 * @out:	output binary file
 * @dirname:	output directory
 *
 * This optimized dependency file contains an ordered structure that is
 * more easily processed by modprobe in a time sensitive manner.
 *
 */
static int output_deps_bin(struct module *modules,
			FILE *out, char *dirname)
{
	struct module *i;
	struct index_node *index;
	char *line;
	char *p;

	index = index_create();

	for (i = modules; i; i = i->next) {
		struct list_head *j, *tmp;
		char modname[strlen(i->pathname)+1];
		
		order_dep_list(i, i);
		
		filename2modname(modname, i->pathname);
		nofail_asprintf(&line, "%s:",
		                compress_path(i->pathname, dirname));
		p = line;
		list_for_each_safe(j, tmp, &i->dep_list) {
			struct module *dep
				= list_entry(j, struct module, dep_list);
			nofail_asprintf(&line, "%s %s",
			                p,
			                compress_path(dep->pathname, dirname));
			free(p);
			p = line;
			list_del_init(j);
		}
		if (index_insert(index, modname, line, i->order) && warn_dups)
			warn("duplicate module deps:\n%s\n",line);
		free(line);
	}
	
	index_write(index, out);
	index_destroy(index);

	return 1;
}

/**
 * smells_like_module - detect common module extensions
 *
 * @name:	filename
 *
 */
static int smells_like_module(const char *name)
{
	return ends_in(name,".ko") || ends_in(name, ".ko.gz");
}

typedef struct module *(*do_module_t)(const char *dirname,
				      const char *filename,
				      struct module *next,
				      struct module_search *search,
				      struct module_overrides *overrides);

/**
 * is_higher_priority - find modules replacing other modules
 *
 * @newpath:	new module path
 * @oldpath:	old module path
 * @search:	path search order
 * @overrides:	module override directives
 *
 * Compares one module (path) to another module (path) and determines
 * whether the new module should replace the existing module of the
 * same name. Overriding is handled very coarsely for the moment.
 *
 */
static int is_higher_priority(const char *newpath, const char *oldpath,
			      struct module_search *search,
			      struct module_overrides *overrides)
{
	struct module_search *tmp;
	struct module_overrides *ovtmp;
	int i = 0;
	int prio_builtin = -1;
	int prio_new = -1;
	int prio_old = -1;

/* The names already match, now we check for overrides and directory search
 * order
 */
	for (ovtmp = overrides; ovtmp != NULL; ovtmp = ovtmp->next) {
		if (streq(ovtmp->modfile, newpath))
			return 1;
		if (streq(ovtmp->modfile, oldpath))
			return 0;
	}
	for (i = 0, tmp = search; tmp != NULL; tmp = tmp->next, i++) {
		if (streq(tmp->search_path, MODULE_BUILTIN_KEY))
			prio_builtin = i;
		else if (strncmp(tmp->search_path, newpath, tmp->len) == 0)
			prio_new = i;
		else if (strncmp(tmp->search_path, oldpath, tmp->len) == 0)
			prio_old = i;
	}
	if (prio_new < 0)
		prio_new = prio_builtin;
	if (prio_old < 0)
		prio_old = prio_builtin;

	return prio_new > prio_old;
}

/**
 * do_module - process a module file
 *
 * @dirname:	directory containing module
 * @filename:	module disk file
 * @list:	list of modules
 * @search:	path search order
 * @overrides:	module override directives
 *
 */
static struct module *do_module(const char *dirname,
				       const char *filename,
				       struct module *list,
				       struct module_search *search,
				       struct module_overrides *overrides)
{
	struct module *new, **i;

	new = grab_module(dirname, filename);
	if (!new)
		return list;

	/* Check if module is already in the list. */
	for (i = &list; *i; i = &(*i)->next) {

		if (streq((*i)->basename, filename)) {
			char newpath[strlen(dirname) + strlen("/")
				      + strlen(filename) + 1];

			sprintf(newpath, "%s/%s", dirname, filename);

			/* if module matches an existing entry (name) but */
			/* has a higher priority, replace existing entry. */
			if (is_higher_priority(newpath, (*i)->pathname,search,
					       overrides)) {
				del_module(i, *i);
				
				new->next = *i;
				*i = new;
			} else
				del_module(NULL, new);

			return list;
		}
	}

	/* Not in the list already. Just prepend. */
	new->next = list;
	return new;
}

/**
 * grab_dir - process a directory of modules
 *
 * @dirname:	directory name
 * @dir:	open directory reference
 * @do_mod:	do_module function to use
 * @search:	path search order
 * @overrides:	module overrides directives
 *
 */
static struct module *grab_dir(const char *dirname,
			       DIR *dir,
			       struct module *next,
			       do_module_t do_mod,
			       struct module_search *search,
			       struct module_overrides *overrides)
{
	struct dirent *dirent;

	while ((dirent = readdir(dir)) != NULL) {
		if (smells_like_module(dirent->d_name))
			next = do_mod(dirname, dirent->d_name, next,
				      search, overrides);
		else if (!streq(dirent->d_name, ".")
			 && !streq(dirent->d_name, "..")
			 && !streq(dirent->d_name, "source")
			 && !streq(dirent->d_name, "build")) {

			DIR *sub;
			char subdir[strlen(dirname) + 1
				   + strlen(dirent->d_name) + 1];
			sprintf(subdir, "%s/%s", dirname, dirent->d_name);
			sub = opendir(subdir);
			if (sub) {
				next = grab_dir(subdir, sub, next, do_mod,
						search, overrides);
				closedir(sub);
			}
		}
	}
	return next;
}

/**
 * grab_basedir - top-level module processing
 *
 * @dirname:	top-level directory name
 * @search:	path search order
 * @overrides:	module overrides directives
 *
 */
static struct module *grab_basedir(const char *dirname,
				   struct module_search *search,
				   struct module_overrides *overrides)
{
	DIR *dir;
	struct module *list;

	dir = opendir(dirname);
	if (!dir) {
		warn("Couldn't open directory %s: %s\n",
		     dirname, strerror(errno));
		return NULL;
	}
	list = grab_dir(dirname, dir, NULL, do_module, search, overrides);
	closedir(dir);

	return list;
}

/**
 * sort_modules - order modules in list on modules.order if available
 *
 * @dirname:	directory name
 * @list:	module list
 *
 * Using the modules.order file (if available), give every module an index
 * based on its position in the file, and order list based on the index. If
 * no ordering data is available, fallback to existing unordered list.
 *
 */
static struct module *sort_modules(const char *dirname, struct module *list)
{
	struct module *tlist = NULL, **tpos = &tlist;
	FILE *modorder;
	int dir_len = strlen(dirname) + 1;
	char file_name[dir_len + strlen("modules.order") + 1];
	char line[10240];
	unsigned int linenum = 0;

	sprintf(file_name, "%s/%s", dirname, "modules.order");

	modorder = fopen(file_name, "r");
	if (!modorder) {
		/* Older kernels don't generate modules.order.  Just
		   return if the file doesn't exist. */
		if (errno == ENOENT)
			return list;
		fatal("Could not open '%s': %s\n", file_name, strerror(errno));
	}

	sprintf(line, "%s/", dirname);

	/* move modules listed in modorder file to tlist in order */
	while (fgets(line, sizeof(line), modorder)) {
		struct module **pos, *mod;
		int len = strlen(line);

		linenum++;
		if (line[len - 1] == '\n')
			line[len - 1] = '\0';

		for (pos = &list; (mod = *pos); pos = &(*pos)->next) {
			if (streq(line, mod->pathname + dir_len)) {
				mod->order = linenum;
				*pos = mod->next;
				mod->next = NULL;
				*tpos = mod;
				tpos = &mod->next;
				break;
			}
		}
	}

	/* append the rest */
	*tpos = list;

	fclose(modorder);

	return tlist;
}

/**
 * calculate_deps - calculate deps for module
 *
 * @module:	module to process
 *
 */
static void calculate_deps(struct module *module)
{
	unsigned int i;
	struct string_table *symnames;
	struct string_table *symtypes;
	uint64_t *symvers = NULL;
	struct elf_file *file;

	module->num_deps = 0;
	module->deps = NULL;
	file = module->file;

	symnames = file->ops->load_dep_syms(file, &symtypes,
			check_symvers ? &symvers : NULL);
	if (!symnames || !symtypes)
		return;

	for (i = 0; i < symnames->cnt; i++) {
		const char *name;
		uint64_t ver;
		struct module *owner;
		int weak;

		name = symnames->str[i];
		ver = symvers ? symvers[i] : 0;
		weak = (*(symtypes->str[i]) == 'W');
		owner = find_symbol(name, ver, module->pathname, weak);
		if (owner) {
			info("%s needs \"%s\": %s\n",
			       module->pathname, name,
			       owner->pathname);
			add_dep(module, owner);
		}
	}

	free(symnames);
	free(symtypes);
	free(symvers);
}

/**
 * parse_modules - process the modules list
 *
 * @module:	module list
 *
 * Process each module in the (sorted by sort_modules) list for symbols,
 * dependencies, and other meta-data that will be output later.
 *
 */
static struct module *parse_modules(struct module *list)
{
	struct module *i;
	struct elf_file *file;
	struct string_table *syms;
	int j;

	for (i = list; i; i = i->next) {
		uint64_t *symvers = NULL;
		file = i->file;
		syms = file->ops->load_symbols(file,
				check_symvers ? &symvers : NULL);
		if (syms) {
			for (j = 0; j < syms->cnt; j++)
				add_symbol(skip_symprefix(syms->str[j]),
					symvers ? symvers[j] : 0, i);
			strtbl_free(syms);
		}
		free(symvers);
		file->ops->fetch_tables(file, &i->tables);
	}
	
	for (i = list; i; i = i->next)
		calculate_deps(i);
	
	/* Strip out modules with dependency loops. */
 again:
	for (i = list; i; i = i->next) {
		if (has_dep_loop(i, NULL)) {
			warn("Module %s ignored, due to loop\n",
			     i->pathname + skipchars);
			del_module(&list, i);
			goto again;
		}
	}
	
	return list;
}

/**
 * output_symbols - output symbol alias information
 *
 * @unused:	unused
 * @out:	output file reference
 * @dirname:	output directory
 *
 * Output the symbol hash table, in the form of symbol alias entries, to
 * an ascii text file of the form modules.symbols (depending on file).
 *
 */
static int output_symbols(struct module *unused, FILE *out, char *dirname)
{
	unsigned int i;

	fprintf(out, "# Aliases for symbols, used by symbol_request().\n");
	for (i = 0; i < SYMBOL_HASH_SIZE; i++) {
		struct symbol *s;

		for (s = symbolhash[i]; s; s = s->next) {
			if (s->owner) {
				char modname[strlen(s->owner->pathname)+1];
				filename2modname(modname, s->owner->pathname);
				fprintf(out, "alias symbol:%s %s\n",
					s->name, modname);
			}
		}
	}
	return 1;
}

/**
 * output_symbols_bin - output symbol alias information in binary format
 *
 * @unused:	unused
 * @out:	output file reference
 * @dirname:	output directory
 *
 * Output the symbol hash table, in the form of symbol alias entries, to
 * a trie ordered output file e.g. of the form modules.symbols.bin.
 *
 */
static int output_symbols_bin(struct module *unused, FILE *out, char *dirname)
{
	struct index_node *index;
	unsigned int i;
	char *alias;
	int duplicate;

	index = index_create();
	
	for (i = 0; i < SYMBOL_HASH_SIZE; i++) {
		struct symbol *s;

		for (s = symbolhash[i]; s; s = s->next) {
			if (s->owner) {
				char modname[strlen(s->owner->pathname)+1];
				filename2modname(modname, s->owner->pathname);
				nofail_asprintf(&alias, "symbol:%s", s->name);
				duplicate = index_insert(index, alias, modname,
							 s->owner->order);
				if (duplicate && warn_dups)
					warn("duplicate module syms:\n%s %s\n",
						alias, modname);
				free(alias);
			}
		}
	}
	
	index_write(index, out);
	index_destroy(index);

	return 1;
}

/**
 * output_builtin_bin - output list of built-in modules in binary format
 *
 * @unused:	unused
 * @out:	output file reference
 * @dirname:	output directory
 *
 */
static int output_builtin_bin(struct module *unused, FILE *out, char *dirname)
{
	struct index_node *index;
	char *textfile, *line;
	unsigned int linenum;
	FILE *f;

	nofail_asprintf(&textfile, "%s/modules.builtin", dirname);
	if (!(f = fopen(textfile, "r"))) {
		if (errno != ENOENT)
			fatal("Could not open '%s': %s\n",
					textfile, strerror(errno));
		free(textfile);
		return 0;
	}
	free(textfile);
	index = index_create();

	while ((line = getline_wrapped(f, &linenum)) != NULL) {
		char *module = line;

		if (!*line || *line == '#') {
			free(line);
			continue;
		}
		filename2modname(module, module);
		index_insert(index, module, "", 0);
		free(line);
	}
	fclose(f);
	index_write(index, out);
	index_destroy(index);

	return 1;
}

/**
 * output_aliases - output list of module aliases
 *
 * @modules:	list of modules
 * @out:	output file reference
 * @dirname:	output directory
 *
 */
static int output_aliases(struct module *modules, FILE *out, char *dirname)
{
	struct module *i;
	struct elf_file *file;
	struct string_table *tbl;
	int j;

	fprintf(out, "# Aliases extracted from modules themselves.\n");
	for (i = modules; i; i = i->next) {
		char modname[strlen(i->pathname)+1];

		file = i->file;
		filename2modname(modname, i->pathname);

		/* Grab from old-style .modalias section. */
		tbl = file->ops->load_strings(file, ".modalias", NULL);
		for (j = 0; tbl && j < tbl->cnt; j++)
			fprintf(out, "alias %s %s\n", tbl->str[j], modname);
		strtbl_free(tbl);

		/* Grab from new-style .modinfo section. */
		tbl = file->ops->load_strings(file, ".modinfo", NULL);
		for (j = 0; tbl && j < tbl->cnt; j++) {
			const char *p = tbl->str[j];
			if (strstarts(p, "alias="))
				fprintf(out, "alias %s %s\n",
					p + strlen("alias="), modname);
		}
		strtbl_free(tbl);
	}
	return 1;
}

/**
 * output_aliases_bin - output list of module aliases in binary format
 *
 * @modules:	list of modules
 * @out:	outout file reference
 * @dirname:	output directory
 *
 */
static int output_aliases_bin(struct module *modules, FILE *out, char *dirname)
{
	struct module *i;
	struct elf_file *file;
	struct string_table *tbl;
	int j;
	char *alias;
	struct index_node *index;
	int duplicate;

	index = index_create();
	
	for (i = modules; i; i = i->next) {
		char modname[strlen(i->pathname)+1];

		file = i->file;
		filename2modname(modname, i->pathname);

		/* Grab from old-style .modalias section. */
		tbl = file->ops->load_strings(file, ".modalias", NULL);
		for (j = 0; tbl && j < tbl->cnt; j++) {
			alias = NOFAIL(strdup(tbl->str[j]));
			underscores(alias);
			duplicate = index_insert(index, alias, modname, i->order);
			if (duplicate && warn_dups)
				warn("duplicate module alias:\n%s %s\n",
					alias, modname);
			free(alias);
		}
		strtbl_free(tbl);

		/* Grab from new-style .modinfo section. */
		tbl = file->ops->load_strings(file, ".modinfo", NULL);
		for (j = 0; tbl && j < tbl->cnt; j++) {
			const char *p = tbl->str[j];
			if (strstarts(p, "alias=")) {
				alias = NOFAIL(strdup(p + strlen("alias=")));
				underscores(alias);
				duplicate = index_insert(index, alias, modname, i->order);
				if (duplicate && warn_dups)
					warn("duplicate module alias:\n%s %s\n",
						alias, modname);
				free(alias);
			}
		}
		strtbl_free(tbl);
	}
	
	index_write(index, out);
	index_destroy(index);

	return 1;
}

/**
 * output_softdeps - output module softdeps (non-implicit dependencies)
 *
 * @modules:	list of modules
 * @out:	output file reference
 * @dirname:	output directory
 *
 */
static int output_softdeps(struct module *modules, FILE *out, char *dirname)
{
	struct module *i;
	struct elf_file *file;
	struct string_table *tbl;
	int j;

	fprintf(out, "# Soft dependencies extracted from modules themselves.\n");
	fprintf(out, "# Copy, with a .conf extension, to /etc/modprobe.d to use "
		"it with modprobe.\n");
	for (i = modules; i; i = i->next) {
		char modname[strlen(i->pathname)+1];

		file = i->file;
		filename2modname(modname, i->pathname);

		/* Grab from new-style .modinfo section. */
		tbl = file->ops->load_strings(file, ".modinfo", NULL);
		for (j = 0; tbl && j < tbl->cnt; j++) {
			const char *p = tbl->str[j];
			if (strstarts(p, "softdep="))
				fprintf(out, "softdep %s %s\n",
					modname, p + strlen("softdep="));
		}
		strtbl_free(tbl);
	}
	return 1;
}

/**
 * output_devname - output device names required by modules
 *
 * @modules:	list of modules
 * @out:	output file reference
 * @dirname:	output directory
 *
 */
static int output_devname(struct module *modules, FILE *out, char *dirname)
{
	struct module *m;

	fprintf(out, "# Device nodes to trigger on-demand module loading.\n");
	for (m = modules; m != NULL; m = m->next) {
		struct string_table *tbl;
		int i;
		char type = '\0';
		const char *devname = NULL;

		tbl = m->file->ops->load_strings(m->file, ".modinfo", NULL);
		for (i = 0; tbl && i < tbl->cnt; i++) {
			const char *p = tbl->str[i];
			unsigned int maj, min;

			if (sscanf(p, "alias=char-major-%u-%u", &maj, &min) == 2)
				type = 'c';
			else if (sscanf(p, "alias=block-major-%u-%u", &maj, &min) == 2)
				type = 'b';
			else if (strstarts(p, "alias=devname:"))
				devname = &p[strlen("alias=devname:")];

			if (type && devname) {
				char modname[strlen(m->pathname)+1];

				filename2modname(modname, m->pathname);
				fprintf(out, "%s %s %c%u:%u\n",
					modname, devname, type, maj, min);
				break;
			}
		}
		strtbl_free(tbl);
	}
	return 1;
}

struct depfile {
	const char *name;
	int (*func)(struct module *, FILE *, char *dirname);
	int map_file;
};

/* The possible output files - those with map_file unset typically not made */
static const struct depfile depfiles[] = {
	{ "modules.dep", output_deps, 0 }, /* This is what we check for '-A'. */
	{ "modules.dep.bin", output_deps_bin, 0 },
	{ "modules.pcimap", output_pci_table, 1 },
	{ "modules.usbmap", output_usb_table, 1 },
	{ "modules.ccwmap", output_ccw_table, 1 },
	{ "modules.ieee1394map", output_ieee1394_table, 1 },
	{ "modules.isapnpmap", output_isapnp_table, 1 },
	{ "modules.inputmap", output_input_table, 1 },
	{ "modules.ofmap", output_of_table, 1 },
	{ "modules.seriomap", output_serio_table, 1 },
	{ "modules.alias", output_aliases, 0 },
	{ "modules.alias.bin", output_aliases_bin, 0 },
	{ "modules.softdep", output_softdeps, 0 },
	{ "modules.symbols", output_symbols, 0 },
	{ "modules.symbols.bin", output_symbols_bin, 0 },
	{ "modules.builtin.bin", output_builtin_bin, 0 },
	{ "modules.devname", output_devname, 0 },
};

/**
 * any_modules_newer - determine if modules are newer than ref time
 *
 * @dirname:	directory to process
 * @mtime:	comparison time
 *
 * The worst case is that we process modules we didn't need to. It is
 * therefore safer to go with "true" if we can't figure it out.
 *
 */
static int any_modules_newer(const char *dirname, time_t mtime)
{
	DIR *dir;
	struct dirent *dirent;

	dir = opendir(dirname);
	if (!dir)
		return 1;

	while ((dirent = readdir(dir)) != NULL) {
		struct stat st;
		char file[strlen(dirname) + 1 + strlen(dirent->d_name) + 1];

		if (streq(dirent->d_name, ".") || streq(dirent->d_name, ".."))
			continue;

		sprintf(file, "%s/%s", dirname, dirent->d_name);
		if (lstat(file, &st) != 0)
			goto ret_true;

		if (smells_like_module(dirent->d_name)) {
			if (st.st_mtime > mtime)
				goto ret_true;
		} else if (S_ISDIR(st.st_mode)) {
			if (any_modules_newer(file, mtime))
				goto ret_true;
		}
	}
	closedir(dir);
	return 0;

ret_true:
	closedir(dir);
	return 1;
}

/**
 * depfile_out_of_date - check if module dep files are older than any modules
 *
 * @dirname:	directory to process
 *
 * Use any_modules_newer to determine if the dep files are up to date.
 *
 */
static int depfile_out_of_date(const char *dirname)
{
	struct stat st;
	char depfile[strlen(dirname) + 1 + strlen(depfiles[0].name) + 1];

	sprintf(depfile, "%s/%s", dirname, depfiles[0].name);

	if (stat(depfile, &st) != 0)
		return 1;

	return any_modules_newer(dirname, st.st_mtime);
}

/**
 * strsep_skipspace - skip over delimitors in strings
 *
 * @string:	string to process
 * @delim:	delimitor (e.g. ' ')
 *
 */
static char *strsep_skipspace(char **string, char *delim)
{
	if (!*string)
		return NULL;
	*string += strspn(*string, delim);
	return strsep(string, delim);
}

/**
 * add_search - add a new module search path
 *
 * @search_path:	path to search
 * @len:		length of path
 * @search:		list of search paths
 *
 */
static struct module_search *add_search(const char *search_path,
					size_t len,
					struct module_search *search)
{

	struct module_search *new;
	
	new = NOFAIL(malloc(sizeof(*new)));
	new->search_path = NOFAIL(strdup(search_path));
	new->len = len;
	new->next = search;

	return new;
	
}

/**
 * add_override - add a new module override entry
 *
 * @modfile:	name of module file
 * @overrides:	list of override entries
 *
 */
static struct module_overrides *add_override(const char *modfile,
					     struct module_overrides *overrides)
{

	struct module_overrides *new;
	
	new = NOFAIL(malloc(sizeof(*new)));
	new->modfile = NOFAIL(strdup(modfile));
	new->next = overrides;

	return new;
	
}

static int parse_config_scan(const char *filename,
			     const char *basedir,
			     const char *kernelversion,
			     struct module_search **search,
			     struct module_overrides **overrides);

/**
 * parse_config_file - process an individual configuration file
 *
 * @filename:		name of config file
 * @basedir:		module base directory
 * @kernelversion:	kernel version to process
 * @search:		search path order
 * @overrides:		module override entries
 *
 */
static int parse_config_file(const char *filename,
			     const char *basedir,
			     const char *kernelversion,
			     struct module_search **search,
			     struct module_overrides **overrides)
{
	char *line;
	unsigned int linenum = 0;
	FILE *cfile;

	cfile = fopen(filename, "r");
	if (!cfile) {
		if (errno != ENOENT)
			fatal("could not open '%s', reason: %s\n", filename,
			      strerror(errno));
		return 0;
	}

	while ((line = getline_wrapped(cfile, &linenum)) != NULL) {
		char *ptr = line;
		char *cmd, *modname;

		cmd = strsep_skipspace(&ptr, "\t ");

		if (cmd == NULL || cmd[0] == '#' || cmd[0] == '\0') {
			free(line);
			continue;
		}

		if (streq(cmd, "search")) {
			char *search_path;
			
			while ((search_path = strsep_skipspace(&ptr, "\t "))) {
				char *dirname;
				size_t len;

				if (strcmp(search_path,
						MODULE_BUILTIN_KEY) == 0) {
					*search = add_search(MODULE_BUILTIN_KEY,
							     0, *search);
					continue;
				}
				nofail_asprintf(&dirname, "%s%s%s/%s", basedir,
					MODULE_DIR, kernelversion, search_path);
				len = strlen(dirname);
				*search = add_search(dirname, len, *search);
				free(dirname);
			}
		} else if (streq(cmd, "override")) {
			char *pathname = NULL, *version, *subdir;
			modname = strsep_skipspace(&ptr, "\t ");
			version = strsep_skipspace(&ptr, "\t ");
			subdir = strsep_skipspace(&ptr, "\t ");

			if (!regex_match(kernelversion, (const char *)version))
				continue;

			nofail_asprintf(&pathname, "%s%s%s/%s/%s.ko", basedir,
				MODULE_DIR, kernelversion, subdir, modname);

			*overrides = add_override(pathname, *overrides);
			free(pathname);
		} else if (streq(cmd, "include")) {
			char *newfilename;

			newfilename = strsep_skipspace(&ptr, "\t ");
			if (!newfilename) {
				grammar(cmd, filename, linenum);
			} else {
				warn("\"include %s\" is deprecated, "
				     "please use /etc/depmod.d\n", newfilename);
				if (strstarts(newfilename, "/etc/depmod.d")) {
					warn("\"include /etc/depmod.d\" is "
					     "the default, ignored\n");
				} else {
					if (!parse_config_scan(newfilename, basedir,
							       kernelversion,
							       search, overrides))
					warn("Failed to open included"
					     " config file %s: %s\n",
					     newfilename, strerror(errno));
				}
			}
		} else if (streq(cmd, "make_map_files")) {
			char *option;

			option = strsep_skipspace(&ptr, "\t ");
			if (!option)
				grammar(cmd, filename, linenum);
			else {
				if (streq(option, "yes"))
					make_map_files = 1;
				else if (streq(option, "no"))
					make_map_files = 0;
				else
					grammar(cmd, filename, linenum);
			}
                } else
                        grammar(cmd, filename, linenum);

                free(line);
        }
        fclose(cfile);
        return 1;
}

/**
 * parse_config_scan - handle a directory of config files
 *
 * @filename:		name of directory
 * @basedir:		module base directory
 * @kernelversion:	kernel version to process
 * @search:		search path order
 * @overrides:		module override entries
 *
 */
static int parse_config_scan(const char *filename,
			     const char *basedir,
			     const char *kernelversion,
			     struct module_search **search,
			     struct module_overrides **overrides)
{
	DIR *dir;
	int ret = 0;

	dir = opendir(filename);
	if (dir) {
		struct file_entry {
			struct list_head node;
			char name[];
		};
		LIST_HEAD(files_list);
		struct file_entry *fe, *fe_tmp;
		struct dirent *i;

		/* sort files from directory into list */
		while ((i = readdir(dir)) != NULL) {
			size_t len;

			if (i->d_name[0] == '.')
				continue;
			if (!config_filter(i->d_name))
				continue;

			len = strlen(i->d_name);
			if (len < 6 || strcmp(&i->d_name[len-5], ".conf") != 0)
				warn("All config files need .conf: %s/%s, "
				     "it will be ignored in a future release.\n",
				     filename, i->d_name);
			fe = malloc(sizeof(struct file_entry) + len + 1);
			if (fe == NULL)
				continue;
			strcpy(fe->name, i->d_name);
			list_for_each_entry(fe_tmp, &files_list, node)
				if (strcmp(fe_tmp->name, fe->name) >= 0)
					break;
			list_add_tail(&fe->node, &fe_tmp->node);
		}
		closedir(dir);

		/* parse list of files */
		list_for_each_entry_safe(fe, fe_tmp, &files_list, node) {
			char *cfgfile;

			nofail_asprintf(&cfgfile, "%s/%s", filename, fe->name);
			if (!parse_config_file(cfgfile, basedir, kernelversion,
					       search, overrides))
				warn("Failed to open config file "
				     "%s: %s\n", fe->name, strerror(errno));
			free(cfgfile);
			list_del(&fe->node);
			free(fe);
		}

		ret = 1;
	} else {
		if (parse_config_file(filename, basedir, kernelversion, search,
				      overrides))
			ret = 1;
	}

	return ret;
}

/**
 * parse_toplevel_config - handle top-level depmod.conf, depmod.d
 *
 * @filename:		possibly overridden config
 * @basedir:		module base directory
 * @kernelversion:	kernel version to process
 * @search:		search path order
 * @overrides:		module override entries
 *
 */
static void parse_toplevel_config(const char *filename,
				  const char *basedir,
				  const char *kernelversion,
				  struct module_search **search,
				  struct module_overrides **overrides)
{
	if (filename) {
		if (!parse_config_scan(filename, basedir, kernelversion, search,
				 overrides))
			fatal("Failed to open config file %s: %s\n",
			      filename, strerror(errno));
		return;
	}

	/* deprecated config file */
	if (parse_config_file("/etc/depmod.conf", basedir, kernelversion,
			      search, overrides) > 0)
		warn("Deprecated config file /etc/depmod.conf, "
		      "all config files belong into /etc/depmod.d/.\n");

	/* default config */
	parse_config_scan("/etc/depmod.d", basedir, kernelversion,
			  search, overrides);
}

/* Local to main, but not freed on exit.  Keep valgrind quiet. */
static struct module *list = NULL;
static struct module_search *search = NULL;
static struct module_overrides *overrides = NULL;

int main(int argc, char *argv[])
{
	int opt, all = 0, maybe_all = 0, doing_stdout = 0;
	char *basedir = "", *dirname, *version;
	char *system_map = NULL, *module_symvers = NULL;
	int i;
	const char *config = NULL;

	if (native_endianness() == 0)
		abort();

	while ((opt = getopt_long(argc, argv, "aAb:C:E:F:euqrvnP:hVwm", options, NULL))
	       != -1) {
		switch (opt) {
		case 'a':
			all = 1;
			break;
		case 'A':
			maybe_all = 1;
			break;
		case 'b':
			basedir = optarg;
			skipchars = strlen(basedir);
			break;
		case 'C':
			config = optarg;
			break;
		case 'E':
			module_symvers = optarg;
			check_symvers = 1;
			break;
		case 'F':
			system_map = optarg;
			break;
		case 'e':
			print_unknown = 1;
			break;
		case 'u':
		case 'q':
		case 'r':
			break;
		case 'v':
			verbose = 1;
			break;
		case 'n':
			doing_stdout = 1;
			break;
		case 'P':
			if (optarg[1] != '\0')
				fatal("-P only takes a single char\n");
			sym_prefix = optarg[0];
			break;
		case 'h':
			print_usage(argv[0]);
			exit(0);
			break;
		case 'V':
			printf("%s %s\n", PACKAGE, VERSION);
			exit(0);
		case 'w':
			warn_dups = 1;
			break;
		case 'm':
			force_map_files = 1;
			break;
		default:
			print_usage(argv[0]);
			exit(1);
		}
	}

	if (module_symvers)
		load_module_symvers(module_symvers);
	else if (system_map)
		load_system_map(system_map);
	else if (print_unknown) {
		warn("-e needs -E or -F\n");
		print_unknown = 0;
	}

	/* They can specify the version naked on the command line */
	if (optind < argc && is_version_number(argv[optind])) {
		version = NOFAIL(strdup(argv[optind]));
		optind++;
	} else {
		struct utsname buf;
		uname(&buf);
		version = NOFAIL(strdup(buf.release));
	}

	/* Check for old version. */
	if (old_module_version(version)) {
		fprintf(stderr, "Kernel version %s requires old depmod\n",
			version);
		exit(2);
	}

	/* Depmod -a by default if no names. */
	if (optind == argc)
		all = 1;

	nofail_asprintf(&dirname, "%s%s%s", basedir, MODULE_DIR, version);

	if (maybe_all) {
		if (!doing_stdout && !depfile_out_of_date(dirname))
			exit(0);
		all = 1;
	}

	parse_toplevel_config(config, basedir, version, &search, &overrides);

	/* For backward compatibility add "updates" to the head of the search
	 * list here. But only if there was no "search" option specified.
	 */
	if (!search) {
		char *dirname;
		size_t len;

		nofail_asprintf(&dirname, "%s%s%s/updates", basedir,
				MODULE_DIR, version);
		len = strlen(dirname);
		search = add_search(dirname, len, search);
	}
	if (!all) {
		/* Do command line args. */
		for (opt = optind; opt < argc; opt++) {
			struct module *new;

			if (argv[opt][0] != '/')
				fatal("modules must be specified using absolute paths.\n"
					"\"%s\" is a relative path\n", argv[opt]);

			new = grab_module(NULL, argv[opt]);
			if (!new) {
				/* cmd-line specified modules must exist */
				fatal("grab_module() failed for module %s\n", argv[opt]);
			}
			new->next = list;
			list = new;
		}
	} else {
		list = grab_basedir(dirname,search,overrides);
	}
	list = sort_modules(dirname,list);
	list = parse_modules(list);

	for (i = 0; i < ARRAY_SIZE(depfiles); i++) {
		FILE *out;
		int res;
		const struct depfile *d = &depfiles[i];
		char depname[strlen(dirname) + 1 + strlen(d->name) + 1];
		char tmpname[strlen(dirname) + 1 + strlen(d->name) +
						strlen(".temp") + 1];

		if (d->map_file && !make_map_files && !force_map_files)
			continue;

		sprintf(depname, "%s/%s", dirname, d->name);
		sprintf(tmpname, "%s/%s.temp", dirname, d->name);
		if (!doing_stdout) {
			out = fopen(tmpname, "w");
			if (!out)
				fatal("Could not open %s for writing: %s\n",
					tmpname, strerror(errno));
		} else {
			out = stdout;
			if (ends_in(depname, ".bin"))
				continue;
		}
		res = d->func(list, out, dirname);
		if (doing_stdout)
			continue;
		fclose(out);
		if (res) {
			if (rename(tmpname, depname) < 0)
				fatal("Could not rename %s into %s: %s\n",
					tmpname, depname, strerror(errno));
		} else {
			if (unlink(tmpname) < 0)
				warn("Could not delete %s: %s\n",
					tmpname, strerror(errno));
		}
	}

	free(dirname);
	free(version);
	
	return 0;
}
