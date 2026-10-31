/* TC
 * id:    pfw_snwprintf_basic
 * area:  stdio_printf
 * funcs: _snwprintf
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _snprintf, _snwprintf, _snprintf_l, _snwprintf_l
 * note:  return = wide chars written excluding NUL; %ls wide string, %hs narrow string, %s wide in wide functions (MS default)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }

TC_CASE(ints_strings)
{
    wchar_t b[64]; volatile const wchar_t *w = L"wide"; volatile const char *a = "narrow"; int n;
    fill(b, 64);
    n = _snwprintf(b, 64, L"%d|%ls|%hs|%s", -17, (const wchar_t *)w, (const char *)a, (const wchar_t *)w);
    CHECK_WSTR(b, L"-17|wide|narrow|wide");
    CHECK_EQ_I(n, 20);
}
TC_CASE(empty_format_returns_0)
{
    wchar_t b[8]; int n; fill(b, 8);
    n = _snwprintf(b, 8, L"");
    CHECK_EQ_I(n, 0); CHECK(b[0] == 0);
}
TC_CASE(long_buffer_terminated)
{
    wchar_t b[32]; int n; fill(b, 32);
    n = _snwprintf(b, 31, L"abc%d", 12);
    CHECK_EQ_I(n, 5); CHECK_WSTR(b, L"abc12"); CHECK(b[5] == 0);
}
TC_CASE(literal_only)
{
    wchar_t b[32]; int n; fill(b, 32);
    n = _snwprintf(b, 32, L"hello world");
    CHECK_EQ_I(n, 11); CHECK_WSTR(b, L"hello world");
}
TC_CASE(char_and_percent)
{
    wchar_t b[32]; int n; fill(b, 32);
    n = _snwprintf(b, 32, L"%lc|%%|%5ls|%-4ls|", (wchar_t)L'q', L"ab", L"cd");
    CHECK_WSTR(b, L"q|%|   ab|cd  |"); CHECK_EQ_I(n, 15);
}
TC_CASE(non_ascii_wide)
{
    wchar_t b[16]; volatile wchar_t w = 0x20AC; int n; fill(b, 16);
    n = _snwprintf(b, 16, L"[%lc]", (wchar_t)w);
    CHECK_EQ_I(n, 3); CHECK(b[1] == 0x20AC && b[2] == L']' && b[3] == 0);
}

int main(void) { tc_begin(); TC_RUN(ints_strings); TC_RUN(empty_format_returns_0); TC_RUN(long_buffer_terminated); TC_RUN(literal_only); TC_RUN(char_and_percent); TC_RUN(non_ascii_wide); return tc_end(); }
