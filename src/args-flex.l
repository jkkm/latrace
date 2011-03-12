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

%option prefix="lt_args_"

%{

#include <string.h>

#include "config.h"
#include "args-bison.h"
#include "lib-include.h"

extern struct lt_include *lt_args_sinc;

%}

alphnum		[-0-9a-zA-Z_]
name		({alphnum})+
filename	([-0-9a-zA-Z\./_])+

%x comment include
%%

"/*"			BEGIN(comment);
<comment>[^*\n]*	/* eat anything that's not a '*' */
<comment>"*"+[^*/\n]*	/* eat up '*'s not followed by '/'s */
<comment>\n		{ lt_inc_stack(lt_args_sinc)->lineno++; }
<comment>"*"+"/"	BEGIN(INITIAL);

"#include"		{ BEGIN(include); return INCLUDE; }
<include>{filename}	{ lt_args_lval.s = strdup(lt_args_text); return FILENAME; }
<include>"\""		{ return '"'; }
<include>\n		{ BEGIN(INITIAL); }
<include>.		{ ; }

"extern"		{ ; }
"const"			{ ; }
<<EOF>>			{ return END; }
"struct"		{ return STRUCT; }
"enum"			{ return ENUM; }
"typedef"		{ return TYPEDEF; }
{name}			{ lt_args_lval.s = strdup(lt_args_text); return NAME; }
"\*"+			{ return POINTER; }
")"			{ return ')'; }
"("			{ return '('; }
"}"			{ return '}'; }
"{"			{ return '{'; }
";"			{ return ';'; }
","			{ return ','; }
"="			{ return '='; }
\			{ ; }
\n			{ lt_inc_stack(lt_args_sinc)->lineno++; }
.			{ ; }

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

void lt_args_error(const char *m)
{
	printf("header file [%s] line %d: %s\n",
		lt_inc_stack(lt_args_sinc)->file,
		lt_inc_stack(lt_args_sinc)->lineno,
		m);
}
