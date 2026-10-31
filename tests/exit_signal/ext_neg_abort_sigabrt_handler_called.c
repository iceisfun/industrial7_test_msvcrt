/* TC
 * id:    ext_neg_abort_sigabrt_handler_called
 * area:  exit_signal
 * funcs: abort signal _exit
 * procs: _set_abort_behavior
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   C11 7.22.4.1 abort raises SIGABRT; MS docs abort: handler may run, process still terminates with 3
 * note:  Child processes. Second case: handler returns, abort must not return; exit code 3 with Watson disabled (documented for abort).
 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>
#include <process.h>

typedef unsigned int (__cdecl *sab_t)(unsigned int, unsigned int);

static void __cdecl h_exit(int sig) { _exit(sig == SIGABRT ? 42 : 43); }
static void __cdecl h_ret(int sig)  { (void)sig; }

TC_CASE(handler_exits)
{ DWORD c = 0; CHECK(tc_spawn_self("hx", &c, 20000)); CHECK_EQ_U(c, 42); }
TC_CASE(handler_returns)
{ DWORD c = 0; CHECK(tc_spawn_self("hr", &c, 20000)); CHECK_EQ_U(c, 3); }

int main(void)
{
    const char *n;
    tc_begin();
    if ((n = tc_child_name()) != NULL) {
        sab_t sab = (sab_t)tc_crt_proc("_set_abort_behavior");
        if (sab) sab(0, 0x2 | 0x1);
        SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
        if (signal(SIGABRT, n[1] == 'x' ? h_exit : h_ret) == SIG_ERR) return 79;
        abort();
        return 77;
    }
    TC_RUN(handler_exits); TC_RUN(handler_returns);
    return tc_end();
}
