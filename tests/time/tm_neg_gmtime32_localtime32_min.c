/* TC
 * id:    tm_neg_gmtime32_localtime32_min
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _gmtime32 _localtime32
 * crt:   >=90
 * tier:  p0
 * conf:  observed
 * ref:   MS docs gmtime, _gmtime32, _localtime32 (NULL if the time is before midnight Jan 1 1970 UTC)
 * note:  Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). _gmtime32 accepts down to -43200 (1969-12-31 12:00:00 UTC), so -1 is valid (returns the 1970-01-01 00:00 tm, truncation toward zero), -43201 and -86400 return NULL; _localtime32 rejects every negative value. Docs say NULL before 1970; Windows is more permissive for gmtime. Called through procs.
 *        errno is not asserted (not documented for the range case). TZ=UTC0.
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <limits.h>
TC_DECL_PROC(struct tm *, _gmtime32, (const __time32_t *));
TC_DECL_PROC(struct tm *, _localtime32, (const __time32_t *));
static volatile __time32_t v32;
static void tz(void) { _putenv("TZ=UTC0"); _tzset(); }
TC_CASE(gm_int_min) { __time32_t t; tz(); v32 = INT_MIN; t = v32; CHECK(tc_p__gmtime32(&t) == NULL); }
TC_CASE(gm_minus_day) { __time32_t t; tz(); v32 = -86400; t = v32; CHECK(tc_p__gmtime32(&t) == NULL); }
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
TC_CASE(gm_minus_one) { __time32_t t; struct tm *p; tz(); v32 = -1; t = v32; p = tc_p__gmtime32(&t); CHECK(p != NULL); if (p) { CHECK_EQ_I(p->tm_year, 70); CHECK_EQ_I(p->tm_mday, 1); CHECK_EQ_I(p->tm_hour, 0); } /* observed: _gmtime32(-1) yields the 1970-01-01 00:00 tm (truncation toward zero; _gmtime32_s(-1) shows tm_sec == -1), unlike _gmtime64(-1) = 1969-12-31 23:59:59 */ }
TC_CASE(gm_below_min) { __time32_t t; tz(); v32 = -43201; t = v32; CHECK(tc_p__gmtime32(&t) == NULL); }
TC_CASE(local_minus_one) { __time32_t t; tz(); v32 = -1; t = v32; CHECK(tc_p__localtime32(&t) == NULL); }
#else
TC_CASE(gm_minus_one) { __time32_t t; tz(); v32 = -1; t = v32; CHECK(tc_p__gmtime32(&t) == NULL); }
#endif
TC_CASE(local_int_min) { __time32_t t; tz(); v32 = INT_MIN; t = v32; CHECK(tc_p__localtime32(&t) == NULL); }
TC_CASE(local_minus_day) { __time32_t t; tz(); v32 = -86400; t = v32; CHECK(tc_p__localtime32(&t) == NULL); }
TC_CASE(gm_zero_ok) { __time32_t t; struct tm *p; tz(); v32 = 0; t = v32; p = tc_p__gmtime32(&t); CHECK(p != NULL); if (p) CHECK_EQ_I(p->tm_year, 70); }
#if TC_CRT_VER >= 140   /* ucrtbase: observed on Win10 22H2; older CRTs keep the original expectation (not run on Windows) */
int main(void) { tc_begin(); tc_iph_install(); TC_LOAD_PROC(_gmtime32); TC_LOAD_PROC(_localtime32); CHECK(tc_p__gmtime32 && tc_p__localtime32); if (!tc_p__gmtime32 || !tc_p__localtime32) return tc_end(); TC_RUN(gm_int_min); TC_RUN(gm_minus_day); TC_RUN(gm_minus_one); TC_RUN(gm_below_min); TC_RUN(local_minus_one); TC_RUN(local_int_min); TC_RUN(local_minus_day); TC_RUN(gm_zero_ok); return tc_end(); }
#else
int main(void) { tc_begin(); tc_iph_install(); TC_LOAD_PROC(_gmtime32); TC_LOAD_PROC(_localtime32); CHECK(tc_p__gmtime32 && tc_p__localtime32); if (!tc_p__gmtime32 || !tc_p__localtime32) return tc_end(); TC_RUN(gm_int_min); TC_RUN(gm_minus_day); TC_RUN(gm_minus_one); TC_RUN(local_int_min); TC_RUN(local_minus_day); TC_RUN(gm_zero_ok); return tc_end(); }
#endif
