/* TC
 * id:    mem_neg_wmemcpy_s_params
 * area:  memory
 * kind:  negative
 * funcs:
 * procs: wmemcpy_s wmemmove_s
 * crt:   >=100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs wmemcpy_s, wmemmove_s (error conditions table: EINVAL for NULL dest/src, ERANGE when count > size)
 * note:  Sizes are in wchar_t elements, not bytes. Contents of dest after failure are not asserted.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *wcp_t)(wchar_t *, size_t, const wchar_t *, size_t);
static wcp_t p_cpy, p_mov;
static wchar_t src[8] = { 1, 2, 3, 4, 5, 6, 7, 8 }, dst[8];

TC_CASE(wmemcpy_s_dest_null_EINVAL)
{
    int r; if (!p_cpy) TC_SKIP("not exported");
    tc_iph_count = 0; r = p_cpy(NULL, 8, src, 4);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wmemcpy_s_src_null_EINVAL)
{
    int r; if (!p_cpy) TC_SKIP("not exported");
    tc_iph_count = 0; r = p_cpy(dst, 8, NULL, 4);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wmemcpy_s_range_ERANGE)
{
    int r; if (!p_cpy) TC_SKIP("not exported");
    tc_iph_count = 0; r = p_cpy(dst, 4, src, 5);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wmemmove_s_dest_null_EINVAL)
{
    int r; if (!p_mov) TC_SKIP("not exported");
    tc_iph_count = 0; r = p_mov(NULL, 8, src, 4);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wmemmove_s_range_ERANGE)
{
    int r; if (!p_mov) TC_SKIP("not exported");
    tc_iph_count = 0; r = p_mov(dst, 4, src, 5);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    p_cpy = (wcp_t)tc_crt_proc("wmemcpy_s"); p_mov = (wcp_t)tc_crt_proc("wmemmove_s");
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup no handler API\n"); return 1; }
    TC_RUN(wmemcpy_s_dest_null_EINVAL); TC_RUN(wmemcpy_s_src_null_EINVAL); TC_RUN(wmemcpy_s_range_ERANGE);
    TC_RUN(wmemmove_s_dest_null_EINVAL); TC_RUN(wmemmove_s_range_ERANGE);
    return tc_end();
}
