/* TC
 * id:    pfp_neg_fatal_bad_conv
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf sprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p0
 * conf:  observed
 * ref:   MS docs "Parameter validation", printf type field characters: an invalid format string invokes the invalid parameter handler
 * note:  Observed on Windows 10 22H2 ucrtbase: format validation (unknown conversion such as '%y') exists only in the secure printf family. sprintf_s("%y")
 *        terminates under the default handler with 0xC0000409 and returns -1 / EINVAL under a returning handler; plain sprintf("%y") is lenient: it
 *        prints the character ("y", returns 1), never calls the handler and does not terminate. Children first call tc_iph_default() (the mingw-w64
 *        startup installs a no-op handler). The earlier expectation that plain sprintf terminates was wrong.
 */
#include "tc.h"
#include <stdio.h>
/* sprintf_s is an inline over __stdio_common_vsprintf_s in the UCRT headers (not exported from ucrtbase) */
TC_CASE(secure_unknown_conv_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("ys_default", &code, 20000);
    CHECK(ran);
    CHECK(code != 77);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(secure_unknown_conv_handler_returns)
{
    DWORD code = 0; int ran = tc_spawn_self("ys_handler", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 55);
}
TC_CASE(plain_unknown_conv_is_lenient)
{
    DWORD code = 0; int ran = tc_spawn_self("y_default", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 77);   /* default handler never invoked: the call returned */
}
TC_CASE(plain_unknown_conv_prints_char)
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
        tc_iph_default();
        if (c[0] == 'y' && c[1] == 's') {
            if (c[3] == 'h') {   /* ys_handler */
                tc_iph_install(); tc_iph_count = 0;
                r = sprintf_s(b, sizeof b, f);
                return (r < 0 && tc_iph_count == 1) ? 55 : 56;
            }
            sprintf_s(b, sizeof b, f);
            return 77;
        }
        if (c[2] == 'h') {   /* y_handler: plain sprintf, handler installed but never called */
            tc_iph_install(); tc_iph_count = 0;
            r = sprintf(b, f);
            return (r == 1 && b[0] == 'y' && b[1] == 0 && tc_iph_count == 0) ? 55 : 56;
        }
        sprintf(b, f);
        return 77;
    }
    TC_RUN(secure_unknown_conv_terminates); TC_RUN(secure_unknown_conv_handler_returns);
    TC_RUN(plain_unknown_conv_is_lenient); TC_RUN(plain_unknown_conv_prints_char);
    return tc_end();
}
