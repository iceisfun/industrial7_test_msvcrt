/* TC
 * id:    ct_neg_probe_l_variants_wrong_locale_state
 * area:  ctype
 * kind:  negative
 * funcs: isalpha toupper setlocale
 * procs: _create_locale _free_locale _isalpha_l _toupper_l
 * crt:   >=100
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _create_locale, _isalpha_l, _toupper_l
 * note:  PROBES that an explicit "C" locale object is independent of the global cp1252 locale: the _l variants use
 *        the object, not the global state (0xE9 is alpha/maps to 0xC9 only in cp1252). Skipped if .1252 unavailable.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

TC_DECL_PROC(void *, _create_locale, (int, const char *));
TC_DECL_PROC(void, _free_locale, (void *));
TC_DECL_PROC(int, _isalpha_l, (int, void *));
TC_DECL_PROC(int, _toupper_l, (int, void *));
static volatile int vc;

TC_CASE(c_object_unaffected_by_global_cp1252)
{
    void *loc;
    if (!tc_p__create_locale || !tc_p__free_locale || !tc_p__isalpha_l || !tc_p__toupper_l) TC_SKIP("missing");
    loc = tc_p__create_locale(LC_ALL, "C");
    if (!loc) TC_SKIP("no C locale object");
    if (!setlocale(LC_ALL, ".1252")) { tc_p__free_locale(loc); TC_SKIP("cp1252 unavailable"); }
    vc = 0xE9;
    CHECK(isalpha(vc) != 0);
    CHECK_EQ_I(toupper(vc), 0xC9);
    CHECK_EQ_I(tc_p__isalpha_l(vc, loc), 0);
    CHECK_EQ_I(tc_p__toupper_l(vc, loc), 0xE9);
    setlocale(LC_ALL, "C");
    tc_p__free_locale(loc);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_create_locale); TC_LOAD_PROC(_free_locale); TC_LOAD_PROC(_isalpha_l); TC_LOAD_PROC(_toupper_l);
    TC_RUN(c_object_unaffected_by_global_cp1252);
    return tc_end();
}
