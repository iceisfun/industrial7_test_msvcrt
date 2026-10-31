/* TC
 * id:    gcz_clogf_basic
 * area:  math_c99
 * funcs:
 * procs: clogf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 G.6.3.2; MS docs clog
 * note:  
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
static fn1_t f;
TC_CASE(one){ fc_t r=f(mk(1,0)); CHECK_BITS_F(r.x,0.0f); CHECK_BITS_F(r.y,0.0f); }
TC_CASE(e){ fc_t r=f(mk((float)2.71828182845904523536,0)); CHECK_NEAR(r.x,1,1e-6); CHECK_NEAR(r.y,0,1e-6); }
TC_CASE(i){ fc_t r=f(mk(0,1)); CHECK_NEAR(r.x,0,1e-6); CHECK_NEAR(r.y,PI/2,1e-6); }
TC_CASE(minus1){ fc_t r=f(mk(-1,0)); CHECK_NEAR(r.x,0,1e-6); CHECK_NEAR(r.y,PI,1e-6); }
TC_CASE(one_plus_i){ fc_t r=f(mk(1,1)); CHECK_NEAR(r.x,0.34657359027997265,1e-6); CHECK_NEAR(r.y,PI/4,1e-6); }
TC_CASE(neg_imag){ fc_t r=f(mk(0.5f,-0.5f)); CHECK_NEAR(r.y,-PI/4,1e-6); CHECK_NEAR(r.x,-0.34657359027997265,1e-6); }
int main(void){ tc_begin(); f=(fn1_t)tc_crt_proc("clogf"); if(!f){ tc_write("TC FAIL " TC_TEST_ID "::__clogf missing\n"); return 1; }
 TC_RUN(one);TC_RUN(e);TC_RUN(i);TC_RUN(minus1);TC_RUN(one_plus_i);TC_RUN(neg_imag); return tc_end(); }
