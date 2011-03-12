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


#ifndef LIB_INCLUDE_H
#define LIB_INCLUDE_H

#include <stdio.h>
#include "config.h"

#define MAX_INCLUDE_DEPTH  10

struct lt_include_stack {
	FILE *in;
	void *buf;
	char *file;
	int   lineno;
};

#ifndef YY_TYPEDEF_YY_BUFFER_STATE
#define YY_TYPEDEF_YY_BUFFER_STATE
typedef struct yy_buffer_state *YY_BUFFER_STATE;
#endif

typedef YY_BUFFER_STATE (*yy_create_buffer_t)(FILE*, int);
typedef void (*yy_switch_to_buffer_t)(YY_BUFFER_STATE);
typedef void (*yy_delete_buffer_t)(YY_BUFFER_STATE);

struct lt_include {
	struct lt_include_stack stack[MAX_INCLUDE_DEPTH];
	int stack_idx;

	FILE                **in;
	yy_create_buffer_t    create_buffer;
	yy_switch_to_buffer_t switch_to_buffer;
	yy_delete_buffer_t    delete_buffer;
};

int lt_inc_open(struct lt_config_shared *cfg, struct lt_include *inc,
		    char *file);
int lt_inc_close(struct lt_config_shared *cfg, struct lt_include *inc);
struct lt_include_stack* lt_inc_stack(struct lt_include *inc);

#endif /* LIB_INCLUDE_H */
