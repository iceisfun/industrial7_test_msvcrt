/* TC
 * id:    ucr_timespec_variants
 * area:  ucrt_only
 * funcs:
 * procs: _timespec32_get _timespec64_get timespec_get
 * crt:   win10
 * tier:  p1
 * conf:  documented
 * ref:   MS docs timespec_get, _timespec32_get, _timespec64_get
 * Class D: "timespec_get" is not in the Win10 22H2 ucrtbase export table (baseline exit 2, no TC output); it is a header inline over _timespec64_get,
 *        so timespec_get_p falls back to the exported _timespec64_get.
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
    /* class D: ucrtbase does not export timespec_get; <time.h> inlines it over _timespec64_get, so test that exported entry point */
    if (!timespec_get_p) timespec_get_p = timespec64_get_p;
}

TC_CASE(variants_agree)
{
    tc_ts32 a; tc_ts64 b; tc_ts64 c; long long d;
    CHECK_EQ_I(timespec64_get_p(&b, 1), 1);
    CHECK_EQ_I(timespec32_get_p(&a, 1), 1);
    CHECK_EQ_I(timespec_get_p(&c, 1), 1);
    d = (long long)a.tv_sec - (long long)b.tv_sec; if (d < 0) d = -d;
    CHECK(d <= 5);
    d = (long long)c.tv_sec - (long long)b.tv_sec; if (d < 0) d = -d;
    CHECK(d <= 5);
}
TC_CASE(nsec_range)
{
    tc_ts32 a; tc_ts64 b;
    CHECK_EQ_I(timespec64_get_p(&b, 1), 1);
    CHECK_EQ_I(timespec32_get_p(&a, 1), 1);
    CHECK(b.tv_nsec >= 0 && b.tv_nsec <= 999999999);
    CHECK(a.tv_nsec >= 0 && a.tv_nsec <= 999999999);
}
int main(void) { tc_begin(); load(); if (!timespec_get_p) return 2; TC_RUN(variants_agree); TC_RUN(nsec_range); return tc_end(); }
