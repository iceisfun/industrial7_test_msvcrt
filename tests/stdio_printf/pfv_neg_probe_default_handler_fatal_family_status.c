/* TC
 * id:    pfv_neg_probe_default_handler_fatal_family_status
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s fprintf _snprintf swprintf
 * crt:   win10
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation" (termination is documented, the exact status is not)
 * note:  PROBE: exact termination status of the UCRT default invalid-parameter path for the same six entry points as
 *        pfv_neg_default_handler_fatal_family: expected __fastfail 0xC0000409. Fatal calls run in child processes.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const char *volatile nullfmt = NULL;
static const wchar_t *volatile nullwfmt = NULL;
static char *volatile nullbuf = NULL;
static FILE *volatile nullstream = NULL;
static const char *volatile badfmt = "%y";
static const char *volatile longstr = "abcdefgh";

static void chk(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(sprintf_s_trunc_fatal) { chk("strunc"); }
TC_CASE(sprintf_s_null_buf_fatal) { chk("snull"); }
TC_CASE(fprintf_null_stream_fatal) { chk("fnull"); }
TC_CASE(invalid_conv_fatal) { chk("badconv"); }
TC_CASE(snprintf_null_fmt_fatal) { chk("snfmt"); }
TC_CASE(swprintf_null_fmt_fatal) { chk("swnull"); }

int main(void)
{
    const char *c; tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char b[4];
        if (tc_strequ(c, "strunc")) sprintf_s(b, sizeof b, "%s", longstr);
        else if (tc_strequ(c, "snull")) sprintf_s(nullbuf, 4, "x");
        else if (tc_strequ(c, "fnull")) fprintf(nullstream, "x");
        else if (tc_strequ(c, "badconv")) sprintf_s(b, sizeof b, badfmt);
        else if (tc_strequ(c, "snfmt")) _snprintf(b, sizeof b, nullfmt);
        else if (tc_strequ(c, "swnull")) { wchar_t w[4]; swprintf(w, 4, nullwfmt); }
        return 77;
    }
    TC_RUN(sprintf_s_trunc_fatal); TC_RUN(sprintf_s_null_buf_fatal); TC_RUN(fprintf_null_stream_fatal);
    TC_RUN(invalid_conv_fatal); TC_RUN(snprintf_null_fmt_fatal); TC_RUN(swprintf_null_fmt_fatal);
    return tc_end();
}
