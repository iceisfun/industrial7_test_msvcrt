/* TC
 * id:    pfw_neg_fatal_default_iph_legacy
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs "Parameter Validation" (VS2005+): with the default handler an invalid parameter ends the process via Watson
 * note:  Probes that the default invalid-parameter handling of the msvcr80-120 wide printf family terminates the
 *        process abnormally (exit code neither 0 nor the 77 "survived" sentinel). The exact status
 *        (0xC0000417 / 0xC0000409 / an unhandled exception code) differs by CRT/OS and is only recorded
 *        loosely: it must be a 0xC000xxxx-style failure status or at least nonzero. A control child proves the
 *        spawn mechanism reports ordinary exit codes. Fatal paths run in child processes.
 */
#include "tc.h"
#include <wchar.h>
#include <stdio.h>

static int is_abnormal(DWORD code) { return code != 0 && code != 77 && code != 5 && code < 0xdead0000u; }

TC_CASE(control_child_exit_code)
{
    DWORD code = 0; int ran = tc_spawn_self("control", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 5);
}
TC_CASE(child_snwprintf_null_fmt)
{
    DWORD code = 0; int ran = tc_spawn_self("nullfmt", &code, 20000);
    CHECK(ran);
    CHECK(is_abnormal(code));
    CHECK(code == TC_STATUS_INVALID_CRT_PARAM || code == TC_STATUS_FASTFAIL || (code & 0xC0000000u) == 0xC0000000u);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        wchar_t buf[16]; wchar_t *volatile fmt = NULL;
        if (c[0] == 'c') return 5;
        if (c[0] == 'n') { _snwprintf(buf, 16, fmt); return 77; }
        return 78;
    }
    TC_RUN(control_child_exit_code); TC_RUN(child_snwprintf_null_fmt);
    return tc_end();
}
