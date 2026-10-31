/* TC
 * id:    gwc_wcstoi64_l_basic
 * area:  conv
 * funcs: _wcstoi64_l _wcstoui64_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strtoi64, _wcstoi64, _strtoi64_l, _wcstoi64_l
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <limits.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;
static __int64 S(const wchar_t *s, wchar_t **e, int b) { vp = s; return _wcstoi64_l(vp, e, b, L); }
static unsigned __int64 U(const wchar_t *s, wchar_t **e, int b) { vp = s; return _wcstoui64_l(vp, e, b, L); }

TC_CASE(max_min)
{
    wchar_t *e; errno = 0;
    CHECK_EQ_I(S(L"9223372036854775807", &e, 10), _I64_MAX);
    CHECK_EQ_I(S(L"-9223372036854775808", &e, 10), _I64_MIN);
    CHECK_EQ_I(errno, 0);
}
TC_CASE(umax) { const wchar_t *s = L"18446744073709551615"; wchar_t *e; errno = 0; CHECK_EQ_U(U(s, &e, 10), _UI64_MAX); CHECK_EQ_P(e, s + 20); CHECK_EQ_I(errno, 0); }
TC_CASE(hex) { const wchar_t *s = L"0xFFFFFFFFFF"; wchar_t *e; CHECK_EQ_I(S(s, &e, 16), 0xFFFFFFFFFFLL); CHECK_EQ_P(e, s + 12); CHECK_EQ_U(U(L"FFFFFFFFFF", &e, 16), 0xFFFFFFFFFFULL); }
TC_CASE(base0) { wchar_t *e; CHECK_EQ_I(S(L"0x1F", &e, 0), 31); CHECK_EQ_I(S(L"017", &e, 0), 15); CHECK_EQ_I(S(L"17", &e, 0), 17); CHECK_EQ_U(U(L"0x100000000", &e, 0), 0x100000000ULL); }
TC_CASE(ws_sign_stop)
{
    const wchar_t *s = L" \t-5000000000x"; const wchar_t *t = L"+77 8"; wchar_t *e;
    CHECK_EQ_I(S(s, &e, 10), -5000000000LL); CHECK_EQ_P(e, s + 13);
    CHECK_EQ_I(S(t, &e, 10), 77); CHECK_EQ_P(e, t + 3);
}
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(max_min); TC_RUN(umax); TC_RUN(hex); TC_RUN(base0); TC_RUN(ws_sign_stop);
    _free_locale(L);
    return tc_end();
}
