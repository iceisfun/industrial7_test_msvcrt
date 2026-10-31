/* TC
 * id:    gcz_neg_cpow_zero_base_probe
 * area:  math_c99
 * funcs:
 * procs: cpow
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 G.7 (cpow special values unspecified)
 * note:  PROBE: cpow with zero base; most-likely values, neither C nor MS docs specify them.
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
static fn2_t f;
TC_CASE(zero_sq){ dc_t r=f(mk(0,0),mk(2,0)); CHECK_BITS_D(r.x,0.0); CHECK_BITS_D(r.y,0.0); }
TC_CASE(zero_zero){ dc_t r=f(mk(0,0),mk(0,0)); CHECK_BITS_D(r.x,1.0); CHECK_BITS_D(r.y,0.0); }
TC_CASE(zero_neg){ dc_t r=f(mk(0,0),mk(-1,0)); CHECK(!(r.x==r.x) || r.x==pinf()); }
int main(void){ tc_begin(); f=(fn2_t)tc_crt_proc("cpow"); if(!f){ tc_write("TC FAIL " TC_TEST_ID "::__cpow missing\n"); return 1; }
 TC_RUN(zero_sq);TC_RUN(zero_zero);TC_RUN(zero_neg); return tc_end(); }
