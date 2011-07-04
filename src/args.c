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


#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <setjmp.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>

#include "config.h"
#include "lib-include.h"

#define LT_EQUAL           " = "

extern int errno;
extern FILE *lt_args_in;
int  lt_args_parse();
void lt_args__switch_to_buffer (YY_BUFFER_STATE new_buffer  );
void lt_args__delete_buffer (YY_BUFFER_STATE b  );
YY_BUFFER_STATE lt_args__create_buffer (FILE *file,int size  );

static struct lt_include inc = {
	.create_buffer    = lt_args__create_buffer,
	.switch_to_buffer = lt_args__switch_to_buffer,
	.delete_buffer    = lt_args__delete_buffer,
	.in               = &lt_args_in,
};

int lt_args_parse_init(struct lt_config_shared *cfg, struct lt_include *inc);

static int enum_init = 0;


/* hardcoded POD types */
static struct lt_arg args_def_pod[] = {
	{
		.dtype     = LT_ARGS_DTYPE_POD, 
		.type_id   = LT_ARGS_TYPEID_VOID,
		.type_len  = sizeof(void),
		.type_name = "void",
		.pointer   = 0,
		.name      = "",
		.mmbcnt    = 0,
                .arch      = NULL,
		.en        = NULL,
		.args_head = NULL,
		.args_list = { NULL, NULL }
	},
	{
		.dtype     = LT_ARGS_DTYPE_POD, 
		.type_id   = LT_ARGS_TYPEID_SHORT,
		.type_len  = sizeof(short),
		.type_name = "short",
		.pointer   = 0,
		.name      = "",
		.mmbcnt    = 0,
                .arch      = NULL,
		.en        = NULL,
		.args_head = NULL,
		.args_list = { NULL, NULL }
	},
	{
		.dtype     = LT_ARGS_DTYPE_POD, 
		.type_id   = LT_ARGS_TYPEID_USHORT,
		.type_len  = sizeof(unsigned short),
		.type_name = "u_short",
		.pointer   = 0,
		.name      = "", 
		.mmbcnt    = 0,
                .arch      = NULL,
		.en        = NULL,
		.args_head = NULL,
		.args_list = { NULL, NULL }
	},
	{
		.dtype     = LT_ARGS_DTYPE_POD, 
		.type_id   = LT_ARGS_TYPEID_INT,
		.type_len  = sizeof(int),
		.type_name = "int",
		.pointer   = 0,
		.name      = "",
		.mmbcnt    = 0,
                .arch      = NULL,
		.en        = NULL,
		.args_head = NULL,
		.args_list = { NULL, NULL }
	},
	{
		.dtype     = LT_ARGS_DTYPE_POD, 
		.type_id   = LT_ARGS_TYPEID_UINT,
		.type_len  = sizeof(unsigned int),
		.type_name = "u_int",
		.pointer   = 0,
		.name      = "",
		.mmbcnt    = 0,
                .arch      = NULL,
		.en        = NULL,
		.args_head = NULL,
		.args_list = { NULL, NULL }
	},
	{
		.dtype     = LT_ARGS_DTYPE_POD, 
		.type_id   = LT_ARGS_TYPEID_LONG,
		.type_len  = sizeof(long),
		.type_name = "long",
		.pointer   = 0,
		.name      = "",
		.mmbcnt    = 0,
                .arch      = NULL,
		.en        = NULL,
		.args_head = NULL,
		.args_list = { NULL, NULL }
	},
	{
		.dtype     = LT_ARGS_DTYPE_POD, 
		.type_id   = LT_ARGS_TYPEID_ULONG,
		.type_len  = sizeof(unsigned long),
		.type_name = "u_long",
		.pointer   = 0,
		.name      = "",
		.mmbcnt    = 0,
                .arch      = NULL,
		.en        = NULL,
		.args_head = NULL,
		.args_list = { NULL, NULL }
	},
	{
		.dtype     = LT_ARGS_DTYPE_POD, 
		.type_id   = LT_ARGS_TYPEID_CHAR,
		.type_len  = sizeof(char),
		.type_name = "char",
		.pointer   = 0,
		.name      = "",
		.mmbcnt    = 0,
                .arch      = NULL,
		.en        = NULL,
		.args_head = NULL,
		.args_list = { NULL, NULL }
	},
	{
		.dtype     = LT_ARGS_DTYPE_POD, 
		.type_id   = LT_ARGS_TYPEID_UCHAR,
		.type_len  = sizeof(unsigned char),
		.type_name = "u_char",
		.pointer   = 0,
		.name      = "",
		.mmbcnt    = 0,
                .arch      = NULL,
		.en        = NULL,
		.args_head = NULL,
		.args_list = { NULL, NULL }
	},
	{
		.dtype     = LT_ARGS_DTYPE_POD, 
		.type_id   = LT_ARGS_TYPEID_LLONG,
		.type_len  = sizeof(long long),
		.type_name = "llong",
		.pointer   = 0,
		.name      = "",
		.mmbcnt    = 0,
                .arch      = NULL,
		.en        = NULL,
		.args_head = NULL,
		.args_list = { NULL, NULL }
	},
	{
		.dtype     = LT_ARGS_DTYPE_POD, 
		.type_id   = LT_ARGS_TYPEID_ULLONG,
		.type_len  = sizeof(unsigned long long),
		.type_name = "u_llong",
		.pointer   = 0,
		.name      = "",
		.mmbcnt    = 0,
                .arch      = NULL,
		.en        = NULL,
		.args_head = NULL,
		.args_list = { NULL, NULL }
	},
	{
		.dtype     = LT_ARGS_DTYPE_POD, 
		.type_id   = LT_ARGS_TYPEID_DOUBLE,
		.type_len  = sizeof(double),
		.type_name = "double",
		.pointer   = 0,
		.name      = "",
		.mmbcnt    = 0,
                .arch      = NULL,
		.en        = NULL,
		.args_head = NULL,
		.args_list = { NULL, NULL }
	},
	{
		.dtype     = LT_ARGS_DTYPE_POD, 
		.type_id   = LT_ARGS_TYPEID_FLOAT,
		.type_len  = sizeof(float),
		.type_name = "float",
		.pointer   = 0,
		.name      = "",
		.mmbcnt    = 0,
                .arch      = NULL,
		.en        = NULL,
		.args_head = NULL,
		.args_list = { NULL, NULL }
	},
};

