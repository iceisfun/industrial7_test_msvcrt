/* TC
 * id:    pff_inf_nan_width_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   C11 7.21.6.1 field width (nan(ind) text per UCRT docs); MS docs printf width specification
 * note:  width applies to inf/nan with space padding; -nan(ind) is 9 chars so use width 12
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static double dfb(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }
static volatile double v_zero = 0.0, v_one = 1.0;
static char b[512];
static double mkinf(void) { return v_one / v_zero; }

TC_CASE(right) { volatile double p = mkinf(); sprintf(b, "[%10f]", p); CHECK_STR(b, "[       inf]"); }
TC_CASE(left) { volatile double p = mkinf(); sprintf(b, "[%-10f]", p); CHECK_STR(b, "[inf       ]"); }
TC_CASE(plus) { volatile double p = mkinf(); sprintf(b, "[%+10f]", p); CHECK_STR(b, "[      +inf]"); }
TC_CASE(neg) { volatile double n = -mkinf(); sprintf(b, "[%10f]", n); CHECK_STR(b, "[      -inf]"); }
TC_CASE(nan_e) { volatile double q = dfb(0x7FF8000000000000ULL); sprintf(b, "[%10e]", q); CHECK_STR(b, "[       nan]"); }
TC_CASE(ind_left) { volatile double d = dfb(0xFFF8000000000000ULL); sprintf(b, "[%-12g]", d); CHECK_STR(b, "[-nan(ind)   ]"); }
int main(void) { tc_begin(); TC_RUN(right); TC_RUN(left); TC_RUN(plus); TC_RUN(neg); TC_RUN(nan_e); TC_RUN(ind_left); return tc_end(); }
