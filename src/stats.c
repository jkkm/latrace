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


#include <search.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include "config.h"


struct timeval tv_program_start;
struct timeval tv_program_stop;

/* res = a - b*/
static int tv_sub(struct timeval *res, struct timeval *a, struct timeval *b)
{
	res->tv_sec = a->tv_sec - b->tv_sec;

	if (a->tv_usec > b->tv_usec)
		res->tv_usec = a->tv_usec - b->tv_usec;
	else {
		res->tv_usec = a->tv_usec + (1000000 - b->tv_usec);
		if (res->tv_sec)
			res->tv_sec--;
	}

	return 0;
}

/* res = a + b*/
static int tv_add(struct timeval *res, struct timeval *a, struct timeval *b)
{
	struct timeval tv;
	tv.tv_sec = a->tv_sec + b->tv_sec;
	tv.tv_usec = a->tv_usec + b->tv_usec;
	if (tv.tv_usec > 1000000) {
		tv.tv_usec -= 1000000;
		tv.tv_sec++;
	}

	*res = tv;
	return 0;
}

int lt_stats_alloc(struct lt_config_app *cfg, struct lt_thread *t)
{
	int i;

	if (!t->sym_array) {
		if (!hcreate_r(t->sym_max = LT_SYM_HMAX, &t->sym_htab)) {
			perror("hcreate_r failed");
			return -1;
		}

		t->sym_array = (struct lt_stats_sym**) malloc(
				t->sym_max * sizeof(struct lt_stats_sym*));

		if (!t->sym_array) {
			perror("malloc failed");
			return -1;
		}

		return 0;
	}

	PRINT_VERBOSE(cfg, 1,
		"too bad, out of symbols, reallocation... %p - %d\n", 
		t->sym_array, t->sym_cnt);

	/* hash table reallocation */
	t->sym_max += LT_SYM_HMAX;
	hdestroy_r(&t->sym_htab);

	PRINT_VERBOSE(cfg, 1, "%s\n", "creating new hash table");

	if (!hcreate_r(t->sym_max, &t->sym_htab)) {
		perror("hcreate_r failed");
		return -1;
	}

	t->sym_array = (struct lt_stats_sym**) realloc(t->sym_array, 
			t->sym_max * sizeof(struct lt_stats_sym*));

	if (!t->sym_array) {
		perror("realloc failed");
		return -1;
	}

	PRINT_VERBOSE(cfg, 1,
		"adding symbols to new hash table %p\n", 
		t->sym_array);

	for(i = 0; i < t->sym_cnt; i++) {
		ENTRY e, *ep;
		struct lt_stats_sym *sym;

		sym = t->sym_array[i];
		e.key = sym->name;
		e.data = sym;

		PRINT_VERBOSE(cfg, 1, "adding symbol %s\n", sym->name);

		if (0 == hsearch_r(e, ENTER, &ep, &t->sym_htab)) {
			printf("failed to add hash item during reallocation\n");
			return -1;
		}
	}

	PRINT_VERBOSE(cfg, 1, "%s\n", "reallocation ok");
	return 0;
}

int lt_stats_sym(struct lt_config_app *cfg, struct lt_thread *t, 
	struct lt_fifo_msym* m)
{
	ENTRY e, *ep;
	char buf[1000];
	struct lt_stats_sym *sym;
	jmp_buf env;
	int realloc = 0;

	sprintf(buf, "%s%s", m->data + m->sym, m->data + m->lib);
	e.key = strdup(buf);
	e.data = 0;

	/* array got out of space */
	if ((t->sym_cnt == t->sym_max) && (-1 == lt_stats_alloc(cfg, t)))
		return -1;

	/* hash table got out of space */
	if (setjmp(env))
		realloc = 1;

	if (0 == hsearch_r(e, ENTER, &ep, &t->sym_htab)) {
		if (!realloc) {
			if (-1 == lt_stats_alloc(cfg, t))
				return -1;
			longjmp(env, 1);
		}
		return -1;
	}

	if (!ep->data) {
		sym = malloc(sizeof(struct lt_stats_sym));
		memset(sym, 0, sizeof(struct lt_stats_sym));

		ep->data = sym;

		sym->name = e.key;
		sym->sym = strdup(m->data + m->sym);
		sym->lib = strdup(m->data + m->lib);

		t->sym_array[t->sym_cnt] = sym;
		t->sym_cnt++;

		PRINT_VERBOSE(cfg, 1,
			"adding symbol %d %s\n", t->sym_cnt, sym->name);
	} else
		free(e.key);

	sym = ep->data;
	if (FIFO_MSG_TYPE_ENTRY == m->h.type) {
		sym->call++;
		sym->tv_cur = m->h.tv;
	}

