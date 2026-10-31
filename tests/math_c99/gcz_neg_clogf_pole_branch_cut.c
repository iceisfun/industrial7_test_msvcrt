/* TC
 * id:    gcz_neg_clogf_pole_branch_cut
 * area:  math_c99
 * funcs:
 * procs: clogf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 G.6.3.2; MS docs clog
 * note:  Pole divide-by-zero exception/errno not asserted.
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
TC_CASE(pos_zero){ fc_t r=f(mk(0.0f,0.0f)); CHECK(r.x==-pinf()); CHECK_BITS_F(r.y,0.0f); }
TC_CASE(negzero_poszero){ fc_t r=f(mk(-0.0f,0.0f)); CHECK(r.x==-pinf()); CHECK_NEAR(r.y,PI,1e-6); }
TC_CASE(negzero_negzero){ fc_t r=f(mk(-0.0f,-0.0f)); CHECK(r.x==-pinf()); CHECK_NEAR(r.y,-PI,1e-6); }
TC_CASE(cut_below){ fc_t r=f(mk(-1,-0.0f)); CHECK_NEAR(r.x,0,1e-6); CHECK_NEAR(r.y,-PI,1e-6); }
TC_CASE(neginf){ fc_t r=f(mk(-pinf(),1)); CHECK(r.x==pinf()); CHECK_NEAR(r.y,PI,1e-6); }
TC_CASE(inf_nan){ fc_t r=f(mk(pinf(),qnan())); CHECK(r.x==pinf()); CHECK_IS_NAN(r.y); }
TC_CASE(nan_nan){ fc_t r=f(mk(qnan(),qnan())); CHECK_IS_NAN(r.x); CHECK_IS_NAN(r.y); }
int main(void){ tc_begin(); f=(fn1_t)tc_crt_proc("clogf"); if(!f){ tc_write("TC FAIL " TC_TEST_ID "::__clogf missing\n"); return 1; }
 TC_RUN(pos_zero);TC_RUN(negzero_poszero);TC_RUN(negzero_negzero);TC_RUN(cut_below);TC_RUN(neginf);TC_RUN(inf_nan);TC_RUN(nan_nan); return tc_end(); }
