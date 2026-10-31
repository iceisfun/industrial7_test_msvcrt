/* TC
 * id:    mb_neg_mbsnbcnt_null_probe
 * area:  mbcs
 * funcs: _mbsnbcnt _mbsnccnt _setmbcp _getmbcp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbsnbcnt, _mbsnccnt
 * note:  PROBE: NULL string (count 2) -> returns 0, errno EINVAL, handler called. CP932 selected so the validating MBCS path runs.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static const unsigned char * volatile np = NULL;

TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(nbcnt_null_iph)
{
    errno = 0; tc_iph_count = 0;
    CHECK_EQ_U(_mbsnbcnt(np, 2), 0);
    CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
TC_CASE(nccnt_null_iph)
{
    errno = 0; tc_iph_count = 0;
    CHECK_EQ_U(_mbsnccnt(np, 2), 0);
    CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
}
int main(void)
{
    tc_begin(); tc_iph_install(); old_cp = _getmbcp();
    TC_RUN(cp); TC_RUN(nbcnt_null_iph); TC_RUN(nccnt_null_iph); 
    _setmbcp(old_cp); return tc_end();
}
