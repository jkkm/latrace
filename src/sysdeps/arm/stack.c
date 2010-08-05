/*
  ARM EABI argument extraction

  Copyright (C) 2009, 2010 Akos Pasztory <akos.pasztory@gmail.com>

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

static inline unsigned long align_up(unsigned long v, unsigned long a)
{
	return (v + a-1) & ~(a-1);
}

/*
 * The .arch field is split the following way:
 * -- valid  :1
 * -- size   :15
 * -- offset :16
 */

static inline void arch_set(struct lt_arg *arg,
			    unsigned int size, unsigned int offs)
{
	arg->arch = (void *)((size << 16) | offs | 0x80000000);
}

static inline int arch_valid(struct lt_arg *arg)
{
	return (unsigned int)arg->arch & 0x80000000;
}

static inline unsigned int arch_get_size(struct lt_arg *arg)
{
	return ((unsigned int)arg->arch & ~0x80000000) >> 16;
}

static inline unsigned int arch_get_offs(struct lt_arg *arg)
{
	return (unsigned int)arg->arch & 0xffff;
}

/* Processes a struct entirely in memory. */
static int process_struct_mem(struct lt_config_shared *cfg, struct lt_arg *arg,
			      void *pval, struct lt_args_data *data)
{
	int i;
	unsigned long npval;
	struct lt_arg *a;

	lt_args_cb_struct(cfg, LT_ARGS_STRUCT_ITSELF, arg, 0, data, 0);
	npval = (unsigned long)pval;
	i = 0;
	lt_list_for_each_entry(a, arg->args_head, args_list) {
		lt_args_cb_struct(cfg, LT_ARGS_STRUCT_ARG, a,
				  (void *)(npval + arch_get_offs(a)),
				  data, i+1 == arg->mmbcnt);
		++i;
	}
	return 0;
}

/* Returns a bytearray slice from OFFS with SIZE length, treating the
 * registers and the stack consecutively.  Return value is static,
 * only valid until the next get() call. */
static void *get(unsigned long offs, int size, La_regs *regs)
{
	/* NB: size of buf is fixed.  But if we only copy POD types, hopefully
	 * it won't become a problem. */
	static __thread char buf[64];
	int i;

	for (i = 0; i < size; ++i) {
		if (offs+i < 16)
			buf[i] = ((char *)regs->lr_reg)[offs+i];
		else
			buf[i] = *(char *)(regs->lr_sp + (offs+i-16));
	}
	return buf;
}

/* Calculate member sizes and offsets of a structure.  Returns the
 * size of the entire structure. */
static unsigned int calcstruct(struct lt_arg *arg)
{
	unsigned int maxl, moffs;
	struct lt_arg *a;

	/* XXX: this is incorrect for struct-in-struct... */
	maxl = moffs = 0;
	lt_list_for_each_entry(a, arg->args_head, args_list) {
		/* We need to know the strictest aligned member to
		 * determine the size of the entire struct. */
		if (a->type_len > maxl)
			maxl = a->type_len;
		/* Align members naturally. */
		moffs = align_up(moffs, a->type_len);
		arch_set(a, a->type_len, moffs);
		moffs += a->type_len;
	}
	return align_up(moffs, maxl);
}

/* Calculates all offsets and `real' (padded) sizes of arguments
 * (including struct members).  Stores them in ->arch. */
static void calcofs(struct lt_args_sym *asym)
{
	int i;
	unsigned int offs;
	struct lt_arg *arg;

	/* If function returns a struct by value larger than a word,
	 * it is stored in callee-allocated memory and r0 contains the
	 * address. */
	arg = asym->args[LT_ARGS_RET];
	offs = 0;
	if (arg->dtype == LT_ARGS_DTYPE_STRUCT) {
		if (!arg->pointer)
			offs = 4;
		arch_set(arg, calcstruct(arg), 0);
	}
	for (i = 1; i < asym->argcnt; ++i) {
		unsigned int asize;

		arg = asym->args[i];
		/* Struct sizes/offsets should be calculated at all times. */
		if (arg->dtype == LT_ARGS_DTYPE_STRUCT)
			calcstruct(arg);
		if (arg->dtype != LT_ARGS_DTYPE_STRUCT || arg->pointer) {
			/* Non-composite type. */
			/* Size is 4 if arg is smaller than a word. */
			asize = arg->type_len;
			if (asize < 4)
				asize = 4;
			/* Types are aligned to their sizes. */
			offs = align_up(offs, asize);
			arch_set(arg, asize, offs);
		} else {
			/* Composite type. */
			asize = calcstruct(arg);
			arch_set(arg, asize, offs);
		}
		offs += asize;
		/* If we are allocating from registers (4*4 bytes),
		 * each arg must start in a new one. */
		if (offs < 16)
			offs = align_up(offs, 4);
	}
}

