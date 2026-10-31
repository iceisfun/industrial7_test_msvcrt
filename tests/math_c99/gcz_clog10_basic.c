/* TC
 * id:    gcz_clog10_basic
 * area:  math_c99
 * funcs:
 * procs: clog10
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs clog10 (UCRT)
 * note:  clog10 is a Microsoft extension: real = log10|z|, imag = arg(z)/ln(10).
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
TC_CASE(hundred){ dc_t r=f(mk(100,0)); CHECK_NEAR(r.x,2,1e-13); CHECK_NEAR(r.y,0,1e-13); }
TC_CASE(one){ dc_t r=f(mk(1,0)); CHECK_NEAR(r.x,0,1e-14); CHECK_NEAR(r.y,0,1e-14); }
TC_CASE(ten){ dc_t r=f(mk(10,0)); CHECK_NEAR(r.x,1,1e-13); }
TC_CASE(ten_i){ dc_t r=f(mk(0,10)); CHECK_NEAR(r.x,1,1e-13); CHECK_NEAR(r.y,PI/(2*2.30258509299404568402),1e-13); }
TC_CASE(minus1){ dc_t r=f(mk(-1,0)); CHECK_NEAR(r.x,0,1e-13); CHECK_NEAR(r.y,1.3643763538418412,1e-12); }
TC_CASE(tenth){ dc_t r=f(mk(0.1,0)); CHECK_NEAR(r.x,-1,1e-13); }
int main(void){ tc_begin(); f=(fn1_t)tc_crt_proc("clog10"); if(!f){ tc_write("TC FAIL " TC_TEST_ID "::__clog10 missing\n"); return 1; }
 TC_RUN(hundred);TC_RUN(one);TC_RUN(ten);TC_RUN(ten_i);TC_RUN(minus1);TC_RUN(tenth); return tc_end(); }
