/* TC
 * id:    tm_gmtime32_s_basic
 * area:  time
 * funcs:
 * procs: _gmtime32_s
 * crt:   >=90 msvcrt
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _gmtime32_s, _gmtime64_s
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

TC_DECL_PROC(int, _gmtime32_s, (struct tm *, const __time32_t *));

static volatile __time32_t v_t;

TC_CASE(epoch)
{
    struct tm tm; int r;
    v_t = 0; r = tc_p__gmtime32_s(&tm, (const __time32_t *)&v_t);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(tm.tm_year, 70); CHECK_EQ_I(tm.tm_mon, 0); CHECK_EQ_I(tm.tm_mday, 1);
    CHECK_EQ_I(tm.tm_hour, 0); CHECK_EQ_I(tm.tm_min, 0); CHECK_EQ_I(tm.tm_sec, 0);
    CHECK_EQ_I(tm.tm_wday, 4); CHECK_EQ_I(tm.tm_yday, 0); CHECK_EQ_I(tm.tm_isdst, 0);
}
TC_CASE(int_max)
{
    struct tm tm; int r;
    v_t = 0x7fffffff; r = tc_p__gmtime32_s(&tm, (const __time32_t *)&v_t);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(tm.tm_year, 138); CHECK_EQ_I(tm.tm_mon, 0); CHECK_EQ_I(tm.tm_mday, 19);
    CHECK_EQ_I(tm.tm_hour, 3); CHECK_EQ_I(tm.tm_min, 14); CHECK_EQ_I(tm.tm_sec, 7);
    CHECK_EQ_I(tm.tm_wday, 2); CHECK_EQ_I(tm.tm_yday, 18); CHECK_EQ_I(tm.tm_isdst, 0);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_gmtime32_s); if (!tc_p__gmtime32_s) { tc_write("TC FAIL " TC_TEST_ID "::exported _gmtime32_s missing\n"); return 1; } TC_RUN(epoch); TC_RUN(int_max); return tc_end(); }
