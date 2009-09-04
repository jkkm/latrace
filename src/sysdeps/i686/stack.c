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

static int process_struct(struct lt_config_shared *cfg, struct lt_arg *arg,
	void *pval, struct lt_args_data *data)
{
	struct lt_arg *a;
	int i = 0;

	lt_args_cb_struct(cfg, LT_ARGS_STRUCT_ITSELF, arg, pval, data, 0);

	if (arg->pointer)
		pval = *((void**) pval);

	lt_list_for_each_entry(a, arg->args_head, args_list) {
		int last = (i + 1) == arg->mmbcnt;

		/* For the type size up to word (4), the offset of the member 
		   is aligned to its size.  For the type size over the word, 
		   the offset of the member is aligned to word. */

		int nsize = a->type_len > 4 ? 4 : a->type_len;
		int naligned = (u_int) pval % nsize;

		if (naligned)
			pval += nsize - naligned;

		lt_args_cb_struct(cfg, LT_ARGS_STRUCT_ARG, a, pval, data, last);

		pval += a->type_len;
		i++;
	}

	return 0;
}

/*
  the traced program stack (in regs) 
  should look like this:

  ...
  esp + 12  2nd argument
  esp + 8   1st argument
  esp + 4   possible return structure/union address
  esp       return function address

*/
int lt_stack_process(struct lt_config_shared *cfg, struct lt_args_sym *asym,
	La_regs *regs, struct lt_args_data *data)
{
	int i;
	void *pval;
	struct lt_arg *argret;

	/* get the esp reg and skip the return address */
	pval = (void*) regs->lr_esp;
	pval += sizeof(void*);


	/* if the function returns structure by value,
	   there's a hidden first argument we need to skip */
	argret = asym->args[LT_ARGS_RET];
	if ((!argret->pointer) &&
	    (argret->dtype == LT_ARGS_DTYPE_STRUCT))
		pval += sizeof(void*);


	for(i = 1; i < asym->argcnt; i++) {

		struct lt_arg *arg = asym->args[i];
		int last = (i + 1) == asym->argcnt;

		lt_args_cb_arg(cfg, arg, pval, data, last, 1);

		if ((cfg->args_detailed) && 
		    (LT_ARGS_DTYPE_STRUCT == arg->dtype))
			process_struct(cfg, arg, pval, data);

		pval += arg->pointer ? sizeof(void*) : LT_STACK_ALIGN(arg->type_len);
	}

	return 0;
}

/* x86 is easy, everything is in eax register */
int lt_stack_process_ret(struct lt_config_shared *cfg, struct lt_args_sym *asym,
	La_retval *regs, struct lt_args_data *data)
{
	struct lt_arg *arg;
	void *pval;
	
	pval = (void*) &(regs->lrv_eax);
	arg = asym->args[LT_ARGS_RET];

	lt_args_cb_arg(cfg, arg, pval, data, 1, 0);

	if ((cfg->args_detailed) && 
	    (LT_ARGS_DTYPE_STRUCT == arg->dtype)) {

		/* The process_struct function does its own 
		   dereference of pval value in case of pointer 
		   argument, so we need to prepare pval correctly. */
		if (!arg->pointer)
			pval = (void*) regs->lrv_eax;

		process_struct(cfg, arg, pval, data);
	}

	return 0;
}
