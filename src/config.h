/*
  Copyright (C) 2008, 2009 Jiri Olsa <olsajiri@gmail.com>

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
#include "autoconf.h"

#ifdef __GNUC__
#define NORETURN __attribute__((__noreturn__))
#else
#define NORETURN
#ifndef __attribute__
#define __attribute__(x)
#endif
#endif

/* TODO put this to autoconf.h */
#define LT_VERSION_MINOR	1
#define LT_VERSION_MAJOR	0
#define LT_VERSION (LT_VERSION_MAJOR*256 + LT_VERSION_MINOR)

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

struct lt_config_shared {
#define LT_MAGIC  ((LT_VERSION << 16) + 0xdead)
	unsigned int magic;

#define LT_LIBS_MAXSIZE     200
	char libs_to[LT_LIBS_MAXSIZE];
	char libs_both[LT_LIBS_MAXSIZE];
	char libs_from[LT_LIBS_MAXSIZE];

	char libs_subst[LT_LIBS_MAXSIZE];

#define LT_SYMBOLS_MAXSIZE  200
	char symbols[LT_SYMBOLS_MAXSIZE];

	char flow_below[LT_SYMBOLS_MAXSIZE];

#define LT_MAXFILE 200
	char output[LT_MAXFILE];
	FILE *fout;

	char args_def[LT_MAXFILE];
	char args_enabled;
	char args_detailed;
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
	unsigned int framesize;

	/* for 'not_follow_fork' */
	pid_t pid;

	/* used by both app and lib */
	unsigned int indent_depth;

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

	char *flow_below[LT_NAMES_MAX];
	int flow_below_cnt;

	struct lt_objsearch subst[LT_NAMES_MAX];
	int subst_cnt;

	char *dir;
	int init_ok;
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

enum {
	LT_ARGS_DTYPE_POD = 1,
	LT_ARGS_DTYPE_STRUCT, 
};

enum {
	LT_ARGS_TYPEID_VOID = 1,
	LT_ARGS_TYPEID_SHORT,
	LT_ARGS_TYPEID_USHORT,
	LT_ARGS_TYPEID_INT,
	LT_ARGS_TYPEID_UINT,
	LT_ARGS_TYPEID_LONG,
	LT_ARGS_TYPEID_ULONG,
	LT_ARGS_TYPEID_CHAR,
	LT_ARGS_TYPEID_UCHAR,
	LT_ARGS_TYPEID_LLONG,
	LT_ARGS_TYPEID_ULLONG,
	LT_ARGS_TYPEID_DOUBLE,
	LT_ARGS_TYPEID_FLOAT,

	LT_ARGS_TYPEID_CUSTOM = 1000
};

struct lt_enum_elem {
	char *name;
	char *strval;
	long val;
	int undef;
	struct lt_list_head list;
};

struct lt_enum {
	char *name;
	int cnt;
	struct lt_enum_elem *elems;
};

struct lt_arg {
	/* argument type */
	int dtype;

	/* argument type properties */
	int   type_id;
	u_int type_len;
	char  *type_name;

	/* argument value properties */
	int pointer;
	char *name;

	/* for structures only */
	int mmbcnt;

	/* architecture dependent */
	void *arch;

	/* enum record */
	struct lt_enum *en;

	/* struct arguments head */
	struct lt_list_head *args_head;
	/* nested arguments list if present */
	struct lt_list_head args_list;
};

struct lt_args_sym {
	char *name;

	int argcnt;
#define LT_ARGS_RET 0
	struct lt_arg **args;
};

struct lt_args_include {
	FILE *yyin;
	void *yybuf;
	char *file;
	int   lineno;
};

/* used in lt_args_cb_struct for argument type */
enum { 
	LT_ARGS_STRUCT_ITSELF = 0,
	LT_ARGS_STRUCT_ARG 
};

struct lt_args_data {
	int arglen;

	/* function arguments */
	char *args_buf;
	int   args_len;
	int   args_totlen;

	/* detailed structs */
	char *argsd_buf;
	int   argsd_len;
	int   argsd_totlen;
};

/* ctl */
int main_ctl(int argc, char **argv);

/* global */
int lt_config(struct lt_config_app *cfg, int argc, char **argv);
int lt_run(struct lt_config_app *cfg);

/* fifo */
int lt_fifo_create(struct lt_config_audit *cfg, char *dir);
int lt_fifo_open(struct lt_config_app *cfg, char *name);
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

/* arguments */
int lt_args_init(struct lt_config_shared *cfg);
int lt_args_sym_entry(struct lt_config_shared *cfg, char *sym, La_regs *regs, 
			char **argbuf, char **argdbuf);
int lt_args_sym_exit(struct lt_config_shared *cfg, char *sym, La_regs *inregs,
			La_retval *outregs, char **argbuf, char **argdbuf);
int lt_args_add_enum(struct lt_config_shared *cfg, char *name, 
			struct lt_list_head *h);
struct lt_enum_elem* lt_args_get_enum(struct lt_config_shared *cfg, char *name, char *val);
int lt_args_add_struct(struct lt_config_shared *cfg, char *type_name, 
			struct lt_list_head *h);
int lt_args_add_sym(struct lt_config_shared *cfg, struct lt_arg *sym, 
			struct lt_list_head *h);
int lt_args_add_typedef(struct lt_config_shared *cfg, char *base, 
	char *new, int pointer);
int lt_args_buf_open(struct lt_config_shared *cfg, char *file);
int lt_args_buf_close(struct lt_config_shared *cfg);
struct lt_arg* lt_args_getarg(struct lt_config_shared *cfg, char *type,
				char *name, int pointer, int create, char *enum_name);
int lt_args_cb_arg(struct lt_config_shared *cfg, struct lt_arg *arg,
			void *pval, struct lt_args_data *data, int last, 
			int dspname);
int lt_args_cb_struct(struct lt_config_shared *cfg, int type, 
			struct lt_arg *arg, void *pval, 
			struct lt_args_data *data, int last);

/* output */
int lt_out_entry(struct lt_config_shared *cfg, struct timeval *tv,
		const char *symname, char *lib_to,
		char *argbuf, char *argdbuf);
int lt_out_exit(struct lt_config_shared *cfg, struct timeval *tv,
		const char *symname, char *lib_to,
		char *argbuf, char *argdbuf);

/* stack handling */
int lt_stack_process(struct lt_config_shared *cfg, struct lt_args_sym *asym, 
			La_regs *regs, struct lt_args_data *data);
int lt_stack_process_ret(struct lt_config_shared *cfg, struct lt_args_sym *asym,
			La_retval *regs, struct lt_args_data *data);

/* la_objsearch */
int lt_objsearch_init(struct lt_config_audit *cfg, char **ptr, int cnt);
char* lt_objsearch(struct lt_config_audit *cfg, const char *name, 
		uintptr_t *cookie, unsigned int flag);

#define PRINT(fmt, args...) \
do { \
	char lpbuf[1024]; \
	sprintf(lpbuf, "[%d %s:%05d] %s", \
		(pid_t) syscall(SYS_gettid), \
		__FUNCTION__, \
		__LINE__, \
		fmt); \
	printf(lpbuf, ## args); \
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
