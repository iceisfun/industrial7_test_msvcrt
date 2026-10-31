/* TC
 * id:    scn_neg_sscanf_float_nomatch
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.21.6.2p12-16 (matching failure / input failure return values); MS docs sscanf return value
 * note:  No-digit float text is a matching failure: returns 0 (or EOF if input ends right after "-" / "."; C counts that as input failure, so both accepted) and leaves the target untouched; empty/whitespace-only input is an input failure returning EOF.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static double chk(const char *s, int *ret, const char *fmt)
{
    double d = 123.0;
    *ret = sscanf(s, fmt, &d);
    return d;
}
TC_CASE(letters)  { int r; double d = chk("abc", &r, "%lf"); CHECK_EQ_I(r, 0); CHECK_BITS_D(d, 123.0); }
TC_CASE(sign_only){ int r; double d = chk("-", &r, "%lf"); CHECK(r == 0 || r == EOF); CHECK_BITS_D(d, 123.0); }
TC_CASE(lone_dot) { int r; double d = chk(".", &r, "%lf"); CHECK(r == 0 || r == EOF); CHECK_BITS_D(d, 123.0); }
TC_CASE(exp_only) { int r; double d = chk("e5", &r, "%lf"); CHECK_EQ_I(r, 0); CHECK_BITS_D(d, 123.0); }
TC_CASE(empty)    { int r; double d = chk("", &r, "%lf"); CHECK_EQ_I(r, EOF); CHECK_BITS_D(d, 123.0); }
TC_CASE(spaces)   { int r; double d = chk("  \t\n", &r, "%lf"); CHECK_EQ_I(r, EOF); CHECK_BITS_D(d, 123.0); }
TC_CASE(width_zero_digits) { int r; double d = chk("-x1", &r, "%3lf"); CHECK_EQ_I(r, 0); CHECK_BITS_D(d, 123.0); }
TC_CASE(second_fails) { double a = 1, b = 123.0; int r = sscanf("2.5 zz", "%lf %lf", &a, &b); CHECK_EQ_I(r, 1); CHECK_BITS_D(a, 2.5); CHECK_BITS_D(b, 123.0); }

int main(void) { tc_begin(); TC_RUN(letters); TC_RUN(sign_only); TC_RUN(lone_dot); TC_RUN(exp_only); TC_RUN(empty); TC_RUN(spaces); TC_RUN(width_zero_digits); TC_RUN(second_fails); return tc_end(); }
