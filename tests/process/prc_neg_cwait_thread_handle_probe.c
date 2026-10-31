/* TC
 * id:    prc_neg_cwait_thread_handle_probe
 * area:  process
 * funcs: _cwait _beginthreadex
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _cwait (process handles only; -1 on error)
 * note:  PROBE: _cwait is documented for process handles. Handing it a THREAD handle is expected to fail (GetExitCodeProcess
 *        rejects a thread handle) with -1. Three independent guesses, one case each: return -1; *termstat untouched on failure;
 *        handle not closed by the CRT on failure. The latter two are only checked when -1 was returned.
 */
#include "tc.h"
#include <process.h>

static HANDLE g_ev;
static unsigned __stdcall worker(void *p) { (void)p; WaitForSingleObject(g_ev, 10000); return 0; }

static intptr_t g_r; static int g_st, g_closeable;

static void run(void)
{
    unsigned id; uintptr_t h;
    g_st = 0x5A5A; g_r = -2; g_closeable = -1;
    g_ev = CreateEventA(NULL, TRUE, FALSE, NULL);
    h = _beginthreadex(NULL, 0, worker, NULL, 0, &id);
    if (!h) { CloseHandle(g_ev); return; }
    SetEvent(g_ev);
    g_r = _cwait(&g_st, (intptr_t)h, 0);
    if (g_r == -1) g_closeable = CloseHandle((HANDLE)h) ? 1 : 0;   /* only meaningful if the CRT left it open */
    CloseHandle(g_ev);
}
/* each guess in its own case so one wrong guess cannot mask another */
TC_CASE(thread_handle_fails)   { run(); CHECK(g_r != -2); CHECK_EQ_I(g_r, -1); }
TC_CASE(termstat_untouched)    { run(); if (g_r == -1) CHECK_EQ_I(g_st, 0x5A5A); }
TC_CASE(handle_not_closed)     { run(); if (g_r == -1) CHECK_EQ_I(g_closeable, 1); }
int main(void) { tc_begin(); TC_RUN(thread_handle_fails); TC_RUN(termstat_untouched); TC_RUN(handle_not_closed); return tc_end(); }
