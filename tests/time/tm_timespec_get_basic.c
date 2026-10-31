/* TC
 * id:    tm_timespec_get_basic
 * area:  time
 * funcs:
 * procs: timespec_get _timespec64_get
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.27.2.5; MS docs timespec_get, _timespec64_get
 */
#include "tc.h"
#include <time.h>

static long long unow(void)
{
    FILETIME f; ULARGE_INTEGER u; GetSystemTimeAsFileTime(&f); u.LowPart = f.dwLowDateTime; u.HighPart = f.dwHighDateTime;
    return (long long)(u.QuadPart / 10000000ULL) - 11644473600LL;
}
struct ts64 { __time64_t tv_sec; long tv_nsec; };
typedef int (__cdecl *tsg_t)(struct ts64 *, int);
static tsg_t p_timespec_get, p_ts64;

static void run(tsg_t f)
{
    struct ts64 ts; long long now;
    ts.tv_sec = 0; ts.tv_nsec = -1;
    CHECK_EQ_I(f(&ts, 1), 1);
    now = unow();
    CHECK(ts.tv_nsec >= 0 && ts.tv_nsec < 1000000000L);
    CHECK(ts.tv_sec >= now - 5 && ts.tv_sec <= now + 5);
}
TC_CASE(timespec_get_utc) { CHECK(p_timespec_get != NULL); if (p_timespec_get) run(p_timespec_get); }
TC_CASE(timespec64_get_utc) { CHECK(p_ts64 != NULL); if (p_ts64) run(p_ts64); }
int main(void)
{
    tc_begin();
    p_timespec_get = (tsg_t)tc_crt_proc("timespec_get");
    p_ts64 = (tsg_t)tc_crt_proc("_timespec64_get");
    TC_RUN(timespec_get_utc); TC_RUN(timespec64_get_utc);
    return tc_end();
}
