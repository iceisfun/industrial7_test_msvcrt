/* TC
 * id:    pfs_neg_probe_wide_s_bad_length_modifiers
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   UCRT wide printf: unsupported length-modifier / conversion combinations are invalid -> invalid parameter handler, EINVAL, -1
 * note:  PROBE: wide-printf counterpart of pfs_neg_probe_string_bad_length_modifiers: %hhs, %lls, %Lc rejected; controls %hs and %ls still work afterwards.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static const wchar_t *volatile wf_hhs = L"%hhs";
static const wchar_t *volatile wf_lls = L"%lls";
static const wchar_t *volatile wf_Lc = L"%Lc";
static const wchar_t *volatile wf_hs = L"%hs";
static const wchar_t *volatile wf_ls = L"%ls";
static volatile int xc = 'x';

TC_CASE(hhs)
{
    wchar_t w[16]; int n; errno = 0; tc_iph_count = 0;
    n = _snwprintf(w, 16, wf_hhs, "x");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(lls)
{
    wchar_t w[16]; int n; errno = 0; tc_iph_count = 0;
    n = _snwprintf(w, 16, wf_lls, L"x");
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(Lc)
{
    wchar_t w[16]; int n; errno = 0; tc_iph_count = 0;
    n = _snwprintf(w, 16, wf_Lc, xc);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(controls_after)
{
    wchar_t w[16]; int n; tc_iph_count = 0;
    n = _snwprintf(w, 16, wf_hs, "ok");
    CHECK_EQ_I(n, 2); CHECK(w[0] == L'o' && w[1] == L'k');
    n = _snwprintf(w, 16, wf_ls, L"ok");
    CHECK_EQ_I(n, 2); CHECK(w[0] == L'o' && w[1] == L'k');
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(hhs); TC_RUN(lls); TC_RUN(Lc); TC_RUN(controls_after);
    return tc_end();
}
