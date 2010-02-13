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


#include <link.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <bits/wordsize.h>
#include <gnu/lib-names.h>
#include <stdlib.h>

#include "config.h"


extern struct lt_config_audit cfg;

static __thread int pipe_fd = 0;
static __thread int flow_below_stack = 0;


static int check_names(char *name, char **ptr)
{
	char *n;

	for(n = *ptr; n; n = *(++ptr)) {
		if (strstr(name, n)) {
			PRINT_VERBOSE(&cfg, 2,
				"return %d for name %s\n", 1, name);
			return 1;
		}
	}

	PRINT_VERBOSE(&cfg, 2, "return %d for name %s\n",
			0, name);
	return 0;
}

static int check_flow_below(const char *symname, int in)
{
	int ret = flow_below_stack;

	if (check_names((char*) symname, cfg.flow_below))
		in ? ret = ++flow_below_stack : flow_below_stack--;

	return ret;
}

static int sym_entry(const char *symname, char *lib_from, char *lib_to, 
			La_regs *regs)
{
	int argret;
	char *argbuf = "", *argdbuf = "";
	struct timeval tv;

	PRINT_VERBOSE(&cfg, 2, "%s@%s\n", symname, lib_to);

	if (cfg.flow_below_cnt && !check_flow_below(symname, 1))
		return 0;

	if (lt_sh(&cfg, timestamp) || lt_sh(&cfg, counts))
		gettimeofday(&tv, NULL);

	argret = lt_sh(&cfg, args_enabled) ?
		lt_args_sym_entry(cfg.sh, (char*) symname, regs, &argbuf, &argdbuf) : -1;

	if (lt_sh(&cfg, pipe)) {
		char buf[FIFO_MSG_MAXLEN];
		int len;

		if (!pipe_fd)
			pipe_fd = lt_fifo_create(&cfg, cfg.dir);

		len = lt_fifo_msym_get(&cfg, buf, FIFO_MSG_TYPE_ENTRY, &tv,
				(char*) symname, lib_to, argbuf, argdbuf);

		return lt_fifo_send(&cfg, pipe_fd, buf, len);
	}

	lt_sh(&cfg, indent_depth)++;

	lt_out_entry(cfg.sh, &tv, symname, lib_to,
			argbuf, argdbuf);

	if (!argret) {
		free(argbuf);
		if (lt_sh(&cfg, args_detailed) && (*argdbuf))
			free(argdbuf);
	}

	return 0;
}

static int sym_exit(const char *symname, char *lib_from, char *lib_to, 
			const La_regs *inregs, La_retval *outregs)
{
	int argret;
	char *argbuf = "", *argdbuf = "";
	struct timeval tv;

	PRINT_VERBOSE(&cfg, 2, "%s@%s\n", symname, lib_to);

	if (cfg.flow_below_cnt && !check_flow_below(symname, 0))
		return 0;

	if (lt_sh(&cfg, timestamp) || lt_sh(&cfg, counts))
		gettimeofday(&tv, NULL);

	argret = lt_sh(&cfg, args_enabled) ?
		lt_args_sym_exit(cfg.sh, (char*) symname,
			(La_regs*) inregs, outregs, &argbuf, &argdbuf) : -1;

	if (lt_sh(&cfg, pipe)) {
		char buf[FIFO_MSG_MAXLEN];
		int len;

		len = lt_fifo_msym_get(&cfg, buf, FIFO_MSG_TYPE_EXIT, &tv,
				(char*) symname, lib_to, argbuf, argdbuf);

		return lt_fifo_send(&cfg, pipe_fd, buf, len);
	}

	lt_out_exit(cfg.sh, &tv, symname, lib_from,
			argbuf, argdbuf);

	lt_sh(&cfg, indent_depth)--;

	if (!argret) {
		free(argbuf);

		if (lt_sh(&cfg, args_detailed) && (*argdbuf))
			free(argdbuf);
	}

	return 0;
}

static int check_pid()
{
	pid_t pid = getpid();

	PRINT_VERBOSE(&cfg, 1, "tid = %d, cfg tid = %d\n",
			pid, lt_sh(&cfg, pid));

	if (pid != lt_sh(&cfg, pid))
		return -1;

	return 0;
}

