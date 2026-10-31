/* TC
 * id:    wcs_neg_wcsxfrm_params
 * area:  wstring
 * funcs: wcsxfrm
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strxfrm, wcsxfrm, _strxfrm_l: errno EINVAL, return INT_MAX on error
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

TC_CASE(src_null)
{
    wchar_t d[4] = {0}; wchar_t * volatile dp = d; const wchar_t * volatile s = NULL; size_t r;
    tc_iph_count = 0; errno = 0;
    r = wcsxfrm(dp, s, 4);
    CHECK_EQ_U(r, INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(dest_null_n_pos)
{
    wchar_t * volatile d = NULL; const wchar_t * volatile s = L"abc"; size_t r;
    tc_iph_count = 0; errno = 0;
    r = wcsxfrm(d, s, 4);
    CHECK_EQ_U(r, INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(dest_null_n_zero_ok)
{
    wchar_t * volatile d = NULL; const wchar_t * volatile s = L"abc"; size_t r;
    tc_iph_count = 0; errno = 0;
    r = wcsxfrm(d, s, 0);
    CHECK_EQ_U(r, 3); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(src_null); TC_RUN(dest_null_n_pos); TC_RUN(dest_null_n_zero_ok);
    return tc_end();
}
