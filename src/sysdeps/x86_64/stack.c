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


#include "config.h"
#include "stack.h"

static __thread int ass_integer;
static __thread int ass_sse;
static __thread int ass_memory;

#define ASS_CLEANUP() \
do { \
	ass_integer = ass_sse = ass_memory = 0; \
} while(0)

/* INTEGER registers used for function arguments. */
static long ass_regs_integer[] = {
	LT_64_REG_RDI,
	LT_64_REG_RSI,
	LT_64_REG_RDX,
	LT_64_REG_RCX,
	LT_64_REG_R8,
	LT_64_REG_R9,
};
#define ASS_REGS_INTEGER_CNT  (sizeof(ass_regs_integer)/sizeof(long))
#define ASS_REGS_INTEGER_SIZE (sizeof(ass_regs_integer))

/* INTEGER registers used for function return arguments. */
static long ass_regs_integer_ret[] = {
	LT_64_REG_RAX,
	LT_64_REG_RDX,
};
#define ASS_REGS_INTEGER_RET_CNT  (sizeof(ass_regs_integer_ret)/sizeof(long))
#define ASS_REGS_INTEGER_RET_SIZE (sizeof(ass_regs_integer_ret))

/* SSE registers used for function arguments. */
static long ass_regs_sse[] = {
	LT_64_REG_XMM0,
	LT_64_REG_XMM1,
	LT_64_REG_XMM2,
	LT_64_REG_XMM3,
	LT_64_REG_XMM4,
	LT_64_REG_XMM5,
	LT_64_REG_XMM6,
	LT_64_REG_XMM7,
};
#define ASS_REGS_SSE_CNT  (sizeof(ass_regs_sse)/sizeof(long))

/* SSE registers used for function return arguments. They 
   are xmm0 and xmm1, so we reuse the ass_regs_sse array.*/
#define ASS_REGS_SSE_RET_CNT  (2)

static int classificate_arg(struct lt_config_shared *cfg, struct lt_arg *arg,
				int ret, int align);

static int classificate_memory(struct lt_config_shared *cfg, 
			struct lt_arg *arg, int align)
{
	if (align)
		ass_memory = LT_STACK_ALIGN(ass_memory);
	else {
		int naligned = ass_memory % arg->type_len;

		if (naligned)
			ass_memory += arg->type_len - naligned;
	}

	PRINT_VERBOSE(cfg, 2, "%s - ass %d\n",
			arg->name, ass_memory);

	ARCH_SET(arg, ARCH_FLAG_MEM, ass_memory);

	if (arg->pointer)
		ass_memory += sizeof(void*);
	else
		ass_memory += arg->type_len;
		
	return 0;
}

static int classificate_integer(struct lt_config_shared *cfg,
			struct lt_arg *arg, int align, int regs_size)
{
	if (!align) {
		int ass = LT_STACK_ALIGN(ass_integer);
		int naligned = ass_integer % arg->type_len;

		if (naligned)
			ass_integer += arg->type_len - naligned;

		if ((ass_integer + arg->type_len) > ass)
			ass_integer = LT_STACK_ALIGN(ass_integer);
	}

	PRINT_VERBOSE(cfg, 2,
			"ass %s - reg size %d - ass_integer %d\n", 
			arg->name, regs_size, ass_integer);

	if (ass_integer >= regs_size)
		return -1;

	ARCH_SET(arg, ARCH_FLAG_REG_INTEGER, ass_integer);

	if (arg->pointer)
		ass_integer += sizeof(void*);
	else
		ass_integer += align ? sizeof(void*) : arg->type_len;

	return 0;
}

static int classificate_sse(struct lt_config_shared *cfg, struct lt_arg *arg,
				int sse_cnt)
{
	PRINT_VERBOSE(cfg, 2, "ass %s %d %d\n",
			arg->name, ASS_REGS_SSE_CNT, ass_sse);

	if (sse_cnt == ass_sse)
		return -1;

	ARCH_SET(arg, ARCH_FLAG_REG_SSE, ass_sse++);
	return 0;
}

static void struct_arch(struct lt_config_shared *cfg, struct lt_arg *sarg,
			struct lt_arg *farg)
{
	if (sarg->pointer)
		return;

	/* Structures passed by value dont have the arch assigned by default.
	   If the first argument is returned in memory, the structure takes
	   this pointer, if it is register, we keep it NULL, and lt_args_cb_arg
	    will print out the REG kw for value.*/
	PRINT_VERBOSE(cfg, 2,
			"first argument for struct %s has flag: %d\n", 
			sarg->name, ARCH_GET_FLAG(farg));

	if (ARCH_GET_FLAG(farg) == ARCH_FLAG_MEM)
		sarg->arch = farg->arch;
	else
		ARCH_SET(sarg, ARCH_FLAG_SVREG, 0);
}


