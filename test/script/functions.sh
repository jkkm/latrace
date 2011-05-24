#!/bin/bash

function CHECK_RET
{
	if [ $? -ne 0 ]; then
		echo "FAILED"
	else
		echo -n "."
	fi
}

function TEST
{
	func=$1

	source $PWD/test/script/$1.sh
	echo -n "$1"
	eval $1
	echo OK
}
