/* TC
 * id:    err_doserrno_thread_local
 * area:  errno_param
 * funcs: 
 * procs: _get_doserrno _set_doserrno
 * crt:   >=100
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _get_doserrno
 * note:  PROBE: _doserrno is stored in per-thread data like errno (worker's write does not affect main).
 */
#include "tc.h"

TC_DECL_PROC(int, _get_doserrno, (unsigned long *));
TC_DECL_PROC(int, _set_doserrno, (unsigned long));
static volatile unsigned long w_val;

static DWORD WINAPI worker(LPVOID p)
{
    unsigned long v = 0; (void)p;
    tc_p__set_doserrno(123); tc_p__get_doserrno(&v); w_val = v; return 0;
}
TC_CASE(isolated)
{
    unsigned long v = 0; HANDLE h;
    tc_p__set_doserrno(5);
    h = CreateThread(NULL, 0, worker, NULL, 0, NULL);
    CHECK(h != NULL);
    if (h) { WaitForSingleObject(h, 10000); CloseHandle(h); }
    CHECK_EQ_U(w_val, 123);
    tc_p__get_doserrno(&v); CHECK_EQ_U(v, 5);
    tc_p__set_doserrno(0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_get_doserrno); TC_LOAD_PROC(_set_doserrno);
    if (!tc_p__get_doserrno || !tc_p__set_doserrno) { tc_write("TC FAIL " TC_TEST_ID "::__setup\n"); return 1; }
    TC_RUN(isolated);
    return tc_end();
}
