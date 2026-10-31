/* TC
 * id:    m99_neg_gamma_huge_negative_probe
 * area:  math_c99
 * funcs: tgamma
 * procs: lgamma
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.5.3, F.10.5.4 (negative integers: lgamma pole +inf, tgamma NaN/invalid)
 * note:  PROBE: every double with magnitude >= 2^52 is an integer, so -1e300 is a negative integer: the standard gives tgamma = NaN and lgamma = +inf. Real implementations reduce via sin(pi*x) and may mishandle huge arguments, hence a probe. errno asserted in {0,ERANGE,EDOM}.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

typedef double (__cdecl *pd1)(double);
static pd1 p_lg;
static volatile double v_zero = 0.0, v_one = 1.0;
#define DINF (v_one / v_zero)
#define ERR_OK() (errno == 0 || errno == ERANGE || errno == EDOM)

TC_CASE(tgamma_huge_negative)
{
    volatile double h = -1e300; double r;
    errno = 0; r = tgamma(h); CHECK(r != r); CHECK(ERR_OK());
}
TC_CASE(lgamma_huge_negative)
{
    volatile double h = -1e300;
    if (!p_lg) TC_SKIP("no lgamma");
    errno = 0; CHECK_BITS_D(p_lg(h), DINF); CHECK(ERR_OK());
}
int main(void)
{
    tc_begin();
    p_lg = (pd1)tc_crt_proc("lgamma");
    TC_RUN(tgamma_huge_negative); TC_RUN(lgamma_huge_negative);
    return tc_end();
}