#define LT_ARGS_DEF_POD_NUM     (sizeof(args_def_pod)/sizeof(struct lt_arg))

/* struct, typedef, enum */
static struct lt_arg args_def_struct[LT_ARGS_DEF_STRUCT_NUM];
static struct lt_arg args_def_typedef[LT_ARGS_DEF_TYPEDEF_NUM];
static int args_def_struct_cnt  = 0;
static int args_def_typedef_cnt = 0;
static struct hsearch_data args_enum_tab;

static struct lt_enum* getenum(struct lt_config_shared *cfg, char *name)
{
	struct lt_enum *en;
	ENTRY e, *ep;

	PRINT_VERBOSE(cfg, 1, "request for <%s>\n", name);

	if (!enum_init) {
		PRINT_VERBOSE(cfg, 1, "no enum added so far\n", name);
		return NULL;
	}

	e.key = name;
	hsearch_r(e, FIND, &ep, &args_enum_tab);

	if (!ep) {
		PRINT_VERBOSE(cfg, 1, "failed to find enum <%s>\n", name);
		return NULL;
	}

	en = (struct lt_enum*) ep->data;

	PRINT_VERBOSE(cfg, 1, "found %p <%s>\n", en, en->name);
	return en;
}

static int enum_comp(const void *ep1, const void *ep2)
{
	struct lt_enum_elem *e1 = (struct lt_enum_elem*) ep1;
	struct lt_enum_elem *e2 = (struct lt_enum_elem*) ep2;

	return e1->val - e2->val;
}

