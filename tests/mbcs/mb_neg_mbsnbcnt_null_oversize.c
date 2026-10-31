/* TC
 * id:    mb_neg_mbsnbcnt_null_oversize
 * area:  mbcs
 * funcs: _mbsnbcnt _mbsnccnt _setmbcp _getmbcp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbsnbcnt, _mbsnccnt
 * note:  counting stops at the terminating NUL; count 0 and empty string give 0; no handler call. (NULL cases: mb_neg_mbsnbcnt_null_probe.)
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static unsigned char s[] = {0x82,0xA0,'a',0};   /* CP932: 1 DBCS char + 'a' = 2 chars, 3 bytes */
static unsigned char e[] = {0};

TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(nbcnt_count_gt_len_stops_at_nul)
{
    tc_iph_count = 0;
    CHECK_EQ_U(_mbsnbcnt(s, 100), 3);   /* bytes in at most 100 chars */
    CHECK_EQ_U(_mbsnbcnt(s, 3), 3);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(nccnt_count_gt_len_stops_at_nul)
{
    tc_iph_count = 0;
    CHECK_EQ_U(_mbsnccnt(s, 100), 2);   /* chars in at most 100 bytes */
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(nbcnt_zero_count) { CHECK_EQ_U(_mbsnbcnt(s, 0), 0); }
TC_CASE(nccnt_zero_count) { CHECK_EQ_U(_mbsnccnt(s, 0), 0); }
TC_CASE(empty_string)
{
    CHECK_EQ_U(_mbsnbcnt(e, 5), 0);
    CHECK_EQ_U(_mbsnccnt(e, 5), 0);
}
int main(void)
{
    tc_begin(); tc_iph_install(); old_cp = _getmbcp();
    TC_RUN(cp); TC_RUN(nbcnt_count_gt_len_stops_at_nul);
    TC_RUN(nccnt_count_gt_len_stops_at_nul); TC_RUN(nbcnt_zero_count); TC_RUN(nccnt_zero_count); TC_RUN(empty_string);
    _setmbcp(old_cp); return tc_end();
}
