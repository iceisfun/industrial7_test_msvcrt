/* TC
 * id:    ucr_neg_timespec_get_null_probe
 * area:  ucrt_only
 * funcs:
 * procs: timespec_get _timespec64_get
 * crt:   win10
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.27.2.5 (UB for NULL); MS parameter validation convention
 * note:  PROBE: timespec_get_p(NULL, TIME_UTC) and timespec64_get_p(NULL, TIME_UTC) return 0, set errno=EINVAL and call the invalid-parameter handler once.
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
#include <errno.h>

static void *volatile v_null;

TC_CASE(timespec_get_null)
{
    int r; tc_iph_count = 0; errno = 0;
    r = timespec_get_p((tc_ts64 *)v_null, 1);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(timespec64_get_null)
{
    int r; tc_iph_count = 0; errno = 0;
    r = timespec64_get_p((tc_ts64 *)v_null, 1);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); load(); if (!timespec_get_p) return 2; if (!tc_iph_install()) return 2; TC_RUN(timespec_get_null); TC_RUN(timespec64_get_null); return tc_end(); }
