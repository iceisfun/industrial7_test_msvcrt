/* TC
 * id:    pfs_neg_probe_wide_s_bad_length_modifiers
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   UCRT wide printf: unsupported length-modifier / conversion combinations are invalid -> invalid parameter handler, EINVAL, -1
 * note:  OBSERVED (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "lls n == -1 got=1", "Lc n == -1 got=1", "hhs ... iph got=0 errno got=0"; zzp_probe4 "_snwprintf %hhs: r=11 iph=0" (a NARROW "x"
 *        was read as a wide string, i.e. hh is ignored and %s keeps its wide-function meaning), "%lls: r=1 w=[x]", "%Lc: r=1 w=[x]"; skeptic probe, twice: hhs with L"xy" -> r=2 w=[xy], hs with L"xy" -> r=1 w=[x]): in _snwprintf the
 *        "bad" modifiers %hhs, %lls and %Lc are NOT rejected: the extra length letters are accepted/ignored, the conversion behaves as plain %s / %c
 *        (wide), no handler, errno unchanged. The hhs case now passes a wide string (the old narrow argument made the result read adjacent
 *        memory). Controls %hs/%ls unchanged. C11 7.21.6.1p7: hh is for integer conversions only, ll likewise, L for floating only -> UB for s/c,
 *        so conforming. Stable.
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
    n = _snwprintf(w, 16, wf_hhs, L"xy");   /* two chars so that hh taken as h (narrow) would give 1, not 2 */
    CHECK_EQ_I(n, 2); CHECK(w[0] == L'x' && w[1] == L'y' && w[2] == 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
TC_CASE(lls)
{
    wchar_t w[16]; int n; errno = 0; tc_iph_count = 0;
    n = _snwprintf(w, 16, wf_lls, L"x");
    CHECK_EQ_I(n, 1); CHECK(w[0] == L'x' && w[1] == 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
}
TC_CASE(Lc)
{
    wchar_t w[16]; int n; errno = 0; tc_iph_count = 0;
    n = _snwprintf(w, 16, wf_Lc, xc);
    CHECK_EQ_I(n, 1); CHECK(w[0] == L'x' && w[1] == 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
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
