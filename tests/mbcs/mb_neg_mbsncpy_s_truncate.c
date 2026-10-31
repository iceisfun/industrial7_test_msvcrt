/* TC
 * id:    mb_neg_mbsncpy_s_truncate
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * procs: _mbsncpy_s _mbsnbcpy_s
 * kind:  negative
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _mbsncpy_s, _mbsnbcpy_s: _TRUNCATE returns STRUNCATE (80)
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
#define TRUNC ((size_t)-1)
TC_DECL_PROC(int, _mbsncpy_s, (unsigned char *, size_t, const unsigned char *, size_t));
TC_CASE(truncate_sbcs)
{
    unsigned char dst[8], src[] = "abcdefgh";
    if (!tc_p__mbsncpy_s || !tc_iph_install()) TC_SKIP("unavailable");
    fill(dst, 8); tc_iph_count = 0;
    CHECK_EQ_I(tc_p__mbsncpy_s(dst, 5, src, TRUNC), 80);
    CHECK_STR((char*)dst, "abcd");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_DECL_PROC(int, _mbsnbcpy_s, (unsigned char *, size_t, const unsigned char *, size_t));
TC_CASE(truncate_nb_sbcs)
{
    unsigned char dst[8], src[] = "abcdefgh";
    if (!tc_p__mbsnbcpy_s || !tc_iph_install()) TC_SKIP("unavailable");
    fill(dst, 8); tc_iph_count = 0;
    CHECK_EQ_I(tc_p__mbsnbcpy_s(dst, 5, src, TRUNC), 80);
    CHECK_STR((char*)dst, "abcd");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(range)
{
    unsigned char dst[8], src[] = "abcdefgh";
    if (!tc_p__mbsncpy_s || !tc_iph_install()) TC_SKIP("unavailable");
    fill(dst, 8); tc_iph_count = 0;
    CHECK_EQ_I(tc_p__mbsncpy_s(dst, 4, src, 8), ERANGE);
    CHECK_EQ_U(dst[0], 0);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_dst)
{
    unsigned char src[] = "abc";
    if (!tc_p__mbsncpy_s || !tc_iph_install()) TC_SKIP("unavailable");
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__mbsncpy_s(NULL, 8, src, 2), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); old_cp = _getmbcp(); TC_LOAD_PROC(_mbsncpy_s); TC_LOAD_PROC(_mbsnbcpy_s); 
    TC_RUN(cp); TC_RUN(truncate_sbcs); TC_RUN(truncate_nb_sbcs); TC_RUN(range); TC_RUN(null_dst);
    _setmbcp(old_cp); return tc_end();
}
