
#include <config.h>
#include <test/test-lib.h>
#include <string.h>

void test_libsym1_1(void);
void test_libsym1_2(void);
void test_libsym1_3(void);
void test_libsym2_1(void);
void test_libsym2_2(void);
void test_libsym2_3(void);
void test_libsym3_1(void);
void test_libsym3_2(void);
void test_libsym3_3(void);

static void test_libsym(void)
{
	test_libsym1_1();
	test_libsym1_2();
	test_libsym1_3();
	test_libsym2_1();
	test_libsym2_2();
	test_libsym2_3();
	test_libsym3_1();
	test_libsym3_2();
	test_libsym3_3();
}

/* TODO
 * so far following tests check for displayed output.. need
 * some intelligent regex to check unwanted output is really
 * not displayed */

static int test_all(struct lt_config_shared *sh)
{
	struct re_test_data data1[] = {
		{ RE_TEST_TYPE_STR,   0, -1, "test_libsym1_1 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym1_2 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym1_3 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_1 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_2 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_3 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_1 \\[[^]]+lib-test-libsym3.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_2 \\[[^]]+lib-test-libsym3.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_3 \\[[^]]+lib-test-libsym3.so\\]" },
	};
	#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

	config_clear(sh);
	sh->hide_tid = 1;

	LOCAL_TEST(data1, DATA_CNT(1), test_libsym());
	PASSED();
	return 0;
}

static int test_lib1(struct lt_config_shared *sh)
{
	struct re_test_data data1[] = {
		{ RE_TEST_TYPE_STR,   0, -1, "test_libsym1_1 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym1_2 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym1_3 \\[[^]]+lib-test-libsym1.so\\]" },
	};
	#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

	config_clear(sh);
	sh->hide_tid = 1;

	LOCAL_TEST(data1, DATA_CNT(1), test_libsym());
	PASSED();
	return 0;
}

static int test_lib2(struct lt_config_shared *sh)
{
	struct re_test_data data1[] = {
		{ RE_TEST_TYPE_STR,   0, -1, "test_libsym2_1 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_2 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_3 \\[[^]]+lib-test-libsym2.so\\]" },
	};
	#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

	config_clear(sh);
	sh->hide_tid = 1;

	LOCAL_TEST(data1, DATA_CNT(1), test_libsym());
	PASSED();
	return 0;
}

static int test_lib3(struct lt_config_shared *sh)
{
	struct re_test_data data1[] = {
		{ RE_TEST_TYPE_STR,   0, -1, "test_libsym3_1 \\[[^]]+lib-test-libsym3.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_2 \\[[^]]+lib-test-libsym3.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_3 \\[[^]]+lib-test-libsym3.so\\]" },
	};
	#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

	config_clear(sh);
	sh->hide_tid = 1;

	LOCAL_TEST(data1, DATA_CNT(1), test_libsym());
	PASSED();
	return 0;
}

static int test_lib1lib2(struct lt_config_shared *sh)
{
	struct re_test_data data1[] = {
		{ RE_TEST_TYPE_STR,   0, -1, "test_libsym1_1 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym1_2 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym1_3 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_1 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_2 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_3 \\[[^]]+lib-test-libsym2.so\\]" },
	};
	#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

	config_clear(sh);
	sh->hide_tid = 1;

	LOCAL_TEST(data1, DATA_CNT(1), test_libsym());
	PASSED();
	return 0;
}

static int test_lib2lib3(struct lt_config_shared *sh)
{
	struct re_test_data data1[] = {
		{ RE_TEST_TYPE_STR,   0, -1, "test_libsym2_1 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_2 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_3 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_1 \\[[^]]+lib-test-libsym3.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_2 \\[[^]]+lib-test-libsym3.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_3 \\[[^]]+lib-test-libsym3.so\\]" },
	};
	#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

	config_clear(sh);
	sh->hide_tid = 1;

	LOCAL_TEST(data1, DATA_CNT(1), test_libsym());
	PASSED();
	return 0;
}

