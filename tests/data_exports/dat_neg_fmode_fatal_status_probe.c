/* TC
 * id:    dat_neg_fmode_fatal_status_probe
 * area:  data_exports
 * funcs: _get_fmode _set_fmode
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs "Parameter validation"; STATUS_STACK_BUFFER_OVERRUN / __fastfail
 * note:  PROBE: exact exit status of a child that hits the default invalid-parameter handler on Windows 10 ucrtbase.
 *        Expected 0xC0000409 (__fastfail(FAST_FAIL_INVALID_ARG)); could be 0xC0000417 on some configurations.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(get_null_status)
{
    DWORD a = 0; CHECK(tc_spawn_self("getnull", &a, 20000));
    CHECK_EQ_U(a, TC_STATUS_FASTFAIL);
}
TC_CASE(set_bad_status)
{
    DWORD b = 0; CHECK(tc_spawn_self("setbad", &b, 20000));
    CHECK_EQ_U(b, TC_STATUS_FASTFAIL);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        int *volatile p = NULL; volatile int bad = 0x1234;
        if (c[0] == 'g') _get_fmode(p); else _set_fmode(bad);
        return 77;
    }
    TC_RUN(get_null_status); TC_RUN(set_bad_status);
    return tc_end();
}
