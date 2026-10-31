/* TC
 * id:    prc_beginthreadex_null_thrdaddr
 * area:  process
 * funcs: _beginthreadex
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _beginthreadex (security, stack_size, thrdaddr optional)
 */
#include "tc.h"
#include <process.h>

static unsigned __stdcall plain(void *a) { (void)a; return 5; }
static unsigned __stdcall bigstack(void *a)
{
    volatile char buf[100 * 1024]; size_t i; unsigned sum = 0;
    for (i = 0; i < sizeof buf; i += 4096) buf[i] = (char)(i / 4096 + 1);
    for (i = 0; i < sizeof buf; i += 4096) sum += (unsigned char)buf[i];
    (void)a; return sum == 325u ? 9 : 1;   /* 25 pages: 1+2+...+25 */
}
static DWORD join(HANDLE h) { DWORD c = 0xdeadbeef; WaitForSingleObject(h, 5000); GetExitCodeThread(h, &c); CloseHandle(h); return c; }

TC_CASE(all_null) { HANDLE h = (HANDLE)_beginthreadex(NULL, 0, plain, NULL, 0, NULL); CHECK(h != NULL); if (h) CHECK_EQ_U(join(h), 5); }
TC_CASE(explicit_stack) { HANDLE h = (HANDLE)_beginthreadex(NULL, 256 * 1024, bigstack, NULL, 0, NULL); CHECK(h != NULL); if (h) CHECK_EQ_U(join(h), 9); }
TC_CASE(initflag_zero_runs) { HANDLE h = (HANDLE)_beginthreadex(NULL, 0, plain, NULL, 0, NULL); CHECK(h != NULL); if (!h) return; CHECK_EQ_U(WaitForSingleObject(h, 5000), WAIT_OBJECT_0); CloseHandle(h); }

int main(void) { tc_begin(); TC_RUN(all_null); TC_RUN(explicit_stack); TC_RUN(initflag_zero_runs); return tc_end(); }
