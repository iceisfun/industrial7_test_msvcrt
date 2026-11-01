/* TC
 * id:    tm_neg_ctime64_args
 * area:  time
 * kind:  negative
 * funcs: _ctime64
 * crt:   >=90
 * tier:  p0
 * conf:  observed
 * note:  errno for the out-of-range cases is not documented; see tm_neg_ctime_probe_range_errno.
 * ref:   MS docs ctime, _ctime64: NULL if time is before midnight Jan 1 1970 UTC or after 23:59:59 Dec 31 3000 UTC; NULL timer is an invalid parameter (EINVAL)
 *        beyond_max uses 32536800000 (first rejected value on ucrtbase), 32535244800 is accepted there (Thu Jan  1 08:00:00 3001).
 */
#include "tc.h"

/* Observed limits of the real Windows 10 ucrtbase (build 19045): _MAX__TIME64_T = 32536799999 (3001-01-19 07:59:59 UTC) for local-time style
 * conversions (_localtime64, _ctime64*, _wctime64*, _mktime64, _mkgmtime64, _futime64); _gmtime64 accepts [-43200, 32536850399].
 * The MS docs say 3000-12-31 23:59:59 UTC / 1970-01-01; Windows is more permissive. Older CRTs keep the documented-era values (unverified). */
#if TC_CRT_VER >= 140
#define T64_LOCAL_BAD 32536800000LL
#define T64_GM_BAD    32536850400LL
#define T64_GM_NEG_BAD (-43201LL)
#else
#define T64_LOCAL_BAD 32535244800LL
#define T64_GM_BAD    32535244800LL
#define T64_GM_NEG_BAD (-1LL)
#endif
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>

static volatile __time64_t vt;

TC_CASE(null_timer)
{
    const __time64_t *volatile p = NULL; char *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _putenv("TZ=UTC0"); _tzset(); tc_iph_count = 0; errno = 0;
    r = _ctime64((const __time64_t *)p);
    CHECK(r == NULL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(minus_one)
{
    __time64_t t; char *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _putenv("TZ=UTC0"); _tzset(); tc_iph_count = 0; errno = 0;
    vt = -1; t = vt; r = _ctime64(&t);
    CHECK(r == NULL);
}
TC_CASE(beyond_max)
{
    __time64_t t; char *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _putenv("TZ=UTC0"); _tzset(); tc_iph_count = 0; errno = 0;
    vt = T64_LOCAL_BAD; t = vt; r = _ctime64(&t);
    CHECK(r == NULL);
}
TC_CASE(llong_min)
{
    __time64_t t; char *r;
    if (!tc_iph_install()) TC_SKIP("no iph");
    _putenv("TZ=UTC0"); _tzset(); tc_iph_count = 0; errno = 0;
    vt = LLONG_MIN; t = vt; r = _ctime64(&t);
    CHECK(r == NULL);
}
int main(void) { tc_begin(); TC_RUN(null_timer); TC_RUN(minus_one); TC_RUN(beyond_max); TC_RUN(llong_min); return tc_end(); }
