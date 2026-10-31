/* TC
 * id:    loc_neg_strcoll_l_null_args
 * area:  locale
 * funcs: _strcoll_l _stricoll_l _strncoll_l _strnicoll_l _wcscoll_l _wcsicoll_l _wcsncoll_l _wcsnicoll_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strcoll_l, _stricoll_l, _strncoll_l, _wcscoll_l (_NLSCMPERROR on error)
 * note:  NULL string pointer in each *coll_l family member: INT_MAX, EINVAL, IPH.
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>
#include <limits.h>

static const char *volatile v_null_s;
static const wchar_t *volatile v_null_w;
static const char *volatile v_a = "a";
static const wchar_t *volatile v_wa = L"a";
static _locale_t loc;

#define NEG(call) do { int r_; errno = 0; tc_iph_count = 0; r_ = (call); \
    CHECK_EQ_I(r_, _NLSCMPERROR); CHECK_EQ_I(errno, EINVAL); \
    if (have_iph) CHECK_EQ_I(tc_iph_count, 1); } while (0)
static int have_iph;

TC_CASE(strcoll_l_null) { NEG(_strcoll_l(v_null_s, v_a, loc)); NEG(_strcoll_l(v_a, v_null_s, loc)); }
TC_CASE(stricoll_l_null) { NEG(_stricoll_l(v_null_s, v_a, loc)); NEG(_stricoll_l(v_a, v_null_s, loc)); }
TC_CASE(strncoll_l_null) { NEG(_strncoll_l(v_null_s, v_a, 1, loc)); NEG(_strncoll_l(v_a, v_null_s, 1, loc)); }
TC_CASE(strnicoll_l_null) { NEG(_strnicoll_l(v_null_s, v_a, 1, loc)); NEG(_strnicoll_l(v_a, v_null_s, 1, loc)); }
TC_CASE(wcscoll_l_null) { NEG(_wcscoll_l(v_null_w, v_wa, loc)); NEG(_wcscoll_l(v_wa, v_null_w, loc)); }
TC_CASE(wcsicoll_l_null) { NEG(_wcsicoll_l(v_null_w, v_wa, loc)); NEG(_wcsicoll_l(v_wa, v_null_w, loc)); }
TC_CASE(wcsncoll_l_null) { NEG(_wcsncoll_l(v_null_w, v_wa, 1, loc)); NEG(_wcsncoll_l(v_wa, v_null_w, 1, loc)); }
TC_CASE(wcsnicoll_l_null) { NEG(_wcsnicoll_l(v_null_w, v_wa, 1, loc)); NEG(_wcsnicoll_l(v_wa, v_null_w, 1, loc)); }
int main(void)
{
    tc_begin();
    have_iph = tc_iph_install();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(strcoll_l_null); TC_RUN(stricoll_l_null); TC_RUN(strncoll_l_null); TC_RUN(strnicoll_l_null);
    TC_RUN(wcscoll_l_null); TC_RUN(wcsicoll_l_null); TC_RUN(wcsncoll_l_null); TC_RUN(wcsnicoll_l_null);
    _free_locale(loc);
    return tc_end();
}
