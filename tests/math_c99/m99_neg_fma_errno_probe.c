/* TC
 * id:    m99_neg_fma_errno_probe
 * area:  math_c99
 * funcs: fma
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 F.10.10.1; MS docs fma
 * note:  PROBE: errno EDOM/ERANGE or 0 and status-word flags (_SW_INVALID 0x10, _SW_OVERFLOW 0x04) after fma invalid/overflow.
 */
#include "tc.h"
#include <math.h>
#include <float.h>
#include <errno.h>
#include <limits.h>
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
static double d_inf(void) { return v_one / v_zero; }
static double d_nan(void) { return d_inf() - d_inf(); }
static float f_inf(void) { return f_one / f_zero; }
static float f_nan(void) { return f_inf() - f_inf(); }
static double d_snan(void) { union { unsigned long long u; double d; } x; x.u = 0x7ff0000000000001ULL; return x.d; }


TC_CASE(errno_invalid) { volatile double z=0.0, o=1.0; errno=0; (void)fma(z,d_inf(),o); CHECK(errno==0||errno==EDOM); }
TC_CASE(errno_overflow) { volatile double m=DBL_MAX, t=2.0, z=0.0; errno=0; (void)fma(m,t,z); CHECK(errno==0||errno==ERANGE); }
TC_CASE(flag_invalid) { volatile double z=0.0, o=1.0; _clearfp(); (void)fma(z,d_inf(),o); CHECK(_statusfp() & 0x10); }
TC_CASE(flag_overflow) { volatile double m=DBL_MAX, t=2.0, z=0.0; _clearfp(); (void)fma(m,t,z); CHECK(_statusfp() & 0x04); }
int main(void) { tc_begin(); TC_RUN(errno_invalid); TC_RUN(errno_overflow); TC_RUN(flag_invalid); TC_RUN(flag_overflow); return tc_end(); }
