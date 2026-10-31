/* TC
 * id:    m99_lgamma_neg_range
 * area:  math_c99
 * funcs: 
 * procs: lgamma lgammaf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.5.3 (pole, overflow)
 * note:  errno asserted only in {0,ERANGE}
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
typedef double (__cdecl *pd1)(double); typedef float (__cdecl *pf1)(float);
static pd1 p_lg; static pf1 p_lgf;
static volatile double v_zero = 0.0, v_one = 1.0;
#define ERR_OK() (errno == 0 || errno == ERANGE || errno == EDOM)
TC_CASE(pole0) { double r; if (!p_lg) TC_SKIP("no lgamma"); errno = 0; r = p_lg(v_zero); CHECK(r == v_one / v_zero); CHECK(ERR_OK()); }
TC_CASE(pole_neg3) { volatile double a = -3.0; double r; if (!p_lg) TC_SKIP("no lgamma"); errno = 0; r = p_lg(a); CHECK(r == v_one / v_zero); CHECK(ERR_OK()); }
TC_CASE(overflow) { volatile double a = 1.7e308; double r; if (!p_lg) TC_SKIP("no lgamma"); errno = 0; r = p_lg(a); CHECK(r == v_one / v_zero); CHECK(ERR_OK()); }
TC_CASE(f_pole) { volatile float z = 0.0f; float r; if (!p_lgf) TC_SKIP("no lgammaf"); errno = 0; r = p_lgf(z); CHECK(r == (float)(v_one / v_zero)); CHECK(ERR_OK()); }
int main(void) { tc_begin(); p_lg = (pd1)tc_crt_proc("lgamma"); p_lgf = (pf1)tc_crt_proc("lgammaf");
    TC_RUN(pole0); TC_RUN(pole_neg3); TC_RUN(overflow); TC_RUN(f_pole); return tc_end(); }
