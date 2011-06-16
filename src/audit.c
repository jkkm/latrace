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
static __thread int indent_depth = 0;


static int check_names(char *name, char **ptr)
{
	char *n;

	for(n = *ptr; n; n = *(++ptr)) {
		if ((n[0] == '*' && strstr(name, n + 1)) ||
		    strcmp(name, n) == 0) {
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

static void free_argbuf(int argret, char *argbuf, char *argdbuf)
{
	if (argret)
		return;

	free(argbuf);
	if (lt_sh(&cfg, args_detailed) && (*argdbuf))
		free(argdbuf);
}

static int sym_entry(const char *symname, void *ptr,
		     char *lib_from, char *lib_to, La_regs *regs)
{
	int argret = -1;
	char *argbuf = "", *argdbuf = "";
	struct timeval tv;
	struct lt_symbol *sym = NULL;

	PRINT_VERBOSE(&cfg, 2, "%s@%s\n", symname, lib_to);

	if (cfg.flow_below_cnt && !check_flow_below(symname, 1))
		return 0;

	if (lt_sh(&cfg, timestamp) || lt_sh(&cfg, counts))
		gettimeofday(&tv, NULL);

	if (lt_sh(&cfg, global_symbols))
		sym = lt_symbol_get(cfg.sh, ptr, symname);

#ifdef CONFIG_ARCH_HAVE_ARGS
	argret = lt_sh(&cfg, args_enabled) ?
		lt_args_sym_entry(cfg.sh, sym, regs, &argbuf, &argdbuf) : -1;
#endif

	if (lt_sh(&cfg, pipe)) {
		char buf[FIFO_MSG_MAXLEN];
		int len;

		if (!pipe_fd)
			pipe_fd = lt_fifo_create(&cfg, cfg.dir);

		len = lt_fifo_msym_get(&cfg, buf, FIFO_MSG_TYPE_ENTRY, &tv,
				(char*) symname, lib_to, argbuf, argdbuf);

		free_argbuf(argret, argbuf, argdbuf);
		return lt_fifo_send(&cfg, pipe_fd, buf, len);
	}

	indent_depth++;

	lt_out_entry(cfg.sh, &tv, syscall(SYS_gettid),
			indent_depth,
			symname, lib_to,
			argbuf, argdbuf);

	free_argbuf(argret, argbuf, argdbuf);
	return 0;
}

static int sym_exit(const char *symname, void *ptr,
			 char *lib_from, char *lib_to,
			 const La_regs *inregs, La_retval *outregs)
{
	int argret = -1;
	char *argbuf = "", *argdbuf = "";
	struct timeval tv;
	struct lt_symbol *sym = NULL;

	PRINT_VERBOSE(&cfg, 2, "%s@%s\n", symname, lib_to);

	if (cfg.flow_below_cnt && !check_flow_below(symname, 0))
		return 0;

	if (lt_sh(&cfg, timestamp) || lt_sh(&cfg, counts))
		gettimeofday(&tv, NULL);

	if (lt_sh(&cfg, global_symbols))
		sym = lt_symbol_get(cfg.sh, ptr, symname);

#ifdef CONFIG_ARCH_HAVE_ARGS
	argret = lt_sh(&cfg, args_enabled) ?
		lt_args_sym_exit(cfg.sh, sym,
			(La_regs*) inregs, outregs, &argbuf, &argdbuf) : -1;
#endif

	if (lt_sh(&cfg, pipe)) {
		char buf[FIFO_MSG_MAXLEN];
		int len;

		len = lt_fifo_msym_get(&cfg, buf, FIFO_MSG_TYPE_EXIT, &tv,
				(char*) symname, lib_to, argbuf, argdbuf);

		free_argbuf(argret, argbuf, argdbuf);
		return lt_fifo_send(&cfg, pipe_fd, buf, len);
	}

	lt_out_exit(cfg.sh, &tv, syscall(SYS_gettid),
			indent_depth,
			symname, lib_from,
			argbuf, argdbuf);

	indent_depth--;

	free_argbuf(argret, argbuf, argdbuf);
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

#define CHECK_DISABLED(ret) \
do { \
	if (lt_sh(&cfg, disabled)) \
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

static unsigned int la_symbind(ElfW(Sym) *sym, const char *symname)
{
	unsigned int flags = 0;

	/* particular symbols specified, omit all others */
	if (cfg.symbols_cnt) {
		flags = LA_SYMB_NOPLTENTER|LA_SYMB_NOPLTEXIT;
		if (check_names((char*) symname, cfg.symbols))
			flags = 0;
	}

	/* we might want just pltenter for some.. eg for _setjmp */
	if (cfg.symbols_noexit_cnt) {
		if (check_names((char*) symname, cfg.symbols_noexit))
			flags = LA_SYMB_NOPLTEXIT;
	}

	/* and keep omit options the strongest */
	if (cfg.symbols_omit_cnt) {
		if (check_names((char*) symname, cfg.symbols_omit))
			flags = LA_SYMB_NOPLTENTER|LA_SYMB_NOPLTEXIT;
	}

	/* we are interested in this symbol */
	if (lt_sh(&cfg, global_symbols) &&
	    !(flags & LA_SYMB_NOPLTENTER))
		lt_symbol_bind(cfg.sh, (void*) sym->st_value, symname);

	return flags;
}

void la_activity(uintptr_t *cookie, unsigned int act)
{
	PRINT_VERBOSE(&cfg, 2, "%s\n", "entry");
}

char* la_objsearch(const char *name, uintptr_t *cookie, unsigned int flag)
{
	if (flag == LA_SER_ORIG)
		return (char*) name;

	return lt_objsearch(&cfg, name, cookie, flag);
}

void la_preinit(uintptr_t *__cookie)
{
	PRINT_VERBOSE(&cfg, 2, "%s\n", "entry");
}

unsigned int la_objclose(uintptr_t *__cookie)
{
	PRINT_VERBOSE(&cfg, 2, "%s\n", "entry");
	return 0;
}

#if __ELF_NATIVE_CLASS == 32
uintptr_t la_symbind32(Elf32_Sym *sym, unsigned int ndx, uintptr_t *refcook,
		uintptr_t *defcook, unsigned int *flags, const char *symname)
{
	*flags = la_symbind(sym, symname);
	return sym->st_value;
}
#elif __ELF_NATIVE_CLASS == 64
uintptr_t la_symbind64(Elf64_Sym *sym, unsigned int ndx, uintptr_t *refcook,
		uintptr_t *defcook, unsigned int *flags, const char *symname)
{
	*flags = la_symbind(sym, symname);
	return sym->st_value;
}
#endif

ElfW(Addr)
pltenter(ElfW(Sym) *sym, unsigned int ndx, uintptr_t *refcook,
          uintptr_t *defcook, La_regs *regs, unsigned int *flags,
          const char *symname, long int *framesizep)
{
	struct link_map *lr = (struct link_map*) *refcook;
	struct link_map *ld = (struct link_map*) *defcook;

	do {
		CHECK_DISABLED(sym->st_value);

		CHECK_PID(sym->st_value);

		sym_entry(symname, (void*) sym->st_value,
			  lr ? lr->l_name : NULL,
			  ld ? ld->l_name : NULL,
			  regs);

	} while(0);

	*framesizep = lt_stack_framesize(&cfg, regs);

	return sym->st_value;
}

unsigned int pltexit(ElfW(Sym) *sym, unsigned int ndx, uintptr_t *refcook,
         uintptr_t *defcook, const La_regs *inregs, La_retval *outregs,
         const char *symname)
{
	struct link_map *lr = (struct link_map*) *refcook;
	struct link_map *ld = (struct link_map*) *defcook;

	do {
		CHECK_PID(0);

		sym_exit(symname, (void*) sym->st_value,
			 lr ? lr->l_name : NULL,
			 ld ? ld->l_name : NULL,
			 inregs, outregs);

	} while(0);

	return 0;
}