static int classificate_struct_try(struct lt_config_shared *cfg, 
			struct lt_arg *arg, int allmem, int ret)
{
	struct lt_arg *a;
	int first = 1;
	int saved_ass_integer = ass_integer;
	int saved_ass_sse     = ass_sse;
	int saved_ass_memory  = ass_memory;

	lt_list_for_each_entry(a, arg->args_head, args_list) {

		if (allmem)
			classificate_memory(cfg, a, first);
		else {
			if (-1 == classificate_arg(cfg, a, ret, 0))
				return -1;

			if (ARCH_GET_FLAG(arg) == ARCH_FLAG_MEM) {
				/* There is not enough registers, reset to 
				   have the structure in memory only. */
				ass_integer = saved_ass_integer;
				ass_sse     = saved_ass_sse;
				ass_memory  = saved_ass_memory;
				return -1;
			}

		}

		if (first) {
			struct_arch(cfg, arg, a);
			first = 0;
		}
	}

	return 0;
}

static int get_sizeof(struct lt_config_shared *cfg, struct lt_arg *arg)
{
	struct lt_arg *a;
	int size = 0;

	lt_list_for_each_entry(a, arg->args_head, args_list) {

		int naligned = (u_int) size % a->type_len;

		if (naligned)
			size += a->type_len - naligned;

		size += a->type_len;
	}

	PRINT_VERBOSE(cfg, 2, "sizeof(struct %s) = %d\n",
		arg->type_name, size);

	return size;
}

static int classificate_struct(struct lt_config_shared *cfg, struct lt_arg *arg,
				int ret)
{
	int allmem = 0;
	int size = get_sizeof(cfg, arg);

	/* If the structure is bigger than 16B or passed as pointer then 
	   it is in the memory (no registers). If the structure is up to 
	   16B and passed by value, we might need t classificate its members,
	   because they could fit in registers. */
	if ((size > 16) || (arg->pointer))
		allmem = 1;

	PRINT_VERBOSE(cfg, 2,
		"struct %s - length sum %d - allmem %d\n", 
		arg->type_name, arg->type_len, allmem);

	if (-1 == classificate_struct_try(cfg, arg, allmem, ret))
		classificate_struct_try(cfg, arg, 1, ret);

	return 0;
}

static int classificate_arg_type(struct lt_config_shared *cfg, 
				struct lt_arg *arg)
{
	int class;

	do {
		/* pointers are INTEGER class by default */
		if (arg->pointer) {
			class = LT_CLASS_INTEGER;
			break;
		}

		switch(arg->type_id) {
		case LT_ARGS_TYPEID_CHAR:
		case LT_ARGS_TYPEID_UCHAR:
		case LT_ARGS_TYPEID_SHORT:
		case LT_ARGS_TYPEID_USHORT:
		case LT_ARGS_TYPEID_INT:
		case LT_ARGS_TYPEID_UINT:
		case LT_ARGS_TYPEID_LONG:
		case LT_ARGS_TYPEID_ULONG:
		case LT_ARGS_TYPEID_LLONG:
		case LT_ARGS_TYPEID_ULLONG:
			class = LT_CLASS_INTEGER;
			break;

		case LT_ARGS_TYPEID_DOUBLE:
		case LT_ARGS_TYPEID_FLOAT:
			class = LT_CLASS_SSE;
			break;

		default:
			class = LT_CLASS_NONE;
			break;
		}

	} while(0);

	PRINT_VERBOSE(cfg, 2,
			"argument %s dtype %d - type %s(%d) - class %d\n", 
			arg->name, arg->dtype, arg->type_name, arg->type_id, 
			class);
	return class;
}

static int classificate_arg(struct lt_config_shared *cfg, struct lt_arg *arg,
				int ret, int align)
{
	int class;
	int class_failed = 0;

	PRINT_VERBOSE(cfg, 2, "got arg \"%s\"\n",
			arg->name);

	ARCH_ZERO(arg);

	if (arg->dtype != LT_ARGS_DTYPE_POD) {

		if (-1 == classificate_struct(cfg, arg, ret))
			return -1;

		/* If the structure is passed by pointer, we 
		   still need the pointer classification. */
		if (!arg->pointer)
			return 0;
	}

	class = classificate_arg_type(cfg, arg);
	if (-1 == class)
		return -1;

	/* Nothing to be done for NONE class (void type) */
	if (LT_CLASS_NONE == class)
		return 0;

	switch(class) {
	case LT_CLASS_SSE:
		class_failed = classificate_sse(cfg, arg, 
				ret ? ASS_REGS_SSE_RET_CNT : 
				      ASS_REGS_SSE_CNT);
		break;
			
	case LT_CLASS_INTEGER:
		class_failed = classificate_integer(cfg, arg, align,
				ret ? ASS_REGS_INTEGER_RET_SIZE : 
				      ASS_REGS_INTEGER_SIZE);
		break;

	case LT_CLASS_MEMORY:
		classificate_memory(cfg, arg, 1);
		break;
	}

	/* If class INTEGER or SSE ran out of registers, 
	   then arg is in memory. */
	if (class_failed)
		classificate_memory(cfg, arg, 1);

	return 0;
}

