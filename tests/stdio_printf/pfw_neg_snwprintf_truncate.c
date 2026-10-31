/* TC
 * id:    pfw_neg_snwprintf_truncate
 * area:  stdio_printf
 * funcs: _snwprintf
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf/_snwprintf: count chars written; negative if len > count; no NUL if len == count
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }

TC_CASE(ret_m1)
{
    wchar_t b[16]; const wchar_t *volatile s = L"abcdefgh"; int n; fill(b);
    n = _snwprintf(b, 4, L"%ls", s);
    CHECK_EQ_I(n, -1);
}
TC_CASE(first_count_chars_written)
{
    wchar_t b[16]; const wchar_t *volatile s = L"abcdefgh"; fill(b);
    _snwprintf(b, 4, L"%ls", s);
    CHECK(b[0] == L'a' && b[1] == L'b' && b[2] == L'c' && b[3] == L'd');
}
TC_CASE(no_nul_at_count)
{
    wchar_t b[16]; const wchar_t *volatile s = L"abcdefgh"; fill(b);
    _snwprintf(b, 4, L"%ls", s);
    CHECK(b[3] != 0);
    CHECK(b[4] == L'Z');
}
TC_CASE(count_len_minus_1_ret_m1)
{
    wchar_t b[16]; const wchar_t *volatile s = L"abcdefgh"; int n; fill(b);
    n = _snwprintf(b, 7, L"%ls", s);
    CHECK_EQ_I(n, -1);
    CHECK(b[6] == L'g' && b[7] == L'Z');
}
TC_CASE(no_iph_called)
{
    wchar_t b[16]; const wchar_t *volatile s = L"abcdefgh"; fill(b);
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0;
    _snwprintf(b, 4, L"%ls", s);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); TC_RUN(ret_m1); TC_RUN(first_count_chars_written); TC_RUN(no_nul_at_count);
    TC_RUN(count_len_minus_1_ret_m1); TC_RUN(no_iph_called); return tc_end();
}
