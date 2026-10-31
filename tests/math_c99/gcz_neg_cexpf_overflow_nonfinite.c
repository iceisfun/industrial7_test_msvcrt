/* TC
 * id:    gcz_neg_cexpf_overflow_nonfinite
 * area:  math_c99
 * funcs:
 * procs: cexpf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 G.6.3.1; MS docs cexp
 * note:  errno and the imaginary part of overflowed results are deliberately not asserted.
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
TC_CASE(neginf){ fc_t r=f(mk(-pinf(),0)); CHECK_BITS_F(r.x,0.0f); CHECK(r.y==0.0f); }
TC_CASE(posinf){ fc_t r=f(mk(pinf(),0)); CHECK(r.x==pinf()); CHECK(r.y==0.0f); }
TC_CASE(neginf_infimag){ fc_t r=f(mk(-pinf(),pinf())); CHECK(r.x==0.0f); CHECK(r.y==0.0f); }
TC_CASE(infimag){ fc_t r=f(mk(0,pinf())); CHECK_IS_NAN(r.x); CHECK_IS_NAN(r.y); }
TC_CASE(nan_zero){ fc_t r=f(mk(qnan(),0)); CHECK_IS_NAN(r.x); CHECK(r.y==0.0f); }
TC_CASE(overflow){ fc_t r=f(mk(100,0)); CHECK(r.x==pinf()); }
TC_CASE(underflow){ fc_t r=f(mk(-200,0)); CHECK(r.x==0.0f); }
int main(void){ tc_begin(); f=(fn1_t)tc_crt_proc("cexpf"); if(!f){ tc_write("TC FAIL " TC_TEST_ID "::__cexpf missing\n"); return 1; }
 TC_RUN(neginf);TC_RUN(posinf);TC_RUN(neginf_infimag);TC_RUN(infimag);TC_RUN(nan_zero);TC_RUN(overflow);TC_RUN(underflow); return tc_end(); }
