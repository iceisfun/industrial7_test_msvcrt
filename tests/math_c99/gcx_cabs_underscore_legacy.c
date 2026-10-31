/* TC
 * id:    gcx_cabs_underscore_legacy
 * area:  math_c99
 * funcs:
 * procs: _cabs
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _cabs (math.h, struct _complex of two doubles, passed by value)
 * note:  3-4-5 results use a tolerance: hypot-style scaling is not guaranteed exact by any document.
 */
#include "tc.h"

typedef struct { double re, im; } Dc;
typedef struct { float re, im; } Fc;
static Dc D(double r, double i) { Dc z; z.re = r; z.im = i; return z; }
static Fc F(float r, float i) { Fc z; z.re = r; z.im = i; return z; }
static volatile double v_zero = 0.0, v_one = 1.0;
static double INF_(void) { return v_one / v_zero; }
static double NAN_(void) { return INF_() - INF_(); }
typedef double (__cdecl *pt_cabs)(Dc); static pt_cabs p_cabs;

TC_CASE(exported) { CHECK(p_cabs != NULL); }
TC_CASE(pythagorean)
{
    if (!p_cabs) TC_SKIP("not exported");
    CHECK_NEAR(p_cabs(D(3.0, 4.0)), 5.0, 1e-13);
    CHECK_NEAR(p_cabs(D(5.0, 12.0)), 13.0, 1e-13);
}
TC_CASE(signs)
{
    if (!p_cabs) TC_SKIP("not exported");
    CHECK_NEAR(p_cabs(D(-3.0, -4.0)), 5.0, 1e-13);
    CHECK_NEAR(p_cabs(D(-3.0, 4.0)), 5.0, 1e-13);
}
TC_CASE(axes_zero)
{
    if (!p_cabs) TC_SKIP("not exported");
    CHECK_BITS_D(p_cabs(D(0.0, 0.0)), 0.0);
    CHECK_BITS_D(p_cabs(D(-5.0, 0.0)), 5.0);
    CHECK_BITS_D(p_cabs(D(0.0, 12.0)), 12.0);
}

int main(void)
{
    tc_begin();
    p_cabs = (pt_cabs)tc_crt_proc("_cabs");
    TC_RUN(exported); TC_RUN(pythagorean); TC_RUN(signs); TC_RUN(axes_zero);
    return tc_end();
}
