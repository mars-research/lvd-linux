#! /bin/sh

mkdir -p tests/tmp/proc

# Old style
echo 'foo_bar 100 0' > tests/tmp/proc/modules

[ "`rmmod foo_bar`" = "DELETE_MODULE: foo_bar EXCL NONBLOCK " ]

[ "`rmmod /lib/modules/$MODTEST_UNAME/kernel/foo_bar.o`" = "DELETE_MODULE: foo_bar EXCL NONBLOCK " ]

[ "`rmmod /lib/modules/$MODTEST_UNAME/kernel/foo_bar.ko`" = "DELETE_MODULE: foo_bar EXCL NONBLOCK " ]

[ "`rmmod /lib/modules/$MODTEST_UNAME/kernel/foo-bar.ko`" = "DELETE_MODULE: foo_bar EXCL NONBLOCK " ]

[ "`rmmod foo-bar`" = "DELETE_MODULE: foo_bar EXCL NONBLOCK " ]
