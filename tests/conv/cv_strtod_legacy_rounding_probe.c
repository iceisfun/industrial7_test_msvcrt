/* TC
 * id:    cv_strtod_legacy_rounding_probe
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.22.1.3, IEC 60559 correct rounding
 * note:  Probes correct rounding of hard cases on every CRT: 1e23 (decimal value lies very close to a halfway point) and
 *        2.2250738585072011e-308 (just below the denormal/normal midpoint). Pre-UCRT CRTs go through an 80-bit
 *        intermediate and may double-round; UCRT is expected exact.
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static double D(const char *volatile s) { return strtod(s, NULL); }
static double FB(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }

TC_CASE(one_e23) { CHECK_BITS_D(D("1e23"), FB(0x44B52D02C7E14AF6ULL)); }
TC_CASE(largest_denormal_boundary) { CHECK_BITS_D(D("2.2250738585072011e-308"), FB(0x000FFFFFFFFFFFFFULL)); }

int main(void)
{
    tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");
    TC_RUN(one_e23); TC_RUN(largest_denormal_boundary);
    return tc_end();
}
