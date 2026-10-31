/* TC
 * id:    prc_neg_beginthread_zero_arg_ok_and_null_state
 * area:  process
 * funcs: _beginthread _beginthreadex
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _beginthread/_beginthreadex (return values)
 * note:  PROBE: after a rejected call with a NULL start address (invalid-parameter handler installed), a valid call must still
 *        succeed and the thread must run (CRT thread state not poisoned). The NULL rejection itself is the guess (not spelled
 *        out in the docs) and is not asserted here; see prc_neg_beginthread_null_start.
 */
#include "tc.h"
#include <process.h>

static HANDLE g_ev;
static void __cdecl w1(void *p) { (void)p; SetEvent(g_ev); }
static unsigned __stdcall w2(void *p) { (void)p; SetEvent(g_ev); return 0; }

TC_CASE(beginthread_after_null)
{
    uintptr_t r;
    CHECK(tc_iph_install());
    g_ev = CreateEventA(NULL, TRUE, FALSE, NULL);
    r = _beginthread(NULL, 0, NULL);
    r = _beginthread(w1, 0, NULL);
    CHECK(r != (uintptr_t)-1);
    CHECK(r != 0);
    CHECK_EQ_U(WaitForSingleObject(g_ev, 5000), WAIT_OBJECT_0);
    CloseHandle(g_ev);
}
TC_CASE(beginthreadex_after_null)
{
    uintptr_t r; unsigned id = 0;
    CHECK(tc_iph_install());
    g_ev = CreateEventA(NULL, TRUE, FALSE, NULL);
    r = _beginthreadex(NULL, 0, NULL, NULL, 0, NULL);
    r = _beginthreadex(NULL, 0, w2, NULL, 0, &id);
    CHECK(r != 0);
    CHECK_EQ_U(WaitForSingleObject(g_ev, 5000), WAIT_OBJECT_0);
    if (r) { WaitForSingleObject((HANDLE)r, 5000); CloseHandle((HANDLE)r); }
    CloseHandle(g_ev);
}
int main(void) { tc_begin(); TC_RUN(beginthread_after_null); TC_RUN(beginthreadex_after_null); return tc_end(); }
