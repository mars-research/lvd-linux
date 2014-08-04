#! /bin/sh

mkdir -p tests/tmp/proc

# Old style
echo 'foo_bar 100 0' > tests/tmp/proc/modules

[ "`rmmod -f foo_bar`" = "DELETE_MODULE: foo_bar EXCL TRUNC NONBLOCK " ]
[ "`rmmod -f foo-bar`" = "DELETE_MODULE: foo_bar EXCL TRUNC NONBLOCK " ]

[ "`rmmod -f /lib/modules/$MODTEST_UNAME/kernel/foo_bar.o`" = "DELETE_MODULE: foo_bar EXCL TRUNC NONBLOCK " ]
[ "`rmmod -f /lib/modules/$MODTEST_UNAME/kernel/foo_bar.ko`" = "DELETE_MODULE: foo_bar EXCL TRUNC NONBLOCK " ]
[ "`rmmod -f /lib/modules/$MODTEST_UNAME/kernel/foo-bar.ko`" = "DELETE_MODULE: foo_bar EXCL TRUNC NONBLOCK " ]

[ "`rmmod -w foo_bar`" = "DELETE_MODULE: foo_bar EXCL " ]
[ "`rmmod -w foo-bar`" = "DELETE_MODULE: foo_bar EXCL " ]

[ "`rmmod -w /lib/modules/$MODTEST_UNAME/kernel/foo_bar.o`" = "DELETE_MODULE: foo_bar EXCL " ]
[ "`rmmod -w /lib/modules/$MODTEST_UNAME/kernel/foo_bar.ko`" = "DELETE_MODULE: foo_bar EXCL " ]
[ "`rmmod -w /lib/modules/$MODTEST_UNAME/kernel/foo-bar.ko`" = "DELETE_MODULE: foo_bar EXCL " ]

# Both should work on "in-use" modules.
echo 'foo_bar 100 1' > tests/tmp/proc/modules

[ "`rmmod -f foo_bar`" = "DELETE_MODULE: foo_bar EXCL TRUNC NONBLOCK " ]
[ "`rmmod -w foo_bar`" = "DELETE_MODULE: foo_bar EXCL " ]
