/* TC
 * id:    gcx_probe_norm_inf_nan
 * area:  math_c99
 * funcs:
 * procs: norm normf
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs norm (complex.h)
 * note:  PROBE: norm(inf,nan) assumed NaN (re*re+im*im => inf+nan=nan); real Windows may implement norm through
 *        a hypot-like path and return inf. Isolated from the documented norm tests.
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
typedef double (__cdecl *pt_norm)(Dc); static pt_norm p_norm;
typedef float (__cdecl *pt_normf)(Fc); static pt_normf p_normf;

TC_CASE(norm_inf_nan)
{
    double inf = INF_(), nan = NAN_();
    if (!p_norm) TC_SKIP("norm not exported");
    CHECK_IS_NAN(p_norm(D(inf, nan)));
    CHECK_IS_NAN(p_norm(D(nan, inf)));
}
TC_CASE(normf_inf_nan)
{
    float inf = (float)INF_(), nan = (float)NAN_();
    if (!p_normf) TC_SKIP("normf not exported");
    CHECK_IS_NAN(p_normf(F(inf, nan)));
}

int main(void)
{
    tc_begin();
    p_norm = (pt_norm)tc_crt_proc("norm");
    p_normf = (pt_normf)tc_crt_proc("normf");
    TC_RUN(norm_inf_nan); TC_RUN(normf_inf_nan);
    return tc_end();
}
