/*
  Copyright (C) 2008, 2009, 2010 Jiri Olsa <olsajiri@gmail.com>

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
#include <signal.h>

#include "config.h"

extern char **environ;
extern struct timeval tv_program_start;
extern struct timeval tv_program_stop;

struct lt_process_args {
	pid_t pid;
	char *dir;
	int fd_notify;
	int fd_tty_master;
};

static volatile int exit_flag = 0;

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

static int get_config_dir(char *buf, int len)
{
	snprintf(buf, len , "%s-XXXXXX", CONFIG_LT_CONFIG);
	if (NULL == mkdtemp(buf)) {
		perror("mkdtemp failed");
		return -1;
	}

	return 0;
}

static int get_fifo(struct lt_config_app *cfg, int fd_notify,
			char *dir, pid_t *pid)
{
	unsigned char buf[1000];
	struct inotify_event *event = (struct inotify_event*) buf;

	if (-1 == read(fd_notify, event, 1000)) {
		perror("read notify failed");
		return -1;
	}

	sscanf(event->name, "fifo-%d", pid);

	PRINT_VERBOSE(cfg, 1, "thread id %d, got fifo: %s\n",
			*pid, event->name);

	return lt_fifo_open(cfg, dir, event->name);
}

static int process_fifo(struct lt_config_app *cfg, struct lt_thread *t)
{
	static char buf[FIFO_MSG_MAXLEN];
	struct lt_fifo_mbase *mbase = (struct lt_fifo_mbase*) buf;
	struct lt_fifo_msym *msym = (struct lt_fifo_msym*) buf;

	if (-1 == lt_fifo_recv(cfg, t, mbase, FIFO_MSG_MAXLEN))
		return -1;

	if ((FIFO_MSG_TYPE_ENTRY != mbase->type) &&
	    (FIFO_MSG_TYPE_EXIT  != mbase->type)) {
		PRINT_VERBOSE(cfg, 1, "unexpected message type %d\n",
				mbase->type);
		return -1;
	}

	if (lt_sh(cfg, counts))
		return lt_stats_sym(cfg, t, msym);

	if (FIFO_MSG_TYPE_ENTRY == msym->h.type) {

		t->indent_depth++;
		lt_out_entry(cfg->sh, &msym->h.tv, msym->h.tid,
				t->indent_depth,
				msym->data + msym->sym,
				msym->data + msym->lib,
				msym->data + msym->arg,
				msym->data + msym->argd);

	} else if (FIFO_MSG_TYPE_EXIT == msym->h.type) {

		lt_out_exit(cfg->sh, &msym->h.tv, msym->h.tid,
				t->indent_depth,
				msym->data + msym->sym,
				msym->data + msym->lib,
				msym->data + msym->arg,
				msym->data + msym->argd);

		t->indent_depth--;
	}

	return 0;
}

static int process(struct lt_config_app *cfg, struct lt_process_args *pa)
{
	int finish = 0, getin = 1, status;
	fd_set cfg_set, wrk_set;
	int fd_notify = pa->fd_notify;
	int fd_tty_master = pa->fd_tty_master;
	int max_fd = 0;
#define MAX(a,b) ((a) < (b) ? (b) : (a))

	FD_ZERO(&cfg_set);

	/* We are here either because of the
	 *   - pipe option
	 *   - or tty output option */

	if (lt_sh(cfg, pipe)) {
		PRINT_VERBOSE(cfg, 1, "doing pipe\n");
		FD_SET(fd_notify, &cfg_set);
		max_fd = fd_notify;
	}

	if (cfg->output_tty) {
		PRINT_VERBOSE(cfg, 1, "doing output tty\n");
		FD_SET(fd_tty_master, &cfg_set);
		max_fd = MAX(fd_notify, fd_tty_master);
	}

	while((waitpid(pa->pid, &status, WNOHANG) == 0) ||
		/* let all the thread fifo close */
		(finish) || 
		/* Get inside at least once, in case the traced program 
		   finished before we got here.  Another case is if there's 
		   an input on notify descriptor, we want to try another 
		   select to be sure we dont miss any other event. */
		(getin))
	{

#define CONTINUE_ZERO_RET() { \
if (!ret) \
	continue; \
if (ret < 0) \
	printf("latrace failed: select bug ret %d\n", ret); \
}

		struct timeval tv = { 0, 100 };
		struct lt_thread *t;
		int ret;

		/* we got a signal, there's nothing to wait for.. */
		if (exit_flag)
			break;

		getin = 0;
		wrk_set = cfg_set;

		ret = select(max_fd + 1, &wrk_set, NULL, NULL, &tv);
		if (-1 == ret) {
			if (errno != EINTR)
				perror("select failed");
			return -1;
		}

		CONTINUE_ZERO_RET();

		/* process notify */
		if (FD_ISSET(fd_notify, &wrk_set)) {
			int fd;
			pid_t pid;

			/* try to get any event at least once again */
			getin = 1;

			if (-1 == (fd = get_fifo(cfg, fd_notify, pa->dir, &pid)))
				continue;

			if (NULL == (t = lt_thread_add(cfg, fd, pid))) {
				close(fd);
				continue;
			}

			finish++;

			FD_SET(fd, &cfg_set);
			max_fd = MAX(max_fd, fd);
			ret--;
		}

		CONTINUE_ZERO_RET();

		if (cfg->output_tty &&
		    FD_ISSET(fd_tty_master, &wrk_set)) {
			if (tty_process(cfg, fd_tty_master))
				FD_CLR(fd_tty_master, &cfg_set);
			ret--;
		}

		CONTINUE_ZERO_RET();

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
#undef CONTINUE_ZERO_RET
#undef MAX
	}

	return status;
}

