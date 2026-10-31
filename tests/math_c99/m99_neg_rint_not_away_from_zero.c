/* TC
 * id:    m99_neg_rint_not_away_from_zero
 * area:  math_c99
 * funcs: rint nearbyint lrint llrint rintf nearbyintf round lround
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.12.9.3-7.12.9.5, F.10.6.4 (current rounding mode, default to-nearest-even); MS docs rint/lrint
 * note:  Zero results are compared by value only (sign of zero for rint(-0.5) is probed separately). Default rounding mode ties-to-even; contrasts with round/lround (half away from zero).
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


TC_CASE(rint_table) { volatile double a=0.5,b=1.5,c=2.5,d=-2.5,e=-0.5,f=3.5;
    CHECK_BITS_D(rint(a),0.0); CHECK_BITS_D(rint(b),2.0); CHECK_BITS_D(rint(c),2.0); CHECK_BITS_D(rint(d),-2.0); CHECK(rint(e)==0.0); CHECK_BITS_D(rint(f),4.0); }
TC_CASE(nearbyint_table) { volatile double a=0.5,b=1.5,c=2.5,d=-2.5,e=-0.5;
    CHECK_BITS_D(nearbyint(a),0.0); CHECK_BITS_D(nearbyint(b),2.0); CHECK_BITS_D(nearbyint(c),2.0); CHECK_BITS_D(nearbyint(d),-2.0); CHECK(nearbyint(e)==0.0); }
TC_CASE(float_table) { volatile float a=0.5f,b=2.5f,c=-2.5f,d=1.5f;
    CHECK_BITS_F(rintf(a),0.0f); CHECK_BITS_F(rintf(b),2.0f); CHECK_BITS_F(rintf(c),-2.0f); CHECK_BITS_F(rintf(d),2.0f);
    CHECK_BITS_F(nearbyintf(a),0.0f); CHECK_BITS_F(nearbyintf(b),2.0f); CHECK_BITS_F(nearbyintf(c),-2.0f); CHECK_BITS_F(nearbyintf(d),2.0f); }
TC_CASE(lrint_table) { volatile double a=2.5,b=-2.5,c=3.5,d=0.5,e=-1.5;
    CHECK_EQ_I(lrint(a),2); CHECK_EQ_I(lrint(b),-2); CHECK_EQ_I(lrint(c),4); CHECK_EQ_I(llrint(d),0); CHECK_EQ_I(llrint(e),-2); }
TC_CASE(contrast_round) { volatile double a=2.5; CHECK_BITS_D(round(a),3.0); CHECK_EQ_I(lround(a),3); CHECK(rint(a) != round(a)); CHECK(lrint(a) != lround(a)); }
int main(void) { tc_begin(); TC_RUN(rint_table); TC_RUN(nearbyint_table); TC_RUN(float_table); TC_RUN(lrint_table); TC_RUN(contrast_round); return tc_end(); }
