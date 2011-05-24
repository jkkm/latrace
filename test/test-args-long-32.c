
#include <test/test-args.h>

#include <test/test-args.h>

int test_long(struct lt_config_shared *sh)
{
	struct re_test_data data1[] = {
		{ RE_TEST_TYPE_STR,  0, 27, "test_long_1\\(a = 1294967294\\)" },
		{ RE_TEST_TYPE_STR, 28, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_long_1 = -1294967294" },
	};
	struct re_test_data data2[] = {
		{ RE_TEST_TYPE_STR,  0, 39, "test_long_2\\(a = 123410, b = -268435455\\)" },
		{ RE_TEST_TYPE_STR, 40, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_long_2 = -536747502" },
	};
	struct re_test_data data3[] = {
		{ RE_TEST_TYPE_STR,  0, 38, "test_long_3\\(a = 1, b = -2, c = 234217\\)" },
		{ RE_TEST_TYPE_STR, 39, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_long_3 = -468434" },
	};
	struct re_test_data data4[] = {
		{ RE_TEST_TYPE_STR,  0, 55, "test_long_4\\(a = 2025479151, b = 2, c = 9119999, d = -1\\)" },
		{ RE_TEST_TYPE_STR, 56, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_long_4 = 1786259934" },
	};
	struct re_test_data data5[] = {
		{ RE_TEST_TYPE_STR,  0, 63, "test_long_5\\(a = -1, b = 2147483647, c = 13, d = 100, e = 34121\\)" },
		{ RE_TEST_TYPE_STR, 64, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_long_5 = 44357300" },
	};
	struct re_test_data data6[] = {
		{ RE_TEST_TYPE_STR,  0, 79, "test_long_6\\(a = 100, b = 102143210, c = -345436543, d = 12, e = -45, f = -1324\\)" },
		{ RE_TEST_TYPE_STR, 80, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_long_6 = 111854720" },
	};
	struct re_test_data data7[] = {
		{ RE_TEST_TYPE_STR,  0, 71, "test_long_7\\(a = 1, b = 24321, c = -3, d = 4, e = 432145, f = 6, g = 27\\)" },
		{ RE_TEST_TYPE_STR, 72, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_long_7 = -666544408" },
	};
	struct re_test_data data8[] = {
		{ RE_TEST_TYPE_STR,  0, 96, "test_long_8\\(a = -11111, b = 214321543, c = 30, d = -4, e = 51432123, f = -123, g = 7000, h = 76\\)" },
		{ RE_TEST_TYPE_STR, 97, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_long_8 = 0" },
	};
	struct re_test_data data9[] = {
		{ RE_TEST_TYPE_STR,  0, 93, "test_long_9\\(a = -10, b = 1, c = 3, d = 12424234, e = 9, f = 3, g = 14321311, h = -99, i = 10\\)" },
		{ RE_TEST_TYPE_STR, 94, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_long_9 = 1488062792" },
	};
	#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

	CONFIG_CLEAR_ARGS(sh);

	LOCAL_TEST(data1, DATA_CNT(1), test_long_1(1294967294));
	LOCAL_TEST(data2, DATA_CNT(2), test_long_2(123410, -268435455));
	LOCAL_TEST(data3, DATA_CNT(3), test_long_3(1, -2, 234217));
	LOCAL_TEST(data4, DATA_CNT(4), test_long_4(2025479151, 2, 9119999, -1));
	LOCAL_TEST(data5, DATA_CNT(5), test_long_5(-1, 2147483647, 13, 100, 34121));
	LOCAL_TEST(data6, DATA_CNT(6), test_long_6(100, 102143210, -345436543, 12, -45, -1324));
	LOCAL_TEST(data7, DATA_CNT(7), test_long_7(1, 24321, -3, 4, 432145, 6, 27));
	LOCAL_TEST(data8, DATA_CNT(8), test_long_8(-11111, 214321543, 30, -4, 51432123, -123, 7000, 76));
	LOCAL_TEST(data9, DATA_CNT(9), test_long_9(-10, 1, 3, 12424234, 9, 3, 14321311, -99, 10));

	PASSED();
	return 0;
}
