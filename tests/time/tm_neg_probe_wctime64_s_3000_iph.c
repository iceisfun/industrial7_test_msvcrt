/* TC
 * id:    tm_neg_probe_wctime64_s_3000_iph
 * area:  time
 * kind:  negative
 * funcs: _wctime64_s
 * crt:   >=90
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _wctime64_s (time after 3000-12-31 23:59:59 UTC is invalid)
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _wctime64_s limit is 32536799999 (3001-01-19), not 3000-12-31; 32536800000 returns EINVAL and does NOT invoke the invalid-parameter handler (id is historical).
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
static volatile __time64_t v_big = 32536800000LL;
#else
static volatile __time64_t v_big = 32535216000LL;
#endif

TC_CASE(handler_runs)
{
    wchar_t buf[40]; __time64_t t = v_big; errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(_wctime64_s(buf, 40, &t), EINVAL);
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    CHECK_EQ_I(tc_iph_count, 0);   /* observed: range failure does not call the handler */
#else
    CHECK(tc_iph_count >= 1);
#endif
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(handler_runs); return tc_end(); }
