/* TC
 * id:    gtm_wcsftime_internal_probe
 * area:  time
 * funcs: free
 * procs: _Wcsftime _Gettnames
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   undocumented yvals/xtime helper _Wcsftime; format semantics as C11 7.29.5.1 wcsftime
 * note:  PROBE: _Wcsftime(wchar_t*, size_t, const wchar_t*, const struct tm*, void *lc_time) with lc_time from _Gettnames(). Return value not asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

typedef void *(__cdecl *gett_t)(void);
typedef size_t (__cdecl *wf_t)(wchar_t *, size_t, const wchar_t *, const struct tm *, void *);
static gett_t pg; static wf_t pw;

static struct tm mk(void)
{
    struct tm t; memset(&t, 0, sizeof t);
    t.tm_year = 100; t.tm_mday = 2; t.tm_yday = 1; t.tm_hour = 3; t.tm_min = 4; t.tm_sec = 5;
    return t;
}
TC_CASE(date)
{
    wchar_t b[64]; struct tm t = mk(); void *lc; if (!pg || !pw) TC_SKIP("missing");
    lc = pg(); wmemset(b, L'x', 64);
    (void)pw(b, 64, L"%Y-%m-%d", &t, lc);
    CHECK_WSTR(b, L"2000-01-02"); free(lc);
}
TC_CASE(names)
{
    wchar_t b[64]; struct tm t = mk(); void *lc; if (!pg || !pw) TC_SKIP("missing");
    lc = pg(); wmemset(b, L'x', 64);
    (void)pw(b, 64, L"%a %b", &t, lc);
    CHECK_WSTR(b, L"Sun Jan"); free(lc);
}
TC_CASE(am_pm)
{
    wchar_t b[64]; struct tm t = mk(); void *lc; if (!pg || !pw) TC_SKIP("missing");
    lc = pg(); wmemset(b, L'x', 64);
    (void)pw(b, 64, L"%p", &t, lc);
    CHECK_WSTR(b, L"AM"); free(lc);
}
int main(void)
{
    tc_begin();
    pg = (gett_t)tc_crt_proc("_Gettnames"); pw = (wf_t)tc_crt_proc("_Wcsftime");
    TC_RUN(date); TC_RUN(names); TC_RUN(am_pm);
    return tc_end();
}
