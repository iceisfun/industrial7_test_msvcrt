/* TC
 * id:    loc_neg_strcoll_null_args
 * area:  locale
 * funcs: strcoll wcscoll
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcoll, wcscoll (_NLSCMPERROR on error)
 * note:  NULL arguments: returns _NLSCMPERROR (INT_MAX), errno EINVAL, invalid-parameter handler invoked.
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static const char *volatile v_null_s;
static const wchar_t *volatile v_null_w;
static const char *volatile v_a = "a";
static const wchar_t *volatile v_wa = L"a";

TC_CASE(strcoll_first_null) { tc_iph_install(); errno = 0; CHECK_EQ_I(strcoll(v_null_s, v_a), _NLSCMPERROR); }
TC_CASE(strcoll_second_null) { tc_iph_install(); errno = 0; CHECK_EQ_I(strcoll(v_a, v_null_s), _NLSCMPERROR); }
TC_CASE(wcscoll_first_null) { tc_iph_install(); errno = 0; CHECK_EQ_I(wcscoll(v_null_w, v_wa), _NLSCMPERROR); }
TC_CASE(wcscoll_second_null) { tc_iph_install(); errno = 0; CHECK_EQ_I(wcscoll(v_wa, v_null_w), _NLSCMPERROR); }
TC_CASE(errno_einval)
{
    tc_iph_install();
    errno = 0; strcoll(v_null_s, v_a); CHECK_EQ_I(errno, EINVAL);
    errno = 0; strcoll(v_a, v_null_s); CHECK_EQ_I(errno, EINVAL);
    errno = 0; wcscoll(v_null_w, v_wa); CHECK_EQ_I(errno, EINVAL);
    errno = 0; wcscoll(v_wa, v_null_w); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(iph_count_4)
{
    if (!tc_iph_install()) TC_SKIP("no IPH api");
    tc_iph_count = 0;
    strcoll(v_null_s, v_a); strcoll(v_a, v_null_s); wcscoll(v_null_w, v_wa); wcscoll(v_wa, v_null_w);
    CHECK_EQ_I(tc_iph_count, 4);
}
int main(void)
{
    tc_begin();
    TC_RUN(strcoll_first_null); TC_RUN(strcoll_second_null); TC_RUN(wcscoll_first_null); TC_RUN(wcscoll_second_null);
    TC_RUN(errno_einval); TC_RUN(iph_count_4);
    return tc_end();
}
