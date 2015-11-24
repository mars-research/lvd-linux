#! /bin/sh

mkdir -p tests/tmp/proc
touch tests/tmp/proc/modules

# This should be true
[ "`lsmod`" = "Module                  Size  Used by" ]
