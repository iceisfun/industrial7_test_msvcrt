/* TC
 * id:    err_errno_thread_isolation
 * area:  errno_param
 * funcs: _errno
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs errno (per-thread); CreateThread
 * note:  Uses kernel32 CreateThread; CRT lazily allocates per-thread data.
 */
#include "tc.h"
#include <errno.h>

static int *worker_ptr, worker_seen, worker_after;
static volatile LONG stage;

static DWORD WINAPI w1(LPVOID arg)
{
    (void)arg;
    worker_ptr = _errno();
    errno = ERANGE;
    worker_seen = errno;
    InterlockedExchange(&stage, 1);
    while (stage < 2) Sleep(1);
    worker_after = errno;
    return 0;
}
static int run_thread(LPTHREAD_START_ROUTINE f, LPVOID a)
{
    HANDLE h = CreateThread(NULL, 0, f, a, 0, NULL);
    if (!h) return 0;
    if (WaitForSingleObject(h, 10000) != WAIT_OBJECT_0) { CloseHandle(h); return 0; }
    CloseHandle(h); return 1;
}
TC_CASE(main_unaffected)
{
    HANDLE h; stage = 0; errno = EDOM;
    h = CreateThread(NULL, 0, w1, NULL, 0, NULL); CHECK(h != NULL); if (!h) return;
    { int n = 0; while (stage < 1 && n++ < 10000) Sleep(1); }
    if (stage < 1) { CHECK(stage >= 1); InterlockedExchange(&stage, 2); WaitForSingleObject(h, 10000); CloseHandle(h); return; }
    CHECK_EQ_I(errno, EDOM);            /* worker set ERANGE */
    CHECK_EQ_I(worker_seen, ERANGE);
    CHECK(worker_ptr != _errno());
    errno = ENOMEM; InterlockedExchange(&stage, 2);
    WaitForSingleObject(h, 10000); CloseHandle(h);
    CHECK_EQ_I(worker_after, ERANGE);   /* main's later write not visible in worker */
    CHECK_EQ_I(errno, ENOMEM); errno = 0;
}
static DWORD WINAPI w2(LPVOID arg)
{
    int mine = (int)(INT_PTR)arg, i, bad = 0;
    for (i = 0; i < 2000; i++) { errno = mine; if ((i & 63) == 0) Sleep(0); if (errno != mine) bad++; }
    return (DWORD)bad;
}
TC_CASE(two_workers)
{
    HANDLE h[2]; DWORD r0 = 99, r1 = 99;
    h[0] = CreateThread(NULL, 0, w2, (LPVOID)(INT_PTR)EINVAL, 0, NULL);
    h[1] = CreateThread(NULL, 0, w2, (LPVOID)(INT_PTR)EDOM, 0, NULL);
    CHECK(h[0] && h[1]); if (!h[0] || !h[1]) return;
    CHECK(WaitForMultipleObjects(2, h, TRUE, 10000) == WAIT_OBJECT_0);
    GetExitCodeThread(h[0], &r0); GetExitCodeThread(h[1], &r1);
    CloseHandle(h[0]); CloseHandle(h[1]);
    CHECK_EQ_U(r0, 0); CHECK_EQ_U(r1, 0);
}
int main(void) { tc_begin(); (void)run_thread; TC_RUN(main_unaffected); TC_RUN(two_workers); return tc_end(); }
