/* TC
 * id:    m99_remainder_neg_errno_probe
 * area:  math_c99
 * funcs: remainder remquo
 * procs: fmod
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.10, 7.12.1 (math_errhandling); MS docs remainder, remquo
 * note:  Calls go to the real ucrtbase export via GetProcAddress (a bare call in a mingw exe lands in libmingwex static code, not the DLL).
 *        PROBE: whether remainder/remquo set errno=EDOM on domain errors (MS math_errhandling has MATH_ERRNO); the fmod(1,0) case records the established EDOM baseline.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>
TC_DECL_PROC(double, fmod, (double, double));
#undef fmod
#define fmod tc_p_fmod
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
static float f_inf(void) { return f_one / f_zero; }
static float f_nan(void) { return f_inf() - f_inf(); }

TC_CASE(remainder_by_zero) { volatile double x = 1.0, z = 0.0; errno = 0; CHECK_IS_NAN(remainder(x, z)); CHECK_EQ_I(errno, EDOM); }
TC_CASE(remquo_inf) { volatile double t = 2.0; double i = d_inf(); int q = 0; errno = 0; CHECK_IS_NAN(remquo(i, t, &q)); CHECK_EQ_I(errno, EDOM); }
TC_CASE(remquo_by_zero) { volatile double x = 1.0, z = 0.0; int q = 0; errno = 0; CHECK_IS_NAN(remquo(x, z, &q)); CHECK_EQ_I(errno, EDOM); }
TC_CASE(fmod_baseline) { volatile double x = 1.0, z = 0.0; errno = 0; CHECK_IS_NAN(fmod(x, z)); CHECK_EQ_I(errno, EDOM); }
int main(void) { tc_begin(); TC_LOAD_PROC(fmod); if (!tc_p_fmod) { tc_write("TC FAIL " TC_TEST_ID "::proc fmod not exported\n"); return 1; } TC_RUN(remainder_by_zero); TC_RUN(remquo_inf); TC_RUN(remquo_by_zero); TC_RUN(fmod_baseline); return tc_end(); }
