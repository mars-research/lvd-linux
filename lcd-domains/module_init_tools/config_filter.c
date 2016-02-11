/* config_filter.c: handle hidden or non-configuration files in config dirs.

   These functions are called whenever we will parse configuration files to
   ensure we are not picking up backups, SCM meta data, package artifacts.
   Eventually all config files will have to follow a convention so these
   lists of possible exceptions should not have to grow by very much.
*/

#include <string.h>

#include "util.h"
#include "config_filter.h"

int config_filter(const char *name)
{
	const char *const *p;

	static const char *const skip_prefix[] = {
		".",
		"~",
		"CVS",
		NULL
	};

	static const char *const skip_suffix[] = {
		".rpmsave",
		".rpmorig",
		".rpmnew",
		".dpkg-old",
		".dpkg-dist",
		".dpkg-new",
		".dpkg-bak",
		".bak",
		".orig",
		".rej",
		".YaST2save",
		".-",
		"~",
		",v",
		NULL
	};

	for (p = skip_prefix; *p; p++) {
		if (strstarts(name, *p))
			return 0;
	}

	for (p = skip_suffix; *p; p++) {
		if (strlen(name) >= strlen(*p) &&
		    streq(*p, strchr(name, 0) - strlen(*p)))
		    return 0;
 	}

	return 1;
}

