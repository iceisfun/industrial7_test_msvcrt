/* TC
 * id:    mth_cabs_basic
 * area:  math_basic
 * funcs:
 * procs: _cabs
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _cabs
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
typedef double (__cdecl *cabs_t)(struct _complex);
static cabs_t p_cabs;
#define _cabs p_cabs

static volatile double v_zero = 0.0, v_one = 1.0;

static double C(double r, double i)
{
    volatile double vr = r, vi = i;
    struct _complex c;
    c.x = vr; c.y = vi;
    return _cabs(c);
}
TC_CASE(t345) { CHECK_BITS_D(C(3, 4), 5.0); }
TC_CASE(neg) { CHECK_BITS_D(C(-5, -12), 13.0); }
TC_CASE(zero) { CHECK_BITS_D(C(0, 0), 0.0); }
TC_CASE(imag_only) { CHECK_BITS_D(C(0, -2), 2.0); }
int main(void) { tc_begin(); p_cabs = (cabs_t)tc_crt_proc("_cabs"); if (!p_cabs) { tc_write("TC FAIL " TC_TEST_ID "::_cabs not exported\n"); return 1; } TC_RUN(t345); TC_RUN(neg); TC_RUN(zero); TC_RUN(imag_only); return tc_end(); }