static struct lt_enum_elem* get_enumelem(struct lt_config_shared *cfg, 
	long val, struct lt_enum *en)
{
	struct lt_enum_elem key;
	key.val = val;

	PRINT_VERBOSE(cfg, 1, "looking for %p <%s> value %ld\n",
			en, en->name, val);

	return bsearch(&key, en->elems, en->cnt, 
		sizeof(struct lt_enum_elem), enum_comp);
}

static struct lt_enum_elem* find_enumelem(struct lt_config_shared *cfg,
	char *name, struct lt_enum *en)
{
	struct lt_enum_elem *elem;
	int i;

	for(i = 0; en->cnt; i++) {
		elem = &en->elems[i];

		if (!strcmp(elem->name, name))
			return elem;
	}

	return NULL;
}

int lt_args_add_enum(struct lt_config_shared *cfg, char *name, 
			struct lt_list_head *h)
{
	ENTRY e, *ep;
	struct lt_enum_elem *elem, *last = NULL;
	struct lt_enum *en;
	int i = 0;

	if (NULL == (en = malloc(sizeof(*en))))
		return -1;

	memset(en, 0x0, sizeof(*en));
	en->name = name;

	/* Initialize the hash table holding enum names */
	if (!enum_init) {
	        if (!hcreate_r(LT_ARGS_DEF_ENUM_NUM, &args_enum_tab)) {
	                perror("failed to create has table:");
			free(en);
	                return -1;
	        }
		enum_init = 1;
	}

	e.key = en->name;
	e.data = en;

	if (!hsearch_r(e, ENTER, &ep, &args_enum_tab)) {
		perror("hsearch_r failed");
		free(en);
		return 1;
	}

	/* We've got enum inside the hash, let's prepare the enum itself.
	   The 'elems' field is going to be the qsorted list of 
	   'struct enum_elem's */
	lt_list_for_each_entry(elem, h, list)
		en->cnt++;

	if (NULL == (en->elems = malloc(sizeof(struct lt_enum_elem) * en->cnt)))
		return -1;

	PRINT_VERBOSE(cfg, 3, "enum %s (%d elems) not fixed\n",
			en->name, en->cnt);

	/*
	 * The enum element can be:
	 *
	 * 1) defined
	 * 2) undefined
	 * 3) defined via string reference
	 *
	 * ad 1) no work
	 * ad 2) value of previous element is used
	 * ad 3) we look for the string reference in defined elements' names
	 *
	 * This being said, following actions will happen now:
	 *
	 * - copy all the values to the prepared array
	 * - fix the values based on the above
	 * - sort the array
	 */

	lt_list_for_each_entry(elem, h, list) {

		PRINT_VERBOSE(cfg, 3, "\t %s = %d/%s\n",
			elem->name, elem->val, elem->strval);

		en->elems[i++] = *elem;
	}

	PRINT_VERBOSE(cfg, 3, "enum %s (%d elems) fixed\n",
			en->name, en->cnt);

	/* fixup values */
	for(i = 0; i < en->cnt; i++) {
		elem = &en->elems[i];

		if (!elem->undef) {
			last = elem;
			continue;
		}

		if (elem->strval) {
			/* undefined text value, try to find it in
			 * previous enum definitions */

			struct lt_enum_elem *e;

			e = find_enumelem(cfg, elem->strval, en);
			if (!e) {
				printf("failed to find '%s=%s' enum definition\n",
				       elem->name, elem->strval);
				return -1;
			}

			elem->val = e->val;

		} else {
			/* undefined value -> take last defined + 1 */
			if (!last)
				elem->val = 0;
			else
				elem->val = last->val + 1;
		}

		PRINT_VERBOSE(cfg, 3, "\t %s = %d\n",
			elem->name, elem->val);

		last = elem;
	}

	/* finaly sort the array */
	qsort(en->elems, en->cnt, sizeof(struct lt_enum_elem), enum_comp);
	return 0;
}

