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

#include <string.h>
#include <stdio.h>

#include "config.h"


static struct lt_config_app cfg;

enum {
	LT_TYPE_UNKNOWN,
	LT_TYPE_LATRACE,
	LT_TYPE_CTL,
};

struct lt_type {
	char *name;
	int type;
};

static struct lt_type types[] = {
	{ .name = "latrace",     .type = LT_TYPE_LATRACE },
	{ .name = "latrace-ctl", .type = LT_TYPE_CTL },
};

#define TYPES_COUNT (sizeof(types)/sizeof(struct lt_type))

static int get_type(char *name)
{
	int i;
	char *cname = strrchr(name, '/');

	if (!cname)
		cname = name;
	else
		cname++;

	for(i = 0; i < TYPES_COUNT; i++) {
		struct lt_type *type = &types[i];

		if (!strcmp(cname, type->name))
			return type->type;
	}

	return LT_TYPE_UNKNOWN;
}

static int main_latrace(int argc, char **argv)
{
	if (-1 == lt_config(&cfg, argc, argv))
		return -1;

	if (-1 == lt_run(&cfg))
		return -1;

	if ((lt_sh(&cfg, pipe)) && (*lt_sh(&cfg, output)))
		fclose(lt_sh(&cfg, fout));

	if (lt_sh(&cfg, counts))
		lt_stats_show(&cfg);

	return 0;
}

int main(int argc, char **argv)
{

	switch(get_type(argv[0])) {

	case LT_TYPE_LATRACE:
		return main_latrace(argc, argv);

	case LT_TYPE_CTL:
		return main_ctl(argc, argv);
	}

	printf("wtf..\n");
	return -1;
}
