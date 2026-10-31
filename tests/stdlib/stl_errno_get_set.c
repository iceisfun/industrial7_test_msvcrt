/* TC
 * id:    stl_errno_get_set
 * area:  stdlib
 * funcs: _get_errno _set_errno _errno
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _get_errno, _set_errno; errno is per-thread
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(set_get)
{
    int e = -1;
    CHECK_EQ_I(_set_errno(ERANGE), 0);
    CHECK_EQ_I(_get_errno(&e), 0);
    CHECK_EQ_I(e, ERANGE);
    CHECK_EQ_I(_set_errno(0), 0);
    CHECK_EQ_I(_get_errno(&e), 0);
    CHECK_EQ_I(e, 0);
}
TC_CASE(macro_agrees)
{
    int e = -1;
    _set_errno(EDOM);
    CHECK_EQ_I(errno, EDOM);
    CHECK_EQ_I(*_errno(), EDOM);
    errno = EINVAL;
    _get_errno(&e);
    CHECK_EQ_I(e, EINVAL);
    errno = 0;
}
static DWORD WINAPI thr(LPVOID p)
{
    int e = -1;
    _set_errno(EDOM);
    _get_errno(&e);
    *(volatile int *)p = e;
    return 0;
}
TC_CASE(per_thread)
{
    volatile int te = -1; int e = -1; DWORD id;
    HANDLE h;
    _set_errno(ERANGE);
    h = CreateThread(NULL, 0, thr, (LPVOID)&te, 0, &id);
    CHECK(h != NULL);
    if (h) { CHECK(WaitForSingleObject(h, 10000) == WAIT_OBJECT_0); CloseHandle(h); }
    CHECK_EQ_I(te, EDOM);
    _get_errno(&e);
    CHECK_EQ_I(e, ERANGE);   /* main thread's value unaffected */
    _set_errno(0);
}
int main(void) { tc_begin(); TC_RUN(set_get); TC_RUN(macro_agrees); TC_RUN(per_thread); return tc_end(); }
