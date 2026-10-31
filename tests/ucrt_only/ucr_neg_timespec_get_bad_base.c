/* TC
 * id:    ucr_neg_timespec_get_bad_base
 * area:  ucrt_only
 * funcs:
 * procs: timespec_get
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.27.2.5 (returns zero if the base is not supported); MS docs timespec_get
 * note:  Only bases 0, -1 and 100 are asserted (TIME_MONOTONIC=2 of C23 deliberately left out). Only the zero return is asserted; "ts untouched" is in ucr_neg_timespec_get_bad_base_untouched_probe; handler count is not asserted.
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

static volatile int v_base;

static void bad(int base)
{
    tc_ts64 ts; int r;
    ts.tv_sec = 0x12345; ts.tv_nsec = 0x6789;
    v_base = base;
    r = timespec_get_p(&ts, v_base);
    CHECK_EQ_I(r, 0);
}
TC_CASE(base_zero) { bad(0); }
TC_CASE(base_minus1) { bad(-1); }
TC_CASE(base_100) { bad(100); }
int main(void) { tc_begin(); load(); if (!timespec_get_p) return 2; tc_iph_install(); TC_RUN(base_zero); TC_RUN(base_minus1); TC_RUN(base_100); return tc_end(); }
