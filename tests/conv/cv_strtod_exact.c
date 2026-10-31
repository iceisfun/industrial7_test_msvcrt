/* TC
 * id:    cv_strtod_exact
 * area:  conv
 * funcs: 
 * procs: strtod
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.3, IEC 60559 correct rounding (round-to-nearest-even); MS docs strtod
 * note:  1e23 and the near-midpoint denormal moved to cv_strtod_legacy_rounding_probe (old CRTs convert via an 80-bit intermediate, so double rounding is possible). Values chosen so that even old CRTs are expected to round correctly. Very long mantissas are in cv_strtod_longdigits_probe.
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static double D(const char *volatile s) { return strtod(s, NULL); }
static double FB(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }

TC_CASE(tenth) { CHECK_BITS_D(D("0.1"), FB(0x3FB999999999999AULL)); }
TC_CASE(three_tenths) { CHECK_BITS_D(D("0.3"), FB(0x3FD3333333333333ULL)); }
TC_CASE(tie_down) { CHECK_BITS_D(D("9007199254740993"), FB(0x4340000000000000ULL)); /* 2^53, tie to even */ }
TC_CASE(tie_up) { CHECK_BITS_D(D("9007199254740995"), FB(0x4340000000000002ULL)); /* 2^53+4, tie to even */ }
TC_CASE(min_normal) { CHECK_BITS_D(D("2.2250738585072014e-308"), FB(0x0010000000000000ULL)); }
TC_CASE(dbl_max) { CHECK_BITS_D(D("1.7976931348623157e308"), FB(0x7FEFFFFFFFFFFFFFULL)); }

int main(void)
{
    tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");
    TC_RUN(tenth); TC_RUN(three_tenths); TC_RUN(tie_down); TC_RUN(tie_up);
    TC_RUN(min_normal); TC_RUN(dbl_max);
    return tc_end();
}
