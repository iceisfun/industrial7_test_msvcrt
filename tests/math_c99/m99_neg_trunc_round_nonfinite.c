/* TC
 * id:    m99_neg_trunc_round_nonfinite
 * area:  math_c99
 * funcs: trunc truncf round roundf nearbyint nearbyintf rint rintf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 F.10.6.5-F.10.6.10 (rounding functions return arg for +-0, +-inf; 7.12.1 no errno clear); MS docs trunc/round/rint/nearbyint
 * note:  errno sentinel ERANGE must be left untouched (library functions never set errno to zero). Large-value rounding must be identity.
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


typedef double (*dfn)(double); typedef float (*ffn)(float);
static double dtrunc(double x){return trunc(x);} static double dround(double x){return round(x);}
static double dnear(double x){return nearbyint(x);} static double drint(double x){return rint(x);}
static float ftrunc(float x){return truncf(x);} static float fround(float x){return roundf(x);}
static float fnear(float x){return nearbyintf(x);} static float frint(float x){return rintf(x);}
static const dfn dfns[4] = { dtrunc, dround, dnear, drint };
static const ffn ffns[4] = { ftrunc, fround, fnear, frint };
TC_CASE(inf_untouched) { int i; for (i = 0; i < 4; i++) { volatile double p = d_inf(), n = -d_inf(); errno = ERANGE;
    CHECK_BITS_D(dfns[i](p), d_inf()); CHECK_BITS_D(dfns[i](n), -d_inf()); CHECK_EQ_I(errno, ERANGE); } }
TC_CASE(nan_untouched) { int i; for (i = 0; i < 4; i++) { volatile double q = d_nan(); double r; errno = ERANGE; r = dfns[i](q); CHECK_IS_NAN(r); CHECK_EQ_I(errno, ERANGE); } }
TC_CASE(huge_identity) { int i; volatile double a = 9007199254740994.0, b = 1e300, c = DBL_MAX, d = -DBL_MAX;
    for (i = 0; i < 4; i++) { errno = 0; CHECK_BITS_D(dfns[i](a), 9007199254740994.0); CHECK_BITS_D(dfns[i](b), 1e300);
    CHECK_BITS_D(dfns[i](c), DBL_MAX); CHECK_BITS_D(dfns[i](d), -DBL_MAX); CHECK(errno != ERANGE); } }
TC_CASE(float_identity) { int i; volatile float a = 8388608.0f, b = 1e30f, c = FLT_MAX;
    for (i = 0; i < 4; i++) { CHECK_BITS_F(ffns[i](a), 8388608.0f); CHECK_BITS_F(ffns[i](b), 1e30f); CHECK_BITS_F(ffns[i](c), FLT_MAX); } }
TC_CASE(negative_zero) { volatile double z = -0.0, p = 0.0;
    CHECK_BITS_D(nearbyint(z), -0.0); CHECK_BITS_D(trunc(z), -0.0); CHECK_BITS_D(round(z), -0.0); CHECK_BITS_D(rint(z), -0.0);
    CHECK_BITS_D(trunc(p), 0.0); CHECK_BITS_D(round(p), 0.0); }
TC_CASE(round_half_away) { volatile double a = -0.5, b = 2.5, c = -2.5; CHECK_BITS_D(round(a), -1.0); CHECK_BITS_D(round(b), 3.0); CHECK_BITS_D(round(c), -3.0); }
int main(void) { tc_begin(); TC_RUN(inf_untouched); TC_RUN(nan_untouched); TC_RUN(huge_identity); TC_RUN(float_identity); TC_RUN(negative_zero); TC_RUN(round_half_away); return tc_end(); }
