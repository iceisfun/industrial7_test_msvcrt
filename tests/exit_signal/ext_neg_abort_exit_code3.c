/* TC
 * id:    ext_neg_abort_exit_code3
 * area:  exit_signal
 * funcs: abort
 * procs: _set_abort_behavior
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs abort: exit code 3 (CALL_REPORTFAULT off); MS docs _set_abort_behavior
 * note:  Child process; _set_abort_behavior(0,_CALL_REPORTFAULT) so no Watson/WER, SetErrorMode as well.
 */
#include "tc.h"
#include <stdlib.h>

typedef unsigned int (__cdecl *sab_t)(unsigned int, unsigned int);

TC_CASE(reportfault_off)
{ DWORD c = 0; CHECK(tc_spawn_self("a1", &c, 20000)); CHECK_EQ_U(c, 3); }
TC_CASE(no_message_either)
{ DWORD c = 0; CHECK(tc_spawn_self("a2", &c, 20000)); CHECK_EQ_U(c, 3); }

int main(void)
{
    const char *n;
    tc_begin();
    if ((n = tc_child_name()) != NULL) {
        sab_t sab = (sab_t)tc_crt_proc("_set_abort_behavior");
        if (!sab) return 78;
        SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
        if (n[1] == '1') sab(0, 0x2);            /* _CALL_REPORTFAULT off */
        else             sab(0, 0x2 | 0x1);      /* also _WRITE_ABORT_MSG off */
        abort();
        return 77;
    }
    TC_RUN(reportfault_off); TC_RUN(no_message_either);
    return tc_end();
}
