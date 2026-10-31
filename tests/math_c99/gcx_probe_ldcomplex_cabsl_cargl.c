/* TC
 * id:    gcx_probe_ldcomplex_cabsl_cargl
 * area:  math_c99
 * funcs:
 * procs: cabsl cargl
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs norm, cabsl, cargl; C11 G.6
 * note:  PROBE: _Ldcomplex assumed to have the layout of two doubles on the MS ABI (sizeof(long double)==8),
 *        so cabsl/cargl take the same argument as cabs/carg.
 */
#include "tc.h"
#include <math.h>

typedef struct { double re, im; } Dc;
typedef struct { float re, im; } Fc;
static Dc D(double r, double i) { Dc z; z.re = r; z.im = i; return z; }
static Fc F(float r, float i) { Fc z; z.re = r; z.im = i; return z; }
static volatile double v_zero = 0.0, v_one = 1.0;
static double INF_(void) { return v_one / v_zero; }
static double NAN_(void) { return INF_() - INF_(); }
typedef double (__cdecl *pt_ld)(Dc); static pt_ld p_cabsl, p_cargl;

TC_CASE(cabsl_3_4)
{
    if (!p_cabsl) TC_SKIP("cabsl not exported");
    CHECK_NEAR(p_cabsl(D(3.0, 4.0)), 5.0, 1e-12);
}
TC_CASE(cargl_i)
{
    if (!p_cargl) TC_SKIP("cargl not exported");
    CHECK_NEAR(p_cargl(D(0.0, 1.0)), 1.5707963267948966, 1e-12);
}

int main(void)
{
    tc_begin();
    p_cabsl = (pt_ld)tc_crt_proc("cabsl");
    p_cargl = (pt_ld)tc_crt_proc("cargl");
    TC_RUN(cabsl_3_4); TC_RUN(cargl_i);
    return tc_end();
}
