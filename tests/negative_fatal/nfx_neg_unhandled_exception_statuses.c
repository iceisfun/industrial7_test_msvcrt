/* TC
 * id:    nfx_neg_unhandled_exception_statuses
 * area:  negative_fatal
 * funcs: signal
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   NTSTATUS values; SetErrorMode SEM_NOGPFAULTERRORBOX
 * note:  Probes: with no handler, an unhandled AV / int divide-by-zero / ud2 terminates the child with the NT status as exit code (0xC0000005 / 0xC0000094 / 0xC000001D). This is OS behaviour, not CRT; machine-dependent if a JIT debugger (AeDebug) or WER policy is configured, hence probe. Children install no handler.
 */
#include "tc.h"
#include <signal.h>

static volatile int *volatile p;
static volatile int v_a, v_b, v_r;

TC_CASE(null_write) { DWORD c = 0; CHECK(tc_spawn_self("av", &c, 20000)); CHECK_EQ_U(c, 0xC0000005u); }
TC_CASE(int_div0)   { DWORD c = 0; CHECK(tc_spawn_self("div", &c, 20000)); CHECK_EQ_U(c, 0xC0000094u); }
TC_CASE(ud2)        { DWORD c = 0; CHECK(tc_spawn_self("ud2", &c, 20000)); CHECK_EQ_U(c, 0xC000001Du); }

int main(void)
{
    const char *n;
    tc_begin();
    if ((n = tc_child_name()) != NULL) {
        signal(SIGTERM, SIG_DFL);
        if (n[0] == 'a') { p = NULL; *p = 1; }
        else if (n[0] == 'd') { v_a = 1; v_b = 0; v_r = v_a / v_b; }
        else __builtin_trap();
        return 77;
    }
    TC_RUN(null_write); TC_RUN(int_div0); TC_RUN(ud2);
    return tc_end();
}
