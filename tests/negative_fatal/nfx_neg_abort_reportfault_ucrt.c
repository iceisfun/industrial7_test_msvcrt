/* TC
 * id:    nfx_neg_abort_reportfault_ucrt
 * area:  negative_fatal
 * kind:  negative
 * funcs: abort
 * procs: _set_abort_behavior
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _set_abort_behavior: _CALL_REPORTFAULT
 * note:  PROBE: with _CALL_REPORTFAULT set, abort() on ucrtbase fast-fails with 0xC0000409.
 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>

typedef unsigned int (__cdecl *sab_t)(unsigned int, unsigned int);
TC_CASE(fastfail)
{
    DWORD code = 0; int ran = tc_spawn_self("rf", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) {
        sab_t sab = (sab_t)tc_crt_proc("_set_abort_behavior");
        if (!sab) return 78;
        sab(2, 2); abort(); return 77;
    }
    TC_RUN(fastfail);
    return tc_end();
}
