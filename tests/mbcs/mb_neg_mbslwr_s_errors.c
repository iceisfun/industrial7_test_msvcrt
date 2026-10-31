/* TC
 * id:    mb_neg_mbslwr_s_errors
 * area:  mbcs
 * funcs:
 * procs: _mbslwr_s _mbsupr_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strlwr_s, _strupr_s, _mbslwr_s, _mbsupr_s (str NULL with size > 0 / not terminated -> EINVAL)
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
typedef int (__cdecl *f_t)(unsigned char *, size_t);
static f_t p_lwr, p_upr;
static unsigned char * volatile ns = NULL;
#define BEGIN do { errno = 0; tc_iph_count = 0; } while (0)
#define EXPECT do { CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); } while (0)

TC_CASE(null_nonzero_size_einval)
{
    CHECK(p_lwr); if (!p_lwr) return;
    BEGIN; CHECK_EQ_I(p_lwr(ns, 4), EINVAL); EXPECT;
}
TC_CASE(nonnull_size_zero_einval)
{
    unsigned char s[4] = {'A','b',0};
    CHECK(p_lwr); if (!p_lwr) return;
    BEGIN; CHECK_EQ_I(p_lwr(s, 0), EINVAL); EXPECT;
    CHECK_STR((char *)s, "Ab");
}
TC_CASE(unterminated_einval)
{
    unsigned char s[2] = {'A','B'};
    CHECK(p_lwr); if (!p_lwr) return;
    BEGIN; CHECK_EQ_I(p_lwr(s, 2), EINVAL); EXPECT;
}
TC_CASE(upr_variants)
{
    unsigned char s[2] = {'a','b'}, t[4] = {'a','b',0};
    CHECK(p_upr); if (!p_upr) return;
    BEGIN; CHECK_EQ_I(p_upr(ns, 4), EINVAL); EXPECT;
    BEGIN; CHECK_EQ_I(p_upr(t, 0), EINVAL); EXPECT; CHECK_STR((char *)t, "ab");
    BEGIN; CHECK_EQ_I(p_upr(s, 2), EINVAL); EXPECT;
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_lwr = (f_t)tc_crt_proc("_mbslwr_s"); p_upr = (f_t)tc_crt_proc("_mbsupr_s");
    TC_RUN(null_nonzero_size_einval); TC_RUN(nonnull_size_zero_einval);
    TC_RUN(unterminated_einval); TC_RUN(upr_variants);
    return tc_end();
}
