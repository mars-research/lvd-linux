#! /bin/bash -e

make libcap.microkernel $@
make libfipc.microkernel $@
make libasync.microkernel $@
make microkernel $@

make libcap.liblcd $@
make libfipc.liblcd $@
make libasync.liblcd $@
make liblcd $@

make libcap.libdeker $@
make libfipc.libdeker $@
make libasync.libdeker $@
make liblcd_for_deker $@
make libdeker

make test_mods_deker $@
