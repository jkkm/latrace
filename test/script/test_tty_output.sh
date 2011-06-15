#!/bin/bash

. $PWD/test/script/functions.sh

function tto_init
{
	cat > /tmp/tto_latrace.conf <<EOF
OPTIONS {
        OUTPUT_TTY = "/tmp/tto_latrace.output"
}
EOF
}

function tto_cleanup
{
	rm -f /tmp/tto_latrace.conf
	rm -f /tmp/tto_latrace.output
}

function test_tty_output
{
	tto_init

	LD_LIBRARY_PATH=$PWD ./latrace -N /tmp/tto_latrace.conf -q /bin/cat \
		$PWD/test/script/test_tty_output.sh > /dev/null

	diff /tmp/tto_latrace.output $PWD/test/script/test_tty_output.sh
	CHECK_RET

	tto_cleanup
}
