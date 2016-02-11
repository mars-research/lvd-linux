#! /bin/sh

# Make sure it finds the real one this time.
PATH=`pwd`/tests/build:$PATH
valgrind -q --num-callers=8 --leak-check=yes --suppressions=tests/valgrind/suppressions --log-fd=3 `basename $0` "$@" 3>tests/tmp/valgrind
ret=$?

if [ -s tests/tmp/valgrind ]; then
    echo VALGRIND FAILED for $0 "$@" > /proc/$PPID/fd/1
    cat tests/tmp/valgrind > /proc/$PPID/fd/1
    kill $PPID
    exit 1
fi
exit $ret
