/* TC
 * id:    mb_neg_mbslen_null
 * area:  mbcs
 * funcs: _mbslen _setmbcp _getmbcp
 * procs: _mbslen_l _mbstrlen_l
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _mbslen, _mbslen_l, _mbstrlen_l (parameter validation, errno EINVAL)
 * note:  PROBE: _mbstrlen_l(NULL) -> invalid-parameter handler called (count>=1), errno == EINVAL, and the returned length is
 *        either 0 or (size_t)-1 (the exact value is not asserted). _mbslen(NULL) is deliberately NOT called: its docs do not
 *        define NULL handling and it may dereference NULL (crash). The original intent (MBCS path) is covered by the mb_locale case.
 *        Observed Win10 22H2 ucrtbase: _mbstrlen_l(NULL, NULL) is NOT validated and faults with 0xC0000005 (also with the default or an
 *        installed handler) in the C locale (strlen path); _setmbcp(932) does NOT select the multibyte path -- class C: the call runs in a
 *        child and the fault is asserted. Under setlocale(LC_ALL, ".932") (multibyte path) _mbstrlen_l(NULL, NULL) does not fault: it
 *        returns (size_t)-1, errno == EILSEQ, handler NOT called (observed, case mbstrlen_l_null_mb_locale). Stable.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
#include <locale.h>
typedef size_t (__cdecl *len_l_t)(const unsigned char *, void *);
static len_l_t p_mbslen_l, p_mbstrlen_l;
static const unsigned char * volatile np = NULL;
static unsigned char abc_[] = "abc";
static const unsigned char * volatile abc = abc_;
static int old_cp;

TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(mbstrlen_l_null_access_violation)
{
    DWORD code = 0; int ran = tc_spawn_self("mbstrlen_l", &code, 20000);
    CHECK(p_mbstrlen_l != NULL);
    CHECK(ran); CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);   /* observed: no NULL validation */
}
TC_CASE(mbstrlen_l_null_mb_locale)
{
    DWORD code = 0; int ran = tc_spawn_self("mbstrlen_l_mb", &code, 20000);
    CHECK(p_mbstrlen_l != NULL);
    CHECK(ran); CHECK_EQ_U(code, 0x5A);   /* child: r==(size_t)-1, errno==EILSEQ, tc_iph_count==0 */
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
    const char *child;
    tc_begin(); tc_iph_install(); old_cp = _getmbcp();
    p_mbslen_l = (len_l_t)tc_crt_proc("_mbslen_l");
    p_mbstrlen_l = (len_l_t)tc_crt_proc("_mbstrlen_l");
    if ((child = tc_child_name()) != NULL) {
        _setmbcp(932);
        if (p_mbstrlen_l && tc_strequ(child, "mbstrlen_l")) (void)p_mbstrlen_l(np, NULL);
        if (p_mbstrlen_l && tc_strequ(child, "mbstrlen_l_mb")) {
            size_t r; if (!setlocale(LC_ALL, ".932")) return 90;
            tc_iph_count = 0; errno = 0; r = p_mbstrlen_l(np, NULL);
            return (r == (size_t)-1 && errno == EILSEQ && tc_iph_count == 0) ? 0x5A : 0x5B;
        }
        return 0;   /* only reached when the call did not fault */
    }
    TC_RUN(cp); TC_RUN(mbstrlen_l_null_access_violation); TC_RUN(mbstrlen_l_null_mb_locale); TC_RUN(valid_after_null_unaffected);
    _setmbcp(old_cp); return tc_end();
}
