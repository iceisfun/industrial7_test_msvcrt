/* TC
 * id:    pfi_int_flag_plus_ignored_unsigned
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

TC_CASE(plus_u) { char b[64]; int n; n = sprintf(b, "%+u", vu); CHECK_STR(b, "42"); CHECK_EQ_I(n, 2); }
TC_CASE(plus_x) { char b[64]; int n; n = sprintf(b, "%+x", (unsigned)255); CHECK_STR(b, "ff"); CHECK_EQ_I(n, 2); }
TC_CASE(plus_o) { char b[64]; int n; n = sprintf(b, "%+o", (unsigned)8); CHECK_STR(b, "10"); CHECK_EQ_I(n, 2); }
TC_CASE(space_u) { char b[64]; int n; n = sprintf(b, "% u", (unsigned)5); CHECK_STR(b, "5"); CHECK_EQ_I(n, 1); }
TC_CASE(space_x) { char b[64]; int n; n = sprintf(b, "% x", (unsigned)255); CHECK_STR(b, "ff"); CHECK_EQ_I(n, 2); }

int main(void) { tc_begin(); TC_RUN(plus_u); TC_RUN(plus_x); TC_RUN(plus_o); TC_RUN(space_u); TC_RUN(space_x); return tc_end(); }
