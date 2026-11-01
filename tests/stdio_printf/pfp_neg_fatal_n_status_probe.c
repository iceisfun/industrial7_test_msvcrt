/* TC
 * id:    pfp_neg_fatal_n_status_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _set_invalid_parameter_handler (default handler terminates); __fastfail
 * note:  PROBE: exit status of a process that hits the default invalid-parameter handler (sprintf with %n,
 *        disabled by default) is 0xC0000409 (STATUS_STACK_BUFFER_OVERRUN via __fastfail). Not formally documented.
 *        Win10 22H2 fix: child calls tc_iph_default() first (mingw crtexe installs a no-op handler).
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(child_status)
{
    DWORD code = 0; int ran = tc_spawn_self("n_default", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        char b[32]; int n = 0;
        tc_iph_default();
        volatile int *volatile pn = &n;
        sprintf(b, "ab%n", pn);
        return 77;
    }
    TC_RUN(child_status);
    return tc_end();
}
