/* TC
 * id:    tm_neg_localtime64_s_args
 * area:  time
 * kind:  negative
 * funcs: _localtime64_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _localtime_s, _localtime32_s, _localtime64_s: error conditions table (EINVAL; fields set to -1)
 * note:  handler invocation for NULL/negative timer is not asserted here (see tm_neg_localtime_probe_iph_timer); beyond-max return is the implementation range check
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>

static volatile __time64_t v_t;
static void fields_m1(const struct tm *t)
{
    CHECK_EQ_I(t->tm_sec, -1); CHECK_EQ_I(t->tm_min, -1); CHECK_EQ_I(t->tm_hour, -1); CHECK_EQ_I(t->tm_mday, -1);
    CHECK_EQ_I(t->tm_mon, -1); CHECK_EQ_I(t->tm_year, -1); CHECK_EQ_I(t->tm_wday, -1); CHECK_EQ_I(t->tm_yday, -1); CHECK_EQ_I(t->tm_isdst, -1);
}

TC_CASE(null_tm) { v_t = 0; tc_iph_count = 0; CHECK_EQ_I(_localtime64_s(NULL, (const __time64_t *)&v_t), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_time) { struct tm t; t.tm_sec = 5; tc_iph_count = 0; CHECK_EQ_I(_localtime64_s(&t, NULL), EINVAL); fields_m1(&t); }
TC_CASE(negative) { struct tm t; t.tm_sec = 5; v_t = -1; tc_iph_count = 0; CHECK_EQ_I(_localtime64_s(&t, (const __time64_t *)&v_t), EINVAL); fields_m1(&t); }
TC_CASE(beyond_max) { struct tm t; v_t = 32535244800LL; tc_iph_count = 0; CHECK_EQ_I(_localtime64_s(&t, (const __time64_t *)&v_t), EINVAL); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return tc_end();
    _putenv("TZ=UTC0"); _tzset();
    TC_RUN(null_tm); TC_RUN(null_time); TC_RUN(negative); TC_RUN(beyond_max);
    _putenv("TZ=");
    return tc_end();
}
