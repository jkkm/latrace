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

%option prefix="lt_config_"

%{

#include <string.h>

#include "config.h"
#include "config-bison.h"
#include "lib-include.h"

extern struct lt_include *lt_config_sinc;


#define NEW_LINE() \
do { \
	lt_inc_stack(lt_config_sinc)->lineno++; \
} while(0)

#define RETURN_STR(token) \
do { \
	lt_config_lval.s = strdup(lt_config_text); return token; \
} while(0)

#define RETURN_LONG(token) \
do { \
	lt_config_lval.l = atol(lt_config_text); return token; \
} while(0)

%}

num		[-0-9]
value		({num})+
name		([-0-9a-zA-Z\./_\-\*])+
bool		YES|NO
comment		^([\s\t])*#.*

%x comment include options

%%

<<EOF>>			{ return END; }
"\n"			{ NEW_LINE(); }
{comment}		{ ; }
.			{ ; }

INCLUDE			{ BEGIN(include); return INCLUDE; }
<include>{name}		{ RETURN_STR(NAME); }
<include>"\""		{ return '"'; }
<include>\n		{ BEGIN(INITIAL); NEW_LINE(); }
<include>.		{ ; }

OPTIONS				{ BEGIN(options); return OPTIONS; }
<options>HEADERS		{ return OPT_HEADERS; }
<options>INDENT_SYM		{ return OPT_INDENT_SYM; }
<options>PIPE			{ return OPT_PIPE; }
<options>TIMESTAMP		{ return OPT_TIMESTAMP; }
<options>FRAMESIZE		{ return OPT_FRAMESIZE; }
<options>FRAMESIZE_CHECK	{ return OPT_FRAMESIZE_CHECK; }
<options>HIDE_TID		{ return OPT_HIDE_TID; }
<options>FOLLOW_FORK		{ return OPT_FOLLOW_FORK; }
<options>FOLLOW_EXEC		{ return OPT_FOLLOW_EXEC; }
<options>DEMANGLE		{ return OPT_DEMANGLE; }
<options>BRACES			{ return OPT_BRACES; }
<options>ENABLE_ARGS		{ return OPT_ENABLE_ARGS; }
<options>DETAIL_ARGS		{ return OPT_DETAIL_ARGS; }
<options>OUTPUT_TTY		{ return OPT_OUTPUT_TTY; }
<options>LIBS			{ return OPT_LIBS; }
<options>LIBS_TO		{ return OPT_LIBS_TO; }
<options>LIBS_FROM		{ return OPT_LIBS_FROM; }
<options>SYM			{ return OPT_SYM; }
<options>SYM_OMIT		{ return OPT_SYM_OMIT; }
<options>SYM_BELOW		{ return OPT_SYM_BELOW; }
<options>SYM_NOEXIT		{ return OPT_SYM_NOEXIT; }
<options>ARGS_STRING_POINTER_LENGTH { return OPT_ARGS_STRING_POINTER_LENGTH; }

<options>{bool}		{ RETURN_STR(BOOL); }
<options>{value}	{ RETURN_LONG(VALUE); }
<options>{name}		{ RETURN_STR(NAME); }
<options>{comment}	{ ; }
<options>"}"		{ BEGIN(INITIAL); return '}'; }
<options>"{"		{ return '{'; }
<options>"="		{ return '='; }
<options>"\""		{ return '"'; }
<options>","		{ return ','; }
<options>"\\"		{ ; }
<options>"\n"		{ NEW_LINE(); }
<options>.		{ ; }

%%

#ifndef yywrap
int yywrap()
{
	return 1;
	/* XXX not to get the compiler 'not used' warning */
	yyunput(0, NULL);
	input();
}
#endif

void lt_config_error(const char *m)
{
	printf("conf file [%s] line %d: %s\n",
		lt_inc_stack(lt_config_sinc)->file,
		lt_inc_stack(lt_config_sinc)->lineno,
		m);
}
