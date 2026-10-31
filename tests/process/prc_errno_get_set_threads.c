/* TC
 * id:    prc_errno_get_set_threads
 * area:  process
 * funcs: _beginthreadex
 * procs: _get_errno _set_errno
 * crt:   >=80
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _get_errno, _set_errno (VS2005+)
 * note:  procs because old import libs lack them.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

TC_DECL_PROC(errno_t, _get_errno, (int *));
TC_DECL_PROC(errno_t, _set_errno, (int));

static volatile int wret = -1, wval = -1;
static unsigned __stdcall worker(void *p)
{
    (void)p;
    wret = tc_p__set_errno(EDOM);
    wval = errno;
    return 0;
}
TC_CASE(set_get)
{
    int v = 0;
    if (!tc_p__set_errno || !tc_p__get_errno) { CHECK(0); return; }
    CHECK_EQ_I(tc_p__set_errno(ERANGE), 0);
    CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_I(tc_p__get_errno(&v), 0);
    CHECK_EQ_I(v, ERANGE);
}
TC_CASE(per_thread)
{
    HANDLE h; unsigned id;
    if (!tc_p__set_errno) { CHECK(0); return; }
    tc_p__set_errno(ERANGE);
    h = (HANDLE)_beginthreadex(NULL, 0, worker, NULL, 0, &id);
    CHECK(h != NULL);
    if (!h) return;
    CHECK_EQ_I(WaitForSingleObject(h, 10000), WAIT_OBJECT_0);
    CloseHandle(h);
    CHECK_EQ_I(wret, 0);
    CHECK_EQ_I(wval, EDOM);
    CHECK_EQ_I(errno, ERANGE);
    errno = 0;
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_errno); TC_LOAD_PROC(_set_errno);
    TC_RUN(set_get); TC_RUN(per_thread);
    return tc_end();
}
