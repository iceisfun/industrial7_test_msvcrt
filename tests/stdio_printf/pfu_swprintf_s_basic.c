/* TC
 * id:    pfu_swprintf_s_basic
 * area:  stdio_printf
 * funcs: swprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, swprintf_s (sizeOfBuffer in wide characters); printf type field characters: in wide functions %s is wide, %S/%hs narrow, %c wide, %C/%hc narrow, %ls/%ws wide
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(strings)
{
    wchar_t b[64]; volatile int d = 42; int n;
    n = swprintf_s(b, 64, L"%d|%ls|%hs|%S|%s", d, L"wide", "narrow", "nar2", L"w2");
    CHECK_WSTR(b, L"42|wide|narrow|nar2|w2"); CHECK_EQ_I(n, 22);
}
TC_CASE(chars)
{
    wchar_t b[16]; volatile wchar_t wc = L'W'; volatile int nc = 'n'; int n;
    n = swprintf_s(b, 16, L"%c|%lc|%hc|%C", wc, wc, nc, nc);
    CHECK_WSTR(b, L"W|W|n|n"); CHECK_EQ_I(n, 7);
}
TC_CASE(exact_fit)
{
    wchar_t b[6]; volatile const wchar_t *s = L"abcde"; int n;
    n = swprintf_s(b, 6, L"%ls", (const wchar_t *)s);
    CHECK_EQ_I(n, 5); CHECK_WSTR(b, L"abcde"); CHECK_EQ_I(b[5], 0);
}
TC_CASE(empty_format)
{
    wchar_t b[4]; volatile const wchar_t *f = L""; int n; b[0] = L'x';
    n = swprintf_s(b, 4, (const wchar_t *)f);
    CHECK_EQ_I(n, 0); CHECK_EQ_I(b[0], 0);
}
TC_CASE(size_in_elements)
{
    wchar_t b[6]; volatile const wchar_t *s = L"12345"; int n;
    n = swprintf_s(b, sizeof b / sizeof b[0], L"%ls", (const wchar_t *)s);
    CHECK_EQ_I(n, 5); CHECK_WSTR(b, L"12345");
}
TC_CASE(width_precision)
{
    wchar_t b[64]; int n;
    n = swprintf_s(b, 64, L"[%6ws][%-6hs][%.2ws][%.3hs][%6.2ls]", L"ab", "cd", L"efgh", "ijklm", L"nopq");
    CHECK_WSTR(b, L"[    ab][cd    ][ef][ijk][    no]"); CHECK_EQ_I(n, 33);
}
int main(void)
{
    tc_begin();
    TC_RUN(strings); TC_RUN(chars); TC_RUN(exact_fit); TC_RUN(empty_format); TC_RUN(size_in_elements); TC_RUN(width_precision);
    return tc_end();
}
