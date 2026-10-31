/* TC
 * id:    wcs_neg_wcsxfrm_l_params
 * area:  wstring
 * funcs:
 * procs: _wcsxfrm_l
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strxfrm, wcsxfrm, _strxfrm_l, _wcsxfrm_l; C11 7.29.4.4.4
 * note:  src NULL / dest NULL with n>0: EINVAL, handler, return INT_MAX. dest NULL with n==0 = length query. Nothing is written
 *        at or beyond dest[n]; contents of dest after truncation are indeterminate and not asserted. C locale: length == wcslen.
 */
#include "tc.h"
#include <errno.h>
#include <limits.h>

TC_DECL_PROC(size_t, _wcsxfrm_l, (wchar_t *, const wchar_t *, size_t, void *));
static const wchar_t *volatile ps = L"abcdef";
static const wchar_t *volatile pn = NULL;
static wchar_t *volatile pd = NULL;
static void *volatile ploc = NULL;
#define PRE() do { if (!tc_p__wcsxfrm_l) TC_SKIP("not exported"); tc_iph_count = 0; errno = 0; } while (0)

TC_CASE(src_null)
{
    wchar_t d[4] = {0}; wchar_t *volatile dp = d; size_t r; PRE();
    r = tc_p__wcsxfrm_l(dp, pn, 4, ploc);
    CHECK_EQ_U(r, INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(dest_null_n_pos)
{
    size_t r; PRE();
    r = tc_p__wcsxfrm_l(pd, ps, 4, ploc);
    CHECK_EQ_U(r, INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(dest_null_n_zero_ok)
{
    size_t r; PRE();
    r = tc_p__wcsxfrm_l(pd, ps, 0, ploc);
    CHECK_EQ_U(r, 6); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(n_zero_dest_untouched)
{
    wchar_t d[4] = { 0x5555, 0x5555, 0x5555, 0x5555 }; wchar_t *volatile dp = d; size_t r; PRE();
    r = tc_p__wcsxfrm_l(dp, ps, 0, ploc);
    CHECK_EQ_U(r, 6); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(d[0], 0x5555); CHECK_EQ_I(d[1], 0x5555); CHECK_EQ_I(d[3], 0x5555);
}
TC_CASE(truncate_no_write_beyond_n)
{
    wchar_t d[10]; wchar_t *volatile dp = d; size_t r; int i; PRE();
    for (i = 0; i < 10; i++) d[i] = 0x5555;
    r = tc_p__wcsxfrm_l(dp, ps, 3, ploc);
    CHECK_EQ_U(r, 6);   /* required length, not the stored length */
    for (i = 3; i < 10; i++) CHECK_EQ_I(d[i], 0x5555);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_wcsxfrm_l);
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(src_null); TC_RUN(dest_null_n_pos); TC_RUN(dest_null_n_zero_ok);
    TC_RUN(n_zero_dest_untouched); TC_RUN(truncate_no_write_beyond_n);
    return tc_end();
}
