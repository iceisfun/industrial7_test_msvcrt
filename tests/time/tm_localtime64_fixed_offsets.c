/* TC
 * id:    tm_localtime64_fixed_offsets
 * area:  time
 * funcs: _localtime64
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _tzset (TZ syntax tzn [+|-]hh[:mm[:ss]])
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

static volatile __time64_t v_t;
static void set_tz(const char *s) { _putenv(s); _tzset(); }
#define CHK_TM(r, Y, Mo, D, H, Mi, S, DST) do { CHECK((r) != NULL); if (r) { CHECK_EQ_I((r)->tm_year, (Y) - 1900); CHECK_EQ_I((r)->tm_mon, (Mo) - 1); CHECK_EQ_I((r)->tm_mday, D); CHECK_EQ_I((r)->tm_hour, H); CHECK_EQ_I((r)->tm_min, Mi); CHECK_EQ_I((r)->tm_sec, S); CHECK_EQ_I((r)->tm_isdst, DST); } } while (0)

TC_CASE(est5) { struct tm *r; set_tz("TZ=EST5"); v_t = 86400LL * 10; r = _localtime64((const __time64_t *)&v_t); CHK_TM(r, 1970, 1, 10, 19, 0, 0, 0); }
TC_CASE(jst) { struct tm *r; set_tz("TZ=JST-9"); v_t = 0; r = _localtime64((const __time64_t *)&v_t); CHK_TM(r, 1970, 1, 1, 9, 0, 0, 0); }
TC_CASE(plus530) { struct tm *r; set_tz("TZ=XXX-5:30"); v_t = 86400LL * 10; r = _localtime64((const __time64_t *)&v_t); CHK_TM(r, 1970, 1, 11, 5, 30, 0, 0); }
TC_CASE(seconds) { struct tm *r; set_tz("TZ=XXX3:30:15"); v_t = 86400LL * 10; r = _localtime64((const __time64_t *)&v_t); CHK_TM(r, 1970, 1, 10, 20, 29, 45, 0); }
int main(void) { tc_begin(); TC_RUN(est5); TC_RUN(jst); TC_RUN(plus530); TC_RUN(seconds); return tc_end(); }
