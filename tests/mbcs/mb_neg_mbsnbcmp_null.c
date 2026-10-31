/* TC
 * id:    mb_neg_mbsnbcmp_null
 * area:  mbcs
 * funcs: _mbsnbcmp _mbsnbicmp _mbsncmp _mbsnicmp _setmbcp _getmbcp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbsnbcmp, _mbsnbicmp, _mbsncmp, _mbsnicmp (return _NLSCMPERROR on error)
 * note:  PROBE: NULL string with count>0 -> _NLSCMPERROR (0x7fffffff), errno EINVAL, handler called.
 *        CP932 is selected first so the MBCS (validating) code path runs rather than a single-byte strXXX delegate.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
#define NLSERR 0x7fffffff
static int old_cp;
static const unsigned char * volatile np = NULL;
static const unsigned char a[] = "abc";
static const unsigned char * volatile ap = a;
#define BEGIN do { errno = 0; tc_iph_count = 0; } while (0)
#define EXPECT_ERR do { CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); } while (0)

TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(nbcmp_null_first) { BEGIN; CHECK_EQ_I(_mbsnbcmp(np, ap, 2), NLSERR); EXPECT_ERR; }
TC_CASE(nbcmp_null_second) { BEGIN; CHECK_EQ_I(_mbsnbcmp(ap, np, 2), NLSERR); EXPECT_ERR; }
TC_CASE(nbicmp_null) { BEGIN; CHECK_EQ_I(_mbsnbicmp(np, ap, 2), NLSERR); EXPECT_ERR; }
TC_CASE(ncmp_null) { BEGIN; CHECK_EQ_I(_mbsncmp(np, ap, 2), NLSERR); EXPECT_ERR; }
TC_CASE(nicmp_null) { BEGIN; CHECK_EQ_I(_mbsnicmp(ap, np, 2), NLSERR); EXPECT_ERR; }
int main(void)
{
    tc_begin(); tc_iph_install(); old_cp = _getmbcp();
    TC_RUN(cp); TC_RUN(nbcmp_null_first); TC_RUN(nbcmp_null_second); TC_RUN(nbicmp_null); TC_RUN(ncmp_null); TC_RUN(nicmp_null);
    _setmbcp(old_cp); return tc_end();
}
