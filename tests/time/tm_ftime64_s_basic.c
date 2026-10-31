/* TC
 * id:    tm_ftime64_s_basic
 * area:  time
 * funcs: _ftime64_s _ftime32_s
 * crt:   >=90 msvcrt
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ftime_s, _ftime32_s, _ftime64_s
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>

static void set_tz(const char *s) { _putenv(s); _tzset(); }

TC_CASE(s64)
{
    struct __timeb64 tb; __time64_t t; set_tz("TZ=UTC0"); tb.time = 0; tb.millitm = 5000;
    CHECK_EQ_I(_ftime64_s(&tb), 0); t = _time64(NULL);
    CHECK(tb.time >= t - 3 && tb.time <= t + 3); CHECK(tb.millitm < 1000);
}
TC_CASE(s32) { struct __timeb32 tb; set_tz("TZ=UTC0"); tb.millitm = 5000; CHECK_EQ_I(_ftime32_s(&tb), 0); CHECK(tb.time > 1500000000L); CHECK(tb.millitm < 1000); }
int main(void) { tc_begin(); TC_RUN(s64); TC_RUN(s32); return tc_end(); }