struct lt_enum_elem* lt_args_get_enum(struct lt_config_shared *cfg, 
	char *name, char *val)
{
	struct lt_enum_elem* elem;

	if (NULL == (elem = malloc(sizeof(*elem))))
		return NULL;

	memset(elem, 0x0, sizeof(*elem));
	elem->undef = 1;

	if (val) {
		long num;
		char *endptr;

		errno = 0;
		num = strtol(val, &endptr, 10);

		/* parse errors */
		if ((errno == ERANGE && (num == LONG_MAX || num == LONG_MIN)) || 
		    (errno != 0 && num == 0)) {
			free(elem);
			return NULL;
		}

		if (endptr != val) {
			elem->val   = num;
			elem->undef = 0;
		} else {
			/* if no digits were found, we assume the
			 * value is set by string reference */
			elem->strval = strdup(val);
		}

	}

	elem->name = strdup(name);

	PRINT_VERBOSE(cfg, 3, "enum elem %s = %d, strval %s, undef = %d\n",
			elem->name, elem->val, elem->strval, elem->undef);
	return elem;
}

int lt_args_add_struct(struct lt_config_shared *cfg, char *type_name, 
			struct lt_list_head *h)
{
	struct lt_arg *arg, sarg;

	if ((args_def_struct_cnt + 1) == LT_ARGS_DEF_STRUCT_NUM)
		return 1;

	/* check if the struct name is already 
	   defined as a type */
	if (lt_args_getarg(cfg, type_name, NULL, 0, 1, NULL))
		return -1;

	memset(&sarg, 0, sizeof(sarg));
	sarg.dtype     = LT_ARGS_DTYPE_STRUCT;
	sarg.type_id   = LT_ARGS_TYPEID_CUSTOM + args_def_struct_cnt;
	sarg.type_name = type_name;
	sarg.args_head = h;

	PRINT_VERBOSE(cfg, 3, "struct [%s] type %d\n",
			sarg.type_name, sarg.type_id);

	lt_list_for_each_entry(arg, sarg.args_head, args_list) {

		PRINT_VERBOSE(cfg, 3, "\t %s %s %u\n",
				arg->type_name, arg->name, arg->type_len);

		/* This is not what sizeof would return on the structure.
		   The sizeof is arch dependent, this is pure sum. */
		sarg.type_len += arg->type_len;
		sarg.mmbcnt++;
	}

	args_def_struct[args_def_struct_cnt++] = sarg;

	PRINT_VERBOSE(cfg, 3, "%d.struct - final len = %u\n",
			args_def_struct_cnt, sarg.type_len);
	return 0;
}

int lt_args_add_sym(struct lt_config_shared *cfg, struct lt_arg *ret, 
			struct lt_list_head *h)
{
	ENTRY e, *ep;
	struct lt_args_sym *sym;
	struct lt_arg *arg;
	int i = 0;

	PRINT_VERBOSE(cfg, 3, "got symbol '%s %s'\n",
			ret->type_name, ret->name);

	if (NULL == (sym = (struct lt_args_sym*) malloc(sizeof(*sym))))
		return -1;

	memset(sym, 0, sizeof(*sym));
	sym->name = ret->name;

	sym->argcnt = 1;
	lt_list_for_each_entry(arg, h, args_list)
		sym->argcnt++;

	sym->args = (struct lt_arg**) malloc(sym->argcnt * sizeof(struct lt_arg**));
	if (!sym->args)
		/* no need to fre sym, since we are going
		   to exit the program anyway */
		return -1;

	PRINT_VERBOSE(cfg, 3, "got return %s, ptr %d\n",
			ret->type_name, ret->pointer);

	sym->args[i++] = ret;
	lt_list_for_each_entry(arg, h, args_list) {
		PRINT_VERBOSE(cfg, 3, "\t '%s %s'\n",
				arg->type_name, arg->name);
		sym->args[i++] = arg;
	}

	e.key = sym->name;
	e.data = sym;

	if (!hsearch_r(e, ENTER, &ep, &cfg->args_tab)) {
		perror("hsearch_r failed");
		free(sym);
		/* we dont want to exit just because 
		   we ran out of our symbol limit */
		PRINT_VERBOSE(cfg, 3, "reach the symbol number limit %u\n",
				LT_ARGS_TAB);
	} else
		PRINT_VERBOSE(cfg, 3, "got symbol %s (%d args)\n",
				sym->name, sym->argcnt);

