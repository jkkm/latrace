#!/bin/bash

# Force locale to ensure we always get the same output
export LC_ALL=C

# common tests
echo "[APP test-common]"
LD_LIBRARY_PATH=$PWD ./latrace -qR ./test-common

# arguments tests
echo "[APP test-args]"
LD_LIBRARY_PATH=$PWD ./latrace -qR -a $PWD/test/lib-test-args.conf ./test-args

# script tests
echo "[SCRIPTS]"
. $PWD/test/script/functions.sh

TEST test_tty_output
TEST test_kill
