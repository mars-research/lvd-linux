/* modprobe.c: add or remove a module from the kernel, intelligently.
 *
 * Copyright (C) 2001        Rusty Russell.
 * Copyright (C) 2002, 2003  Rusty Russell, IBM Corporation.
 * Copyright (C) 2006-2011   Jon Masters <jcm@jonmasters.org>, and others.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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

#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <elf.h>
#include <getopt.h>
#include <fnmatch.h>
#include <asm/unistd.h>
#include <sys/wait.h>
#include <syslog.h>

/* for lcd init module */
#include "../include/uapi/lcd_domains.h"
#include <sys/ioctl.h>

#include "util.h"
#include "elfops.h"
#include "zlibsupport.h"
#include "logging.h"
#include "index.h"
#include "list.h"
#include "config_filter.h"

#include "testing.h"

/* NOTE: in the future, binary indexes will always be used */
static int use_binary_indexes = 1; /* default to enabled. */

/* Limit do_softdep/do_modprobe recursion.
 * This is a simple way to handle dependency loops
 * caused by poorly written softdep commands.
 */
static int recursion_depth = 0;
static const int MAX_RECURSION = 50; /* Arbitrary choice */

extern long init_module(void *, unsigned long, const char *);
extern long delete_module(const char *, unsigned int);

struct module {
	struct list_head list;
	char *modname;
	char filename[0];
};

typedef enum
{
	mit_remove = 1,
	mit_dry_run = 2,
	mit_first_time = 4,
	mit_use_blacklist = 8,
	mit_ignore_commands = 16,
	mit_ignore_loaded = 32,
	mit_quiet_inuse = 64,
	mit_strip_vermagic = 128,
	mit_strip_modversion = 256,
	mit_resolve_alias = 512

} modprobe_flags_t;

#ifndef MODULE_DIR
#define MODULE_DIR "/lib/modules"
#endif

/**
 * print_usage - output the prefered program usage
 *
 * @progname:	binary name invoked
 *
 */
static void print_usage(const char *progname)
{
	fprintf(stderr,
		"Usage: %s [-v] [-V] [-C config-file] [-d <dirname> ] [-n] [-i] [-q] [-b] [-o <modname>] [ --dump-modversions ] <modname> [parameters...]\n"
		"%s -r [-n] [-i] [-v] <modulename> ...\n"
		"%s -l -t <dirname> [ -a <modulename> ...]\n",
		progname, progname, progname);
	exit(1);
}

/**
 * find_module - search module list for module name
 *
 * @filename:	module file name
 * @list:	module list
 *
 */
static struct module *find_module(const char *filename, struct list_head *list)
{
	struct module *i;

	list_for_each_entry(i, list, list) {
		if (streq(i->filename, filename))
			return i;
	}
	return NULL;
}

/**
 * add_module - add a module to the global module list
 *
 * @filename:	module file name
 * @list:	module list
 *
 */
static void add_module(char *filename, int namelen, struct list_head *list)
{
	struct module *mod;

	/* If it's a duplicate: move it to the end, so it gets
	   inserted where it is *first* required. */
	mod = find_module(filename, list);
	if (mod)
		list_del(&mod->list);
	else {
		/* No match.  Create a new module. */
		mod = NOFAIL(malloc(sizeof(struct module) + namelen + 1));
		memcpy(mod->filename, filename, namelen);
		mod->filename[namelen] = '\0';
		mod->modname = NOFAIL(malloc(namelen + 1));
		filename2modname(mod->modname, mod->filename);
	}

	list_add_tail(&mod->list, list);
}

/**
 * free_module - de-allocate module structure
 *
 * @mod:	module structure
 *
 */
static void free_module(struct module *mod)
{
	free(mod->modname);
	free(mod);
}

/**
 * modname_equal - compare module names (up to len), with '_' and '-' equal
 *
 * @a:		first module name
 * @b:		second module name
 * @len:	length to compare
 *
 */
static int modname_equal(const char *a, const char *b, unsigned int len)
{
	unsigned int i;

	if (strlen(b) != len)
		return 0;

	for (i = 0; i < len; i++) {
		if ((a[i] == '_' || a[i] == '-')
		    && (b[i] == '_' || b[i] == '-'))
			continue;
		if (a[i] != b[i])
			return 0;
	}
	return 1;
}

/**
 * add_modules_dep_line - parse a dep line from the module.dep[.bin] file
 *
 * @line:	input file line
 * @name:	module name
 * @list:	list of modules
 * @dirname:	module directory
 *
 * Add dependency information if this line of the dep file matches mod name
 */
static int add_modules_dep_line(char *line,
				const char *name,
				struct list_head *list,
				const char *dirname)
{
	char *ptr;
	int len;
	char *modname, *fullpath;

	/* Ignore lines without : or which start with a # */
	ptr = strchr(line, ':');
	if (ptr == NULL || line[strspn(line, "\t ")] == '#')
		return 0;

	/* Is this the module we are looking for? */
	*ptr = '\0';
	modname = my_basename(line);

	len = strlen(modname);
	if (strchr(modname, '.'))
		len = strchr(modname, '.') - modname;
	if (!modname_equal(modname, name, len))
		return 0;

	/* Create the list. */
	if ('/' == line[0]) {	/* old style deps - absolute path specified */
		add_module(line, ptr - line, list);
	} else {
		nofail_asprintf(&fullpath, "%s/%s", dirname, line);
		add_module(fullpath, strlen(dirname)+1+(ptr - line), list);
		free(fullpath);
	}

	ptr++;
	for(;;) {
		char *dep_start;
		ptr += strspn(ptr, " \t");
		if (*ptr == '\0')
			break;
		dep_start = ptr;
		ptr += strcspn(ptr, " \t");

		/* We handle deps in two possible ways. Either they have */
		/* an absolute path, or a relative path (to toplevel moddir). */

		if ('/' == dep_start[0]) {	/* old style deps */
			add_module(dep_start, ptr - dep_start, list);
		} else {
			nofail_asprintf(&fullpath, "%s/%s", dirname, dep_start);
			add_module(fullpath,
				   strlen(dirname)+1+(ptr - dep_start), list);
			free(fullpath);
		}
	}
	return 1;
}

/**
 * read_depends_file - import the modules.dep.bin file
 *
 * @dirname:	module directory
 * @start_name:	initial module name to search
 * @list:	list of modules
 *
 */
static int read_depends_file(const char *dirname,
			     const char *start_name,
			     struct list_head *list)
{
	char *modules_dep_name;
	char *line;
	struct index_file *modules_dep;

	nofail_asprintf(&modules_dep_name, "%s/%s", dirname, "modules.dep.bin");
	modules_dep = index_file_open(modules_dep_name);
	if (!modules_dep) {
		free(modules_dep_name);
		return 0;
	}

	line = index_search(modules_dep, start_name);
	if (line) {
		/* Value is standard dependency line format */
		if (!add_modules_dep_line(line, start_name, list, dirname))
			fatal("Module index is inconsistent\n");
		free(line);
	}

	index_file_close(modules_dep);
	free(modules_dep_name);
	
	return 1;
}

/**
 * read_depends - import the modules.dep[.bin] file
 *
 * @dirname:	module directory
 * @start_name:	initial module name to search
 * @list:	list of modules
 *
 */
static void read_depends(const char *dirname,
			 const char *start_name,
			 struct list_head *list)
{
	char *modules_dep_name;
	char *line;
	FILE *modules_dep;
	int done = 0;

	if (use_binary_indexes)
		if (read_depends_file(dirname, start_name, list))
			return;

	nofail_asprintf(&modules_dep_name, "%s/%s", dirname, "modules.dep");
	modules_dep = fopen(modules_dep_name, "r");
	if (!modules_dep)
		fatal("Could not load %s: %s\n",
		      modules_dep_name, strerror(errno));

