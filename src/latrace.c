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


#include <stdio.h>

#include "config.h"


static struct lt_config_app cfg;


int main(int argc, char **argv)
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
