#!/usr/bin/env bash

ROOT_DIR=.
BASE_DIR=$ROOT_DIR
CONT_BASE=$ROOT_DIR/scripts/continuous
PID_FILE=$BASE_DIR/qemu_bg.pid
CONT_RUN=$CONT_BASE/run.sh
OUTPUT_FILE=./cont.out
EXPECT_TESTS_BASE=$ROOT_DIR/scripts/expect

final_ret=0
check_post_exit() {
	ret=$?
	if [ 0 -ne $ret ]; then
		echo - - - - - - - - - - - - - - -
		echo ERROR: $1
		echo - - - - - - - - - - - - - - -

		final_ret=$ret
	else
		echo $2
	fi
}

$CONT_RUN generic/qemu_bg "" $PID_FILE

expect $EXPECT_TESTS_BASE/framework/run_all.exp \
	$EXPECT_TESTS_BASE/x86_pjsip_tests.config 10.0.2.16 10.0.2.10 ""

check_post_exit "PJSIP tests FAILED" "PJSIP tests PASSED"

$CONT_RUN generic/qemu_bg_kill "" $PID_FILE
rm $PID_FILE

exit $final_ret
