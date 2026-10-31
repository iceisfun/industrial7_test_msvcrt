/* TC
 * id:    mf_floorf_ceilf_basic
 * area:  math_float
 * funcs: floorf ceilf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.9.2, 7.12.9.1; MS docs floor, ceil
 */
#include "tc.h"
#include <math.h>

TC_CASE(frac) { volatile float a = 2.5f, b = -2.5f; CHECK_BITS_F(floorf(a), 2.0f); CHECK_BITS_F(ceilf(a), 3.0f); CHECK_BITS_F(floorf(b), -3.0f); CHECK_BITS_F(ceilf(b), -2.0f); }
TC_CASE(integers) { volatile float a = 7.0f, b = -7.0f; CHECK_BITS_F(floorf(a), 7.0f); CHECK_BITS_F(ceilf(a), 7.0f); CHECK_BITS_F(floorf(b), -7.0f); CHECK_BITS_F(ceilf(b), -7.0f); }
TC_CASE(two_pow_23) { volatile float a = 8388608.0f, b = -8388608.0f; CHECK_BITS_F(floorf(a), 8388608.0f); CHECK_BITS_F(ceilf(a), 8388608.0f); CHECK_BITS_F(floorf(b), -8388608.0f); CHECK_BITS_F(ceilf(b), -8388608.0f); }
TC_CASE(just_below_2pow23) { volatile float a = 8388607.5f; CHECK_BITS_F(floorf(a), 8388607.0f); CHECK_BITS_F(ceilf(a), 8388608.0f); }
TC_CASE(tiny) { volatile float a = -1e-30f, b = 1e-30f; CHECK_BITS_F(floorf(a), -1.0f); CHECK_BITS_F(ceilf(b), 1.0f); }

int main(void) { tc_begin(); TC_RUN(frac); TC_RUN(integers); TC_RUN(two_pow_23); TC_RUN(just_below_2pow23); TC_RUN(tiny); return tc_end(); }
