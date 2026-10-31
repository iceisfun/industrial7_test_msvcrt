/* TC
 * id:    mb_neg_mbscpy_s_null
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * procs: _mbscpy_s _mbscat_s
 * kind:  negative
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mbscpy_s, _mbscat_s: EINVAL for NULL dst/src and for an unterminated destination
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_DECL_PROC(int, _mbscpy_s, (unsigned char *, size_t, const unsigned char *));
TC_DECL_PROC(int, _mbscat_s, (unsigned char *, size_t, const unsigned char *));
TC_CASE(cpy_null_dst)
{
    unsigned char src[] = {'a',0};
    if (!tc_p__mbscpy_s || !tc_iph_install()) TC_SKIP("unavailable");
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__mbscpy_s(NULL, 10, src), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(cpy_null_src)
{
    unsigned char dst[10];
    if (!tc_p__mbscpy_s || !tc_iph_install()) TC_SKIP("unavailable");
    fill(dst, 10); tc_iph_count = 0;
    CHECK_EQ_I(tc_p__mbscpy_s(dst, 10, NULL), EINVAL);
    CHECK_EQ_U(dst[0], 0);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(cat_null_dst)
{
    unsigned char src[] = {'a',0};
    if (!tc_p__mbscat_s || !tc_iph_install()) TC_SKIP("unavailable");
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__mbscat_s(NULL, 10, src), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(cat_unterminated)
{
    unsigned char dst[4] = {'a','a','a','a'}, src[] = {'b',0};
    if (!tc_p__mbscat_s || !tc_iph_install()) TC_SKIP("unavailable");
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__mbscat_s(dst, 4, src), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); old_cp = _getmbcp(); TC_LOAD_PROC(_mbscpy_s); TC_LOAD_PROC(_mbscat_s);
    TC_RUN(cp); TC_RUN(cpy_null_dst); TC_RUN(cpy_null_src); TC_RUN(cat_null_dst); TC_RUN(cat_unterminated);
    _setmbcp(old_cp); return tc_end();
}
