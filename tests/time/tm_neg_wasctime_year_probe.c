/* TC
 * id:    tm_neg_wasctime_year_probe
 * area:  time
 * kind:  negative
 * funcs: _wasctime
 * crt:   >=90
 * tier:  p2
 * conf:  observed
 * ref:   MS docs asctime (year range)
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _wasctime accepts any tm_year >= 0 (tm_year 8100 = calendar year 10000 returns non-NULL, errno 0; years up to 100000 also accepted in probes); negative tm_year (-1901) returns NULL with EINVAL. The year-10000 text is Windows' garbled 4-char year field (":000"), asserted as observed; upper bound beyond tm_year 100000 not probed.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

static struct tm good = { 0, 0, 0, 11, 0, 70, 0, 10, 0 };
static volatile int yr;

TC_CASE(year_8100)
{
    struct tm t = good; wchar_t *r;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler");
    yr = 8100; t.tm_year = yr; tc_iph_count = 0; errno = 0;
    r = _wasctime(&t);
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    CHECK(r != NULL); CHECK_EQ_I(errno, 0);   /* observed: no upper year limit on asctime/_wasctime */
    if (r) CHECK_WSTR(r, L"Sun Jan 11 00:00:00 :000\n");   /* observed: 4-digit year field is filled arithmetically, 10000 -> ":000" */
#else
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL);
#endif
}
TC_CASE(year_negative)
{
    struct tm t = good; wchar_t *r;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler");
    yr = -1901; t.tm_year = yr; tc_iph_count = 0; errno = 0;
    r = _wasctime(&t);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL);
}
int main(void) { tc_begin(); TC_RUN(year_8100); TC_RUN(year_negative); return tc_end(); }