static int classificate(struct lt_config_shared *cfg, struct lt_args_sym *sym)
{
	int i;
	struct lt_arg *arg = sym->args[LT_ARGS_RET];

	PRINT_VERBOSE(cfg, 2, "got symbol \"%s\"\n",
			sym->name);

	ASS_CLEANUP();

	/* Classificate the return value first. */
	if (-1 == classificate_arg(cfg, arg, 1, 1))
		return -1;

	ASS_CLEANUP();

	/* If the return value is memory class, 
	   then the edi register is used as a first hidden arg.*/
	if (ARCH_GET_FLAG(arg) == ARCH_FLAG_MEM)
		ass_integer += 8;

	for(i = 1; i < sym->argcnt; i++) {
		arg = sym->args[i];

		if (-1 == classificate_arg(cfg, arg, 0, 1))
			return -1;
	}

	return 0;
}

static void *get_value_mem(struct lt_config_shared *cfg, struct lt_arg *arg, 
			void *regs, int ret)
{
	long offset = ARCH_GET_OFFSET(arg);
	void *pval;

	if (ret) {
		void *base = (void*) ((struct La_x86_64_retval*) regs)->lrv_rax;
		pval = base + offset;
	} else {
		void *base = (void*) ((struct La_x86_64_regs*) regs)->lr_rsp;
		pval = base +            /* current stack pointer */
		       sizeof(void*) +   /* return function address */
		       offset;
	}

	PRINT_VERBOSE(cfg, 2, "offset = %ld, %s = %p, ret = %d\n",
			offset, arg->name, pval, ret);
	return pval;
}

static void *get_value_reg_integer(struct lt_config_shared *cfg,
			struct lt_arg *arg, void *regs, int ret)
{
	struct La_x86_64_retval *regs_ret = regs;
	struct La_x86_64_regs   *regs_in  = regs;
	void *pval  = NULL;
	long offset = ARCH_GET_OFFSET(arg);
	long qoff   = offset % sizeof(long);
	long reg    = ret ? ass_regs_integer_ret[offset / sizeof(long)] :
			    ass_regs_integer[offset / sizeof(long)];
			

	PRINT_VERBOSE(cfg, 2,
			"offset %ld - reg %ld - qoff %ld - ASS_REGS_CNT %ld - ret %d\n", 
			offset, reg, qoff, ASS_REGS_INTEGER_CNT, ret);

	switch(reg) {
	case LT_64_REG_RAX:
		pval = &regs_ret->lrv_rax;
		break;
	case LT_64_REG_RDX:
		pval = ret ? &regs_ret->lrv_rdx : &regs_in->lr_rdx;
		break;
	case LT_64_REG_RDI:
		pval = &regs_in->lr_rdi;
		break;
	case LT_64_REG_RSI:
		pval = &regs_in->lr_rsi;
		break;
	case LT_64_REG_RCX:
		pval = &regs_in->lr_rcx;
		break;
	case LT_64_REG_R8:
		pval = &regs_in->lr_r8;
		break;
	case LT_64_REG_R9:
		pval = &regs_in->lr_r9;
		break;
	}

	pval += qoff;

	PRINT_VERBOSE(cfg, 2, "argument %s = %p (%lx)\n",
			arg->name, pval, *((u_long*)pval));
	return pval;
}

static void *get_value_reg_sse(struct lt_config_shared *cfg,
			struct lt_arg *arg, void *regs, int ret)
{
	int i = ARCH_GET_OFFSET(arg);
	void *pval = NULL;

	if (ret) {
		struct La_x86_64_retval *regs_ret = regs;

		switch(i) {
		case 0: 
			pval = &regs_ret->lrv_xmm0; 
			break;
		case 1: 
			pval = &regs_ret->lrv_xmm1; 
			break;
		}
	} else {
		struct La_x86_64_regs *regs_in = regs;

		pval = &regs_in->lr_xmm[i];
	}


	PRINT_VERBOSE(cfg, 2, "argument %s = %p (%lx), ret %d\n",
			arg->name, pval, *((u_long*)pval), ret);
	return pval;
}

