/* TC
 * id:    pfm_1_len_l_c_s
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf Type Field Characters: lc/lC = wide char, ls/lS = wide string, hs/hS = narrow string; l/w override the default width of the formatter
 * note:  _snwprintf stands in for swprintf (swprintf's signature differs across CRT generations).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(sprintf_lc) { char b[16]; volatile wint_t c = L'A'; sprintf(b, "%lc", c); CHECK_STR(b, "A"); }
TC_CASE(sprintf_lC) { char b[16]; volatile wint_t c = L'A'; sprintf(b, "%lC", c); CHECK_STR(b, "A"); }
TC_CASE(sprintf_ls) { char b[16]; const wchar_t *volatile s = L"ab"; sprintf(b, "%ls", s); CHECK_STR(b, "ab"); }
TC_CASE(sprintf_lS) { char b[16]; const wchar_t *volatile s = L"ab"; sprintf(b, "%lS", s); CHECK_STR(b, "ab"); }
TC_CASE(sprintf_ls_prec) { char b[16]; const wchar_t *volatile s = L"abcd"; sprintf(b, "[%5.2ls]", s); CHECK_STR(b, "[   ab]"); }
TC_CASE(swprintf_hs) { wchar_t b[16]; const char *volatile s = "ab"; _snwprintf(b, 16, L"%hs", s); CHECK_WSTR(b, L"ab"); }
TC_CASE(swprintf_lc) { wchar_t b[16]; volatile wint_t c = L'z'; _snwprintf(b, 16, L"%lc", c); CHECK_WSTR(b, L"z"); }
TC_CASE(swprintf_lS) { wchar_t b[16]; const wchar_t *volatile s = L"ab"; _snwprintf(b, 16, L"%lS", s); CHECK_WSTR(b, L"ab"); }

int main(void)
{
    tc_begin();
    TC_RUN(sprintf_lc); TC_RUN(sprintf_lC); TC_RUN(sprintf_ls); TC_RUN(sprintf_lS); TC_RUN(sprintf_ls_prec);
    TC_RUN(swprintf_hs); TC_RUN(swprintf_lc); TC_RUN(swprintf_lS);
    return tc_end();
}
