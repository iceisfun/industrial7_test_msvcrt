/* TC
 * id:    m99_neg_fmax_nan_snan_probe
 * area:  math_c99
 * funcs: fmax fmin
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   IEEE 754-2008 maxNum/minNum; C11 F.10.9.2
 * note:  PROBE: signalling NaN operands: numeric result returned, FE_INVALID (_SW_INVALID 0x10) recorded, quiet bit of NaN result, fmin(-0.0,NaN) sign, errno unchanged (0) for NaN operands. On x86 (32-bit) d_snan() returned via x87 would be quieted; x64 only.
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


TC_CASE(snan_numeric) { volatile double s=d_snan(), o=1.0; CHECK_BITS_D(fmax(s,o), 1.0); CHECK_BITS_D(fmin(o,s), 1.0); }
TC_CASE(snan_invalid_flag) { volatile double s=d_snan(), o=1.0; _clearfp(); (void)fmax(s,o); CHECK(_statusfp() & 0x10); }
TC_CASE(snan_snan_quiet) { volatile double s=d_snan(); double r=fmax(s,s); CHECK_IS_NAN(r); CHECK(tc_dbits(r) & 0x0008000000000000ULL); }
TC_CASE(negzero_nan) { volatile double z=-0.0, n=d_nan(); CHECK_BITS_D(fmin(z,n), -0.0); }
TC_CASE(errno_zero) { volatile double n=d_nan(), o=1.0; errno=0; (void)fmax(n,o); (void)fmin(o,n); (void)fmax(n,n); CHECK_EQ_I(errno,0); }
int main(void) { tc_begin(); TC_RUN(snan_numeric); TC_RUN(snan_invalid_flag); TC_RUN(snan_snan_quiet); TC_RUN(negzero_nan); TC_RUN(errno_zero); return tc_end(); }