static void* get_value(struct lt_config_shared *cfg, struct lt_arg *arg,
			void *regs, int ret)
{
	void *pval = NULL;

	PRINT_VERBOSE(cfg, 2, "get value for %s - arch = %lx, flag = %d\n",
			arg->name, ARCH_GET(arg), ARCH_GET_FLAG(arg));

	switch(ARCH_GET_FLAG(arg)) {
	case ARCH_FLAG_MEM:
		pval = get_value_mem(cfg, arg, regs, ret);
		break;

	case ARCH_FLAG_REG_INTEGER:
		pval = get_value_reg_integer(cfg, arg, regs, ret);
		break;

	case ARCH_FLAG_REG_SSE:
		pval = get_value_reg_sse(cfg, arg, regs, ret);
		break;
	}

	return pval;
}

/* Process structure stored completelly in the 
   memory - pointed to by 'pval' arg. */
static int process_struct_mem(struct lt_config_shared *cfg, struct lt_arg *arg,
				void *pval, struct lt_args_data *data)
{
	struct lt_arg *a;
	int i = 0;

	PRINT_VERBOSE(cfg, 2, "for %s - arch = %llx\n",
			arg->name, ARCH_GET(arg));

	lt_args_cb_struct(cfg, LT_ARGS_STRUCT_ITSELF, arg, NULL, data, 0);

	lt_list_for_each_entry(a, arg->args_head, args_list) {
		int last = (i + 1) == arg->mmbcnt;
		void *pv = pval + ARCH_GET_OFFSET(a);

		lt_args_cb_struct(cfg, LT_ARGS_STRUCT_ARG, a, pv, data, last);

		i++;
	}

	return 0;
}

static int process_struct_arg(struct lt_config_shared *cfg, struct lt_arg *arg,
			void *regs, struct lt_args_data *data, int ret)
{
	struct lt_arg *a;
	int i = 0;

	PRINT_VERBOSE(cfg, 2, "for %s - arch = %llx\n",
			arg->name, ARCH_GET(arg));

	lt_args_cb_struct(cfg, LT_ARGS_STRUCT_ITSELF, arg, NULL, data, 0);

	lt_list_for_each_entry(a, arg->args_head, args_list) {
		int last = (i + 1) == arg->mmbcnt;
		void *pval = NULL;

		pval = get_value(cfg, a, regs, ret);

		lt_args_cb_struct(cfg, LT_ARGS_STRUCT_ARG, a, pval, data, last);
		i++;
	}

	return 0;
}

static void process_detailed_struct(struct lt_config_shared *cfg, 
		struct lt_arg *arg, void *pval, struct lt_args_data *data, 
		void *regs, int ret)
{
	if (arg->pointer)
		pval = *((void**) pval);

	if (ARCH_GET_FLAG(arg) == ARCH_FLAG_MEM) {
		process_struct_mem(cfg, arg, pval, data);
	} else {
		if (arg->pointer)
			process_struct_mem(cfg, arg, pval, data);
		else
			process_struct_arg(cfg, arg, regs, data, ret);
	}
}
		

int lt_stack_process(struct lt_config_shared *cfg, struct lt_args_sym *asym, 
			La_regs *regs, struct lt_args_data *data)
{
	int i;

	if (!ARCH_GET(asym->args[LT_ARGS_RET]) &&
	    (-1 == classificate(cfg, asym)))
		return -1;

	for(i = 1; i < asym->argcnt; i++) {
		void *pval = NULL;
		struct lt_arg *arg = asym->args[i];
		int last = (i + 1) == asym->argcnt;

		pval = get_value(cfg, arg, regs, 0);
		if ((!pval) && 
		    (arg->pointer || (LT_ARGS_DTYPE_STRUCT != arg->dtype))) {
			PRINT_VERBOSE(cfg, 2,
				"THIS SHOULD NEVER HAPPEN - arg '%s %s'\n",
				arg->type_name, arg->name);
			continue;
		}

		lt_args_cb_arg(cfg, arg, pval, data, last, 1);

		if ((cfg->args_detailed) && 
		    (LT_ARGS_DTYPE_STRUCT == arg->dtype))
			process_detailed_struct(cfg, arg, pval, data, regs, 0);
	}

	return 0;
}

int lt_stack_process_ret(struct lt_config_shared *cfg, struct lt_args_sym *asym,
			La_retval *regs, struct lt_args_data *data)
{
	struct lt_arg *arg;
	void *pval;

	arg = asym->args[LT_ARGS_RET];
	pval = get_value(cfg, arg, regs, 1);

	lt_args_cb_arg(cfg, arg, pval, data, 1, 0);

	if ((cfg->args_detailed) &&
	    (LT_ARGS_DTYPE_STRUCT == arg->dtype))
		process_detailed_struct(cfg, arg, pval, data, regs, 0);

	return 0;
}
