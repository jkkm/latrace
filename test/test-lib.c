
#include <stdio.h>
#include <config.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <regex.h>
#include <string.h>
#include <strings.h>

#include <test/test-lib.h>

static struct lt_config_shared *get_config(char *config_dir)
{
	struct lt_config_shared* sh;
	int page = sysconf(_SC_PAGE_SIZE);
	int fd, len;
	char config_file[LT_MAXFILE];

	snprintf(config_file, LT_MAXFILE, "%s/config", config_dir);

	if (-1 == (fd = open(config_file, O_RDWR))) {
		perror("open failed");
		return NULL;
	}

	/* align the shared config length */
	len = sizeof(struct lt_config_shared);
	len = (len + page) & ~(page - 1);

	sh = mmap(NULL, len,
		PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0);

	if ((void *) -1 == sh) {
		perror("mmap failed");
		return NULL;
	}

	sh->sh = sh;
	return sh;
}

static int fout_init(struct lt_config_shared *sh, char *config_dir)
{
	char output_file[LT_MAXFILE];
	FILE *file;

	snprintf(output_file, LT_MAXFILE, "%s/test-output", config_dir);

	file = fopen(output_file, "w+");
	if (!file) {
		perror("fopen failed");
		return -1;
	}

	sh->fout = file;
	return 0;
}

int fout_read(struct lt_config_shared *sh, char *buf, int size)
{
	FILE *file = sh->fout;

	/* XXX ugly, but I could not get work pipe/fifo with
	 * streams..  maybe to get rid of streams completely
	 * would be the solution */
	bzero(buf, size);
	rewind(file);
	fread(buf, size, 1, file);
	rewind(file);
	ftruncate(fileno(file), 0);
	return strlen(buf);
}

struct lt_config_shared *config_init(void)
{
	struct lt_config_shared *sh;
	char *config_dir;

	config_dir = getenv("LT_DIR");
	if (!config_dir) {
		printf("failed: could not get config dir\n");
		return NULL;
	}

	sh = get_config(config_dir);
	if (!sh)
		return NULL;

	if (fout_init(sh, config_dir))
		return NULL;

	sh->pipe = 0;
	return sh;
}

void config_clear(struct lt_config_shared *sh)
{
	sh->args_enabled = 0;
	sh->args_detailed = 0;
	sh->disabled = 1;
	sh->verbose = 0;
	sh->timestamp = 0;
	sh->debug = 0;
	sh->indent_sym = 0;
	sh->indent_size = 0;
	sh->braces = 0;
	sh->demangle = 0;
	sh->counts = 0;
	sh->pipe = 0;
	sh->hide_tid = 0;
	sh->not_follow_exec = 0;
	sh->not_follow_fork = 0;
	sh->framesize_check = 0;
	sh->framesize = 1000;
	sh->pid = 0;
}

static void re_err(regex_t *re, int errcode)
{
	char ebuf[BUFSIZE];
	int ret;

	ret = regerror(errcode, re, ebuf, BUFSIZE);
	if (!ret)
		return;

	printf("regex failed: %s\n", ebuf);
}

int re_test(char *line, struct re_test_data *data, int cnt)
{
	int i;

	for(i = 0; i < cnt; i++) {
		int ret;
		struct re_test_data *d = &data[i];
		regmatch_t m[1];
		regex_t re;
		unsigned long val;

		memset(&re, 0x0, sizeof(re));

		ret = regcomp(&re,
			d->pat,
			REG_EXTENDED);
		if (ret) {
			re_err(&re, ret);
			return i;
		}

		ret = regexec(&re, line, 1, m, 0);
		if (ret == REG_NOMATCH) {
			printf("failed: did not match\n");
			return i;
		}

		switch(d->type) {
		case RE_TEST_TYPE_STR:
			/* start offset check */
			if (d->so != RE_TEST_OFF_UNDEF) {
				if (d->so != m[0].rm_so) {
					printf("failed: so mismatch %d <> %d\n",
						d->so, m[0].rm_so);
					return i;
				}
			}

			/* end offset check */
			if (d->eo != RE_TEST_OFF_UNDEF) {
				if (d->eo != m[0].rm_eo) {
					printf("failed: eo mismatch %d <> %d\n",
						d->eo, m[0].rm_eo);
					return i;
				}
			}
			break;
		case RE_TEST_TYPE_INT:
		case RE_TEST_TYPE_PID:
			val = strtoul(&line[m[0].rm_so], NULL, 10);
			if (val != getpid())
				return i;
			break;
		}

		/* TODO ??? line += m[0].rm_eo; */
	}

	return RE_TEST_OK;
}

