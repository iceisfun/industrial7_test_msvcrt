/* TC
 * id:    loc_neg_configthreadlocale_minus_one_and_flags
 * area:  locale
 * funcs: _configthreadlocale setlocale
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _configthreadlocale (valid: 0, _ENABLE_PER_THREAD_LOCALE=1, _DISABLE_PER_THREAD_LOCALE=2; returns -1 on failure)
 * note:  Values -1 and INT_MIN are none of the valid values. 3 and INT_MAX moved to loc_neg_probe_configthreadlocale_flag_combos. EINVAL + invalid-parameter handler is from CRT parameter validation, not spelled out on the doc page.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
#include <limits.h>

static volatile int v_arg;
static int before;

TC_CASE(minus_one)
{
    before = _configthreadlocale(0);
    errno = 0; tc_iph_count = 0; v_arg = -1;
    CHECK_EQ_I(_configthreadlocale(v_arg), -1);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(_configthreadlocale(0), before);
    CHECK(setlocale(LC_ALL, "C") != NULL);
}
TC_CASE(int_min) { v_arg = INT_MIN; CHECK_EQ_I(_configthreadlocale(v_arg), -1); CHECK_EQ_I(_configthreadlocale(0), before); CHECK(setlocale(LC_ALL, "C") != NULL); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(minus_one); TC_RUN(int_min); 
    return tc_end();
}
