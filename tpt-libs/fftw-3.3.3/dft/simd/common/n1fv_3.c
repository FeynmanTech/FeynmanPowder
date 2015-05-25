/*
 * Copyright (c) 2003, 2007-11 Matteo Frigo
 * Copyright (c) 2003, 2007-11 Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/* This file was automatically generated --- DO NOT EDIT */
/* Generated on Sun Nov 25 07:36:52 EST 2012 */

#include "codelet-dft.h"

#ifdef HAVE_FMA

/* Generated by: ../../../genfft/gen_notw_c.native -fma -reorder-insns -schedule-for-pipeline -simd -compact -variables 4 -pipeline-latency 8 -n 3 -name n1fv_3 -include n1f.h */

/*
 * This function contains 6 FP additions, 4 FP multiplications,
 * (or, 3 additions, 1 multiplications, 3 fused multiply/add),
 * 11 stack variables, 2 constants, and 6 memory accesses
 */
#include "n1f.h"

static void n1fv_3(const R *ri, const R *ii, R *ro, R *io, stride is, stride os, INT v, INT ivs, INT ovs)
{
     DVK(KP500000000, +0.500000000000000000000000000000000000000000000);
     DVK(KP866025403, +0.866025403784438646763723170752936183471402627);
     {
	  INT i;
	  const R *xi;
	  R *xo;
	  xi = ri;
	  xo = ro;
	  for (i = v; i > 0; i = i - VL, xi = xi + (VL * ivs), xo = xo + (VL * ovs), MAKE_VOLATILE_STRIDE(6, is), MAKE_VOLATILE_STRIDE(6, os)) {
	       V T1, T2, T3, T6, T4, T5;
	       T1 = LD(&(xi[0]), ivs, &(xi[0]));
	       T2 = LD(&(xi[WS(is, 1)]), ivs, &(xi[WS(is, 1)]));
	       T3 = LD(&(xi[WS(is, 2)]), ivs, &(xi[0]));
	       T6 = VMUL(LDK(KP866025403), VSUB(T3, T2));
	       T4 = VADD(T2, T3);
	       T5 = VFNMS(LDK(KP500000000), T4, T1);
	       ST(&(xo[0]), VADD(T1, T4), ovs, &(xo[0]));
	       ST(&(xo[WS(os, 1)]), VFMAI(T6, T5), ovs, &(xo[WS(os, 1)]));
	       ST(&(xo[WS(os, 2)]), VFNMSI(T6, T5), ovs, &(xo[0]));
	  }
     }
     VLEAVE();
}

static const kdft_desc desc = { 3, XSIMD_STRING("n1fv_3"), {3, 1, 3, 0}, &GENUS, 0, 0, 0, 0 };

void XSIMD(codelet_n1fv_3) (planner *p) {
     X(kdft_register) (p, n1fv_3, &desc);
}

#else				/* HAVE_FMA */

/* Generated by: ../../../genfft/gen_notw_c.native -simd -compact -variables 4 -pipeline-latency 8 -n 3 -name n1fv_3 -include n1f.h */

/*
 * This function contains 6 FP additions, 2 FP multiplications,
 * (or, 5 additions, 1 multiplications, 1 fused multiply/add),
 * 11 stack variables, 2 constants, and 6 memory accesses
 */
#include "n1f.h"

static void n1fv_3(const R *ri, const R *ii, R *ro, R *io, stride is, stride os, INT v, INT ivs, INT ovs)
{
     DVK(KP500000000, +0.500000000000000000000000000000000000000000000);
     DVK(KP866025403, +0.866025403784438646763723170752936183471402627);
     {
	  INT i;
	  const R *xi;
	  R *xo;
	  xi = ri;
	  xo = ro;
	  for (i = v; i > 0; i = i - VL, xi = xi + (VL * ivs), xo = xo + (VL * ovs), MAKE_VOLATILE_STRIDE(6, is), MAKE_VOLATILE_STRIDE(6, os)) {
	       V T1, T4, T6, T2, T3, T5;
	       T1 = LD(&(xi[0]), ivs, &(xi[0]));
	       T2 = LD(&(xi[WS(is, 1)]), ivs, &(xi[WS(is, 1)]));
	       T3 = LD(&(xi[WS(is, 2)]), ivs, &(xi[0]));
	       T4 = VADD(T2, T3);
	       T6 = VBYI(VMUL(LDK(KP866025403), VSUB(T3, T2)));
	       ST(&(xo[0]), VADD(T1, T4), ovs, &(xo[0]));
	       T5 = VFNMS(LDK(KP500000000), T4, T1);
	       ST(&(xo[WS(os, 2)]), VSUB(T5, T6), ovs, &(xo[0]));
	       ST(&(xo[WS(os, 1)]), VADD(T5, T6), ovs, &(xo[WS(os, 1)]));
	  }
     }
     VLEAVE();
}

static const kdft_desc desc = { 3, XSIMD_STRING("n1fv_3"), {5, 1, 1, 0}, &GENUS, 0, 0, 0, 0 };

void XSIMD(codelet_n1fv_3) (planner *p) {
     X(kdft_register) (p, n1fv_3, &desc);
}

#endif				/* HAVE_FMA */
