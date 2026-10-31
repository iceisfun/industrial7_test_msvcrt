/* TC
 * id:    wcs_neg_wcsnset_s_overflow
 * area:  wstring
 * funcs:
 * procs: _wcsnset_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strnset_s, _wcsnset_s: fills at most count characters, stops at the terminating NUL
 * note:  The "buffer too small for the string" EINVAL case is probed separately (wcs_neg_wcsnset_s_size_small_probe).
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

TC_DECL_PROC(errno_t, _wcsnset_s, (wchar_t *, size_t, wchar_t, size_t));

TC_CASE(huge_count_stops_at_nul)
{
    wchar_t b[8] = { L'a', L'b', L'c', 0, L'Q', L'Q', L'Q', L'Q' }; int r;
    if (!tc_p__wcsnset_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0;
    r = tc_p__wcsnset_s(b, 8, L'x', (size_t)-1);
    CHECK_EQ_I(r, 0); CHECK_WSTR(b, L"xxx"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count0_unchanged)
{
    wchar_t b[8] = { L'a', L'b', L'c', 0 }; int r;
    if (!tc_p__wcsnset_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0;
    r = tc_p__wcsnset_s(b, 8, L'x', 0);
    CHECK_EQ_I(r, 0); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(canary_beyond_nul_intact)
{
    wchar_t b[8] = { L'a', L'b', 0, L'Q', L'Q', L'Q', L'Q', L'Q' }; int r, i;
    if (!tc_p__wcsnset_s || !tc_iph_install()) TC_SKIP("missing");
    r = tc_p__wcsnset_s(b, 8, L'x', 100);
    CHECK_EQ_I(r, 0); CHECK_WSTR(b, L"xx"); CHECK_EQ_I(b[2], 0);
    for (i = 3; i < 8; i++) CHECK_EQ_I(b[i], L'Q');
}
TC_CASE(null_and_size0_einval)
{
    wchar_t b[4] = { L'a', 0 }; int r;
    if (!tc_p__wcsnset_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0;
    r = tc_p__wcsnset_s(NULL, 4, L'x', 1); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    r = tc_p__wcsnset_s(b, 0, L'x', 1);    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 2);
    CHECK_EQ_I(b[0], L'a');
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_wcsnset_s);
    TC_RUN(huge_count_stops_at_nul); TC_RUN(count0_unchanged); TC_RUN(canary_beyond_nul_intact); TC_RUN(null_and_size0_einval);
    return tc_end();
}
