/* TC
 * id:    exp_neg_gmtime64_range
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _gmtime64
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _gmtime64: NULL if time is before midnight Jan 1 1970 or after 23:59:59 Dec 31 3000 UTC
 * note:  ucrtbase omitted: newer docs/behaviour for negative times differ (see exp_neg_probe_gmtime64_ucrt).
 */
#include "tc.h"
#include <time.h>

typedef struct tm *(__cdecl *gm_t)(const __time64_t *);

TC_CASE(before_epoch)
{
    gm_t g = (gm_t)tc_crt_proc("_gmtime64");
    volatile __time64_t t = -1; __time64_t tt = t;
    CHECK(g != NULL);
    if (g) CHECK(g(&tt) == NULL);
}
TC_CASE(after_year_3000)
{
    gm_t g = (gm_t)tc_crt_proc("_gmtime64");
    volatile __time64_t t = 32535216000LL; __time64_t tt = t;   /* 3001-01-01 00:00:00 UTC */
    CHECK(g != NULL);
    if (g) CHECK(g(&tt) == NULL);
}
TC_CASE(last_valid_second)
{
    gm_t g = (gm_t)tc_crt_proc("_gmtime64");
    volatile __time64_t t = 32535215999LL; __time64_t tt = t;   /* 3000-12-31 23:59:59 UTC */
    struct tm *r;
    CHECK(g != NULL);
    if (!g) return;
    r = g(&tt);
    CHECK(r != NULL);
    if (r) { CHECK_EQ_I(r->tm_year, 1100); CHECK_EQ_I(r->tm_mon, 11); CHECK_EQ_I(r->tm_mday, 31); CHECK_EQ_I(r->tm_sec, 59); }
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(before_epoch); TC_RUN(after_year_3000); TC_RUN(last_valid_second); return tc_end(); }
