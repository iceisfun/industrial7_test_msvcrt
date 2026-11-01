/* TC
 * id:    m99_neg_lrint_range_nan_probe
 * area:  math_c99
 * funcs: lrint llrint lrintf llrintf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   C11 F.10.6.5 (unspecified, FE_INVALID); MS docs lrint (returns 0x80000000 style on failure not documented)
 * note:  OBSERVED on Win10 22H2 (guest -cpu host; result paths may differ on CPUs without the same ISA extensions): lrint(2^31)=lrint(-2^31-1)=0, llrint(+inf)=llrint(NaN)=0,
 *        lrintf(1e10f)=0, llrintf(-inf)=0 (NOT the x86 integer indefinite); llrint(2^63)=LLONG_MIN, in-range values exact, errno not ERANGE, FE_INVALID set for NaN. C11 F.10.6.5 leaves out-of-range results unspecified.
 *        PROBE: out-of-range/NaN/Inf results of lrint family are the x86 integer indefinite (LONG_MIN/LLONG_MIN); errno must not be ERANGE; FE_INVALID (_SW_INVALID) set for NaN.
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


TC_CASE(lrint_range) { volatile double a=2147483648.0,b=-2147483649.0,c=2147483647.0,d=-2147483648.0;
    CHECK_EQ_I(lrint(c), LONG_MAX); CHECK_EQ_I(lrint(d), LONG_MIN); CHECK_EQ_I(lrint(a), 0); CHECK_EQ_I(lrint(b), 0); }
TC_CASE(llrint_range) { volatile double a=9223372036854775808.0; volatile double i=d_inf(), n=d_nan();
    CHECK_EQ_I(llrint(a), LLONG_MIN); CHECK_EQ_I(llrint(i), 0); CHECK_EQ_I(llrint(n), 0); }
TC_CASE(float_range) { volatile float a=1e10f, i=f_inf(); CHECK_EQ_I(lrintf(a), 0); CHECK_EQ_I(llrintf(-i), 0); }
TC_CASE(errno_not_erange) { volatile double a=2147483648.0, n=d_nan(); errno=0; (void)lrint(a); CHECK(errno != ERANGE);
    errno=0; (void)llrint(n); CHECK(errno != ERANGE); errno=0; (void)lrint(n); CHECK(errno == 0 || errno == EDOM); }
TC_CASE(fe_invalid_nan) { volatile double n=d_nan(); _clearfp(); (void)lrint(n); CHECK(_statusfp() & 0x10); }
int main(void) { tc_begin(); TC_RUN(lrint_range); TC_RUN(llrint_range); TC_RUN(float_range); TC_RUN(errno_not_erange); TC_RUN(fe_invalid_nan); return tc_end(); }
