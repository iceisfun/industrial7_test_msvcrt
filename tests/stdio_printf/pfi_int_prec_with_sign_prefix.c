/* TC
 * id:    pfi_int_prec_with_sign_prefix
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

TC_CASE(plus) { char b[64]; int n; n = sprintf(b, "%+.5d", v42); CHECK_STR(b, "+00042"); CHECK_EQ_I(n, 6); }
TC_CASE(space) { char b[64]; int n; n = sprintf(b, "% .5d", v42); CHECK_STR(b, " 00042"); CHECK_EQ_I(n, 6); }
TC_CASE(alt_x) { char b[64]; int n; n = sprintf(b, "%#.5x", (unsigned)255); CHECK_STR(b, "0x000ff"); CHECK_EQ_I(n, 7); }
TC_CASE(alt_x_width) { char b[64]; int n; n = sprintf(b, "%#10.5x", (unsigned)255); CHECK_STR(b, "   0x000ff"); CHECK_EQ_I(n, 10); }
TC_CASE(neg_width) { char b[64]; int n; n = sprintf(b, "%10.5d", vm42); CHECK_STR(b, "    -00042"); CHECK_EQ_I(n, 10); }

int main(void) { tc_begin(); TC_RUN(plus); TC_RUN(space); TC_RUN(alt_x); TC_RUN(alt_x_width); TC_RUN(neg_width); return tc_end(); }
