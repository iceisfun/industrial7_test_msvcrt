/* TC
 * id:    pfi_int_prec_exceeds_width
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

TC_CASE(d) { char b[64]; int n; n = sprintf(b, "%3.6d", v42); CHECK_STR(b, "000042"); CHECK_EQ_I(n, 6); }
TC_CASE(left) { char b[64]; int n; n = sprintf(b, "%-3.6d", v42); CHECK_STR(b, "000042"); CHECK_EQ_I(n, 6); }
TC_CASE(x) { char b[64]; int n; n = sprintf(b, "%2.4x", (unsigned)255); CHECK_STR(b, "00ff"); CHECK_EQ_I(n, 4); }

int main(void) { tc_begin(); TC_RUN(d); TC_RUN(left); TC_RUN(x); return tc_end(); }
