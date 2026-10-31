/* TC
 * id:    pfs_neg_swprintf_truncation
 * area:  stdio_printf
 * funcs: swprintf

 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.2.3 swprintf: returns negative if n or more wide characters would have been written
 * note:  ucrt NUL-terminates the truncated output (the "abc" check is the Microsoft behaviour; the standard says output is truncated and terminated too)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
static const wchar_t *volatile f_ls = L"%ls";
static const wchar_t *volatile s_abcdef = L"abcdef";
TC_CASE(too_small)
{
    wchar_t w[16]; int n; fill(w);
    n = swprintf(w, 4, f_ls, s_abcdef);
    CHECK(n < 0);
    CHECK(w[0] == L'a' && w[1] == L'b' && w[2] == L'c' && w[3] == 0);
    CHECK(w[4] == L'Z');
}
TC_CASE(count_zero)
{
    wchar_t w[16]; int n; fill(w);
    n = swprintf(w, 0, f_ls, s_abcdef);
    CHECK(n < 0);
    CHECK(w[0] == L'Z');
}
TC_CASE(exact_fit)
{
    wchar_t w[16]; int n; fill(w);
    n = swprintf(w, 7, f_ls, s_abcdef);
    CHECK_EQ_I(n, 6);
    CHECK_WSTR(w, L"abcdef");
    fill(w);
    n = swprintf(w, 6, f_ls, s_abcdef);
    CHECK(n < 0);
}
int main(void) { tc_begin(); TC_RUN(too_small); TC_RUN(count_zero); TC_RUN(exact_fit); return tc_end(); }
