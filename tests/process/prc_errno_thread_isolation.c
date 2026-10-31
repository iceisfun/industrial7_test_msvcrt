/* TC
 * id:    prc_errno_thread_isolation
 * area:  process
 * funcs: _beginthreadex _errno
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.5p3 (errno has thread storage duration); MS docs errno
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static HANDLE g_started, g_go;
static volatile int g_initial;
static unsigned __stdcall worker(void *a)
{
    (void)a;
    g_initial = errno;            /* fresh thread: no assertion on this value here */
    errno = ERANGE;
    SetEvent(g_started);
    WaitForSingleObject(g_go, 5000);
    return (unsigned)errno;       /* must still be ERANGE */
}

TC_CASE(isolation)
{
    DWORD code = 0xdeadbeef; HANDLE h;
    g_started = CreateEventA(NULL, FALSE, FALSE, NULL); g_go = CreateEventA(NULL, FALSE, FALSE, NULL);
    errno = EDOM;
    h = (HANDLE)_beginthreadex(NULL, 0, worker, NULL, 0, NULL);
    CHECK(h != NULL); if (!h) return;
    CHECK_EQ_U(WaitForSingleObject(g_started, 5000), WAIT_OBJECT_0);
    CHECK_EQ_I(errno, EDOM);          /* thread's write did not leak into main */
    errno = ENOENT;                    /* main writes while thread is alive */
    SetEvent(g_go);
    WaitForSingleObject(h, 5000);
    GetExitCodeThread(h, &code);
    CHECK_EQ_U(code, ERANGE);         /* main's write did not leak into thread */
    CHECK_EQ_I(errno, ENOENT);
    CloseHandle(h);
}
int main(void) { tc_begin(); TC_RUN(isolation); return tc_end(); }
