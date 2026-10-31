/* TC
 * id:    gtm_neg_wcsftime_internal_small_buffer
 * area:  time
 * funcs: free
 * procs: _Wcsftime _Gettnames
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.29.5.1 wcsftime (no more than maxsize wide characters written); MS docs wcsftime (NULL args -> invalid parameter, EINVAL)
 * note:  PROBE: internal _Wcsftime(wchar_t*, size, fmt, tm, lc_time) assumed to share public wcsftime behaviour: never writes past size,
 *        and NULL tm / NULL format invoke the invalid-parameter handler with errno EINVAL. Size 0 may or may not call the handler (not asserted).
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <errno.h>

typedef void *(__cdecl *gett_t)(void);
typedef uintptr_t (__cdecl *wf_t)(wchar_t *, size_t, const wchar_t *, const struct tm *, void *);
static gett_t pg; static wf_t pw;

static struct tm mk(void)
{
    struct tm t; memset(&t, 0, sizeof t);
    t.tm_year = 100; t.tm_mday = 2; t.tm_yday = 1; t.tm_hour = 3; t.tm_min = 4; t.tm_sec = 5;
    return t;
}
static int guard_ok(const wchar_t *b, size_t from, size_t to)
{ size_t i; for (i = from; i < to; i++) if (b[i] != L'G') return 0; return 1; }

TC_CASE(size4_no_overrun)
{
    wchar_t b[32]; struct tm t = mk(); void *lc; if (!pg || !pw) TC_SKIP("missing");
    wmemset(b, L'G', 32); lc = pg();
    (void)pw(b, 4, L"%Y-%m-%d", &t, lc);
    CHECK(guard_ok(b, 4, 32));
    free(lc);
}
TC_CASE(size0_no_write)
{
    wchar_t b[16]; struct tm t = mk(); void *lc; if (!pg || !pw) TC_SKIP("missing");
    wmemset(b, L'G', 16); lc = pg();
    (void)pw(b, 0, L"%Y-%m-%d", &t, lc);
    CHECK(guard_ok(b, 0, 16));
    free(lc);
}
TC_CASE(null_tm)
{
    wchar_t b[16]; void *lc; if (!pg || !pw) TC_SKIP("missing");
    wmemset(b, L'G', 16); lc = pg(); tc_iph_count = 0; errno = 0;
    (void)pw(b, 16, L"%Y", NULL, lc);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
    free(lc);
}
TC_CASE(null_format)
{
    wchar_t b[16]; struct tm t = mk(); void *lc; if (!pg || !pw) TC_SKIP("missing");
    wmemset(b, L'G', 16); lc = pg(); tc_iph_count = 0; errno = 0;
    (void)pw(b, 16, NULL, &t, lc);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
    free(lc);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    pg = (gett_t)tc_crt_proc("_Gettnames"); pw = (wf_t)tc_crt_proc("_Wcsftime");
    TC_RUN(size4_no_overrun); TC_RUN(size0_no_write); TC_RUN(null_tm); TC_RUN(null_format);
    return tc_end();
}
