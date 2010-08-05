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


#ifndef STACK_H
#define STACK_H

/*
  TODO Describe the implementation in more details TODO

  Architecture specific data inside lt_arg.

  bits    64                32|31                 0
          --------------------|--------------------
  fields                flags |              offset


  offset
    - offset or memory or registers
  flags
    - memory/registers switch
*/

#define LT_STACK_ALIGN(arg) ((arg + 7) & ~7)

#define ARCH_POS_FLAGS   32

#define ARCH_FLAG_MEM          1L
#define ARCH_FLAG_REG_INTEGER  2L
#define ARCH_FLAG_REG_SSE      3L
#define ARCH_FLAG_SVREG        4L

#define ARCH_GET(arg)        ((int64_t) ((arg)->arch))
#define ARCH_GET_FLAG(arg)   ((int)  ((long) arg->arch >> ARCH_POS_FLAGS))
#define ARCH_GET_OFFSET(arg) ((long) ((long) arg->arch & 0xFFFFFFFFL))

#define ARCH_ZERO(arg) \
do {\
	arg->arch = NULL; \
} while(0)

#define ARCH_SET(arg, flags, offset) \
do { \
	arg->arch = (void*) (offset + \
		    (flags  << ARCH_POS_FLAGS)); \
} while(0)

/* registers index */
#define LT_64_REG_RAX     1
#define LT_64_REG_RCX     2
#define LT_64_REG_RDX     3
#define LT_64_REG_RDI     4
#define LT_64_REG_RSI     5
#define LT_64_REG_R8      6
#define LT_64_REG_R9      7
#define LT_64_REG_RBP     8
#define LT_64_REG_RSP     9
#define LT_64_REG_XMM0    10
#define LT_64_REG_XMM1    11
#define LT_64_REG_XMM2    12
#define LT_64_REG_XMM3    13
#define LT_64_REG_XMM4    14
#define LT_64_REG_XMM5    15
#define LT_64_REG_XMM6    16
#define LT_64_REG_XMM7    17

/* argument classes */
#define LT_CLASS_NONE     1
#define LT_CLASS_INTEGER  2
#define LT_CLASS_SSE      3
#define LT_CLASS_MEMORY   4096

#endif // !STACK_H
