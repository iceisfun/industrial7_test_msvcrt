/* TC
 * id:    mb_neg_mbscat_s_overflow
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * procs: _mbscat_s
 * kind:  negative
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mbscat_s: ERANGE, dest[0] set to 0, invalid parameter handler invoked
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_DECL_PROC(int, _mbscat_s, (unsigned char *, size_t, const unsigned char *));
TC_CASE(too_small)
{
    unsigned char dst[8] = {'a','b',0}, src[] = {'c','d',0};
    if (!tc_p__mbscat_s || !tc_iph_install()) TC_SKIP("unavailable");
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__mbscat_s(dst, 4, src), ERANGE);
    CHECK_EQ_U(dst[0], 0);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(exact_fit)
{
    unsigned char dst[8] = {'a','b',0}, src[] = {'c','d',0};
    if (!tc_p__mbscat_s || !tc_iph_install()) TC_SKIP("unavailable");
    tc_iph_count = 0;
    CHECK_EQ_I(tc_p__mbscat_s(dst, 5, src), 0);
    CHECK_STR((char*)dst, "abcd");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); old_cp = _getmbcp(); TC_LOAD_PROC(_mbscat_s);
    TC_RUN(cp); TC_RUN(too_small); TC_RUN(exact_fit);
    _setmbcp(old_cp); return tc_end();
}
