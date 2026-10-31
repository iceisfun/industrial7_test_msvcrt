/* TC
 * id:    loc_probe_get_current_locale_snapshot
 * area:  locale
 * funcs: _get_current_locale _free_locale setlocale
 * procs: _isalpha_l
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   none
 * note:  PROBE: a _locale_t from _get_current_locale is a snapshot: it keeps its classification after the global locale
 *        is switched back to C, and a snapshot taken in C is unaffected by later switching to 1252.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

TC_DECL_PROC(int, _isalpha_l, (int, _locale_t));
static const char *volatile n_1252 = "English_United States.1252";
static const char *volatile n_c = "C";
static volatile int ch = 0xE9;

TC_CASE(snapshot_survives_switch_back)
{
    _locale_t l;
    if (!setlocale(LC_ALL, n_1252)) { setlocale(LC_ALL, n_c); TC_SKIP("1252 locale unavailable"); }
    l = _get_current_locale();
    setlocale(LC_ALL, n_c);
    CHECK(l != NULL); if (!l) return;
    CHECK(tc_p__isalpha_l(ch, l) != 0);
    _free_locale(l);
}
TC_CASE(c_snapshot_survives_switch_to_1252)
{
    _locale_t l = _get_current_locale();
    CHECK(l != NULL);
    if (!setlocale(LC_ALL, n_1252)) { setlocale(LC_ALL, n_c); if (l) _free_locale(l); TC_SKIP("1252 locale unavailable"); }
    if (l) { CHECK_EQ_I(tc_p__isalpha_l(ch, l) != 0, 0); _free_locale(l); }
    setlocale(LC_ALL, n_c);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_isalpha_l); if (!tc_p__isalpha_l) { tc_write("TC FAIL " TC_TEST_ID "::_isalpha_l missing\n"); return 1; }
    TC_RUN(snapshot_survives_switch_back); TC_RUN(c_snapshot_survives_switch_to_1252);
    setlocale(LC_ALL, "C");
    return tc_end();
}
