/* TC
 * id:    tm_neg_gmtime64_s_beyond_max
 * area:  time
 * funcs:
 * procs: _gmtime64_s
 * crt:   >=90 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _gmtime64_s
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

TC_DECL_PROC(int, _gmtime64_s, (struct tm *, const __time64_t *));

static volatile __time64_t v_t;

static void bad(__time64_t t)
{
    struct tm tm; int r; memset(&tm, 0, sizeof tm); v_t = t; errno = 0;
    r = tc_p__gmtime64_s(&tm, (const __time64_t *)&v_t);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(just_past) { bad(32535244800LL); }
TC_CASE(llong_max) { bad(LLONG_MAX); }
TC_CASE(llong_min) { bad(LLONG_MIN); }
TC_CASE(boundary_ok)
{
    struct tm tm; int r; v_t = 32535215999LL;
    r = tc_p__gmtime64_s(&tm, (const __time64_t *)&v_t);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tm.tm_year, 1100); CHECK_EQ_I(tm.tm_mon, 11); CHECK_EQ_I(tm.tm_mday, 31);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_gmtime64_s); if (!tc_p__gmtime64_s) { tc_write("TC FAIL " TC_TEST_ID "::exported _gmtime64_s missing\n"); return 1; } CHECK(tc_iph_install()); TC_RUN(just_past); TC_RUN(llong_max); TC_RUN(llong_min); TC_RUN(boundary_ok); return tc_end(); }
