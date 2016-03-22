#! /bin/bash -e

make libcap.libdeker DEKER=1 $@
make libfipc.libdeker DEKER=1 $@
make libasync.libdeker DEKER=1 $@
make liblcd_for_deker DEKER=1 $@
make libdeker DEKER=1
