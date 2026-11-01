/* TC
 * id:    ucr_neg_timespec_get_bad_base_untouched_probe
 * area:  ucrt_only
 * funcs:
 * procs: _timespec64_get _timespec32_get
 * crt:   win10
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.27.2.5; MS docs timespec_get
 * note:  PROBE: with an unsupported base the CRT returns 0 and leaves *ts unmodified (standard text does not require this). Called through the real export
 *        _timespec64_get (ucrtbase does not export "timespec_get"; observed GetProcAddress NULL).
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
    timespec_get_p = (tc_tsget64_t)tc_crt_proc("_timespec64_get");   /* ucrtbase exports _timespec64_get/_timespec32_get; "timespec_get" is an inline wrapper */
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
    CHECK_EQ_I(ts.tv_sec, 0x12345);
    CHECK_EQ_I(ts.tv_nsec, 0x6789);
}
TC_CASE(base_zero) { bad(0); }
TC_CASE(base_minus1) { bad(-1); }
TC_CASE(base_100) { bad(100); }
int main(void) { tc_begin(); load(); if (!timespec_get_p) return 2; tc_iph_install(); TC_RUN(base_zero); TC_RUN(base_minus1); TC_RUN(base_100); return tc_end(); }
