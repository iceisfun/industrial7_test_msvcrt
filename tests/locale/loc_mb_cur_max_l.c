/* TC
 * id:    loc_mb_cur_max_l
 * area:  locale
 * funcs: ___mb_cur_max_l_func _create_locale _free_locale setlocale ___mb_cur_max_func
 * crt:   >=90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs MB_CUR_MAX, _create_locale
 * note:  ___mb_cur_max_l_func(_locale_t) is the helper behind the _MB_CUR_MAX_L macro. Cases skip if the locale cannot be created.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

int __cdecl ___mb_cur_max_l_func(_locale_t);
static const char *volatile n_c = "C";
static const char *volatile n_1252 = "English_United States.1252";
static const char *volatile n_932 = ".932";

TC_CASE(c_locale_1)
{
    _locale_t l = _create_locale(LC_ALL, n_c);
    CHECK(l != NULL); if (!l) return;
    CHECK_EQ_I(___mb_cur_max_l_func(l), 1);
    _free_locale(l);
}
TC_CASE(cp1252_1)
{
    _locale_t l = _create_locale(LC_ALL, n_1252);
    if (!l) TC_SKIP("1252 locale unavailable");
    CHECK_EQ_I(___mb_cur_max_l_func(l), 1);
    _free_locale(l);
}
TC_CASE(cp932_2)
{
    _locale_t l = _create_locale(LC_ALL, n_932);
    if (!l) TC_SKIP("932 locale unavailable");
    CHECK_EQ_I(___mb_cur_max_l_func(l), 2);
    _free_locale(l);
}
TC_CASE(global_locale_unaffected)
{
    _locale_t lc, l932;
    if (!setlocale(LC_ALL, n_932)) { setlocale(LC_ALL, n_c); TC_SKIP("932 locale unavailable"); }
    lc = _create_locale(LC_ALL, n_c);
    CHECK(lc != NULL);
    CHECK_EQ_I(___mb_cur_max_func(), 2);
    if (lc) { CHECK_EQ_I(___mb_cur_max_l_func(lc), 1); _free_locale(lc); }
    setlocale(LC_ALL, n_c);
    l932 = _create_locale(LC_ALL, n_932);
    if (l932) { CHECK_EQ_I(___mb_cur_max_func(), 1); CHECK_EQ_I(___mb_cur_max_l_func(l932), 2); _free_locale(l932); }
}
int main(void)
{
    tc_begin();
    TC_RUN(c_locale_1); TC_RUN(cp1252_1); TC_RUN(cp932_2); TC_RUN(global_locale_unaffected);
    setlocale(LC_ALL, "C");
    return tc_end();
}
