#!/bin/bash

. $PWD/test/script/functions.sh

function test_args
{
	LD_LIBRARY_PATH=$PWD ./latrace -qR -a $PWD/test/lib-test-args.conf ./test-args > /dev/null
	CHECK_RET
}
