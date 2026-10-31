/* TC
 * id:    tm_neg_timespec_get_probe_null
 * area:  time
 * funcs:
 * procs: _timespec64_get
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _timespec64_get (UCRT parameter validation convention)
 * note:  PROBES whether _timespec64_get(NULL, TIME_UTC) validates its parameter: expects return 0,
 *        errno EINVAL and one invalid-parameter handler call. Undocumented.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

struct ts64 { __time64_t tv_sec; long tv_nsec; };
typedef int (__cdecl *tsg_t)(struct ts64 *, int);

TC_CASE(null_ts)
{
    tsg_t f = (tsg_t)tc_crt_proc("_timespec64_get");
    struct ts64 *volatile np = NULL;
    CHECK(f != NULL);
    if (!f) return;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(f(np, 1), 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null_ts); return tc_end(); }
