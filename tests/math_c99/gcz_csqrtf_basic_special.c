/* TC
 * id:    gcz_csqrtf_basic_special
 * area:  math_c99
 * funcs:
 * procs: csqrtf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 G.6.4.2; MS docs csqrt, csqrtf
 * note:  _Fcomplex is 8 bytes, passed/returned in integer registers on x64.
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
TC_CASE(basic){ fc_t r=f(mk(-1,0)); CHECK_NEAR(r.x,0,1e-6); CHECK_NEAR(r.y,1,1e-6); r=f(mk(4,0)); CHECK_BITS_F(r.x,2.0f); CHECK_BITS_F(r.y,0.0f); }
TC_CASE(three_four){ fc_t r=f(mk(3,4)); CHECK_NEAR(r.x,2,1e-5); CHECK_NEAR(r.y,1,1e-5); }
TC_CASE(zero){ fc_t r=f(mk(0,0)); CHECK_BITS_F(r.x,0.0f); CHECK_BITS_F(r.y,0.0f); }
TC_CASE(negzero_imag){ fc_t r=f(mk(-4,-0.0f)); CHECK_BITS_F(r.x,0.0f); CHECK_BITS_F(r.y,-2.0f); }
TC_CASE(large){ fc_t r=f(mk(1e30f,0)); CHECK(r.x>0.99e15f && r.x<1.01e15f); CHECK(r.y==0.0f); }
int main(void){ tc_begin(); f=(fn1_t)tc_crt_proc("csqrtf"); if(!f){ tc_write("TC FAIL " TC_TEST_ID "::__csqrtf missing\n"); return 1; }
 TC_RUN(basic);TC_RUN(three_four);TC_RUN(zero);TC_RUN(negzero_imag);TC_RUN(large); return tc_end(); }
