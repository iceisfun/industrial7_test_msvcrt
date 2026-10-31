/* TC
 * id:    pff_nan_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS UCRT breaking changes (printf inf/nan C99 style)
 * note:  MS UCRT behaviour: nan, nan(ind), nan(snan); signaling case built by bits through a union (no FP ops)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static double dfb(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }
static volatile double v_zero = 0.0, v_one = 1.0;
static char b[512];

static volatile double v_ind;
TC_CASE(quiet) { volatile double d = dfb(0x7FF8000000000000ULL); sprintf(b, "%f", d); CHECK_STR(b, "nan"); }
TC_CASE(indefinite) { volatile double d = dfb(0xFFF8000000000000ULL); sprintf(b, "%f", d); CHECK_STR(b, "-nan(ind)"); }
TC_CASE(div_result) { v_ind = v_zero / v_zero; sprintf(b, "%f", v_ind); CHECK_STR(b, "-nan(ind)"); }
TC_CASE(signaling) { volatile double d = dfb(0x7FF4000000000000ULL); sprintf(b, "%f", d); CHECK_STR(b, "nan(snan)"); }
int main(void) { tc_begin(); TC_RUN(quiet); TC_RUN(indefinite); TC_RUN(div_result); TC_RUN(signaling); return tc_end(); }
