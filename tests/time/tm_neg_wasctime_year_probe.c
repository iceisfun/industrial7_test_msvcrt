/* TC
 * id:    tm_neg_wasctime_year_probe
 * area:  time
 * kind:  negative
 * funcs: _wasctime
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs asctime (year range)
 * note:  PROBE: tm_year 8100 (calendar year 10000) is expected to be rejected (NULL, errno EINVAL) because the output would need a 5-digit year; and tm_year -1901 (year -1) likewise. Exact acceptance limit is not documented precisely.
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
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL);
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
