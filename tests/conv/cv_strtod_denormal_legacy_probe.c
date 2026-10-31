/* TC
 * id:    cv_strtod_denormal_legacy_probe
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   win7
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * note:  Probes denormal parsing on pre-UCRT CRTs (80-bit intermediate; denormal handling unverified). Same expectations as cv_strtod_denormal.
 * ref:   C11 7.22.1.3, IEC 60559 round-to-nearest; 2^-1075 = 2.4703282292062327208e-324 (half the smallest denormal)
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static double D(const char *volatile s) { return strtod(s, NULL); }
static double FB(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }

TC_CASE(min_denormal) { CHECK_BITS_D(D("4.9406564584124654e-324"), FB(1)); }
TC_CASE(just_below_half) { CHECK_BITS_D(D("2.4703282292062327e-324"), FB(0)); }
TC_CASE(just_above_half) { CHECK_BITS_D(D("2.4703282292062328e-324"), FB(1)); }
TC_CASE(max_denormal_decimal) { CHECK_BITS_D(D("2.2250738585072009e-308"), FB(0x000FFFFFFFFFFFFFULL)); }
TC_CASE(mid_denormal) { double d = D("1e-310"); CHECK(d > 0.0); CHECK(d < 2.2250738585072014e-308); CHECK(tc_dbits(d) < 0x0010000000000000ULL); }
TC_CASE(neg_min_denormal) { CHECK_BITS_D(D("-4.94e-324"), FB(0x8000000000000001ULL)); }

int main(void)
{
    tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod"); 
    TC_RUN(min_denormal); TC_RUN(just_below_half); TC_RUN(just_above_half);
    TC_RUN(max_denormal_decimal); TC_RUN(mid_denormal); TC_RUN(neg_min_denormal);
    return tc_end();
}
