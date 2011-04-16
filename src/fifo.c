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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/inotify.h>

#include "config.h"


static char *get_notify_dir(char *dir)
{
	static char notify_dir[LT_MAXFILE];
	static int initialized = 0;
	int s;

	if (initialized)
		return notify_dir;

	s = snprintf(notify_dir, LT_MAXFILE, "%s/fifo", dir);
	if (s >= LT_MAXFILE)
		return NULL;

	initialized = 1;
	return notify_dir;
}

int lt_fifo_create(struct lt_config_audit *cfg, char *dir)
{
	int fd;
	char fifo[100];

	sprintf(fifo, "%s/fifo-%d", get_notify_dir(dir),
		(pid_t) syscall(SYS_gettid));

	if (-1 == mkfifo(fifo, 0666)) {
		perror("mkfifo failed");
		return -1;
	}

	if (-1 == (fd = open(fifo, O_WRONLY))) {
		perror("open fifo failed");
		return -1;
	}

	return fd;
}

int lt_fifo_open(struct lt_config_app *cfg, char *dir, char *name)
{
	int fd;
	char str_fifo[LT_MAXFILE];

	snprintf(str_fifo, LT_MAXFILE, "%s/%s", get_notify_dir(dir), name);
	PRINT_VERBOSE(cfg, 1, "opening fifo: %s\n", str_fifo);

	if (-1 == (fd = open(str_fifo, O_RDONLY)))
		perror("open fifo failed");

	PRINT_VERBOSE(cfg, 1, "pipe openned fd: %d\n", fd);
	return fd;
}

int lt_fifo_notify_fd(struct lt_config_app *cfg, char *dir)
{
	int fd;
	char *notify_dir = get_notify_dir(dir);
	struct stat st;

	if (stat(notify_dir, &st)) {
		if (mkdir(notify_dir, S_IRWXU)) {
			perror("mkdir failed");
			return -1;
		}
		if (stat(notify_dir, &st)) {
			perror("stat failed");
			return -1;
		}
	}

	if (!S_ISDIR(st.st_mode)) {
		PRINT_VERBOSE(cfg, 1, "WTF '%s' is not a directory..\n",
				notify_dir);
		return -1;
	}

	if (-1 == (fd = inotify_init())) {
		perror("inotify_init failed");
		return -1;
	}

	if (-1 == inotify_add_watch(fd, notify_dir, IN_CREATE)) {
		perror("inotify_add_watch failed");
		return -1;
	}

	PRINT_VERBOSE(cfg, 1, "fifo notification set to: %s\n", notify_dir);
	return fd;
}

int lt_fifo_send(struct lt_config_audit *cfg, int fd, char *buf, int len)
{
	static unsigned int written = 0;

	if (-1 == write(fd, buf, len)) {
		perror("write failed");
		return -1;
	}

	written += len;
	PRINT_VERBOSE(cfg, 1, "sending %d, total %u\n",
			len, written);
	return 0;
}

int lt_fifo_recv(struct lt_config_app *cfg, struct lt_thread *t, void *buf, 
		int bufsize)
{
	static unsigned int red = 0;
	ssize_t size;
	struct lt_fifo_mbase *h = buf;

	if (-1 == (size = read(t->fifo_fd, h, sizeof(struct lt_fifo_mbase)))) {
		perror("read failed");
		return -1;
	}

	if (size == 0)
		return -1;

	red += size;

	PRINT_VERBOSE(cfg, 1, "received %d\n", h->len);

	if ((size + h->len) > bufsize) {
		printf("thread %d - buffer max size reached\n", t->tid);
		return -1;
	}

	if (-1 == (size = read(t->fifo_fd, buf + sizeof(*h), h->len))) {
		perror("read failed");
		return -1;
	}

	red += size;

	PRINT_VERBOSE(cfg, 1, "received %d, total %u\n",
			size + sizeof(*h), red);
	return 0;
}

int lt_fifo_msym_get(struct lt_config_audit *cfg, char *buf, int type,
			struct timeval *tv, char *symname, char *libto,
			char *arg, char *argd)
{
	struct lt_fifo_msym *m = (struct lt_fifo_msym*) buf;
	int len_data, len = sizeof(struct lt_fifo_msym);

	/* TODO need proper buf size checking */
	m->h.type = type;
	m->h.tid = (pid_t) syscall(SYS_gettid);
	m->h.tv = *tv;

	m->sym = 0;
	m->lib = strlen(symname);
	m->arg = m->lib + strlen(libto) + 1;
	m->argd = m->arg + strlen(arg) + 1;

	len_data = sprintf(m->data, "%s %s %s %s", symname, libto, arg, argd);

	m->data[m->lib++]   = 0x0;
	m->data[m->arg++]   = 0x0;
	m->data[m->argd++]  = 0x0;
	m->data[len_data++] = 0x0;

	len += len_data;
	m->h.len = len_data + (sizeof(*m) - sizeof(struct lt_fifo_mbase));

	PRINT_VERBOSE(cfg, 1, "sending data %d <%s> <%s> <%s> <%s>\n",
						m->h.len, 
						m->data + m->sym, 
						m->data + m->lib,
						m->data + m->arg,
						m->data + m->argd);
	return len;
}
