/* TC
 * id:    gwc_neg_wcstoll_l_overflow
 * area:  conv
 * funcs: _wcstoll_l _wcstoull_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4 (LLONG_MAX/LLONG_MIN/ULLONG_MAX and ERANGE); MS docs wcstoll
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
TC_CASE(pos_over) { wchar_t *e; errno = 0; CHECK_EQ_I(S(L"9223372036854775808", &e, 10), LLONG_MAX); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(neg_under) { wchar_t *e; errno = 0; CHECK_EQ_I(S(L"-9223372036854775809", &e, 10), LLONG_MIN); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(u_over) { wchar_t *e; errno = 0; CHECK_EQ_U(U(L"18446744073709551616", &e, 10), ULLONG_MAX); CHECK_EQ_I(errno, ERANGE); }
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(pos_over); TC_RUN(neg_under); TC_RUN(u_over);
    _free_locale(L);
    return tc_end();
}
