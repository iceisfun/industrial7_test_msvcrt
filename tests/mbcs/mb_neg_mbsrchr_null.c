/* TC
 * id:    mb_neg_mbsrchr_null
 * area:  mbcs
 * funcs: _mbsrchr _mbsspn _mbscspn _mbspbrk _setmbcp _getmbcp
 * procs: _mbsrchr_l
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbsrchr, _mbsspn, _mbscspn, _mbspbrk, _mbsrchr_l
 * note:  PROBE: NULL string/set -> invalid-parameter handler called, errno EINVAL; _mbsrchr/_mbspbrk return NULL, _mbsspn/_mbscspn return 0.
 *        CP932 is selected first so the MBCS (validating) code path runs rather than a single-byte strXXX delegate.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
typedef unsigned char *(__cdecl *rchr_l_t)(const unsigned char *, unsigned int, void *);
static rchr_l_t p_rchr_l;
static int old_cp;
static const unsigned char * volatile np = NULL;
static const unsigned char ab[] = "abc";
static const unsigned char * volatile ap = ab;
#define BEGIN do { errno = 0; tc_iph_count = 0; } while (0)
#define EXPECT do { CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); } while (0)

TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(rchr_null)
{
    BEGIN; CHECK(_mbsrchr(np, 'a') == NULL); EXPECT;
    if (p_rchr_l) { BEGIN; CHECK(p_rchr_l(np, 'a', NULL) == NULL); EXPECT; } else CHECK(p_rchr_l != NULL);
}
TC_CASE(spn_null_str) { BEGIN; CHECK_EQ_U(_mbsspn(np, ap), 0); EXPECT; }
TC_CASE(spn_null_set) { BEGIN; CHECK_EQ_U(_mbsspn(ap, np), 0); EXPECT; }
TC_CASE(cspn_null)
{
    BEGIN; CHECK_EQ_U(_mbscspn(np, ap), 0); EXPECT;
    BEGIN; CHECK_EQ_U(_mbscspn(ap, np), 0); EXPECT;
}
TC_CASE(pbrk_null)
{
    BEGIN; CHECK(_mbspbrk(np, ap) == NULL); EXPECT;
    BEGIN; CHECK(_mbspbrk(ap, np) == NULL); EXPECT;
}
int main(void)
{
    tc_begin(); tc_iph_install(); old_cp = _getmbcp();
    p_rchr_l = (rchr_l_t)tc_crt_proc("_mbsrchr_l");
    TC_RUN(cp); TC_RUN(rchr_null); TC_RUN(spn_null_str); TC_RUN(spn_null_set); TC_RUN(cspn_null); TC_RUN(pbrk_null);
    _setmbcp(old_cp); return tc_end();
}