#define CHECK_PID(ret) \
do { \
	if (cfg.sh->not_follow_fork && \
	    check_pid()) \
		return ret; \
} while(0)

unsigned int la_version(unsigned int v)
{
	return v;
}

unsigned int la_objopen(struct link_map *l, Lmid_t a, uintptr_t *cookie)
{
	char *name = l->l_name;

	if (!cfg.init_ok)
		return 0;

	if (!name)
		return 0;

	/* executable itself */
	if (!(*name))
		return LA_FLG_BINDTO | LA_FLG_BINDFROM;

	/* audit all as default */
	if ((!cfg.libs_to_cnt) &&
	    (!cfg.libs_from_cnt) &&
	    (!cfg.libs_both_cnt))
		return LA_FLG_BINDTO | LA_FLG_BINDFROM;

	if (check_names(name, cfg.libs_to))
		return LA_FLG_BINDTO;

	if (check_names(name, cfg.libs_from))
		return LA_FLG_BINDFROM;

	if (check_names(name, cfg.libs_both))
		return LA_FLG_BINDTO | LA_FLG_BINDFROM;

	/* wrong library name specified ? */
	return 0;
}

static unsigned int la_symbind(const char *symname)
{
	unsigned int flags = 0;

	if (cfg.symbols_cnt) {
		flags = LA_SYMB_NOPLTENTER;
		if (check_names((char*) symname, cfg.symbols))
			flags = 0;
	}

	return flags;
}

void la_activity(uintptr_t *cookie, unsigned int act)
{
	PRINT_VERBOSE(&cfg, 2, "entry\n");
}

char* la_objsearch(const char *name, uintptr_t *cookie, unsigned int flag)
{
	if (flag == LA_SER_ORIG)
		return (char*) name;

	return lt_objsearch(&cfg, name, cookie, flag);
}

void la_preinit(uintptr_t *__cookie)
{
	PRINT_VERBOSE(&cfg, 2, "entry\n");
}

unsigned int la_objclose(uintptr_t *__cookie)
{
	PRINT_VERBOSE(&cfg, 2, "entry\n");
	return 0;
}

uintptr_t la_symbind32(Elf32_Sym *sym, unsigned int ndx, uintptr_t *refcook,
		uintptr_t *defcook, unsigned int *flags, const char *symname)
{
	*flags = la_symbind(symname);
	return sym->st_value;
}

uintptr_t la_symbind64(Elf64_Sym *sym, unsigned int ndx, uintptr_t *refcook,
		uintptr_t *defcook, unsigned int *flags, const char *symname)
{
	*flags = la_symbind(symname);
	return sym->st_value;
}

ElfW(Addr)
pltenter(ElfW(Sym) *sym, unsigned int ndx, uintptr_t *refcook,
          uintptr_t *defcook, La_regs *regs, unsigned int *flags,
          const char *symname, long int *framesizep)
{
	struct link_map *lr = (struct link_map*) *refcook;
	struct link_map *ld = (struct link_map*) *defcook;

	do {
		if (lt_sh(&cfg, disabled))
			break;

		CHECK_PID(sym->st_value);

		sym_entry(symname, lr ? lr->l_name : NULL,
				   ld ? ld->l_name : NULL, regs);

	} while(0);

	*framesizep = lt_sh(&cfg, framesize);
	return sym->st_value;
}

unsigned int pltexit(ElfW(Sym) *sym, unsigned int ndx, uintptr_t *refcook,
         uintptr_t *defcook, const La_regs *inregs, La_retval *outregs,
         const char *symname)
{
	struct link_map *lr = (struct link_map*) *refcook;
	struct link_map *ld = (struct link_map*) *defcook;

	do {
		if (lt_sh(&cfg, disabled))
			break;

		CHECK_PID(0);

		sym_exit(symname, lr ? lr->l_name : NULL,
				  ld ? ld->l_name : NULL, inregs, outregs);

	} while(0);

	return 0;
}
