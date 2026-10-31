/* TC
 * id:    loc_neg_create_locale_failure_no_global_effect
 * area:  locale
 * funcs: _create_locale _free_locale setlocale ___lc_codepage_func _configthreadlocale
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _create_locale (returns NULL on failure; creates an object independent of the current locale)
 */
#include "tc.h"
unsigned int __cdecl ___lc_codepage_func(void);
#include <locale.h>
#include <errno.h>

#define EN "English_United States.1252"
static const char *volatile n_en = EN;
static const char *volatile n_bad = "zz_nowhere";
static const char *volatile n_utf7 = ".65000";
static volatile int v_cat;
static int mode0;

TC_CASE(invalid_name)
{
    _locale_t l = _create_locale(LC_ALL, n_bad);
    CHECK(l == NULL); if (l) _free_locale(l);
    CHECK_STR(setlocale(LC_ALL, NULL), EN);
}
TC_CASE(utf7)
{
    _locale_t l = _create_locale(LC_CTYPE, n_utf7);
    CHECK(l == NULL); if (l) _free_locale(l);
    CHECK_EQ_I(___lc_codepage_func(), 1252);
}
TC_CASE(bad_category)
{
    _locale_t l; v_cat = 99; tc_iph_count = 0;
    l = _create_locale(v_cat, "C");
    CHECK(l == NULL); if (l) _free_locale(l);
    CHECK_STR(setlocale(LC_ALL, NULL), EN);
}
TC_CASE(thread_mode) { CHECK_EQ_I(_configthreadlocale(0), mode0); }
int main(void)
{
    tc_begin(); tc_iph_install();
    if (!setlocale(LC_ALL, n_en)) { tc_write("TC FAIL " TC_TEST_ID "::setup\n"); return 1; }
    mode0 = _configthreadlocale(0);
    TC_RUN(invalid_name); TC_RUN(utf7); TC_RUN(bad_category); TC_RUN(thread_mode);
    setlocale(LC_ALL, "C");
    return tc_end();
}
