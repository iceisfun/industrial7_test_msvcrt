/* TC
 * id:    m99_log2_exp2_exact_probe
 * area:  math_c99
 * funcs: log2 exp2
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.12.6.4, 7.12.7.2 (no exactness requirement); MS docs log2, exp2
 * note:  probes that log2/exp2 return bit-exact results for exact powers of two, and exp2(-1074) is the exact
 *        smallest denormal (not flushed). Expected on a quality implementation; not guaranteed by the standard.
 */
#include "tc.h"
#include <math.h>
TC_CASE(log2_pow2) { volatile double b = 2, c = 8, d = 1024, e = 0.5;
    CHECK_BITS_D(log2(b), 1.0); CHECK_BITS_D(log2(c), 3.0); CHECK_BITS_D(log2(d), 10.0); CHECK_BITS_D(log2(e), -1.0); }
TC_CASE(exp2_int) { volatile double b = 10, c = -1, d = 3, e = 1023;
    CHECK_BITS_D(exp2(b), 1024.0); CHECK_BITS_D(exp2(c), 0.5); CHECK_BITS_D(exp2(d), 8.0); CHECK_BITS_D(exp2(e), 0x1p1023); }
TC_CASE(exp2_denorm) { volatile double a = -1074; CHECK_BITS_D(exp2(a), 4.9406564584124654e-324); }
TC_CASE(float_pow2) { volatile float a = 8.0f, b = 4.0f; CHECK_BITS_F(log2f(a), 3.0f); CHECK_BITS_F(exp2f(b), 16.0f); }
int main(void) { tc_begin(); TC_RUN(log2_pow2); TC_RUN(exp2_int); TC_RUN(exp2_denorm); TC_RUN(float_pow2); return tc_end(); }
