/* TC
 * id:    pfw_snwprintf_exact_fit
 * area:  stdio_printf
 * funcs: _snwprintf
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _snwprintf: if len == count, len wchars stored and no terminating NUL
 * note:  documented; truncation (len>count) is covered by pfi_int_neg_snwprintf_truncate
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }

TC_CASE(exact_fit_ret_len_and_no_nul)
{
    wchar_t b[16]; int n; fill(b, 16);
    n = _snwprintf(b, 5, L"%ls", L"abcde");
    CHECK_EQ_I(n, 5);
    CHECK(b[0] == L'a' && b[4] == L'e');
    CHECK(b[5] == L'Z');
}
TC_CASE(exact_fit_int)
{
    wchar_t b[16]; volatile int v = 12345; int n; fill(b, 16);
    n = _snwprintf(b, 5, L"%d", v);
    CHECK_EQ_I(n, 5); CHECK(b[4] == L'5'); CHECK(b[5] == L'Z');
}
TC_CASE(count_len_plus_1_terminates)
{
    wchar_t b[16]; int n; fill(b, 16);
    n = _snwprintf(b, 6, L"%ls", L"abcde");
    CHECK_EQ_I(n, 5); CHECK_WSTR(b, L"abcde"); CHECK(b[5] == 0); CHECK(b[6] == L'Z');
}

int main(void) { tc_begin(); TC_RUN(exact_fit_ret_len_and_no_nul); TC_RUN(exact_fit_int); TC_RUN(count_len_plus_1_terminates); return tc_end(); }
