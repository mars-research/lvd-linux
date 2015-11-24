/* Extract module info: useful for both the curious and for scripts. */
#define _GNU_SOURCE /* asprintf rocks */
#include <elf.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/mman.h>

#include "util.h"
#include "logging.h"
#include "elfops.h"
#include "zlibsupport.h"
#include "testing.h"

#ifndef MODULE_DIR
#define MODULE_DIR "/lib/modules"
#endif

struct param
{
	struct param *next;
	const char *name; 	/* Terminated by a colon */
	const char *param;
	const char *type;
};

static struct param *add_param(const char *name, struct param **list)
{
	struct param *i;
	unsigned int namelen = strcspn(name, ":") + 1;

	for (i = *list; i; i = i->next)
		if (strncmp(i->name, name, namelen) == 0)
			return i;
	i = NOFAIL(malloc(sizeof(*i) + namelen+1));
	strncpy((char *)(i + 1), name, namelen);
	((char *)(i + 1))[namelen] = '\0';
	i->name = (char *)(i + 1);
	i->param = NULL;
	i->type = NULL;
	i->next = *list;
	*list = i;
	return i;
}

static void print_tag(const char *tag, struct string_table *tags,
		      const char *filename, char sep)
{
	int j;
	unsigned int taglen = strlen(tag);

	if (streq(tag, "filename")) {
		printf("%s%c", filename, sep);
		return;
	}

	for (j = 0; j < tags->cnt; j++) {
		const char *info = tags->str[j];
		if (strncmp(info, tag, taglen) == 0 && info[taglen] == '=')
			printf("%s%c", info + taglen + 1, sep);
	}
}

static void print_all(struct string_table *tags,
		      const char *filename, char sep)
{
	int j;
	struct param *i, *params = NULL;

	printf("%-16s%s%c", "filename:", filename, sep);
	for (j = 0; j < tags->cnt; j++) {
		const char *info = tags->str[j];
		char *eq, *colon;

		/* We expect this in parm and parmtype. */
		colon = strchr(info, ':');

		/* We store these for handling at the end */
		if (strstarts(info, "parm=") && colon) {
			i = add_param(info + strlen("parm="), &params);
			i->param = colon + 1;
			continue;
		}
		if (strstarts(info, "parmtype=") && colon) {
			i = add_param(info + strlen("parmtype="), &params);
			i->type = colon + 1;
			continue;
		}

		if (!sep) {
			printf("%s%c", info, sep);
			continue;
		}

		eq = strchr(info, '=');
		/* Warn if no '=' maybe? */
		if (eq) {
			char tag[eq - info + 2];
			strncpy(tag, info, eq - info);
			tag[eq-info] = ':';
			tag[eq-info+1] = '\0';
			printf("%-16s%s%c", tag, eq+1, sep);
		}
	}

	/* Now show parameters. */
	for (i = params; i; i = i->next) {
		if (!i->param)
			printf("%-16s%s%s%c", "parm:", i->name, i->type, sep);
		else if (i->type)
			printf("%-16s%s%s (%s)%c",
			       "parm:", i->name, i->param, i->type, sep);
		else 
			printf("%-16s%s%s%c", "parm:", i->name, i->param, sep);
	}
}

static const struct option options[] =
{
	{"author", 0, 0, 'a'},
	{"description", 0, 0, 'd'},
	{"license", 0, 0, 'l'},
	{"parameters", 0, 0, 'p'},
	{"filename", 0, 0, 'n'},
	{"version", 0, 0, 'V'},
	{"help", 0, 0, 'h'},
	{"null", 0, 0, '0'},
	{"field", 1, 0, 'F'},
	{0, 0, 0, 0}
};

/* - and _ are equivalent, and expect suffix. */
static int name_matches(const char *line, const char *end, const char *modname)
{
	unsigned int i;
	char *p;

	/* Ignore comment lines */
	if (line[strspn(line, "\t ")] == '#')
		return 0;

	/* Find last / before colon. */
	p = memchr(line, ':', end - line);
	if (!p)
		return 0;
	while (p > line) {
		if (*p == '/') {
			p++;
			break;
		}
		p--;
	}

	for (i = 0; modname[i]; i++) {
		/* Module names can't have colons. */
		if (modname[i] == ':')
			continue;
		if (modname[i] == p[i])
			continue;
		if (modname[i] == '_' && p[i] == '-')
			continue;
		if (modname[i] == '-' && p[i] == '_')
			continue;
		return 0;
	}
	/* Must match all the way to the extension */
	return (p[i] == '.');
}

