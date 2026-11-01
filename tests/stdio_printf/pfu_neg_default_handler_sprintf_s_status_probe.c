/* TC
 * id:    pfu_neg_default_handler_sprintf_s_status_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s printf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs Parameter Validation; UCRT default handler calls __fastfail(FAST_FAIL_INVALID_ARG)
 * note:  Probing exactly: the exit status of a child terminated by the UCRT default invalid-parameter handler is 0xC0000409 (STATUS_STACK_BUFFER_OVERRUN via __fastfail). tc_begin's _set_abort_behavior does not change this.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(sprintf_s_status) { DWORD c = 0; CHECK(tc_spawn_self("sp", &c, 20000)); CHECK_EQ_U(c, TC_STATUS_FASTFAIL); }
TC_CASE(printf_s_null)    { DWORD c = 0; CHECK(tc_spawn_self("pf", &c, 20000)); CHECK_EQ_U(c, TC_STATUS_FASTFAIL); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char b[16]; volatile size_t two = 2;
        tc_iph_default(); /* mingw startup installs a no-op handler; restore the CRT default */
 const char *volatile nf = NULL;
        if (c[0] == 's') sprintf_s(b, two, "abc");
        else printf_s(nf);
        return 77;
    }
    TC_RUN(sprintf_s_status); TC_RUN(printf_s_null);
    return tc_end();
}
