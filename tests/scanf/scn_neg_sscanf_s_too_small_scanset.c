/* TC
 * id:    scn_neg_sscanf_s_too_small_scanset
 * area:  scanf
 * kind:  negative
 * funcs: sscanf_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs scanf_s: size argument follows %[ pointer; overflow invokes the invalid parameter handler
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vin;
TC_CASE(overflow_iph)
{
    char a[8]; int i; for (i = 0; i < 8; i++) a[i] = 'Z';
    CHECK(tc_iph_install()); tc_iph_count = 0; vin = "abcdefgh";
    (void)sscanf_s((const char *)vin, "%[a-z]", a, (unsigned)4);
    CHECK(tc_iph_count >= 1);
    CHECK_EQ_I(a[4], 'Z'); CHECK_EQ_I(a[7], 'Z');
}
TC_CASE(width_below_size_ok)
{
    char a[8]; int n, i; for (i = 0; i < 8; i++) a[i] = 'Z';
    CHECK(tc_iph_install()); tc_iph_count = 0; vin = "abcdefgh";
    n = sscanf_s((const char *)vin, "%3[a-z]", a, (unsigned)4);
    CHECK_EQ_I(n, 1); CHECK_STR(a, "abc"); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(a[4], 'Z');
}
int main(void) { tc_begin(); TC_RUN(overflow_iph); TC_RUN(width_below_size_ok); return tc_end(); }
