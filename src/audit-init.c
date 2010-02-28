/*
  Copyright (C) 2008, 2009 Jiri Olsa <olsajiri@gmail.com>

  This file is part of the latrace.

  The latrace is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The latrace is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the latrace (file COPYING).  If not, see 
  <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>

#include "config.h"

struct lt_config_audit cfg;

static int init_ctl_config(char *file)
{
	void *sh;
	int len;
	int page = sysconf(_SC_PAGE_SIZE);
	int fd;

	if (-1 == (fd = open(file, O_RDWR))) {
		perror("open failed");
		return -1;
	}

	/* align the shared config length */
	len = sizeof(struct lt_config_shared);
	len = (len + page) & ~(page - 1);

	sh = mmap(NULL, len,
		PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0);

	if ((void *) -1 == sh) {
		PRINT_VERBOSE(&cfg, 1,
			"mmap failed: %s\n", strerror(errno));
		return -1;
	}

	/* switching to the mmaped shared config */
	cfg.sh = sh;
	return 0;
}

static int read_config(char *dir)
{
	int fd;
	off_t len;
	char file[LT_MAXFILE];

	memset(&cfg, 0, sizeof(cfg));

	cfg.dir = dir;
	sprintf(file, "%s/config", dir);

	if (-1 == (fd = open(file, O_RDONLY))) {
		perror("open failed");
		return -1;
	}

	if (-1 == read(fd, &cfg.sh_storage, sizeof(cfg.sh_storage))) {
		perror("read failed");
		return -1;
	}

	if (-1 == (len = lseek(fd, 0, SEEK_END))) {
		perror("lseek failed");
		return -1;
	}

	if (len != sizeof(cfg.sh_storage)) {
		printf("config file size differs\n");
		return -1;
	}

	if (LT_MAGIC != cfg.sh_storage.magic) {
		printf("config file magic check failed\n");
		return -1;
	}

	cfg.sh = cfg.sh_storage.sh = &cfg.sh_storage;

	/*
	 * If we are not controled, we can close the file,
	 * since we read everything we needed.
	 */
	close(fd);

	if (lt_sh(&cfg, ctl_config) && init_ctl_config(file))
		printf("ctl config failed, carring on with standard\n");

	return 0;
}

static int get_names(struct lt_config_audit *cfg, char *names, char **ptr)
{
	char* s;
	int cnt = 0;

	PRINT_VERBOSE(cfg, 1, "names: [%s] max: %d\n",
			names, LT_NAMES_MAX);

	s = strchr(names, LT_NAMES_SEP);
	while(NULL != (s = strchr(names, LT_NAMES_SEP)) && (cnt < LT_NAMES_MAX)) {
		*s = 0x0;
		PRINT_VERBOSE(cfg, 1, "got: %s", names);
		ptr[cnt++] = names;
		names = ++s;
	}

	if (cnt) {
		ptr[cnt++] = names;
		PRINT_VERBOSE(cfg, 1, "got: %s\n", names);
	}

	if (!cnt && *names) {
		ptr[0] = names;
		cnt = 1;
		PRINT_VERBOSE(cfg, 1, "got: %s\n", names);
	}

	ptr[cnt] = NULL;

	if (!cnt)
		return -1;

	PRINT_VERBOSE(cfg, 1, "got %d entries\n", cnt);
	return cnt;
}

int audit_init(int argc, char **argv, char **env)
{
	if (-1 == read_config(getenv("LT_DIR")))
		return -1;

	/* -Aa */
	if (lt_sh(&cfg, args_enabled) && lt_args_init(cfg.sh))
		return -1;

	/* -t */
	if ((*lt_sh(&cfg, libs_to)) &&
	    (-1 == (cfg.libs_to_cnt = get_names(&cfg, lt_sh(&cfg, libs_to), cfg.libs_to)))) {
		printf("latrace failed to parse libs to\n");
		return -1;
	}

	/* -f */
	if ((*lt_sh(&cfg, libs_from)) &&
	    (-1 == (cfg.libs_from_cnt = get_names(&cfg, lt_sh(&cfg, libs_from), cfg.libs_from)))) {
		printf("latrace failed to parse libs from\n");
		return -1;
	}

	/* -l */
	if ((*lt_sh(&cfg, libs_both)) &&
	    (-1 == (cfg.libs_both_cnt = get_names(&cfg, lt_sh(&cfg, libs_both), cfg.libs_both)))) {
		printf("latrace failed to parse libs from\n");
		return -1;
	}

	/* -s */
	if ((*lt_sh(&cfg, symbols)) &&
	    (-1 == (cfg.symbols_cnt = get_names(&cfg, lt_sh(&cfg, symbols), cfg.symbols)))) {
		printf("latrace failed to parse symbols\n");
		return -1;
	}

	/* -b */
	if ((*lt_sh(&cfg, flow_below)) &&
	    (-1 == (cfg.flow_below_cnt = get_names(&cfg, lt_sh(&cfg, flow_below), cfg.flow_below)))) {
		printf("latrace failed to parse symbols in flow-below option\n");
		return -1;
	}

	/* -L */
	if (*lt_sh(&cfg, libs_subst)) {

		char *ptr[LT_NAMES_MAX];
		int cnt;

		if (-1 == (cnt = get_names(&cfg, lt_sh(&cfg, libs_subst), ptr))) {
			printf("latrace failed to parse input for subst option\n");
			return -1;
		}

		if (-1 == lt_objsearch_init(&cfg, ptr, cnt)) {
			printf("latrace failed to nitialize subst option\n");
			return -1;
		}
	}

	/* -o FIXME put fout out of the shared structure */
	lt_sh(&cfg, fout) = stdout;
	if ((*lt_sh(&cfg, output)) &&
	    (NULL == (lt_sh(&cfg, fout) = fopen(lt_sh(&cfg, output), "w")))) {
		printf("latrace failed to open output file %s\n", lt_sh(&cfg, output));
		return -1;
	}

	/* -E */
	if (lt_sh(&cfg, not_follow_exec))
		unsetenv("LD_AUDIT");

	/* -F */
	if (lt_sh(&cfg, not_follow_fork))
		lt_sh(&cfg, pid) = getpid();

	cfg.init_ok = 1;
	return 0;
}

void finalize(void) __attribute__((destructor));

void
finalize(void)
{
	if ((!lt_sh(&cfg, pipe)) && (*lt_sh(&cfg, output)))
		fclose(lt_sh(&cfg, fout));
}
