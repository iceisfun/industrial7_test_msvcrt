/* TC
 * id:    wcs_neg_wcslwr_s_small
 * area:  wstring
 * funcs:
 * procs: _wcslwr_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strlwr_s, _wcslwr_s, _mbslwr_s
 * note:  Docs: EINVAL if str is NULL or not NUL-terminated within sizeInWords; invalid parameter handler invoked.
 *        The state of the buffer after failure is not asserted here (see wcs_neg_wcslwr_s_clear_probe).
 */
#include "tc.h"
#include <errno.h>

TC_DECL_PROC(int, _wcslwr_s, (wchar_t *, size_t));

TC_CASE(too_small)
{
    wchar_t b[8] = { L'A', L'B', L'C', 0 };
    int r;
    if (!tc_p__wcslwr_s) TC_SKIP("not exported");
    tc_iph_count = 0; errno = 0;
    r = tc_p__wcslwr_s(b, 3);   /* needs 4 */
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_zero)
{
    wchar_t b[4] = { L'A', L'B', 0, 0 };
    int r;
    if (!tc_p__wcslwr_s) TC_SKIP("not exported");
    tc_iph_count = 0; errno = 0;
    r = tc_p__wcslwr_s(b, 0);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_str)
{
    int r;
    if (!tc_p__wcslwr_s) TC_SKIP("not exported");
    tc_iph_count = 0; errno = 0;
    r = tc_p__wcslwr_s(NULL, 10);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(exact_fit)
{
    wchar_t b[8] = { L'A', L'B', L'C', 0, L'Z', L'Z', L'Z', 0 };
    int r;
    if (!tc_p__wcslwr_s) TC_SKIP("not exported");
    tc_iph_count = 0; errno = 0;
    r = tc_p__wcslwr_s(b, 4);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(tc_iph_count, 0);
    CHECK(b[0] == L'a' && b[1] == L'b' && b[2] == L'c' && b[3] == 0 && b[4] == L'Z');
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_wcslwr_s);
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(too_small); TC_RUN(size_zero); TC_RUN(null_str); TC_RUN(exact_fit);
    return tc_end();
}
