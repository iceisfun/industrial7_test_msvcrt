/* TC
 * id:    pfu_neg_default_handler_sprintf_s_status_old_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs Parameter Validation
 * note:  Probing: exit status of a child killed by the pre-UCRT default invalid-parameter handler is 0xC0000417 (STATUS_INVALID_CRUNTIME_PARAMETER) or 0xC0000409 (fast fail; host-OS dependent). Either is accepted; anything else, 0 or 77 fails.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(status)
{
    DWORD c = 0;
    CHECK(tc_spawn_self("sp", &c, 20000));
    CHECK(c == TC_STATUS_INVALID_CRT_PARAM || c == TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) { char b[16]; volatile size_t two = 2; sprintf_s(b, two, "abc"); return 77; }
    TC_RUN(status);
    return tc_end();
}
