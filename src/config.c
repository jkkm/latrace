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


#include <string.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "config.h"
#include "lib-include.h"

extern FILE *lt_config_in;
int  lt_config_parse();
void lt_config__switch_to_buffer (YY_BUFFER_STATE new_buffer  );
void lt_config__delete_buffer (YY_BUFFER_STATE b  );
YY_BUFFER_STATE lt_config__create_buffer (FILE *file,int size  );

static struct lt_include inc = {
	.stack_idx        = 0,
	.create_buffer    = lt_config__create_buffer,
	.switch_to_buffer = lt_config__switch_to_buffer,
	.delete_buffer    = lt_config__delete_buffer,
	.in               = &lt_config_in,
};

int lt_config_parse_init(struct lt_config_app *cfg, struct lt_include *inc);

static void usage() NORETURN;
static void usage()
{
	printf("usage: latrace [-ltfsbcCpADaoyIiBdvTFELVh] command [arg ... ]\n\n");
	printf("    -l, --libs lib1,lib2...         audit from and to lib1, lib2 ...\n");
	printf("    -t, --libs-to lib1,lib2...      audit to lib1, lib2 ...\n");
	printf("    -f, --libs-from lib1,lib2...    audit from lib1, lib2 ...\n");
	printf("    -s, --sym sym1,sym2...          audit symbols sym1, sym2 ... \n");
	printf("    -n, --sym-omit sym1,sym2...     omit symbols sym1, sym2 ... \n");
	printf("    -L, --lib-subst s1,s2...        objsearch LD_AUDIT interface (see man page)\n");
	printf("\n");
	printf("    -c, --counts                    display statistics counts of symbols\n");
	printf("    -C, --sort-counts stat          implies -c, plus sort the statistics by 'stat':\n");
	printf("                                    time,per,call,ucall,lib,sym  (default is call)\n");
	printf("    -p, --pipe                      use pipe to latrace process to send audit data\n");
	printf("                                    latrace app is then the one displaying the output\n");
	printf("    -N, --conf                      config file\n");
	printf("\n");
#ifndef CONFIG_ARCH_HAVE_ARGS
	printf("    -[ADa]                          arguments display support not compiled in\n");
#else
	printf("    -A, --enable-args               enable arguments output (definitions from headers)\n");
	printf("    -D, --detail-args               display struct arguments in more detail\n");
	printf("    -a, --args file                 arguments definition file, implies \'-A\'\n");
#endif
	printf("\n");
	printf("    -y, --framesize number          framesize for storing the stack before pltexit (default 1000)\n");
	printf("    -Y, --no-framesize-check        disable framesize check\n");
	printf("    -F, --no-follow-fork            dont follow fork calls - childs\n");
	printf("    -E, --no-follow-exec            dont follow exec calls\n");
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
	printf("    -q, --disable                   disable auditing\n");
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
	printf("latrace version " CONFIG_VERSION "\n");
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

/* read conf file */
static int read_config(struct lt_config_app *cfg, char *file)
{
	int ret = 0;
	lt_config_parse_init(cfg, &inc);

	PRINT_VERBOSE(cfg, 1, "config file %s\n", file);

	if (lt_inc_open(cfg->sh, &inc, file))
		return -1;

	if (lt_config_parse()) {
		printf("failed to parse config file %s\n", file);
		ret = -1;
        }

	return ret;
}

#define CHECK_BOOL(str, sval, ival)    \
do {                                   \
	if (ival != -1)                \
		val = ival;            \
	else if (!strcmp(sval, "YES")) \
		val = 1;               \
	else if (!strcmp(sval, "NO"))  \
		val = 0;               \
	else                           \
		return -1;             \
} while(0)

#define CHECK_INT(val, sval, ival) \
do {                               \
	val = ival;                \
	if (val == -1)             \
		val = atoi(sval);  \
} while(0)

static int process_option_val(struct lt_config_app *cfg, int idx,
			      char *sval, int ival)
{
	int val;

	PRINT_VERBOSE(cfg, 1, "option idx %d, sval %s, ival %d\n",
		      idx, sval, ival);

	switch(idx) {
	case LT_OPT_HEADERS:
		strcpy(lt_sh(cfg, args_def), sval);

		PRINT_VERBOSE(cfg, 1, "HEADERS '%s'\n",
			      lt_sh(cfg, args_def));
		break;

	case LT_OPT_INDENT_SYM:
		CHECK_INT(val, sval, ival);
		lt_sh(cfg, indent_size) = val;

		PRINT_VERBOSE(cfg, 1, "INDENT_SYM %d\n",
			      lt_sh(cfg, indent_size));
		break;

	case LT_OPT_PIPE:
		CHECK_BOOL(val, sval, ival);
		lt_sh(cfg, pipe) = val;

		PRINT_VERBOSE(cfg, 1, "PIPE %d\n",
			      lt_sh(cfg, pipe));
		break;

	case LT_OPT_TIMESTAMP:
		CHECK_BOOL(val, sval, ival);
		lt_sh(cfg, timestamp) = val;

		PRINT_VERBOSE(cfg, 1, "TIMESTAMP %d\n",
			      lt_sh(cfg, timestamp));
		break;

	case LT_OPT_FRAMESIZE:
		CHECK_INT(val, sval, ival);
		lt_sh(cfg, framesize) = val;

		PRINT_VERBOSE(cfg, 1, "FRAMESIZE %d\n",
			      lt_sh(cfg, framesize));
		break;

	case LT_OPT_FRAMESIZE_CHECK:
		CHECK_BOOL(val, sval, ival);
		lt_sh(cfg, framesize_check) = val;

		PRINT_VERBOSE(cfg, 1, "FRAMESIZE_CHECK %d\n",
			      lt_sh(cfg, framesize_check));
		break;

	case LT_OPT_HIDE_TID:
		CHECK_BOOL(val, sval, ival);
		lt_sh(cfg, hide_tid) = val;

		PRINT_VERBOSE(cfg, 1, "HIDE_TID %d\n",
			      lt_sh(cfg, hide_tid));
		break;

	case LT_OPT_FOLLOW_FORK:
		CHECK_BOOL(val, sval, ival);
		lt_sh(cfg, not_follow_fork) = !val;

		PRINT_VERBOSE(cfg, 1, "NOT FOLLOW_FORK %d\n",
			      lt_sh(cfg, not_follow_fork));
		break;

	case LT_OPT_FOLLOW_EXEC:
		CHECK_BOOL(val, sval, ival);
		lt_sh(cfg, not_follow_exec) = !val;

		PRINT_VERBOSE(cfg, 1, "NOT FOLLOW_EXEC %d\n",
			      lt_sh(cfg, not_follow_exec));
		break;

	case LT_OPT_DEMANGLE:
		CHECK_BOOL(val, sval, ival);
		lt_sh(cfg, demangle) = val;

		PRINT_VERBOSE(cfg, 1, "DEMANGLE %d\n",
			      lt_sh(cfg, demangle));
		break;

	case LT_OPT_BRACES:
		CHECK_BOOL(val, sval, ival);
		lt_sh(cfg, braces) = val;

		PRINT_VERBOSE(cfg, 1, "BRACES %d\n",
			      lt_sh(cfg, braces));
		break;

	case LT_OPT_ENABLE_ARGS:
		CHECK_BOOL(val, sval, ival);
		lt_sh(cfg, args_enabled) = val;

		PRINT_VERBOSE(cfg, 1, "ENABLE_ARGS %d\n",
			      lt_sh(cfg, args_enabled));
		break;

	case LT_OPT_DETAIL_ARGS:
		CHECK_BOOL(val, sval, ival);
		lt_sh(cfg, args_detailed) = val;

		PRINT_VERBOSE(cfg, 1, "DETAIL_ARGS %d\n",
			      lt_sh(cfg, args_detailed));
		break;

	case LT_OPT_OUTPUT_TTY:
		cfg->output_tty = 1;
		strcpy(cfg->output_tty_file, sval);

		PRINT_VERBOSE(cfg, 1, "OUTPUT_TTY '%s'\n",
			      cfg->output_tty_file);
		break;

	case LT_OPT_LIBS:
		strcpy(lt_sh(cfg, libs_both), sval);
		PRINT_VERBOSE(cfg, 1, "LIBS '%s'\n",
			      lt_sh(cfg, libs_both));
		break;

	case LT_OPT_LIBS_TO:
		strcpy(lt_sh(cfg, libs_to), sval);
		PRINT_VERBOSE(cfg, 1, "LIBS_TO '%s'\n",
			      lt_sh(cfg, libs_to));
		break;

	case LT_OPT_LIBS_FROM:
		strcpy(lt_sh(cfg, libs_from), sval);
		PRINT_VERBOSE(cfg, 1, "LIBS_FROM '%s'\n",
			      lt_sh(cfg, libs_from));
		break;

	case LT_OPT_SYM:
		strcpy(lt_sh(cfg, symbols), sval);
		PRINT_VERBOSE(cfg, 1, "SYM '%s'\n",
			      lt_sh(cfg, symbols));
		break;

	case LT_OPT_SYM_OMIT:
		strcpy(lt_sh(cfg, symbols_omit), sval);
		PRINT_VERBOSE(cfg, 1, "SYM_OMIT '%s'\n",
			      lt_sh(cfg, symbols_omit));
		break;

	case LT_OPT_SYM_BELOW:
		strcpy(lt_sh(cfg, flow_below), sval);
		PRINT_VERBOSE(cfg, 1, "SYM_BELOW '%s'\n",
			      lt_sh(cfg, flow_below));
		break;

	case LT_OPT_SYM_NOEXIT:
		strcpy(lt_sh(cfg, symbols_noexit), sval);
		PRINT_VERBOSE(cfg, 1, "SYM_NOEXIT '%s'\n",
			      lt_sh(cfg, symbols_noexit));
		break;

	case LT_OPT_ARGS_STRING_POINTER_LENGTH:
		CHECK_BOOL(val, sval, ival);
		lt_sh(cfg, args_string_pointer_length) = val;

		PRINT_VERBOSE(cfg, 1, "ARGS_STRING_POINTER_LENGTH %d\n",
			      lt_sh(cfg, args_detailed));
		break;

	default:
		return -1;
	}
	return 0;
}

static int process_option(struct lt_config_app *cfg, struct lt_config_opt *opt)
{
	return process_option_val(cfg, opt->idx, opt->sval, opt->nval);
}

int lt_config_opt_process(struct lt_config_app *cfg, struct lt_list_head *list)
{
	struct lt_config_opt *opt;

	lt_list_for_each_entry(opt, list, list) {
		int ret;

		ret = process_option(cfg, opt);
		if (ret)
			return ret;
	}

	return 0;
}

struct lt_config_opt *lt_config_opt_new(struct lt_config_app *cfg,
					int idx, char *sval, long nval)
{
	struct lt_config_opt *opt;

	opt = malloc(sizeof(*opt));
	if (!opt)
		return NULL;

	lt_init_list_head(&opt->list);
	opt->idx  = idx;
	opt->sval = sval ? strdup(sval) : NULL;
	opt->nval = nval;

	PRINT_VERBOSE(cfg, 1, "idx %d = %s, %d\n",
		      opt->idx, opt->sval , opt->nval);
	return opt;
}

int lt_config_ln_add(struct lt_list_head *head, char *name)
{
	struct lt_config_ln *ln = malloc(sizeof(*ln));

        if (!ln)
                return -1;

        ln->name = strdup(name);
        lt_init_list_head(&ln->list);
        lt_list_add_tail(&ln->list, head);
	return 0;
}

int lt_config_ln_fill(struct lt_list_head *head, char *buf, int size)
{
        struct lt_config_ln *ln, *n;
        int first = 1;
	char *b = buf;

        buf[0] = 0x00;

        lt_list_for_each_entry_safe(ln, n, head, list) {
		int ret;

		ret = snprintf(b, size, "%s%s",
				first ? "" : ",",
				ln->name);

		if (ret >= size)
			return -1;

		size -= ret;
		b += ret;
		first = 0;

		lt_list_del(&ln->list);
		free(ln->name);
		free(ln);
        }

        return 0;
}

int lt_config(struct lt_config_app *cfg, int argc, char **argv)
{
	memset(cfg, 0, sizeof(*cfg));
	cfg->sh = cfg->sh_storage.sh = &cfg->sh_storage;

	/* default values settings */
	lt_sh(cfg, magic)           = LT_CONFIG_MAGIC;
	lt_sh(cfg, framesize)       = 1000;
	lt_sh(cfg, framesize_check) = 1;
	lt_sh(cfg, fout)            = stdout;
	lt_sh(cfg, indent_sym)      = 1;
	lt_sh(cfg, indent_size)     = 2;
	lt_sh(cfg, args_maxlen)     = LR_ARGS_MAXLEN;
	lt_sh(cfg, args_detail_maxlen) = LR_ARGS_DETAIL_MAXLEN;
	cfg->csort = LT_CSORT_CALL;
	cfg->output_tty_fd = -1;

	/* read the default config file first */
	if (read_config(cfg, LT_CONF_DIR "/latrace.conf")) {
		printf("failed: read config file '" LT_CONF_DIR "/latrace.conf'\n");
		usage();
	}

	while (1) {
		int c;
		int option_index = 0;
		static struct option long_options[] = {
			{"sym", required_argument, 0, 's'},
			{"sym-omit", required_argument, 0, 'n'},
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
			{"conf", required_argument, 0, 'N'},
			{"args", required_argument, 0, 'a'},
			{"enable-args", required_argument, 0, 'A'},
			{"detail-args", required_argument, 0, 'D'},
			{"framesize", required_argument, 0, 'y'},
			{"no-framesize-check ", no_argument, 0, 'Y'},
			{"lib-subst", required_argument, 0, 'L'},
			{"verbose", no_argument, 0, 'v'},
			{"hide-tid", no_argument, 0, 'T'},
			{"no-follow-fork", no_argument, 0, 'F'},
			{"no-follow-exec", no_argument, 0, 'E'},
			{"disable", no_argument, 0, 'q'},
			{"ctl-config", no_argument, 0, 'R'},
			{"version", no_argument, 0, 'V'},
			{"help", no_argument, 0, 'h'},
			{0, 0, 0, 0}
		};

		c = getopt_long(argc, argv, "+s:n:l:t:f:vhi:BdISb:cC:y:YL:po:a:N:ADVTFERq",
					long_options, &option_index);

		if (c == -1)
			break;

		switch (c) {
		case 'l':
			if (strlen(optarg) >= LT_LIBS_MAXSIZE)
				return -1;

			process_option_val(cfg, LT_OPT_LIBS, optarg, -1);
			break;

		case 't':
			if (strlen(optarg) >= LT_LIBS_MAXSIZE)
				return -1;

			process_option_val(cfg, LT_OPT_LIBS_TO, optarg, -1);
			break;

		case 'f':
			if (strlen(optarg) >= LT_LIBS_MAXSIZE)
				return -1;

			process_option_val(cfg, LT_OPT_LIBS_FROM, optarg, -1);
			break;

		case 's':
			if (strlen(optarg) >= LT_SYMBOLS_MAXSIZE)
				return -1;

			process_option_val(cfg, LT_OPT_SYM, optarg, -1);
			break;

		case 'n':
			if (strlen(optarg) >= LT_SYMBOLS_MAXSIZE)
				return -1;

			process_option_val(cfg, LT_OPT_SYM_OMIT, optarg, -1);
			break;

		case 'b':
			if (strlen(optarg) >= LT_SYMBOLS_MAXSIZE)
				return -1;

			process_option_val(cfg, LT_OPT_SYM_BELOW, optarg, -1);
			break;

		case 'v':
			lt_sh(cfg, verbose)++;
			break;

		case 'S':
			process_option_val(cfg, LT_OPT_TIMESTAMP, NULL, 1);
			break;

		case 'T':
			process_option_val(cfg, LT_OPT_HIDE_TID, NULL, 1);
			break;

		case 'F':
			process_option_val(cfg, LT_OPT_FOLLOW_FORK, NULL, 0);
			break;

		case 'E':
			process_option_val(cfg, LT_OPT_FOLLOW_EXEC, NULL, 0);
			break;

		case 'i':
			lt_sh(cfg, indent_size) = atoi(optarg);
			break;

		case 'B':
			process_option_val(cfg, LT_OPT_BRACES, NULL, 0);
			break;

		case 'd':
			#ifndef CONFIG_LIBERTY
			printf("demangle support not compiled in," \
				" liberty not found\n");
			break;
			#endif

			process_option_val(cfg, LT_OPT_DEMANGLE, NULL, 0);
			break;

		case 'I':
			lt_sh(cfg, indent_sym) = 0;
			break;

		case 'y':
			process_option_val(cfg, LT_OPT_FRAMESIZE, optarg, -1);
			break;

		case 'Y':
			process_option_val(cfg, LT_OPT_FRAMESIZE_CHECK, NULL, 0);
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
			process_option_val(cfg, LT_OPT_PIPE, NULL, 1);
			break;

		#ifndef CONFIG_ARCH_HAVE_ARGS
		case 'a':
		case 'A':
		case 'D':
			printf("Arguments display support not compiled in");
			break;
		#else
		case 'a':
			process_option_val(cfg, LT_OPT_HEADERS, optarg, -1);

			/* falling through */
		case 'A':
			process_option_val(cfg, LT_OPT_ENABLE_ARGS, NULL, 1);
			break;

		case 'D':
			process_option_val(cfg, LT_OPT_DETAIL_ARGS, NULL, 1);
			break;
		#endif /* CONFIG_ARCH_HAVE_ARGS */

		case 'N':
			/* read user-specifide config file */
			if (read_config(cfg, optarg)) {
				printf("failed: read config file '%s'\n", optarg);
				usage();
			}
			break;

		case 'o':
			strcpy(lt_sh(cfg, output), optarg);
			break;

		case 'q':
			lt_sh(cfg, disabled) = 1;
			break;

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
