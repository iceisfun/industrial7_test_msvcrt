/* TC
 * id:    loc_neg_setlocale_failed_keeps_codepage_and_ctype
 * area:  locale
 * funcs: setlocale ___lc_codepage_func isalpha localeconv _get_current_locale _free_locale
 * procs: _isalpha_l
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs setlocale (NULL on failure, current locale unchanged); localeconv; _get_current_locale; MB_CUR_MAX
 * note:  ___lc_codepage_func is an undocumented export (observed value 1252); the NULL/unchanged claim itself is documented.
 *        English_United States.1252 is assumed installable on every Windows (all tests in this corpus rely on it).
 */
#include "tc.h"
unsigned int __cdecl ___lc_codepage_func(void);
#include <locale.h>
#include <ctype.h>
#include <stdlib.h>

#define EN "English_United States.1252"
TC_DECL_PROC(int, _isalpha_l, (int, _locale_t));
static const char *volatile n_en = EN;
static const char *volatile n_bad = "zz_nowhere";
static const char *volatile n_utf7 = ".65000";
static volatile int ch = 0xE9;

static void verify(void)
{
    struct lconv *lc = localeconv();
    _locale_t l;
    CHECK_STR(setlocale(LC_ALL, NULL), EN);
    CHECK_EQ_I(___lc_codepage_func(), 1252);
    CHECK(isalpha(ch) != 0);
    CHECK_EQ_I(MB_CUR_MAX, 1);
    CHECK(lc != NULL);
    if (lc) { CHECK_STR(lc->decimal_point, "."); CHECK_STR(lc->thousands_sep, ","); }
    l = _get_current_locale();
    CHECK(l != NULL);
    if (l) { CHECK(tc_p__isalpha_l(ch, l) != 0); _free_locale(l); }
}
TC_CASE(fail_all) { CHECK(setlocale(LC_ALL, n_bad) == NULL); verify(); }
TC_CASE(fail_utf7_ctype) { CHECK(setlocale(LC_CTYPE, n_utf7) == NULL); verify(); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_isalpha_l);
    if (!tc_p__isalpha_l || !setlocale(LC_ALL, n_en)) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }
    TC_RUN(fail_all); TC_RUN(fail_utf7_ctype);
    setlocale(LC_ALL, "C");
    return tc_end();
}
