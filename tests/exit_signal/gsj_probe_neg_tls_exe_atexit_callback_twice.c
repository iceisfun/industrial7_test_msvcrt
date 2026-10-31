/* TC
 * id:    gsj_probe_neg_tls_exe_atexit_callback_twice
 * area:  exit_signal
 * kind:  negative
 * funcs:
 * procs: _register_thread_local_exe_atexit_callback
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   none (undocumented)
 * note:  PROBING that registering a thread-local exe atexit callback twice is rejected (abnormal termination). The
 *        NULL-registration case is a pure guess; its exit code is only required to arrive within the timeout.
 *        Child processes only.
 */
#include "tc.h"

typedef void (__cdecl *tls_cb_t)(void *, unsigned long, void *);
TC_DECL_PROC(void, _register_thread_local_exe_atexit_callback, (tls_cb_t));

static void __cdecl cb(void *h, unsigned long reason, void *r) { (void)h; (void)reason; (void)r; }

TC_CASE(twice_does_not_return_normally)
{
    DWORD code = 0; int ran = tc_spawn_self("twice", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
}
TC_CASE(null_finishes_within_timeout)
{
    DWORD code = 0; int ran = tc_spawn_self("null", &code, 20000);
    CHECK(ran);   /* exit code itself is recorded by the runner only */
}
int main(void)
{
    const char *c;
    tc_begin(); TC_LOAD_PROC(_register_thread_local_exe_atexit_callback);
    if ((c = tc_child_name()) != NULL) {
        if (!tc_p__register_thread_local_exe_atexit_callback) return 78;
        if (c[0] == 't') { tc_p__register_thread_local_exe_atexit_callback(cb); tc_p__register_thread_local_exe_atexit_callback(cb); }
        else tc_p__register_thread_local_exe_atexit_callback(NULL);
        return 77;
    }
    TC_RUN(twice_does_not_return_normally); TC_RUN(null_finishes_within_timeout);
    return tc_end();
}