	/* Stop at first line, as we can have duplicates (eg. symlinks
           from boot/ */
	while (!done && (line = getline_wrapped(modules_dep, NULL)) != NULL) {
		done = add_modules_dep_line(line, start_name, list, dirname);
		free(line);
	}
	fclose(modules_dep);
	free(modules_dep_name);
}

/**
 * insert_moderror - convert standard insert error numbers into error messages
 *
 * @err:	error number
 *
 */
static const char *insert_moderror(int err)
{
	switch (err) {
	case ENOEXEC:
		return "Invalid module format";
	case ENOENT:
		return "Unknown symbol in module, or unknown parameter (see dmesg)";
	case ENOSYS:
		return "Kernel does not have module support";
	default:
		return strerror(err);
	}
}

/**
 * remove_moderror - convert standard remove error numbers into error messages
 *
 * @err:	error number
 *
 */
static const char *remove_moderror(int err)
{
	switch (err) {
	case ENOENT:
		return "No such module";
	case ENOSYS:
		return "Kernel does not have module unloading support";
	default:
		return strerror(err);
	}
}
/**
 * clear_magic - strip any versioning information from the module
 *
 * @module:	mapped module file
 *
 */
static void clear_magic(struct elf_file *module)
{
	struct string_table *tbl;
	int j;

	/* Old-style: __vermagic section */
	module->ops->strip_section(module, "__vermagic");

	/* New-style: in .modinfo section */
	tbl = module->ops->load_strings(module, ".modinfo", NULL);
	for (j = 0; tbl && j < tbl->cnt; j++) {
		const char *p = tbl->str[j];
		if (strstarts(p, "vermagic=")) {
			memset((char *)p, 0, strlen(p));
			return;
		}
	}
}

/* keep track of module options from config file(s) */
struct module_options
{
	struct module_options *next;
	char *modulename;
	char *options;
};

/* keep track of module install commands from config file(s) */
struct module_command
{
	struct module_command *next;
	char *modulename;
	char *command;
};

/* keep track of module aliases added in the config file(s) */
struct module_alias
{
	struct module_alias *next;
	char *aliasname;
	char *module;
};

/* keep track of modules blacklisted in the config file(s) */
struct module_blacklist
{
	struct module_blacklist *next;
	char *modulename;
};

/* keep track of module softdeps added in the config file(s) */
struct module_softdep
{
	struct module_softdep *next;
	char *buf;
	/* The modname and string tables point to buf. */
	char *modname;
	struct string_table *pre;
	struct string_table *post;
};

/* keep track of all config options */
struct modprobe_conf
{
	struct module_options *options;
	struct module_command *commands;
	struct module_alias *aliases;
	struct module_blacklist *blacklist;
	struct module_softdep *softdeps;
};

/**
 * add_options - module options added in the config file(s)
 *
 * @modname:	module name
 * @option:	options string
 * @options:	list of options
 *
 */
static struct module_options *
add_options(const char *modname,
	    const char *option,
	    struct module_options *options)
{
	struct module_options *new;
	char *tab; 

	new = NOFAIL(malloc(sizeof(*new)));
	new->modulename = NOFAIL(strdup(modname));
	new->options = NOFAIL(strdup(option));
	/* We can handle tabs, kernel can't. */
	for (tab = strchr(new->options, '\t'); tab; tab = strchr(tab, '\t'))
		*tab = ' ';
	new->next = options;
	return new;
}

/**
 * add_command - module install commands added in the config file(s)
 *
 * @modname:	module name
 * @command:	command string
 * @commands:	list of commands
 *
 */
static struct module_command *
add_command(const char *modname,
	       const char *command,
	       struct module_command *commands)
{
	struct module_command *new;

	new = NOFAIL(malloc(sizeof(*new)));
	new->modulename = NOFAIL(strdup(modname));
	new->command = NOFAIL(strdup(command));
	new->next = commands;
	return new;
}

/**
 * add_alias - module aliases added in the config file(s)
 *
 * @aliasname:	alias string
 * @modname:	module name
 * @aliases:	list of aliases
 *
 */
static struct module_alias *
add_alias(const char *aliasname, const char *modname, struct module_alias *aliases)
{
	struct module_alias *new;

	new = NOFAIL(malloc(sizeof(*new)));
	new->aliasname = NOFAIL(strdup(aliasname));
	new->module = NOFAIL(strdup(modname));
	new->next = aliases;
	return new;
}


/**
 * find_aliases - find aliases for a module
 *
 * @aliases:	list of aliases
 * @name:	module name
 *
 */
static struct module_alias *
find_aliases(const struct module_alias *aliases,
	     const char *name)
{
	struct module_alias *result = NULL;
	while (aliases) {
		char *aliasname = aliases->aliasname;
		char *modname = aliases->module;
		if (fnmatch(aliasname, name, 0) == 0)
			result = add_alias(aliasname, modname, result);
		aliases = aliases->next;
	}
	return result;
}

/**
 * free_aliases - de-allocate the aliases list
 *
 * @alias_list:	list of aliases
 *
 */
static void free_aliases(struct module_alias *alias_list)
{
	while (alias_list) {
		struct module_alias *alias;

		alias = alias_list;
		alias_list = alias_list->next;

		free(alias->aliasname);
		free(alias->module);
		free(alias);
	}
}

/**
 * add_blacklist - blacklist modules in config file(s)
 *
 * @modname:	module name
 * @blacklist	list of blacklisted module names
 *
 */
static struct module_blacklist *
add_blacklist(const char *modname, struct module_blacklist *blacklist)
{
	struct module_blacklist *new;

	new = NOFAIL(malloc(sizeof(*new)));
	new->modulename = NOFAIL(strdup(modname));
	new->next = blacklist;
	return new;
}

/**
 * find_blacklist - lookup any potentially blacklisted module
 *
 * @modname:	module name
 * @blacklist:	list of blacklisted module names
 *
 */
static int
find_blacklist(const char *modname, const struct module_blacklist *blacklist)
{
	while (blacklist) {
		if (streq(blacklist->modulename, modname))
			return 1;
		blacklist = blacklist->next;
	}
	return 0;
}

/**
 * apply_blacklist - remove blacklisted modules from alias list
 *
 * @aliases:	module alias list
 * @blacklist:	list of blacklisted module names
 *
 */
static void
apply_blacklist(struct module_alias **aliases,
		const struct module_blacklist *blacklist)
{
	struct module_alias *result = NULL;
	struct module_alias *alias = *aliases;
	while (alias) {
		char *modname = alias->module;
		if (!find_blacklist(modname, blacklist))
			result = add_alias(alias->aliasname, modname, result);
		alias = alias->next;
	}
	free_aliases(*aliases);
	*aliases = result;
}

/**
 * find_command - lookup any install commands for a module
 *
 * @modname:	module name
 * @commands:	list of install commands
 *
 */
static const char *find_command(const char *modname,
				const struct module_command *commands)
{
	while (commands) {
		if (fnmatch(commands->modulename, modname, 0) == 0)
			return commands->command;
		commands = commands->next;
	}
	return NULL;
}

/**
 * find_softdep - lookup any softdeps for a module
 *
 * @modname:	module name
 * @softdeps:	list of module softdeps
 *
 */
static const struct module_softdep *
find_softdep(const char *modname, const struct module_softdep *softdeps)
{
	while (softdeps) {
		if (fnmatch(softdeps->modname, modname, 0) == 0)
			return softdeps;
		softdeps = softdeps->next;
	}
	return NULL;
}

/**
 * append_option - add further options to modules (tail)
 *
 * @options:	existing option string
 * @newoption:	additional option string
 *
 * options supplied on the command line
 */
static char *append_option(char *options, const char *newoption)
{
	options = NOFAIL(realloc(options, strlen(options) + 1
				 + strlen(newoption) + 1));
	if (strlen(options)) strcat(options, " ");
	strcat(options, newoption);
	return options;
}

