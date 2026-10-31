/* TC
 * id:    loc_mbstowcs_s_basic
 * area:  locale
 * funcs: mbstowcs_s _mbstowcs_s_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs mbstowcs_s, _mbstowcs_s_l (pReturnValue includes the terminating null)
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <locale.h>

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
errno_t __cdecl mbstowcs_s(size_t *, wchar_t *, size_t, const char *, size_t);
errno_t __cdecl _mbstowcs_s_l(size_t *, wchar_t *, size_t, const char *, size_t, _locale_t);

static char hello[] = "hello";
static char empty[] = "";

TC_CASE(full_includes_nul)
{
    wchar_t w[16]; size_t n = 99; errno_t r;
    r = mbstowcs_s(&n, w, 16, hello, 16);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 6); CHECK_WSTR(w, L"hello");
    n = 99; r = mbstowcs_s(&n, w, 6, hello, 5);   /* exact fit incl. NUL */
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 6); CHECK_WSTR(w, L"hello");
}
TC_CASE(null_dest_size_query)
{
    size_t n = 99; errno_t r = mbstowcs_s(&n, NULL, 0, hello, 0);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 6);
}
TC_CASE(count_limit_adds_nul)
{
    wchar_t w[16]; size_t n = 99; errno_t r; int i;
    for (i = 0; i < 16; i++) w[i] = 0x5a5a;
    r = mbstowcs_s(&n, w, 16, hello, 3);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 4); CHECK_WSTR(w, L"hel"); CHECK_EQ_I(w[3], 0);
}
TC_CASE(empty_string_returns_1)
{
    wchar_t w[4]; size_t n = 99; errno_t r = mbstowcs_s(&n, w, 4, empty, 4);
    CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 1); CHECK_EQ_I(w[0], 0);
}
TC_CASE(l_variant)
{
    wchar_t w[16]; size_t n = 99; errno_t r; _locale_t c = _create_locale(LC_ALL, "C"), l;
    static char eacute[] = "caf\xE9";
    CHECK(c != NULL);
    if (c) {
        r = _mbstowcs_s_l(&n, w, 16, hello, 16, c);
        CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 6); CHECK_WSTR(w, L"hello");
        _free_locale(c);
    }
    l = _create_locale(LC_ALL, "English_United States.1252");
    if (l) {   /* 1252 maps 0xE9 to U+00E9 */
        n = 99; r = _mbstowcs_s_l(&n, w, 16, eacute, 16, l);
        CHECK_EQ_I(r, 0); CHECK_EQ_U(n, 5); CHECK_EQ_I(w[3], 0xE9); CHECK_EQ_I(w[4], 0);
        _free_locale(l);
    }
}
int main(void)
{
    tc_begin();
    TC_RUN(full_includes_nul); TC_RUN(null_dest_size_query); TC_RUN(count_limit_adds_nul);
    TC_RUN(empty_string_returns_1); TC_RUN(l_variant);
    return tc_end();
}
