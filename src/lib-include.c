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


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "config.h"
#include "lib-include.h"

#define YY_BUF_SIZE        16384

extern int errno;

static FILE* open_include_dir(struct lt_config_shared *cfg, char *file,
			      char *dir)
{
	FILE *f;
	char fn[LT_MAXFILE];
	int len_file, len_dir;

	len_file = strlen(file);
	len_dir  = strlen(dir);

	if (len_file > (LT_MAXFILE - len_dir - 1)) {
		printf("file name length crossed the max %u: %s\n",
			(u_int) (LT_MAXFILE - len_dir), file);
		return NULL;
	}

	sprintf(fn, "%s/%s", dir, file);

	f = fopen(fn, "r");
	if (f) {
		PRINT_VERBOSE(cfg, 1, "open ok [%s]\n", fn);
		return f;
	}

	PRINT_VERBOSE(cfg, 1, "open failed [%s]: %s\n",
			fn, strerror(errno));
	return NULL;
}

static FILE* open_include(struct lt_config_shared *cfg, char *file)
{
	FILE *f;

	/* we got an absolute path */
	if ((NULL != (f = fopen(file, "r")))) {
		PRINT_VERBOSE(cfg, 1, "open ok [%s]\n", file);
		return f;
	}

	/* give up if there was already the absolute name */
	if (*file == '/') {
		printf("open failed [%s]: %s\n", file, strerror(errno));
		return NULL;
	}

	PRINT_VERBOSE(cfg, 1, "open failed [%s]: %s\n",
			file, strerror(errno));

	/* not an absolute name, give it a chance
	   inside of the LT_CONF_DIR directory */
	f = open_include_dir(cfg, file, LT_CONF_DIR);
	if (f)
		return f;

	/* not in LT_CONF_DIR directory, give it a chance
	   inside of the LT_CONF_HEADERS_DIR directory */
	f = open_include_dir(cfg, file, LT_CONF_HEADERS_DIR);
	if (f)
		return f;

	return NULL;
}

int lt_inc_open(struct lt_config_shared *cfg, struct lt_include *inc,
		    char *file)
{
	struct lt_include_stack *inc_stack;
	FILE *f;

	PRINT_VERBOSE(cfg, 1, "opening buffer for [%s] depth %d\n",
			file, inc->stack_idx);

	if ((inc->stack_idx + 1) == MAX_INCLUDE_DEPTH) {
		printf("include depth overstep");
		return -1;
	}

	if (NULL == (f = open_include(cfg, file)))
		return -1;

	*inc->in = f;

	inc_stack = &inc->stack[inc->stack_idx++];
	memset(inc_stack, 0, sizeof(*inc_stack));

	inc_stack->in     = f;
	inc_stack->file   = strdup(file);
	inc_stack->lineno = 1;
	inc_stack->buf    = inc->create_buffer(f, YY_BUF_SIZE);

	inc->switch_to_buffer(inc_stack->buf);

	PRINT_VERBOSE(cfg, 1, "opened buffer for [%s] depth %d\n",
			file, inc->stack_idx);
	return 0;
}

int lt_inc_close(struct lt_config_shared *cfg, struct lt_include *inc)
{
	struct lt_include_stack *inc_stack = &inc->stack[--inc->stack_idx];

	PRINT_VERBOSE(cfg, 1, "buffer closed [%s], depth [%d]\n",
			inc_stack->file, inc->stack_idx);

	free(inc_stack->file);

	/* EOF with no other includes on stack */
	if (!inc->stack_idx)
		return -1;

	/* looks like the base buffer is  cleaned up by the
	   flex itself, so we do the actual cleaning
	   only for includes */
	inc->delete_buffer(inc_stack->buf);
	fclose(inc_stack->in);

	inc_stack = &inc->stack[inc->stack_idx - 1];
	inc->switch_to_buffer(inc_stack->buf);
	return 0;
}

struct lt_include_stack* lt_inc_stack(struct lt_include *inc)
{
	struct lt_include_stack *inc_stack = &inc->stack[inc->stack_idx - 1];
	return inc_stack;
}