static int test_lib1lib2lib3(struct lt_config_shared *sh)
{
	struct re_test_data data1[] = {
		{ RE_TEST_TYPE_STR,   0, -1, "test_libsym1_1 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym1_2 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym1_3 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_1 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_2 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_3 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_1 \\[[^]]+lib-test-libsym3.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_2 \\[[^]]+lib-test-libsym3.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_3 \\[[^]]+lib-test-libsym3.so\\]" },
	};
	#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

	config_clear(sh);
	sh->hide_tid = 1;

	LOCAL_TEST(data1, DATA_CNT(1), test_libsym());
	PASSED();
	return 0;
}

static int test_sym1(struct lt_config_shared *sh)
{
	struct re_test_data data1[] = {
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym1_1 \\[[^]]+lib-test-libsym1.so\\]" },
	};
	#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

	config_clear(sh);
	sh->hide_tid = 1;

	LOCAL_TEST(data1, DATA_CNT(1), test_libsym());
	PASSED();
	return 0;
}

static int test_sym2sym3(struct lt_config_shared *sh)
{
	struct re_test_data data1[] = {
		{ RE_TEST_TYPE_STR,   0, -1, "test_libsym2_2 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_3 \\[[^]]+lib-test-libsym3.so\\]" },
	};
	#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

	config_clear(sh);
	sh->hide_tid = 1;

	LOCAL_TEST(data1, DATA_CNT(1), test_libsym());
	PASSED();
	return 0;
}

static int test_omit_sym1(struct lt_config_shared *sh)
{
	struct re_test_data data1[] = {
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym1_2 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym1_3 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_1 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_2 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_3 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_1 \\[[^]]+lib-test-libsym3.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_2 \\[[^]]+lib-test-libsym3.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_3 \\[[^]]+lib-test-libsym3.so\\]" },
	};
	#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

	config_clear(sh);
	sh->hide_tid = 1;

	LOCAL_TEST(data1, DATA_CNT(1), test_libsym());
	PASSED();
	return 0;
}

static int test_omit_sym2sym3(struct lt_config_shared *sh)
{
	struct re_test_data data1[] = {
		{ RE_TEST_TYPE_STR,   0, -1, "test_libsym1_1 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym1_2 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym1_3 \\[[^]]+lib-test-libsym1.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_1 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym2_2 \\[[^]]+lib-test-libsym2.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_1 \\[[^]]+lib-test-libsym3.so\\]" },
		{ RE_TEST_TYPE_STR,  -1, -1, "test_libsym3_3 \\[[^]]+lib-test-libsym3.so\\]" },
	};
	#define DATA_CNT(num) (sizeof(data ## num)/sizeof(struct re_test_data))

	config_clear(sh);
	sh->hide_tid = 1;

	LOCAL_TEST(data1, DATA_CNT(1), test_libsym());
	PASSED();
	return 0;
}

int main(int argc, char **argv)
{
	struct lt_config_shared *sh;
	char *test;

	if (argc != 2)
		return -1;

	test = argv[1];

	sh = config_init();
	if (!sh)
		return -1;

	if (!strcmp(test, "all"))
		TEST(test_all);
	else if (!strcmp(test, "lib1"))
		TEST(test_lib1);
	else if (!strcmp(test, "lib2"))
		TEST(test_lib2);
	else if (!strcmp(test, "lib3"))
		TEST(test_lib3);
	else if (!strcmp(test, "lib1lib2"))
		TEST(test_lib1lib2);
	else if (!strcmp(test, "lib2lib3"))
		TEST(test_lib2lib3);
	else if (!strcmp(test, "lib1lib2lib3"))
		TEST(test_lib1lib2lib3);
	else if (!strcmp(test, "sym1"))
		TEST(test_sym1);
	else if (!strcmp(test, "sym2sym3"))
		TEST(test_sym2sym3);
	else if (!strcmp(test, "omit_sym1"))
		TEST(test_omit_sym1);
	else if (!strcmp(test, "omit_sym2sym3"))
		TEST(test_omit_sym2sym3);

	return 0;
}
