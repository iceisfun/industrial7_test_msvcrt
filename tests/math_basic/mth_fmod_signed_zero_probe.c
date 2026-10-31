/* TC
 * id:    mth_fmod_signed_zero_probe
 * area:  math_basic
 * funcs: fmod
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 F.10.7.1 (fmod(+-0, y) is +-0; zero result has the sign of x)
 * note:  probing signed-zero results of fmod on every CRT incl. old x64 msvcrt/msvcr80-100, whose handling is undocumented
 */
#include "tc.h"
#include <math.h>

static volatile double v_zero = 0.0, v_one = 1.0, v_inf, v_nan;
static void init(void) { v_inf = v_one / v_zero; v_nan = v_inf - v_inf; }

TC_CASE(zero_numer) { volatile double z = 0.0, nz = -0.0, f = 5.0; CHECK_BITS_D(fmod(z, f), 0.0); CHECK_BITS_D(fmod(nz, f), -0.0); }
TC_CASE(exact_multiple) { volatile double a = 6.0, b = -6.0, c = 3.0; CHECK_BITS_D(fmod(a, c), 0.0); CHECK_BITS_D(fmod(b, c), -0.0); }

int main(void) { tc_begin(); init(); TC_RUN(zero_numer); TC_RUN(exact_multiple); return tc_end(); }
