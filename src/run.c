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


#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#include "config.h"

extern char **environ;
extern struct timeval tv_program_start;
extern struct timeval tv_program_stop;


static int store_config(struct lt_config_app *cfg, char *file)
{
	int fd;
	int mode = S_IRUSR;

	if (lt_sh(cfg, ctl_config)) {
		printf("controled config: %s\n", file);
		mode |= S_IWUSR;
	}

	if (-1 == (fd = open(file, O_CREAT | O_TRUNC | O_RDWR, mode))) {
		perror("open failed");
		return -1;
	}

        if (-1 == write(fd, cfg->sh, sizeof(*cfg->sh))) {
                perror("read failed");
                return -1;
        }

	close(fd);

	return 0;
}

static int set_dir_notify(char *dir)
{
	int fd;

	if (-1 == (fd = inotify_init())) {
		perror("inotify_init failed");
		return -1;
	}

	if (-1 == inotify_add_watch(fd, dir, IN_CREATE)) {
		perror("inotify_add_watch failed");
		return -1;
	}

	return fd;
}

static int get_fifo_dir(char *buf, int len)
{
	snprintf(buf, len , "%s-XXXXXX", CONFIG_LT_CONFIG);
	if (NULL == mkdtemp(buf)) {
		perror("mkdtemp failed");
		return -1;
	}

	return 0;
}

static int get_fifo(struct lt_config_app *cfg, int notify_fd, 
			char *dir, pid_t *pid)
{
	char str_fifo[LT_MAXFILE];
	unsigned char buf[1000];
	struct inotify_event *event = (struct inotify_event*) buf;

	if (-1 == read(notify_fd, event, 1000)) {
		perror("read notify failed");
		return -1;
	}

	sscanf(event->name, "fifo-%d", pid);
	sprintf(str_fifo, "%s/%s", dir, event->name);

	PRINT_VERBOSE(cfg, 1, "thread id %d, got fifo: %s\n",
			*pid, str_fifo);

	return lt_fifo_open(cfg, str_fifo);
}

static int process_fifo(struct lt_config_app *cfg, struct lt_thread *t)
{
	static char buf[FIFO_MSG_MAXLEN];

	struct lt_fifo_mbase *mbase = (struct lt_fifo_mbase*) buf;

	if (-1 == lt_fifo_recv(cfg, t, mbase, FIFO_MSG_MAXLEN))
		return -1;

	if ((FIFO_MSG_TYPE_ENTRY != mbase->type) &&
	    (FIFO_MSG_TYPE_EXIT  != mbase->type)) {
		PRINT_VERBOSE(cfg, 1, "unexpected message type %d\n",
				mbase->type);
		return -1;
	}

	struct lt_fifo_msym *msym = (struct lt_fifo_msym*) buf;

	if (lt_sh(cfg, counts))
		return lt_stats_sym(cfg, t, msym);

	if (FIFO_MSG_TYPE_ENTRY == msym->h.type) {

		lt_sh(cfg, indent_depth)++;
		lt_out_entry(cfg->sh, &msym->h.tv,
				msym->data + msym->sym,
				msym->data + msym->lib,
				msym->data + msym->arg,
				msym->data + msym->argd);

	} else if (FIFO_MSG_TYPE_EXIT == msym->h.type) {

		lt_out_exit(cfg->sh, &msym->h.tv,
				msym->data + msym->sym,
				msym->data + msym->lib,
				msym->data + msym->arg,
				msym->data + msym->argd);

		lt_sh(cfg, indent_depth)--;
	}

	return 0;
}