/**
 * prepend_option - add further options to modules (head)
 *
 * @options:	existing option string
 * @newoption:	additional option string
 *
 * options supplied in the config file(s)
 */
static char *prepend_option(char *options, const char *newoption)
{
	size_t l1, l2;
	l1 = strlen(options);
	l2 = strlen(newoption);
	/* the resulting string will look like
	 * newoption + ' ' + options + '\0' */
	if (l1) {
		options = NOFAIL(realloc(options, l2 + 1 + l1 + 1));
		memmove(options + l2 + 1, options, l1 + 1);
		options[l2] = ' ';
		memcpy(options, newoption, l2);
	} else {
		options = NOFAIL(realloc(options, l2 + 1));
		memcpy(options, newoption, l2);
		options[l2] = '\0';
	}
	return options;
}

/**
 * add_extra_options - add any relevant options from the config file(s)
 *
 * @modname:	module name
 * @optstring:	options
 * @options:	list of options
 *
 */
static char *add_extra_options(const char *modname,
			       const char *optstring,
			       const struct module_options *options)
{
	char *opts = NOFAIL(strdup(optstring));

	while (options) {
		if (streq(options->modulename, modname))
			opts = prepend_option(opts, options->options);
		options = options->next;
	}
	return opts;
}

/**
 * module_in_procfs - check if module is known to be loaded already
 *
 * @modname:	module name
 * @usecount:	update usecount if possible (-1 if unknown)
 *
 */
static int module_in_procfs(const char *modname, unsigned int *usecount)
{
	FILE *proc_modules;
	char *line;

again:
	/* Might not be mounted yet.  Don't fail. */
	proc_modules = fopen("/proc/modules", "r");
	if (!proc_modules)
		return -1;

	while ((line = getline_wrapped(proc_modules, NULL)) != NULL) {
		char *entry = strtok(line, " \n");

		if (entry && streq(entry, modname)) {
			/* If it exists, usecount is the third entry. */
			if (!strtok(NULL, " \n"))
				goto out;

			if (!(entry = strtok(NULL, " \n"))) /* usecount */
				goto out;
			else
				if (usecount)
					*usecount = atoi(entry);

			/* Followed by - then status. */
			if (strtok(NULL, " \n")
			    && (entry = strtok(NULL, " \n")) != NULL) {
				/* No locking, we might hit cases
				 * where module is in flux.  Spin. */
				if (streq(entry, "Loading")
				    || streq(entry, "Unloading")) {
					usleep(100000);
					free(line);
					fclose(proc_modules);
					goto again;
				}
			}

		out:
			free(line);
			fclose(proc_modules);
			return 1;
		}
		free(line);
	}
	fclose(proc_modules);
	return 0;
}

/**
 * read_attribute - read sysfs file attributes into a buffer
 *
 * @filename:	name of sysfs file
 * @buf:	buffer
 * @buflen:	size of buffer
 *
 */
static int read_attribute(const char *filename, char *buf, size_t buflen)
{
	FILE *file;
	char *s;

	file = fopen(filename, "r");
	if (file == NULL)
		return (errno == ENOENT) ? 0 : -1;
	s = fgets(buf, buflen, file);
	fclose(file);

	/* return -1 if any problems */

	return (s == NULL) ? -1 : 1;
}

/**
 * module_builtin - try to determine whether a module is built-in
 *
 * @dirname:	module directory
 * @modname:	name of module
 *
 */
static int module_builtin(const char *dirname, const char *modname)
{
	struct index_file *index;
	char *filename, *value;

	nofail_asprintf(&filename, "%s/modules.builtin.bin", dirname);
	index = index_file_open(filename);
	free(filename);

	/* return -1 if no builtin list available (modern depmod file) */
	if (!index)
		return -1;

	value = index_search(index, modname);
	free(value);
	return value ? 1 : 0;
}

/**
 * module_in_sysfs - try to determine if module has a sysfs entry
 *
 * @modname:	module name
 * @usecount:	update use count if possible (-1 if unknown)
 *
 */
static int module_in_sysfs(const char *modname, unsigned int *usecount)
{
	int ret;
	char *name;
	struct stat finfo;

	const int ATTR_LEN = 16;
	char attr[ATTR_LEN];

	/* Check sysfs is mounted */
	if (stat("/sys/module", &finfo) < 0)
		return -1;

	/* Find module. */
	nofail_asprintf(&name, "/sys/module/%s", modname);
	ret = stat(name, &finfo);
	free(name);
	if (ret < 0)
		return (errno == ENOENT) ? 0 : -1; /* Not found or unknown. */

	nofail_asprintf(&name, "/sys/module/%s/initstate", modname);
	ret = read_attribute(name, attr, ATTR_LEN);
	if (ret == 0) {
		free(name);
		nofail_asprintf(&name, "/sys/module/%s", modname);
		if (stat(name, &finfo) < 0) {
			/* module was removed before we could read initstate */
			ret = 0;
		} else {
			/* initstate not available (2.6.19 or earlier) */
			ret = -1;
		}
		free(name);
		return ret;
	}

	/* Wait for the existing module to either go live or disappear. */
	while (ret == 1 && !streq(attr, "live\n")) {
		usleep(100000);
		ret = read_attribute(name, attr, ATTR_LEN);
	}
	free(name);

	if (ret != 1)
		return ret;

	/* Get reference count, if it exists. */
	if (usecount != NULL) {
		nofail_asprintf(&name, "/sys/module/%s/refcnt", modname);
		ret = read_attribute(name, attr, ATTR_LEN);
		free(name);
		if (ret == 1)
			*usecount = atoi(attr);
	}

	return 1;
}

/**
 * module_in_kernel - try to determine if the module is loaded (sysfs,procfs)
 *
 * @modname:	name of module
 * @usecount:	update module use count (-1 if unknown)
 *
 */
static int module_in_kernel(const char *modname, unsigned int *usecount)
{
	int result;

	result = module_in_sysfs(modname, usecount);
	if (result != -1)
		return result;

	/* /sys/module/%s/initstate is only available since 2.6.20,
	   fallback to /proc/modules to get module state on earlier kernels. */
	return module_in_procfs(modname, usecount);
}

/**
 * dump_modversions - output list of module version checksums
 *
 * @filename:	module file name
 * @error:	error function to use
 *
 */
static void dump_modversions(const char *filename, errfn_t error)
{
	struct elf_file *module;

	module = grab_elf_file(filename);
	if (!module) {
		error("%s: %s\n", filename, strerror(errno));
		return;
	}
	if (module->ops->dump_modvers(module) < 0)
		error("Wrong section size in '%s'\n", filename);
	release_elf_file(module);
}

/**
 * type_matches - constrain wildcard module matching to subdirectory
 *
 * @path:	path string
 * @subpath:	search path for subpath
 *
 * Legacy function used to support deprecated option
 */
static int type_matches(const char *path, const char *subpath)
{
	char *subpath_with_slashes;
	int ret;

	nofail_asprintf(&subpath_with_slashes, "/%s/", subpath);

	ret = (strstr(path, subpath_with_slashes) != NULL);
	free(subpath_with_slashes);
	return ret;
}

/**
 * do_wildcard - match modules (possibly in directory names containing "type")
 *
 * @dirname:	module directory
 * @type:	possible subdirectory limiting
 * @wildcard:	what to match
 *
 */
