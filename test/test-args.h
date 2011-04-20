#ifndef TEST_ARGS_H
#define TEST_ARGS_H

#include <config.h>

#include <test/test-lib.h>
#include <test/lib-test-args.h>

#define CONFIG_CLEAR_ARGS(sh) \
do { \
	config_clear(sh); \
	sh->args_enabled = 1; \
	sh->hide_tid = 1; \
} while(0)

#define LOCAL_TEST(data, cnt, test) \
do { \
	char buf[BUFSIZE]; \
	int ret; \
	TEST_START(); \
	test; \
	TEST_STOP(); \
	ret = fout_read(sh, buf, BUFSIZE); \
	if (!ret) \
		return -1; \
	ret = re_test(buf, data, cnt); \
	if (RE_TEST_OK != ret) \
		FAILED("test %i, pattern '%s'\n", ret, data[ret].pat); \
} while(0)

#endif /* TEST_ARGS_H */
