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


#include <string.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"

static void usage() NORETURN;
static void usage()
{
	printf("usage: latrace [-ltfsbcCpADaoyIiBdvTFELVh] command [arg ... ]\n\n");
	printf("    -l, --libs lib1,lib2...         audit from and to lib1, lib2 ...\n");
	printf("    -t, --libs-to lib1,lib2...      audit to lib1, lib2 ...\n");
	printf("    -f, --libs-from lib1,lib2...    audit from lib1, lib2 ...\n");
	printf("    -s, --sym sym1,sym2...          audit symbols sym1, sym2 ... \n");
	printf("    -L, --lib-subst s1,s2...        objsearch LD_AUDIT interface (see man page)\n");
	printf("\n");
	printf("    -c, --counts                    display statistics counts of symbols\n");
	printf("    -C, --sort-counts stat          implies -c, plus sort the statistics by 'stat':\n");
	printf("                                    time,per,call,ucall,lib,sym  (default is call)\n");
	printf("    -p, --pipe                      use pipe to latrace process to send audit data\n");
	printf("                                    latrace app is then the one displaying the output\n");
	printf("\n");
	printf("    -A, --enable-args               enable arguments output (definitions from /etc/latrace.conf)\n");
	printf("    -D, --detail-args               display struct arguments in more detail\n");
	printf("    -a, --args file                 arguments definition file, implies \'-A\'\n");
	printf("\n");
	printf("    -y, --framesize number          framesize for storing the stack before pltexit (default 1000)\n");
	printf("    -F, --not-follow-fork           dont follow fork calls - childs\n");
	printf("    -E, --not-follow-exec           dont follow exec calls\n");
	printf("\n");
	printf("    -S, --timestamp                 display timestamp for each symbol\n");
	printf("    -b, --flow-below sym1,sym2...   display flow only for sym1, sym2 ... \n");
	printf("    -I, --no-indent-sym             do not indent symbols based on the their stack depth\n");
	printf("    -i, --indent-sym indent_size    specify indent size specification\n");
	printf("    -B, --braces                    allways display braces {}\n");
	printf("    -d, --demangle                  run the symbol name throught the C++ demangler\n");
	printf("    -T, --hide-tid                  dont display thread id\n");
	printf("    -o, --output file               store output to file\n");
	printf("\n");
	printf("    -R, --ctl-config                controled config\n");
	printf("    -q, --disable                   disable auditing (enables -R)\n");
	printf("\n");
	printf("    -v, --verbose                   verbose output\n");
	printf("    -V, --version                   display version\n");
	printf("    -h, --help                      display help\n");
	printf("\n");

	exit(1);
}

static void version() NORETURN;
static void version()
{
	printf("latrace version " LT_VER "\n");
	exit(0);
}

static struct lt_config_tv counts_sort[] = {
	{ .type = LT_CSORT_TIME,    .name = "time" },
	{ .type = LT_CSORT_PERCENT, .name = "per" },
	{ .type = LT_CSORT_CALL,    .name = "call" },
	{ .type = LT_CSORT_UCALL,   .name = "ucall" },
	{ .type = LT_CSORT_LIB,     .name = "lib" },
	{ .type = LT_CSORT_SYM,     .name = "sym" }
};

#define COUNTS_SORT_NUM (sizeof(counts_sort)/sizeof(struct lt_config_tv))

static int get_type(struct lt_config_app *cfg, struct lt_config_tv *tv, 
		int num, char *name)
{
	int i;

	for(i = 0; i < num; i++)
		if (!strcmp(name, tv[i].name))
			return tv[i].type;

	printf("failed to find name: %s\n", name);
	return -1;
}