static int do_wildcard(const char *dirname,
		       const char *type,
		       const char *wildcard)
{
	char *modules_dep_name;
	char *line, *wcard;
	FILE *modules_dep;

	/* Canonicalize wildcard */
	wcard = strdup(wildcard);
	underscores(wcard);

	nofail_asprintf(&modules_dep_name, "%s/%s", dirname, "modules.dep");
	modules_dep = fopen(modules_dep_name, "r");
	if (!modules_dep)
		fatal("Could not load %s: %s\n",
		      modules_dep_name, strerror(errno));

	while ((line = getline_wrapped(modules_dep, NULL)) != NULL) {
		char *ptr;

		/* Ignore lines without : or which start with a # */
		ptr = strchr(line, ':');
		if (ptr == NULL || line[strspn(line, "\t ")] == '#')
			goto next;
		*ptr = '\0';

		/* "type" must match complete directory component(s). */
		if (!type || type_matches(line, type)) {
			char modname[strlen(line)+1];

			filename2modname(modname, line);
			if (fnmatch(wcard, modname, 0) == 0)
				printf("%s\n", line);
		}
	next:
		free(line);
	}

	free(modules_dep_name);
	free(wcard);
	return 0;
}

/**
 * strsep_skipspace - ignore certain delimitor characters in strings
 *
 * @string:	what to search
 * @delim:	delimitor string
 *
 */
static char *strsep_skipspace(char **string, char *delim)
{
	if (!*string)
		return NULL;
	*string += strspn(*string, delim);
	return strsep(string, delim);
}

static int parse_config_scan(struct modprobe_conf *conf,
			     int dump_only,
			     int removing, ...);

/**
 * parse_config_file - read in configuration file options
 *
 * @filename:	name of file
 * @conf:	config options lists
 * @dump_only:	print out config
 * @removing:	determine whether to run install/softdep/etc.
 *
 */
static int parse_config_file(const char *filename,
			    struct modprobe_conf *conf,
			    int dump_only,
			    int removing)
{
	char *line;
	unsigned int linenum = 0;
	FILE *cfile;

	struct module_options **options = &conf->options;
	struct module_command **commands = &conf->commands;
	struct module_alias **aliases = &conf->aliases;
	struct module_blacklist **blacklist = &conf->blacklist;

	cfile = fopen(filename, "r");
	if (!cfile)
		return 0;

	while ((line = getline_wrapped(cfile, &linenum)) != NULL) {
		char *ptr = line;
		char *cmd, *modname;

		/* output configuration */
		if (dump_only)
			printf("%s\n", line);

		cmd = strsep_skipspace(&ptr, "\t ");
		if (cmd == NULL || cmd[0] == '#' || cmd[0] == '\0') {
			free(line);
			continue;
		}

		if (streq(cmd, "alias")) {
			char *wildcard = strsep_skipspace(&ptr, "\t ");
			char *realname = strsep_skipspace(&ptr, "\t ");
			if (!wildcard || !realname)
				goto syntax_error;
			*aliases = add_alias(underscores(wildcard),
					     underscores(realname),
					     *aliases);
		} else if (streq(cmd, "include")) {
			struct modprobe_conf newconf = *conf;
			newconf.aliases = NULL;
			char *newfilename;
			newfilename = strsep_skipspace(&ptr, "\t ");
			if (!newfilename)
				goto syntax_error;

			warn("\"include %s\" is deprecated, "
			     "please use /etc/modprobe.d\n", newfilename);
			if (strstarts(newfilename, "/etc/modprobe.d")) {
				warn("\"include /etc/modprobe.d\" is "
				     "the default, ignored\n");
			} else {
				if (!parse_config_scan(&newconf, dump_only,
						       removing, newfilename,
						       NULL))
					warn("Failed to open included"
					      " config file %s: %s\n",
					      newfilename, strerror(errno));
			}
			/* Files included override aliases,
			   etc that was already set ... */
			if (newconf.aliases)
				*aliases = newconf.aliases;

		} else if (streq(cmd, "options")) {
			modname = strsep_skipspace(&ptr, "\t ");
			if (!modname || !ptr)
				goto syntax_error;

			ptr += strspn(ptr, "\t ");
			*options = add_options(underscores(modname),
					       ptr, *options);

		} else if (streq(cmd, "install")) {
			modname = strsep_skipspace(&ptr, "\t ");
			if (!modname || !ptr)
				goto syntax_error;
			if (!removing) {
				ptr += strspn(ptr, "\t ");
				*commands = add_command(underscores(modname),
							ptr, *commands);
			}
		} else if (streq(cmd, "blacklist")) {
			modname = strsep_skipspace(&ptr, "\t ");
			if (!modname)
				goto syntax_error;
			if (!removing) {
				*blacklist = add_blacklist(underscores(modname),
							*blacklist);
			}
		} else if (streq(cmd, "remove")) {
			modname = strsep_skipspace(&ptr, "\t ");
			if (!modname || !ptr)
				goto syntax_error;
			if (removing) {
				ptr += strspn(ptr, "\t ");
				*commands = add_command(underscores(modname),
							ptr, *commands);
			}
		} else if (streq(cmd, "softdep")) {
			char *tk;
			int pre = 0, post = 0;
			struct string_table *pre_modnames = NULL;
			struct string_table *post_modnames = NULL;
			struct module_softdep *new;

			modname = strsep_skipspace(&ptr, "\t ");
			if (!modname || !ptr)
				goto syntax_error;
			modname = underscores(modname);

			while ((tk = strsep_skipspace(&ptr, "\t ")) != NULL) {
				tk = underscores(tk);

				if (streq(tk, "pre:")) {
					pre = 1; post = 0;
				} else if (streq(tk, "post:")) {
					pre = 0; post = 1;
				} else if (pre) {
					pre_modnames = NOFAIL(
						strtbl_add(tk, pre_modnames));
				} else if (post) {
					post_modnames = NOFAIL(
						strtbl_add(tk, post_modnames));
				} else {
					strtbl_free(pre_modnames);
					strtbl_free(post_modnames);
					goto syntax_error;
				}
			}
			new = NOFAIL(malloc(sizeof(*new)));
			new->buf = line;
			new->modname = modname;
			new->pre = pre_modnames;
			new->post = post_modnames;
			new->next = conf->softdeps;
			conf->softdeps = new;

			line = NULL; /* Don't free() this line. */
				     /* allocated in buf above */

		} else if (streq(cmd, "config")) {
			char *tmp = strsep_skipspace(&ptr, "\t ");

			if (!tmp)
				goto syntax_error;
			if (streq(tmp, "binary_indexes")) {
				tmp = strsep_skipspace(&ptr, "\t ");
				if (streq(tmp, "yes"))
					use_binary_indexes = 1;
				if (streq(tmp, "no"))
					use_binary_indexes = 0;
			}
		} else {
syntax_error:
			grammar(cmd, filename, linenum);
		}

		free(line);
	}
	fclose(cfile);
	return 1;
}

/**
 * read_aliases_file - process binary module aliases file
 *
 * @filename:	alias file
 * @name:	module name
 * @dump_only:	dump aliases only
 * @aliases:	list of aliases
 *
 */
static int read_aliases_file(const char *filename,
			     const char *name,
			     int dump_only,
			     struct module_alias **aliases)
{
	struct index_value *realnames;
	struct index_value *realname;
	char *binfile;
	struct index_file *index;

	nofail_asprintf(&binfile, "%s.bin", filename);
	index = index_file_open(binfile);
	if (!index) {
		free(binfile);
		return 0;
	}

	if (dump_only) {
		index_dump(index, stdout, "alias ");
		free(binfile);
		index_file_close(index);
		return 1;
	}

	realnames = index_searchwild(index, name);
	for (realname = realnames; realname; realname = realname->next)
		*aliases = add_alias("*", realname->value, *aliases);
	index_values_free(realnames);

	free(binfile);
	index_file_close(index);
	return 1;
}

/**
 * read_aliases - process module aliases file
 *
 * @filename:	alias file
 * @name:	module name
 * @dump_only:	dump aliases only
 * @aliases:	list of aliases
 *
 */
