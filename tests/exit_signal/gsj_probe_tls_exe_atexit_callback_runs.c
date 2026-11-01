/* TC
 * id:    gsj_probe_tls_exe_atexit_callback_runs
 * area:  exit_signal
 * funcs:
 * procs: _register_thread_local_exe_atexit_callback
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   none (undocumented; used by the VC startup code for thread_local destructors in exes)
 * note:  Class A (test bug: the old expectation was wrong). OBSERVED on Win10 22H2 19045 ucrtbase (see ledger for run count): a callback
 *        registered with _register_thread_local_exe_atexit_callback is NOT invoked when a thread exits (thread-detach destructors in an
 *        MSVC exe come from the exe's own TLS callback, not from the UCRT); the UCRT invokes it once from exit() with reason
 *        DLL_PROCESS_DETACH (0). The child registers the callback, runs a thread to completion, then calls exit(0); the callback
 *        records what it saw and ends the process with ExitProcess(200 + reason + 10 * calls_before_this_one). Expected child code 200.
 *        Registration is once per process, hence the child process. The probe name is kept for id stability.
 */
#include "tc.h"
#include <stdlib.h>

typedef void (__cdecl *tls_cb_t)(void *, unsigned long, void *);
TC_DECL_PROC(void, _register_thread_local_exe_atexit_callback, (tls_cb_t));
static volatile LONG g_calls;

static void __cdecl cb(void *h, unsigned long reason, void *r)
{
    LONG before = InterlockedIncrement(&g_calls) - 1;
    (void)h; (void)r;
    ExitProcess(200 + (unsigned)(reason & 7) + 10 * (unsigned)before);
}
static DWORD WINAPI thr(LPVOID a) { (void)a; return 0; }

static int child_main(void)
{
    HANDLE h;
    if (!tc_p__register_thread_local_exe_atexit_callback) return 90;
    tc_p__register_thread_local_exe_atexit_callback(cb);
    h = CreateThread(NULL, 0, thr, NULL, 0, NULL);
    if (!h) return 91;
    WaitForSingleObject(h, 10000); CloseHandle(h);
    Sleep(100);
    if (g_calls != 0) return 92;   /* callback ran on thread exit */
    exit(0);                       /* the callback must run here and end the process with 200 */
    return 93;
}

TC_CASE(exported) { CHECK(tc_p__register_thread_local_exe_atexit_callback != NULL); }
TC_CASE(runs_at_exit_not_on_thread_detach)
{
    DWORD code = 0; int ran;
    if (!tc_p__register_thread_local_exe_atexit_callback) TC_SKIP("missing");
    ran = tc_spawn_self("exit", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 200u);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_register_thread_local_exe_atexit_callback);
    if (tc_child_name()) return child_main();
    TC_RUN(exported); TC_RUN(runs_at_exit_not_on_thread_detach);
    return tc_end();
}
