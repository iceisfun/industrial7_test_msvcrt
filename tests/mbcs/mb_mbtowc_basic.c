/* TC
 * id:    mb_mbtowc_basic
 * area:  mbcs
 * funcs: mbtowc setlocale
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.22.7.2; MS docs mbtowc; cp932 table (0x82A0 = U+3042, 0xB1 = U+FF71)
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <stdio.h>

TC_CASE(c_locale)
{
    volatile const char *a = "A", *e = ""; wchar_t wc = 1;
    CHECK_EQ_I(mbtowc(&wc, (const char *)a, 1), 1); CHECK_EQ_U(wc, L'A');
    wc = 1; CHECK_EQ_I(mbtowc(&wc, (const char *)e, 1), 0); CHECK_EQ_U(wc, 0);
    CHECK_EQ_I(mbtowc(NULL, (const char *)a, 1), 1);
}
TC_CASE(null_stateless) { volatile const char *n = NULL; CHECK_EQ_I(mbtowc(NULL, (const char *)n, 0), 0); }
TC_CASE(cp932)
{
    static const char dbc[] = { (char)0x82, (char)0xA0, 0 }, kana[] = { (char)0xB1, 0 };
    volatile const char *p = dbc, *k = kana; wchar_t wc = 0;
    if (!setlocale(LC_CTYPE, ".932")) TC_SKIP("cp932 locale unavailable");
    CHECK_EQ_I(mbtowc(&wc, (const char *)p, 2), 2); CHECK_EQ_U(wc, 0x3042);
    wc = 0; CHECK_EQ_I(mbtowc(&wc, (const char *)k, 1), 1); CHECK_EQ_U(wc, 0xFF71);
    CHECK_EQ_I(mbtowc(NULL, (const char *)p, 2), 2);
    setlocale(LC_ALL, "C");
}

int main(void) { tc_begin(); TC_RUN(c_locale); TC_RUN(null_stateless); TC_RUN(cp932); setlocale(LC_ALL, "C"); return tc_end(); }
