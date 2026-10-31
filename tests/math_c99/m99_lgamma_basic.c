/* TC
 * id:    m99_lgamma_basic
 * area:  math_c99
 * funcs: 
 * procs: lgamma lgammaf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.12.8.3; MS docs lgamma
 * note:  via procs because the mingw ucrtbase import lib lacks it
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
typedef double (__cdecl *pd1)(double); typedef float (__cdecl *pf1)(float);
static pd1 p_lg; static pf1 p_lgf;
static volatile double v_a;
TC_CASE(exported) { CHECK(p_lg != NULL); CHECK(p_lgf != NULL); }
TC_CASE(zeros) { if (!p_lg) TC_SKIP("no lgamma"); v_a = 1.0; CHECK_BITS_D(p_lg(v_a), 0.0); v_a = 2.0; CHECK_BITS_D(p_lg(v_a), 0.0); }
TC_CASE(values) { if (!p_lg) TC_SKIP("no lgamma");
    v_a = 3.0; CHECK_NEAR(p_lg(v_a), 0.6931471805599453, 1e-14);
    v_a = 10.0; CHECK_NEAR(p_lg(v_a), 12.801827480081469, 1e-12);
    v_a = 0.5; CHECK_NEAR(p_lg(v_a), 0.5723649429247001, 1e-14);
    v_a = -0.5; CHECK_NEAR(p_lg(v_a), 1.2655121234846454, 1e-13); }
TC_CASE(floatv) { volatile float f = 5.0f; if (!p_lgf) TC_SKIP("no lgammaf"); CHECK_NEAR(p_lgf(f), 3.1780538303479458, 1e-5); }
int main(void) { tc_begin(); p_lg = (pd1)tc_crt_proc("lgamma"); p_lgf = (pf1)tc_crt_proc("lgammaf");
    TC_RUN(exported); TC_RUN(zeros); TC_RUN(values); TC_RUN(floatv); return tc_end(); }
