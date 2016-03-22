#! /bin/bash -e

make libcap.microkernel $@
make libfipc.microkernel $@
make libasync.microkernel $@
make microkernel $@
