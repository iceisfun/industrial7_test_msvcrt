/* TC
 * id:    loc_neg_mbstowcs_l_wcstombs_l_null_locale_probe
 * area:  locale
 * funcs: _mbstowcs_l _wcstombs_l _mbtowc_l _wctomb_l _mbstowcs_s_l _wcstombs_s_l
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbstowcs_l (locale parameter); CRT convention that a NULL _locale_t means the current locale
 * note:  PROBE: a NULL _locale_t passed to the *_l conversion functions is believed to select the current ("C") locale, to not crash
 *        and not invoke the invalid-parameter handler. ASCII data only.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static const char *volatile v_s = "abc";
static const wchar_t *volatile v_w = L"abc";
static _locale_t volatile v_nl;

TC_CASE(mbstowcs_l)
{
    wchar_t b[8]; setlocale(LC_ALL, "C");
    CHECK_EQ_U(_mbstowcs_l(b, v_s, 4, v_nl), 3); CHECK_WSTR(b, L"abc");
}
TC_CASE(wcstombs_l)
{
    char b[8]; setlocale(LC_ALL, "C");
    CHECK_EQ_U(_wcstombs_l(b, v_w, 4, v_nl), 3); CHECK_STR(b, "abc");
}
TC_CASE(mbtowc_wctomb_roundtrip)
{
    wchar_t w = 0; char c[4] = {0}; setlocale(LC_ALL, "C");
    CHECK_EQ_I(_mbtowc_l(&w, v_s, 1, v_nl), 1); CHECK_EQ_I(w, 'a');
    CHECK_EQ_I(_wctomb_l(c, w, v_nl), 1); CHECK_EQ_I(c[0], 'a');
}
TC_CASE(secure_variants)
{
    wchar_t w[8]; char c[8]; size_t n = 99; setlocale(LC_ALL, "C");
    CHECK_EQ_I(_mbstowcs_s_l(&n, w, 8, v_s, 3, v_nl), 0); CHECK_EQ_U(n, 4); CHECK_WSTR(w, L"abc");
    n = 99;
    CHECK_EQ_I(_wcstombs_s_l(&n, c, 8, v_w, 3, v_nl), 0); CHECK_EQ_U(n, 4); CHECK_STR(c, "abc");
}
TC_CASE(no_iph)
{
    wchar_t w[8]; char c[8]; size_t n;
    if (!tc_iph_install()) TC_SKIP("no IPH api");
    tc_iph_count = 0; setlocale(LC_ALL, "C");
    _mbstowcs_l(w, v_s, 4, v_nl); _wcstombs_l(c, v_w, 4, v_nl); _mbtowc_l(w, v_s, 1, v_nl); _wctomb_l(c, L'a', v_nl);
    _mbstowcs_s_l(&n, w, 8, v_s, 3, v_nl); _wcstombs_s_l(&n, c, 8, v_w, 3, v_nl);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(mbstowcs_l); TC_RUN(wcstombs_l); TC_RUN(mbtowc_wctomb_roundtrip); TC_RUN(secure_variants); TC_RUN(no_iph);
    return tc_end();
}
