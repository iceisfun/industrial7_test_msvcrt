/* TC
 * id:    nfx_neg_raise_default_other_signals_probe
 * area:  negative_fatal
 * funcs: raise
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs raise (default action terminates, exit code 3); C11 7.14.2.1
 * note:  Probes: raise(sig) with SIG_DFL for SIGINT/SIGBREAK/SIGSEGV/SIGILL/SIGFPE terminates the process with exit code 3. Per-signal behaviour unverified.
 */
#include "tc.h"
#include <signal.h>

static volatile int v_sig;
static int run(const char *n) { DWORD c = 0; if (!tc_spawn_self(n, &c, 20000)) return -1; return (int)c; }

TC_CASE(sigint)   { CHECK_EQ_I(run("int"), 3); }
TC_CASE(sigbreak) { CHECK_EQ_I(run("brk"), 3); }
TC_CASE(sigsegv)  { CHECK_EQ_I(run("segv"), 3); }
TC_CASE(sigill)   { CHECK_EQ_I(run("ill"), 3); }
TC_CASE(sigfpe)   { CHECK_EQ_I(run("fpe"), 3); }

int main(void)
{
    const char *n;
    tc_begin();
    if ((n = tc_child_name()) != NULL) {
        v_sig = n[0] == 'i' && n[1] == 'n' ? SIGINT : n[0] == 'b' ? SIGBREAK : n[0] == 's' ? SIGSEGV
              : n[0] == 'i' ? SIGILL : SIGFPE;
        signal(v_sig, SIG_DFL);
        raise(v_sig);
        return 77;
    }
    TC_RUN(sigint); TC_RUN(sigbreak); TC_RUN(sigsegv); TC_RUN(sigill); TC_RUN(sigfpe);
    return tc_end();
}
