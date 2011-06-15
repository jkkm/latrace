#!/bin/bash

. $PWD/test/script/functions.sh

function test_common
{
	LD_LIBRARY_PATH=$PWD ./latrace -qR ./test-common > /dev/null
	CHECK_RET
}
