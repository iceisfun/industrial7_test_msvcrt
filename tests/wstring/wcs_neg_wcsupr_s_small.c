/* TC
 * id:    wcs_neg_wcsupr_s_small
 * area:  wstring
 * funcs:
 * procs: _wcsupr_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strupr_s, _wcsupr_s, _mbsupr_s
 * note:  Docs: EINVAL if str is NULL or not NUL-terminated within sizeInWords; invalid parameter handler invoked.
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, _wcsupr_s, (wchar_t *, size_t));

TC_CASE(too_small)
{
    wchar_t b[8] = { L'a', L'b', L'c', 0 };
    int r;
    if (!tc_p__wcsupr_s) TC_SKIP("not exported");
    tc_iph_count = 0; errno = 0;
    r = tc_p__wcsupr_s(b, 3);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_str)
{
    int r;
    if (!tc_p__wcsupr_s) TC_SKIP("not exported");
    tc_iph_count = 0; errno = 0;
    r = tc_p__wcsupr_s(NULL, 5);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_zero)
{
    wchar_t b[4] = { L'a', 0 };
    int r;
    if (!tc_p__wcsupr_s) TC_SKIP("not exported");
    tc_iph_count = 0; errno = 0;
    r = tc_p__wcsupr_s(b, 0);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(unterminated)
{
    wchar_t b[4] = { L'a', L'b', L'c', L'd' };
    int r;
    if (!tc_p__wcsupr_s) TC_SKIP("not exported");
    tc_iph_count = 0; errno = 0;
    r = tc_p__wcsupr_s(b, 4);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(exact_fit)
{
    wchar_t b[4] = { L'a', L'b', L'c', 0 };
    int r;
    if (!tc_p__wcsupr_s) TC_SKIP("not exported");
    tc_iph_count = 0;
    r = tc_p__wcsupr_s(b, 4);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0);
    CHECK(b[0] == L'A' && b[1] == L'B' && b[2] == L'C' && b[3] == 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_wcsupr_s);
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(too_small); TC_RUN(null_str); TC_RUN(size_zero); TC_RUN(unterminated); TC_RUN(exact_fit);
    return tc_end();
}
