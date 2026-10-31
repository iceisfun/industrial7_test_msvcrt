/* TC
 * id:    prc_beginthread_basic
 * area:  process
 * funcs: _beginthread _endthread
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _beginthread, _endthread
 * note:  The _beginthread handle is closed by the CRT: never wait on or close it; synchronise with an event.
 */
#include "tc.h"
#include <process.h>

static HANDLE g_ev; static void *volatile g_arg; static volatile LONG g_after;
static int g_tag;
static void __cdecl explicit_end(void *a) { g_arg = a; SetEvent(g_ev); _endthread(); InterlockedExchange(&g_after, 1); }
static void __cdecl fall_off(void *a) { g_arg = a; SetEvent(g_ev); }

TC_CASE(explicit_endthread)
{
    uintptr_t r; g_arg = NULL; ResetEvent(g_ev);
    r = _beginthread(explicit_end, 0, &g_tag);
    CHECK(r != (uintptr_t)-1); CHECK(r != 0);
    CHECK_EQ_U(WaitForSingleObject(g_ev, 2000), WAIT_OBJECT_0);
    CHECK_EQ_P(g_arg, &g_tag);
    Sleep(20); CHECK_EQ_I(g_after, 0);
}
TC_CASE(implicit_end)
{
    uintptr_t r; g_arg = NULL; ResetEvent(g_ev);
    r = _beginthread(fall_off, 0, &g_tag);
    CHECK(r != (uintptr_t)-1); CHECK(r != 0);
    CHECK_EQ_U(WaitForSingleObject(g_ev, 2000), WAIT_OBJECT_0);
    CHECK_EQ_P(g_arg, &g_tag);
}
int main(void)
{
    tc_begin(); g_ev = CreateEventA(NULL, TRUE, FALSE, NULL);
    TC_RUN(explicit_endthread); TC_RUN(implicit_end); return tc_end();
}
