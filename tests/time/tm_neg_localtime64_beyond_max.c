/* TC
 * id:    tm_neg_localtime64_beyond_max
 * area:  time
 * kind:  negative
 * funcs: _localtime64
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _localtime64: range ends at 23:59:59 Dec 31 3000 UTC
 * note:  ucrtbase accepts _localtime64 up to 32536799999 (TZ=UTC0); just_beyond is 32536800000 there.
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

static volatile __time64_t v_t;

TC_CASE(just_beyond) { struct tm *p; v_t = T64_LOCAL_BAD; p = _localtime64((const __time64_t *)&v_t); CHECK(p == NULL); }
TC_CASE(llong_max) { struct tm *p; v_t = LLONG_MAX; p = _localtime64((const __time64_t *)&v_t); CHECK(p == NULL); }
#if TC_CRT_VER >= 140
TC_CASE(observed_last_valid) { struct tm *p; v_t = 32536799999LL; /* 3001-01-19 07:59:59 UTC */
    p = _localtime64((const __time64_t *)&v_t); CHECK(p != NULL);
    if (p) { CHECK_EQ_I(p->tm_year, 1101); CHECK_EQ_I(p->tm_mon, 0); CHECK_EQ_I(p->tm_mday, 19); CHECK_EQ_I(p->tm_hour, 7); CHECK_EQ_I(p->tm_sec, 59); } }
#endif
TC_CASE(last_valid) { struct tm *p; v_t = 32535215999LL; /* 3000-12-31 23:59:59 UTC */
    p = _localtime64((const __time64_t *)&v_t); CHECK(p != NULL);
    if (p) { CHECK_EQ_I(p->tm_year, 1100); CHECK_EQ_I(p->tm_mon, 11); CHECK_EQ_I(p->tm_mday, 31); CHECK_EQ_I(p->tm_hour, 23); CHECK_EQ_I(p->tm_sec, 59); } }

int main(void)
{
    tc_begin();
    if (TC_AT_LEAST(80)) tc_iph_install();
    _putenv("TZ=UTC0"); _tzset();
    TC_RUN(just_beyond);
#if TC_CRT_VER >= 140
    TC_RUN(observed_last_valid);
#endif
    TC_RUN(llong_max); TC_RUN(last_valid);
    _putenv("TZ=");
    return tc_end();
}