static int process(struct lt_config_app *cfg, pid_t pid, char *dir, int notify_fd)
{
	int finish = 0, getin = 1;
	int max_fd = notify_fd;
	fd_set cfg_set, wrk_set;
	int status;
	
	FD_ZERO(&cfg_set);
	FD_SET(notify_fd, &cfg_set);

	while(!waitpid(pid, &status, WNOHANG) || 
		/* let all the thread fifo close */
		(finish) || 
		/* Get inside at least once, in case the traced program 
		   finished before we got here.  Another case is if there's 
		   an input on notify descriptor, we want to try another 
		   select to be sure we dont miss any other event. */
		(getin))
	{

		struct timeval tv = { 0, 100 };
		struct lt_thread *t;
		int ret;

		getin = 0;

		wrk_set = cfg_set;
		if (-1 == (ret = select(max_fd + 1, &wrk_set, NULL, NULL, &tv))) {
			perror("select failed");
			return -1;
		}

		if (!ret)
			continue;

		/* process notify */
		if (FD_ISSET(notify_fd, &wrk_set)) {
			int fd;
			pid_t pid;

			/* try to get any event at least once again */
			getin = 1;

			if (-1 == (fd = get_fifo(cfg, notify_fd, dir, &pid)))
				continue;

			if (NULL == (t = lt_thread_add(cfg, fd, pid))) {
				close(fd);
				continue;
			}

			finish++;

			FD_SET(fd, &cfg_set);
			max_fd = (max_fd < fd ? fd : max_fd);
			ret--;
		}

		/* process fifo */
		for(t = cfg->threads; t ; t = t->next) {
			if (FD_ISSET(t->fifo_fd, &wrk_set)) {
				if (-1 == process_fifo(cfg, t)) {
					FD_CLR(t->fifo_fd, &cfg_set);
					gettimeofday(&t->tv_stop, NULL);
					finish--;
				}
				ret--;
			}
		}
	}

	return status;
}

static int remove_dir(struct lt_config_app *cfg, char *name)
{
	DIR *dir;
	struct dirent *d;

	if (NULL == (dir = opendir(name))) {
		perror("opendir failed");
		return -1;
	}

	while((d = readdir(dir))) {
		char file[LT_MAXFILE];

		if (!strcmp(".", d->d_name) ||
		    (!strcmp("..", d->d_name)))
			continue;

		sprintf(file, "%s/%s", name, d->d_name);
		if (-1 == (unlink(file)))
			perror("unlink failed");
	}

	closedir(dir);

	if (-1 == remove(name)) {
		perror("remove failed");
		return -1;
	}

	return 0;
}

int lt_run(struct lt_config_app *cfg)
{
	pid_t child_pid;
	char str_dir[LT_MAXFILE];
	char str_cfg[LT_MAXFILE];
	int status;
	int notify_fd = -1;

	if (-1 == get_fifo_dir(str_dir, LT_MAXFILE))
		return -1;

	sprintf(str_cfg, "%s/config", str_dir);
	if (-1 == store_config(cfg, str_cfg)) 
		return -1;

	if (lt_sh(cfg, pipe) &&
	   (-1 == (notify_fd = set_dir_notify(str_dir))))
		return -1;

	gettimeofday(&tv_program_start, NULL);

	if (0 == (child_pid = fork())) {
		char str_audit[100];

		sprintf(str_audit, "%s/libltaudit.so.%s", CONFIG_LIBDIR, LT_VER);
		setenv("LD_AUDIT", str_audit, 1);
		setenv("LT_DIR", str_dir, 1);

		PRINT_VERBOSE(cfg, 1, "executing %s\n", cfg->prog);

		if (-1 == execvp(cfg->prog, cfg->arg)) {
			printf("execve failed for \"%s\" : %s\n", 
				cfg->prog, strerror(errno));
			return -1;
		}
	} else if (child_pid < 0) {
		perror("fork failed");
		return -1;
	}

	if (lt_sh(cfg, pipe))
		status = process(cfg, child_pid, str_dir, notify_fd);
	else
		waitpid(child_pid, &status, 0);

	gettimeofday(&tv_program_stop, NULL);

	printf("\n%s finished - ", cfg->prog);

	if (WIFEXITED(status)) {
		printf("exited, status=%d\n", WEXITSTATUS(status));
	} else if (WIFSIGNALED(status)) {
		printf("killed by signal %d\n", WTERMSIG(status));
	}

	remove_dir(cfg, str_dir);
	return 0;
}
