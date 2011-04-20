#!/bin/sh

# common tests
LD_LIBRARY_PATH=$PWD ./latrace -q ./test-common

# arguments tests
LD_LIBRARY_PATH=$PWD ./latrace -q -a $PWD/test/lib-test-args.conf ./test-args
