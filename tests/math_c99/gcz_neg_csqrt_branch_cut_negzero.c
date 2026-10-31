/* TC
 * id:    gcz_neg_csqrt_branch_cut_negzero
 * area:  math_c99
 * funcs:
 * procs: csqrt
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 G.6.4.2; MS docs csqrt
 * note:  Result must follow the sign of the zero imaginary part on the negative real axis.
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
TC_CASE(neg4_negzero){ dc_t r=f(mk(-4,-0.0)); CHECK_BITS_D(r.x,0.0); CHECK_BITS_D(r.y,-2.0); }
TC_CASE(neg4_poszero){ dc_t r=f(mk(-4,0.0)); CHECK_BITS_D(r.x,0.0); CHECK_BITS_D(r.y,2.0); }
TC_CASE(negzero_poszero){ dc_t r=f(mk(-0.0,0.0)); CHECK_BITS_D(r.x,0.0); CHECK_BITS_D(r.y,0.0); }
TC_CASE(negzero_negzero){ dc_t r=f(mk(-0.0,-0.0)); CHECK_BITS_D(r.x,0.0); CHECK_BITS_D(r.y,-0.0); }
TC_CASE(neginf){ dc_t r=f(mk(-pinf(),1)); CHECK_BITS_D(r.x,0.0); CHECK(r.y==pinf()); }
TC_CASE(inf_nan){ dc_t r=f(mk(pinf(),qnan())); CHECK(r.x==pinf()); CHECK_IS_NAN(r.y); }
TC_CASE(conj_symmetry){ double xs[3]={2,-3,-5.5}, ys[3]={1.5,4,2}; int i; for(i=0;i<3;i++){ dc_t a=f(mk(xs[i],ys[i])), b=f(mk(xs[i],-ys[i])); CHECK_NEAR(a.x,b.x,1e-14); CHECK_NEAR(a.y,-b.y,1e-14); } }
int main(void){ tc_begin(); f=(fn1_t)tc_crt_proc("csqrt"); if(!f){ tc_write("TC FAIL " TC_TEST_ID "::__csqrt missing\n"); return 1; }
 TC_RUN(neg4_negzero);TC_RUN(neg4_poszero);TC_RUN(negzero_poszero);TC_RUN(negzero_negzero);TC_RUN(neginf);TC_RUN(inf_nan);TC_RUN(conj_symmetry); return tc_end(); }
