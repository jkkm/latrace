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


# libltaudit.so 
AUDIT_BIN=libltaudit.so.$(LT_VER)
AUDIT_LDFLAGS="-Wl,-init=audit_init" "-Wl,-soname,$(AUDIT_BIN)" -fPIC -shared
AUDIT_LIBS=$(LIBS) $(LEXLIB)

AUDIT_OBJS=\
	src/audit.o \
	src/audit-init.o \
	src/fifo.o \
	src/args-bison.o \
	src/args-flex.o \
	src/args.o \
	src/output.o \
	src/objsearch.o \
	src/sysdeps/$(CONFIG_SYSDEP_DIR)/stack.o

OBJS+=$(AUDIT_OBJS)
PROGRAMS+= $(AUDIT_BIN)

$(AUDIT_BIN): $(AUDIT_OBJS)
	$(QUIET_LD)$(CC) $(AUDIT_LDFLAGS) -o $@ $(AUDIT_OBJS) $(AUDIT_LIBS)

install::
	$(call install,$(AUDIT_BIN),$(libdir),644)

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
	src/output.o

OBJS+=$(LATRACE_OBJS)
PROGRAMS+=$(LATRACE_BIN)
CPPFLAGS+=-DCONFIG_LIBDIR=\"$(libdir)\"
CPPFLAGS+=-DLT_ARGS_DEF_DIR=\"$(confdir)\"
CPPFLAGS+=-DLT_ARGS_DEF_CONF=\"$(sysconfdir)/latrace.conf\"

$(LATRACE_BIN): $(LATRACE_OBJS)
	$(QUIET_LD)$(CC) $(LDFLAGS) -o $@ $(LATRACE_OBJS) $(LATRACE_LIBS) $(LATRACE_LIB)

install::
	$(call install,$(LATRACE_BIN),$(bindir),755)
	$(call link,$(LATRACE_BIN),$(LATRACE_CTL),$(bindir))

