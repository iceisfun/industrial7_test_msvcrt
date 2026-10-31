/* TC
 * id:    loc_create_locale_independent_of_global
 * area:  locale
 * funcs: _create_locale _free_locale  _tolower_l _toupper_l setlocale
 * procs: isalpha _isalpha_l _isupper_l
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _create_locale; _isalpha_l; _tolower_l; Windows-1252: 0xC9 = E-acute capital, 0xE9 = e-acute small
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

typedef int (__cdecl *isalpha_t)(int);
static isalpha_t p_isalpha;
TC_DECL_PROC(int, _isalpha_l, (int, _locale_t));
TC_DECL_PROC(int, _isupper_l, (int, _locale_t));
static const char *volatile n_1252 = "English_United States.1252";
static volatile int ch_up = 0xC9, ch_lo = 0xE9;
static _locale_t loc;

TC_CASE(global_c_not_alpha)
{
    CHECK(p_isalpha != NULL); if (!p_isalpha) return;
    CHECK_EQ_I(p_isalpha(ch_up) != 0, 0);
    CHECK_EQ_I(p_isalpha(ch_lo) != 0, 0);
}
TC_CASE(loc_1252_alpha)
{
    if (!loc) TC_SKIP("1252 locale unavailable");
    CHECK(tc_p__isalpha_l(ch_up, loc) != 0); CHECK(tc_p__isalpha_l(ch_lo, loc) != 0);
    CHECK(tc_p__isupper_l(ch_up, loc) != 0); CHECK(tc_p__isupper_l(ch_lo, loc) == 0);
}
TC_CASE(upper_lower_mapping)
{
    if (!loc) TC_SKIP("1252 locale unavailable");
    CHECK_EQ_I(_tolower_l(ch_up, loc), 0xE9);
    CHECK_EQ_I(_toupper_l(ch_lo, loc), 0xC9);
}
TC_CASE(global_still_c)
{
    if (!p_isalpha) return;
    CHECK_EQ_I(p_isalpha(ch_up) != 0, 0);
    CHECK_STR(setlocale(LC_CTYPE, NULL), "C");
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_isalpha_l); TC_LOAD_PROC(_isupper_l); if (!tc_p__isalpha_l) { tc_write("TC FAIL " TC_TEST_ID "::_isalpha_l missing\n"); return 1; }
    p_isalpha = (isalpha_t)tc_crt_proc("isalpha");
    loc = _create_locale(LC_ALL, n_1252);
    TC_RUN(global_c_not_alpha); TC_RUN(loc_1252_alpha); TC_RUN(upper_lower_mapping); TC_RUN(global_still_c);
    if (loc) _free_locale(loc);
    return tc_end();
}
