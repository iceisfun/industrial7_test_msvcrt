/* TC
 * id:    tm_neg_probe_old_crt_mktime_null
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _mktime64 asctime
 * crt:   msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _mktime64, asctime (NULL argument)
 * note:  msvcrt.dll excluded: NULL handling there is unknown and may crash. Probes NULL arguments on pre-UCRT runtimes: _mktime64 returns -1, asctime returns NULL with errno EINVAL. The invalid
 *        parameter handler is installed where the runtime supports it (msvcr100+); how often it fires is not asserted.
 *        _mktime64 is resolved dynamically and skipped if not exported.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

TC_DECL_PROC(__time64_t, _mktime64, (struct tm *));
TC_DECL_PROC(char *, asctime, (const struct tm *));

TC_CASE(mktime64)    { if (!tc_p__mktime64) TC_SKIP("not exported"); errno = 0; CHECK_EQ_I(tc_p__mktime64(NULL), -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(asctime)     { char *r; if (!tc_p_asctime) TC_SKIP("not exported"); errno = 0; r = tc_p_asctime(NULL); CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); }

int main(void)
{
    tc_begin(); if (!tc_iph_install()) { tc_write("TC SKIP " TC_TEST_ID " no invalid-parameter handler API\n"); return tc_end(); } TC_LOAD_PROC(_mktime64); TC_LOAD_PROC(asctime);
    TC_RUN(asctime); TC_RUN(mktime64);
    return tc_end();
}
