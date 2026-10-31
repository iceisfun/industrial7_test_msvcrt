/* TC
 * id:    loc_get_current_locale_tracks_global
 * area:  locale
 * funcs: _get_current_locale _free_locale setlocale
 * procs: _isalpha_l
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _get_current_locale ("gets a locale object representing the current locale")
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

TC_DECL_PROC(int, _isalpha_l, (int, _locale_t));
static const char *volatile n_1252 = "English_United States.1252";
static const char *volatile n_c = "C";
static volatile int ch = 0xE9;

TC_CASE(snapshot_in_c)
{
    _locale_t l = _get_current_locale();
    CHECK(l != NULL); if (!l) return;
    CHECK_EQ_I(tc_p__isalpha_l(ch, l) != 0, 0);
    _free_locale(l);
}
TC_CASE(snapshot_in_1252)
{
    _locale_t l;
    if (!setlocale(LC_ALL, n_1252)) { setlocale(LC_ALL, n_c); TC_SKIP("1252 locale unavailable"); }
    l = _get_current_locale();
    CHECK(l != NULL);
    if (l) { CHECK(tc_p__isalpha_l(ch, l) != 0); _free_locale(l); }
    setlocale(LC_ALL, n_c);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_isalpha_l); if (!tc_p__isalpha_l) { tc_write("TC FAIL " TC_TEST_ID "::_isalpha_l missing\n"); return 1; }
    TC_RUN(snapshot_in_c); TC_RUN(snapshot_in_1252);
    setlocale(LC_ALL, "C");
    return tc_end();
}
