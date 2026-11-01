/* TC
 * id:    gtm_neg_strftime_internal_bad_tm_probe
 * area:  time
 * funcs: free
 * procs: _Strftime _Gettnames
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs strftime (invalid tm fields / unknown format specifier invoke the invalid parameter handler, errno EINVAL); C11 7.27.3.5
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _Strftime invokes the invalid-parameter handler TWICE for an out-of-range tm_mon/tm_wday/tm_hour (errno EINVAL) and once for an unknown specifier (%Q); only 'handler called >=1 times and errno EINVAL' is shared with the public strftime. The trailing lone '%' case only checks that the call returns, the buffer stays within bounds and no more than one handler call happens.
 *        (tm_mon 0..11, tm_wday 0..6, tm_hour 0..23, known specifiers). Mirrors tm_neg_strftime_invalid_tm. The trailing lone '%'
 *        case only checks that the call returns, the buffer stays within bounds and no more than one handler call happens.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

typedef void *(__cdecl *gett_t)(void);
typedef uintptr_t (__cdecl *sf_t)(char *, size_t, const char *, const struct tm *, void *);
static gett_t pg; static sf_t ps;

static struct tm mk(void)
{
    struct tm t; memset(&t, 0, sizeof t);
    t.tm_year = 100; t.tm_mday = 2; t.tm_yday = 1; t.tm_hour = 3; t.tm_min = 4; t.tm_sec = 5;
    return t;
}
static void run(struct tm *t, const char *fmt, int want_iph)
{
    char b[64]; void *lc; if (!pg || !ps) TC_SKIP("missing");
    memset(b, 'G', sizeof b); lc = pg(); tc_iph_count = 0; errno = 0;
    (void)ps(b, 32, fmt, t, lc);
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
    CHECK_EQ_I(tc_iph_count, want_iph);   /* observed: 2 handler calls for a bad tm field, 1 for an unknown specifier */
#else
    CHECK_EQ_I(tc_iph_count, 1);
#endif
    CHECK_EQ_I(errno, EINVAL);
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
#else
    (void)want_iph;
#endif
    { size_t i; for (i = 32; i < sizeof b; i++) if (b[i] != 'G') { CHECK(0); break; } }
    free(lc);
}
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
TC_CASE(mon12) { struct tm t = mk(); volatile int v = 12; t.tm_mon = v; run(&t, "%b", 2); }
TC_CASE(wday7) { struct tm t = mk(); volatile int v = 7; t.tm_wday = v; run(&t, "%a", 2); }
TC_CASE(hour24) { struct tm t = mk(); volatile int v = 24; t.tm_hour = v; run(&t, "%H", 2); }
#else
TC_CASE(mon12) { struct tm t = mk(); volatile int v = 12; t.tm_mon = v; run(&t, "%b", 1); }
TC_CASE(wday7) { struct tm t = mk(); volatile int v = 7; t.tm_wday = v; run(&t, "%a", 1); }
TC_CASE(hour24) { struct tm t = mk(); volatile int v = 24; t.tm_hour = v; run(&t, "%H", 1); }
#endif
TC_CASE(unknown_Q) { struct tm t = mk(); run(&t, "%Q", 1); }
TC_CASE(trailing_percent)
{
    char b[64]; struct tm t = mk(); void *lc; if (!pg || !ps) TC_SKIP("missing");
    memset(b, 'G', sizeof b); lc = pg(); tc_iph_count = 0;
    (void)ps(b, 32, "ab%", &t, lc);
    CHECK(tc_iph_count <= 1);
    { size_t i; for (i = 32; i < sizeof b; i++) if (b[i] != 'G') { CHECK(0); break; } }
    free(lc);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    pg = (gett_t)tc_crt_proc("_Gettnames"); ps = (sf_t)tc_crt_proc("_Strftime");
    TC_RUN(mon12); TC_RUN(wday7); TC_RUN(hour24); TC_RUN(unknown_Q); TC_RUN(trailing_percent);
    return tc_end();
}
