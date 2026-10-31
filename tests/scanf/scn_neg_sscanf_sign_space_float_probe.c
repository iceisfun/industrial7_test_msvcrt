/* TC
 * id:    scn_neg_sscanf_sign_space_float_probe
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.2 p10 (no white space between sign and digits)
 * note:  PROBE. "- 5", "+-5" and "--5" with %lf: expected a matching failure (return 0, output and %n untouched). The probed
 *        point is the RETURN VALUE: 0 (standard) versus EOF (-1) if the CRT's float scanner reports the stop as end of input.
 *        Output/%n untouched is asserted in separate cases so a wrong return guess does not mask them.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vs;
TC_CASE(f_minus_space_ret) { double v = 99.0; int n; vs = "- 5"; n = sscanf((const char *)vs, "%lf", &v); CHECK_EQ_I(n, 0); }
TC_CASE(f_double_sign_ret) { double v = 99.0; int n; vs = "+-5"; n = sscanf((const char *)vs, "%lf", &v); CHECK_EQ_I(n, 0); vs = "--5"; n = sscanf((const char *)vs, "%lf", &v); CHECK_EQ_I(n, 0); }
TC_CASE(f_untouched)
{
    double v = 99.0; int k = -7; vs = "- 5"; (void)sscanf((const char *)vs, "%lf%n", &v, &k);
    CHECK_BITS_D(v, 99.0); CHECK_EQ_I(k, -7);
    vs = "+-5"; (void)sscanf((const char *)vs, "%lf%n", &v, &k); CHECK_BITS_D(v, 99.0); CHECK_EQ_I(k, -7);
}
int main(void) { tc_begin(); TC_RUN(f_minus_space_ret); TC_RUN(f_double_sign_ret); TC_RUN(f_untouched); return tc_end(); }
