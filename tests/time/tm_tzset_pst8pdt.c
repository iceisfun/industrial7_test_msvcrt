/* TC
 * id:    tm_tzset_pst8pdt
 * area:  time
 * funcs: _tzset _get_timezone _get_daylight _get_tzname
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _tzset, _get_timezone, _get_daylight, _get_tzname
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>

TC_CASE(pst8pdt)
{
    long tz = 0; int dl = 0; char b0[16], b1[16]; size_t n0 = 0, n1 = 0;
    _putenv("TZ=PST8PDT"); _tzset();
    CHECK_EQ_I(_get_timezone(&tz), 0); CHECK_EQ_I(tz, 28800);
    CHECK_EQ_I(_get_daylight(&dl), 0); CHECK_EQ_I(dl, 1);
    CHECK_EQ_I(_get_tzname(&n0, b0, sizeof b0, 0), 0); CHECK_EQ_U(n0, 4); CHECK_STR(b0, "PST");
    CHECK_EQ_I(_get_tzname(&n1, b1, sizeof b1, 1), 0); CHECK_EQ_U(n1, 4); CHECK_STR(b1, "PDT");
}
int main(void) { tc_begin(); TC_RUN(pst8pdt); return tc_end(); }
