/*
  Copyright (C) 2011 Jiri Olsa <olsajiri@gmail.com>

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

%name-prefix "lt_config_"

%{
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "lib-include.h"

struct lt_include *lt_config_sinc;
static struct lt_config_app *scfg;

int lt_config_lex(void);
void lt_config_error(const char *m);

#define ERROR(fmt, args...)          \
do {                                 \
	char ebuf[1024];             \
	sprintf(ebuf, fmt, ## args); \
	lt_config_error(ebuf);       \
	YYERROR;                     \
} while(0)

static LT_LIST_HEAD(opt_list);

#define OPTION_ADD(idx, sval, nval) \
do { \
	struct lt_config_opt *opt; \
	opt = lt_config_opt_new(scfg, idx, sval, nval); \
	if (!opt) \
		ERROR("failed to process option\n"); \
	lt_list_add_tail(&opt->list, &opt_list); \
} while(0)

static struct lt_list_head ln_names;

%}

%token INCLUDE NAME BOOL VALUE END
%token OPTIONS
%token OPT_HEADERS OPT_INDENT_SYM OPT_PIPE
%token OPT_TIMESTAMP OPT_FRAMESIZE OPT_FRAMESIZE_CHECK
%token OPT_HIDE_TID OPT_FOLLOW_FORK OPT_FOLLOW_EXEC
%token OPT_DEMANGLE OPT_BRACES OPT_ENABLE_ARGS
%token OPT_DETAIL_ARGS OPT_OUTPUT_TTY
%token OPT_LIBS OPT_LIBS_TO OPT_LIBS_FROM
%token OPT_SYM OPT_SYM_OMIT OPT_SYM_BELOW OPT_SYM_NOEXIT
%token OPT_ARGS_STRING_POINTER_LENGTH

%union
{
	char *s;
	unsigned long l;
}

%type <s> NAME
%type <s> BOOL
%type <l> VALUE

%%
entry:
entry include_def
|
entry options_def
|
entry END
{
	if (lt_inc_close(scfg->sh, lt_config_sinc))
		return 0;
}
|
/* left blank intentionally */

include_def: INCLUDE '"' NAME '"'
{
        if (lt_inc_open(scfg->sh, lt_config_sinc, $3))
                ERROR("failed to process include");
}

options_def: OPTIONS '{' OPTIONS_DEF '}'
{
	struct lt_config_opt *opt, *opth;

	if (lt_config_opt_process(scfg, &opt_list))
                ERROR("failed to process options");

	lt_list_for_each_entry_safe(opt, opth, &opt_list, list) {
		lt_list_del(&opt->list);
		free(opt);
	}
}

