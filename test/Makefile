
LDFLAGS+=-L.

lib-test-common.so: test/lib-test-common.o
	$(QUIET_LD)$(CC) $(CFLAGS) -fPIC -shared $(LDFLAGS) -o $@ $^

lib-test-args.so: test/lib-test-args.o
	$(QUIET_LD)$(CC) $(CFLAGS) -fPIC -shared $(LDFLAGS) -o $@ $^

lib-test-libsym1.so: test/lib-test-libsym1.o
	$(QUIET_LD)$(CC) $(CFLAGS) -fPIC -shared $(LDFLAGS) -o $@ $^

lib-test-libsym2.so: test/lib-test-libsym2.o
	$(QUIET_LD)$(CC) $(CFLAGS) -fPIC -shared $(LDFLAGS) -o $@ $^

lib-test-libsym3.so: test/lib-test-libsym3.o
	$(QUIET_LD)$(CC) $(CFLAGS) -fPIC -shared $(LDFLAGS) -o $@ $^

test-common: lib-test-common.so test/test-lib.o test/test-common.o
	$(QUIET_LD)$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ lib-test-common.so

test-args: lib-test-args.so test/test-lib.o test/test-args.o test/sysdeps/$(CONFIG_SYSDEP_DIR)/test-args.o
	$(QUIET_LD)$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ lib-test-args.so

test-kill: test/test-kill.o
	$(QUIET_LD)$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test-libsym: lib-test-libsym1.so lib-test-libsym2.so lib-test-libsym3.so test/test-libsym.o test/test-lib.o
	$(QUIET_LD)$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ lib-test-libsym1.so lib-test-libsym2.so lib-test-libsym3.so

OBJS+=test/test-lib.o

PROGRAMS+=\
	lib-test-args.so \
	test-args
OBJS+=\
	test/lib-test-args.o \
	test/test-args.o \
	test/sysdeps/$(CONFIG_SYSDEP_DIR)/test-args.o

PROGRAMS+=\
	lib-test-common.so \
	test-common
OBJS+=\
	test/lib-test-common.o \
	test/test-common.o

PROGRAMS+=\
	lib-test-libsym1.so \
	lib-test-libsym2.so \
	lib-test-libsym3.so \
	test-libsym
OBJS+=\
	test/test-libsym.o \
	test/lib-test-libsym1.o

PROGRAMS+=\
	test-kill
OBJS+=\
	test/test-kill.o

