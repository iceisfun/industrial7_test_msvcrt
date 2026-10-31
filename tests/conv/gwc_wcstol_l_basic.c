/* TC
 * id:    gwc_wcstol_l_basic
 * area:  conv
 * funcs: _wcstol_l _wcstoul_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.4; MS docs strtol, wcstol, _strtol_l, _wcstol_l
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <limits.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;
static long  S(const wchar_t *s, wchar_t **e, int b) { vp = s; return _wcstol_l(vp, e, b, L); }
static unsigned long U(const wchar_t *s, wchar_t **e, int b) { vp = s; return _wcstoul_l(vp, e, b, L); }

TC_CASE(decimal) { const wchar_t *s = L"123"; wchar_t *e; CHECK_EQ_I(S(s, &e, 10), 123); CHECK_EQ_P(e, s + 3); }
TC_CASE(ws_sign)
{
    const wchar_t *s = L" \t\n-42"; const wchar_t *t = L"  +7"; wchar_t *e;
    CHECK_EQ_I(S(s, &e, 10), -42); CHECK_EQ_P(e, s + 6);
    CHECK_EQ_I(S(t, &e, 10), 7); CHECK_EQ_P(e, t + 4);
}
TC_CASE(stop_char) { const wchar_t *s = L"98xyz"; wchar_t *e; CHECK_EQ_I(S(s, &e, 10), 98); CHECK_EQ_P(e, s + 2); }
TC_CASE(limits)
{
    wchar_t *e; errno = 0;
    CHECK_EQ_I(S(L"2147483647", &e, 10), 2147483647L);
    CHECK_EQ_I(S(L"-2147483648", &e, 10), (-2147483647L - 1));
    CHECK_EQ_I(errno, 0);
}
TC_CASE(ulong_max) { wchar_t *e; errno = 0; CHECK_EQ_U(U(L"4294967295", &e, 10), 4294967295UL); CHECK_EQ_I(errno, 0); }
TC_CASE(no_digits) { const wchar_t *s = L"  abc"; wchar_t *e; CHECK_EQ_I(S(s, &e, 10), 0); CHECK_EQ_P(e, s); }

int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(decimal); TC_RUN(ws_sign); TC_RUN(stop_char); TC_RUN(limits); TC_RUN(ulong_max); TC_RUN(no_digits);
    _free_locale(L);
    return tc_end();
}
