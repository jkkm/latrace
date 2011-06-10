/*
  Copyright (C) 2008, 2009, 2010 Jiri Olsa <olsajiri@gmail.com>

  This file is part of the latrace.

  The latrace is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The latrace is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the latrace (file COPYING).  If not, see 
  <http://www.gnu.org/licenses/>.
*/


#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <search.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <stdint.h>
#include <unistd.h>

#include "audit.h"
#include "list.h"

#ifdef CONFIG_ARCH_HAVE_ARGS
#include "args.h"
#endif

#ifdef __GNUC__
#define NORETURN __attribute__((__noreturn__))
#else
#define NORETURN
#ifndef __attribute__
#define __attribute__(x)
#endif
#endif

#define LT_NAMES_MAX  50
#define LT_NAMES_SEP  ','

#define LT_SYM_HMAX    1000

#define LT_ARGS_DEF_STRUCT_NUM  1000
#define LT_ARGS_DEF_TYPEDEF_NUM 1000
#define LT_ARGS_DEF_ENUM_NUM    1000


enum { 
	LT_CSORT_TIME = 0,
	LT_CSORT_PERCENT, 
	LT_CSORT_CALL, 
	LT_CSORT_UCALL, 
	LT_CSORT_LIB,
	LT_CSORT_SYM
};

struct lt_config_tv {
	int type;
	char *name;
};

enum {
	LT_OPT_HEADERS = 1,
	LT_OPT_PIPE,
	LT_OPT_INDENT_SYM,
	LT_OPT_TIMESTAMP,
	LT_OPT_FRAMESIZE,
	LT_OPT_FRAMESIZE_CHECK,
	LT_OPT_HIDE_TID,
	LT_OPT_FOLLOW_FORK,
	LT_OPT_FOLLOW_EXEC,
	LT_OPT_DEMANGLE,
	LT_OPT_BRACES,
	LT_OPT_ENABLE_ARGS,
	LT_OPT_DETAIL_ARGS,
	LT_OPT_OUTPUT_TTY,
	LT_OPT_LIBS,
	LT_OPT_LIBS_TO,
	LT_OPT_LIBS_FROM,
	LT_OPT_SYM,
	LT_OPT_SYM_OMIT,
	LT_OPT_SYM_BELOW,
	LT_OPT_SYM_NOEXIT,
	LT_OPT_ARGS_STRING_POINTER_LENGTH,
};

struct lt_config_opt {
	int idx;
	char *sval;
	unsigned long nval;
	struct lt_list_head list;
};

struct lt_config_shared {
#define LT_CONFIG_VERSION	1
#define LT_CONFIG_MAGIC		((LT_CONFIG_VERSION << 16) + 0xdead)
	unsigned int magic;

#define LT_LIBS_MAXSIZE     200
	char libs_to[LT_LIBS_MAXSIZE];
	char libs_both[LT_LIBS_MAXSIZE];
	char libs_from[LT_LIBS_MAXSIZE];

	char libs_subst[LT_LIBS_MAXSIZE];

#define LT_SYMBOLS_MAXSIZE  200
	char symbols[LT_SYMBOLS_MAXSIZE];
	char symbols_omit[LT_SYMBOLS_MAXSIZE];
	char symbols_noexit[LT_SYMBOLS_MAXSIZE];

	char flow_below[LT_SYMBOLS_MAXSIZE];

#define LT_MAXFILE 200
	char output[LT_MAXFILE];
	FILE *fout;

	char args_def[LT_MAXFILE];
	char args_enabled;
	char args_detailed;
	char args_string_pointer_length;
#define LR_ARGS_MAXLEN 1000
	int  args_maxlen;
#define LR_ARGS_DETAIL_MAXLEN 1000
	int  args_detail_maxlen;
#define LT_ARGS_TAB 10000
	struct hsearch_data args_tab;

	int disabled;
	int ctl_config;
	int verbose;
	int timestamp;
	int debug;
	int indent_sym;
	int indent_size;
	int braces;
	int demangle;
	int counts;
	int pipe;
	int hide_tid;
	int not_follow_exec;
	int not_follow_fork;
	int framesize_check;
	unsigned int framesize;
	int global_symbols;

	/* for 'not_follow_fork' */
	pid_t pid;

	/* XXX feel like an idiot.. find another way!!! */
	struct lt_config_shared *sh;
};

struct lt_config_app {
	/*
	 * This is to copy the lt_config_audit, so we can use
	 * one PRINT_VERBOSE only.
	 */
	struct lt_config_shared *sh;
	struct lt_config_shared sh_storage;

	char *prog;
#define LT_NUM_ARG 500
	char *arg[LT_NUM_ARG];
	int arg_num;

	int csort;

	int  output_tty;
	int  output_tty_fd;
	char output_tty_file[LT_MAXFILE];

	struct lt_thread *threads;
	struct lt_thread *iter;
};

struct lt_config_ctl {
	struct lt_config_shared *sh;
	char *config;

	int set_disabled;
	int disabled;
};

enum {
	LT_OS_PATH = 1,  /* '=' */
	LT_OS_PTN,       /* '%' */
	LT_OS_PTN2PATH,  /* '~' */
};

struct lt_objsearch {
	int   type;
	char *src;
	char *dst;
};

struct lt_config_audit {

	/*
	 * Normally sh points to the sh_storage. When using
	 * ctl-config feature, the shared config is stored
	 * in mmaped area.
        */
	struct lt_config_shared *sh;
	struct lt_config_shared sh_storage;

	char *libs_to[LT_NAMES_MAX];
	int libs_to_cnt;

	char *libs_from[LT_NAMES_MAX];
	int libs_from_cnt;

	char *libs_both[LT_NAMES_MAX];
	int libs_both_cnt;

