#! /bin/bash -e

make libcap.microkernel $@
make libfipc.microkernel $@
make libasync.microkernel $@
make microkernel $@
make libcap.liblcd $@
make libfipc.liblcd $@
make libasync.liblcd $@
make liblcd $@
make test_mods $@
