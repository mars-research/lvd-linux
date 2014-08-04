/* config_filter.h: handle hidden or non-configuration files in config dirs.

   These functions are called whenever we will parse configuration files to
   ensure we are not picking up backups, SCM meta data, package artifacts.
   Eventually all config files will have to follow a convention so these
   lists of possible exceptions should not have to grow by very much.
*/
#ifndef _MODINITTOOLS_CONFIG_FILTER_H
#define _MODINITTOOLS_CONFIG_FILTER_H

/* returns 0 if the name matches a well-known backup pattern */
int config_filter(const char *name);

#endif
