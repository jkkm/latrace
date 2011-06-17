#!/bin/bash

. $PWD/test/script/functions.sh

function test_common
{
	cat > /tmp/tc_latrace.conf <<EOF
void test_common_args_ptrlen(char *ptr);
EOF

	LD_LIBRARY_PATH=$PWD ./latrace -a /tmp/tc_latrace.conf -qR ./test-common > /dev/null
	CHECK_RET

	rm -f /tmp/tc_latrace.conf
}
