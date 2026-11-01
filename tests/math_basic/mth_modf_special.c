/* TC
 * id:    mth_modf_special
 * area:  math_basic
 * funcs:
 * procs: modf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 F.10.3.12; MS docs modf
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
TC_DECL_PROC(double, modf, (double, double *));
#undef modf
#define modf tc_p_modf

static volatile double v_zero = 0.0, v_one = 1.0;

TC_CASE(pos_inf) { volatile double x = v_one / v_zero; double ip = -99; double f = modf(x, &ip); CHECK_BITS_D(f, 0.0); CHECK(ip == x); }
TC_CASE(neg_inf) { volatile double x = -(v_one / v_zero); double ip = -99; double f = modf(x, &ip); CHECK_BITS_D(f, -0.0); CHECK(ip == x); }

int main(void) { tc_begin(); TC_LOAD_PROC(modf); if (!tc_p_modf) { tc_write("TC FAIL " TC_TEST_ID "::proc modf not exported\n"); return 1; } TC_RUN(pos_inf); TC_RUN(neg_inf); return tc_end(); }
