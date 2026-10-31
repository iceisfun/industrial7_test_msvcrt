/* TC
 * id:    mb_neg_mbslen_null
 * area:  mbcs
 * funcs: _mbslen _setmbcp _getmbcp
 * procs: _mbslen_l _mbstrlen_l
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbslen, _mbslen_l, _mbstrlen_l (parameter validation, errno EINVAL)
 * note:  PROBE: _mbstrlen_l(NULL) -> invalid-parameter handler called (count>=1), errno == EINVAL, and the returned length is
 *        either 0 or (size_t)-1 (the exact value is not asserted). _mbslen(NULL) is deliberately NOT called: its docs do not
 *        define NULL handling and it may dereference NULL (crash). CP932 is selected so the MBCS (validating) path is used.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
typedef size_t (__cdecl *len_l_t)(const unsigned char *, void *);
static len_l_t p_mbslen_l, p_mbstrlen_l;
static const unsigned char * volatile np = NULL;
static unsigned char abc_[] = "abc";
static const unsigned char * volatile abc = abc_;
static int old_cp;
static int ok_ret(size_t r) { return r == 0 || r == (size_t)-1; }

TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(mbstrlen_l_null)
{
    size_t r;
    CHECK(p_mbstrlen_l != NULL); if (!p_mbstrlen_l) return;
    errno = 0; tc_iph_count = 0;
    r = p_mbstrlen_l(np, NULL);
    CHECK(ok_ret(r));
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(valid_after_null_unaffected)
{
    tc_iph_count = 0;
    CHECK_EQ_U(_mbslen(abc), 3);
    if (p_mbslen_l) CHECK_EQ_U(p_mbslen_l(abc, NULL), 3);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install(); old_cp = _getmbcp();
    p_mbslen_l = (len_l_t)tc_crt_proc("_mbslen_l");
    p_mbstrlen_l = (len_l_t)tc_crt_proc("_mbstrlen_l");
    TC_RUN(cp); TC_RUN(mbstrlen_l_null); TC_RUN(valid_after_null_unaffected);
    _setmbcp(old_cp); return tc_end();
}
