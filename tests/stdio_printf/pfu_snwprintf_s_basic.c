/* TC
 * id:    pfu_snwprintf_s_basic
 * area:  stdio_printf
 * funcs: _snwprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snwprintf_s
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(count_ge_len)
{
    wchar_t w[16]; volatile size_t n = 16, c = 10; int r;
    r = _snwprintf_s(w, n, c, L"%d-%d", 12, 34);
    CHECK_EQ_I(r, 5);
    CHECK_WSTR(w, L"12-34");
}
TC_CASE(count_eq_len)
{
    wchar_t w[16]; volatile size_t n = 16, c = 5; int r;
    r = _snwprintf_s(w, n, c, L"%d-%d", 12, 34);
    CHECK_EQ_I(r, 5);
    CHECK_WSTR(w, L"12-34");
}
TC_CASE(str_widths)
{
    wchar_t w[32]; volatile size_t n = 32, c = 31; int r;
    r = _snwprintf_s(w, n, c, L"[%ls][%5ls][%-5ls][%hs][%5hs]", L"ab", L"cd", L"ef", "gh", "ij");
    CHECK_EQ_I(r, 29);
    CHECK_WSTR(w, L"[ab][   cd][ef   ][gh][   ij]");
}
int main(void)
{
    tc_begin();
    TC_RUN(count_ge_len); TC_RUN(count_eq_len); TC_RUN(str_widths);
    return tc_end();
}
