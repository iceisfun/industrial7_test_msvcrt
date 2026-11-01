/* TC
 * id:    pfs_neg_probe_fatal_status_ucrt
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s sprintf
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs Parameter validation; __fastfail
 * note:  PROBE: exact exit status of the UCRT default invalid-parameter path: expected 0xC0000409 (STATUS_STACK_BUFFER_OVERRUN,
 *        fast fail) for both a sprintf_s overflow and a sprintf NULL format. Measure on Windows 10/11.
 *        Win10 22H2 fix: child calls tc_iph_default() first (mingw crtexe installs a no-op handler).
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile nullfmt = NULL;
static const char * volatile sfmt = "%s";

TC_CASE(overflow_status)
{
    DWORD code = 0; CHECK(tc_spawn_self("overflow", &code, 20000));
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(nullfmt_status)
{
    DWORD code = 0; CHECK(tc_spawn_self("nullfmt", &code, 20000));
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    const char *c; char b[2];
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        tc_iph_default();
        if (c[0] == 'o') sprintf_s(b, 2, sfmt, "abc");
        else sprintf(b, nullfmt);
        return 77;
    }
    TC_RUN(overflow_status); TC_RUN(nullfmt_status);
    return tc_end();
}
