/* TC
 * id:    m99_neg_lgamma_sign_pole_probe
 * area:  math_c99
 * funcs: 
 * procs: lgamma lgammaf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.8.3 (pole/range errors; errno use is implementation-defined)
 * note:  PROBE: which errno lgamma/lgammaf set on a pole (0, -1, -0.0) and on overflow (1e308). Assumed ERANGE (C: pole error is a range error). Observed errno values are also printed as TC INFO lines.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

typedef double (__cdecl *pd1)(double); typedef float (__cdecl *pf1)(float);
static pd1 p_lg; static pf1 p_lgf;
static volatile double v_zero = 0.0, v_one = 1.0;
#define DINF (v_one / v_zero)

static void info(const char *what, int e)
{ char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write(" errno="); tc_write(b); tc_write("\n"); }

TC_CASE(pole_zero)  { volatile double z = 0.0; double r; if (!p_lg) TC_SKIP("x"); errno = 0; r = p_lg(z); info("lgamma(0)", errno); CHECK(r == DINF); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(pole_neg_zero) { volatile double z = -0.0; double r; if (!p_lg) TC_SKIP("x"); errno = 0; r = p_lg(z); info("lgamma(-0)", errno); CHECK(r == DINF); }
TC_CASE(pole_minus_one) { volatile double z = -1.0; double r; if (!p_lg) TC_SKIP("x"); errno = 0; r = p_lg(z); info("lgamma(-1)", errno); CHECK(r == DINF); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(overflow) { volatile double z = 1e308; double r; if (!p_lg) TC_SKIP("x"); errno = 0; r = p_lg(z); info("lgamma(1e308)", errno); CHECK(r == DINF); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(float_pole) { volatile float z = 0.0f; float r; if (!p_lgf) TC_SKIP("x"); errno = 0; r = p_lgf(z); info("lgammaf(0)", errno); CHECK(r == (float)DINF); CHECK_EQ_I(errno, ERANGE); }
int main(void)
{
    tc_begin();
    p_lg = (pd1)tc_crt_proc("lgamma"); p_lgf = (pf1)tc_crt_proc("lgammaf");
    TC_RUN(pole_zero); TC_RUN(pole_neg_zero); TC_RUN(pole_minus_one); TC_RUN(overflow); TC_RUN(float_pole);
    return tc_end();
}
