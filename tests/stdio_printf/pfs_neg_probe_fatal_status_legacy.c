/* TC
 * id:    pfs_neg_probe_fatal_status_legacy
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * crt:   msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs Parameter validation; STATUS_INVALID_CRUNTIME_PARAMETER
 * note:  PROBE: exit status of the default invalid-parameter path on VS2010-2013 CRTs. msvcr100 is expected to give 0xC0000417
 *        (STATUS_INVALID_CRUNTIME_PARAMETER); msvcr110/120 may use __fastfail (0xC0000409) instead. Either is accepted
 *        (anything else fails); the actual code is printed as TC INFO.
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile sfmt = "%s";

TC_CASE(overflow_status)
{
    DWORD code = 0; CHECK(tc_spawn_self("overflow", &code, 20000));
    CHECK(code == TC_STATUS_INVALID_CRT_PARAM || code == TC_STATUS_FASTFAIL);
    { char n[32]; tc_write("TC INFO exit="); tc_hex(code, n); tc_write(n); tc_write("\n"); }
}
int main(void)
{
    char b[2];
    tc_begin();
    if (tc_child_name() != NULL) { sprintf_s(b, 2, sfmt, "abc"); return 77; }
    TC_RUN(overflow_status);
    return tc_end();
}