	return 0;
}

static struct lt_arg* argdup(struct lt_config_shared *cfg, struct lt_arg *asrc)
{
	struct lt_arg *arg, *a;
        struct lt_list_head *h;

	PRINT_VERBOSE(cfg, 2, "got arg '%s %s', dtype %d\n",
			asrc->type_name, asrc->name, asrc->dtype);

	if (NULL == (arg = malloc(sizeof(*arg)))) {
		perror("malloc failed");
		return NULL;
	}

	*arg = *asrc;

	if (arg->dtype != LT_ARGS_DTYPE_STRUCT)
		return arg;

	/* For structures we need also to copy all its arguments. */

        if (NULL == (h = (struct lt_list_head*) malloc(sizeof(*h)))) {
		perror("malloc failed");
		free(arg);
		return NULL;
	}
                
        lt_init_list_head(h);

	lt_list_for_each_entry(a, asrc->args_head, args_list) {
		struct lt_arg *aa;

		/* XXX Not sure how safe is this one... 
		   might need some attention in future :) */
		if (NULL == (aa = argdup(cfg, a))) {
			free(h);
			free(arg);
			return NULL;
		}

		lt_list_add_tail(&aa->args_list, h);
	}
	
	arg->args_head = h;
	return arg;
}

static struct lt_arg* find_arg(struct lt_config_shared *cfg, char *type, 
			struct lt_arg argsdef[], int size, int create)
{
	int i;

	for(i = 0; i < size; i++) {
		struct lt_arg *arg;
		struct lt_arg adef = argsdef[i];

		PRINT_VERBOSE(cfg, 3, "%d. looking for [%s] - [%s]\n",
					i, type, adef.type_name);

		if (strcmp(type, adef.type_name))
			continue;

		if (!create)
			return &argsdef[i];

		arg = argdup(cfg, &adef);

		PRINT_VERBOSE(cfg, 3, "found %d\n", arg->type_id);
		return arg;
	}

	return NULL;
}

struct lt_arg* lt_args_getarg(struct lt_config_shared *cfg, char *type, 
			char *name, int pointer, int create, char *enum_name)
{
	struct lt_arg *arg;

	do {
		if ((arg = find_arg(cfg, type, 
			args_def_pod, LT_ARGS_DEF_POD_NUM, create)))
			break;

		if ((arg = find_arg(cfg, type, 
			args_def_struct, args_def_struct_cnt, create)))
			break;

		if ((arg = find_arg(cfg, type, 
			args_def_typedef, args_def_typedef_cnt, create)))
			break;

		return NULL;

	} while(0);

	if (!create)
		return arg;

	/* Find out the enum definition if the enum 
	   name is provided. */
	if (enum_name)
		arg->en = getenum(cfg, enum_name);

	arg->name    = strdup(name);

	/* If the type is already a pointer (could be for typedef), 
	   give it a chance to show up. There's only one pointer for 
	   the arg, since there's no reason to go dreper. */
	if (!arg->pointer)
		arg->pointer = pointer;

	return arg;
}

int lt_args_add_typedef(struct lt_config_shared *cfg, char *base, 
	char *new, int pointer)
{
	struct lt_arg *arg;
	int i;

	if ((args_def_typedef_cnt + 1) == LT_ARGS_DEF_TYPEDEF_NUM)
		return 2;

	/* check if the typedef name is already 
	   defined as a type */
	if (lt_args_getarg(cfg, new, NULL, 0, 0, NULL))
		return 1;

	do {
		if ((arg = find_arg(cfg, base, 
			args_def_pod, LT_ARGS_DEF_POD_NUM, 0)))
			break;

		if ((arg = find_arg(cfg, base, 
			args_def_typedef, args_def_typedef_cnt, 0)))
			break;

		PRINT_VERBOSE(cfg, 3, "%s not found\n", base);
		return -1;

	} while(0);

	PRINT_VERBOSE(cfg, 3, "got [%s]\n", new);

	args_def_typedef[i = args_def_typedef_cnt++] = *arg;

	arg = &args_def_typedef[i];
	arg->type_name = strdup(new);
	arg->pointer = pointer;

	lt_init_list_head(&arg->args_list);

	PRINT_VERBOSE(cfg, 3, "%d.typedef - got [%s] [%s]\n",
			args_def_typedef_cnt, base, arg->type_name);
	return 0;
}

