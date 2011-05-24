
#include <stdio.h>
#include <sys/types.h>

#include <test/test-args.h>

static int test_char(struct lt_config_shared *sh)
{
	struct re_test_data data1[] = {
		{ RE_TEST_TYPE_STR,  0, 21, "test_char_1\\(a = 0x0a\\)" },
		{ RE_TEST_TYPE_STR, 22, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_char_1 = 0x64 'd'" },
	};
	struct re_test_data data2[] = {
		{ RE_TEST_TYPE_STR,  0, 31, "test_char_2\\(a = 0x0a, b = 0x14\\)" },
		{ RE_TEST_TYPE_STR, 32, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_char_2 = 0xf6" },
	};
	struct re_test_data data3[] = {
		{ RE_TEST_TYPE_STR,  0, 41, "test_char_3\\(a = 0x01, b = 0xfe, c = 0x07\\)" },
		{ RE_TEST_TYPE_STR, 42, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_char_3 = 0xf7" },
	};
	struct re_test_data data4[] = {
		{ RE_TEST_TYPE_STR,  0, 59, "test_char_4\\(a = 0x64 'd', b = 0x02, c = 0x63 'c', d = 0xff\\)" },
		{ RE_TEST_TYPE_STR, 60, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_char_4 = 0xc9" },
	};
	struct re_test_data data5[] = {
		{ RE_TEST_TYPE_STR,  0, 69, "test_char_5\\(a = 0xff, b = 0x15, c = 0x0d, d = 0x20 ' ', e = 0x79 'y'\\)" },
		{ RE_TEST_TYPE_STR, 70, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_char_5 = 0x02" },
	};
	struct re_test_data data6[] = {
		{ RE_TEST_TYPE_STR,  0, 75, "test_char_6\\(a = 0x64 'd', b = 0xc8, c = 0xd5, d = 0x0c, e = 0xc7, f = 0xf2\\)" },
		{ RE_TEST_TYPE_STR, 76, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_char_6 = 0x63 'c'" },
	};
	struct re_test_data data7[] = {
		{ RE_TEST_TYPE_STR,  0, 81, "test_char_7\\(a = 0x01, b = 0x02, c = 0x03, d = 0x04, e = 0x05, f = 0x06, g = 0x07\\)" },
		{ RE_TEST_TYPE_STR, 82, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_char_7 = 0x19" },
	};
	struct re_test_data data8[] = {
		{ RE_TEST_TYPE_STR,  0, 95, "test_char_8\\(a = 0xf5, b = 0x15, c = 0x1e, d = 0xfc, e = 0x17, f = 0x85, g = 0x46 'F', h = 0x06\\)" },
		{ RE_TEST_TYPE_STR, 96, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_char_8 = 0x39 '9'" },
	};
	struct re_test_data data9[] = {
		{ RE_TEST_TYPE_STR,   0, 109, "test_char_9\\(a = 0xf6, b = 0x01, c = 0x03, d = 0x04, e = 0x09, f = 0x63 'c', g = 0x4e 'N', h = 0xf7, i = 0x0c\\)" },
		{ RE_TEST_TYPE_STR, 110,  -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR,  -1,  -1, "\\} test_char_9 = 0xd0" },
	};
	#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

	CONFIG_CLEAR_ARGS(sh);

	LOCAL_TEST(data1, DATA_CNT(1), test_char_1(10));
	LOCAL_TEST(data2, DATA_CNT(2), test_char_2(10, 20));
	LOCAL_TEST(data3, DATA_CNT(3), test_char_3(1, -2, 7));
	LOCAL_TEST(data4, DATA_CNT(4), test_char_4(100, 2, 99, -1));
	LOCAL_TEST(data5, DATA_CNT(5), test_char_5(-1, 21, 13, 32, 121));
	LOCAL_TEST(data6, DATA_CNT(6), test_char_6(100, 200, -43, 12, 199, -14));
	LOCAL_TEST(data7, DATA_CNT(7), test_char_7(1, 2, 3, 4, 5, 6, 7));
	LOCAL_TEST(data8, DATA_CNT(8), test_char_8(-11, 21, 30, -4, 23, -123, 70, 6));
	LOCAL_TEST(data9, DATA_CNT(9), test_char_9(-10, 1, 3, 4, 9, 99, 78, -9, 12));

	PASSED();
	return 0;
}

static int test_short(struct lt_config_shared *sh)
{
	struct re_test_data data1[] = {
		{ RE_TEST_TYPE_STR,  0, 21, "test_short_1\\(a = 100\\)" },
		{ RE_TEST_TYPE_STR, 22, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_short_1 = -100" },
	};
	struct re_test_data data2[] = {
		{ RE_TEST_TYPE_STR,  0, 28, "test_short_2\\(a = 10, b = 20\\)" },
		{ RE_TEST_TYPE_STR, 29, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_short_2 = 30" },
	};
	struct re_test_data data3[] = {
		{ RE_TEST_TYPE_STR,  0, 34, "test_short_3\\(a = 1, b = -2, c = 7\\)" },
		{ RE_TEST_TYPE_STR, 35, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_short_3 = -8" },
	};
	struct re_test_data data4[] = {
		{ RE_TEST_TYPE_STR,  0, 45, "test_short_4\\(a = 100, b = 2, c = 999, d = -1\\)" },
		{ RE_TEST_TYPE_STR, 46, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_short_4 = 2097" },
	};
	struct re_test_data data5[] = {
		{ RE_TEST_TYPE_STR,  0, 60, "test_short_5\\(a = -1, b = 21477, c = 4313, d = 100, e = 3121\\)" },
		{ RE_TEST_TYPE_STR, 61, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_short_5 = 1583" },
	};
	struct re_test_data data6[] = {
		{ RE_TEST_TYPE_STR,  0, 70, "test_short_6\\(a = 100, b = 200, c = -3543, d = 12, e = 9999, f = -1324\\)" },
		{ RE_TEST_TYPE_STR, 71, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_short_6 = -25344" },
	};
	struct re_test_data data7[] = {
		{ RE_TEST_TYPE_STR,  0, 61, "test_short_7\\(a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7\\)" },
		{ RE_TEST_TYPE_STR, 62, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_short_7 = -19" },
	};
	struct re_test_data data8[] = {
		{ RE_TEST_TYPE_STR,  0, 88, "test_short_8\\(a = -11111, b = 2143, c = 30, d = -4, e = 5123, f = -123, g = 7000, h = 76\\)" },
		{ RE_TEST_TYPE_STR, 89, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_short_8 = 3134" },
	};
	struct re_test_data data9[] = {
		{ RE_TEST_TYPE_STR,  0, 86, "test_short_9\\(a = -10, b = 1, c = 3, d = 1234, e = 9, f = 3, g = 1311, h = -99, i = 10\\)" },
		{ RE_TEST_TYPE_STR, 87, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_short_9 = -2482" },
	};
	#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

	CONFIG_CLEAR_ARGS(sh);

	LOCAL_TEST(data1, DATA_CNT(1), test_short_1(100));
	LOCAL_TEST(data2, DATA_CNT(2), test_short_2(10, 20));
	LOCAL_TEST(data3, DATA_CNT(3), test_short_3(1, -2, 7));
	LOCAL_TEST(data4, DATA_CNT(4), test_short_4(100, 2, 999, -1));
	LOCAL_TEST(data5, DATA_CNT(5), test_short_5(-1, 21477, 4313, 100, 3121));
	LOCAL_TEST(data6, DATA_CNT(6), test_short_6(100, 200, -3543, 12, 9999, -1324));
	LOCAL_TEST(data7, DATA_CNT(7), test_short_7(1, 2, 3, 4, 5, 6, 7));
	LOCAL_TEST(data8, DATA_CNT(8), test_short_8(-11111, 2143, 30, -4, 5123, -123, 7000, 76));
	LOCAL_TEST(data9, DATA_CNT(9), test_short_9(-10, 1, 3, 1234, 9, 3, 1311, -99, 10));

	PASSED();
	return 0;
}

static int test_int(struct lt_config_shared *sh)
{
	struct re_test_data data1[] = {
		{ RE_TEST_TYPE_STR,  0, 19, "test_int_1\\(a = 100\\)" },
		{ RE_TEST_TYPE_STR, 20, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_int_1 = -100" },
	};
	struct re_test_data data2[] = {
		{ RE_TEST_TYPE_STR,  0, 26, "test_int_2\\(a = 10, b = 20\\)" },
		{ RE_TEST_TYPE_STR, 27, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_int_2 = 30" },
	};
	struct re_test_data data3[] = {
		{ RE_TEST_TYPE_STR,  0, 32, "test_int_3\\(a = 1, b = -2, c = 7\\)" },
		{ RE_TEST_TYPE_STR, 33, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_int_3 = -8" },
	};
	struct re_test_data data4[] = {
		{ RE_TEST_TYPE_STR,  0, 46, "test_int_4\\(a = 1000, b = 2, c = 99999, d = -1\\)" },
		{ RE_TEST_TYPE_STR, 47, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_int_4 = 200997" },
	};
	struct re_test_data data5[] = {
		{ RE_TEST_TYPE_STR,  0, 66, "test_int_5\\(a = -1, b = 2147483647, c = 654313, d = 100, e = 34121\\)" },
		{ RE_TEST_TYPE_STR, 67, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_int_5 = -2144725863" },
	};
	struct re_test_data data6[] = {
		{ RE_TEST_TYPE_STR,  0, 77, "test_int_6\\(a = 100, b = 200, c = -345436543, d = 12, e = 99999999, f = -1324\\)" },
		{ RE_TEST_TYPE_STR, 78, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_int_6 = -1223446720" },
	};
	struct re_test_data data7[] = {
		{ RE_TEST_TYPE_STR,  0, 59, "test_int_7\\(a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7\\)" },
		{ RE_TEST_TYPE_STR, 60, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_int_7 = -19" },
	};
	struct re_test_data data8[] = {
		{ RE_TEST_TYPE_STR,  0, 91, "test_int_8\\(a = -11111, b = 214321543, c = 30, d = -4, e = 5123, f = -123, g = 7000, h = 76\\)" },
		{ RE_TEST_TYPE_STR, 92, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_int_8 = 214322534" },
	};
	struct re_test_data data9[] = {
		{ RE_TEST_TYPE_STR,  0, 84, "test_int_9\\(a = -10, b = 1, c = 3, d = 1234, e = 9, f = 3, g = 1311, h = -99, i = 10\\)" },
		{ RE_TEST_TYPE_STR, 85, -1, "\\[.*lib-test-args.so\\] \\{" },
		{ RE_TEST_TYPE_STR, -1, -1, "\\} test_int_9 = -2482" },
	};
	#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

	CONFIG_CLEAR_ARGS(sh);

	LOCAL_TEST(data1, DATA_CNT(1), test_int_1(100));
	LOCAL_TEST(data2, DATA_CNT(2), test_int_2(10, 20));
	LOCAL_TEST(data3, DATA_CNT(3), test_int_3(1, -2, 7));
	LOCAL_TEST(data4, DATA_CNT(4), test_int_4(1000, 2, 99999, -1));
	LOCAL_TEST(data5, DATA_CNT(5), test_int_5(-1, 2147483647, 654313, 100, 34121));
	LOCAL_TEST(data6, DATA_CNT(6), test_int_6(100, 200, -345436543, 12, 99999999, -1324));
	LOCAL_TEST(data7, DATA_CNT(7), test_int_7(1, 2, 3, 4, 5, 6, 7));
	LOCAL_TEST(data8, DATA_CNT(8), test_int_8(-11111, 214321543, 30, -4, 5123, -123, 7000, 76));
	LOCAL_TEST(data9, DATA_CNT(9), test_int_9(-10, 1, 3, 1234, 9, 3, 1311, -99, 10));

	PASSED();
	return 0;
}

int main(int argc, char **argv)
{
	struct lt_config_shared *sh;

	sh = config_init();
	if (!sh)
		return -1;

	TEST(test_char);
	TEST(test_short);
	TEST(test_int);

	/*
	 * test_long is arch specific, since long differes within x86 32 and
	 * 64 bits (not sure about ARM), while char/short/int are same.
	 * */
	extern int test_long(struct lt_config_shared *sh);
	TEST(test_long);

	return 0;
}
