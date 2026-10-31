/* TC
 * id:    scn_neg_sscanf_alpha_for_number
 * area:  scanf
 * kind:  negative
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p10-12 (input item not matching the expected subject sequence is a matching failure)
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vin;
TC_CASE(abc_ints)
{
    int d = 5; unsigned u = 5, x = 5, o = 5;
    vin = "abc"; CHECK_EQ_I(sscanf((const char *)vin, "%d", &d), 0);
    vin = "abc"; CHECK_EQ_I(sscanf((const char *)vin, "%u", &u), 0);
    vin = "xyz"; CHECK_EQ_I(sscanf((const char *)vin, "%x", &x), 0);
    vin = "abc"; CHECK_EQ_I(sscanf((const char *)vin, "%o", &o), 0);
    CHECK_EQ_I(d, 5); CHECK_EQ_U(u, 5); CHECK_EQ_U(x, 5); CHECK_EQ_U(o, 5);
}
TC_CASE(abc_float) { float f = 2.5f; vin = "abc"; CHECK_EQ_I(sscanf((const char *)vin, "%f", &f), 0); CHECK_BITS_F(f, 2.5f); }
TC_CASE(exponent_only) { float f = 2.5f; vin = "e5"; CHECK_EQ_I(sscanf((const char *)vin, "%f", &f), 0); CHECK_BITS_F(f, 2.5f); }
int main(void) { tc_begin(); TC_RUN(abc_ints); TC_RUN(abc_float); TC_RUN(exponent_only); return tc_end(); }
