#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>

#include "util.h"
#include "logging.h"
#include "index.h"

static void write_index(const char *filename)
{
	struct index_node *index;
	char *line, *pos;
	FILE *cfile;
	unsigned int linenum = 0;
	
	cfile = fopen(filename, "w");
	if (!cfile)
		fatal("Could not open %s for writing: %s\n",
		      filename, strerror(errno));
	
	index = index_create();
	
	while((line = getline_wrapped(stdin, &linenum))) {
		pos = strchr(line, ' ');
		*pos++ = '\0';
		index_insert(index, line, pos, linenum);
		free(line);
	}
	
	index_write(index, cfile);
	index_destroy(index);
	fclose(cfile);
}

static struct index_file *open_index(const char *filename)
{
	struct index_file *index;

	index = index_file_open(filename);
	if (!index) {
		if (errno == EINVAL)
			fatal("%s has wrong magic or version number", filename);

		fatal("Could not open %s for reading: %s\n",
		      filename, strerror(errno));
	}

	return index;
}

static void dump_index(const char *filename)
{
	struct index_file *index = open_index(filename);

	index_dump(index, stdout, "");

	index_file_close(index);
}

static void search_index(const char *filename, char *key)
{
	struct index_file *index = open_index(filename);
	char *value;

	value = index_search(index, key);
	if (value)
		printf("Found value:\n%s\n", value);
	else
		printf("Not found.\n");
	
	free(value);
	index_file_close(index);
}

static void searchwild_index(const char *filename, char *key)
{
	struct index_file *index = open_index(filename);
	struct index_value *values, *v;

	values = index_searchwild(index, key);
	if (values)
		printf("Found value(s):\n");
	else
		printf("Not found.\n");	

	for (v = values; v; v = v->next)
		printf("%s\n", v->value);

	index_values_free(values);
	index_file_close(index);
}

static void print_usage(const char *progname)
{
	fprintf(stderr,
		"Usage: %s [MODE] [FILE] ...\n"
		" -o, --output <outfile>\n"
		" -d, --dump <infile>\n"
		" -s, --search <key> <infile>\n"
		" -w, --searchwild <key> <infile>\n"
		,progname);
	exit(1);
}

static const struct option options[] = {
	{ "output", 0, NULL, 'o' },
	{ "dump", 0, NULL, 'd' },
	{ "search", 1, NULL, 's' },
	{ "searchwild", 1, NULL, 'w' },
};

int main(int argc, char *argv[])
{
	int opt;
	char mode = 0;
	char *filename = NULL;
	char *key = NULL;
	
	while ((opt = getopt_long(argc, argv, "ods:w:", options, NULL))
		       != -1) {
		switch (opt) {
			case 'o':
				mode = 'o';
				break;
			case 'd':
				mode = 'd';
				break;
			case 's':
				mode = 's';
				key = optarg;
				break;
			case 'w':
				mode = 'w';
				key = optarg;
				break;
			default:
				print_usage(argv[0]);
		}
	}
	if (!mode)
		print_usage(argv[0]);
	
	if (optind >= argc)
		print_usage(argv[0]);
	filename = argv[optind];
	
	switch(mode) {
		case 'o':
			write_index(filename);
			break;
		case 'd':
			dump_index(filename);
			break;
		case 's':
			search_index(filename, key);
			break;
		case 'w':
			searchwild_index(filename, key);
			break;
	}
	
	return 0;
}