OPTIONS_DEF:
OPTIONS_DEF OPT_HEADERS '=' '"' NAME '"'
{
	OPTION_ADD(LT_OPT_HEADERS, $5, -1);
}
|
OPTIONS_DEF OPT_INDENT_SYM '=' VALUE
{
	OPTION_ADD(LT_OPT_INDENT_SYM, NULL, $4);
}
|
OPTIONS_DEF OPT_PIPE '=' BOOL
{
	OPTION_ADD(LT_OPT_PIPE, $4, -1);
}
|
OPTIONS_DEF OPT_TIMESTAMP '=' BOOL
{
	OPTION_ADD(LT_OPT_TIMESTAMP, $4, -1);
}
|
OPTIONS_DEF OPT_FRAMESIZE '=' VALUE
{
	OPTION_ADD(LT_OPT_FRAMESIZE, NULL, $4);
}
|
OPTIONS_DEF OPT_FRAMESIZE_CHECK '=' BOOL
{
	OPTION_ADD(LT_OPT_FRAMESIZE_CHECK, $4, -1);
}
|
OPTIONS_DEF OPT_HIDE_TID '=' BOOL
{
	OPTION_ADD(LT_OPT_HIDE_TID, $4, -1);
}
|
OPTIONS_DEF OPT_FOLLOW_FORK '=' BOOL
{
	OPTION_ADD(LT_OPT_FOLLOW_FORK, $4, -1);
}
|
OPTIONS_DEF OPT_FOLLOW_EXEC '=' BOOL
{
	OPTION_ADD(LT_OPT_FOLLOW_EXEC, $4, -1);
}
|
OPTIONS_DEF OPT_DEMANGLE '=' BOOL
{
	OPTION_ADD(LT_OPT_DEMANGLE, $4, -1);
}
|
OPTIONS_DEF OPT_BRACES '=' BOOL
{
	OPTION_ADD(LT_OPT_BRACES, $4, -1);
}
|
OPTIONS_DEF OPT_ENABLE_ARGS '=' BOOL
{
	OPTION_ADD(LT_OPT_ENABLE_ARGS, $4, -1);
}
|
OPTIONS_DEF OPT_DETAIL_ARGS '=' BOOL
{
	OPTION_ADD(LT_OPT_DETAIL_ARGS, $4, -1);
}
|
OPTIONS_DEF OPT_OUTPUT_TTY '=' '"' NAME '"'
{
	OPTION_ADD(LT_OPT_OUTPUT_TTY, $5, -1);
}
|
OPTIONS_DEF OPT_LIBS '=' list_names_comma
{
	char libs[LT_LIBS_MAXSIZE];

	if (lt_config_ln_fill(&ln_names, libs, LT_LIBS_MAXSIZE))
		ERROR("failed to process libs option");

	OPTION_ADD(LT_OPT_LIBS, libs, -1);
}
|
OPTIONS_DEF OPT_LIBS_TO '=' list_names_comma
{
	char libs_to[LT_LIBS_MAXSIZE];

	if (lt_config_ln_fill(&ln_names, libs_to, LT_LIBS_MAXSIZE))
		ERROR("failed to process libs_to option");

	OPTION_ADD(LT_OPT_LIBS_TO, libs_to, -1);
}
|
OPTIONS_DEF OPT_LIBS_FROM '=' list_names_comma
{
	char libs_from[LT_LIBS_MAXSIZE];

	if (lt_config_ln_fill(&ln_names, libs_from, LT_LIBS_MAXSIZE))
		ERROR("failed to process libs_from option");

	OPTION_ADD(LT_OPT_LIBS_FROM, libs_from, -1);
}
|
OPTIONS_DEF OPT_SYM '=' list_names_comma
{
	char sym[LT_LIBS_MAXSIZE];

	if (lt_config_ln_fill(&ln_names, sym, LT_LIBS_MAXSIZE))
		ERROR("failed to process sym option");

	OPTION_ADD(LT_OPT_SYM, sym, -1);
}
|
OPTIONS_DEF OPT_SYM_OMIT '=' list_names_comma
{
	char sym_omit[LT_SYMBOLS_MAXSIZE];

	if (lt_config_ln_fill(&ln_names, sym_omit, LT_SYMBOLS_MAXSIZE))
		ERROR("failed to process sym_omit option");

	OPTION_ADD(LT_OPT_SYM_OMIT, sym_omit, -1);
}
|
OPTIONS_DEF OPT_SYM_BELOW '=' list_names_comma
{
	char sym_below[LT_SYMBOLS_MAXSIZE];

	if (lt_config_ln_fill(&ln_names, sym_below, LT_SYMBOLS_MAXSIZE))
		ERROR("failed to process sym_below option");

	OPTION_ADD(LT_OPT_SYM_BELOW, sym_below, -1);
}
|
OPTIONS_DEF OPT_SYM_NOEXIT '=' list_names_comma
{
	char sym_noexit[LT_SYMBOLS_MAXSIZE];

	if (lt_config_ln_fill(&ln_names, sym_noexit, LT_SYMBOLS_MAXSIZE))
		ERROR("failed to process sym_below option");

	OPTION_ADD(LT_OPT_SYM_NOEXIT, sym_noexit, -1);
}
|
OPTIONS_DEF OPT_ARGS_STRING_POINTER_LENGTH '=' BOOL
{
	OPTION_ADD(LT_OPT_ARGS_STRING_POINTER_LENGTH, $4, -1);
}
|
/* left blank intentionally */

list_names_comma:
list_names_comma ',' NAME
{
	if (lt_config_ln_add(&ln_names, $3))
		ERROR("failed to add list name");
}
|
NAME
{
	if (lt_config_ln_add(&ln_names, $1))
		ERROR("failed to add list name");
}

%%

int lt_config_parse_init(struct lt_config_app *cfg, struct lt_include *inc)
{
        scfg = cfg;
        lt_config_sinc = inc;
	lt_init_list_head(&ln_names);
        return 0;
}