static int read_aliases(const char *filename,
			const char *name,
			int dump_only,
			struct module_alias **aliases)
{
	char *line;
	unsigned int linenum = 0;
	FILE *cfile;

	/* prefer the binary version if available and configured */
	if (use_binary_indexes)
		if (read_aliases_file(filename, name, dump_only, aliases))
			return 1;

	cfile = fopen(filename, "r");
	if (!cfile)
		return 0;

	while ((line = getline_wrapped(cfile, &linenum)) != NULL) {
		char *ptr = line;
		char *cmd;

		if (dump_only)
			printf("%s\n", line);

		cmd = strsep_skipspace(&ptr, "\t ");
		if (cmd == NULL || cmd[0] == '#' || cmd[0] == '\0') {
			free(line);
			continue;
		}

		if (streq(cmd, "alias")) {
			char *wildcard = strsep_skipspace(&ptr, "\t ");
			char *realname = strsep_skipspace(&ptr, "\t ");
			if (!wildcard || !realname)
				goto syntax_error;
			if (fnmatch(underscores(wildcard),name,0) == 0)
				*aliases = add_alias(wildcard,
						     underscores(realname),
						     *aliases);
		} else {
syntax_error:
			grammar(cmd, filename, linenum);
		}

		free(line);
	}
	fclose(cfile);
	return 1;
}

/**
 * parse_config_scan - process a directory of configuration files
 *
 * @conf:	config options lists
 * @dump_only:	print out config
 * @removing:	determine whether to run install/softdep/etc.
 *
 */
static int parse_config_scan(struct modprobe_conf *conf,
			     int dump_only,
			     int removing, ...)
{
	va_list filelist;
	char *filename;
	DIR *dir;
	struct file_entry {
		struct list_head node;
		char *name;
		char *path;
	};
	struct file_entry *fe, *fe_tmp;
	LIST_HEAD(files_list);
	int ret = 0;

	va_start(filelist, removing);

	while ((filename = va_arg(filelist, char*))) {
		dir = opendir(filename);
		if (dir) {
			struct dirent *i;

			/* sort files from directories into list, ignoring duplicates */
			while ((i = readdir(dir)) != NULL) {
				size_t len;
				int cmp = -1;

				if (i->d_name[0] == '.')
					continue;
				if (!config_filter(i->d_name))
					continue;

				len = strlen(i->d_name);
				if (len < 6 ||
				    (strcmp(&i->d_name[len-5], ".conf") != 0 &&
				     strcmp(&i->d_name[len-6], ".alias") != 0))
					warn("All config files need .conf: %s/%s, "
					     "it will be ignored in a future release.\n",
					     filename, i->d_name);
				fe = malloc(sizeof(struct file_entry));
				if (fe == NULL)
					continue;

				list_for_each_entry(fe_tmp, &files_list, node)
					if ((cmp = strcmp(fe_tmp->name, i->d_name)) >= 0)
						break;

				if (cmp != 0) {
					fe->name = malloc(len + 1);
					fe->path = malloc(strlen(filename) + 1);
					strcpy(fe->name, i->d_name);
					strcpy(fe->path, filename);

					if (cmp < 0)
						list_add_tail(&fe->node, &files_list);
					else
						list_add_tail(&fe->node, &fe_tmp->node);
				} else
					info("Ignoring config file %s/%s\n", filename, i->d_name);

			}
			closedir(dir);

			ret = 1;
		} else {
			if (parse_config_file(filename, conf, dump_only, removing))
				ret = 1;
		}
	}

	/* parse list of files */
	list_for_each_entry_safe(fe, fe_tmp, &files_list, node) {
		char *cfgfile;

		nofail_asprintf(&cfgfile, "%s/%s", fe->path, fe->name);
		if (!parse_config_file(cfgfile, conf,
				       dump_only, removing))
			warn("Failed to open config file %s: %s\n",
			     cfgfile, strerror(errno));
		free(cfgfile);
		list_del(&fe->node);
		free(fe->name);
		free(fe->path);
		free(fe);
	}

	va_end(filelist);
	return ret;
}

/**
 * parse_toplevel_config - search configuration directories
 *
 * @filename:	specified on command line
 * @conf:	config options lists
 * @dump_only:	print out config
 * @removing:	determine whether to run install/softdep/etc.
 *
 */
static void parse_toplevel_config(const char *filename,
				  struct modprobe_conf *conf,
				  int dump_only,
				  int removing)
{
	if (filename) {
		if (!parse_config_scan(conf, dump_only, removing, filename,
				       NULL))
			fatal("Failed to open config file %s: %s\n",
			      filename, strerror(errno));
		return;
	}

	/* deprecated config file */
	if (parse_config_file("/etc/modprobe.conf", conf,
			      dump_only, removing) > 0)
		warn("Deprecated config file /etc/modprobe.conf, "
		      "all config files belong into /etc/modprobe.d/.\n");

	/* default config */
	parse_config_scan(conf, dump_only, removing, "/run/modprobe.d",
			  "/etc/modprobe.d", "/usr/local/lib/modprobe.d",
			  "/lib/modprobe.d", NULL);
}

/**
 * parse_kcmdline - process configuration options on kernel boot line
 *
 * @dump_only:	print out config
 * @conf:	config options lists
 *
 */
static int parse_kcmdline(int dump_only, struct modprobe_conf *conf)
{
	char *line;
	unsigned int linenum = 0;
	FILE *kcmdline;
	struct module_options **options = &conf->options;
	struct module_blacklist **blacklist = &conf->blacklist;

	kcmdline = fopen("/proc/cmdline", "r");
	if (!kcmdline)
		return 0;

	while ((line = getline_wrapped(kcmdline, &linenum)) != NULL) {
		char *ptr = line;
		char *arg;

		while ((arg = strsep_skipspace(&ptr, "\t ")) != NULL) {
			char *sep, *modname, *opt;

			if (strstr(arg, "modprobe.blacklist=") != NULL) {
				ptr = strchr(arg,'=') + 1;

				while ((modname = strsep(&ptr, ",")) != NULL) {
					if (dump_only)
						printf("blacklist %s\n", modname);

					*blacklist = add_blacklist(underscores(modname), *blacklist);
				}
			}

			sep = strchr(arg, '.');
			if (sep) {
				if (!strchr(sep, '='))
					continue;
				modname = arg;
				*sep = '\0';
				opt = ++sep;

				if (dump_only)
					printf("options %s %s\n", modname, opt);

				*options = add_options(underscores(modname),
						       opt, *options);
			}
		}

		free(line);
	}
	fclose(kcmdline);
	return 1;
}

/**
 * add_to_env_var - set environment variables
 *
 * @option:	variable to set
 *
 */
static void add_to_env_var(const char *option)
{
	const char *oldenv;

	if ((oldenv = getenv("MODPROBE_OPTIONS")) != NULL) {
		char *newenv;
		nofail_asprintf(&newenv, "%s %s", oldenv, option);
		setenv("MODPROBE_OPTIONS", newenv, 1);
		free(newenv);
	} else
		setenv("MODPROBE_OPTIONS", option, 1);
}

/**
 * merge_args - load options from the environment (head)
 *
 * @args:	list of arguments
 * @argv:	program arguments
 * @argc:	argument count
 *
 */
static char **merge_args(char *args, char *argv[], int *argc)
{
	char *arg, *argstring;
	char **newargs = NULL;
	unsigned int i, num_env = 0;

	if (!args)
		return argv;

	argstring = NOFAIL(strdup(args));
	for (arg = strtok(argstring, " "); arg; arg = strtok(NULL, " ")) {
		num_env++;
		newargs = NOFAIL(realloc(newargs,
					 sizeof(newargs[0])
					 * (num_env + *argc + 1)));
		newargs[num_env] = arg;
	}

	if (!newargs)
		return argv;

	/* Append commandline args */
	newargs[0] = argv[0];
	for (i = 1; i <= *argc; i++)
		newargs[num_env+i] = argv[i];

	*argc += num_env;
	return newargs;
}

