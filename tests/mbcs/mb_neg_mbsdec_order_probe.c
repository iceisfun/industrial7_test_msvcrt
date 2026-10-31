/* TC
 * id:    mb_neg_mbsdec_order_probe
 * area:  mbcs
 * funcs: _mbsinc _mbsdec _mbclen
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbsdec
 * note:  PROBE: _mbsdec with start > current returns NULL (handler count not asserted); _mbsinc on a NUL byte advances by 1;
 *        _mbclen on a NUL byte returns 1.
 */
#include "tc.h"
#include <mbstring.h>
static const unsigned char abc[] = "abc";
static const unsigned char * volatile ap = abc;
static const unsigned char * volatile ap2 = abc + 2;
static const unsigned char z[] = {0, 0};
static const unsigned char * volatile zp = z;

TC_CASE(mbsdec_start_greater_than_cur) { CHECK(_mbsdec(ap2, ap) == NULL); }
TC_CASE(mbsinc_on_nul) { CHECK(_mbsinc(zp) == z + 1); }
TC_CASE(mbclen_on_nul) { CHECK_EQ_U(_mbclen(zp), 1); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(mbsdec_start_greater_than_cur); TC_RUN(mbsinc_on_nul); TC_RUN(mbclen_on_nul);
    return tc_end();
}
