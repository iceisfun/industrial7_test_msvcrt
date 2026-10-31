/* TC
 * id:    ct_neg_probe_isleadbyte_l_null_locale
 * area:  ctype
 * kind:  negative
 * funcs:
 * procs: _isleadbyte_l
 * crt:   >=100
 * tier:  p0
 * conf:  probe
 * ref:   MS docs isleadbyte, _isleadbyte_l
 * note:  PROBES that _isleadbyte_l(c, NULL) uses the current ("C", single-byte) locale: 0 for all bytes, no crash,
 *        no invalid-parameter call.
 */
#include "tc.h"

TC_DECL_PROC(int, _isleadbyte_l, (int, void *));
static volatile int vc;

TC_CASE(sample_bytes_not_lead)
{
    if (!tc_p__isleadbyte_l) TC_SKIP("missing");
    vc = 0x81; CHECK_EQ_I(tc_p__isleadbyte_l(vc, NULL), 0);
    vc = 0xE0; CHECK_EQ_I(tc_p__isleadbyte_l(vc, NULL), 0);
    vc = 0xFF; CHECK_EQ_I(tc_p__isleadbyte_l(vc, NULL), 0);
}
TC_CASE(all_bytes_no_iph)
{
    int c;
    if (!tc_p__isleadbyte_l) TC_SKIP("missing");
    tc_iph_count = 0;
    for (c = 0; c < 256; c++) { vc = c; if (tc_p__isleadbyte_l(vc, NULL)) { CHECK_EQ_I(c, -1); break; } }
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_isleadbyte_l); tc_iph_install();
    TC_RUN(sample_bytes_not_lead); TC_RUN(all_bytes_no_iph);
    return tc_end();
}
