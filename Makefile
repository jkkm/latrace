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


-include src/autoconf.make

confdir  := $(sysconfdir)/latrace.d
confarch := $(sysconfdir)/latrace.d/sysdeps/$(CONFIG_SYSDEP_DIR)

# looks like DESTDIR is a standard, but prioritize ROOTDIR anyway
ifdef DESTDIR
ifndef ROOTDIR
ROOTDIR=$(DESTDIR)
endif
endif

ifneq ($(findstring $(MAKEFLAGS),w),w)
PRINT_DIR = --no-print-directory
else # "make -w"
NO_SUBDIR = :
endif

# nice output definition
# Mostly copied from kernel and git makefiles.

ifndef V
	QUIET_CC          = @echo "  CC" $@;
	QUIET_LD          = @echo "  LD" $@;
	QUIET_LEX         = @echo "  LE" $@;
	QUIET_YACC        = @echo "  YA" $@;
	QUIET_DEP         = @echo "  DEP" $@;
	QUIET_GEN         = @echo "  GEN" $@;
	QUIET_ASCIIDOC    = @echo "  ASCIIDOC" $@;
	QUIET_XMLTO       = @echo "  XMLTO" $@;
	QUIET_PKG         = @echo -n "  PKG ";
	QUIET_INSTALL     = @echo -n "  INSTALL ";
	QUIET_LN          = @echo -n "  LN ";
	QUIET_RM          = @echo -n "  CLEAN ";
	QUIET_MKDIR       = @echo -n "  MKDIR ";
endif

# install file, arguments:
#    1 - file to install
#    2 - directory to install to
#    3 - permissions
define install
	$(QUIET_INSTALL) echo $(ROOTDIR)$2/$(notdir $1) | sed 's:[/]\+:/:g'; \
	mkdir -p $(ROOTDIR)$2; \
	install -m $3 $1 $(ROOTDIR)$2;
endef

# install link, arguments:
#    1 - source
#    2 - dest
#    3 - directory
define link
	$(QUIET_LN) echo $(ROOTDIR)$3/$(notdir $2) | sed 's:[/]\+:/:g'; \
	$(RM) -f $(ROOTDIR)$3/$(notdir $2); \
	(cd $(ROOTDIR)/$3; ln -s $1 $2)
endef

# remove file/dir
define remove
	$(QUIET_RM) echo $1; $(RM) -rf $1
endef

.PHONY: all clean tags install package .FORCE-LATRACE-CFLAGS

all::

install:: all
	$(call install,etc/latrace.conf,$(sysconfdir),644)
	$(call install,etc/latrace.d/ctype.conf,$(confdir),644)
	$(call install,etc/latrace.d/inet.conf,$(confdir),644)
	$(call install,etc/latrace.d/misc.conf,$(confdir),644)
	$(call install,etc/latrace.d/typedefs.conf,$(confdir),644)
	$(call install,etc/latrace.d/stdlib.conf,$(confdir),644)
	$(call install,etc/latrace.d/string.conf,$(confdir),644)
	$(call install,etc/latrace.d/ctype.conf,$(confdir),644)
	$(call install,etc/latrace.d/ncurses.conf,$(confdir),644)
	$(call install,etc/latrace.d/stdio.conf,$(confdir),644)
	$(call install,etc/latrace.d/dirent.conf,$(confdir),644)
	$(call install,etc/latrace.d/unistd.conf,$(confdir),644)
	$(call install,etc/latrace.d/libintl.conf,$(confdir),644)
	$(call install,etc/latrace.d/dlfcn.conf,$(confdir),644)
	$(call install,etc/latrace.d/fcntl.conf,$(confdir),644)
	$(call install,etc/latrace.d/getopt.conf,$(confdir),644)
	$(call install,etc/latrace.d/signal.conf,$(confdir),644)
	$(call install,etc/latrace.d/ioctl.conf,$(confdir),644)
	$(call install,etc/latrace.d/socket.conf,$(confdir),644)
	$(call install,etc/latrace.d/netdb.conf,$(confdir),644)
	$(call install,etc/latrace.d/stat.conf,$(confdir),644)
	$(call install,etc/latrace.d/wait.conf,$(confdir),644)
	$(call install,etc/latrace.d/utmp.conf,$(confdir),644)
	$(call install,etc/latrace.d/time.conf,$(confdir),644)
	$(call install,etc/latrace.d/termios.conf,$(confdir),644)
	$(call install,etc/latrace.d/term.conf,$(confdir),644)
	$(call install,etc/latrace.d/syslog.conf,$(confdir),644)
	$(call install,etc/latrace.d/pwd.conf,$(confdir),644)
	$(call install,etc/latrace.d/libio.conf,$(confdir),644)
	$(call install,etc/latrace.d/locale.conf,$(confdir),644)
	$(call install,etc/latrace.d/pthread.conf,$(confdir),644)
	$(call install,etc/latrace.d/resource.conf,$(confdir),644)
	$(call install,etc/latrace.d/mman.conf,$(confdir),644)
