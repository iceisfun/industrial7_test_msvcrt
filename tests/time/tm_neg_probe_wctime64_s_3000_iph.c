/* TC
 * id:    tm_neg_probe_wctime64_s_3000_iph
 * area:  time
 * kind:  negative
 * funcs: _wctime64_s
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _wctime64_s (time after 3000-12-31 23:59:59 UTC is invalid)
 * note:  PROBES that the invalid-parameter handler (not just errno/EINVAL) runs for a timer just past the 3000 limit.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

static volatile __time64_t v_big = 32535216000LL;

TC_CASE(handler_runs)
{
    wchar_t buf[40]; __time64_t t = v_big; errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(_wctime64_s(buf, 40, &t), EINVAL);
    CHECK(tc_iph_count >= 1);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(handler_runs); return tc_end(); }
