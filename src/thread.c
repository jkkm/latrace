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


#include <stdlib.h>
#include <string.h>

#include "config.h"


struct lt_thread *lt_thread_add(struct lt_config_app *cfg, int fd, pid_t pid)
{
	struct lt_thread *t;

	if (NULL == (t = (struct lt_thread*) malloc(sizeof(struct lt_thread)))) {
		perror("malloc failed");
		return NULL;
	}

	memset(t, 0x0, sizeof(*t));

	if (-1 == lt_stats_alloc(cfg, t)) {
		free(t);
		return NULL;
	}

	t->fifo_fd = fd;
	t->tid = pid;
	gettimeofday(&t->tv_start, NULL);

	t->next = cfg->threads;
	cfg->threads = t;
	return t;
}

struct lt_thread *lt_thread_first(struct lt_config_app *cfg)
{
	return cfg->iter = cfg->threads;
}

struct lt_thread *lt_thread_next(struct lt_config_app *cfg)
{
	return cfg->iter = (cfg->iter ? cfg->iter->next : NULL); 
}
