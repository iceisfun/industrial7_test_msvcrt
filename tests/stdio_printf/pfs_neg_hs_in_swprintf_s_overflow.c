/* TC
 * id:    pfs_neg_hs_in_swprintf_s_overflow
 * area:  stdio_printf
 * funcs: swprintf_s _snwprintf_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s, sprintf_s_l, swprintf_s, _snprintf_s (_TRUNCATE); printf type field (%hs in wide = narrow string)
 * note:  Narrow %hs argument in the wide secure printf: buffer-too-small invokes the invalid-parameter handler and sets
 *        buffer[0]=0; _TRUNCATE truncates, keeps the terminator and does not invoke the handler.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(overflow_exact)
{
    wchar_t w[8]; volatile const char *s = "abcd"; int n;
    if (!tc_iph_install()) TC_SKIP("no iph");
    w[0] = L'X'; tc_iph_count = 0;
    n = swprintf_s(w, 4, L"%hs", (const char *)s);
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(w[0], 0);
}
TC_CASE(fits_with_terminator)
{
    wchar_t w[8]; volatile const char *s = "abcd"; int n;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0;
    n = swprintf_s(w, 5, L"%hs", (const char *)s);
    CHECK_EQ_I(n, 4); CHECK_WSTR(w, L"abcd"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(truncate)
{
    wchar_t w[8]; volatile const char *s = "abcdef"; int n;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0;
    n = _snwprintf_s(w, 4, _TRUNCATE, L"%hs", (const char *)s);
    CHECK_EQ_I(n, -1);
    CHECK_WSTR(w, L"abc");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(precision_fits)
{
    wchar_t w[8]; volatile const char *s = "abcdef"; int n;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0;
    n = swprintf_s(w, 3, L"%.2hs", (const char *)s);
    CHECK_EQ_I(n, 2); CHECK_WSTR(w, L"ab"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(overflow_exact); TC_RUN(fits_with_terminator); TC_RUN(truncate); TC_RUN(precision_fits);
    return tc_end();
}
