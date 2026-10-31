/* TC
 * id:    m99_scalbn_basic
 * area:  math_c99
 * funcs: scalbn scalbnf scalbln scalblnf
 * procs: 
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.12.6.13; MS docs scalbn, scalbln
 * note:  exact scaling
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
static volatile double v_a; static volatile float v_f;
TC_CASE(simple) { v_a = 1; CHECK_BITS_D(scalbn(v_a, 10), 1024.0); v_a = 3; CHECK_BITS_D(scalbn(v_a, -1), 1.5); v_a = 1.25; CHECK_BITS_D(scalbn(v_a, 0), 1.25); }
TC_CASE(to_subnormal) { v_a = 1; CHECK_EQ_U(tc_dbits(scalbn(v_a, -1074)), 1); }
TC_CASE(scalbln_v) { v_a = 1; CHECK_BITS_D(scalbln(v_a, 10), 1024.0); CHECK_EQ_U(tc_dbits(scalbln(v_a, 1023)), 0x7fe0000000000000ULL); }
TC_CASE(float_v) { v_f = 1; CHECK_BITS_F(scalbnf(v_f, 7), 128.0f); CHECK_BITS_F(scalblnf(v_f, -1), 0.5f); }
TC_CASE(neg_zero) { v_a = -0.0; CHECK_BITS_D(scalbn(v_a, 5), -0.0); }
int main(void) { tc_begin(); TC_RUN(simple); TC_RUN(to_subnormal); TC_RUN(scalbln_v); TC_RUN(float_v); TC_RUN(neg_zero); return tc_end(); }
