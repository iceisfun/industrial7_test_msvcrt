/* TC
 * id:    ucr_timespec_get_basic
 * area:  ucrt_only
 * funcs:
 * procs: timespec_get
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.27.2.5 timespec_get; MS docs timespec_get, _timespec32_get, _timespec64_get
 */
#include "tc.h"

typedef struct { long long tv_sec; long tv_nsec; } tc_ts64;
typedef struct { long tv_sec; long tv_nsec; } tc_ts32;
typedef int (__cdecl *tc_tsget64_t)(tc_ts64 *, int);
typedef int (__cdecl *tc_tsget32_t)(tc_ts32 *, int);
static tc_tsget64_t timespec_get_p, timespec64_get_p;
static tc_tsget32_t timespec32_get_p;
static void load(void)
{
    timespec_get_p = (tc_tsget64_t)tc_crt_proc("timespec_get");
    timespec64_get_p = (tc_tsget64_t)tc_crt_proc("_timespec64_get");
    timespec32_get_p = (tc_tsget32_t)tc_crt_proc("_timespec32_get");
}

static volatile int v_base = 1;

static long long win_unix_secs(void)
{
    FILETIME ft; ULARGE_INTEGER u;
    GetSystemTimeAsFileTime(&ft);
    u.LowPart = ft.dwLowDateTime; u.HighPart = ft.dwHighDateTime;
    return (long long)(u.QuadPart / 10000000ULL) - 11644473600LL;
}

TC_CASE(returns_base_and_range)
{
    tc_ts64 ts; int r;
    ts.tv_sec = 0; ts.tv_nsec = -1;
    r = timespec_get_p(&ts, v_base);
    CHECK_EQ_I(r, 1);
    CHECK(ts.tv_nsec >= 0 && ts.tv_nsec <= 999999999);
    CHECK((long long)ts.tv_sec > 1577836800LL);
}
TC_CASE(matches_system_time)
{
    tc_ts64 ts; long long d, w;
    CHECK_EQ_I(timespec_get_p(&ts, v_base), 1);
    w = win_unix_secs();
    d = (long long)ts.tv_sec - w; if (d < 0) d = -d;
    CHECK(d <= 120);
}
TC_CASE(monotone_enough)
{
    tc_ts64 a, b;
    CHECK_EQ_I(timespec_get_p(&a, v_base), 1);
    CHECK_EQ_I(timespec_get_p(&b, v_base), 1);
    CHECK((long long)b.tv_sec >= (long long)a.tv_sec - 1);
}
int main(void) { tc_begin(); load(); if (!timespec_get_p) return 2; TC_RUN(returns_base_and_range); TC_RUN(matches_system_time); TC_RUN(monotone_enough); return tc_end(); }
