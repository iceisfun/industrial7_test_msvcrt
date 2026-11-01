/* TC
 * id:    tm_timespec_get_basic
 * area:  time
 * funcs:
 * procs: timespec_get _timespec64_get
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.27.2.5; MS docs timespec_get, _timespec64_get
 * note:  [time bomb] the 32-bit case compares _timespec32_get tv_sec with the guest clock (+-5 s) and would fail after 2038-01-19 and if the guest clock is unset. Observed on Windows 10 22H2 build 19045 ucrtbase (triage-win10 wp2-time-0). ucrtbase exports _timespec32_get and _timespec64_get but NOT timespec_get (the header defines it inline over _timespec64_get); the test now exercises the exported _timespec32_get (struct {__time32_t; long}) and _timespec64_get with base TIME_UTC=1. Time check is +-5s of the system clock.
 */
#include "tc.h"
#include <time.h>

static long long unow(void)
{
    FILETIME f; ULARGE_INTEGER u; GetSystemTimeAsFileTime(&f); u.LowPart = f.dwLowDateTime; u.HighPart = f.dwHighDateTime;
    return (long long)(u.QuadPart / 10000000ULL) - 11644473600LL;
}
struct ts64 { __time64_t tv_sec; long tv_nsec; };
struct ts32 { __time32_t tv_sec; long tv_nsec; };  /* _timespec32_get */
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
TC_CASE(timespec32_get_utc)
{
    struct ts32 ts; long long now;
    CHECK(p_timespec_get != NULL); if (!p_timespec_get) return;
    ts.tv_sec = 0; ts.tv_nsec = -1;
    CHECK_EQ_I(p_timespec_get((struct ts64 *)&ts, 1), 1);
    now = unow();
    CHECK(ts.tv_nsec >= 0 && ts.tv_nsec < 1000000000L);
    CHECK(ts.tv_sec >= now - 5 && ts.tv_sec <= now + 5);
}
TC_CASE(timespec64_get_utc) { CHECK(p_ts64 != NULL); if (p_ts64) run(p_ts64); }
int main(void)
{
    tc_begin();
    p_timespec_get = (tsg_t)tc_crt_proc("_timespec32_get");  /* timespec_get itself is not exported from ucrtbase 19045 (inline in the headers) */
    p_ts64 = (tsg_t)tc_crt_proc("_timespec64_get");
    TC_RUN(timespec32_get_utc); TC_RUN(timespec64_get_utc);
    return tc_end();
}
