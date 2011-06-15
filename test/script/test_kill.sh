#!/bin/bash

. $PWD/test/script/functions.sh

function tk_init
{
	arg=$1

	if [ x$arg == xnopipe ]; then
		cat > /tmp/tk_latrace.conf <<EOF
OPTIONS {
	PIPE=NO
}
EOF
	else
		echo > /tmp/tk_latrace.conf
	fi
}

function tk_cleanup
{
	rm -f /tmp/tk_latrace.conf
	rm -f test-kill.expect
	rm -f test-kill.out
}


function tk_wait_start
{
	name=$1

	while [ -z `pgrep $name` ];
	do
		usleep 100
	done
}

function tk_wait_stop
{
	pid=$1

	while [ `kill -0 $pid 2> /dev/null; echo $?` == "0" ];
	do
		usleep 100
	done
}

function test_kill_handlers
{
	tk_init $1
	./latrace -N /tmp/tk_latrace.conf -q $PWD/test-kill 1 > test-kill.out &

	tk_wait_start test-kill

	pid_latrace=$!
	pid_test=`pgrep test-kill`

	kill -2 $pid_latrace

	tk_wait_stop $pid_latrace
	tk_wait_stop $pid_test

	echo -e "\nlatrace interrupted, killing child (pid $pid_test)" > test-kill.expect
	echo -e "\n$PWD/test-kill finished - exited, status=0" >> test-kill.expect

	diff test-kill.out test-kill.expect
	CHECK_RET
}

function test_kill_nohandlers
{
	tk_init $1
	./latrace -N /tmp/tk_latrace.conf -q $PWD/test-kill 2 > test-kill.out &

	tk_wait_start test-kill

	pid_latrace=$!
	pid_test=`pgrep test-kill`

	kill -2 $pid_latrace

	tk_wait_stop $pid_latrace
	tk_wait_stop $pid_test

	echo -e "\nlatrace interrupted, killing child (pid $pid_test)" > test-kill.expect
	echo -e "\n$PWD/test-kill finished - killed by signal 15" >> test-kill.expect

	diff test-kill.out test-kill.expect
	if [ $? -ne 0 ]; then
		echo "FAILED test_kill"
		read
		exit
	fi

	echo -n .
}


function test_kill_blocked
{
	tk_init $1
	./latrace -N /tmp/tk_latrace.conf -q $PWD/test-kill 3 > test-kill.out &

	tk_wait_start test-kill

	pid_latrace=$!
	pid_test=`pgrep test-kill`

	kill -2 $pid_latrace

	tk_wait_stop $pid_latrace
	tk_wait_stop $pid_test

	echo -e "\nlatrace interrupted, killing child (pid $pid_test)" > test-kill.expect
	echo -e "\n$PWD/test-kill finished - killed by signal 9" >> test-kill.expect

	diff test-kill.out test-kill.expect
	CHECK_RET
}

function test_kill
{
	test_kill_handlers
	test_kill_handlers nopipe

	test_kill_nohandlers
	test_kill_nohandlers nopipe

	test_kill_blocked
	test_kill_blocked nopipe

	tk_cleanup
}
