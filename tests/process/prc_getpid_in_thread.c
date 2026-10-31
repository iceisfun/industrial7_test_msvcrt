/* TC
 * id:    prc_getpid_in_thread
 * area:  process
 * funcs: _getpid _beginthreadex
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _getpid, _beginthreadex
 */
#include "tc.h"
#include <process.h>

static volatile int g_pid; static volatile DWORD g_win;
static unsigned __stdcall worker(void *a) { (void)a; g_pid = _getpid(); g_win = GetCurrentProcessId(); return 0; }

TC_CASE(worker_pid)
{
    unsigned id; HANDLE h = (HANDLE)_beginthreadex(NULL, 0, worker, NULL, 0, &id);
    CHECK(h != NULL); if (!h) return;
    CHECK_EQ_U(WaitForSingleObject(h, 5000), WAIT_OBJECT_0);
    CloseHandle(h);
    CHECK_EQ_I(g_pid, _getpid());
    CHECK_EQ_U((unsigned)g_pid, g_win);
}

int main(void) { tc_begin(); TC_RUN(worker_pid); return tc_end(); }
