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

#ifndef ARGS_H
#define ARGS_H

#define LT_ARGS_ARCH_CONF 1

struct lt_config_shared;

static inline
char* lt_args_arch_conf(struct lt_config_shared *cfg)
{
	static char buf[LT_MAXFILE];

	sprintf(buf, "%s/%s",
		LT_CONF_HEADERS_DIR,
		"sysdeps/x86_64/latrace.h");

	return buf;
}

#endif /* ARGS_H */