/**
 * gather_options - load options from command line
 *
 * @argv:	program arguments
 *
 */
static char *gather_options(char *argv[])
{
	char *optstring = NOFAIL(strdup(""));

	/* Rest is module options */
	while (*argv) {
		/* Quote value if it contains spaces. */
		unsigned int eq = strcspn(*argv, "=");

		if (strchr(*argv+eq, ' ') && !strchr(*argv, '"')) {
			char quoted[strlen(*argv) + 3];
			(*argv)[eq] = '\0';
			sprintf(quoted, "%s=\"%s\"", *argv, *argv+eq+1);
			optstring = append_option(optstring, quoted);
		} else
			optstring = append_option(optstring, *argv);
		argv++;
	}
	return optstring;
}

/**
 * do_command - execute a module install or remove command
 *
 * @modname:		module name
 * @command:		command string
 * @dry_run:		possibly do nothing
 * @error:		error function
 * @type:		install or remove
 * @cmdline_opts:	substitute for $CMDLINE_OPTS
 *
 */
static void do_command(const char *modname,
		       const char *command,
		       int dry_run,
		       errfn_t error,
		       const char *type,
		       const char *cmdline_opts)
{
	int ret;
	char *p, *replaced_cmd = NOFAIL(strdup(command));

	while ((p = strstr(replaced_cmd, "$CMDLINE_OPTS")) != NULL) {
		char *new;
		nofail_asprintf(&new, "%.*s%s%s",
			 (int)(p - replaced_cmd), replaced_cmd, cmdline_opts,
			 p + strlen("$CMDLINE_OPTS"));
		free(replaced_cmd);
		replaced_cmd = new;
	}

	info("%s %s\n", type, replaced_cmd);
	if (dry_run)
		goto out;

	setenv("MODPROBE_MODULE", modname, 1);
	ret = system(replaced_cmd);
	if (ret == -1 || WEXITSTATUS(ret))
		error("Error running %s command for %s\n", type, modname);

out:
	free(replaced_cmd);
}

/* Forward declaration */
static int do_modprobe(const char *modname,
		const char *cmdline_opts,
		const struct modprobe_conf *conf,
		const char *dirname,
		errfn_t error,
		modprobe_flags_t flags);

/**
 * do_softdep - load or unload module soft dependencies
 *
 * @softdep:		soft dependency module(s)
 * @cmdline_opts:	command line options
 * @conf:		config options lists
 * @dirname:		module directory
 * @error:		error function
 * @flags:		general flags
 *
 */
static void do_softdep(const struct module_softdep *softdep,
		       const char *cmdline_opts,
		       const struct modprobe_conf *conf,
		       const char *dirname,
		       errfn_t error,
		       modprobe_flags_t flags)
{
	struct string_table *pre_modnames, *post_modnames;
	modprobe_flags_t softdep_flags = flags;
	int i, j;

	softdep_flags &= ~mit_first_time;
	softdep_flags &= ~mit_ignore_commands;
	if (flags & mit_remove)
		softdep_flags |= mit_quiet_inuse;

	if (++recursion_depth >= MAX_RECURSION)
		fatal("modprobe: softdep dependency loop encountered %s %s\n",
			(flags & mit_remove) ? "removing" : "inserting",
			softdep->modname);

	if (flags & mit_remove) {
		/* Reverse module order if removing. */
		pre_modnames = softdep->post;
		post_modnames = softdep->pre;
	} else {
		pre_modnames = softdep->pre;
		post_modnames = softdep->post;
	}

	/* Modprobe pre_modnames */

	for (i = 0; pre_modnames && i < pre_modnames->cnt; i++) {
		/* Reverse module order if removing. */
		j = (flags & mit_remove) ? pre_modnames->cnt-1 - i : i;

		do_modprobe(pre_modnames->str[j], "",
			conf, dirname, warn, softdep_flags);
	}

	/* Modprobe main module, passing cmdline_opts, ignoring softdep */

	do_modprobe(softdep->modname, cmdline_opts,
		conf, dirname, warn, flags | mit_ignore_commands);

	/* Modprobe post_modnames */

	for (i = 0; post_modnames && i < post_modnames->cnt; i++) {
		/* Reverse module order if removing. */
		j = (flags & mit_remove) ? post_modnames->cnt-1 - i : i;

		do_modprobe(post_modnames->str[j], "", conf,
			dirname, warn, softdep_flags);
	}
}

#define DEVICE_NAME "/dev/lcd"
static int lcd_init_module(void *module_image, unsigned long len,
			char *param_values)
{
	int ret;
	struct lcd_init_module_args ma;
	int fd;

	ma.module_image = module_image;
	ma.len = len;
	ma.param_values = param_values;

	fd = open(DEVICE_NAME, O_RDONLY);
	if (fd < 0) {
		ret = -1;
		perror("lcd-modprobe");
		goto fail;
	}
	ret = ioctl(fd, LCD_INIT_MODULE, &ma);
	if (ret) {
		perror("lcd-modprobe");
		goto fail_fd;
	}

	close(fd);

	return 0;

fail_fd:
	close(fd);
fail:
	return ret;
}

/**
 * insmod - load a module(s)
 *
 * @list:		list of modules
 * @optstring:		module options
 * @cmdline_opts:	command line options
 * @conf:		config options lists
 * @dirname:		module directory
 * @error:		error function
 * @flags:		general flags
 *
 */
static int insmod(struct list_head *list,
		   const char *optstring,
		   const char *cmdline_opts,
		   const struct modprobe_conf *conf,
		   const char *dirname,
		   errfn_t error,
		   modprobe_flags_t flags)
{
	int ret;
	struct elf_file *module;
	const struct module_softdep *softdep;
	const char *command;
	struct module *mod = list_entry(list->next, struct module, list);
	int rc = 0;
	int already_loaded;
	char *opts = NULL;

	/* Take us off the list. */
	list_del(&mod->list);

	/* Do things we (or parent) depend on first. */
	if (!list_empty(list)) {
		modprobe_flags_t f = flags;
		f &= ~mit_first_time;
		f &= ~mit_ignore_commands;
		if ((rc = insmod(list, "", "", conf, dirname, warn, f)) != 0)
		{
			error("Error inserting %s (%s): %s\n",
				mod->modname, mod->filename,
				insert_moderror(errno));
			goto out;
		}
	}

	/* Don't do ANYTHING if already in kernel. */
	already_loaded = module_in_kernel(mod->modname, NULL);

	if (!(flags & mit_ignore_loaded) && already_loaded == 1) {
		if (flags & mit_first_time)
			error("Module %s already in kernel.\n", mod->modname);
		goto out;
	}

	/* load any soft dependency modules */
	softdep = find_softdep(mod->modname, conf->softdeps);
	if (softdep && !(flags & mit_ignore_commands)) {
		do_softdep(softdep, cmdline_opts, conf, dirname, error, flags);
		goto out;
	}

	/* run any install commands for this module */
	command = find_command(mod->modname, conf->commands);
	if (command && !(flags & mit_ignore_commands)) {
		if (already_loaded == -1) {
			warn("/sys/module/ not present or too old,"
				" and /proc/modules does not exist.\n");
			warn("Ignoring install commands for %s"
				" in case it is already loaded.\n",
				mod->modname);
		} else {
			do_command(mod->modname, command, flags & mit_dry_run,
				   error, "install", cmdline_opts);
			goto out;
		}
	}

	/* open the module */
	module = grab_elf_file(mod->filename);
	if (!module) {
		error("Could not read '%s': %s\n", mod->filename,
			(errno == ENOEXEC) ? "Invalid module format" :
				strerror(errno));
		goto out;
	}
	if (flags & mit_strip_modversion)
		module->ops->strip_section(module, "__versions");
	if (flags & mit_strip_vermagic)
		clear_magic(module);

	/* Config file might have given more options */
	opts = add_extra_options(mod->modname, optstring, conf->options);

	info("insmod %s %s\n", mod->filename, opts);

	if (flags & mit_dry_run)
		goto out_elf_file;

	/* request kernel linkage */
	ret = lcd_init_module(module->data, module->len, opts);
	if (ret != 0) {
		if (errno == EEXIST) {
			if (flags & mit_first_time)
				error("Module %s already in kernel.\n",
				      mod->modname);
			goto out_elf_file;
		}
		/* don't warn noisely if we're loading multiple aliases. */
		/* one of the aliases may try to use hardware we don't have. */
		if ((error != warn) || (verbose))
			error("Error inserting %s (%s): %s\n",
			      mod->modname, mod->filename,
			      insert_moderror(errno));
		rc = 1;
	}
 out_elf_file:
	release_elf_file(module);
	free(opts);
 out:
	free_module(mod);
	return rc;
}