int lt_args_init(struct lt_config_shared *cfg)
{
	char *file = LT_CONF_HEADERS_FILE;
	int ret = 0;

	if (!hcreate_r(LT_ARGS_TAB, &cfg->args_tab)) {
		perror("failed to create has table:");
		return -1;
	}

	lt_args_parse_init(cfg, &inc);

	if (*cfg->args_def)
		file = cfg->args_def;

	PRINT_VERBOSE(cfg, 1, "arguments definition file %s\n", file);

	if (lt_inc_open(cfg, &inc, file))
		return -1;

	if (lt_args_parse()) {
		printf("failed to header file(s) %s\n", file);
		ret = -1;
	}

#if defined(LT_ARGS_ARCH_CONF)
	/* Some architectures provides specific
	 * configuration file. */
	if (lt_inc_open(cfg, &inc, lt_args_arch_conf(cfg)))
		return -1;

	if (lt_args_parse()) {
		printf("failed to parse config file %s\n", file);
		ret = -1;
	}
#endif

	return ret;
}

static int getstr_addenum(struct lt_config_shared *cfg, struct lt_arg *arg,
			char *argbuf, int alen, long val)
{
	char *enstr = NULL;
	struct lt_enum_elem *elem;

	if (!arg->en)
		return 0;

	if (NULL != (elem = get_enumelem(cfg, val, arg->en)))
		enstr = elem->name;

	if (enstr)
		return snprintf(argbuf, alen, "%s", enstr);

	return 0;
}