static int remove_dir(struct lt_config_app *cfg, char *name)
{
	DIR *dir;
	struct dirent *d;

	PRINT_VERBOSE(cfg, 1, "removing %s\n", name);

	if (NULL == (dir = opendir(name))) {
		perror("opendir failed");
		return -1;
	}

	while((d = readdir(dir))) {
		char file[LT_MAXFILE];
		struct stat st;

		if (!strcmp(".", d->d_name) ||
		    (!strcmp("..", d->d_name)))
			continue;

		sprintf(file, "%s/%s", name, d->d_name);
		if (stat(file, &st)) {
			perror("stat failed");
			continue;
		}

		if (S_ISDIR(st.st_mode)) {
			remove_dir(cfg, file);
			continue;
		}

		if (unlink(file))
			perror("unlink failed");
	}

	closedir(dir);

	if (-1 == remove(name)) {
		perror("remove failed");
		return -1;
	}

	return 0;
}

static void sig_term_handler(int sig)
{
	exit_flag = 1;
}

static int setup_signals(void)
{
	struct sigaction act;

	bzero(&act, sizeof(act));
	act.sa_handler = sig_term_handler;

        if (sigaction(SIGTERM, &act, NULL) ||
	    sigaction(SIGINT,  &act, NULL))
		return -1;

	return 0;
}

static int run_setup(struct lt_config_app *cfg,
		     struct lt_process_args *pa)
{
	char str_cfg[LT_MAXFILE];

	sprintf(str_cfg, "%s/config", pa->dir);
	if (store_config(cfg, str_cfg))
		return -1;

	/* new thread notification descriptor */
	if (lt_sh(cfg, pipe) &&
	   (-1 == (pa->fd_notify = lt_fifo_notify_fd(cfg, pa->dir))))
		return -1;

	/* tty master descriptor */
	if (cfg->output_tty &&
	    (-1 == (pa->fd_tty_master = tty_master(cfg))))
		return -1;

	return 0;
}

static void run_cleanup(struct lt_config_app *cfg,
			struct lt_process_args *pa)
{
	if (lt_sh(cfg, pipe))
		close(pa->fd_notify);

	if (cfg->output_tty)
		tty_close(cfg);

	remove_dir(cfg, pa->dir);
}

static int run_child(struct lt_config_app *cfg,
		     struct lt_process_args *pa)
{

	if (0 == (pa->pid = fork())) {
		char str_audit[100];

		sprintf(str_audit, "%s/libltaudit.so.%s", CONFIG_LIBDIR,
			CONFIG_VERSION);

		setenv("LD_AUDIT", str_audit, 1);
		setenv("LT_DIR", pa->dir, 1);

		if (cfg->output_tty &&
		    tty_init(cfg, pa->fd_tty_master))
			return -1;

		PRINT_VERBOSE(cfg, 1, "executing %s\n", cfg->prog);

		if (-1 == execvp(cfg->prog, cfg->arg)) {
			int err = errno;
			tty_restore(cfg);
			printf("execve failed for \"%s\" : %s\n", 
				cfg->prog, strerror(err));
		}
		exit(-1);

	} else if (pa->pid < 0) {
		perror("fork failed");
		return -1;
	}

	return 0;
}

static int kill_child(struct lt_config_app *cfg,
		      struct lt_process_args *pa)
{
	int status;
	int cnt = 5 * 1000;
	int nomercy = 1;

	kill(pa->pid, SIGTERM);

	/* be gracious, 5 seconds should be enough
	 * for everyone ;) */
	while(cnt--) {
		if (waitpid(pa->pid, &status, WNOHANG)) {
			nomercy = 0;
			break;
		}
		usleep(1000);
	}

	if (nomercy) {
		kill(pa->pid, SIGKILL);
		waitpid(pa->pid, &status, 0);
	}

	return status;
}

int lt_run(struct lt_config_app *cfg)
{
	char ret = (char) -1;
	char str_dir[LT_MAXFILE];
	struct lt_process_args pa = { .dir = str_dir };

	if (setup_signals())
		return -1;

	if (get_config_dir(pa.dir, LT_MAXFILE))
		return -1;

	do {
		int status;

		if (run_setup(cfg, &pa))
			break;

		gettimeofday(&tv_program_start, NULL);

		if (run_child(cfg, &pa))
			break;

		if (lt_sh(cfg, pipe) || cfg->output_tty)
			status = process(cfg, &pa);
		else
			waitpid(pa.pid, &status, 0);

		gettimeofday(&tv_program_stop, NULL);

		if (exit_flag) {
			printf("\nlatrace interrupted, killing child (pid %d)\n",
				pa.pid);
			status = kill_child(cfg, &pa);
		}

		printf("\n%s finished - ", cfg->prog);

		if (WIFEXITED(status)) {
			printf("exited, status=%d\n", WEXITSTATUS(status));
		} else if (WIFSIGNALED(status)) {
			printf("killed by signal %d\n", WTERMSIG(status));
		}

		ret = WEXITSTATUS(status);
	} while(0);

	run_cleanup(cfg, &pa);
	return ret;
}
