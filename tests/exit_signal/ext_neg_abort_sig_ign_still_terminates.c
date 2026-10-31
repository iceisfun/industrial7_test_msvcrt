/* TC
 * id:    ext_neg_abort_sig_ign_still_terminates
 * area:  exit_signal
 * funcs: abort signal
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs abort (exit code 3 when abort behavior flags cleared; handler returning does not stop termination); C11 7.22.4.1
 * note:  Child ignores SIGABRT then calls abort(); process must still terminate (exit code 3 with
 *        report-fault/abort message disabled by tc_begin). Fatal path runs in a child.
 */
#include "tc.h"
#include <signal.h>
#include <stdlib.h>

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("ign", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK_EQ_U(code, 3);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        void (__cdecl *(__cdecl *sg)(int, void (__cdecl *)(int)))(int) = signal;
        sg(SIGABRT, SIG_IGN);
        abort();
        return 77;
    }
    TC_RUN(child_terminates);
    return tc_end();
}
