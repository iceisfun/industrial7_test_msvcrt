/* TC
 * id:    tm_neg_gmtime64_s_null_tm
 * area:  time
 * funcs:
 * procs: _gmtime64_s
 * crt:   >=90 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _gmtime64_s error table
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

TC_DECL_PROC(int, _gmtime64_s, (struct tm *, const __time64_t *));

static volatile __time64_t v_t;

TC_CASE(null_tm)
{
    int r; v_t = 0; errno = 0; tc_iph_count = 0;
    r = tc_p__gmtime64_s(NULL, (const __time64_t *)&v_t);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(both_null)
{
    int r; errno = 0; tc_iph_count = 0;
    r = tc_p__gmtime64_s(NULL, NULL);
    CHECK_EQ_I(r, EINVAL);
    CHECK(tc_iph_count >= 1);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_gmtime64_s); if (!tc_p__gmtime64_s) { tc_write("TC FAIL " TC_TEST_ID "::exported _gmtime64_s missing\n"); return 1; } CHECK(tc_iph_install()); TC_RUN(null_tm); TC_RUN(both_null); return tc_end(); }
