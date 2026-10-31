/* TC
 * id:    pff_inf_nan_legacy_e_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win7
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   pre-UCRT CRT printf output (1.#INF etc.)
 * note:  probed: exact tail padding of legacy inf/nan under %e, precision and width
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static double dfb(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }
static volatile double v_zero = 0.0, v_one = 1.0;
static char b[512];
static double mkinf(void) { return v_one / v_zero; }

static int starts(const char *s, const char *p) { while (*p) { if (*s++ != *p++) return 0; } return 1; }
TC_CASE(e_inf) { volatile double p = mkinf(); sprintf(b, "%e", p); CHECK_STR(b, "1.#INF00e+000"); }
TC_CASE(e_qnan) { volatile double q = dfb(0x7FF8000000000000ULL); sprintf(b, "%e", q); CHECK_STR(b, "1.#QNAN0e+000"); }
TC_CASE(prec) { volatile double p = mkinf(); sprintf(b, "%.2f", p); CHECK(starts(b, "1.#")); }
TC_CASE(width) { volatile double p = mkinf(); sprintf(b, "[%12f]", p); CHECK_STR(b, "[    1.#INF00]"); }
TC_CASE(upper) { volatile double p = mkinf(); sprintf(b, "%E", p); CHECK(starts(b, "1.#INF")); }
int main(void) { tc_begin(); TC_RUN(e_inf); TC_RUN(e_qnan); TC_RUN(prec); TC_RUN(width); TC_RUN(upper); return tc_end(); }
