/* TC
 * id:    scn_neg_child_null_input_fatal
 * area:  scanf
 * kind:  negative
 * funcs: sscanf swscanf fscanf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs sscanf/swscanf/fscanf + "Parameter validation": NULL input/format invokes the invalid parameter handler; the default handler terminates the process
 * note:  Termination (not returning) is documented; the exact status 0xC0000409 is observed Win10 behaviour, not documented.
 *        Each NULL argument is exercised in its own child process with no handler installed. Exact status is 0xC0000409
 *        (fast-fail) on UCRT; 77 means the call returned instead of terminating.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const char *volatile vs; static const wchar_t *volatile vw; static FILE *volatile vf;
static const char *volatile vfmt; static const wchar_t *volatile vwfmt;

static void expect_fatal(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK(code != 0); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(sscanf_null_str)    { expect_fatal("sscanf_str"); }
TC_CASE(sscanf_null_fmt)    { expect_fatal("sscanf_fmt"); }
TC_CASE(swscanf_null_str)   { expect_fatal("swscanf_str"); }
TC_CASE(swscanf_null_fmt)   { expect_fatal("swscanf_fmt"); }
TC_CASE(fscanf_null_stream) { expect_fatal("fscanf_stream"); }
TC_CASE(fscanf_null_fmt)    { expect_fatal("fscanf_fmt"); }

static int child_main(const char *n)
{
    int x = 0; vs = NULL; vw = NULL; vf = NULL; vfmt = NULL; vwfmt = NULL;
    if (tc_strequ(n, "sscanf_str")) sscanf(vs, "%d", &x);
    else if (tc_strequ(n, "sscanf_fmt")) sscanf("1", vfmt, &x);
    else if (tc_strequ(n, "swscanf_str")) swscanf(vw, L"%d", &x);
    else if (tc_strequ(n, "swscanf_fmt")) swscanf(L"1", vwfmt, &x);
    else if (tc_strequ(n, "fscanf_stream")) fscanf(vf, "%d", &x);
    else if (tc_strequ(n, "fscanf_fmt")) fscanf(stdin, vfmt, &x);
    return 77;
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(sscanf_null_str); TC_RUN(sscanf_null_fmt); TC_RUN(swscanf_null_str);
    TC_RUN(swscanf_null_fmt); TC_RUN(fscanf_null_stream); TC_RUN(fscanf_null_fmt);
    return tc_end();
}
