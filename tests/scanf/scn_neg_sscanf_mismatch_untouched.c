/* TC
 * id:    scn_neg_sscanf_mismatch_untouched
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p9-12 (matching failure: no assignment for the failed directive)
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vin;
TC_CASE(d_alpha) { int x = 0x5a5a, n; vin = "abc"; n = sscanf((const char *)vin, "%d", &x); CHECK_EQ_I(n, 0); CHECK_EQ_I(x, 0x5a5a); }
TC_CASE(f_comma) { float f = 1.5f; int n; vin = ","; n = sscanf((const char *)vin, "%f", &f); CHECK_EQ_I(n, 0); CHECK_BITS_F(f, 1.5f); }
TC_CASE(s_empty_c_empty)
{
    char s[4] = {'Q','Q','Q','Q'}, c = 'Q'; int n;
    vin = ""; n = sscanf((const char *)vin, "%s", s); CHECK_EQ_I(n, EOF); CHECK_EQ_I(s[0], 'Q');
    vin = ""; n = sscanf((const char *)vin, "%c", &c); CHECK_EQ_I(n, EOF); CHECK_EQ_I(c, 'Q');
}
TC_CASE(x_zz) { unsigned x = 0x5a5a; int n; vin = "zz"; n = sscanf((const char *)vin, "%x", &x); CHECK_EQ_I(n, 0); CHECK_EQ_U(x, 0x5a5a); }
TC_CASE(second_fails_first_kept)
{
    int a = 0x5a5a, b = 0x5a5a, n; vin = "12 xyz"; n = sscanf((const char *)vin, "%d %d", &a, &b);
    CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 12); CHECK_EQ_I(b, 0x5a5a);
}
int main(void) { tc_begin(); TC_RUN(d_alpha); TC_RUN(f_comma); TC_RUN(s_empty_c_empty); TC_RUN(x_zz); TC_RUN(second_fails_first_kept); return tc_end(); }
