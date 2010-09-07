
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "config.h"

static __thread void *stack_start = NULL;
static __thread void *stack_end   = NULL;

static unsigned long get_stack_size(struct lt_config_audit *cfg)
{
	static unsigned long stack_size = 0;
	static struct rlimit rlim;

	if (stack_size)
		return stack_size;

	if (getrlimit(RLIMIT_STACK, &rlim))
		return 0;

	PRINT_VERBOSE(cfg, 1, "stack size: %lx\n", rlim.rlim_cur);

	return stack_size = rlim.rlim_cur;
}

/* find and update current stack boundaries */
static int load_stack(struct lt_config_audit *cfg, void *sp)
{
	FILE *maps;
	char line[128];
	int found = 0;
	void *start, *end;

	maps = fopen("/proc/self/maps", "r");
	if (!maps) {
		PRINT_VERBOSE(cfg, 1, "failed to open maps: %s\n",
				strerror(errno));
		return -1;
	}


	while (!found && fgets(line, sizeof(line), maps)) {

		if (1 != sscanf(line, "%*p-%p", &end))
			continue;

		/* FIXME
		 * what if the new stack is not GROWSDOWN?
		 * let's get the limit via RLIMIT_STACK,
		 * which should be ok for most cases now..
		 *
		 * it does not feel safe/complete, but probably better
		 * than nothing
		 */
		start = end - get_stack_size(cfg);

		found = ((start < sp) && (sp < end));

		PRINT_VERBOSE(cfg, 1, "start %p, end %p, sp %p, in %d\n",
				start, end, sp, found);
	}

	fclose(maps);

	if (found) {
		stack_start = start;
		stack_end   = end;
	}

	return found ? 0 : -1;
}

/* check the current stack pointer and check its boundaries */
int lt_stack_framesize(struct lt_config_audit *cfg, La_regs *regs)
{
	void *sp_top, *sp_bottom;
	void *sp = (void*) regs->sp_reg;
	unsigned int framesize = lt_sh(cfg, framesize);

	if (!lt_sh(cfg, framesize_check))
		return framesize;

	/* got here first time, or we are out of bounds */
	if (!stack_start ||
	    (sp < stack_start || sp > stack_end)) {

		/* we are screeeeeewed */
		if (load_stack(cfg, sp))
			return framesize;
	}

	/* FIXME what about stacks growing up.. arm might */
	sp_top = sp + framesize;

	if (sp_top > stack_end) {
		framesize = stack_end - sp - 1;
		PRINT_VERBOSE(cfg, 1,
			"top reached, framesize changed to %lu\n",
			framesize);
	}

	sp_bottom = sp - framesize;

	if (sp_bottom < stack_start) {
		framesize = sp - stack_start;
		PRINT_VERBOSE(cfg, 1,
			"bottom reached, framesize changed to %lu\n",
			framesize);
	}

	return framesize;
}
