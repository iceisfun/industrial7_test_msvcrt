/* TC
 * id:    gcz_cpow_basic
 * area:  math_c99
 * funcs:
 * procs: cpow
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.3.8.2; MS docs cpow
 * note:  
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
TC_CASE(int_pow){ dc_t r=f(mk(2,0),mk(3,0)); CHECK_NEAR(r.x,8,1e-12); CHECK_NEAR(r.y,0,1e-12); }
TC_CASE(i_sq){ dc_t r=f(mk(0,1),mk(2,0)); CHECK_NEAR(r.x,-1,1e-12); CHECK_NEAR(r.y,0,1e-12); }
TC_CASE(sqrt4){ dc_t r=f(mk(4,0),mk(0.5,0)); CHECK_NEAR(r.x,2,1e-12); CHECK_NEAR(r.y,0,1e-12); }
TC_CASE(onei_sq){ dc_t r=f(mk(1,1),mk(2,0)); CHECK_NEAR(r.x,0,1e-12); CHECK_NEAR(r.y,2,1e-12); }
TC_CASE(e_ipi){ dc_t r=f(mk(2.71828182845904523536,0),mk(0,PI)); CHECK_NEAR(r.x,-1,1e-12); CHECK_NEAR(r.y,0,1e-12); }
TC_CASE(pow1){ dc_t r=f(mk(3,-2),mk(1,0)); CHECK_NEAR(r.x,3,1e-12); CHECK_NEAR(r.y,-2,1e-12); }
TC_CASE(pow0){ dc_t r=f(mk(5,7),mk(0,0)); CHECK_NEAR(r.x,1,1e-12); CHECK_NEAR(r.y,0,1e-12); }
int main(void){ tc_begin(); f=(fn2_t)tc_crt_proc("cpow"); if(!f){ tc_write("TC FAIL " TC_TEST_ID "::__cpow missing\n"); return 1; }
 TC_RUN(int_pow);TC_RUN(i_sq);TC_RUN(sqrt4);TC_RUN(onei_sq);TC_RUN(e_ipi);TC_RUN(pow1);TC_RUN(pow0); return tc_end(); }
