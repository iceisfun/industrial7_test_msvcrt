/* TC
 * id:    prc_neg_cwait_invalid_handle
 * area:  process
 * kind:  negative
 * funcs: _cwait
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _cwait: returns -1 on error with errno ECHILD or EINVAL
 * note:  errno is documented as ECHILD/EINVAL; EBADF (ERROR_INVALID_HANDLE mapping) is also accepted as the CRT may
 *        map the failed wait directly. Never passes (intptr_t)-1: that is the current-process pseudo handle and the wait would block.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static void check_handle(intptr_t h)
{
    int status = 0x1234; intptr_t r; int e, ok;
    errno = 0;
    r = _cwait(&status, h, 0);
    e = errno;
    CHECK_EQ_I(r, -1);
    ok = (e == ECHILD || e == EINVAL || e == EBADF);
    CHECK(ok);
}
TC_CASE(handle_zero)    { volatile intptr_t h = 0; check_handle(h); }
TC_CASE(handle_garbage) { volatile intptr_t h = 0x7ffff00; check_handle(h); }
TC_CASE(null_status)
{
    volatile intptr_t h = 0; intptr_t r; int e, ok;
    errno = 0;
    r = _cwait(NULL, h, 0);
    e = errno;
    CHECK_EQ_I(r, -1);
    ok = (e == ECHILD || e == EINVAL || e == EBADF);
    CHECK(ok);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(handle_zero); TC_RUN(handle_garbage); TC_RUN(null_status);
    return tc_end();
}
