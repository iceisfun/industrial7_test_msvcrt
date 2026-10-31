/* TC
 * id:    ucr_neg_o_exit_abort_child
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _o_exit _o_abort _set_abort_behavior
 * crt:   win10
 * tier:  p1
 * conf:  documented
 * ref:   MS docs exit (status becomes process exit code); abort (exit code 3 when _CALL_REPORTFAULT is not set); _set_abort_behavior
 * note:  The child clears _CALL_REPORTFAULT (0x2) with _set_abort_behavior(0, 2) so abort() ends via exit code 3 rather than
 *        the default fast-fail/WER path. The default path is covered by ucr_neg_abort_default_exitcode_probe.
 */
#include "tc.h"

typedef void (__cdecl *vi_t)(int);
typedef void (__cdecl *v_t)(void);
typedef unsigned (__cdecl *sab_t)(unsigned, unsigned);

TC_CASE(exit7) { DWORD c = 0; CHECK(tc_spawn_self("exit", &c, 20000)); CHECK_EQ_U(c, 7); }
TC_CASE(abort3) { DWORD c = 0; CHECK(tc_spawn_self("abort", &c, 20000)); CHECK_EQ_U(c, 3); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        sab_t sab = (sab_t)tc_crt_proc("_set_abort_behavior");
        SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS);
        if (tc_strequ(c, "exit")) { vi_t f = (vi_t)tc_crt_proc("_o_exit"); if (f) f(7); }
        else if (tc_strequ(c, "abort")) { v_t f = (v_t)tc_crt_proc("_o_abort"); if (sab) sab(0, 2); if (f && sab) f(); }
        return 77;
    }
    TC_RUN(exit7); TC_RUN(abort3);
    return tc_end();
}
