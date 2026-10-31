/* TC
 * id:    tm_neg_localtime64_s_probe_fill
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _localtime64_s _gmtime64_s
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _localtime64_s (on error the fields of the structure are set to -1)
 * note:  PROBE: after EINVAL for LLONG_MAX / LLONG_MIN, every int field of struct tm is -1. Expected on ucrtbase; legacy CRTs may differ.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
TC_DECL_PROC(int, _localtime64_s, (struct tm *, const __time64_t *));
TC_DECL_PROC(int, _gmtime64_s, (struct tm *, const __time64_t *));
static volatile __time64_t v64;
static void allm1(const struct tm *t)
{ CHECK_EQ_I(t->tm_sec, -1); CHECK_EQ_I(t->tm_min, -1); CHECK_EQ_I(t->tm_hour, -1); CHECK_EQ_I(t->tm_mday, -1); CHECK_EQ_I(t->tm_mon, -1);
  CHECK_EQ_I(t->tm_year, -1); CHECK_EQ_I(t->tm_wday, -1); CHECK_EQ_I(t->tm_yday, -1); CHECK_EQ_I(t->tm_isdst, -1); }
TC_CASE(local_max) { struct tm tm = {0}; __time64_t t; int r; _putenv("TZ=UTC0"); _tzset(); v64 = LLONG_MAX; t = v64; tm.tm_sec = 5; tm.tm_year = 5; r = tc_p__localtime64_s(&tm, &t); CHECK_EQ_I(r, EINVAL); allm1(&tm); }
TC_CASE(local_min) { struct tm tm = {0}; __time64_t t; int r; _putenv("TZ=UTC0"); _tzset(); v64 = LLONG_MIN; t = v64; tm.tm_sec = 5; tm.tm_year = 5; r = tc_p__localtime64_s(&tm, &t); CHECK_EQ_I(r, EINVAL); allm1(&tm); }
TC_CASE(gm_max) { struct tm tm = {0}; __time64_t t; int r; v64 = LLONG_MAX; t = v64; tm.tm_sec = 5; tm.tm_year = 5; r = tc_p__gmtime64_s(&tm, &t); CHECK_EQ_I(r, EINVAL); allm1(&tm); }
int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(_localtime64_s); TC_LOAD_PROC(_gmtime64_s);
    CHECK(tc_p__localtime64_s && tc_p__gmtime64_s); if (!tc_p__localtime64_s || !tc_p__gmtime64_s) return tc_end();
    TC_RUN(local_max); TC_RUN(local_min); TC_RUN(gm_max); return tc_end();
}