static int getstr_pod(struct lt_config_shared *cfg, int dspname, struct lt_arg *arg, 
				void *pval, char *argbuf, int *arglen)
{
	int len = 0, alen = *arglen;
	int namelen = strlen(arg->name);

	PRINT_VERBOSE(cfg, 1, "\t arg '%s %s', pval %p, len %d, pointer %d, dtype %d, type_id %d\n",
			arg->type_name, arg->name, pval, alen, arg->pointer, arg->dtype, arg->type_id);

	if (alen < 5)
		return 0;
	
	*arglen = 0;

	if ((dspname) && 
	    (namelen < (alen - 5 - sizeof(LT_EQUAL)))) {
		*arglen  = sprintf(argbuf, "%s"LT_EQUAL, arg->name);
		argbuf  += *arglen;
		alen    -= *arglen;
	}

	/* Get enum resolve for pointers now, the rest 
	   POD is done in ARGS_SPRINTF macro. The char 
	   pointers need special handling later. */
	if ((arg->pointer) &&
	    (arg->type_id != LT_ARGS_TYPEID_CHAR)) {

		void *ptr = *((void**) pval);

		/* Try to get enumed value first. */
		len = getstr_addenum(cfg, arg, argbuf, alen, (long)ptr);

		/* If there's no enum resolved, 
		   just display the ptr value */
		if (!len) {
			if (ptr)
				len = snprintf(argbuf, alen, "%p", ptr);
			else
				len = snprintf(argbuf, alen, "NULL");
		}

		goto out;
	}

#define ARGS_SPRINTF(FMT, TYPE)                                      \
do {                                                                 \
	if (!(len = getstr_addenum(cfg, arg, argbuf, alen,           \
				(long) *((TYPE*) pval))))            \
		len = snprintf(argbuf, alen, FMT, *((TYPE*) pval));  \
} while(0)

	switch(arg->type_id) {
	case LT_ARGS_TYPEID_SHORT:  ARGS_SPRINTF("%hd", short); break;
	case LT_ARGS_TYPEID_USHORT: ARGS_SPRINTF("%hu", unsigned short); break;
	case LT_ARGS_TYPEID_INT:    ARGS_SPRINTF("%d", int); break;
	case LT_ARGS_TYPEID_UINT:   ARGS_SPRINTF("%u", unsigned int); break;
	case LT_ARGS_TYPEID_LONG:   ARGS_SPRINTF("%ld", long); break;
	case LT_ARGS_TYPEID_ULONG:  ARGS_SPRINTF("%lu", unsigned long); break;
	case LT_ARGS_TYPEID_LLONG:  ARGS_SPRINTF("%lld", long long); break;
	case LT_ARGS_TYPEID_ULLONG: ARGS_SPRINTF("%llu", unsigned long long); break;
	case LT_ARGS_TYPEID_DOUBLE: ARGS_SPRINTF("%lf", double); break;
	case LT_ARGS_TYPEID_FLOAT:  ARGS_SPRINTF("%f", float); break;
#undef ARGS_SPRINTF
	case LT_ARGS_TYPEID_CHAR:
		if (arg->pointer) {

			void *val = *((void**) pval);

			if (val) {
				char *s = val;
				int slen = strlen(s);
				int left = alen;
				int info_len = 0;

				if (lt_sh(cfg, args_string_pointer_length)) {
					info_len = snprintf(argbuf, left, "(%p, %zu) ", s, strlen(s));
					left -= info_len;
				}

				if ((slen + 2) > left) {
					snprintf(argbuf + info_len, left, "\"%s", s);
					strncpy(argbuf + left - sizeof("...\"") + 1, "...\"", sizeof("...\""));
				} else {
					strcpy(argbuf + info_len, "\"");
					strcat(argbuf, s);
					strcat(argbuf, "\"");
				}
			} else
				len = snprintf(argbuf, alen, "NULL");
		} else {

			if (!isprint(*((char*) pval)))
				len = snprintf(argbuf, alen, "0x%02x",
						*((unsigned char*) pval));
			else
				len = snprintf(argbuf, alen, "0x%02x \'%c\'",
						*((unsigned char*) pval), *((char*) pval));
		}
		break;

	case LT_ARGS_TYPEID_VOID:
		len = snprintf(argbuf, alen, "void");
		break;
	}

	if (LT_ARGS_DTYPE_STRUCT == arg->dtype) {
		if (pval)
			len = snprintf(argbuf, alen, "v(%p)", pval);
		else
			len = snprintf(argbuf, alen, "v(REG)");
				
	}

out:
	*arglen += strlen(argbuf);

	PRINT_VERBOSE(cfg, 1, "\t arg out len %d - [%s]\n",
			*arglen, argbuf);
	return 0;
}

int lt_args_cb_arg(struct lt_config_shared *cfg, struct lt_arg *arg, void *pval, 
		   struct lt_args_data *data, int last, int dspname)
{
	int len = data->arglen;

	PRINT_VERBOSE(cfg, 1, "arg '%s %s', pval %p, last %d\n",
				arg->type_name, arg->name, pval, last);

	getstr_pod(cfg, dspname, arg, pval, 
			data->args_buf + data->args_totlen, &len);
	data->args_totlen += len;

	if (!last) {
		strcat(data->args_buf, ", ");
		data->args_totlen += 2;
	}

	return 0;
}

int lt_args_cb_struct(struct lt_config_shared *cfg, int type, struct lt_arg *arg, 
		      void *pval, struct lt_args_data *data, int last)
{
	PRINT_VERBOSE(cfg, 1,
		"type %d, arg '%s %s', pval %p, last %d, pointer %d\n",
		type, arg->type_name, arg->name, pval, last, arg->pointer);

	/* initiall call for the structure argument */
	if (type == LT_ARGS_STRUCT_ITSELF) {

		data->argsd_totlen += sprintf(data->argsd_buf + data->argsd_totlen, 
						"struct %s %s = { ", 
						arg->type_name, arg->name);
		return 0;

	/* subsequent calls for all structure arguments */
	} else if (type == LT_ARGS_STRUCT_ARG) {

		int len = cfg->args_detail_maxlen - data->argsd_totlen;

		getstr_pod(cfg, 1, arg, pval, data->argsd_buf + data->argsd_totlen, &len);
		data->argsd_totlen += len;

		if (!last) {
			strcat(data->argsd_buf, ", ");
			data->argsd_totlen += 2;
		} else
			data->argsd_totlen += sprintf(data->argsd_buf + 
						      data->argsd_totlen, " }\n");
	}
	
	return 0;
}

