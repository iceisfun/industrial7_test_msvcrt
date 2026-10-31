/* TC
 * id:    loc_neg_wcreate_locale_invalid
 * area:  locale
 * procs: _wcreate_locale
 * funcs: _free_locale setlocale
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _create_locale, _wcreate_locale (returns NULL if the locale/category is invalid); parameter validation
 * note:  Unknown names / UTF-7 (65000) return NULL per docs ("code page ... CP_UTF7 not supported"). NULL name and bad category are
 *        parameter-validation failures (EINVAL + invalid-parameter handler).
 */
#include "tc.h"
#include <locale.h>

TC_DECL_PROC(_locale_t, _wcreate_locale, (int, const wchar_t *));
#include <errno.h>

static const wchar_t *volatile n_garbage = L"zz_nowhere";
static const wchar_t *volatile n_country = L"English_Nowhere.1252";
static const wchar_t *volatile n_utf7 = L".65000";
static const wchar_t *volatile n_null = NULL;
static volatile int v_cat;

static void global_is_c(void) { CHECK_STR(setlocale(LC_ALL, NULL), "C"); }

TC_CASE(garbage) { _locale_t l = tc_p__wcreate_locale(LC_ALL, n_garbage); CHECK(l == NULL); if (l) _free_locale(l); global_is_c(); }
TC_CASE(unknown_country) { _locale_t l = tc_p__wcreate_locale(LC_ALL, n_country); CHECK(l == NULL); if (l) _free_locale(l); global_is_c(); }
TC_CASE(null_name)
{
    _locale_t l; tc_iph_count = 0; errno = 0;
    l = tc_p__wcreate_locale(LC_ALL, n_null);
    CHECK(l == NULL); if (l) _free_locale(l);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    global_is_c();
}
TC_CASE(bad_category)
{
    _locale_t l;
    tc_iph_count = 0; errno = 0; v_cat = -1;
    l = tc_p__wcreate_locale(v_cat, L"C"); CHECK(l == NULL); if (l) _free_locale(l);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; v_cat = LC_MAX + 1;
    l = tc_p__wcreate_locale(v_cat, L"C"); CHECK(l == NULL); if (l) _free_locale(l);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    global_is_c();
}
TC_CASE(utf7) { _locale_t l = tc_p__wcreate_locale(LC_ALL, n_utf7); CHECK(l == NULL); if (l) _free_locale(l); global_is_c(); }
int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(_wcreate_locale);
    if (!tc_p__wcreate_locale) { tc_write("TC FAIL " TC_TEST_ID "::_wcreate_locale missing\n"); return 1; }
    TC_RUN(garbage); TC_RUN(unknown_country); TC_RUN(null_name); TC_RUN(bad_category); TC_RUN(utf7);
    setlocale(LC_ALL, "C");
    return tc_end();
}
