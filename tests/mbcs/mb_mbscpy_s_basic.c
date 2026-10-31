/* TC
 * id:    mb_mbscpy_s_basic
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * procs: _mbscpy_s _mbscat_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mbscpy_s, _mbscpy_s_l, _mbscat_s
 * note:  procs because mingw's libmsvcr80 lacks the _s functions
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_DECL_PROC(int, _mbscpy_s, (unsigned char *, size_t, const unsigned char *));
TC_DECL_PROC(int, _mbscat_s, (unsigned char *, size_t, const unsigned char *));
TC_CASE(copy_exact)
{
    unsigned char src[] = {0x82,0xA0,0x82,0xA2,0}, dst[5];
    CHECK(tc_p__mbscpy_s); if (!tc_p__mbscpy_s) return;
    CHECK_EQ_I(tc_p__mbscpy_s(dst, 5, src), 0);
    CHECK_MEM(dst, src, 5);
}
TC_CASE(cat)
{
    unsigned char dst[16] = {0x82,0xA0,0}, src[] = {'x',0x82,0xA2,0}, e[] = {0x82,0xA0,'x',0x82,0xA2,0};
    CHECK(tc_p__mbscat_s); if (!tc_p__mbscat_s) return;
    CHECK_EQ_I(tc_p__mbscat_s(dst, 16, src), 0);
    CHECK_MEM(dst, e, 6);
}
int main(void)
{
    tc_begin(); old_cp = _getmbcp();
    TC_LOAD_PROC(_mbscpy_s); TC_LOAD_PROC(_mbscat_s);
    TC_RUN(cp); TC_RUN(copy_exact); TC_RUN(cat);
    _setmbcp(old_cp); return tc_end();
}
