/* TC
 * id:    tm_neg_gmtime32_localtime32_min
 * area:  time
 * kind:  negative
 * funcs:
 * procs: _gmtime32 _localtime32
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs gmtime, _gmtime32, _localtime32 (NULL if the time is before midnight Jan 1 1970 UTC)
 * note:  called through procs so msvcr100 (whose mingw import lib lacks them) is covered. The INT_MAX upper boundary is probed separately in tm_gmtime32_probe_int_max (docs state the range as ending Jan 18 2038).
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
TC_CASE(gm_minus_one) { __time32_t t; tz(); v32 = -1; t = v32; CHECK(tc_p__gmtime32(&t) == NULL); }
TC_CASE(local_int_min) { __time32_t t; tz(); v32 = INT_MIN; t = v32; CHECK(tc_p__localtime32(&t) == NULL); }
TC_CASE(local_minus_day) { __time32_t t; tz(); v32 = -86400; t = v32; CHECK(tc_p__localtime32(&t) == NULL); }
TC_CASE(gm_zero_ok) { __time32_t t; struct tm *p; tz(); v32 = 0; t = v32; p = tc_p__gmtime32(&t); CHECK(p != NULL); if (p) CHECK_EQ_I(p->tm_year, 70); }
int main(void) { tc_begin(); tc_iph_install(); TC_LOAD_PROC(_gmtime32); TC_LOAD_PROC(_localtime32); CHECK(tc_p__gmtime32 && tc_p__localtime32); if (!tc_p__gmtime32 || !tc_p__localtime32) return tc_end(); TC_RUN(gm_int_min); TC_RUN(gm_minus_day); TC_RUN(gm_minus_one); TC_RUN(local_int_min); TC_RUN(local_minus_day); TC_RUN(gm_zero_ok); return tc_end(); }
