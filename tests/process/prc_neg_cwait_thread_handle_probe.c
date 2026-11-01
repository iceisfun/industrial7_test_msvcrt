/* TC
 * id:    prc_neg_cwait_thread_handle_probe
 * area:  process
 * funcs: _cwait _beginthreadex
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _cwait (process handles only; -1 on error)
 * note:  Observed on Win10 22H2 19045 ucrtbase: _cwait on a THREAD handle returns -1, stores -1 into *termstat (not left
 *        untouched) and has already closed the handle (a later CloseHandle fails). The old guesses (termstat untouched, handle
 *        left open) were wrong; the -1 return stays as before.
 *        handle_closed_by_crt assumes the handle value is not reused by another object between the CRT close and the test's own
 *        CloseHandle (single-threaded test, accepted; observed stable).
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
TC_CASE(termstat_set_to_minus1) { run(); CHECK_EQ_I(g_r, -1); CHECK_EQ_I(g_st, -1); }   /* observed: *termstat = -1 on failure */
TC_CASE(handle_closed_by_crt)   { run(); CHECK_EQ_I(g_r, -1); CHECK_EQ_I(g_closeable, 0); } /* observed: the CRT closed the handle even though it failed */
int main(void) { tc_begin(); TC_RUN(thread_handle_fails); TC_RUN(termstat_set_to_minus1); TC_RUN(handle_closed_by_crt); return tc_end(); }
