/* TC
 * id:    pfi_int_flag_zero_pad
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

TC_CASE(d_pos) { char b[64]; int n; n = sprintf(b, "%05d", v42); CHECK_STR(b, "00042"); CHECK_EQ_I(n, 5); }
TC_CASE(d_neg) { char b[64]; int n; n = sprintf(b, "%05d", vm42); CHECK_STR(b, "-0042"); CHECK_EQ_I(n, 5); }
TC_CASE(u) { char b[64]; int n; n = sprintf(b, "%05u", vu); CHECK_STR(b, "00042"); CHECK_EQ_I(n, 5); }
TC_CASE(x) { char b[64]; int n; n = sprintf(b, "%08x", (unsigned)0xbeef); CHECK_STR(b, "0000beef"); CHECK_EQ_I(n, 8); }
TC_CASE(o) { char b[64]; int n; n = sprintf(b, "%05o", (unsigned)8); CHECK_STR(b, "00010"); CHECK_EQ_I(n, 5); }
TC_CASE(plus) { char b[64]; int n; n = sprintf(b, "%0+5d", v42); CHECK_STR(b, "+0042"); CHECK_EQ_I(n, 5); }
TC_CASE(longer) { char b[64]; int n; n = sprintf(b, "%03d", (int)12345); CHECK_STR(b, "12345"); CHECK_EQ_I(n, 5); }

int main(void) { tc_begin(); TC_RUN(d_pos); TC_RUN(d_neg); TC_RUN(u); TC_RUN(x); TC_RUN(o); TC_RUN(plus); TC_RUN(longer); return tc_end(); }
