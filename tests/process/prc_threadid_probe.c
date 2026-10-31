/* TC
 * id:    prc_threadid_probe
 * area:  process
 * funcs: _beginthreadex
 * procs: __threadid
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _threadid (macro over __threadid in the old CRT headers)
 * note:  PROBE: whether __threadid is exported and returns a pointer to the Win32 thread id (GetCurrentThreadId).
 *        Missing export => SKIP. Each probe runs in a child process because the behaviour of calling it is unverified
 *        (a crash would otherwise take the other cases with it); child exit 0 = ok, 10 = not exported, 11 = mismatch.
 */
#include "tc.h"
#include <process.h>

typedef unsigned long *(__cdecl *threadid_t)(void);
static threadid_t p_tid;
static volatile DWORD g_wtid, g_wwin;
static unsigned __stdcall worker(void *a) { (void)a; g_wtid = *p_tid(); g_wwin = GetCurrentThreadId(); return 0; }

static int child_main(const char *n)
{
    p_tid = (threadid_t)tc_crt_proc("__threadid");
    if (!p_tid) return 10;
    if (n[0] == 'm') return (p_tid() != NULL && *p_tid() == GetCurrentThreadId()) ? 0 : 11;
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
