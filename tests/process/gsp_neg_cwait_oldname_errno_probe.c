/* TC
 * id:    gsp_neg_cwait_oldname_errno_probe
 * area:  process
 * funcs: _cwait
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _cwait (errno ECHILD or EINVAL); Windows error mapping of ERROR_INVALID_HANDLE is EBADF
 * note:  OBSERVED on Win10 22H2 (19045): cwait/_cwait on an invalid handle returns -1 with errno ECHILD (10), not EBADF; on a
 *        signaled event handle (not a process) it returns the handle value itself and sets *termstat to 0 (the
 *        GetExitCodeProcess failure is ignored). Earlier guesses EBADF and -1 were wrong (EBADF is excluded by is_echild pinning ECHILD exactly). Handle value is not hard-coded.
 *        Stable.
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
TC_CASE(is_echild) { int ts = 0; errno = 0; cwait(&ts, g_h, 0); CHECK_EQ_I(errno, ECHILD); }
TC_CASE(null_termstat)
{
    errno = 0;
    CHECK_EQ_I(cwait(NULL, g_h, 0), -1);
}
TC_CASE(event_handle)
{
    int ts = 0x5a5a; HANDLE e = CreateEventA(NULL, TRUE, TRUE, NULL);
    CHECK(e != NULL);
    if (!e) return;
    g_h = (intptr_t)e;
    CHECK_EQ_I(cwait(&ts, g_h, 0), (intptr_t)e);   /* observed: returns the handle, GetExitCodeProcess failure is ignored */
    CHECK_EQ_I(ts, 0);
    g_h = (intptr_t)0x7ffffff4;
    CloseHandle(e);
}
int main(void)
{
    tc_begin();
    TC_RUN(in_docs_or_mapped); TC_RUN(is_echild); TC_RUN(null_termstat); TC_RUN(event_handle);
    return tc_end();
}
