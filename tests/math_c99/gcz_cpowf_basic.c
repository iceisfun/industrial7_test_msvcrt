/* TC
 * id:    gcz_cpowf_basic
 * area:  math_c99
 * funcs:
 * procs: cpowf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.3.8.2; MS docs cpow
 * note:  _Fcomplex args are 8-byte structs in integer registers on x64.
 */
#include "tc.h"
#include <math.h>
typedef struct { float x, y; } fc_t;
typedef fc_t (__cdecl *fn1_t)(fc_t);
typedef fc_t (__cdecl *fn2_t)(fc_t, fc_t);
static volatile float v_zero = 0.0f, v_one = 1.0f;
static fc_t mk(float x, float y) { volatile float a = x, b = y; fc_t z; z.x = a; z.y = b; return z; }
static float pinf(void) { return v_one / v_zero; }
static float qnan(void) { return pinf() - pinf(); }
#define PI 3.14159265358979323846
static fn2_t f;
TC_CASE(int_pow){ fc_t r=f(mk(2,0),mk(3,0)); CHECK_NEAR(r.x,8,1e-4); CHECK_NEAR(r.y,0,1e-4); }
TC_CASE(i_sq){ fc_t r=f(mk(0,1),mk(2,0)); CHECK_NEAR(r.x,-1,1e-5); CHECK_NEAR(r.y,0,1e-5); }
TC_CASE(sqrt9){ fc_t r=f(mk(9,0),mk(0.5f,0)); CHECK_NEAR(r.x,3,1e-5); CHECK_NEAR(r.y,0,1e-5); }
TC_CASE(pow0){ fc_t r=f(mk(2,3),mk(0,0)); CHECK_NEAR(r.x,1,1e-5); CHECK_NEAR(r.y,0,1e-5); }
TC_CASE(pow1){ fc_t r=f(mk(-1,2),mk(1,0)); CHECK_NEAR(r.x,-1,1e-5); CHECK_NEAR(r.y,2,1e-5); }
int main(void){ tc_begin(); f=(fn2_t)tc_crt_proc("cpowf"); if(!f){ tc_write("TC FAIL " TC_TEST_ID "::__cpowf missing\n"); return 1; }
 TC_RUN(int_pow);TC_RUN(i_sq);TC_RUN(sqrt9);TC_RUN(pow0);TC_RUN(pow1); return tc_end(); }
