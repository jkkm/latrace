
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "config.h"

static __thread void *stack_start = NULL;
static __thread void *stack_end   = NULL;

static unsigned long get_stack_size(struct lt_config_audit *cfg)
{
	struct rlimit rlim;

	if (getrlimit(RLIMIT_STACK, &rlim))
		return 0;

	PRINT_VERBOSE(cfg, 1, "stack cur size: %lx, max: %lx\n",
			rlim.rlim_cur, rlim.rlim_max);

	return rlim.rlim_cur;
}

/* find and update current stack boundaries */
static int load_stack(struct lt_config_audit *cfg, void *sp)
{
	FILE *maps;
	char line[128];
	int found = 0;
	void *start, *end;
	unsigned long stack_size;

	maps = fopen("/proc/self/maps", "r");
	if (!maps) {
		PRINT_VERBOSE(cfg, 1, "failed to open maps: %s\n",
				strerror(errno));
		return -1;
	}

	stack_size = get_stack_size(cfg);
	/* XXX any sane idea what to do now ?*/
	if (!stack_size)
		return -1;

	while (!found && fgets(line, sizeof(line), maps)) {

		if (2 != sscanf(line, "%p-%p", &start, &end))
			continue;

		PRINT_VERBOSE(cfg, 1, "line  start %p, end %p\n", start, end);

		/* FIXME someone smart please figure out faster way,
		* (somehow within the sscanf call?)
		*
		* Also what if the new stack is not GROWSDOWN,
		* bounded by RLIMIT_STACK?
		*/
		if (strstr(line, "[stack]")) {
			void *new_start = end - get_stack_size(cfg);

			/* FIXME weird, need to investigate, looks like the stack
			* area could grow more than the stack limit (eg for xpdf)
			*
			* taking the lower value for now
			*/
			if (new_start < start)
				start = new_start;
		}

		found = ((start < sp) && (sp < end));

		PRINT_VERBOSE(cfg, 1, "final start %p, end %p, sp %p, in %d\n",
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

	/* FIXME what about stacks growing up.. */
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
