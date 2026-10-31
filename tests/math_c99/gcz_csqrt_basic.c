/* TC
 * id:    gcz_csqrt_basic
 * area:  math_c99
 * funcs:
 * procs: csqrt
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 G.6.4.2; MS docs csqrt
 * note:  _Dcomplex is 16 bytes: by pointer / hidden return on x64; typedef hand-written, resolved at run time.
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
TC_CASE(exact){ dc_t r; r=f(mk(4,0)); CHECK_BITS_D(r.x,2.0); CHECK_BITS_D(r.y,0.0); r=f(mk(-4,0)); CHECK_NEAR(r.x,0,1e-14); CHECK_NEAR(r.y,2,1e-14); }
TC_CASE(three_four){ dc_t r=f(mk(3,4)); CHECK_NEAR(r.x,2,1e-14); CHECK_NEAR(r.y,1,1e-14); }
TC_CASE(zero){ dc_t r=f(mk(0,0)); CHECK_BITS_D(r.x,0.0); CHECK_BITS_D(r.y,0.0); }
TC_CASE(minus2i){ dc_t r=f(mk(0,-2)); CHECK_NEAR(r.x,1,1e-14); CHECK_NEAR(r.y,-1,1e-14); }
TC_CASE(square_back){ dc_t z=mk(-7.5,3.25), r=f(z); double re=r.x*r.x-r.y*r.y, im=2*r.x*r.y; CHECK_NEAR(re,-7.5,1e-12); CHECK_NEAR(im,3.25,1e-12); CHECK(r.x>=0); }
int main(void){ tc_begin(); f=(fn1_t)tc_crt_proc("csqrt"); if(!f){ tc_write("TC FAIL " TC_TEST_ID "::__csqrt missing\n"); return 1; }
 TC_RUN(exact);TC_RUN(three_four);TC_RUN(zero);TC_RUN(minus2i);TC_RUN(square_back); return tc_end(); }
