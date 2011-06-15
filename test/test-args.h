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

#endif /* TEST_ARGS_H */
