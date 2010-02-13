#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "config.h"


static void usage() NORETURN;
static void usage()
{
	printf("usage: latrace-ctl [-d] config\n\n");
	printf("    -d <0/1>  - disable/enable auditing\n");
	printf("\n");

	exit(1);
}

static int mmap_config(struct lt_config_ctl *cfg)
{
	struct lt_config_shared* sh;
	int page = sysconf(_SC_PAGE_SIZE);
	int fd, len;

	if (-1 == (fd = open(cfg->config, O_RDWR))) {
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
		PRINT_VERBOSE(cfg, 1,
			"mmap failed: %s\n", strerror(errno));
		return -1;
	}

	cfg->sh = sh;
	return 0;
}

static int config_ctl(struct lt_config_ctl *cfg, int argc, char **argv)
{
	memset(cfg, 0x0, sizeof(*cfg));

	while (1) {
		int c;
		int option_index = 0;
		static struct option long_options[] = {
			{"disable", required_argument, 0, 'd'},
			{"help", no_argument, 0, 'h'},
			{0, 0, 0, 0}
		};

		c = getopt_long(argc, argv, "+d:",
                        long_options, &option_index);

		if (c == -1)
			break;

		switch (c) {
		case 'd':
			cfg->set_disabled = 1;
			cfg->disabled = atoi(optarg);
			break;
		}; /* switch */
	} /* while */

	if (optind < argc)
		cfg->config = argv[optind];
	else {
		printf("failed: no config specified\n");
		usage();
	}

	return 0;
}

int main_ctl(int argc, char **argv)
{
	struct lt_config_ctl cfg;

	if (config_ctl(&cfg, argc, argv))
		return -1;

	if (mmap_config(&cfg))
		return -1;

	printf("controling config %s\n", cfg.config);

	if (cfg.set_disabled) {
		printf(" -> disabled = %d\n", cfg.disabled);
		lt_sh(&cfg, disabled) = cfg.disabled;
	}

	return 0;
}
