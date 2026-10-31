/* TC
 * id:    loc_wcstombs_s_basic
 * area:  locale
 * funcs: wcstombs_s _wcstombs_s_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs wcstombs_s, _wcstombs_s_l (pReturnValue includes the terminating null)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>

errno_t __cdecl wcstombs_s(size_t *, char *, size_t, const wchar_t *, size_t);
errno_t __cdecl _wcstombs_s_l(size_t *, char *, size_t, const wchar_t *, size_t, _locale_t);

static wchar_t w_hello[] = L"hello", w_eur[] = L"\x20ac";

TC_CASE(full_with_nul)
{
    char b[16]; size_t n = 99; errno_t r = wcstombs_s(&n, b, 16, w_hello, 16);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 6); CHECK_STR(b, "hello");
}
TC_CASE(length_query)
{
    size_t n = 99; errno_t r = wcstombs_s(&n, NULL, 0, w_hello, 0);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 6);
}
TC_CASE(count_limit_adds_nul)
{
    char b[16]; size_t n = 99; errno_t r; int i;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    r = wcstombs_s(&n, b, 16, w_hello, 3);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 4); CHECK_STR(b, "hel"); CHECK_EQ_I(b[3], 0);
}
TC_CASE(euro_1252)
{
    unsigned char b[8] = {0}; size_t n = 99; errno_t r;
    if (!setlocale(LC_ALL, "English_United States.1252")) TC_SKIP("no 1252 locale");
    r = wcstombs_s(&n, (char *)b, 8, w_eur, 8);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 2); CHECK_EQ_U(b[0], 0x80); CHECK_EQ_U(b[1], 0);
}
TC_CASE(l_variant)
{
    char b[16]; size_t n = 99; errno_t r; _locale_t c = _create_locale(LC_ALL, "C");
    CHECK(c != NULL);
    if (!c) return;
    r = _wcstombs_s_l(&n, b, 16, w_hello, 16, c);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 6); CHECK_STR(b, "hello");
    _free_locale(c);
}
int main(void)
{
    tc_begin();
    TC_RUN(full_with_nul); TC_RUN(length_query); TC_RUN(count_limit_adds_nul);
    TC_RUN(euro_1252); TC_RUN(l_variant);
    setlocale(LC_ALL, "C");
    return tc_end();
}
