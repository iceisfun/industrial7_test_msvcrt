/* TC
 * id:    pfp_neg_fatal_n_default
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * procs: _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_s/_set_printf_count_output: %n is disabled by default in the UCRT; a %n conversion then calls the invalid parameter handler
 * note:  Fatal path in a child process. The exact termination status (0xC0000409) is probed in pfp_neg_fatal_n_status_probe.
 *        Child "n_handler": with a returning handler installed, sprintf with %n must return -1 and call the handler once.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("n_default", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);       /* 77 = sprintf returned normally */
    CHECK(code != 0);
}
TC_CASE(child_handler_returns)
{
    DWORD code = 0; int ran = tc_spawn_self("n_handler", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 55);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char b[32]; int n = 0; int r;
        volatile int *volatile pn = &n;
        if (c[2] == 'h') {   /* n_handler */
            tc_iph_install(); tc_iph_count = 0;
            r = sprintf(b, "ab%n", pn);
            return (r == -1 && tc_iph_count == 1) ? 55 : 56;
        }
        tc_iph_default();
        sprintf(b, "ab%n", pn);
        return 77;
    }
    TC_RUN(child_terminates); TC_RUN(child_handler_returns);
    return tc_end();
}
