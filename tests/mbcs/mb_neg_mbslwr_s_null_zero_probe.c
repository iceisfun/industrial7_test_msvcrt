/* TC
 * id:    mb_neg_mbslwr_s_null_zero_probe
 * area:  mbcs
 * funcs:
 * procs: _mbslwr_s _mbsupr_s
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strlwr_s (str NULL, size 0)
 * note:  PROBE: str == NULL with numberOfElements == 0 is accepted by the _s case-conversion functions (returns 0, no
 *        handler call). The docs' error table could be read as EINVAL for size 0, so this is only a probe.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
typedef int (__cdecl *f_t)(unsigned char *, size_t);
static f_t p_lwr, p_upr;
static unsigned char * volatile ns = NULL;
TC_CASE(null_zero_size_ok)
{
    CHECK(p_lwr && p_upr); if (!p_lwr || !p_upr) return;
    tc_iph_count = 0;
    CHECK_EQ_I(p_lwr(ns, 0), 0);
    CHECK_EQ_I(p_upr(ns, 0), 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_lwr = (f_t)tc_crt_proc("_mbslwr_s"); p_upr = (f_t)tc_crt_proc("_mbsupr_s");
    TC_RUN(null_zero_size_ok);
    return tc_end();
}
