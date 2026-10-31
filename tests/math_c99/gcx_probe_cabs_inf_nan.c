/* TC
 * id:    gcx_probe_cabs_inf_nan
 * area:  math_c99
 * funcs:
 * procs: cabs cabsf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  probe
 * ref:   C11 G.6, F.10.4.3 (hypot(+-inf,NaN)=+inf)
 * note:  PROBE: cabs/cabsf(+-inf, NaN) return +inf per C99 G.6/F.10.4.3; real msvcr120 may differ (x87-era hypot).
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>

typedef struct { double re, im; } Dc;
typedef struct { float re, im; } Fc;
static Dc D(double r, double i) { Dc z; z.re = r; z.im = i; return z; }
static Fc F(float r, float i) { Fc z; z.re = r; z.im = i; return z; }
static volatile double v_zero = 0.0, v_one = 1.0;
static double INF_(void) { return v_one / v_zero; }
static double NAN_(void) { return INF_() - INF_(); }
typedef double (__cdecl *pt_cabs)(Dc); static pt_cabs p_cabs;
typedef float (__cdecl *pt_cabsf)(Fc); static pt_cabsf p_cabsf;

TC_CASE(exported) { CHECK(p_cabs && p_cabsf); }
TC_CASE(inf_nan)
{
    if (!(p_cabs && p_cabsf)) TC_SKIP("not exported");
    double inf = INF_(), nan = NAN_();
    CHECK_BITS_D(p_cabs(D(inf,nan)), inf);
    CHECK_BITS_D(p_cabs(D(nan,-inf)), inf);
    CHECK_BITS_F(p_cabsf(F((float)inf,(float)nan)), (float)inf);
}

int main(void)
{
    tc_begin();
    p_cabs = (pt_cabs)tc_crt_proc("cabs");
    p_cabsf = (pt_cabsf)tc_crt_proc("cabsf");
    TC_RUN(exported); TC_RUN(inf_nan);
    return tc_end();
}