/**
 * rmmod - unload a module(s)
 *
 * @list:		list of modules
 * @cmdline_opts:	command line options
 * @conf:		config options lists
 * @dirname:		module directory
 * @error:		error function
 * @flags:		general flags
 *
 */
static void rmmod(struct list_head *list,
		  const char *cmdline_opts,
		  const struct modprobe_conf *conf,
		  const char *dirname,
		  errfn_t error,
		  modprobe_flags_t flags)
{
	const struct module_softdep *softdep;
	const char *command;
	unsigned int usecount = 0;
	struct module *mod = list_entry(list->next, struct module, list);
	int exists;

	/* Take first one off the list. */
	list_del(&mod->list);

	/* Don't do ANYTHING if not loaded. */
	exists = module_in_kernel(mod->modname, &usecount);
	if (exists == 0)
		goto nonexistent_module;

	/* Even if renamed, find commands/softdeps to orig. name. */

	softdep = find_softdep(mod->modname, conf->softdeps);
	if (softdep && !(flags & mit_ignore_commands)) {
		do_softdep(softdep, cmdline_opts, conf, dirname, error, flags);
		goto remove_rest;
	}

	/* run any remove commands for this module */
	command = find_command(mod->modname, conf->commands);
	if (command && !(flags & mit_ignore_commands)) {
		if (exists == -1) {
			warn("/sys/module/ not present or too old,"
				" and /proc/modules does not exist.\n");
			warn("Ignoring remove commands for %s"
				" in case it is not loaded.\n",
				mod->modname);
		} else {
			do_command(mod->modname, command, flags & mit_dry_run,
				   error, "remove", cmdline_opts);
			goto remove_rest;
		}
	}

	/* abort if use count is not zero */
	if (usecount != 0) {
		if (!(flags & mit_quiet_inuse))
			error("Module %s is in use.\n", mod->modname);
		goto remove_rest;
	}

	info("rmmod %s\n", mod->filename);

	if (flags & mit_dry_run)
		goto remove_rest;

	/* request kernel unlinkage */
	if (delete_module(mod->modname, O_EXCL) != 0) {
		if (errno == ENOENT)
			goto nonexistent_module;
		error("Error removing %s (%s): %s\n",
		      mod->modname, mod->filename,
		      remove_moderror(errno));
	}

 remove_rest:
	/* Now do things we depend. */
	if (!list_empty(list)) {
		flags &= ~mit_first_time;
		flags &= ~mit_ignore_commands;
		flags |= mit_quiet_inuse;

		rmmod(list, "", conf, dirname, warn, flags);
	}
	free_module(mod);
	return;

nonexistent_module:
	if (flags & mit_first_time)
		fatal("Module %s is not in kernel.\n", mod->modname);
	goto remove_rest;
}

/**
 * handle_module - load or unload a module(s)
 *
 * @modname:		module requested
 * @todo_list:		dependency list
 * @options:		module options
 * @cmdline_opts:	options passed on command line
 * @conf:		config options lists
 * @dirname:		module directory
 * @error:		error function
 * @flags:		general flags
 *
 */
static int handle_module(const char *modname,
			  struct list_head *todo_list,
			  const char *options,
			  const char *cmdline_opts,
			  const struct modprobe_conf *conf,
			  const char *dirname,
			  errfn_t error,
			  modprobe_flags_t flags)
{
	if (list_empty(todo_list)) {
		const char *command;

		/* The dependencies have to be real modules, but
		   handle case where the first is completely bogus. */

		command = find_command(modname, conf->commands);
		if (command && !(flags & mit_ignore_commands)) {
			do_command(modname, command, flags & mit_dry_run, error,
				   (flags & mit_remove) ? "remove":"install", cmdline_opts);
			return 0;
		}

		if (!quiet)
			error("Module %s not found.\n", modname);
		return 1;
	}

	if (flags & mit_remove)
		rmmod(todo_list, cmdline_opts,
		      conf, dirname, error, flags);
	else
		insmod(todo_list, options,
		       cmdline_opts, conf, dirname, error, flags);

	return 0;
}

/**
 * handle_builtin_module - report built-in modules
 *
 * @modname:	module name
 * @error:	error function
 * @flags:	general flags
 *
 */
static int handle_builtin_module(const char *modname,
                          errfn_t error,
                          modprobe_flags_t flags)
{
	if (flags & mit_remove) {
		error("Module %s is builtin\n", modname);
		return 1;
	} else if (flags & mit_first_time) {
		error("Module %s already in kernel (builtin).\n", modname);
		return 1;
	} else if (flags & mit_ignore_loaded) {
		/* --show-depends given */
		info("builtin %s\n", modname);
	}
	return 0;
}

/**
 * do_modprobe - find a module by name or alias and load or unload
 *
 * @modname:		module name
 * @cmdline_opts:	command line options
 * @conf:		config options lists
 * @dirname:		module directory
 * @error:		error function
 * @flags:		general flags
 *
 */
static int do_modprobe(const char *modname,
		const char *cmdline_opts,
		const struct modprobe_conf *conf,
		const char *dirname,
		errfn_t error,
		modprobe_flags_t flags)
{
	struct module_alias *matching_aliases;
	LIST_HEAD(list);
	int failed = 0;

	matching_aliases = find_aliases(conf->aliases, modname);

	/* No luck?  Try symbol names, if starts with symbol:. */
	if (!matching_aliases && strstarts(modname, "symbol:")) {
		char *symfilename;

		nofail_asprintf(&symfilename, "%s/modules.symbols", dirname);
		read_aliases(symfilename, modname, 0, &matching_aliases);
		free(symfilename);
	}
	if (!matching_aliases) {
		if(!strchr(modname, ':'))
			read_depends(dirname, modname, &list);

		/* We only use canned aliases as last resort. */
		if (list_empty(&list)
		    && !find_softdep(modname, conf->softdeps)
		    && !find_command(modname, conf->commands))
		{
			char *aliasfilename;

			nofail_asprintf(&aliasfilename, "%s/modules.alias",
					dirname);
			read_aliases(aliasfilename, modname, 0,
				     &matching_aliases);
			free(aliasfilename);
			/* builtin module? */
			if (!matching_aliases && module_builtin(dirname, modname) > 0) {
				failed |= handle_builtin_module(modname, error,
								flags);
				goto out;
			}
		}
	}

	/* only load blacklisted modules with specific request (no alias) */
	apply_blacklist(&matching_aliases, conf->blacklist);

	if(flags & mit_resolve_alias) {
		struct module_alias *aliases = matching_aliases;

		for(; aliases; aliases=aliases->next)
			printf("%s\n", aliases->module);
		goto out;
	}
	if (matching_aliases) {
		errfn_t err = error;
		struct module_alias *aliases = matching_aliases;

		/* More than one alias?  Don't bail out on failure. */
		if (aliases->next)
			err = warn;
		while (aliases) {
			/* Add the options for this alias. */
			char *opts;
			opts = add_extra_options(modname,
						 cmdline_opts, conf->options);

			read_depends(dirname, aliases->module, &list);
			failed |= handle_module(aliases->module,
				&list, opts, cmdline_opts,
				conf, dirname, err, flags);

			aliases = aliases->next;
			free(opts);
			INIT_LIST_HEAD(&list);
		}
	} else {
		if (flags & mit_use_blacklist
		    && find_blacklist(modname, conf->blacklist))
			goto out;

		failed |= handle_module(modname, &list, cmdline_opts,
			cmdline_opts, conf, dirname, error, flags);
	}

out:
	free_aliases(matching_aliases);
	return failed;
}

