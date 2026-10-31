/* TC
 * id:    ct_neg_probe_isascii_family_iph
 * area:  ctype
 * kind:  negative
 * funcs:
 * procs: __isascii iswascii __toascii
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs isascii, __isascii, iswascii, __toascii
 * note:  PROBES that these pure bit tests accept any int (INT_MIN/INT_MAX/-1) and never call the
 *        invalid-parameter handler. Results: __isascii = (c & ~0x7F)==0, __toascii = c & 0x7F.
 */
#include "tc.h"
#include <limits.h>

typedef int (__cdecl *i_i)(int);
static i_i p_isascii, p_iswascii, p_toascii;
static volatile int vc;

TC_CASE(isascii_extremes)
{
    if (!p_isascii) TC_SKIP("missing");
    vc = INT_MAX; CHECK_EQ_I(p_isascii(vc), 0);
    vc = INT_MIN; CHECK_EQ_I(p_isascii(vc), 0);
    vc = -1;      CHECK_EQ_I(p_isascii(vc), 0);
}
TC_CASE(iswascii_extremes)
{
    if (!p_iswascii) TC_SKIP("missing");
    vc = 0xFFFF; CHECK_EQ_I(p_iswascii(vc), 0);
    vc = 0;      CHECK(p_iswascii(vc) != 0);
}
TC_CASE(toascii_extremes)
{
    if (!p_toascii) TC_SKIP("missing");
    vc = INT_MIN; CHECK_EQ_I(p_toascii(vc), 0);
    vc = INT_MAX; CHECK_EQ_I(p_toascii(vc), 0x7F);
}
TC_CASE(no_iph)
{
    tc_iph_count = 0;
    if (p_isascii) { vc = INT_MAX; (void)p_isascii(vc); vc = INT_MIN; (void)p_isascii(vc); }
    if (p_iswascii) { vc = 0xFFFF; (void)p_iswascii(vc); }
    if (p_toascii) { vc = INT_MIN; (void)p_toascii(vc); }
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    p_isascii = (i_i)tc_crt_proc("__isascii"); p_iswascii = (i_i)tc_crt_proc("iswascii"); p_toascii = (i_i)tc_crt_proc("__toascii");
    tc_iph_install();
    TC_RUN(isascii_extremes); TC_RUN(iswascii_extremes); TC_RUN(toascii_extremes); TC_RUN(no_iph);
    return tc_end();
}
