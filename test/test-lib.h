#ifndef TEST_LIB
#define TEST_LIB

#include <config.h>
#include <stdlib.h>

struct lt_config_shared *config_init(void);
void config_clear(struct lt_config_shared *sh);
int fout_read(struct lt_config_shared *sh, char *buf, int size);

enum {
	RE_TEST_TYPE_STR = 1,
	RE_TEST_TYPE_INT = 2,
	RE_TEST_TYPE_PID = 3,
};

enum {
	RE_TEST_OFF_UNDEF = -1,
};

enum {
	RE_TEST_OK = -1,
};

struct re_test_data {
	int type;
	int so;
	int eo;
	char *pat;
};

int re_test(char *line, struct re_test_data *data, int cnt);

#define TEST_START() sh->disabled = 0
#define TEST_STOP()  sh->disabled = 1

#define PASSED() \
do { \
	printf("."); \
	fflush(NULL); \
} while(0)

#define FAILED(fmt, args...) \
do { \
	char lpbuf[1024]; \
	sprintf(lpbuf, "%s:%d failed - %s", \
		__FUNCTION__, \
		__LINE__, \
		fmt); \
	printf(lpbuf, ## args); \
	fflush(NULL); \
	exit(-1); \
} while(0)

#define TEST(testfn) \
do { \
	if (testfn(sh)) { \
		printf("FAILED %s\n", #testfn); \
		return -1; \
	} \
} while(0)

#define BUFSIZE 1000

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

#endif
