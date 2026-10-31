/* TC
 * id:    err_neg_default_fatal_status_win7_probe
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: strcpy_s
 * crt:   msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "Parameter validation" (promises termination only)
 * note:  PROBE: exact exit status when the default invalid-parameter handler fires in a legacy CRT. Asserts the status is one of 0xC0000417 (no fastfail, e.g. Win7 host or msvcr100) or 0xC0000409 (__fastfail on Win8+ hosts); the observed value is printed as TC INFO. Guess: 0xC0000417 (STATUS_INVALID_CRUNTIME_PARAMETER); anything else (e.g. 0xC0000005) fails.
 */
#include "tc.h"
#include <string.h>

TC_DECL_PROC(int, strcpy_s, (char *, size_t, const char *));

TC_CASE(exit_status)
{
    DWORD code = 0; int ran = tc_spawn_self("badcopy", &code, 20000);
    CHECK(ran);
    char h[24]; tc_hex(code, h); tc_write("TC INFO exit status "); tc_write(h); tc_write("\n");
    /* Win7-era _invoke_watson reports 0xC0000417; a CRT that uses __fastfail on Win8+ hosts reports 0xC0000409. */
    CHECK(code == TC_STATUS_INVALID_CRT_PARAM || code == TC_STATUS_FASTFAIL);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        char *volatile np = NULL;
        TC_LOAD_PROC(strcpy_s);
        if (tc_p_strcpy_s) tc_p_strcpy_s(np, 10, "abc");
        return 77;
    }
    TC_RUN(exit_status);
    return tc_end();
}
