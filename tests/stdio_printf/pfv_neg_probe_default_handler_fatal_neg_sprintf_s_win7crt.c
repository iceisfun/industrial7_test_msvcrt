/* TC
 * id:    pfv_neg_probe_default_handler_fatal_neg_sprintf_s_win7crt
 * area:  stdio_printf
 * funcs: fprintf
 * procs: sprintf_s
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sprintf_s (buffer too small invokes invalid parameter handler); MS docs _set_invalid_parameter_handler
 * note:  PROBE: pre-UCRT default invalid-parameter handler. Documented: with no handler the process is terminated
 *        (via Watson / unhandled exception). Probed: the exact exit code, expected 0xC0000417
 *        (STATUS_INVALID_CRUNTIME_PARAMETER) or 0xC0000409 depending on the CRT/OS; never 77 (returned) and never 0.
 *        Cases: sprintf_s into a too-small buffer, fprintf with a NULL stream. Both run in child processes.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
static char *volatile small_buf;
static FILE *volatile null_stream;

static int fatal_code_ok(DWORD code)
{
    return code == TC_STATUS_INVALID_CRT_PARAM || code == TC_STATUS_FASTFAIL;
}

TC_CASE(sprintf_s_trunc_fatal)
{
    DWORD code = 0; int ran = tc_spawn_self("trunc", &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0);
    CHECK(fatal_code_ok(code));
}
TC_CASE(fprintf_null_stream_fatal)
{
    DWORD code = 0; int ran = tc_spawn_self("nullstream", &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0);
    CHECK(fatal_code_ok(code));
}

int main(void)
{
    const char *c;
    static char buf[4];
    tc_begin();
    TC_LOAD_PROC(sprintf_s);
    if ((c = tc_child_name()) != NULL) {
        small_buf = buf;
        if (c[0] == 't') tc_p_sprintf_s(small_buf, 4, "%s", "toolongstring");
        else fprintf(null_stream, "x");
        return 77;
    }
    TC_RUN(sprintf_s_trunc_fatal);
    TC_RUN(fprintf_null_stream_fatal);
    return tc_end();
}
