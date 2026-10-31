/* TC
 * id:    mf_modff_basic
 * area:  math_float
 * funcs: modff
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  standard
 * ref:   C11 F.10.3.12; MS docs modf, modff
 * note:  -0 fractional parts and infinity args are in mf_modff_special_probe
 */
#include "tc.h"
#include <math.h>

TC_CASE(pos) { volatile float x = 3.75f; float i = -1.0f, r = modff(x, &i); CHECK_BITS_F(r, 0.75f); CHECK_BITS_F(i, 3.0f); }
TC_CASE(neg) { volatile float x = -3.75f; float i = -1.0f, r = modff(x, &i); CHECK_BITS_F(r, -0.75f); CHECK_BITS_F(i, -3.0f); }
TC_CASE(int_pos) { volatile float x = 5.0f; float i = -1.0f, r = modff(x, &i); CHECK_BITS_F(r, 0.0f); CHECK_BITS_F(i, 5.0f); }
TC_CASE(int_neg) { volatile float x = -5.0f; float i = -1.0f; (void)modff(x, &i); CHECK_BITS_F(i, -5.0f); }
TC_CASE(small) { volatile float x = 0.25f; float i = -1.0f, r = modff(x, &i); CHECK_BITS_F(r, 0.25f); CHECK_BITS_F(i, 0.0f); }

int main(void) { tc_begin(); TC_RUN(pos); TC_RUN(neg); TC_RUN(int_pos); TC_RUN(int_neg); TC_RUN(small); return tc_end(); }
