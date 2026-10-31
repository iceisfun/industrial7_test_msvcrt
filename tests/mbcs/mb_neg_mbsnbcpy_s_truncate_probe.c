/* TC
 * id:    mb_neg_mbsnbcpy_s_truncate_probe
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * procs: _mbsnbcpy_s _mbsncpy_s
 * kind:  negative
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _mbsnbcpy_s (_TRUNCATE -> STRUNCATE)
 * note:  PROBE: _mbsnbcpy_s(dst, 4, {82 A0 82 A2 0}, _TRUNCATE) returns STRUNCATE (80) and leaves {82 A0 00}
 *        (the split trailing lead byte is dropped); SBCS truncation is the documented part in mb_neg_mbsncpy_s_truncate.
 *        PROBE (2): same for _mbsncpy_s(dst, 4, ...) (character-count variant) leaving {82 A0 00}.
 */
#include "tc.h"
#include <mbstring.h>
static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_DECL_PROC(int, _mbsnbcpy_s, (unsigned char *, size_t, const unsigned char *, size_t));
TC_CASE(dbcs)
{
    unsigned char dst[8], src[] = {0x82,0xA0,0x82,0xA2,0}, e[] = {0x82,0xA0,0};
    if (!tc_p__mbsnbcpy_s || !tc_iph_install()) TC_SKIP("unavailable");
    fill(dst, 8);
    CHECK_EQ_I(tc_p__mbsnbcpy_s(dst, 4, src, (size_t)-1), 80);
    CHECK_MEM(dst, e, sizeof e);
}
TC_DECL_PROC(int, _mbsncpy_s, (unsigned char *, size_t, const unsigned char *, size_t));
TC_CASE(dbcs_nchar)
{
    unsigned char dst[8], src[] = {0x82,0xA0,0x82,0xA2,0}, e[] = {0x82,0xA0,0};
    if (!tc_p__mbsncpy_s || !tc_iph_install()) TC_SKIP("unavailable");
    fill(dst, 8);
    CHECK_EQ_I(tc_p__mbsncpy_s(dst, 4, src, (size_t)-1), 80);
    CHECK_MEM(dst, e, sizeof e);
}
int main(void)
{
    tc_begin(); old_cp = _getmbcp(); TC_LOAD_PROC(_mbsnbcpy_s); TC_LOAD_PROC(_mbsncpy_s);
    TC_RUN(cp); TC_RUN(dbcs); TC_RUN(dbcs_nchar); _setmbcp(old_cp); return tc_end();
}
