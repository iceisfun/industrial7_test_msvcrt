/* TC
 * id:    prc_endthreadex_code
 * area:  process
 * funcs: _beginthreadex _endthreadex
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _endthread, _endthreadex
 */
#include "tc.h"
#include <process.h>

static volatile LONG g_after;
static void __declspec(noinline) nested(unsigned c) { _endthreadex(c); InterlockedExchange(&g_after, 1); }
static unsigned __stdcall w1(void *a) { (void)a; nested(0x1234); InterlockedExchange(&g_after, 2); return 99; }
static unsigned __stdcall w2(void *a) { (void)a; _endthreadex(0xFFFFFFFEu); return 99; }
static DWORD run(unsigned (__stdcall *f)(void *))
{
    DWORD c = 0xdeadbeef; HANDLE h = (HANDLE)_beginthreadex(NULL, 0, f, NULL, 0, NULL);
    if (!h) return 0xdeadbeef;
    WaitForSingleObject(h, 5000); GetExitCodeThread(h, &c); CloseHandle(h); return c;
}
TC_CASE(nested_code) { CHECK_EQ_U(run(w1), 0x1234); CHECK_EQ_I(g_after, 0); }
TC_CASE(unsigned_round_trip) { CHECK_EQ_U(run(w2), 0xFFFFFFFEu); }

int main(void) { tc_begin(); TC_RUN(nested_code); TC_RUN(unsigned_round_trip); return tc_end(); }
