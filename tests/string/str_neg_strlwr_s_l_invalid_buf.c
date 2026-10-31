/* TC
 * id:    str_neg_strlwr_s_l_invalid_buf
 * area:  string
 * funcs: _create_locale _free_locale
 * procs: _strlwr_s_l _strupr_s_l
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strlwr_s, _strlwr_s_l, _strupr_s_l: EINVAL when the buffer is NULL, size is 0, or the string is not null-terminated within size; invalid parameter handler invoked
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <locale.h>
TC_DECL_PROC(int, _strlwr_s_l, (char *, size_t, _locale_t));
TC_DECL_PROC(int, _strupr_s_l, (char *, size_t, _locale_t));
static _locale_t loc;

TC_CASE(null_buf)
{
    char *volatile np = NULL; tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(tc_p__strlwr_s_l(np, 8, loc), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__strupr_s_l(np, 8, loc), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_zero)
{
    char b[8]; strcpy(b, "ABC"); tc_iph_count = 0;
    CHECK_EQ_I(tc_p__strlwr_s_l(b, 0, loc), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__strupr_s_l(b, 0, loc), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(unterminated_in_size)
{
    char b[8]; strcpy(b, "ABCdef"); tc_iph_count = 0; errno = 0;
    CHECK_EQ_I(tc_p__strupr_s_l(b, 3, loc), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    strcpy(b, "ABCdef"); tc_iph_count = 0;
    CHECK_EQ_I(tc_p__strlwr_s_l(b, 3, loc), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(valid_still_works)
{
    char b[8]; strcpy(b, "ABCdef"); tc_iph_count = 0;
    CHECK_EQ_I(tc_p__strlwr_s_l(b, sizeof b, loc), 0); CHECK_STR(b, "abcdef");
    CHECK_EQ_I(tc_p__strupr_s_l(b, 7, loc), 0); CHECK_STR(b, "ABCDEF");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_LOAD_PROC(_strlwr_s_l); TC_LOAD_PROC(_strupr_s_l);
    if (!tc_p__strlwr_s_l || !tc_p__strupr_s_l) { tc_write("TC FAIL " TC_TEST_ID "::__import missing\n"); return 1; }
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__import _create_locale(C) failed\n"); return 1; }
    TC_RUN(null_buf); TC_RUN(size_zero); TC_RUN(unterminated_in_size); TC_RUN(valid_still_works);
    _free_locale(loc);
    return tc_end();
}
