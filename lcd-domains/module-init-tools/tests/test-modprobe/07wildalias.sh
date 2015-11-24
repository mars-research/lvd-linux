#! /bin/sh
# Test wildcard aliases.

rm -rf tests/tmp/*

# Create inputs
MODULE_DIR=tests/tmp/lib/modules/$MODTEST_UNAME
mkdir -p $MODULE_DIR
touch $MODULE_DIR/modules.dep

# Create a simple config file.
mkdir -p tests/tmp/etc/modprobe.d
cat > tests/tmp/etc/modprobe.d/modprobe.conf <<EOF
# Various aliases
alias *wildcard-_* foo
alias /dev/test* bar
EOF

# Simple test.
[ "`modprobe wildcard-_aaa 2>&1`" = "FATAL: Module foo not found." ]
[ "`modprobe wildcard-_ 2>&1`" = "FATAL: Module foo not found." ]
[ "`modprobe wildcard_- 2>&1`" = "FATAL: Module foo not found." ]
[ "`modprobe anotherwildcard-_ 2>&1`" = "FATAL: Module foo not found." ]
[ "`modprobe anotherwildcard-_aaa 2>&1`" = "FATAL: Module foo not found." ]
[ "`modprobe /dev/test 2>&1`" = "FATAL: Module bar not found." ]
[ "`modprobe /dev/test/tmp 2>&1`" = "FATAL: Module bar not found." ]
[ "`modprobe /dev/test7  2>&1`" = "FATAL: Module bar not found." ]
