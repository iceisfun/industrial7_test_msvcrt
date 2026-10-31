/* TC
 * id:    fpe_neg_fpreset_thread_local
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp
 * procs: _fpreset
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _controlfp / _fpreset; Win32 thread context (FP control state is per thread)
 * note:  Worker thread changes RC; main thread's control word must not change. After main's _fpreset the worker still has its own RC. Whether _fpreset restores RC on the calling thread is NOT asserted (not documented; logged). Worker's initial word is logged only.
 */
#include "tc.h"
#include <float.h>
#define ALLRC (_MCW_EM | _MCW_RC | _MCW_DN)
static void logx(const char *what, unsigned long long v) { char b[32]; tc_hex(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write("="); tc_write(b); tc_write("\n"); }

TC_DECL_PROC(void, _fpreset, (void));
static HANDLE ev_set, ev_go;
static volatile unsigned w_init, w_after_set, w_final;
static DWORD WINAPI worker(LPVOID p)
{
    (void)p;
    w_init = _controlfp(0, 0);
    _controlfp(_RC_CHOP, _MCW_RC);
    w_after_set = _controlfp(0, 0);
    SetEvent(ev_set);
    WaitForSingleObject(ev_go, 10000);
    w_final = _controlfp(0, 0);
    return 0;
}
TC_CASE(thread_isolation)
{
    HANDLE th; unsigned orig = _controlfp(0, 0);
    if (!TC_LOAD_PROC(_fpreset)) TC_SKIP("_fpreset not exported");
    ev_set = CreateEventA(NULL, TRUE, FALSE, NULL); ev_go = CreateEventA(NULL, TRUE, FALSE, NULL);
    _controlfp(_RC_DOWN, _MCW_RC);
    th = CreateThread(NULL, 0, worker, NULL, 0, NULL);
    CHECK(th != NULL);
    if (!th) return;
    CHECK_EQ_U(WaitForSingleObject(ev_set, 10000), WAIT_OBJECT_0);
    CHECK_EQ_U(w_after_set & _MCW_RC, _RC_CHOP);
    CHECK_EQ_U(_controlfp(0, 0) & _MCW_RC, _RC_DOWN);     /* worker change did not leak */
    tc_p__fpreset();
    logx("main_rc_after_reset", _controlfp(0, 0) & _MCW_RC);   /* logged only: x64 _fpreset scope varies by CRT */
    SetEvent(ev_go);
    CHECK_EQ_U(WaitForSingleObject(th, 10000), WAIT_OBJECT_0);
    CHECK_EQ_U(w_final & _MCW_RC, _RC_CHOP);              /* main's _fpreset did not reach worker */
    logx("worker_initial", w_init);
    CloseHandle(th); CloseHandle(ev_set); CloseHandle(ev_go);
    _controlfp(orig, ALLRC); _clearfp();
}
int main(void) { tc_begin(); TC_RUN(thread_isolation); return tc_end(); }
