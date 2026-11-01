/* TC
 * id:    loc_neg_get_current_locale_snapshot_not_updated
 * area:  locale
 * funcs: _get_current_locale _free_locale setlocale
 * procs: _isalpha_l _toupper_l
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _get_current_locale (returns a locale object for the current locale); _create_locale objects are independent of later setlocale
 * note:  Win10 triage (class A): "zz" is NOT an invalid locale name on Windows 10 19045 (setlocale(LC_ALL,"zz") returns "zz": any well-formed BCP-47-style
 *        tag is accepted as a custom locale), so the failing name is now "zz_nowhere" (observed NULL). Observed on Windows 10 22H2 build 19045.
 * note:  Inferred: the object is a copy at the time of the call, so a later setlocale does not change its classification.
 *        In C locale 0xE9 is not alpha and _toupper_l leaves it unchanged.
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
 *        [env] user-default locale en-US, build 19045; not stable on a VM with another user locale/NLS data.
 *        [env] the name "zz" is a valid locale name on build 19045 (observed); "zz_nowhere" is rejected (observed).
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

TC_DECL_PROC(int, _isalpha_l, (int, _locale_t));
TC_DECL_PROC(int, _toupper_l, (int, _locale_t));
static const char *volatile n_en = "English_United States.1252";
static const char *volatile n_bad = "zz_nowhere";
static volatile int ch = 0xE9;
static _locale_t snap;

TC_CASE(snapshot_stays_c)
{
    snap = _get_current_locale(); CHECK(snap != NULL); if (!snap) return;
    if (!setlocale(LC_ALL, n_en)) TC_SKIP("1252 unavailable");
    CHECK(isalpha(ch) != 0);
    CHECK_EQ_I(tc_p__isalpha_l(ch, snap), 0);
    CHECK_EQ_I(tc_p__toupper_l(ch, snap), 0xE9);
}
TC_CASE(usable_after_failed_setlocale)
{
    if (!snap) return;
    CHECK(setlocale(LC_ALL, n_bad) == NULL);
    CHECK_EQ_I(tc_p__isalpha_l(ch, snap), 0);
    CHECK_EQ_I(tc_p__isalpha_l('a', snap) != 0, 1);
}
TC_CASE(free_two)
{
    _locale_t b = _get_current_locale();
    CHECK(b != NULL);
    if (snap) _free_locale(snap);
    snap = NULL;
    if (b) { CHECK(tc_p__isalpha_l(ch, b) != 0); _free_locale(b); }
}
int main(void)
{
    tc_begin(); tc_iph_install(); TC_LOAD_PROC(_isalpha_l); TC_LOAD_PROC(_toupper_l);
    if (!tc_p__isalpha_l || !tc_p__toupper_l) { tc_write("TC FAIL " TC_TEST_ID "::procs\n"); return 1; }
    TC_RUN(snapshot_stays_c); TC_RUN(usable_after_failed_setlocale); TC_RUN(free_two);
    setlocale(LC_ALL, "C");
    return tc_end();
}
