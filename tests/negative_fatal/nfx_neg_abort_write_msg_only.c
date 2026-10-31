/* TC
 * id:    nfx_neg_abort_write_msg_only
 * area:  negative_fatal
 * kind:  negative
 * funcs: abort
 * procs: _set_abort_behavior
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_abort_behavior: _WRITE_ABORT_MSG; abort exit code 3
 * note:  Child enables only _WRITE_ABORT_MSG (console app, message goes to stderr).
 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>

typedef unsigned int (__cdecl *sab_t)(unsigned int, unsigned int);
TC_CASE(msg_only)
{
    DWORD code = 0; int ran = tc_spawn_self("wm", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK_EQ_U(code, 3);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) {
        sab_t sab = (sab_t)tc_crt_proc("_set_abort_behavior");
        if (!sab) return 78;
        sab(1, 3); abort(); return 77;
    }
    TC_RUN(msg_only);
    return tc_end();
}
