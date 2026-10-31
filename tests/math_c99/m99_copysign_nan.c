/* TC
 * id:    m99_copysign_nan
 * area:  math_c99
 * funcs:
 * procs: copysign copysignf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.8.1; MS docs copysign
 * note:  copysign is exact; the sign of a NaN operand is honoured/propagated. Only NaN-ness and sign are asserted, not payload.
 */
#include "tc.h"
#include <math.h>
TC_DECL_PROC(double, copysign, (double, double));
TC_DECL_PROC(float, copysignf, (float, float));
static unsigned long long dnan_bits(int neg) { return neg ? 0xfff8000000000000ULL : 0x7ff8000000000000ULL; }
static double mkd(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static float mkf(unsigned int u) { union { float f; unsigned int u; } x; x.u = u; return x.f; }
static int d_isnan_bits(double d) { unsigned long long u = tc_dbits(d); return (u & 0x7ff0000000000000ULL) == 0x7ff0000000000000ULL && (u & 0x000fffffffffffffULL); }
static int f_isnan_bits(float f) { unsigned int u = tc_fbits(f); return (u & 0x7f800000u) == 0x7f800000u && (u & 0x007fffffu); }
TC_CASE(nan_gets_negative) { volatile double n = mkd(dnan_bits(0)), m = -1.0; double r = tc_p_copysign(n, m);
    CHECK(d_isnan_bits(r)); CHECK((tc_dbits(r) >> 63) == 1); }
TC_CASE(nan_gets_positive) { volatile double n = mkd(dnan_bits(1)), p = 1.0; double r = tc_p_copysign(n, p);
    CHECK(d_isnan_bits(r)); CHECK((tc_dbits(r) >> 63) == 0); }
TC_CASE(sign_from_nan) { volatile double neg = mkd(dnan_bits(1)), pos = mkd(dnan_bits(0)), one = 1.0;
    CHECK_BITS_D(tc_p_copysign(one, neg), -1.0); CHECK_BITS_D(tc_p_copysign(one, pos), 1.0); }
TC_CASE(float_variants) { volatile float n = mkf(0x7fc00000u), nn = mkf(0xffc00000u), m = -1.0f, p = 1.0f, one = 1.0f; float r = tc_p_copysignf(n, m), s = tc_p_copysignf(nn, p);
    CHECK(f_isnan_bits(r)); CHECK((tc_fbits(r) >> 31) == 1); CHECK(f_isnan_bits(s)); CHECK((tc_fbits(s) >> 31) == 0);
    CHECK_BITS_F(tc_p_copysignf(one, nn), -1.0f); CHECK_BITS_F(tc_p_copysignf(one, n), 1.0f); }
int main(void) { tc_begin(); TC_LOAD_PROC(copysign); TC_LOAD_PROC(copysignf); if (!tc_p_copysign || !tc_p_copysignf) { tc_write("TC FAIL copysign not exported\n"); return 1; } TC_RUN(nan_gets_negative); TC_RUN(nan_gets_positive); TC_RUN(sign_from_nan); TC_RUN(float_variants); return tc_end(); }
