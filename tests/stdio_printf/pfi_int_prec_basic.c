/* TC
 * id:    pfi_int_prec_basic
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1; MS docs printf format specification
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile int v42 = 42, vm42 = -42;
static volatile unsigned vu = 42;

TC_CASE(d_pos) { char b[64]; int n; n = sprintf(b, "%.5d", v42); CHECK_STR(b, "00042"); CHECK_EQ_I(n, 5); }
TC_CASE(d_neg) { char b[64]; int n; n = sprintf(b, "%.5d", vm42); CHECK_STR(b, "-00042"); CHECK_EQ_I(n, 6); }
TC_CASE(u) { char b[64]; int n; n = sprintf(b, "%.3u", (unsigned)7); CHECK_STR(b, "007"); CHECK_EQ_I(n, 3); }
TC_CASE(x) { char b[64]; int n; n = sprintf(b, "%.8x", (unsigned)0xbeef); CHECK_STR(b, "0000beef"); CHECK_EQ_I(n, 8); }
TC_CASE(o) { char b[64]; int n; n = sprintf(b, "%.4o", (unsigned)8); CHECK_STR(b, "0010"); CHECK_EQ_I(n, 4); }
TC_CASE(shorter) { char b[64]; int n; n = sprintf(b, "%.2d", (int)12345); CHECK_STR(b, "12345"); CHECK_EQ_I(n, 5); }

int main(void) { tc_begin(); TC_RUN(d_pos); TC_RUN(d_neg); TC_RUN(u); TC_RUN(x); TC_RUN(o); TC_RUN(shorter); return tc_end(); }
