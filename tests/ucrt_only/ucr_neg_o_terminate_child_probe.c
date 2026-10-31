/* TC
 * id:    ucr_neg_o_terminate_child_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _o_terminate _set_abort_behavior
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   MS docs terminate (default terminate handler calls abort)
 * note:  Probes that calling the _o_terminate export directly (no active exception, default handler) ends the process with
 *        exit code 3 once _CALL_REPORTFAULT is cleared. The export may instead behave differently (e.g. fast-fail).
 */
#include "tc.h"

typedef void (__cdecl *v_t)(void);
typedef unsigned (__cdecl *sab_t)(unsigned, unsigned);

TC_CASE(terminate3) { DWORD c = 0; CHECK(tc_spawn_self("terminate", &c, 20000)); CHECK_EQ_U(c, 3); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        sab_t sab = (sab_t)tc_crt_proc("_set_abort_behavior"); v_t f = (v_t)tc_crt_proc("_o_terminate");
        SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS);
        if (sab) sab(0, 2);
        if (f && sab) f();
        return 77;
    }
    TC_RUN(terminate3);
    return tc_end();
}
