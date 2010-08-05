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


#include <string.h>

#include "config.h"


static int add_subst(struct lt_config_audit *cfg, char *subst)
{
	struct lt_objsearch *s;
	char *dst, *src = subst;
	int type;

	if (cfg->subst_cnt == LT_NAMES_MAX)
		return -1;

	do {
		if ((dst = strchr(subst, '='))) {
			type = LT_OS_PATH;
			break;
		}

		if ((dst = strchr(subst, '%'))) {
			type = LT_OS_PTN;
			break;
		}

		if ((dst = strchr(subst, '~'))) {
			type = LT_OS_PTN2PATH;
			break;
		}

	} while(0);

	if (!dst)
		return -1;

	*dst++ = 0x0;

	PRINT_VERBOSE(cfg, 2, "adding subst type %d, src [%s], dst [%s]\n",
			type, src, dst);

	s = &cfg->subst[cfg->subst_cnt++];
	s->type = type;
	s->src  = src;
	s->dst  = dst;

	return 0;
}

/*  
    Comparing src with the name, if matched
    replace the name with dst.

    name -> /lib/krava.so
    src  -> /lib/krava.so
    dst  -> /lib/debil.so

    ret  -> /lib/debil.so
*/
static int match_path(struct lt_config_audit *cfg, const char *name, 
			struct lt_objsearch *s, char **ret)
{
	PRINT_VERBOSE(cfg, 2, "name [%s], src [%s], dst [%s]\n",
			name, s->src, s->dst);

	*ret = s->dst;

	if (!strcmp(name, s->src))
		return 1;

	return 0;
}

/*  
    Matching src in the name, if found, 
    replace the src with dst part.

    name -> /lib/krava.so
    src  -> krava
    dst  -> debil

    ret  -> /lib/debil.so
*/
static int match_ptn(struct lt_config_audit *cfg, const char *name, 
			struct lt_objsearch *s, char **ret)
{
	char *pat, *r;

	PRINT_VERBOSE(cfg, 2, "name [%s], src [%s], dst [%s]\n",
			name, s->src, s->dst);

	pat = strstr(name, s->src);
	if (!pat)
		return 0;

	r = *ret = malloc(strlen(name) + strlen(s->dst));
	if (!r) {
		perror("malloc failed");
		return 0;
	}

	strncpy(r, name, pat - name);
	r += (pat - name);
	strcpy(r, s->dst);
	strcat(r, pat + strlen(s->src));

	PRINT_VERBOSE(cfg, 2, "return %s\n", *ret);

	return 1;
}

/*  
    Looking for src in the name, if found, 
    replace the name with dst.

    name -> /lib/krava.so
    src  -> krava
    dst  -> /lib/krava1.so

    ret  -> /lib/krava1.so
*/
static int match_ptn2path(struct lt_config_audit *cfg, const char *name, 
			struct lt_objsearch *s, char **ret)
{
	PRINT_VERBOSE(cfg, 2, "name [%s], src [%s], dst [%s]\n",
			name, s->src, s->dst);

	*ret = s->dst;
	if (strstr(name, s->src))
		return 1;

	return 0;
}

static int match(struct lt_config_audit *cfg, const char *name, 
		struct lt_objsearch *s, char **ret)
{
	switch(s->type) {
	case LT_OS_PATH:
		return match_path(cfg, name, s, ret);
	case LT_OS_PTN:
		return match_ptn(cfg, name, s, ret);
	case LT_OS_PTN2PATH:
		return match_ptn2path(cfg, name, s, ret);
	};

	return 0;
}

int lt_objsearch_init(struct lt_config_audit *cfg, char **ptr, int cnt)
{
	int i;

	for(i = 0; i < cnt; i++)
		if (-1 == add_subst(cfg, ptr[i]))
			return -1;

	return 0;
}

char* lt_objsearch(struct lt_config_audit *cfg, const char *name, 
			uintptr_t *cookie, unsigned int flag)
{
	int i;
	char *ret = NULL;

	for(i = 0; i < cfg->subst_cnt; i++)
		if (match(cfg, name, &cfg->subst[i], &ret))
			return ret;

	return (char*) name;
}
