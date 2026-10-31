/* TC
 * id:    pfi_int_flag_zero_minus_combo
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

TC_CASE(minus_zero) { char b[64]; int n; n = sprintf(b, "%-05d", v42); CHECK_STR(b, "42   "); CHECK_EQ_I(n, 5); }
TC_CASE(zero_minus) { char b[64]; int n; n = sprintf(b, "%0-5d", v42); CHECK_STR(b, "42   "); CHECK_EQ_I(n, 5); }
TC_CASE(hex) { char b[64]; int n; n = sprintf(b, "%-08x", (unsigned)255); CHECK_STR(b, "ff      "); CHECK_EQ_I(n, 8); }

int main(void) { tc_begin(); TC_RUN(minus_zero); TC_RUN(zero_minus); TC_RUN(hex); return tc_end(); }
