/* TC
 * id:    pfi_int_prec_overrides_zero_flag
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p6; MS docs printf format specification
 * note:  Restricted to ucrtbase: whether Win7-era CRTs ignore the 0 flag when a precision is given is only
 *        probed (pfi_probe_prec_zero_flag_old), not asserted here.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile int v42 = 42, vm42 = -42;
static volatile unsigned vu = 42;

TC_CASE(d) { char b[64]; int n; n = sprintf(b, "%08.3d", (int)7); CHECK_STR(b, "     007"); CHECK_EQ_I(n, 8); }
TC_CASE(x) { char b[64]; int n; n = sprintf(b, "%08.3x", (unsigned)7); CHECK_STR(b, "     007"); CHECK_EQ_I(n, 8); }
TC_CASE(u) { char b[64]; int n; n = sprintf(b, "%08.3u", (unsigned)7); CHECK_STR(b, "     007"); CHECK_EQ_I(n, 8); }
TC_CASE(minus) { char b[64]; int n; n = sprintf(b, "%-08.3d", (int)7); CHECK_STR(b, "007     "); CHECK_EQ_I(n, 8); }
TC_CASE(zero_empty) { char b[64]; int n; n = sprintf(b, "%05.d", (int)0); CHECK_STR(b, "     "); CHECK_EQ_I(n, 5); }

int main(void) { tc_begin(); TC_RUN(d); TC_RUN(x); TC_RUN(u); TC_RUN(minus); TC_RUN(zero_empty); return tc_end(); }
