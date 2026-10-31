/* TC
 * id:    loc_neg_configthreadlocale_bad_arg
 * area:  locale
 * funcs: _configthreadlocale
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _configthreadlocale: returns -1 and sets errno to EINVAL for an invalid argument (parameter validation)
 * note:  Arguments 3, -5, 0x1000 are none of 0, 1, 2 (the only valid values). The docs only state "-1 on failure"; the EINVAL + invalid-parameter handler behaviour is known from the CRT parameter validation, not spelled out on the doc page.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>

static volatile int v_arg;
static int before;

TC_CASE(value_3)
{
    before = _configthreadlocale(0);
    v_arg = 3; CHECK_EQ_I(_configthreadlocale(v_arg), -1);
}
TC_CASE(value_negative) { v_arg = -5; CHECK_EQ_I(_configthreadlocale(v_arg), -1); }
TC_CASE(value_large) { v_arg = 0x1000; CHECK_EQ_I(_configthreadlocale(v_arg), -1); }
TC_CASE(errno_einval)
{
    errno = 0; v_arg = 3; CHECK_EQ_I(_configthreadlocale(v_arg), -1); CHECK_EQ_I(errno, EINVAL);
    errno = 0; v_arg = -5; CHECK_EQ_I(_configthreadlocale(v_arg), -1); CHECK_EQ_I(errno, EINVAL);
    errno = 0; v_arg = 0x1000; CHECK_EQ_I(_configthreadlocale(v_arg), -1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(iph_count)
{
    if (!tc_iph_install()) TC_SKIP("no IPH api");
    tc_iph_count = 0;
    v_arg = 3; _configthreadlocale(v_arg);
    CHECK_EQ_I(tc_iph_count, 1);
    v_arg = -5; _configthreadlocale(v_arg);
    CHECK_EQ_I(tc_iph_count, 2);
    v_arg = 0x1000; _configthreadlocale(v_arg);
    CHECK_EQ_I(tc_iph_count, 3);
}
TC_CASE(mode_unchanged) { CHECK_EQ_I(_configthreadlocale(0), before); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(value_3); TC_RUN(value_negative); TC_RUN(value_large); TC_RUN(errno_einval); TC_RUN(iph_count); TC_RUN(mode_unchanged);
    return tc_end();
}
