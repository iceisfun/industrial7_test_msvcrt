/* TC
 * id:    loc_neg_probe_configthreadlocale_flag_combos
 * area:  locale
 * funcs: _configthreadlocale setlocale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _configthreadlocale
 * note:  PROBE: bit combinations that are not one of 0/1/2 (3 = ENABLE|DISABLE, INT_MAX) are believed rejected with -1 and leave the
 *        mode unchanged. The header also defines _GLOBAL/_NEW modifier values (0x10..0x200) so exact masking is not documented.
 */
#include "tc.h"
#include <locale.h>
#include <limits.h>

static volatile int v_arg;
TC_CASE(both_flags) { int b = _configthreadlocale(0); v_arg = 3; CHECK_EQ_I(_configthreadlocale(v_arg), -1); CHECK_EQ_I(_configthreadlocale(0), b); }
TC_CASE(int_max) { int b = _configthreadlocale(0); v_arg = INT_MAX; CHECK_EQ_I(_configthreadlocale(v_arg), -1); CHECK_EQ_I(_configthreadlocale(0), b); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(both_flags); TC_RUN(int_max);
    setlocale(LC_ALL, "C");
    return tc_end();
}
