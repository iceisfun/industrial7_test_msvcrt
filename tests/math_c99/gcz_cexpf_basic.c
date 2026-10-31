/* TC
 * id:    gcz_cexpf_basic
 * area:  math_c99
 * funcs:
 * procs: cexpf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 G.6.3.1; MS docs cexp
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
TC_CASE(zero){ fc_t r=f(mk(0,0)); CHECK_BITS_F(r.x,1.0f); CHECK_BITS_F(r.y,0.0f); r=f(mk(-0.0f,0)); CHECK_BITS_F(r.x,1.0f); CHECK_BITS_F(r.y,0.0f); }
TC_CASE(e){ fc_t r=f(mk(1,0)); CHECK_NEAR(r.x,2.71828182845904523536,1e-5); CHECK_NEAR(r.y,0,1e-6); }
TC_CASE(i_halfpi){ fc_t r=f(mk(0,(float)(PI/2))); CHECK_NEAR(r.x,0,1e-6); CHECK_NEAR(r.y,1,1e-6); }
TC_CASE(i_pi){ fc_t r=f(mk(0,(float)PI)); CHECK_NEAR(r.x,-1,1e-6); CHECK_NEAR(r.y,0,1e-6); }
TC_CASE(ln2){ fc_t r=f(mk((float)0.69314718055994530942,0)); CHECK_NEAR(r.x,2,1e-5); CHECK_NEAR(r.y,0,1e-6); }
int main(void){ tc_begin(); f=(fn1_t)tc_crt_proc("cexpf"); if(!f){ tc_write("TC FAIL " TC_TEST_ID "::__cexpf missing\n"); return 1; }
 TC_RUN(zero);TC_RUN(e);TC_RUN(i_halfpi);TC_RUN(i_pi);TC_RUN(ln2); return tc_end(); }
