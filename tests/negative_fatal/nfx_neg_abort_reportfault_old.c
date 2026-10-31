/* TC
 * id:    nfx_neg_abort_reportfault_old
 * area:  negative_fatal
 * kind:  negative
 * funcs: abort
 * procs: _set_abort_behavior
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _set_abort_behavior
 * note:  PROBE: exact status with _CALL_REPORTFAULT on old CRTs unknown (0xC0000409/0x40000015/3); only assert abnormal termination: != 0 and != 77.
 */
#include "tc.h"
#include <stdlib.h>
#include <signal.h>

typedef unsigned int (__cdecl *sab_t)(unsigned int, unsigned int);
TC_CASE(terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("rfo", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
    CHECK(code != 78);
}
int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) {
        sab_t sab = (sab_t)tc_crt_proc("_set_abort_behavior");
        if (!sab) return 78;
        sab(2, 2); abort(); return 77;
    }
    TC_RUN(terminates);
    return tc_end();
}
