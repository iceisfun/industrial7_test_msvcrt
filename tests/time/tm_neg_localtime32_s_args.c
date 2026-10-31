/* TC
 * id:    tm_neg_localtime32_s_args
 * area:  time
 * kind:  negative
 * funcs: _localtime32_s
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _localtime32_s: error conditions table
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>

static volatile __time32_t v_t;

TC_CASE(null_tm) { v_t = 0; CHECK_EQ_I(_localtime32_s(NULL, (const __time32_t *)&v_t), EINVAL); }
TC_CASE(null_time) { struct tm t; CHECK_EQ_I(_localtime32_s(&t, NULL), EINVAL); CHECK_EQ_I(t.tm_year, -1); CHECK_EQ_I(t.tm_mday, -1); }
TC_CASE(negative) { struct tm t; v_t = -1; CHECK_EQ_I(_localtime32_s(&t, (const __time32_t *)&v_t), EINVAL); CHECK_EQ_I(t.tm_year, -1); CHECK_EQ_I(t.tm_hour, -1); CHECK_EQ_I(t.tm_isdst, -1); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return tc_end();
    _putenv("TZ=UTC0"); _tzset();
    TC_RUN(null_tm); TC_RUN(null_time); TC_RUN(negative);
    _putenv("TZ=");
    return tc_end();
}
