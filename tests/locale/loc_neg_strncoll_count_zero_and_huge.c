/* TC
 * id:    loc_neg_strncoll_count_zero_and_huge
 * area:  locale
 * funcs: _strncoll_l _wcsncoll_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  inferred
 * ref:   C11 7.24.4.4 (n==0 compares nothing); MS docs _strncoll, _wcsncoll
 * note:  count==0 on non-NULL strings: returns 0 without invoking the handler. NULL-with-count-0 deliberately not asserted.
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <locale.h>

static const char *volatile v_a = "abc";
static const char *volatile v_b = "xyz";
static const wchar_t *volatile v_wa = L"abc";
static const wchar_t *volatile v_wb = L"xyz";
static _locale_t loc;

TC_CASE(count0_nonnull_returns0)
{
    CHECK_EQ_I(_strncoll_l(v_a, v_b, 0, loc), 0);
    CHECK_EQ_I(_wcsncoll_l(v_wa, v_wb, 0, loc), 0);
    CHECK(_strncoll_l(v_a, v_b, 1, loc) < 0);
    CHECK(_wcsncoll_l(v_wb, v_wa, 1, loc) > 0);
}
TC_CASE(no_iph)
{
    if (!tc_iph_install()) TC_SKIP("no IPH api");
    tc_iph_count = 0;
    _strncoll_l(v_a, v_b, 0, loc); _wcsncoll_l(v_wa, v_wb, 0, loc);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(count0_nonnull_returns0); TC_RUN(no_iph);
    _free_locale(loc);
    return tc_end();
}
