#  Copyright (C) 2008, 2009 Jiri Olsa <olsajiri@gmail.com>
#
#  This file is part of the latrace.
#
#  The latrace is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  The latrace is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with the latrace (file COPYING).  If not, see 
#  <http://www.gnu.org/licenses/>.


AC_PREREQ(2.61)
AC_INIT(latrace, 0.5.8, olsajiri@gmail.com)
AC_CONFIG_SRCDIR([src/latrace.c])

AC_PROG_CC
AC_PROG_LEX
AC_PROG_YACC
AC_PROG_LN_S

AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([libintl.h])
AC_CHECK_HEADERS([malloc.h])
AC_CHECK_HEADERS([stddef.h])
AC_C_CONST
AC_TYPE_PID_T
AC_FUNC_FORK
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset setenv])
AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNCS([mkfifo])
AC_CHECK_FUNCS([select])
AC_CHECK_FUNCS([strchr])
AC_CHECK_FUNCS([strdup])
AC_CHECK_FUNCS([strstr])
AC_CHECK_FUNCS([strerror])
AC_CHECK_FUNCS([strtol])
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([limits.h])
AC_FUNC_MALLOC
AC_FUNC_ALLOCA
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_SELECT_ARGTYPES
AC_HEADER_DIRENT
AC_HEADER_SYS_WAIT
AC_HEADER_TIME
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT32_T
AC_TYPE_SIZE_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT8_T

date=`date "+%B %G"`
unamem=`uname -m | sed 's/i.86/i686/'`

AC_SUBST(LT_DATE, "$date")
AC_SUBST(CONFIG_SYSDEP_DIR, "$unamem")
AC_SUBST(LT_VER, "AC_PACKAGE_VERSION")

AC_DEFINE(CONFIG_LT_CONFIG, "/tmp/lt-config", [Temporary directory prefix.])
AC_DEFINE(LT_VER, "AC_PACKAGE_VERSION", [Version of latrace.])

if test "$unamem" = "x86_64"; then
	AC_DEFINE(LT_ARCH_X86_64, 1, [The x86_64 arch.])
elif test "$unamem" = "i686"; then
	AC_DEFINE(LT_ARCH_X86, 1, [The x86 arch.])
fi

CONFIG_LIBERTY=[]
AC_SEARCH_LIBS(cplus_demangle,iberty,
		[iberty_found=yes],
		[iberty_found=no],[])
if test "$iberty_found" = "yes"; then
	AC_DEFINE(LT_LIBERTY, 1, "Liberty found.")
	CONFIG_LIBERTY=[-liberty]
fi

AC_SUBST(CONFIG_LIBERTY)

AC_CONFIG_HEADER([src/autoconf.h])
AC_CONFIG_FILES([src/autoconf.make])
AC_CONFIG_FILES([doc/asciidoc.conf])
AC_CONFIG_FILES([etc/latrace.conf])

AC_OUTPUT
