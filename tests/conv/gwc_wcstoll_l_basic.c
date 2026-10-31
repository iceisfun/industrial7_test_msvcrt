/* TC
 * id:    gwc_wcstoll_l_basic
 * area:  conv
 * funcs: _wcstoll_l _wcstoull_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.4; MS docs strtoll, wcstoll, _strtoll_l, _wcstoll_l (VS2013+)
 * note:  not exported by msvcr110 and older
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <limits.h>
#include <wchar.h>

long long __cdecl _wcstoll_l(const wchar_t *, wchar_t **, int, _locale_t);
unsigned long long __cdecl _wcstoull_l(const wchar_t *, wchar_t **, int, _locale_t);
static _locale_t L;
static const wchar_t *volatile vp;
static long long S(const wchar_t *s, wchar_t **e, int b) { vp = s; return _wcstoll_l(vp, e, b, L); }
static unsigned long long U(const wchar_t *s, wchar_t **e, int b) { vp = s; return _wcstoull_l(vp, e, b, L); }
TC_CASE(max_min)
{
    wchar_t *e; errno = 0;
    CHECK_EQ_I(S(L"9223372036854775807", &e, 10), LLONG_MAX);
    CHECK_EQ_I(S(L"-9223372036854775808", &e, 10), LLONG_MIN);
    CHECK_EQ_I(errno, 0);
}
TC_CASE(umax) { const wchar_t *s = L"18446744073709551615"; wchar_t *e; errno = 0; CHECK_EQ_U(U(s, &e, 10), ULLONG_MAX); CHECK_EQ_P(e, s + 20); CHECK_EQ_I(errno, 0); }
TC_CASE(prefix) { wchar_t *e; CHECK_EQ_I(S(L"0xFFFFFFFFFF", &e, 16), 0xFFFFFFFFFFLL); CHECK_EQ_I(S(L"0x1F", &e, 0), 31); CHECK_EQ_I(S(L"017", &e, 0), 15); CHECK_EQ_U(U(L"0x10", &e, 0), 16); }
TC_CASE(endptr) { const wchar_t *s = L"  -123abc"; wchar_t *e; CHECK_EQ_I(S(s, &e, 10), -123); CHECK_EQ_P(e, s + 6); }
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(max_min); TC_RUN(umax); TC_RUN(prefix); TC_RUN(endptr);
    _free_locale(L);
    return tc_end();
}
