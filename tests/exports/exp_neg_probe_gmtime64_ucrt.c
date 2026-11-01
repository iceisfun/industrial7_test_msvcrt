/* TC
 * id:    exp_neg_probe_gmtime64_ucrt
 * area:  exports
 * kind:  negative
 * funcs: 
 * procs: _gmtime64
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _gmtime64
 * note:  Win10 22H2 ucrtbase (observed): _gmtime64(-1) succeeds (1969-12-31 23:59:59); a time 100 years before the epoch returns NULL.
 *        The old probe expected NULL for -1 (as for earlier CRTs).
 */
#include "tc.h"
#include <time.h>

typedef struct tm *(__cdecl *gm_t)(const __time64_t *);

TC_CASE(minus_one)
{
    gm_t g = (gm_t)tc_crt_proc("_gmtime64");
    volatile __time64_t t = -1; __time64_t tt = t;
    struct tm *r;
    CHECK(g != NULL);
    if (!g) return;
    r = g(&tt);
    CHECK(r != NULL);   /* observed on Win10 22H2: UCRT _gmtime64 accepts -1 (earlier CRTs rejected negative times) */
    if (r) { CHECK_EQ_I(r->tm_year, 69); CHECK_EQ_I(r->tm_mon, 11); CHECK_EQ_I(r->tm_mday, 31); CHECK_EQ_I(r->tm_hour, 23); CHECK_EQ_I(r->tm_min, 59); CHECK_EQ_I(r->tm_sec, 59); }
}
TC_CASE(century_before_epoch_null)
{
    gm_t g = (gm_t)tc_crt_proc("_gmtime64");
    volatile __time64_t t = -86400LL * 365 * 100; __time64_t tt = t;
    CHECK(g != NULL);
    if (g) CHECK(g(&tt) == NULL);   /* observed: far-negative times are still rejected */
}
int main(void) { tc_begin(); TC_RUN(minus_one); TC_RUN(century_before_epoch_null); return tc_end(); }
