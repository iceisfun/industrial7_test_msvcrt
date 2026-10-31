/* TC
 * id:    m99_lgamma_special
 * area:  math_c99
 * funcs: 
 * procs: lgamma
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.5.3
 * note:  lgamma special values
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
typedef double (__cdecl *pd1)(double);
static pd1 p_lg;
static volatile double v_zero = 0.0, v_one = 1.0;
TC_CASE(zero) { volatile double z = -0.0; if (!p_lg) TC_SKIP("no lgamma"); CHECK(p_lg(v_zero) == v_one / v_zero); CHECK(p_lg(z) == v_one / v_zero); }
TC_CASE(inf) { volatile double i = v_one / v_zero; if (!p_lg) TC_SKIP("no lgamma"); CHECK(p_lg(i) == i); CHECK(p_lg(-i) == i); }
TC_CASE(neg_int) { volatile double a = -1.0, b = -2.0; if (!p_lg) TC_SKIP("no lgamma"); CHECK(p_lg(a) == v_one / v_zero); CHECK(p_lg(b) == v_one / v_zero); }
TC_CASE(huge) { volatile double a = 1e308; if (!p_lg) TC_SKIP("no lgamma"); CHECK(p_lg(a) == v_one / v_zero); }
TC_CASE(nan) { volatile double n = (v_one / v_zero) - (v_one / v_zero); if (!p_lg) TC_SKIP("no lgamma"); CHECK_IS_NAN(p_lg(n)); }
int main(void) { tc_begin(); p_lg = (pd1)tc_crt_proc("lgamma");
    TC_RUN(zero); TC_RUN(inf); TC_RUN(neg_int); TC_RUN(huge); TC_RUN(nan); return tc_end(); }