static const struct option options[] = { { "version", 0, NULL, 'V' },
				   { "verbose", 0, NULL, 'v' },
				   { "quiet", 0, NULL, 'q' },
				   { "syslog", 0, NULL, 's' },
				   { "show", 0, NULL, 'n' },
				   { "dry-run", 0, NULL, 'n' },
				   { "show-depends", 0, NULL, 'D' },
				   { "resolve-alias", 0, NULL, 'R' },
				   { "dirname", 1, NULL, 'd' },
				   { "set-version", 1, NULL, 'S' },
				   { "config", 1, NULL, 'C' },
				   { "remove", 0, NULL, 'r' },
				   { "showconfig", 0, NULL, 'c' },
				   { "list", 0, NULL, 'l' },
				   { "type", 1, NULL, 't' },
				   { "all", 0, NULL, 'a' },
				   { "ignore-install", 0, NULL, 'i' },
				   { "ignore-remove", 0, NULL, 'i' },
				   { "use-blacklist", 0, NULL, 'b' },
				   { "force", 0, NULL, 'f' },
				   { "force-vermagic", 0, NULL, 1 },
				   { "force-modversion", 0, NULL, 2 },
				   { "first-time", 0, NULL, 3 },
				   { "dump-modversions", 0, NULL, 4 },
				   { NULL, 0, NULL, 0 } };

int main(int argc, char *argv[])
{
	struct utsname buf;
	struct stat statbuf;
	int opt;
	int dump_config = 0;
	int list_only = 0;
	int all = 0;
	int dump_modver = 0;
	unsigned int i, num_modules;
	char *type = NULL;
	const char *configname = NULL;
	char *basedir = "";
	char *cmdline_opts = NULL;
	char *dirname;
	errfn_t error = fatal;
	int failed = 0;
	modprobe_flags_t flags = 0;
	struct modprobe_conf conf = {};

	recursion_depth = 0;

	/* Prepend options from environment. */
	argv = merge_args(getenv("MODPROBE_OPTIONS"), argv, &argc);

	uname(&buf);
	while ((opt = getopt_long(argc, argv, "Vvqsnd:S:C:DRrclt:aibf", options, NULL)) != -1){
		switch (opt) {
		case 'V':
			puts(PACKAGE " version " VERSION);
			exit(0);
		case 'v':
			add_to_env_var("-v");
			verbose = 1;
			break;
		case 'q':
			quiet = 1;
			add_to_env_var("-q");
			break;
		case 's':
			add_to_env_var("-s");
			logging = 1;
			break;
		case 'n':
			flags |= mit_dry_run;
			break;
		case 'd':
			basedir = optarg;
			break;
		case 'S':
			strncpy(buf.release, optarg, sizeof(buf.release));
			buf.release[sizeof(buf.release)-1] = '\0';
			break;
		case 'C':
			configname = optarg;
			add_to_env_var("-C");
			add_to_env_var(configname);
			break;
		case 'D':
			flags |= mit_dry_run;
			flags |= mit_ignore_loaded;
			verbose = 1;
			break;
		case 'R':
			flags |= mit_resolve_alias;
			break;
		case 'r':
			flags |= mit_remove;
			break;
		case 'c':
			dump_config = 1;
			break;
		case 'l':
			list_only = 1;
			break;
		case 't':
			type = optarg;
			break;
		case 'a':
			all = 1;
			error = warn;
			break;
		case 'i':
			flags |= mit_ignore_commands;
			break;
		case 'b':
			flags |= mit_use_blacklist;
			break;
		case 'f':
			flags |= mit_strip_vermagic;
			flags |= mit_strip_modversion;
			break;
		case 1:
			flags |= mit_strip_vermagic;
			break;
		case 2:
			flags |= mit_strip_modversion;
			break;
		case 3:
			flags |= mit_first_time;
			break;
		case 4:
			dump_modver = 1;
			break;
		default:
			print_usage(argv[0]);
		}
	}

	/* If stderr not open, go to syslog */
	if (logging || fstat(STDERR_FILENO, &statbuf) != 0) {
		openlog("modprobe", LOG_CONS, LOG_DAEMON);
		logging = 1;
	}

	if (argc < optind + 1 && !dump_config && !list_only)
		print_usage(argv[0]);

	nofail_asprintf(&dirname, "%s%s/%s", basedir, MODULE_DIR, buf.release);

	/* Old-style -t xxx wildcard?  Only with -l. */
	if (list_only) {
		if (optind+1 < argc)
			fatal("Can't have multiple wildcards\n");
		/* fprintf(stderr, "man find\n"); return 1; */
		failed = do_wildcard(dirname, type, argv[optind]?:"*");
		goto out;
	}
	if (type)
		fatal("-t only supported with -l");

	if (dump_modver) {
		dump_modversions(argv[optind], error);
		goto out;
	}

	/* Read aliases, options etc. */
	parse_toplevel_config(configname, &conf, dump_config, flags & mit_remove);

	/* Read module options from kernel command line */
	parse_kcmdline(dump_config, &conf);

	/* report config only? */	
	if (dump_config) {
		char *aliasfilename, *symfilename;
		struct modprobe_conf conf = {};

		nofail_asprintf(&aliasfilename, "%s/modules.alias", dirname);
		nofail_asprintf(&symfilename, "%s/modules.symbols", dirname);

		read_aliases(aliasfilename, "", 1, &conf.aliases);
		read_aliases(symfilename, "", 1, &conf.aliases);

		goto out;
	}

	if ((flags & mit_remove) || all) {
		num_modules = argc - optind;
		cmdline_opts = NOFAIL(strdup(""));
	} else {
		num_modules = 1;
		cmdline_opts = gather_options(argv+optind+1);
	}

	/* Convert names we are looking for */
	for (i = 0; i < num_modules; i++)
		underscores(argv[optind + i]);

	/* If we have a list of modules to remove, try the unused ones first.
	   Aliases and modules which don't seem to exist are handled later. */
	if (flags & mit_remove) {
		int progress;
		do {
			progress = 0;

			for (i = 0; i < num_modules; i++) {
				const char *modname;
				unsigned usecount;
				LIST_HEAD(list);

				modname = argv[optind + i];
				if (!modname)
					continue;
				if (module_in_kernel(modname, &usecount) != 1)
					continue;
				if (usecount != 0)
					continue;

				read_depends(dirname, modname, &list);

				failed |= handle_module(modname, &list,
					cmdline_opts, cmdline_opts,
					&conf, dirname, error, flags);
				progress++;
				argv[optind + i] = NULL;
				INIT_LIST_HEAD(&list);
			}
		} while (progress > 0);
	}

	/* num_modules is always 1 except for -r or -a. */
	for (i = 0; i < num_modules; i++) {
		const char *modname = argv[optind + i];

		if (!modname)
			continue;

		failed |= do_modprobe(modname, cmdline_opts,
			&conf, dirname, error, flags);
	}

out:
	if (logging)
		closelog();
	free(dirname);
	free(cmdline_opts);
	/* Don't bother to free conf */

	exit(failed);
}
