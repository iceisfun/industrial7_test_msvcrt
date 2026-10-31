/* TC
 * id:    pff_inf_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS UCRT breaking changes (printf inf/nan C99 style); C11 7.21.6.1
 * note:  MS UCRT breaking-changes: printf inf/nan now C99 style rather than 1.#INF; inf built as 1.0/volatile zero
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static double dfb(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }
static volatile double v_zero = 0.0, v_one = 1.0;
static char b[512];
static double mkinf(void) { return v_one / v_zero; }

TC_CASE(f_plain) { volatile double p = mkinf(), n = -mkinf(); sprintf(b, "%f|%f", p, n); CHECK_STR(b, "inf|-inf"); }
TC_CASE(e_g) { volatile double p = mkinf(); sprintf(b, "%e|%g|%E", p, p, p); CHECK_STR(b, "inf|inf|INF"); }
TC_CASE(upper) { volatile double p = mkinf(), n = -mkinf(); sprintf(b, "%F|%E|%G|%F", p, p, p, n); CHECK_STR(b, "INF|INF|INF|-INF"); }
TC_CASE(sign_flags) { volatile double p = mkinf(); sprintf(b, "%+f|% f", p, p); CHECK_STR(b, "+inf| inf"); }
TC_CASE(precision_ignored) { volatile double p = mkinf(); sprintf(b, "%.2f|%.0e|%.10g", p, p, p); CHECK_STR(b, "inf|inf|inf"); }
int main(void) { tc_begin(); TC_RUN(f_plain); TC_RUN(e_g); TC_RUN(upper); TC_RUN(sign_flags); TC_RUN(precision_ignored); return tc_end(); }
