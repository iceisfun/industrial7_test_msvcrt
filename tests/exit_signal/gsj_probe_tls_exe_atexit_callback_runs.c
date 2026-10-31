/* TC
 * id:    gsj_probe_tls_exe_atexit_callback_runs
 * area:  exit_signal
 * funcs:
 * procs: _register_thread_local_exe_atexit_callback
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   none (undocumented; used by the VC startup code for thread_local destructors in exes)
 * note:  PROBING that a callback registered with _register_thread_local_exe_atexit_callback is invoked with
 *        reason DLL_THREAD_DETACH(3) when a thread exits. Registration is once per process, hence its own exe.
 */
#include "tc.h"

typedef void (__cdecl *tls_cb_t)(void *, unsigned long, void *);
TC_DECL_PROC(void, _register_thread_local_exe_atexit_callback, (tls_cb_t));
static volatile LONG g_calls, g_detach;

static void __cdecl cb(void *h, unsigned long reason, void *r)
{
    (void)h; (void)r;
    InterlockedIncrement(&g_calls);
    if (reason == 3) InterlockedIncrement(&g_detach);
}
static DWORD WINAPI thr(LPVOID a) { (void)a; return 0; }

TC_CASE(exported) { CHECK(tc_p__register_thread_local_exe_atexit_callback != NULL); }
TC_CASE(invoked_on_thread_detach)
{
    HANDLE h;
    if (!tc_p__register_thread_local_exe_atexit_callback) TC_SKIP("missing");
    tc_p__register_thread_local_exe_atexit_callback(cb);
    h = CreateThread(NULL, 0, thr, NULL, 0, NULL);
    CHECK(h != NULL);
    if (h) { WaitForSingleObject(h, 10000); CloseHandle(h); }
    Sleep(100); /* detach notification may run slightly after the handle is signalled */
    CHECK(g_calls >= 1);
    CHECK(g_detach >= 1);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_register_thread_local_exe_atexit_callback);
    TC_RUN(exported); TC_RUN(invoked_on_thread_detach);
    return tc_end();
}
