/* TC
 * id:    mb_neg_mbslwr_s_l_null_locale_probe
 * area:  mbcs
 * funcs:
 * procs: _mbslwr_s_l _mbsupr_s_l
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbslwr_s_l (a NULL locale means the current locale)
 * note:  PROBE: the _l variants accept a NULL locale pointer, convert ASCII in the C/default MBCS state, and still
 *        reject a NULL string with nonzero size via EINVAL.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
typedef int (__cdecl *f_t)(unsigned char *, size_t, void *);
static f_t p_lwr, p_upr;
static unsigned char * volatile ns = NULL;
TC_CASE(null_locale_converts)
{
    unsigned char a[4] = {'A','b',0}, b[4] = {'a','B',0};
    CHECK(p_lwr && p_upr); if (!p_lwr || !p_upr) return;
    CHECK_EQ_I(p_lwr(a, 4, NULL), 0); CHECK_STR((char *)a, "ab");
    CHECK_EQ_I(p_upr(b, 4, NULL), 0); CHECK_STR((char *)b, "AB");
}
TC_CASE(null_string_einval)
{
    CHECK(p_lwr && p_upr); if (!p_lwr || !p_upr) return;
    errno = 0; tc_iph_count = 0;
    CHECK_EQ_I(p_lwr(ns, 4, NULL), EINVAL); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(p_upr(ns, 4, NULL), EINVAL);
    CHECK(tc_iph_count >= 2);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_lwr = (f_t)tc_crt_proc("_mbslwr_s_l"); p_upr = (f_t)tc_crt_proc("_mbsupr_s_l");
    TC_RUN(null_locale_converts); TC_RUN(null_string_einval);
    return tc_end();
}
