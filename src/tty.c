
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <setjmp.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "config.h"

static int setup_slave(int fd)
{
	struct termios tio;

	tcgetattr(fd, &tio);

	/* disable "\n" -> "\r\n" */
	tio.c_oflag &= ~ONLCR;

	return tcsetattr(fd, TCSANOW, &tio);
}

int tty_master(struct lt_config_app *cfg)
{
	int mfd;

	if ((mfd = getpt()) < 0) {
		perror("getpt failed");
		return -1;
	}

	if (unlockpt(mfd)) {
		perror("unlockpt failed");
		return -1;
	}

	PRINT_VERBOSE(cfg, 1, "pty master opened succesfully\n");
	return mfd;
}

int tty_init(struct lt_config_app *cfg, int master)
{
	int i, slave, num_files = getdtablesize();
	char *sname;
	jmp_buf env;

	if (setjmp(env)) {
		tty_restore(cfg);
		return -1;
	}

	sname = (char*) ptsname(master);
	if (!sname)
		longjmp(env, 1);

	PRINT_VERBOSE(cfg, 1, "closing all opened descriptors\n");
	for(i = 0; i < num_files; i++)
		close(i);

	/* get new session before we open new controling tty */
	if (-1 == setsid()) {
		perror("setsid failed");
		return -1;
	}

	slave = open(sname, O_RDWR);
	if (slave != 0)
		longjmp(env, 1);

	/* set controling tty */
	if (ioctl(0, TIOCSCTTY, 1))
		longjmp(env, 1);

	if (setup_slave(0))
		longjmp(env, 1);

	dup(0);
	dup(0);
	return 0;
}

void tty_close(struct lt_config_app *cfg)
{
	close(cfg->output_tty_fd);
}

int tty_restore(struct lt_config_app *cfg)
{
	int i, num_files = getdtablesize();

	for(i = 0; i < num_files; i++) {
		if (fcntl(i, F_GETFD, NULL) != -1)
			close(i);
	}

	open("/dev/tty", O_RDWR);
	dup(0);
	dup(0);

	return 0;
}

int tty_process(struct lt_config_app *cfg, int master)
{
#define BUFSIZE 4096
	char buf[BUFSIZE];
	ssize_t ret;
	int fd = cfg->output_tty_fd;

	if (fd == -1) {
		fd = open(cfg->output_tty_file, O_RDWR | O_CREAT | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
		if (fd < 0) {
			perror("failed to open OUTPUT_TTY file,"
				" output is not logged");
			return -1;
		}
		PRINT_VERBOSE(cfg, 1, "opened tty output file %s\n",
				cfg->output_tty_file);

		cfg->output_tty_fd = fd;
	}

	ret = read(master, buf, BUFSIZE);
	/* Most likely the other side closed */
	if (ret <= 0) {
		PRINT_VERBOSE(cfg, 1,
			"failed to read tty, closing [ errno %d '%s']\n",
			errno, strerror(errno));
		return -1;
	}

	if (fd < 0)
		return -1;

	if (write(fd, buf, ret) <= 0) {
		perror("failed to write to OUTPUT_TTY file");
		return -1;
	}

	return 0;
}
