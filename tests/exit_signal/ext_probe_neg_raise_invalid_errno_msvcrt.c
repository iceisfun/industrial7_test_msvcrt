/* TC
 * id:    ext_probe_neg_raise_invalid_errno_msvcrt
 * area:  exit_signal
 * kind:  negative
 * funcs: raise
 * crt:   msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs raise
 * note:  PROBE: msvcrt.dll raise(invalid) sets errno to EINVAL (older CRTs may only return -1 without setting errno).
 */
#include "tc.h"
#include <signal.h>
#include <errno.h>
TC_CASE(errno_einval)
{
    volatile int sn = 9;
    int r;
    errno = 0;
    r = raise(sn);
    CHECK(r != 0);
    CHECK_EQ_I(errno, EINVAL);
}
int main(void) { tc_begin(); TC_RUN(errno_einval); return tc_end(); }
