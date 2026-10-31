/* TC
 * id:    pff_g_exact_digits_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs printf precision; UCRT (since VS2015) prints exact decimal expansion for any precision
 * note:  UCRT printf is exact beyond 17 digits; legacy CRTs zero-fill (see pff_g_wide_precision_legacy_probe).
 */
#include "tc.h"
#include <stdio.h>

static volatile double a = 0.1, c = 3.0, d = 1e23, e = 9.31322574615478515625e-10;

TC_CASE(tenth20) { char b[64]; sprintf(b, "%.20g", a); CHECK_STR(b, "0.10000000000000000555"); }
TC_CASE(third25) { char b[64]; sprintf(b, "%.25g", 1.0 / c); CHECK_STR(b, "0.3333333333333333148296163"); }
TC_CASE(e23) { char b[64]; sprintf(b, "%.17g", d); CHECK_STR(b, "9.9999999999999992e+22"); }
TC_CASE(pow2_exact) { char b[64]; sprintf(b, "%.30g", e); CHECK_STR(b, "9.31322574615478515625e-10"); }

int main(void) { tc_begin(); TC_RUN(tenth20); TC_RUN(third25); TC_RUN(e23); TC_RUN(pow2_exact); return tc_end(); }