int lt_config(struct lt_config_app *cfg, int argc, char **argv)
{
	memset(cfg, 0, sizeof(*cfg));
	cfg->sh = &cfg->sh_storage;

	/* default values settings */
	lt_sh(cfg, magic)       = LT_MAGIC;
	lt_sh(cfg, framesize)   = 1000;
	lt_sh(cfg, fout)        = stdout;
	lt_sh(cfg, indent_sym)  = 1;
	lt_sh(cfg, indent_size) = 2;
	lt_sh(cfg, args_maxlen) = LR_ARGS_MAXLEN;
	lt_sh(cfg, args_detail_maxlen) = LR_ARGS_DETAIL_MAXLEN;
	cfg->csort = LT_CSORT_CALL;

	while (1) {
		int c;
		int option_index = 0;
		static struct option long_options[] = {
			{"sym", required_argument, 0, 's'},
			{"libs", required_argument, 0, 'l'},
			{"libs-to", required_argument, 0, 't'},
			{"libs-from", required_argument, 0, 'f'},
			{"no-indent-sym", no_argument, 0, 'I'},
			{"indent-sym", required_argument, 0, 'i'},
			{"braces", no_argument, 0, 'B'},
			{"demangle", no_argument, 0, 'd'},
			{"timestamp", required_argument, 0, 'S'},
			{"flow-below", required_argument, 0, 'b'},
			{"counts", no_argument, 0, 'c'},
			{"sort-counts", required_argument, 0, 'C'},
			{"pipe", no_argument, 0, 'p'},
			{"output", required_argument, 0, 'o'},
			{"args", required_argument, 0, 'a'},
			{"enable-args", required_argument, 0, 'A'},
			{"detail-args", required_argument, 0, 'D'},
			{"framesize", required_argument, 0, 'y'},
			{"lib-subst", required_argument, 0, 'L'},
			{"verbose", no_argument, 0, 'v'},
			{"hide-tid", no_argument, 0, 'T'},
			{"not-follow-fork", no_argument, 0, 'F'},
			{"not-follow-exec", no_argument, 0, 'E'},
			{"disable", no_argument, 0, 'q'},
			{"ctl-config", no_argument, 0, 'R'},
			{"version", no_argument, 0, 'V'},
			{"help", no_argument, 0, 'h'},
			{0, 0, 0, 0}
		};

		c = getopt_long(argc, argv, "+s:l:t:f:vhi:BdISb:cC:y:L:po:a:ADVTFERq",
					long_options, &option_index);

		if (c == -1)
			break;

		switch (c) {
		case 'l':
			if (strlen(optarg) > LT_LIBS_MAXSIZE)
				return -1;

			strncpy(lt_sh(cfg, libs_both), optarg, strlen(optarg));
			break;

		case 't':
			if (strlen(optarg) > LT_LIBS_MAXSIZE)
				return -1;

			strncpy(lt_sh(cfg, libs_to), optarg, strlen(optarg));
			break;

		case 'f':
			if (strlen(optarg) > LT_LIBS_MAXSIZE)
				return -1;

			strncpy(lt_sh(cfg, libs_from), optarg, strlen(optarg));
			break;

		case 's':
			if (strlen(optarg) > LT_SYMBOLS_MAXSIZE)
				return -1;

			strncpy(lt_sh(cfg, symbols), optarg, strlen(optarg));
			break;

		case 'b':
			if (strlen(optarg) > LT_SYMBOLS_MAXSIZE)
				return -1;

			strncpy(lt_sh(cfg, flow_below), optarg, strlen(optarg));
			break;

		case 'v':
			lt_sh(cfg, verbose)++;
			break;

		case 'S':
			lt_sh(cfg, timestamp) = 1;
			break;

		case 'T':
			lt_sh(cfg, hide_tid) = 1;
			break;

		case 'F':
			lt_sh(cfg, not_follow_fork) = 1;
			break;

		case 'E':
			lt_sh(cfg, not_follow_exec) = 1;
			break;

		case 'i':
			lt_sh(cfg, indent_size) = atoi(optarg);
			break;

		case 'B':
			lt_sh(cfg, braces) = 1;
			break;

		case 'd':
			#ifndef LT_LIBERTY
			printf("demangle support not compiled in," \
				" liberty not found\n");
			break;
			#endif

			lt_sh(cfg, demangle) = 1;
			break;

		case 'I':
			lt_sh(cfg, indent_sym) = 0;
			break;

		case 'y':
			lt_sh(cfg, framesize) = atoi(optarg);
			break;

		case 'L':
			if (strlen(optarg) > LT_SYMBOLS_MAXSIZE)
				return -1;

			strncpy(lt_sh(cfg, libs_subst), optarg, strlen(optarg));
			break;

		case 'C':

			if (-1 == (cfg->csort = get_type(cfg, counts_sort, COUNTS_SORT_NUM, optarg)))
				usage();
			/* falling through */
		case 'c':
			lt_sh(cfg, counts) = 1;
			/* falling through */
		case 'p':
			lt_sh(cfg, pipe) = 1;
			break;

		case 'a':
			strcpy(lt_sh(cfg, args_def), optarg);
			/* falling through */
		case 'A':
			lt_sh(cfg, args_enabled) = 1;
			break;

		case 'D':
			lt_sh(cfg, args_detailed) = 1;
			break;

		case 'o':
			strcpy(lt_sh(cfg, output), optarg);
			break;

		case 'q':
			lt_sh(cfg, disabled) = 1;
			/* falling through */
		case 'R':
			lt_sh(cfg, ctl_config) = 1;
			break;

		case 'V':
			version();
			break;

		case 'h':
			usage();
			break;

		default:
			printf("unknown option '%c'", c);
		} // switch (c)
	} // while(1)

	if (optind < argc) {
		int i_arg = 1;
		cfg->prog = argv[optind++];
		cfg->arg[0] = cfg->prog;
        
		while ((optind < argc) && (i_arg < LT_NUM_ARG)) {
			cfg->arg[i_arg++] = argv[optind++];
		}
		cfg->arg_num = i_arg;
	}

	if (!cfg->prog) {
		printf("failed: no program specified\n");
		usage();
	}

	if ((lt_sh(cfg, pipe)) && (*lt_sh(cfg, output)) &&
	    (NULL == (lt_sh(cfg, fout) = fopen(lt_sh(cfg, output), "w")))) {
		printf("failed to fopen output file %s\n", lt_sh(cfg, output));
		usage();
	}

	return 0;
}