static int getargs(struct lt_config_shared *cfg, struct lt_args_sym *asym, 
		La_regs *regs, char **abuf, char **adbuf)
{
	struct lt_args_data data;
	int arglen;
	char *buf, *bufd;

	if (NULL == (buf = malloc(cfg->args_maxlen)))
		return -1;

	memset(&data, 0, sizeof(data));

	*buf  = 0;
	*abuf = buf;

	if (cfg->args_detailed) {
		if (NULL == (bufd = malloc(cfg->args_detail_maxlen)))
			return -1;

		*bufd  = 0;
		*adbuf = bufd;
		data.argsd_buf = bufd;
		data.argsd_len = cfg->args_detail_maxlen;
	}


	/* makeup the final space for each 
	   argument textual representation  */
	arglen = (cfg->args_maxlen 
		- ((asym->argcnt - 1) * 2)  /* args separating commas */
		 )/ asym->argcnt;


	data.arglen   = arglen;
	data.args_buf = buf;
	data.args_len = cfg->args_maxlen;

	return lt_stack_process(cfg, asym, regs, &data);
}

struct lt_args_sym* lt_args_sym_get(struct lt_config_shared *cfg,
				    const char *sym)
{
	struct lt_args_sym *a;
	ENTRY e, *ep;

	PRINT_VERBOSE(cfg, 1, "request for <%s>\n", sym);

	e.key = (char*) sym;
	hsearch_r(e, FIND, &ep, &cfg->args_tab);

	if (!ep)
		return NULL;

	a = (struct lt_args_sym*) ep->data;

	PRINT_VERBOSE(cfg, 1, "found %p <%s>\n", a, a->name);
	return a;
}

int lt_args_sym_entry(struct lt_config_shared *cfg, struct lt_symbol *sym,
			La_regs *regs, char **argbuf, char **argdbuf)
{
	struct lt_args_sym *asym = sym ? sym->args : NULL;

	if (!asym)
		return -1;

	return getargs(cfg, asym, regs, argbuf, argdbuf);
}

static int getargs_ret(struct lt_config_shared *cfg, struct lt_args_sym *asym, 
		La_retval *regs, char **abuf, char **adbuf)
{
	struct lt_args_data data;
	int arglen, totlen;
	char *buf, *bufd;

	if (NULL == (buf = malloc(cfg->args_maxlen)))
		return -1;

	memset(&data, 0, sizeof(data));

	*buf  = 0;
	*abuf = buf;

	/* TODO get together with getargs function somehow... */
	if (cfg->args_detailed) {

		if (NULL == (bufd = malloc(cfg->args_detail_maxlen)))
			return -1;

		*bufd  = 0;
		*adbuf = bufd;
		data.argsd_buf = bufd;
		data.argsd_len = cfg->args_detail_maxlen;
	}

	arglen = cfg->args_maxlen - sizeof(LT_EQUAL);
	totlen = sizeof(LT_EQUAL) - 1;
	strcat(buf, LT_EQUAL);

	data.arglen      = arglen;
	data.args_buf    = buf;
	data.args_len    = cfg->args_maxlen;
	data.args_totlen = totlen;

	return lt_stack_process_ret(cfg, asym, regs, &data);
}

int lt_args_sym_exit(struct lt_config_shared *cfg, struct lt_symbol *sym,
			La_regs *inregs, La_retval *outregs,
			char **argbuf, char **argdbuf)
{
	struct lt_args_sym *asym = sym ? sym->args : NULL;

	if (!asym)
		return -1;

	return getargs_ret(cfg, asym, outregs, argbuf, argdbuf);
}
