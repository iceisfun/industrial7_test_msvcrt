/* TC
 * id:    mb_neg_mbccpy_null
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * procs: _mbccpy_s _mbccpy_s_l
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _mbccpy_s, _mbccpy_s_l: EINVAL for NULL dst/src or zero size, ERANGE if the buffer cannot hold the char
 * note:  cp932. Legacy _mbccpy NULL handling is in mb_neg_probe_mbccpy_null_legacy.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

typedef int (__cdecl *ccpy_s_t)(unsigned char *, size_t, int *, const unsigned char *);
typedef int (__cdecl *ccpy_s_l_t)(unsigned char *, size_t, int *, const unsigned char *, void *);
static ccpy_s_t p_s; static ccpy_s_l_t p_s_l;
static int old_cp;
static unsigned char *volatile v_null;
static const unsigned char *volatile v_cnull;
static const unsigned char dbcs[] = {0x82, 0xA0, 0};

TC_CASE(s_null_dst)
{
    int n = 99;
    if (!p_s) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(p_s(v_null, 4, &n, dbcs), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(s_null_src)
{
    unsigned char dst[4] = {'x', 'x', 'x', 'x'}; int n = 99;
    if (!p_s) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(p_s(dst, 4, &n, v_cnull), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(s_size_one_dbcs_erange)
{
    unsigned char dst[4] = {'x', 'x', 'x', 'x'}; int n = 99;
    if (!p_s) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(p_s(dst, 1, &n, dbcs), ERANGE);
    CHECK_EQ_U(dst[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(s_size_zero_einval)
{
    unsigned char dst[4] = {'x', 'x', 'x', 'x'}; int n = 99;
    if (!p_s) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(p_s(dst, 0, &n, dbcs), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_U(dst[0], 'x');
}
TC_CASE(s_null_pcopied_ok)
{
    unsigned char dst[4] = {'x', 'x', 'x', 'x'};
    if (!p_s) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(p_s(dst, 4, NULL, dbcs), 0);
    CHECK_EQ_U(dst[0], 0x82); CHECK_EQ_U(dst[1], 0xA0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(s_l_dbcs_success_and_null_dst)
{
    unsigned char dst[4] = {'x', 'x', 'x', 'x'}; int n = 99;
    if (!p_s_l) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(p_s_l(dst, 4, &n, dbcs, NULL), 0); CHECK_EQ_I(n, 2); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(p_s_l(v_null, 4, &n, dbcs, NULL), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_s = (ccpy_s_t)tc_crt_proc("_mbccpy_s"); p_s_l = (ccpy_s_l_t)tc_crt_proc("_mbccpy_s_l");
    old_cp = _getmbcp(); _setmbcp(932);
    TC_RUN(s_null_dst); TC_RUN(s_null_src); TC_RUN(s_size_one_dbcs_erange); TC_RUN(s_size_zero_einval);
    TC_RUN(s_null_pcopied_ok); TC_RUN(s_l_dbcs_success_and_null_dst);
    _setmbcp(old_cp);
    return tc_end();
}
