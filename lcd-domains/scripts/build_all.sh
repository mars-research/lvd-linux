#! /bin/bash -e

make libcap.microkernel $@
make microkernel $@
make libcap.liblcd $@
make liblcd $@
make test_mods $@
