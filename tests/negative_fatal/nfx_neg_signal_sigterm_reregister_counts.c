/* TC
 * id:    nfx_neg_signal_sigterm_reregister_counts
 * area:  negative_fatal
 * kind:  negative
 * funcs: signal raise
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.14.1.1, 7.14.2.1; MS docs signal, raise
 * note:  A handler that re-registers itself is called once per raise regardless of whether the implementation resets the disposition.
 *        The negative counterpart (no re-registration) is nfx_neg_signal_sigterm_handler_reset_after_call.
 */
#include "tc.h"
#include <signal.h>

static volatile int calls;
static void __cdecl h(int s) { calls++; signal(s, h); }

TC_CASE(reregistering_handler_called_twice)
{
    DWORD code = 99; int ran = tc_spawn_self("rereg", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        if (signal(SIGTERM, h) == SIG_ERR) return 1;
        if (raise(SIGTERM) != 0) return 2;
        if (raise(SIGTERM) != 0) return 3;
        return calls == 2 ? 0 : 60 + calls;
    }
    TC_RUN(reregistering_handler_called_twice);
    return tc_end();
}
