/* TC
 * id:    tm_neg_gmtime64_negative
 * area:  time
 * funcs: _gmtime64
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _gmtime64: NULL if before midnight Jan 1 1970
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _gmtime64 accepts down to -43200 (1969-12-31 12:00:00 UTC); -43201, -86400 and LLONG_MIN return NULL with errno EINVAL. Docs say NULL before 1970; Windows deviates.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

static volatile __time64_t v_t;

static void one(__time64_t t)
{
    struct tm *r; v_t = t; errno = 0; tc_iph_count = 0;
    r = _gmtime64((const __time64_t *)&v_t);
    CHECK(r == NULL);
#if TC_CRT_VER >= 90
    CHECK_EQ_I(errno, EINVAL);
#endif
}
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
TC_CASE(minus_one) { one(-43201); }
TC_CASE(minus_one_second_valid)
{
    struct tm *r; v_t = -1; r = _gmtime64((const __time64_t *)&v_t); CHECK(r != NULL);
    if (r) { CHECK_EQ_I(r->tm_year, 69); CHECK_EQ_I(r->tm_mon, 11); CHECK_EQ_I(r->tm_mday, 31); CHECK_EQ_I(r->tm_hour, 23); CHECK_EQ_I(r->tm_min, 59); CHECK_EQ_I(r->tm_sec, 59); }
}
#else
TC_CASE(minus_one) { one(-1); }
#endif
TC_CASE(minus_day) { one(-86400); }
TC_CASE(llong_min) { one(LLONG_MIN); }
TC_CASE(zero_still_valid) { struct tm *r; v_t = 0; r = _gmtime64((const __time64_t *)&v_t); CHECK(r != NULL); if (r) CHECK_EQ_I(r->tm_year, 70); }
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(minus_one); TC_RUN(minus_one_second_valid); TC_RUN(minus_day); TC_RUN(llong_min); TC_RUN(zero_still_valid); return tc_end(); }
#else
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(minus_one); TC_RUN(minus_day); TC_RUN(llong_min); TC_RUN(zero_still_valid); return tc_end(); }
#endif
