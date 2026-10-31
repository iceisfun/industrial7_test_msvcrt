/* TC
 * id:    prc_neg_cwait_errno_probe
 * area:  process
 * funcs: _cwait
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _cwait (errno ECHILD / EINVAL)
 * note:  PROBE: which errno _cwait sets for a bogus process handle. Accepts ECHILD (docs), EBADF (OS error mapping of
 *        ERROR_INVALID_HANDLE) or EINVAL; asserts only that it is one of these, and not 0.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

TC_CASE(errno_set)
{
    int st = 0, e;
    errno = 0; _cwait(&st, (intptr_t)0x7ffffff4, 0); e = errno;
    CHECK(e == ECHILD || e == EBADF || e == EINVAL);
}
int main(void) { tc_begin(); TC_RUN(errno_set); return tc_end(); }