ifeq ($(CONFIG_SYSDEP_DIR),x86_64)
	@mkdir -p $(ROOTDIR)/$(confarch)
	$(call install,etc/sysdeps/$(CONFIG_SYSDEP_DIR)/latrace.conf,$(confarch),644)
	$(call install,etc/sysdeps/$(CONFIG_SYSDEP_DIR)/syscall.conf,$(confarch),644)
endif


ifeq (,$(filter clean mrproper,$(MAKECMDGOALS)))
-include deps.make
endif


# main building schema
# Module subdir (src,libsf) are supposed to fill PROGRAMS and
# OBJS variables, and rule to link the module. The top makefile
# will do the rest.

PROGRAMS=
OBJS=

include src/Makefile
include doc/Makefile

INCLUDES= -Isrc -Isrc/sysdeps/$(CONFIG_SYSDEP_DIR)
ALL_CFLAGS=$(CPPFLAGS) $(CFLAGS) -O2 -fPIC -Wall $(INCLUDES) -D_GNU_SOURCE


%.o: %.c LATRACE-CFLAGS
	$(QUIET_CC)$(CC) -o $*.o -c $(ALL_CFLAGS) $<

.SECONDARY:

%.c: %.l LATRACE-CFLAGS
	$(QUIET_LEX)$(LEX) -t $< > $(basename $<).c

%.c: %.y LATRACE-CFLAGS
	$(QUIET_YACC)$(YACC) -v $< -d -o $(basename $<).c


all:: $(PROGRAMS) LATRACE-CFLAGS

clean::
	$(call remove, $(OBJS) $(PROGRAMS))
	$(call remove, src/args-bison.c src/args-flex.c src/args-bison.h src/args-bison.output)
	$(call remove, lib bin share deps.make latrace-$(LT_VER))

mrproper::
	git clean -xdf

snapshot:
	@$(MAKE) $(PRINT_DIR) package PKG_VER=$(LT_VER)-`date "+%m%d%Y"`

release:
	@$(MAKE) $(PRINT_DIR) package PKG_VER=$(LT_VER)

package:
	$(QUIET_PKG)rm -f latrace-$(PKG_VER); ln -s . latrace-$(PKG_VER); \
	echo "latrace-$(PKG_VER)"; \
	for i in `git ls-tree -r --name-only HEAD`; do \
		echo "latrace-$(PKG_VER)/$$i"; \
	done | tar cjvf latrace-$(PKG_VER).tar.bz2 -T- > /dev/null 2>&1


# dependencies
# The gcc -M depedencies generation needs to repaired to include
# subdirectory name within the target.. at least I haven't find any
# gcc option to do that.
#  - no dependency for flex and bison definitions
DEPS_OBJS=$(filter-out src/args-flex.o src/args-bison.o,$(OBJS))

deps.make:
	$(QUIET_DEP)$(RM) -f deps.make; \
	(for obj in $(DEPS_OBJS); do \
	src=`echo $$obj | sed "s/\.o/.c/"`; \
	$(CC) $(ALL_CFLAGS) -M -MT$$obj $$src;  \
	done) > deps.make

# utilities
tags:
	$(QUIET_GEN)$(RM) -f tags; \
	$(FIND) . -name '*.[hc]' -print | xargs ctags -a

cscope:
	$(QUIET_GEN)$(RM) -f cscope*; \
	$(FIND) . -name '*.[hc]' -print > cscope.files; \
	cscope -b -icscope.files

# detect prefix and cflags changes
TRACK_CFLAGS = $(subst ','\'',$(ALL_CFLAGS)):\
             $(prefix):$(exec_prefix):$(bindir):$(libdir):$(sysconfdir) #'

LATRACE-CFLAGS: .FORCE-LATRACE-CFLAGS
	@FLAGS='$(TRACK_CFLAGS)'; \
	if test x"$$FLAGS" != x"`cat LATRACE-CFLAGS 2>/dev/null`" ; then \
		echo "$$FLAGS" >LATRACE-CFLAGS; \
	fi
