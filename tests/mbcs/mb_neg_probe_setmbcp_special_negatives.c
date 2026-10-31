/* TC
 * id:    mb_neg_probe_setmbcp_special_negatives
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _setmbcp (silent on errno / handler)
 * note:  PROBE: (1) a failed _setmbcp(-1) sets errno == EINVAL (guess); (2) it does NOT call the invalid-parameter handler
 *        (guess); (3) _setmbcp(0) succeeds and _getmbcp() reports 0; (4) _setmbcp(_MB_CP_LOCALE) succeeds in the "C" locale;
 *        (5) INT_MIN and 65536 fail with -1 (guess: not narrowed to 16 bits).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>
#include <limits.h>
#include <locale.h>

static int old_cp;
static volatile int v_cp;

TC_CASE(fail_errno_probe) { v_cp = -1; errno = 0; CHECK_EQ_I(_setmbcp(v_cp), -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(fail_iph_probe) { v_cp = -1; tc_iph_count = 0; _setmbcp(v_cp); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(cp0_result_probe) { v_cp = 0; CHECK_EQ_I(_setmbcp(v_cp), 0); CHECK_EQ_I(_getmbcp(), 0); }
TC_CASE(locale_c_probe)
{
    setlocale(LC_ALL, "C");
    v_cp = -4;
    CHECK_EQ_I(_setmbcp(v_cp), 0);
}
TC_CASE(int_min_fails_probe) { v_cp = INT_MIN; CHECK_EQ_I(_setmbcp(v_cp), -1); }
TC_CASE(cp65536_fails_probe) { v_cp = 65536; CHECK_EQ_I(_setmbcp(v_cp), -1); }
int main(void)
{
    tc_begin(); tc_iph_install(); old_cp = _getmbcp();
    TC_RUN(fail_errno_probe); TC_RUN(fail_iph_probe); TC_RUN(cp0_result_probe);
    TC_RUN(locale_c_probe); TC_RUN(int_min_fails_probe); TC_RUN(cp65536_fails_probe);
    _setmbcp(old_cp);
    return tc_end();
}
