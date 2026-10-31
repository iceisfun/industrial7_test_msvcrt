/* TC
 * id:    tm_neg_gmtime64_s_null_time
 * area:  time
 * funcs:
 * procs: _gmtime64_s
 * crt:   >=90 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _gmtime64_s error table
 * note:  -1 field fill and handler-on-range-error are probed in tm_neg_probe_gmtime_s_error_effects
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

TC_DECL_PROC(int, _gmtime64_s, (struct tm *, const __time64_t *));

static volatile __time64_t v_t;

TC_CASE(null_time)
{
    struct tm tm; int r; memset(&tm, 0, sizeof tm); errno = 0; tc_iph_count = 0;
    r = tc_p__gmtime64_s(&tm, NULL);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(negative_time)
{
    struct tm tm; int r; memset(&tm, 0, sizeof tm); v_t = -1; errno = 0;
    r = tc_p__gmtime64_s(&tm, (const __time64_t *)&v_t);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_gmtime64_s); if (!tc_p__gmtime64_s) { tc_write("TC FAIL " TC_TEST_ID "::exported _gmtime64_s missing\n"); return 1; } CHECK(tc_iph_install()); TC_RUN(null_time); TC_RUN(negative_time); return tc_end(); }
