/* TC
 * id:    pfi_neg_sprintf_s_overflow_fatal
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs sprintf_s: buffer too small invokes the invalid parameter handler (default terminates)
 * note:  Exact status 0xC0000409 (fast-fail) is observed Win10 behaviour; docs only say the process terminates. The child first restores the CRT's
 *        default invalid-parameter handler with tc_iph_default(): the mingw-w64 startup code installs a no-op handler, which otherwise makes the
 *        child survive (exit 77).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(overflow_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("overflow", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char b[2]; volatile int v = 12345;
        tc_iph_default();
        sprintf_s(b, sizeof b, "%d", v);
        return 77;
    }
    TC_RUN(overflow_terminates);
    return tc_end();
}
