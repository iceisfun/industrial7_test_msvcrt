/* TC
 * id:    gcz_neg_clog10_pole_probe
 * area:  math_c99
 * funcs:
 * procs: clog10
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs clog10 (silent on special values)
 * note:  PROBE: whether clog10 mirrors clog's special-value table (C11 G.6.3.2) at zero, inf and nan.
 */
#include "tc.h"
#include <math.h>
typedef struct { double x, y; } dc_t;
typedef dc_t (__cdecl *fn1_t)(dc_t);
typedef dc_t (__cdecl *fn2_t)(dc_t, dc_t);
static volatile double v_zero = 0.0, v_one = 1.0;
static dc_t mk(double x, double y) { volatile double a = x, b = y; dc_t z; z.x = a; z.y = b; return z; }
static double pinf(void) { return v_one / v_zero; }
static double qnan(void) { return pinf() - pinf(); }
#define PI 3.14159265358979323846
static fn1_t f;
TC_CASE(zero){ dc_t r=f(mk(0.0,0.0)); CHECK(r.x==-pinf()); CHECK_BITS_D(r.y,0.0); }
TC_CASE(negzero){ dc_t r=f(mk(-0.0,0.0)); CHECK(r.x==-pinf()); CHECK_NEAR(r.y,1.3643763538418412,1e-12); }
TC_CASE(inf){ dc_t r=f(mk(pinf(),0)); CHECK(r.x==pinf()); }
TC_CASE(nan_nan){ dc_t r=f(mk(qnan(),qnan())); CHECK_IS_NAN(r.x); CHECK_IS_NAN(r.y); }
int main(void){ tc_begin(); f=(fn1_t)tc_crt_proc("clog10"); if(!f){ tc_write("TC FAIL " TC_TEST_ID "::__clog10 missing\n"); return 1; }
 TC_RUN(zero);TC_RUN(negzero);TC_RUN(inf);TC_RUN(nan_nan); return tc_end(); }
