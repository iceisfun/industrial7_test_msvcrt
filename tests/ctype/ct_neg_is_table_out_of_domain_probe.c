/* TC
 * id:    ct_neg_is_table_out_of_domain_probe
 * area:  ctype
 * funcs: isalpha isdigit isspace
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   unspecified by C (UB); UCRT release build tolerates
 * note:  PROBE (isolated because it may index the ctype table out of bounds if the release CRT does not range-check): narrow is* functions given -2, 256, 0x1234 return 0, do not crash, and do not call the invalid-parameter handler.
 */
#include "tc.h"
#include <ctype.h>

static volatile int v;

TC_CASE(is_funcs_zero)
{
    static const int vals[] = { -2, 256, 0x1234 }; int i;
    for (i = 0; i < 3; i++) { v = vals[i]; CHECK_EQ_I((isalpha)(v), 0); CHECK_EQ_I((isdigit)(v), 0); CHECK_EQ_I((isspace)(v), 0); }
}
TC_CASE(no_iph)
{
    int n0;
    if (!tc_iph_install()) TC_SKIP("no handler");
    n0 = tc_iph_count;
    v = -2; (void)(isalpha)(v); v = 256; (void)(isalpha)(v);
    CHECK_EQ_I(tc_iph_count, n0);
}

int main(void) { tc_begin(); TC_RUN(is_funcs_zero); TC_RUN(no_iph); return tc_end(); }
