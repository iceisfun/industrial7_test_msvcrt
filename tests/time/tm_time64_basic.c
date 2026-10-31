/* TC
 * id:    tm_time64_basic
 * area:  time
 * funcs: _time64
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _time64
 */
#include "tc.h"
#include <time.h>
#include <stdlib.h>
#include <errno.h>

TC_CASE(range) { __time64_t t = _time64(NULL); CHECK(t > 1600000000LL && t < 32535244800LL); }
TC_CASE(out_pointer) { __time64_t t = 0, r = _time64(&t), r2 = _time64(NULL); CHECK(t != 0); CHECK(r2 - t >= 0 && r2 - t <= 5); CHECK(r2 - r >= 0 && r2 - r <= 5); }
TC_CASE(matches_system_time)
{
    FILETIME ft; ULARGE_INTEGER u; long long sys; __time64_t t;
    GetSystemTimeAsFileTime(&ft);
    u.LowPart = ft.dwLowDateTime; u.HighPart = ft.dwHighDateTime;
    sys = (long long)(u.QuadPart / 10000000ULL) - 11644473600LL;
    t = _time64(NULL);
    CHECK(t - sys >= -5 && t - sys <= 5);
}
TC_CASE(type_size) { CHECK_EQ_U(sizeof(__time64_t), 8); }
int main(void) { tc_begin(); TC_RUN(range); TC_RUN(out_pointer); TC_RUN(matches_system_time); TC_RUN(type_size); return tc_end(); }
