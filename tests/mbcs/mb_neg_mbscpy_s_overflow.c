/* TC
 * id:    mb_neg_mbscpy_s_overflow
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * procs: _mbscpy_s
 * kind:  negative
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mbscpy_s: ERANGE when dest too small (dst[0] set to 0), EINVAL when size is 0
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_DECL_PROC(int, _mbscpy_s, (unsigned char *, size_t, const unsigned char *));
TC_CASE(too_small)
{
    unsigned char src[] = {0x82,0xA0,0x82,0xA2,0}, dst[8];
    CHECK(tc_p__mbscpy_s); if (!tc_p__mbscpy_s) return;
    if (!tc_iph_install()) TC_SKIP("no iph");
    fill(dst, 8); tc_iph_count = 0;
    CHECK_EQ_I(tc_p__mbscpy_s(dst, 3, src), ERANGE);
    CHECK_EQ_U(dst[0], 0);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_zero)
{
    unsigned char src[] = {0x82,0xA0,0}, dst[8];
    CHECK(tc_p__mbscpy_s); if (!tc_p__mbscpy_s) return;
    if (!tc_iph_install()) TC_SKIP("no iph");
    fill(dst, 8); tc_iph_count = 0;
    CHECK_EQ_I(tc_p__mbscpy_s(dst, 0, src), EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); old_cp = _getmbcp(); TC_LOAD_PROC(_mbscpy_s);
    TC_RUN(cp); TC_RUN(too_small); TC_RUN(size_zero);
    _setmbcp(old_cp); return tc_end();
}
