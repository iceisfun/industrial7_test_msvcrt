/* TC
 * id:    pfp_neg_fatal_bad_conv
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * procs: _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation", printf type field characters: an invalid format string invokes the invalid parameter handler
 * note:  Child-process tests. Documented: '%y' (unknown conversion) terminates under the default handler and returns -1 under a returning handler.
 *        Trailing lone '%' is covered by the probe test pfp_neg_trailing_percent.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(unknown_conv_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("y_default", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK(code != 0);
}
TC_CASE(unknown_conv_handler_returns)
{
    DWORD code = 0; int ran = tc_spawn_self("y_handler", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 55);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char b[32]; int r;
        const char *volatile fy = "%y";
        const char *f = fy;
        if (c[2] == 'h') {   /* *_handler */
            tc_iph_install(); tc_iph_count = 0;
            r = sprintf(b, f);
            return (r < 0 && tc_iph_count == 1) ? 55 : 56;
        }
        sprintf(b, f);
        return 77;
    }
    TC_RUN(unknown_conv_terminates); TC_RUN(unknown_conv_handler_returns);
    return tc_end();
}
