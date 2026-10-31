/* TC
 * id:    mb_neg_mbsinc_null
 * area:  mbcs
 * funcs: _mbsinc _mbsdec _mbsnextc _mbclen _setmbcp _getmbcp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbsinc, _mbsdec, _mbsnextc, _mbclen
 * note:  PROBE: NULL pointer arguments call the invalid-parameter handler, set errno EINVAL, and return NULL (pointer functions)
 *        CP932 is selected first so the MBCS (validating) code path runs rather than a single-byte strXXX delegate.
 *        or 0 (_mbsnextc, _mbclen).
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static const unsigned char * volatile np = NULL;
static const unsigned char abc[] = "abc";
static const unsigned char * volatile ap = abc;
#define BEGIN do { errno = 0; tc_iph_count = 0; } while (0)
#define EXPECT_ERR do { CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); } while (0)

TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(mbsinc_null) { BEGIN; CHECK(_mbsinc(np) == NULL); EXPECT_ERR; }
TC_CASE(mbsdec_null_start) { BEGIN; CHECK(_mbsdec(np, ap) == NULL); EXPECT_ERR; }
TC_CASE(mbsdec_null_cur) { BEGIN; CHECK(_mbsdec(ap, np) == NULL); EXPECT_ERR; }
TC_CASE(mbsnextc_null) { BEGIN; CHECK_EQ_U(_mbsnextc(np), 0); EXPECT_ERR; }
TC_CASE(mbclen_null) { BEGIN; CHECK_EQ_U(_mbclen(np), 0); EXPECT_ERR; }
int main(void)
{
    tc_begin(); tc_iph_install(); old_cp = _getmbcp();
    TC_RUN(cp); TC_RUN(mbsinc_null); TC_RUN(mbsdec_null_start); TC_RUN(mbsdec_null_cur); TC_RUN(mbsnextc_null); TC_RUN(mbclen_null);
    _setmbcp(old_cp); return tc_end();
}
