/* TC
 * id:    mth_cabs_large_probe
 * area:  math_basic
 * funcs:
 * procs: _cabs
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _cabs
 * note:  PROBE: _cabs({1e200,1e200}) avoids intermediate overflow (like _hypot) and returns ~1.4142e200.
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
TC_CASE(big)
{
    double r = C(1e200, 1e200);
    double q = r / 1e200;
    CHECK(r == r && r < 1e301);
    CHECK(q > 1.4142135623730 && q < 1.4142135623732);
}

int main(void) { tc_begin(); p_cabs = (cabs_t)tc_crt_proc("_cabs"); if (!p_cabs) { tc_write("TC FAIL " TC_TEST_ID "::_cabs not exported\n"); return 1; } TC_RUN(big); return tc_end(); }
