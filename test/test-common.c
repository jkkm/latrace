
#include <stdio.h>
#include "test/test-lib.h"
#include <sys/types.h>

extern void test_common_f1(void);
extern void test_common_args_ptrlen(char *ptr);

#define BUFSIZE 1000
#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

static int test_function(struct lt_config_shared *sh)
{
	char buf[BUFSIZE];
	int ret;
	struct re_test_data data[] = {
		{ RE_TEST_TYPE_PID,  1, -1, "[0-9]+" },
		{ RE_TEST_TYPE_STR, -1, -1, "test_common_f1" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\[.*lib-test-common.so\\]" }
	};

	config_clear(sh);

	TEST_START();
	test_common_f1();
	TEST_STOP();

	ret = fout_read(sh, buf, BUFSIZE);
	if (!ret)
		return -1;

	ret = re_test(buf, data, DATA_CNT());
	if (RE_TEST_OK != ret)
		FAILED("test %i, pattern '%s'\n", ret, data[ret].pat);

	PASSED();
	return 0;
}

static int test_args_ptrlen(struct lt_config_shared *sh)
{
	char buf[BUFSIZE];
	int ret;
	struct re_test_data data[] = {
		{ RE_TEST_TYPE_PID,  1, -1, "[0-9]+" },
		{ RE_TEST_TYPE_STR, -1, -1, " test_common_args_ptrlen\\(ptr = \\(0x[0-9a-z]+, 5\\) \"krava\"\\)" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\[.*lib-test-common.so\\]" }
	};

	config_clear(sh);
	sh->args_string_pointer_length = 1;
	sh->args_enabled = 1;

	TEST_START();
	test_common_args_ptrlen("krava");
	TEST_STOP();

	ret = fout_read(sh, buf, BUFSIZE);
	if (!ret)
		return -1;

	ret = re_test(buf, data, DATA_CNT());
	if (RE_TEST_OK != ret)
		FAILED("test %i, pattern '%s'\n", ret, data[ret].pat);

	PASSED();
	return 0;
}

int main(int argc, char **argv)
{
	struct lt_config_shared *sh;

	sh = config_init();
	if (!sh)
		return -1;

	TEST(test_function);
	TEST(test_args_ptrlen);
	return 0;
}
