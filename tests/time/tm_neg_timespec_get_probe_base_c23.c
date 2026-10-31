/* TC
 * id:    tm_neg_timespec_get_probe_base_c23
 * area:  time
 * funcs:
 * procs: timespec_get _timespec64_get
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C23 7.29.2.7 (optional bases TIME_MONOTONIC/TIME_ACTIVE/TIME_THREAD_ACTIVE)
 * note:  PROBES whether ucrtbase rejects (returns 0) the C23 optional time bases 2, 3, 4.
 *        Newer UCRTs may support some; a return of 1 would mean support, not a bug.
 */
#include "tc.h"
#include <time.h>

struct ts64 { __time64_t tv_sec; long tv_nsec; };
typedef int (__cdecl *tsg_t)(struct ts64 *, int);
static volatile int bases[] = { 2, 3, 4 };

TC_CASE(c23_bases_unsupported)
{
    tsg_t f = (tsg_t)tc_crt_proc("timespec_get");
    struct ts64 ts; unsigned i;
    CHECK(f != NULL);
    if (!f) return;
    CHECK(tc_iph_install());
    for (i = 0; i < 3; i++) { ts.tv_sec = 0; ts.tv_nsec = 0; CHECK_EQ_I(f(&ts, bases[i]), 0); }
}
int main(void) { tc_begin(); TC_RUN(c23_bases_unsupported); return tc_end(); }
