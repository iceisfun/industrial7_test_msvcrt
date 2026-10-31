/* TC
 * id:    tm_neg_wasctime_null
 * area:  time
 * kind:  negative
 * funcs: _wasctime
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * note:  Single-digit days are avoided here because day padding differs between CRT generations; see tm_ctime_probe_day_padding.
 * ref:   MS docs asctime, _wasctime (NULL or out-of-range tm: returns NULL, errno EINVAL, invalid parameter handler)
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

static struct tm good = { 0, 0, 0, 11, 0, 70, 0, 10, 0 };
static volatile int sink;

TC_CASE(null_tm)
{
    const struct tm *volatile p = NULL; wchar_t *r;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler");
    tc_iph_count = 0; errno = 0;
    r = _wasctime((const struct tm *)p);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(bad_month)
{
    struct tm t = good; wchar_t *r;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler");
    sink = 12; t.tm_mon = sink; tc_iph_count = 0; errno = 0;
    r = _wasctime(&t);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(valid_still_works)
{
    wchar_t *r; struct tm t = good;
    r = _wasctime(&t);
    CHECK(r != NULL);
    if (r) CHECK_WSTR(r, L"Sun Jan 11 00:00:00 1970\n");
}
int main(void) { tc_begin(); TC_RUN(null_tm); TC_RUN(bad_month); TC_RUN(valid_still_works); return tc_end(); }
