/* TC
 * id:    gwc_wcstol_l_base_prefix
 * area:  conv
 * funcs: _wcstol_l _wcstoul_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.4; MS docs wcstol, _wcstol_l
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

TC_CASE(base0) { wchar_t *e; CHECK_EQ_I(S(L"0x1F", &e, 0), 31); CHECK_EQ_I(S(L"017", &e, 0), 15); CHECK_EQ_I(S(L"17", &e, 0), 17); CHECK_EQ_I(S(L"0X1f", &e, 0), 31); }
TC_CASE(base16)
{
    const wchar_t *a = L"0xAbC", *b = L"abc", *c = L"FF"; wchar_t *e;
    CHECK_EQ_I(S(a, &e, 16), 0xabc); CHECK_EQ_P(e, a + 5);
    CHECK_EQ_I(S(b, &e, 16), 0xabc); CHECK_EQ_P(e, b + 3);
    CHECK_EQ_I(S(c, &e, 16), 255);
}
TC_CASE(base36) { const wchar_t *s = L"zz"; wchar_t *e; CHECK_EQ_I(S(s, &e, 36), 1295); CHECK_EQ_P(e, s + 2); CHECK_EQ_I(S(L"ZZ", &e, 36), 1295); }
TC_CASE(base2) { const wchar_t *s = L"1012"; wchar_t *e; CHECK_EQ_I(S(s, &e, 2), 5); CHECK_EQ_P(e, s + 3); }
TC_CASE(base8) { const wchar_t *s = L"789"; wchar_t *e; CHECK_EQ_I(S(s, &e, 8), 7); CHECK_EQ_P(e, s + 1); }
TC_CASE(unsigned_variant)
{
    const wchar_t *s = L"1012"; const wchar_t *t = L"789"; wchar_t *e;
    CHECK_EQ_U(U(L"0x1F", &e, 0), 31); CHECK_EQ_U(U(L"017", &e, 0), 15); CHECK_EQ_U(U(L"17", &e, 0), 17);
    CHECK_EQ_U(U(L"zz", &e, 36), 1295);
    CHECK_EQ_U(U(s, &e, 2), 5); CHECK_EQ_P(e, s + 3);
    CHECK_EQ_U(U(t, &e, 8), 7); CHECK_EQ_P(e, t + 1);
    CHECK_EQ_U(U(L"0xff", &e, 16), 255);
}
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(base0); TC_RUN(base16); TC_RUN(base36); TC_RUN(base2); TC_RUN(base8); TC_RUN(unsigned_variant);
    _free_locale(L);
    return tc_end();
}
