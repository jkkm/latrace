#!/bin/bash

# Force locale to ensure we always get the same output
export LC_ALL=C

. $PWD/test/script/functions.sh

TEST test_args
TEST test_common
TEST test_tty_output
TEST test_kill
TEST test_libsym
