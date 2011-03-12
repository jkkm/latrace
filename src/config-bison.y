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
	opt = lt_config_opt_new(idx, sval, nval); \
	if (!opt) \
		ERROR("failed to process option\n"); \
	lt_list_add_tail(&opt->list, &opt_list); \
} while(0)
%}

%token INCLUDE FILENAME BOOL VALUE END
%token OPTIONS
%token OPT_HEADERS OPT_INDENT_SYM OPT_PIPE
%token OPT_TIMESTAMP OPT_FRAMESIZE OPT_FRAMESIZE_CHECK
%token OPT_HIDE_TID OPT_FOLLOW_FORK OPT_FOLLOW_EXEC
%token OPT_DEMANGLE OPT_BRACES OPT_ENABLE_ARGS
%token OPT_DETAIL_ARGS

%union
{
	char *s;
	unsigned long l;
}

%type <s> FILENAME
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

include_def: INCLUDE '"' FILENAME '"'
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
OPTIONS_DEF OPT_HEADERS '=' '"' FILENAME '"'
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
/* left blank intentionally */

%%

int lt_config_parse_init(struct lt_config_app *cfg, struct lt_include *inc)
{
        scfg = cfg;
        lt_config_sinc = inc;
        return 0;
}
