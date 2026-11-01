/* TC
 * id:    prc_threadid_probe
 * area:  process
 * funcs: _beginthreadex
 * procs: __threadid
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _threadid (macro over __threadid in the old CRT headers)
 * note:  Observed on Win10 22H2 19045 ucrtbase: __threadid IS exported but returns the Win32 thread id by value (probe printed
 *        0x120 == GetCurrentThreadId() == 288), NOT a pointer to it; the old pointer-returning typedef dereferenced the id
 *        (0xC0000005). Test now compares the returned value with GetCurrentThreadId() on the main and on a _beginthreadex
 *        thread.
 */
#include "tc.h"
#include <process.h>

typedef unsigned long (__cdecl *threadid_t)(void);   /* observed: ucrtbase __threadid returns the thread id by value, not a pointer */
static threadid_t p_tid;
static volatile DWORD g_wtid, g_wwin;
static unsigned __stdcall worker(void *a) { (void)a; g_wtid = p_tid(); g_wwin = GetCurrentThreadId(); return 0; }

static int child_main(const char *n)
{
    p_tid = (threadid_t)tc_crt_proc("__threadid");
    if (!p_tid) return 10;
    if (n[0] == 'm') return (p_tid() == GetCurrentThreadId()) ? 0 : 11;
    {
        unsigned id; HANDLE h = (HANDLE)_beginthreadex(NULL, 0, worker, NULL, 0, &id);
        if (!h) return 12;
        WaitForSingleObject(h, 5000); CloseHandle(h);
        return (g_wtid != GetCurrentThreadId() && g_wtid == g_wwin) ? 0 : 11;
    }
}
static void probe(const char *name)
{
    DWORD c = 0xffff;
    int ran = tc_spawn_self(name, &c, 20000);
    CHECK(ran);
    if (c == 10) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID "::"); tc_write(tc_case_name); tc_write(" __threadid not exported\n"); return; }
    CHECK_EQ_U(c, 0);
}
TC_CASE(main_thread) { probe("main"); }
TC_CASE(worker_thread) { probe("worker"); }

int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(main_thread); TC_RUN(worker_thread); return tc_end();
}
