/* TC
 * id:    tm_neg_timespec_get_bad_base
 * area:  time
 * funcs:
 * procs: timespec_get _timespec64_get
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.27.2.5: returns zero if the base is not supported
 * note:  bases 2.. omitted: C23 defines optional TIME_MONOTONIC etc; see tm_neg_timespec_get_probe_base_c23.
 */
#include "tc.h"
#include <time.h>
#include <limits.h>

struct ts64 { __time64_t tv_sec; long tv_nsec; };
typedef int (__cdecl *tsg_t)(struct ts64 *, int);
static tsg_t p_timespec_get, p_ts64;
static volatile int bases[] = { 0, -1, INT_MAX, INT_MIN };

static void run(tsg_t f)
{
    struct ts64 ts; unsigned i;
    CHECK(tc_iph_install()); /* guard: must not abort even if CRT validates the base */
    for (i = 0; i < sizeof bases / sizeof bases[0]; i++) {
        ts.tv_sec = 0; ts.tv_nsec = 0;
        CHECK_EQ_I(f(&ts, bases[i]), 0);
    }
}
TC_CASE(timespec_get_bad) { CHECK(p_timespec_get != NULL); if (p_timespec_get) run(p_timespec_get); }
TC_CASE(timespec64_get_bad) { CHECK(p_ts64 != NULL); if (p_ts64) run(p_ts64); }
int main(void)
{
    tc_begin();
    p_timespec_get = (tsg_t)tc_crt_proc("timespec_get");
    p_ts64 = (tsg_t)tc_crt_proc("_timespec64_get");
    TC_RUN(timespec_get_bad); TC_RUN(timespec64_get_bad);
    return tc_end();
}
