/* TC
 * id:    pff_inf_nan_zero_flag_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.21.6.1 (0 flag with inf/nan undefined in C99, defined in C11 Annex F)
 * note:  probed: UCRT pads inf/nan with spaces even with the 0 flag
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static double dfb(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }
static volatile double v_zero = 0.0, v_one = 1.0;
static char b[512];
static double mkinf(void) { return v_one / v_zero; }

TC_CASE(inf) { volatile double p = mkinf(); sprintf(b, "[%08f]", p); CHECK_STR(b, "[     inf]"); }
TC_CASE(nan) { volatile double q = dfb(0x7FF8000000000000ULL); sprintf(b, "[%08f]", q); CHECK_STR(b, "[     nan]"); }
TC_CASE(plus_inf) { volatile double p = mkinf(); sprintf(b, "[%+08f]", p); CHECK_STR(b, "[    +inf]"); }
int main(void) { tc_begin(); TC_RUN(inf); TC_RUN(nan); TC_RUN(plus_inf); return tc_end(); }
