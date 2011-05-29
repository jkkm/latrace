#  Copyright (C) 2008, 2009, 2010 Jiri Olsa <olsajiri@gmail.com>
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


# libltaudit.so 
AUDIT_BIN=libltaudit.so.$(CONFIG_VERSION)
AUDIT_LDFLAGS="-Wl,-init=audit_init" "-Wl,-soname,$(AUDIT_BIN)" -fPIC -shared $(LDFLAGS)
AUDIT_LIBS=$(LIBS) $(LEXLIB)

AUDIT_OBJS=\
	src/audit.o \
	src/audit-init.o \
	src/fifo.o \
	src/output.o \
	src/objsearch.o \
	src/stack.o \
	src/symbol.o \
	src/lib-include.o

ifeq ($(CONFIG_ARCH_HAVE_ARGS),y)
AUDIT_OBJS+=\
	src/args-bison.o \
	src/args-flex.o \
	src/args.o \
	src/sysdeps/$(CONFIG_SYSDEP_DIR)/stack.o
endif

OBJS+=$(AUDIT_OBJS)
PROGRAMS+= $(AUDIT_BIN)
# no dependency for flex and bison definitions
OBJS_DEPS_OMIT+=\
	src/args-flex.o \
	src/args-bison.o

$(AUDIT_BIN): $(AUDIT_OBJS)
	$(QUIET_LD)$(CC) $(CFLAGS) $(AUDIT_LDFLAGS) -o $@ $^ $(AUDIT_LIBS)

install::
	$(call install,$(AUDIT_BIN),$(libdir),755)

# latrace binary
LATRACE_BIN=latrace
LATRACE_CTL=latrace-ctl
LATRACE_LIB= $(LIBS)

LATRACE_OBJS=\
	src/latrace.o \
	src/ctl.o \
	src/config.o \
	src/run.o \
	src/stats.o \
	src/fifo.o \
	src/thread.o \
	src/output.o \
	src/config-bison.o \
	src/config-flex.o \
	src/lib-include.o \
	src/tty.o

OBJS+=$(LATRACE_OBJS)
PROGRAMS+=$(LATRACE_BIN)
CPPFLAGS+=-DCONFIG_LIBDIR=\"$(libdir)\"
CPPFLAGS+=-DLT_CONF_DIR=\"$(sysconfdir)/latrace.d\"
CPPFLAGS+=-DLT_CONF_HEADERS_DIR=\"$(sysconfdir)/latrace.d/headers\"
CPPFLAGS+=-DLT_CONF_HEADERS_FILE=\"$(sysconfdir)/latrace.d/headers/latrace.h\"
# no dependency for flex and bison definitions
OBJS_DEPS_OMIT+=\
	src/config-bison.o \
	src/config-flex.o

$(LATRACE_BIN): $(LATRACE_OBJS)
	$(QUIET_LD)$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LATRACE_LIB)

install::
	$(call install,$(LATRACE_BIN),$(bindir),755)
	$(call link,$(LATRACE_BIN),$(LATRACE_CTL),$(bindir))

clean::
	$(call remove, src/config-bison.[ch] src/config-flex.c)
	$(call remove, src/args-bison.[ch] src/args-flex.c)
	$(call remove, src/config-bison.output src/args-bison.output)
