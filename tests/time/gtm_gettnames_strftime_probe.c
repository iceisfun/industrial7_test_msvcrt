/* TC
 * id:    gtm_gettnames_strftime_probe
 * area:  time
 * funcs: free
 * procs: _Gettnames _Strftime
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   undocumented yvals/xtime helpers; format semantics as C11 7.27.3.5 strftime
 * note:  PROBE: _Strftime(char *buf, size_t n, const char *fmt, const struct tm *tm, void *lc_time) with lc_time from _Gettnames().
 *        Its return value convention (length vs buffer pointer) is not asserted; only the buffer content is.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef void *(__cdecl *gett_t)(void);
typedef size_t (__cdecl *sf_t)(char *, size_t, const char *, const struct tm *, void *);
static gett_t pg; static sf_t ps;

static struct tm mk(void)
{
    struct tm t; memset(&t, 0, sizeof t);
    t.tm_year = 100; t.tm_mon = 0; t.tm_mday = 2; t.tm_wday = 0; t.tm_yday = 1; t.tm_hour = 3; t.tm_min = 4; t.tm_sec = 5;
    return t;
}
TC_CASE(gettnames_nonnull)
{
    void *p; if (!pg) TC_SKIP("no _Gettnames");
    p = pg(); CHECK(p != NULL); free(p);
}
TC_CASE(names)
{
    char b[64]; struct tm t = mk(); void *lc; if (!pg || !ps) TC_SKIP("missing");
    lc = pg(); memset(b, 'x', sizeof b);
    (void)ps(b, sizeof b, "%a %A %b %B", &t, lc);
    CHECK_STR(b, "Sun Sunday Jan January"); free(lc);
}
TC_CASE(numeric)
{
    char b[64]; struct tm t = mk(); void *lc; if (!pg || !ps) TC_SKIP("missing");
    lc = pg(); memset(b, 'x', sizeof b);
    (void)ps(b, sizeof b, "%Y-%m-%d %H:%M:%S", &t, lc);
    CHECK_STR(b, "2000-01-02 03:04:05"); free(lc);
}
TC_CASE(percent_literal)
{
    char b[64]; struct tm t = mk(); void *lc; if (!pg || !ps) TC_SKIP("missing");
    lc = pg(); memset(b, 'x', sizeof b);
    (void)ps(b, sizeof b, "100%%", &t, lc);
    CHECK_STR(b, "100%"); free(lc);
}
int main(void)
{
    tc_begin();
    pg = (gett_t)tc_crt_proc("_Gettnames"); ps = (sf_t)tc_crt_proc("_Strftime");
    TC_RUN(gettnames_nonnull); TC_RUN(names); TC_RUN(numeric); TC_RUN(percent_literal);
    return tc_end();
}
