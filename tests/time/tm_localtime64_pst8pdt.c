/* TC
 * id:    tm_localtime64_pst8pdt
 * area:  time
 * funcs: _localtime64
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _tzset (TZ format), _localtime64
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

static volatile __time64_t v_t;
static void set_tz(const char *s) { _putenv(s); _tzset(); }
#define CHK_TM(r, Y, Mo, D, H, Mi, S, DST) do { CHECK((r) != NULL); if (r) { CHECK_EQ_I((r)->tm_year, (Y) - 1900); CHECK_EQ_I((r)->tm_mon, (Mo) - 1); CHECK_EQ_I((r)->tm_mday, D); CHECK_EQ_I((r)->tm_hour, H); CHECK_EQ_I((r)->tm_min, Mi); CHECK_EQ_I((r)->tm_sec, S); CHECK_EQ_I((r)->tm_isdst, DST); } } while (0)

TC_CASE(winter) { struct tm *r; set_tz("TZ=PST8PDT"); v_t = 1579118400LL; r = _localtime64((const __time64_t *)&v_t); CHK_TM(r, 2020, 1, 15, 12, 0, 0, 0); }
TC_CASE(summer) { struct tm *r; set_tz("TZ=PST8PDT"); v_t = 1593864000LL; r = _localtime64((const __time64_t *)&v_t); CHK_TM(r, 2020, 7, 4, 5, 0, 0, 1); }
int main(void) { tc_begin(); TC_RUN(winter); TC_RUN(summer); return tc_end(); }
