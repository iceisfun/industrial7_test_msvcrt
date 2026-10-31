/* TC
 * id:    err_neg_probe_math_pow_zero_negexp
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: pow
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   C11 F.10.4.4 (pow(+-0, y<0 odd int) = +-inf, divide-by-zero); C11 7.12.1 (pole error: ERANGE)
 * note:  PROBE: results are standard (IEEE sign rule: pow(-0,-1) = -inf). The errno value is the probe: a pole error is
 *        ERANGE per C11, older MS CRTs may instead report EDOM or leave 0. Asserted ERANGE for all three.
 */
#include "tc.h"
#include <errno.h>

typedef double (__cdecl *d2_t)(double, double);
static volatile double v_a, v_b;
static d2_t p_pow;
static double call(double a, double b) { v_a = a; v_b = b; errno = 0; return p_pow(v_a, v_b); }
static double inf(void) { volatile double o = 1.0, z = 0.0; return o / z; }

TC_CASE(zero_m1) { double r = call(0.0, -1.0); CHECK_BITS_D(r, inf()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(zero_m3) { double r = call(0.0, -3.0); CHECK_BITS_D(r, inf()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(negzero_m1) { double r = call(-0.0, -1.0); CHECK_BITS_D(r, -inf()); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(zero_m2_even) { double r = call(0.0, -2.0); CHECK_BITS_D(r, inf()); CHECK_EQ_I(errno, ERANGE); }

int main(void)
{
    tc_begin();
    p_pow = (d2_t)tc_crt_proc("pow");
    if (!p_pow) { tc_write("TC FAIL " TC_TEST_ID "::__setup\n"); return 1; }
    TC_RUN(zero_m1); TC_RUN(zero_m3); TC_RUN(negzero_m1); TC_RUN(zero_m2_even);
    errno = 0;
    return tc_end();
}
