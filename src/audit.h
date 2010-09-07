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


#ifndef AUDIT_H
#define AUDIT_H

#include <link.h>

/* stolen from glibc :) */
#ifdef __i386__
# define pltenter la_i86_gnu_pltenter
# define pltexit la_i86_gnu_pltexit
# define La_regs La_i86_regs
# define sp_reg lr_esp
# define La_retval La_i86_retval
# define int_retval lrv_eax
#elif defined __x86_64__
# define pltenter la_x86_64_gnu_pltenter
# define pltexit la_x86_64_gnu_pltexit
# define La_regs La_x86_64_regs
# define sp_reg lr_rsp
# define La_retval La_x86_64_retval
# define int_retval lrv_rax
#elif defined __arm__
# define pltenter la_arm_gnu_pltenter
# define pltexit la_arm_gnu_pltexit
# define La_regs La_arm_regs
# define sp_reg lr_sp
# define La_retval La_arm_retval
# define int_retval lrv_reg[0]
#elif defined __powerpc__ && __WORDSIZE == 32
# define pltenter la_ppc32_gnu_pltenter
# define pltexit la_ppc32_gnu_pltexit
# define La_regs La_ppc32_regs
# define La_retval La_ppc32_retval
# define int_retval lrv_r3
#elif defined __powerpc__ && __WORDSIZE == 64
# define pltenter la_ppc64_gnu_pltenter
# define pltexit la_ppc64_gnu_pltexit
# define La_regs La_ppc64_regs
# define La_retval La_ppc64_retval
# define int_retval lrv_r3
#elif defined __sh__
# define pltenter la_sh_gnu_pltenter
# define pltexit la_sh_gnu_pltexit
# define La_regs La_sh_regs
# define La_retval La_sh_retval
# define int_retval lrv_r0
#elif defined __alpha__
# define pltenter la_alpha_gnu_pltenter
# define pltexit la_alpha_gnu_pltexit
# define La_regs La_alpha_regs
# define La_retval La_alpha_retval
# define int_retval lrv_r0
#elif defined __s390__ && __WORDSIZE == 32
# define pltenter la_s390_32_gnu_pltenter
# define pltexit la_s390_32_gnu_pltexit
# define La_regs La_s390_32_regs
# define La_retval La_s390_32_retval
# define int_retval lrv_r2
#elif defined __s390__ && __WORDSIZE == 64
# define pltenter la_s390_64_gnu_pltenter
# define pltexit la_s390_64_gnu_pltexit
# define La_regs La_s390_64_regs
# define La_retval La_s390_64_retval
# define int_retval lrv_r2
#elif defined __ia64__
# define pltenter la_ia64_gnu_pltenter
# define pltexit la_ia64_gnu_pltexit
# define La_regs La_ia64_regs
# define La_retval La_ia64_retval
# define int_retval lrv_r8
#elif defined __sparc__ && __WORDSIZE == 32
# define pltenter la_sparc32_gnu_pltenter
# define pltexit la_sparc32_gnu_pltexit
# define La_regs La_sparc32_regs
# define La_retval La_sparc32_retval
# define int_retval lrv_reg[0]
#elif defined __sparc__ && __WORDSIZE == 64
# define pltenter la_sparc64_gnu_pltenter
# define pltexit la_sparc64_gnu_pltexit
# define La_regs La_sparc64_regs
# define La_retval La_sparc64_retval
# define int_retval lrv_reg[0]
#endif

#endif // !AUDIT_H
