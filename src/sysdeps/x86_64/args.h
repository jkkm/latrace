#ifndef ARGS_H
#define ARGS_H

#define LT_ARGS_ARCH_CONF 1

struct lt_config_shared;

static inline
char* lt_args_arch_conf(struct lt_config_shared *cfg)
{
	static char buf[LT_MAXFILE];

	sprintf(buf, "%s/%s",
		LT_ARGS_DEF_DIR,
		"sysdeps/x86_64/latrace.conf");

	return buf;
}

#endif /* ARGS_H */
