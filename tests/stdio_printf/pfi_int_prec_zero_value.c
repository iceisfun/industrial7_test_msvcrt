/* TC
 * id:    pfi_int_prec_zero_value
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

TC_CASE(d0) { char b[64]; int n; n = sprintf(b, "%.0d", (int)0); CHECK_STR(b, ""); CHECK_EQ_I(n, 0); }
TC_CASE(d1) { char b[64]; int n; n = sprintf(b, "%.0d", (int)1); CHECK_STR(b, "1"); CHECK_EQ_I(n, 1); }
TC_CASE(u0) { char b[64]; int n; n = sprintf(b, "%.0u", (unsigned)0); CHECK_STR(b, ""); CHECK_EQ_I(n, 0); }
TC_CASE(x0) { char b[64]; int n; n = sprintf(b, "%.0x", (unsigned)0); CHECK_STR(b, ""); CHECK_EQ_I(n, 0); }
TC_CASE(o0) { char b[64]; int n; n = sprintf(b, "%.0o", (unsigned)0); CHECK_STR(b, ""); CHECK_EQ_I(n, 0); }
TC_CASE(bare_dot) { char b[64]; int n; n = sprintf(b, "%.d", (int)0); CHECK_STR(b, ""); CHECK_EQ_I(n, 0); }
TC_CASE(width) { char b[64]; int n; n = sprintf(b, "%5.0d", (int)0); CHECK_STR(b, "     "); CHECK_EQ_I(n, 5); }

int main(void) { tc_begin(); TC_RUN(d0); TC_RUN(d1); TC_RUN(u0); TC_RUN(x0); TC_RUN(o0); TC_RUN(bare_dot); TC_RUN(width); return tc_end(); }
