/* TC
 * id:    err_get_errno_thread_local
 * area:  errno_param
 * funcs: 
 * procs: _get_errno _set_errno
 * crt:   >=100
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs errno, _errno, _set_errno (errno is per-thread)
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(errno_t, _set_errno, (int));
TC_DECL_PROC(errno_t, _get_errno, (int *));

static volatile int w_seen = -999, w_after = -999;

static DWORD WINAPI worker_a(LPVOID p)
{
    (void)p; tc_p__set_errno(ERANGE); tc_p__get_errno((int *)&w_after); return 0;
}
static DWORD WINAPI worker_b(LPVOID p)
{
    int v = -999; (void)p; tc_p__get_errno(&v); w_seen = v;
    tc_p__set_errno(ERANGE); tc_p__get_errno(&v); w_after = v; return 0;
}
static int run(LPTHREAD_START_ROUTINE f)
{
    HANDLE h = CreateThread(NULL, 0, f, NULL, 0, NULL); if (!h) return 0;
    WaitForSingleObject(h, 10000); CloseHandle(h); return 1;
}
TC_CASE(worker_does_not_change_main)
{
    int v = -1;
    tc_p__set_errno(EDOM);
    CHECK(run(worker_a));
    CHECK_EQ_I(w_after, ERANGE);
    CHECK_EQ_I(tc_p__get_errno(&v), 0); CHECK_EQ_I(v, EDOM);
}
TC_CASE(worker_has_own_value)
{
    int v = -1;
    tc_p__set_errno(EDOM);
    CHECK(run(worker_b));
    CHECK_EQ_I(w_after, ERANGE);
    CHECK(w_seen != EDOM);   /* new thread does not inherit the creator's errno */
    tc_p__get_errno(&v); CHECK_EQ_I(v, EDOM);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_errno); TC_LOAD_PROC(_get_errno);
    if (!tc_p__set_errno || !tc_p__get_errno) { tc_write("TC FAIL " TC_TEST_ID "::__setup\n"); return 1; }
    TC_RUN(worker_does_not_change_main); TC_RUN(worker_has_own_value);
    errno = 0;
    return tc_end();
}
