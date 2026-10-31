/* TC
 * id:    prc_beginthreadex_basic
 * area:  process
 * funcs: _beginthreadex
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _beginthread, _beginthreadex
 */
#include "tc.h"
#include <process.h>

static int g_marker; static void *volatile g_arg;
static unsigned __stdcall worker(void *a) { g_arg = a; return 42; }

TC_CASE(basic)
{
    unsigned id = 0; DWORD code = 0; HANDLE h = (HANDLE)_beginthreadex(NULL, 0, worker, &g_marker, 0, &id);
    CHECK(h != NULL); if (!h) return;
    CHECK(id != 0);
    CHECK_EQ_U(id, GetThreadId(h));
    CHECK_EQ_U(WaitForSingleObject(h, 5000), WAIT_OBJECT_0);
    CHECK(GetExitCodeThread(h, &code));
    CHECK_EQ_U(code, 42);
    CHECK_EQ_P(g_arg, &g_marker);
    CHECK(CloseHandle(h));    /* caller owns the handle */
}

int main(void) { tc_begin(); TC_RUN(basic); return tc_end(); }
