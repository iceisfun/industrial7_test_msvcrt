/* TC
 * id:    loc_mbtowc_wctomb_basic
 * area:  locale
 * funcs: mbtowc wctomb _mbtowc_l _wctomb_l
 * procs: _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.7.1, 7.22.8.1; MS docs mbtowc, wctomb
 * note:  Windows encodings are not state-dependent so the NULL,NULL query returns 0
 */
#include "tc.h"
#include <locale.h>
#include <stdlib.h>

TC_DECL_PROC(_locale_t, _create_locale, (int, const char *));
TC_DECL_PROC(void, _free_locale, (_locale_t));
static volatile char v_a = 'A';
static volatile wchar_t v_wa = L'A';

TC_CASE(mbtowc_ascii)
{
    char s[2]; wchar_t wc = 0; _locale_t L;
    s[0] = v_a; s[1] = 0;
    CHECK_EQ_I(mbtowc(&wc, s, 2), 1);
    CHECK_EQ_I(wc, L'A');
    wc = 0; L = tc_p__create_locale ? tc_p__create_locale(LC_ALL, "C") : NULL;
    if (!L) TC_SKIP("_create_locale unavailable");
    { CHECK_EQ_I(_mbtowc_l(&wc, s, 2, L), 1); CHECK_EQ_I(wc, L'A'); tc_p__free_locale(L); }
}
TC_CASE(mbtowc_nul_returns_0)
{
    char s[2] = { 0, 0 }; wchar_t wc = 5;
    CHECK_EQ_I(mbtowc(&wc, s, 1), 0);
    CHECK_EQ_I(wc, 0);
}
TC_CASE(wctomb_ascii)
{
    char b[4] = { 0, 0, 0, 0 }; _locale_t L;
    CHECK_EQ_I(wctomb(b, v_wa), 1);
    CHECK_EQ_I(b[0], 'A');
    b[0] = 0; L = tc_p__create_locale ? tc_p__create_locale(LC_ALL, "C") : NULL;
    if (!L) TC_SKIP("_create_locale unavailable");
    { CHECK_EQ_I(_wctomb_l(b, v_wa, L), 1); CHECK_EQ_I(b[0], 'A'); tc_p__free_locale(L); }
}
TC_CASE(wctomb_nul)
{
    char b[4] = { 'x', 'x', 'x', 'x' };
    CHECK_EQ_I(wctomb(b, L'\0'), 1);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(state_query_zero)
{
    char * volatile np = NULL;
    CHECK_EQ_I(wctomb(np, 0), 0);
    CHECK_EQ_I(mbtowc(NULL, np, 0), 0);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_create_locale); TC_LOAD_PROC(_free_locale); TC_RUN(mbtowc_ascii); TC_RUN(mbtowc_nul_returns_0); TC_RUN(wctomb_ascii); TC_RUN(wctomb_nul); TC_RUN(state_query_zero); return tc_end(); }
