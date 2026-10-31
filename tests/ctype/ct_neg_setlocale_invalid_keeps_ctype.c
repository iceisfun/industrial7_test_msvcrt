/* TC
 * id:    ct_neg_setlocale_invalid_keeps_ctype
 * area:  ctype
 * kind:  negative
 * funcs: setlocale isalpha toupper
 * procs: __pctype_func
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs setlocale ("returns NULL if the locale name is invalid; the locale is unchanged")
 * note:  A failed setlocale(LC_CTYPE, ...) must leave the C-locale ctype tables untouched.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

static volatile int vc;
static const char *volatile bogus1 = "zz_ZZ.99999";
static const char *volatile bogus2 = ".99999";

TC_CASE(bogus_locale_name_returns_NULL) { CHECK(setlocale(LC_CTYPE, bogus1) == NULL); }
TC_CASE(bogus_codepage_returns_NULL) { CHECK(setlocale(LC_CTYPE, bogus2) == NULL); }
TC_CASE(ctype_still_C)
{
    const unsigned short *(__cdecl *pf)(void) = (const unsigned short *(__cdecl *)(void))tc_crt_proc("__pctype_func");
    const unsigned short *t;
    vc = 0xE9; CHECK_EQ_I(isalpha(vc), 0);
    vc = 'a';  CHECK_EQ_I(toupper(vc), 'A');
    vc = 0xE9; CHECK_EQ_I(toupper(vc), 0xE9);
    CHECK(pf != NULL);
    t = pf ? pf() : NULL;
    CHECK(t != NULL);
    if (t) CHECK_EQ_I(t[0xE9] & _ALPHA, 0);
}
TC_CASE(setlocale_query_still_C) { CHECK_STR(setlocale(LC_CTYPE, NULL), "C"); CHECK_STR(setlocale(LC_ALL, NULL), "C"); }

int main(void) { tc_begin(); TC_RUN(bogus_locale_name_returns_NULL); TC_RUN(bogus_codepage_returns_NULL); TC_RUN(ctype_still_C); TC_RUN(setlocale_query_still_C); return tc_end(); }
