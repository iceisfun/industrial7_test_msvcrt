/* TC
 * id:    prc_beginthreadex_suspended
 * area:  process
 * funcs: _beginthreadex
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _beginthreadex (initflag CREATE_SUSPENDED)
 * note:  Sleep only gives a wrongly-running thread a chance; only the "did not run" direction is asserted.
 */
#include "tc.h"
#include <process.h>

static volatile LONG g_ran;
static unsigned __stdcall worker(void *a) { (void)a; InterlockedExchange(&g_ran, 1); return 7; }

TC_CASE(suspended)
{
    unsigned id; DWORD code = 0, prev; HANDLE h = (HANDLE)_beginthreadex(NULL, 0, worker, NULL, CREATE_SUSPENDED, &id);
    CHECK(h != NULL); if (!h) return;
    Sleep(50);
    CHECK_EQ_I(g_ran, 0);
    prev = ResumeThread(h);
    CHECK_EQ_U(prev, 1);
    CHECK_EQ_U(WaitForSingleObject(h, 5000), WAIT_OBJECT_0);
    CHECK_EQ_I(g_ran, 1);
    GetExitCodeThread(h, &code);
    CHECK_EQ_U(code, 7);
    CloseHandle(h);
}

int main(void) { tc_begin(); TC_RUN(suspended); return tc_end(); }