static char *next_line(char *p, const char *end)
{
	char *eol;

	eol = memchr(p, '\n', end - p);
	if (eol)
		return eol + 1;
	return (char *)end + 1;
}

static struct elf_file *grab_module(const char *name,
				    const char *kernel,
				    const char *basedir)
{
	char *data;
	unsigned long size;
	struct utsname buf;
	char *depname, *p, *moddir;
	struct elf_file *module;

	if (strchr(name, '.') || strchr(name, '/')) {
		module = grab_elf_file(name);
		if (!module)
			error("modinfo: could not open %s: %s\n",
				name, strerror(errno));
		return module;
	}

	if (!kernel) {
		uname(&buf);
		kernel = buf.release;
	}
	if (strlen(basedir))
		nofail_asprintf(&moddir, "%s/%s/%s", basedir, MODULE_DIR, kernel);
	else
		nofail_asprintf(&moddir, "%s/%s", MODULE_DIR, kernel);

	/* Search for it in modules.dep. */
	nofail_asprintf(&depname, "%s/%s", moddir, "modules.dep");
	data = grab_file(depname, &size);
	if (!data) {
		error("modinfo: could not open %s\n", depname);
		free(depname);
		return NULL;
	}
	free(depname);

	for (p = data; p < data + size; p = next_line(p, data + size)) {
		if (name_matches(p, data + size, name)) {
			int namelen = strcspn(p, ":");
			const char *dir;
			char *filename;

			if ('/' == p[0])
				dir = basedir; /* old style deps - abs. path */
			else
				dir = moddir; /* new style - relative path */

			if (strlen(dir)) {
				nofail_asprintf(&filename, "%s/%s", dir, p);
				filename[namelen + strlen(dir) + 1] = '\0';
			} else {
				filename = strndup(p, namelen);
			}
			release_file(data, size);
			module = grab_elf_file(filename);
			if (!module)
				error("modinfo: could not open %s: %s\n",
					 filename, strerror(errno));
			free(filename);
			return module;
		}
	}
	release_file(data, size);
	error("modinfo: could not find module %s\n", name);
	return NULL;
}

static void usage(const char *name)
{
	fprintf(stderr, "Usage: %s [-0][-F field][-k kernelversion][-b basedir]  module...\n"
		" Prints out the information about one or more module(s).\n"
		" If a fieldname is given, just print out that field (or nothing if not found).\n"
		" Otherwise, print all information out in a readable form\n"
		" If -0 is given, separate with nul, not newline.\n"
		" If -b is given, use an image of the module tree.\n",
		name);
}

int main(int argc, char *argv[])
{
	const char *field = NULL;
	const char *kernel = NULL;
	char sep = '\n';
	int opt, ret = 0;
	char *basedir = "";

	logging = 0; /* send messages to stderr */

	if (native_endianness() == 0)
		abort();

	while ((opt = getopt_long(argc,argv,"adlpVhn0F:k:b:",options,NULL)) >= 0){
		switch (opt) {
		case 'a': field = "author"; break;
		case 'd': field = "description"; break;
		case 'l': field = "license"; break;
		case 'p': field = "parm"; break;
		case 'n': field = "filename"; break;
		case 'V': printf(PACKAGE " version " VERSION "\n"); exit(0);
		case 'F': field = optarg; break;
		case '0': sep = '\0'; break;
		case 'k': kernel = optarg; break;
		case 'b': basedir = optarg; break;
		case 'h': usage(argv[0]); exit(0); break;
		default:
			usage(argv[0]); exit(1);
		}
	}
	if (argc < optind + 1) {
		usage(argv[0]);
		exit(1);
	}

	for (opt = optind; opt < argc; opt++) {
		struct string_table *tags;
		struct elf_file *mod;

		mod = grab_module(argv[opt], kernel, basedir);
		if (!mod) {
			ret = 1;
			continue;
		}
		tags = mod->ops->load_strings(mod, ".modinfo", NULL);
		if (!tags) {
			release_elf_file(mod);
			continue;
		}
		if (field)
			print_tag(field, tags, mod->pathname, sep);
		else
			print_all(tags, mod->pathname, sep);
		strtbl_free(tags);
		release_elf_file(mod);
	}
	return ret;
}
