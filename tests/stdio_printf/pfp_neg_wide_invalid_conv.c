/* TC
 * id:    pfp_neg_wide_invalid_conv
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf _snwprintf
 * procs: _set_invalid_parameter_handler
 * crt:   >=140
 * tier:  p1
 * conf:  observed
 * ref:   MS docs swprintf, _snwprintf (NULL format -> invalid parameter handler, EINVAL, -1); "Parameter validation"
 * note:  OBSERVED for case unknown_conv (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "unknown_conv r < 0", zzp_probe2 "swprintf %y: r=1 errno=0 iph=0 w=[y]"): swprintf with an
 *        unknown conversion "%y" prints the character literally ("y", % dropped): r=1, no handler. Old belief (negative + handler) was wrong
 *        (C11 7.29.2.1: invalid conversion is UB). percent_n_disabled and null_format are documented/unchanged and pass. Stable.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(unknown_conv)
{
    wchar_t w[16]; const wchar_t *volatile f = L"%y"; int r;
    tc_iph_count = 0;
    r = swprintf(w, 16, f);
    CHECK_EQ_I(r, 1); CHECK(w[0] == L'y' && w[1] == 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(percent_n_disabled)
{
    wchar_t w[16]; const wchar_t *volatile f = L"ab%n"; int n = 99; int *volatile pn = &n; int r;
    tc_iph_count = 0;
    r = swprintf(w, 16, f, pn);
    CHECK(r < 0);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(n, 99);       /* never written */
}
TC_CASE(null_format)
{
    wchar_t w[8]; const wchar_t *volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = _snwprintf(w, 8, f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(unknown_conv); TC_RUN(percent_n_disabled); TC_RUN(null_format);
    return tc_end();
}
