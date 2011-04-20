
LDFLAGS+=-L.

lib-test-common.so: test/lib-test-common.o
	$(QUIET_LD)$(CC) $(CFLAGS) -fPIC -shared $(LDFLAGS) -o $@ $^

lib-test-args.so: test/lib-test-args.o
	$(QUIET_LD)$(CC) $(CFLAGS) -fPIC -shared $(LDFLAGS) -o $@ $^

test-common: test/test-lib.o test/test-common.o
	$(QUIET_LD)$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ lib-test-common.so

test-args: test/test-lib.o test/test-args.o test/sysdeps/$(CONFIG_SYSDEP_DIR)/test-args.o
	$(QUIET_LD)$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ lib-test-args.so

OBJS+=test/test-lib.o

OBJS+=\
	test/lib-test-args.o \
	test/test-args.o \
	test/sysdeps/$(CONFIG_SYSDEP_DIR)/test-args.o

OBJS+=\
	test/lib-test-common.o \
	test/test-common.o

PROGRAMS+=lib-test-common.so test-common
PROGRAMS+=lib-test-args.so test-args
