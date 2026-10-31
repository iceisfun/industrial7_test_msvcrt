/* TC
 * id:    ct_neg_is_out_of_domain_probe
 * area:  ctype
 * funcs: toupper tolower
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   unspecified by C (UB); UCRT release build tolerates
 * note:  PROBE: on release ucrtbase, narrow toupper/tolower given values outside -1..255 return the argument unchanged without crash and without calling the invalid-parameter handler. Values used: 256, 0x1234. (is* functions are probed separately in ct_neg_is_table_out_of_domain_probe.)
 */
#include "tc.h"
#include <ctype.h>

static volatile int v;

TC_CASE(to_funcs_identity)
{
    static const int vals[] = { 256, 0x1234 }; int i;
    for (i = 0; i < 2; i++) { v = vals[i]; CHECK_EQ_I((toupper)(v), vals[i]); CHECK_EQ_I((tolower)(v), vals[i]); }
}
TC_CASE(no_iph)
{
    int n0;
    if (!tc_iph_install()) TC_SKIP("no handler");
    n0 = tc_iph_count;
    v = 256; (void)(toupper)(v); (void)(tolower)(v);
    CHECK_EQ_I(tc_iph_count, n0);
}

int main(void) { tc_begin(); TC_RUN(to_funcs_identity); TC_RUN(no_iph); return tc_end(); }
