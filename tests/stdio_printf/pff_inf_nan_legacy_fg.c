/* TC
 * id:    pff_inf_nan_legacy_fg
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win7
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   pre-UCRT CRT printf output (1.#INF etc.)
 * note:  well-known pre-UCRT output; 0.0/0.0 on x64 yields the default (indefinite) NaN with sign bit set
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static double dfb(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }
static volatile double v_zero = 0.0, v_one = 1.0;
static char b[512];
static double mkinf(void) { return v_one / v_zero; }

TC_CASE(f) { volatile double p = mkinf(), n = -mkinf(); sprintf(b, "%f|%f", p, n); CHECK_STR(b, "1.#INF00|-1.#INF00"); }
TC_CASE(f_nan) { volatile double q = dfb(0x7FF8000000000000ULL); sprintf(b, "%f", q); CHECK_STR(b, "1.#QNAN0"); }
TC_CASE(f_ind) { volatile double d = dfb(0xFFF8000000000000ULL); sprintf(b, "%f", d); CHECK_STR(b, "-1.#IND00"); }
TC_CASE(f_div) { volatile double d = v_zero / v_zero; sprintf(b, "%f", d); CHECK_STR(b, "-1.#IND00"); }
TC_CASE(g) { volatile double p = mkinf(), q = dfb(0x7FF8000000000000ULL), i = dfb(0xFFF8000000000000ULL);
    sprintf(b, "%g|%g|%g", p, q, i); CHECK_STR(b, "1.#INF|1.#QNAN|-1.#IND"); }
int main(void) { tc_begin(); TC_RUN(f); TC_RUN(f_nan); TC_RUN(f_ind); TC_RUN(f_div); TC_RUN(g); return tc_end(); }