/*
 * -- args which fit are in r0..r3, rest is on the stack
 * -- if return type is struct, r0 contains the address for it
 * -- dword sized args must be aligned at even registers
 * -- one argument may be split between registers and stack
 * -- on stack, 8byte values are aligned % 8 == 0
 */
int lt_stack_process(struct lt_config_shared *cfg, struct lt_args_sym *asym,
		     La_regs *regs, struct lt_args_data *data)
{
	int i;

	for (i = 1; i < asym->argcnt; ++i) {
		struct lt_arg *arg;
		void *p;
		unsigned int o, lastp;

		lastp = i+1 == asym->argcnt;
		arg = asym->args[i];
		if (!arch_valid(arg))
			calcofs(asym);
		/* POD */
		if (arg->dtype != LT_ARGS_DTYPE_STRUCT) {
			p = get(arch_get_offs(arg), arch_get_size(arg), regs);
			lt_args_cb_arg(cfg, arg, p, data, lastp, 1);
			continue;
		}
		/* struct pointer */
		if (arg->pointer) {
			p = get(arch_get_offs(arg), sizeof(void *), regs);
			p = *(void **)p;
			lt_args_cb_arg(cfg, arg, p, data, lastp, 1);
			if (cfg->args_detailed)
				process_struct_mem(cfg, arg, p, data);
			continue;
		}
		/* struct by value */
		o = arch_get_offs(arg);
		/* If part of the struct is in registers this outputs 'REG',
		 * otherwise, the stack address is printed. */
		lt_args_cb_arg(cfg, arg,
			       (void *)(o < 16 ? 0 : regs->lr_sp + o-16),
			       data, lastp, 1);
		if (cfg->args_detailed) {
			int j;
			struct lt_arg *m;

			j = 0;
			lt_args_cb_struct(cfg, LT_ARGS_STRUCT_ITSELF, arg,
					  0, data, 0);
			lt_list_for_each_entry(m, arg->args_head, args_list) {
				unsigned int o;

				o = arch_get_offs(arg) + arch_get_offs(m);
				p = get(o, arch_get_size(m), regs);
				lt_args_cb_struct(cfg, LT_ARGS_STRUCT_ARG, m,
						  p, data, j+1 == arg->mmbcnt);
				++j;
			}
		}
	}
	return 0;
}

/*
 * -- 32bit return values in r0
 * -- 64bit in {r0, r1}
 * -- composite < 4bytes in r0
 * -- composite > 4bytes in memory (address is passed as the first parameter
 *    of the call)
 */
int lt_stack_process_ret(struct lt_config_shared *cfg,
			 struct lt_args_sym *asym, La_retval *regs,
			 struct lt_args_data *data)
{
	struct lt_arg *arg;
	void *pval;

	/* I think this works for both 32 and 64 bit values (r0 and r1
	 * are consecutive in lrv_reg[]). */
	pval = &(regs->lrv_reg[0]);
	arg = asym->args[LT_ARGS_RET];
	lt_args_cb_arg(cfg, arg, pval, data, 1, 0);
	if (cfg->args_detailed && arg->dtype == LT_ARGS_DTYPE_STRUCT) {
		unsigned int asize;
		/* AAPCS says: a composite type less than 4 bytes is
		 * returned in r0.  Larger types have memory allocated
		 * by caller and its address passed as the (hidden)
		 * 1st argument.
		 *
		 * NB: Though I, personally, haven't found it in the
		 * AAPCS, it's both logical (and apparently gcc does
		 * so) to return this address in r0. */
		asize = arch_get_size(arg);
		if (arg->pointer || asize > 4)
			pval = (void *)regs->lrv_reg[0];
		else if (asize > 4)
			pval = &regs->lrv_reg[0];
		process_struct_mem(cfg, arg, pval, data);
	}
	return 0;
}
/* End of stack.c */
