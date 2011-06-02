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
AC_INIT(latrace, 0.5.11, olsajiri@gmail.com)
AC_CONFIG_SRCDIR([src/latrace.c])

AC_PROG_CC
AC_PROG_LEX
AS_IF([test "x$LEX" = "x:"],
      [AC_MSG_ERROR([lex/flex is needed.])])
AC_PROG_YACC
AS_IF([test "x$YACC" = "x:"],
      [AC_MSG_ERROR([yacc/bison is needed.])])
AC_PROG_LN_S
AC_PATH_PROG([ASCIIDOC], [asciidoc], [:])
AC_PATH_PROG([XMLTO], [xmlto], [:])

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
AC_SYS_LARGEFILE

date=`date "+%B %G"`
unamem=`uname -m | sed -e 's/i.86/i686/' -e 's/^armv.*/arm/'`

AC_SUBST(CONFIG_DATE, "$date")
AC_SUBST(CONFIG_SYSDEP_DIR, "$unamem")
AC_SUBST(CONFIG_VERSION, "AC_PACKAGE_VERSION")

AC_DEFINE(CONFIG_LT_CONFIG, "/tmp/lt-config", [Temporary directory prefix.])
AC_DEFINE(CONFIG_VERSION, "AC_PACKAGE_VERSION", [Version of latrace.])

# for following architectures we have argument display support
if test "$unamem" = "x86_64" -o\
	"$unamem" = "i686" -o\
	"$unamem" = "arm"; then
	AC_DEFINE(CONFIG_ARCH_HAVE_ARGS)
	AC_SUBST(CONFIG_ARCH_HAVE_ARGS, "y")
else
	AC_MSG_WARN([Arguments display support disabled])
fi

# for following architectures we have automated tests support
if test "$unamem" = "x86_64" -o\
	"$unamem" = "i686" -o\
	"$unamem" = "arm" ; then
	AC_SUBST(CONFIG_ARCH_HAVE_TEST, "y")
else
	AC_MSG_WARN([No automated test support])
fi

AC_SEARCH_LIBS([cplus_demangle], [iberty_pic iberty],
	[AC_DEFINE(CONFIG_LIBERTY, 1, "Liberty found.")],
	[AC_MSG_WARN([libiberty not found, no demangle support (install binutils-dev)])])

AC_CONFIG_HEADER([src/autoconf.h])
AC_CONFIG_FILES([src/autoconf.make])
AC_CONFIG_FILES([doc/asciidoc.conf])
AC_CONFIG_FILES([etc/latrace.d/latrace.conf])

AC_OUTPUT
