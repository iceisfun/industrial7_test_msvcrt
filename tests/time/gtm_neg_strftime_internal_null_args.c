/* TC
 * id:    gtm_neg_strftime_internal_null_args
 * area:  time
 * funcs: free
 * procs: _Strftime _Gettnames
 * crt:   ucrtbase msvcr120 msvcr110
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strftime ('Parameter validation': NULL buffer/format/timeptr invoke the invalid parameter handler, errno EINVAL)
 * note:  PROBE: the internal _Strftime is assumed to share the public validation (iph count 1, errno EINVAL, no crash). The NULL
 *        lc_time_arg case is last and separate: it probes whether NULL falls back to the current locale ("Sun") or is rejected.
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
TC_CASE(null_tm)
{
    char b[32]; void *lc; if (!pg || !ps) TC_SKIP("missing");
    lc = pg(); memset(b, 'x', sizeof b); tc_iph_count = 0; errno = 0;
    (void)ps(b, sizeof b, "%Y", NULL, lc);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
    free(lc);
}
TC_CASE(null_format)
{
    char b[32]; struct tm t = mk(); void *lc; if (!pg || !ps) TC_SKIP("missing");
    lc = pg(); memset(b, 'x', sizeof b); tc_iph_count = 0; errno = 0;
    (void)ps(b, sizeof b, NULL, &t, lc);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
    free(lc);
}
TC_CASE(null_buffer)
{
    struct tm t = mk(); void *lc; if (!pg || !ps) TC_SKIP("missing");
    lc = pg(); tc_iph_count = 0; errno = 0;
    (void)ps(NULL, 32, "%Y", &t, lc);
    CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
    free(lc);
}
/* NULL lc_time may be dereferenced by an internal helper: run in a child so a crash cannot take down the parent.
   Child exit: 10 = formatted "Sun", 12 = invalid-parameter handler ran, 11 = other output. */
TC_CASE(null_lc_time_probe)
{
    DWORD code = 0; char n[24];
    int ran = tc_spawn_self("nulllc", &code, 20000);
    CHECK(ran);
    tc_i64((long long)code, n); tc_write("TC NOTE _Strftime(lc_time=NULL) child exit="); tc_write(n); tc_write("\n");
    CHECK(code == 10 || code == 12);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    if (tc_child_name()) {
        char b[32]; struct tm t = mk(); sf_t f = (sf_t)tc_crt_proc("_Strftime");
        if (!f) return 78;
        memset(b, 'x', sizeof b); b[31] = 0; tc_iph_count = 0;
        (void)f(b, sizeof b, "%a", &t, NULL);
        if (tc_iph_count > 0) return 12;
        return tc_strequ(b, "Sun") ? 10 : 11;
    }
    pg = (gett_t)tc_crt_proc("_Gettnames"); ps = (sf_t)tc_crt_proc("_Strftime");
    TC_RUN(null_tm); TC_RUN(null_format); TC_RUN(null_buffer); TC_RUN(null_lc_time_probe);
    return tc_end();
}
