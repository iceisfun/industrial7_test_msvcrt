/* TC
 * id:    scn_neg_sscanf_s_default_handler_fatal
 * area:  scanf
 * kind:  negative
 * funcs: sscanf_s
 * crt:   >=140
 * tier:  p0
 * conf:  observed
 * ref:   MS docs Parameter validation: without a handler an invalid parameter terminates the process
 * note:  Probes the exact termination status (expected fast-fail 0xC0000409 on ucrtbase) when sscanf_s gets a NULL format with the default handler. Child process.
 *        Win10 22H2 ucrtbase (observed 2026-10-31, stable over 2 runs): the mingw-w64 startup installs a silent invalid-parameter handler, so the child now calls tc_iph_default() first to reach the real UCRT default handler; it then dies with 0xC0000409 (fast-fail).
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile nf;
TC_CASE(child_terminates) { DWORD code = 0; int ran = tc_spawn_self("nullfmt", &code, 20000); CHECK(ran); CHECK(code != 0); CHECK(code != 77); }
TC_CASE(child_fastfail) { DWORD code = 0; int ran = tc_spawn_self("nullfmt", &code, 20000); CHECK(ran); CHECK_EQ_U(code, TC_STATUS_FASTFAIL); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) { int x; tc_iph_default(); sscanf_s("1", nf, &x); return 77; }
    TC_RUN(child_terminates); TC_RUN(child_fastfail);
    return tc_end();
}