	char *symbols[LT_NAMES_MAX];
	int symbols_cnt;

	char *symbols_omit[LT_NAMES_MAX];
	int symbols_omit_cnt;

	char *symbols_noexit[LT_NAMES_MAX];
	int symbols_noexit_cnt;

	char *flow_below[LT_NAMES_MAX];
	int flow_below_cnt;

	struct lt_objsearch subst[LT_NAMES_MAX];
	int subst_cnt;

	char *dir;
	int init_ok;
};

/* config - list name support */
struct lt_config_ln {
	char *name;
	struct lt_list_head list;
};

#define lt_sh(cfg, field) ((cfg)->sh->field)

#define FIFO_MSG_MAXLEN       2000

/* common message data */
struct lt_fifo_mbase {
#define FIFO_MSG_TYPE_ENTRY   1
#define FIFO_MSG_TYPE_EXIT    2
	uint32_t type;
	struct timeval tv;
	pid_t tid;
	int len; /* the rest of the message size */
};

/* symbol message */
struct lt_fifo_msym {
	struct lt_fifo_mbase h;

	int sym;
	int lib;
	int arg;
	int argd;
	char data[0];
};

struct lt_stats_sym {
        char *name;
        char *sym;
        char *lib;

        struct timeval tv_cur;
        struct timeval tv_all;

        unsigned int call;

        /* post mortem statistics */
        float percent;
        unsigned int usec_call;
};

struct lt_thread {
	/* global */
	int fifo_fd;
        pid_t tid;

	int indent_depth;

	/* start/stop time */
	struct timeval tv_start;
	struct timeval tv_stop;

	/* symbol statistics */
        struct lt_stats_sym **sym_array;
        struct hsearch_data sym_htab;
        unsigned int sym_cnt;
        unsigned int sym_max;

	struct lt_thread *next;
};

struct lt_symbol {
	struct lt_args_sym *args;

	/* symbol name */
	const char *name;
	/* symbol address */
	void *ptr;
};

/* ctl */
int main_ctl(int argc, char **argv);

/* global */
int lt_config(struct lt_config_app *cfg, int argc, char **argv);
int lt_run(struct lt_config_app *cfg);

/* fifo */
int lt_fifo_create(struct lt_config_audit *cfg, char *dir);
int lt_fifo_open(struct lt_config_app *cfg, char *dir, char *name);
int lt_fifo_notify_fd(struct lt_config_app *cfg, char *dir);
int lt_fifo_send(struct lt_config_audit *cfg, int fd, char *buf, int len);
int lt_fifo_recv(struct lt_config_app *cfg, struct lt_thread *t, 
		void *buf, int bufsize);
int lt_fifo_msym_get(struct lt_config_audit *cfg, char *buf, int type,
			struct timeval *tv, char *symname, char *libto,
			char *arg, char *argd);

/* counts */
int lt_stats_init(struct lt_config_app *cfg);
int lt_stats_sym(struct lt_config_app *cfg, struct lt_thread *t, 
		struct lt_fifo_msym* m);
int lt_stats_alloc(struct lt_config_app *cfg, struct lt_thread *t);
int lt_stats_show(struct lt_config_app *cfg);

/* thread */
struct lt_thread *lt_thread_add(struct lt_config_app *cfg, int fd, pid_t pid);
struct lt_thread *lt_thread_first(struct lt_config_app *cfg);
struct lt_thread *lt_thread_next(struct lt_config_app *cfg);

/* output */
int lt_out_entry(struct lt_config_shared *cfg, struct timeval *tv,
		pid_t tid, int indent_depth,
		const char *symname, char *lib_to,
		char *argbuf, char *argdbuf);
int lt_out_exit(struct lt_config_shared *cfg, struct timeval *tv,
		pid_t tid, int indent_depth,
		const char *symname, char *lib_to,
		char *argbuf, char *argdbuf);

/* la_objsearch */
int lt_objsearch_init(struct lt_config_audit *cfg, char **ptr, int cnt);
char* lt_objsearch(struct lt_config_audit *cfg, const char *name, 
		uintptr_t *cookie, unsigned int flag);

/* stack */
int lt_stack_framesize(struct lt_config_audit *cfg, La_regs *regs);

/* symbol */
struct lt_symbol* lt_symbol_bind(struct lt_config_shared *cfg,
				void *ptr, const char *name);
struct lt_symbol* lt_symbol_get(struct lt_config_shared *cfg,
				void *ptr, const char *name);

/* config options */
struct lt_config_opt *lt_config_opt_new(struct lt_config_app *cfg,
					int idx, char *sval, long nval);
int lt_config_opt_process(struct lt_config_app *cfg, struct lt_list_head *list);
int lt_config_ln_add(struct lt_list_head *head, char *name);
int lt_config_ln_fill(struct lt_list_head *head, char *buf, int size);

/* tty */
int tty_master(struct lt_config_app *cfg);
int tty_init(struct lt_config_app *cfg, int master);
int tty_restore(struct lt_config_app *cfg);
int tty_process(struct lt_config_app *cfg, int master);
void tty_close(struct lt_config_app *cfg);

#define PRINT(fmt, args...) \
do { \
	char lpbuf[1024]; \
	sprintf(lpbuf, "[%d %s:%05d] %s", \
		(pid_t) syscall(SYS_gettid), \
		__FUNCTION__, \
		__LINE__, \
		fmt); \
	printf(lpbuf, ## args); \
	fflush(NULL); \
} while(0)

#define PRINT_VERBOSE(cfg, cond, fmt, args...) \
do { \
	if (cond > (cfg)->sh->verbose) \
		break; \
	PRINT(fmt, ## args); \
} while(0)


#if defined(__x86_64)
#include "sysdeps/x86_64/args.h"
#endif

#endif // !CONFIG_H
