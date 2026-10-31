/* TC
 * id:    tm_neg_localtime_probe_iph_timer
 * area:  time
 * kind:  negative
 * funcs: _localtime64_s
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _localtime_s (parameter validation); not stated precisely
 * note:  PROBES whether the invalid-parameter handler is invoked for a NULL / negative timer in _localtime64_s.
 *        Most likely: NULL timer invokes it once; a negative timer returns EINVAL WITHOUT invoking it.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

static volatile __time64_t v_t;

TC_CASE(null_timer_iph) { struct tm t; tc_iph_count = 0; CHECK_EQ_I(_localtime64_s(&t, NULL), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(negative_no_iph) { struct tm t; v_t = -1; tc_iph_count = 0; CHECK_EQ_I(_localtime64_s(&t, (const __time64_t *)&v_t), EINVAL); CHECK_EQ_I(tc_iph_count, 0); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return tc_end();
    TC_RUN(null_timer_iph); TC_RUN(negative_no_iph);
    return tc_end();
}
