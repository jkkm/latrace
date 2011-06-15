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


#include <strings.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

static void *root = NULL;
extern struct lt_config_audit cfg;

static int compare(const void *a, const void *b)
{
	const struct lt_symbol *sym_a = a;
	const struct lt_symbol *sym_b = b;

	PRINT_VERBOSE(&cfg, 1, "a %p, b %p\n", sym_a, sym_b);
	PRINT_VERBOSE(&cfg, 1, "a ptr %p, b ptr  %p\n",
			sym_a ? sym_a->ptr : NULL,
			sym_b ? sym_b->ptr : NULL);
	PRINT_VERBOSE(&cfg, 1, "a name %s, b name %s\n",
			sym_a ? sym_a->name : "",
			sym_b ? sym_b->name: "");

	if (!sym_a || !sym_b)
		return 1;

/* XXX There's a glibc bug/feature causing the symbol to
 * have different value in plt entry/exit.. so using name
 * check instead.. so far.. ;) */

#define SEARCH_NAME
#undef SEARCH_PTR

#ifdef SEARCH_NAME
	return strcmp(sym_a->name, sym_b->name);
#endif

#if SEARCH_PTR
	PRINT_VERBOSE(&cfg, 1, "%s(%p) %s(%p)\n",
			sym_a->name, sym_a->ptr,
			sym_b->name, sym_b->ptr);

	if (sym_a->ptr > sym_b->ptr)
		return 1;

	if (sym_a->ptr < sym_b->ptr)
		return -1;

	return 0;
#endif
}

static int symbol_init(struct lt_config_shared *cfg,
		       struct lt_symbol *sym, const char *name)
{
	struct lt_args_sym* a = NULL;

	if (lt_sh(cfg, args_enabled)) {
		a = lt_args_sym_get(cfg, name);
		if (!a)
			return -1;
	}

	sym->args = a;

	PRINT_VERBOSE(cfg, 1, "ok name %s, ptr %p, sym %p\n",
			name, sym->ptr, sym);
	return 0;
}

struct lt_symbol* lt_symbol_bind(struct lt_config_shared *cfg,
				void *ptr, const char *name)
{
	static struct lt_symbol *sym = NULL;
	struct lt_symbol *s = NULL;
	void *val;

	PRINT_VERBOSE(cfg, 1, "checking %s(%p)\n", name, ptr);

	/* symbol already added */
	s = lt_symbol_get(cfg, ptr, name);
	if (s) {
		PRINT_VERBOSE(cfg, 1, "found %s, ptr %p, sym %p\n",
				name, sym->ptr, sym);
		return s;
	}

	if (!sym) {
		sym = malloc(sizeof(*sym));
		if (!sym)
			return NULL;
	}

	bzero(sym, sizeof(*sym));
	sym->ptr  = ptr;
	sym->name = name;

	/* do we care about this symbol? */
	if (symbol_init(cfg, sym, name))
		return NULL;

	/* we do, let's add it */
	val = tsearch((void *) sym, &root, compare);
	if (!val)
		return NULL;

	/* symbol properly added */
	s = (*(void**) val);

	PRINT_VERBOSE(cfg, 1, "added %s, ptr %p, sym %p\n",
			name, sym->ptr, sym);
	sym = NULL;
	return s;
}

struct lt_symbol* lt_symbol_get(struct lt_config_shared *cfg,
				void *ptr, const char *name)
{
	void *val;
	struct lt_symbol *s;
	struct lt_symbol sym = {
		.ptr  = ptr,
		.name = name,
	};

	val = tfind(&sym, &root, compare);

	if (!val)
		s = NULL;
	else
		s = *(struct lt_symbol**) val;

	PRINT_VERBOSE(cfg, 1, "found %p '%s'\n", s, s ? s->name : "");
	return s;
}