	if (FIFO_MSG_TYPE_EXIT == m->h.type) {
		struct timeval tv;
		tv_sub(&tv, &m->h.tv, &sym->tv_cur);
		tv_add(&sym->tv_all, &sym->tv_all, &tv);
	}

	return 0;
}

static int csort;

static int qsort_compar(const void *a, const void *b)
{
	struct lt_stats_sym *data_a = *((struct lt_stats_sym**) a);
	struct lt_stats_sym *data_b = *((struct lt_stats_sym**) b);

	if (csort == LT_CSORT_TIME) {
		if (data_a->tv_all.tv_sec != data_b->tv_all.tv_sec)
			return data_b->tv_all.tv_sec - data_a->tv_all.tv_sec;
		return data_b->tv_all.tv_usec - data_a->tv_all.tv_usec;
	}

	if (csort == LT_CSORT_PERCENT) {
		int percent_a = data_a->percent * 100000;
		int percent_b = data_b->percent * 100000;
		return percent_b - percent_a;
	}

	if (csort == LT_CSORT_CALL) {
		return data_b->call - data_a->call;
	}

	if (csort == LT_CSORT_UCALL) {
		return data_b->usec_call - data_a->usec_call;
	}

	if (csort == LT_CSORT_LIB) {
		int ret = strcmp(data_a->lib, data_b->lib);
		if (ret)
			return ret;
		return strcmp(data_a->sym, data_b->sym);
	}

	if (csort == LT_CSORT_SYM) {
		int ret = strcmp(data_a->sym, data_b->sym);
		if (ret)
			return ret;
		return strcmp(data_a->lib, data_b->lib);
	}

	return 0;
}

static int lt_stats_show_thread(struct lt_config_app *cfg, struct lt_thread *t)
{
	int i;
	struct timeval tv_thread_real;
	struct timeval tv_thread_accu = { 0, 0};
	float time_global;

	PRINT_VERBOSE(cfg, 1, "%s\n", "counting total time");

	for(i = 0; i < t->sym_cnt; i++) {
		struct lt_stats_sym *sym = t->sym_array[i];
		tv_add(&tv_thread_accu, &tv_thread_accu, &sym->tv_all);
	}

	time_global = tv_thread_accu.tv_sec * 1000000 + tv_thread_accu.tv_usec;

	PRINT_VERBOSE(cfg, 1, "%s\n", "counting post mortem statistics");

	for(i = 0; i < t->sym_cnt; i++) {
		struct lt_stats_sym *sym = t->sym_array[i];
		u_int time_sym = sym->tv_all.tv_sec*1000000 + sym->tv_all.tv_usec;

		sym->percent = time_sym / (time_global/100);
		sym->usec_call = time_sym/sym->call;
	}

	PRINT_VERBOSE(cfg, 1, "%s\n", "sorting");

	csort = cfg->csort;
	qsort(t->sym_array, t->sym_cnt, sizeof(struct lt_stats_sym*), qsort_compar);

	PRINT_VERBOSE(cfg, 1, "%s\n", "printing");

	tv_sub(&tv_thread_real, &t->tv_stop, &t->tv_start);
	printf("\nThread %d (runtime %u.%06u sec)\n", 
		t->tid, (u_int) tv_thread_real.tv_sec, (u_int) tv_thread_real.tv_usec);
	printf("%3s %-6s %10s %10s %10s %-30s\n", 
		"sec", "usec", "%", "usec/call", "calls", "symbol");
	printf("%3s %-6s %10s %10s %10s %-30s\n", 
		"---", "------", "----------", "----------", "----------", "------------------------------");

	for(i = 0; i < t->sym_cnt; i++) {
		struct lt_stats_sym *sym = t->sym_array[i];
	
		/* sec:usec */
		printf("%3u:%06u ", (u_int) sym->tv_all.tv_sec, (u_int) sym->tv_all.tv_usec);

		/* % */
		printf("%10f ", sym->percent);

		/* usec/call */
		printf("%10u ", sym->usec_call);

		/* call */
		printf("%10u ", sym->call);

		if (LT_CSORT_LIB == cfg->csort) {
			/* calls symbol */
			printf("%s ", sym->lib);
			printf("%s\n", sym->sym);
		} else {
			printf("%s@", sym->sym);
			printf("%s\n", sym->lib);
		}
	}

	return 0;
}

int lt_stats_show(struct lt_config_app *cfg)
{
	struct lt_thread *t;

	struct timeval tv_program_real;
	tv_sub(&tv_program_real, &tv_program_stop, &tv_program_start);

	printf("\n--------------------------------------------------------------------------\n");
	printf("Statistics for [%s] total runtime: %u.%06u sec\n", 
		cfg->prog, 
		(u_int) tv_program_real.tv_sec, 
		(u_int) tv_program_real.tv_usec);

	for(t = lt_thread_first(cfg); t; t = lt_thread_next(cfg))
		lt_stats_show_thread(cfg, t);

	return 0;
}

