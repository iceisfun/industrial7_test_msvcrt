/* TC
 * id:    gsp_neg_cwait_oldname_errno_probe
 * area:  process
 * funcs: _cwait
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _cwait (errno ECHILD or EINVAL); Windows error mapping of ERROR_INVALID_HANDLE is EBADF
 * note:  PROBE: errno after cwait on an invalid handle, and the behaviour for a signaled non-process handle.
 *        Case in_docs_or_mapped: errno is one of ECHILD/EBADF/EINVAL (nonzero). Case is_ebadf: exactly EBADF (most likely:
 *        _dosmaperr(ERROR_INVALID_HANDLE)). Case is_echild: exactly ECHILD (documented value; may be wrong on real Windows).
 *        Case null_termstat: NULL termstat does not crash and still fails with -1 (errno not asserted).
 *        Case event_handle: a signaled manual-reset event (wait succeeds immediately, GetExitCodeProcess fails) -> -1.
 *        Each exact-errno guess is its own case so they cannot mask each other.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static volatile intptr_t g_h = (intptr_t)0x7ffffff4;

TC_CASE(in_docs_or_mapped)
{
    int ts = 0, e;
    errno = 0; cwait(&ts, g_h, 0); e = errno;
    CHECK(e == ECHILD || e == EBADF || e == EINVAL);
}
TC_CASE(is_ebadf)  { int ts = 0; errno = 0; cwait(&ts, g_h, 0); CHECK_EQ_I(errno, EBADF); }
TC_CASE(is_echild) { int ts = 0; errno = 0; cwait(&ts, g_h, 0); CHECK_EQ_I(errno, ECHILD); }
TC_CASE(null_termstat)
{
    errno = 0;
    CHECK_EQ_I(cwait(NULL, g_h, 0), -1);
}
TC_CASE(event_handle)
{
    int ts = 0; HANDLE e = CreateEventA(NULL, TRUE, TRUE, NULL);
    CHECK(e != NULL);
    if (!e) return;
    g_h = (intptr_t)e;
    CHECK_EQ_I(cwait(&ts, g_h, 0), -1);
    g_h = (intptr_t)0x7ffffff4;
    CloseHandle(e);
}
int main(void)
{
    tc_begin();
    TC_RUN(in_docs_or_mapped); TC_RUN(is_ebadf); TC_RUN(is_echild); TC_RUN(null_termstat); TC_RUN(event_handle);
    return tc_end();
}
