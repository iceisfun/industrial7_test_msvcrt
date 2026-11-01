/* TC
 * id:    tm_neg_gmtime64_beyond_max
 * area:  time
 * funcs: _gmtime64
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _gmtime64: valid through 23:59:59 Dec 31 3000 UTC
 * note:  just_past uses 32536850400, the first value _gmtime64 rejects on ucrtbase (accepted up to 32536850399).
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
#include <limits.h>
#include <stdlib.h>

static volatile __time64_t v_t;

static void bad(__time64_t t)
{
    struct tm *r; v_t = t; errno = 0;
    r = _gmtime64((const __time64_t *)&v_t);
    CHECK(r == NULL);
#if TC_CRT_VER >= 90
    CHECK_EQ_I(errno, EINVAL);
#endif
}
TC_CASE(just_past) { bad(T64_GM_BAD); }
TC_CASE(llong_max) { bad(LLONG_MAX); }
TC_CASE(boundary_valid)
{
    struct tm *r; v_t = 32535215999LL; r = _gmtime64((const __time64_t *)&v_t);
    CHECK(r != NULL);
    if (r) {
        CHECK_EQ_I(r->tm_year, 1100); CHECK_EQ_I(r->tm_mon, 11); CHECK_EQ_I(r->tm_mday, 31);
        CHECK_EQ_I(r->tm_hour, 23); CHECK_EQ_I(r->tm_min, 59); CHECK_EQ_I(r->tm_sec, 59);
        CHECK_EQ_I(r->tm_yday, 364);
    }
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(just_past); TC_RUN(llong_max); TC_RUN(boundary_valid); return tc_end(); }
