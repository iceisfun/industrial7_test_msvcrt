/* TC
 * id:    pfr_neg_probe_default_handler_exit_code
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: sprintf_s
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _invalid_parameter, _set_invalid_parameter_handler
 * note:  ucrtbase does not export sprintf_s (inline), so only msvcr80-120 targets. PROBE: child sprintf_s(NULL,1,"x") with the default handler terminates with 0xC0000417
 *        (STATUS_INVALID_CRUNTIME_PARAMETER) on pre-UCRT CRTs; ucrtbase may instead use __fastfail (0xC0000409), so the
 *        accepted set is {0xC0000417, 0xC0000409}. WER may alter results; raw code is what matters.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));

TC_CASE(exit_code)
{
    DWORD c = 0; CHECK(tc_spawn_self("fatal", &c, 20000));
    CHECK(c == TC_STATUS_INVALID_CRT_PARAM || c == TC_STATUS_FASTFAIL);
    CHECK(c != 77);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(sprintf_s);
    if (!tc_p_sprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    if (tc_child_name()) { char *volatile nb = NULL; tc_p_sprintf_s(nb, 1, "x"); return 77; }
    TC_RUN(exit_code); return tc_end();
}
