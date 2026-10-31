/* TC
 * id:    pfw_swprintf_c99_basic
 * area:  stdio_printf
 * funcs: swprintf
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.2.3 swprintf, 7.29.2.7 vswprintf; MS docs swprintf, _swprintf, swprintf_l
 * note:  C99-style swprintf(buf,count,fmt); msvcr120 and older export only legacy swprintf forms, so ucrtbase only. Truncation: returns negative and buf is NUL-terminated (C11).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

static void fill(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }
static int vs(wchar_t *b, size_t c, const wchar_t *f, ...)
{ va_list ap; int n; va_start(ap, f); n = vswprintf(b, c, f, ap); va_end(ap); return n; }

TC_CASE(fits_returns_len_and_NUL)
{
    wchar_t b[32]; int n; fill(b, 32);
    n = swprintf(b, 32, L"%d/%ls", 7, L"ab");
    CHECK_EQ_I(n, 4); CHECK_WSTR(b, L"7/ab"); CHECK(b[4] == 0);
}
TC_CASE(exact_fit_len_count_minus_1)
{
    wchar_t b[16]; int n; fill(b, 16);
    n = swprintf(b, 6, L"%ls", L"abcde");
    CHECK_EQ_I(n, 5); CHECK_WSTR(b, L"abcde"); CHECK(b[5] == 0); CHECK(b[6] == L'Z');
}
TC_CASE(count_len_plus_1)
{
    wchar_t b[16]; int n; fill(b, 16);
    n = swprintf(b, 7, L"%ls", L"abcde");
    CHECK_EQ_I(n, 5); CHECK_WSTR(b, L"abcde"); CHECK(b[6] == L'Z');
}
TC_CASE(vswprintf_variant)
{
    wchar_t b[32]; int n; fill(b, 32);
    n = vs(b, 32, L"[%5d]", 42);
    CHECK_EQ_I(n, 7); CHECK_WSTR(b, L"[   42]");
}

int main(void) { tc_begin(); TC_RUN(fits_returns_len_and_NUL); TC_RUN(exact_fit_len_count_minus_1); TC_RUN(count_len_plus_1); TC_RUN(vswprintf_variant); return tc_end(); }
