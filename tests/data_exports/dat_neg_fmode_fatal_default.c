/* TC
 * id:    dat_neg_fmode_fatal_default
 * area:  data_exports
 * funcs: _get_fmode _set_fmode
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs "Parameter validation": with the default handler an invalid parameter terminates the process
 * note:  Child processes call _get_fmode(NULL) / _set_fmode(0x1234) with the default invalid-parameter handler.
 *        Asserted: abnormal termination (nonzero, not the child's own 77 fall-through). The exact ucrtbase status
 *        is in dat_neg_fmode_fatal_status_probe.
 */
#include "tc.h"
#include <stdlib.h>

static void chk(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}
TC_CASE(child_get_null_terminates) { chk("getnull"); }
TC_CASE(child_set_bad_terminates)  { chk("setbad"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        int *volatile p = NULL; volatile int bad = 0x1234; int m;
        if (c[0] == 'g') _get_fmode(p); else _set_fmode(bad);
        (void)m;
        return 77;
    }
    TC_RUN(child_get_null_terminates); TC_RUN(child_set_bad_terminates);
    return tc_end();
}
